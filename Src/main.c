#include "main.h" // A header file that have all the includes and relavent structs

const char * waveforms [] = {
	"SINE",
	"SQUARE",
	"TRIANGLE",
	"SAWTOOTH"
};

static int						no_actSig			= 0;		// Number of active signals
static int						no_osc 				= 0;		// Number of oscillators
static Oscillator				* oscillators 		= NULL;		// Array of all oscillators
static audioSignal				* active_signals 	= NULL;		// Array of active signals
static struct mainBufferStruct	mainBuffer;						// Main buffer
static bool						mainThread 			= false;	// Main thread flag
ALLEGRO_DISPLAY					* display			= NULL;		// Display object
ALLEGRO_MUTEX					* mutex 			= NULL;		// Define Mutex

int main(int argc, char ** argv) {

	/*
		Initiat all the needed functions, libraries and variables for the program.
		And handle the display functions of the program
	*/
	must_init(al_init(), "allegro");
	must_init(al_install_audio(), "audio");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_init_primitives_addon(), "primitives");
	must_init(init_main_buffer(), "main buffer");
	must_init(init_oscillators(), "oscillators");
	must_init(init_active_signals(), "active signals");

	// Create timer
	ALLEGRO_TIMER 		* timer = al_create_timer(1.0 / 60.0);

	// Create audio thread
	ALLEGRO_THREAD		* audioThread = al_create_thread(HandleAudio, NULL);
	must_init(audioThread, "audio thread");

	// Create  display object
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	must_init(display, "display");
	
	// queue and event object used by the display thread
	ALLEGRO_EVENT_QUEUE	* queue = al_create_event_queue();
	ALLEGRO_EVENT 		event;

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));

	// Create mutex
	mutex = al_create_mutex_recursive();
	// must_init(mutex, "mutex");

	plot_axis();

	mainThread = true;
	al_start_thread(audioThread);
	al_start_timer(timer);

	// Display thread
	while (1) {

		al_wait_for_event(queue, &event);

		switch (event.type) {

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			goto finish;
			break;
		
		case ALLEGRO_EVENT_TIMER:
			if (no_actSig > 0) 
		 		// plot_waveform();
			break;
		}
	}

finish:
	mainThread = false;
	al_join_thread(audioThread, NULL);
	al_uninstall_audio();
	al_uninstall_keyboard();
	al_shutdown_primitives_addon();
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_mutex(mutex);
	free(oscillators);
	free(active_signals);
	printf("Finished\n");
	return 0;
}

/*
	Initiation function
*/
static void must_init(bool test, const char * msg) {
	
	if (!test) {
		fprintf(stderr, "Error: Could not initialize %s\n", msg);
		exit(1);
	}

}

/*
	Generate Oscillator list based on the osc_conf in:
	../Inc/key_osc_map.h
*/
static bool init_oscillators() {
	
	no_osc = sizeof(osc_conf) / sizeof(osc_conf[0]);
	oscillators = malloc(no_osc * sizeof(Oscillator));
	for (int i = 0; i < no_osc; i++) {
		oscillators[i].type = osc_conf[i].type;
		oscillators[i].frequency = osc_conf[i].freq;
		oscillators[i].phase = 0.0f;
		oscillators[i].amplitude = .5f;
	}
	return true;
}

/*
	Generate Active audioSignal list
	which contains all the active signals oscillators, buffer and status flag
	for each signal
*/
static bool init_active_signals() {
	
	active_signals = malloc(MAX_OSCILLATORS * sizeof(audioSignal));
	for (int i = 0; i < MAX_OSCILLATORS; i++) {
		active_signals[i].osc = &oscillators[i];
		active_signals[i].buffer = malloc(BUFFER_SIZE * sizeof(float));
		active_signals[i].status = 0;
	}
	return true;
}


/*
	mainBuffer initiation function:
	It sets the index initial value to zero
*/
static bool init_main_buffer() {
	mainBuffer.index = 0;
	return true;
}


/*
	Update the main buffer:
	mix all active signals making sure that the maximum value is
	between [-1,1]
*/
static void update_main_buffer(ALLEGRO_AUDIO_STREAM * stream) {

	float	*temp = malloc(BUFFER_SIZE * sizeof(float));

	al_lock_mutex(mutex);

	al_set_audio_stream_playing(stream, false);

	ClearBuffer(BUFFER_SIZE, temp);					// Set all the values in the temporory to zero

	for (int i = 0; i < BUFFER_SIZE; i++) 

		for (int j = 0; j < no_actSig; j++) {

			if (active_signals[j].status == 1) 
				temp[i] += active_signals[j].buffer[i] / no_actSig;	// mix all the active signals maintaining the same ratios between thier amplitudes
			
		}

	// Select the main buffer segment to be filled
	if (mainBuffer.index == 0) {
		mainBuffer.buffer1 = al_get_audio_stream_fragment(stream);
		mainBuffer.index = 1;
		memcpy(mainBuffer.buffer1, temp, BUFFER_SIZE * sizeof(float));
		al_set_audio_stream_fragment(stream, mainBuffer.buffer1);
	} else {
		mainBuffer.buffer2 = al_get_audio_stream_fragment(stream);
		mainBuffer.index = 0;
		memcpy(mainBuffer.buffer2, temp, BUFFER_SIZE * sizeof(float));
		al_set_audio_stream_fragment(stream, mainBuffer.buffer2);
	}

	free(temp);
	al_unlock_mutex(mutex);
	al_set_audio_stream_playing(stream, true);
}

/*
	Adds the oscillator assigned to the key pressed to the 
	active signal list
*/
static void key_down(int keycode) {

	if (no_actSig == MAX_OSCILLATORS) {
		fprintf(stderr, "Error: Maximum number of active signals reached\n");
		return;
	}

	int index = 0;
	no_actSig++;
	for (index; index < no_osc; index++) {
		if (osc_conf[index].key == keycode) {
			fprintf(stdout, "Oscillator of type: %s and frequency: %.2f is added to the active signals.\n", waveforms[osc_conf[index].type], osc_conf[index].freq);
			break;
		}
	}

	if ( index == no_osc ) {
		fprintf(stderr, "Error: Key %s is not assigned to any oscillator\n", al_keycode_to_name(keycode));
		no_actSig--;
		return;
	}

	active_signals[no_actSig - 1].status = 1;
	active_signals[no_actSig - 1].osc = &oscillators[index];
	GenerateWaveform(BUFFER_SIZE, active_signals[no_actSig - 1].buffer, * active_signals[no_actSig - 1].osc);
}

/*
	Clear the active signal list
*/
static void clear_active_signals() {

	printf("Clearing active signals\n");

	for (int i = 0; i < MAX_OSCILLATORS; i++) {
		active_signals[i].status = 0;
		no_actSig = 0;
	}
	no_actSig = 0;
}

/* 
	Handle all audio processes
*/
static void HandleAudio() {

	printf("Main thread running\n");

	ALLEGRO_TIMER 			* timer = al_create_timer(1.0 / 100);
	ALLEGRO_EVENT_QUEUE 	* event_queue = al_create_event_queue();
	ALLEGRO_EVENT 			event;
	ALLEGRO_KEYBOARD_STATE	kst;
	ALLEGRO_AUDIO_STREAM	* stream = al_create_audio_stream	(2, 
																BUFFER_SIZE,
																SAMPLE_FREQUENCY,
																ALLEGRO_AUDIO_DEPTH_FLOAT32,
																ALLEGRO_CHANNEL_CONF_1);

	must_init(stream, "audio stream");
	
	al_reserve_samples(0);
	al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(stream, al_get_default_mixer());

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_audio_stream_event_source(stream));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (mainThread) {

		// printf("No of active signals: %d\n", no_actSig);
		
		al_wait_for_event(event_queue, &event);

		al_get_keyboard_state(&kst);
		if (kst.display != display)
			goto nop;

		switch (event.type) { 

			case ALLEGRO_EVENT_TIMER:
				if (no_actSig != 0)
					update_main_buffer(stream);
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				al_get_keyboard_state(&kst);
				if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) 
					clear_active_signals();
				else 
					key_down(event.keyboard.keycode);
				break;
		}
	nop:
		;
	}
	al_set_audio_stream_playing(stream, false);
	al_drain_audio_stream(stream);
	al_destroy_audio_stream(stream);
	al_destroy_event_queue(event_queue);
}

static void plot_axis() {
	al_set_target_backbuffer(display);
	ALLEGRO_COLOR green = al_map_rgba_f(0, 1, 0, .5);
	al_draw_line(0, CENTER_Y, SCREEN_WIDTH, CENTER_Y, green, 1);
	al_flip_display();
}

static void plot_waveform() {
	ALLEGRO_COLOR	red = al_map_rgba_f(1.0, 0, 0, 1.0);
	float			scale = 1.f * SCREEN_HEIGHT / 2.f;
	float			x1, x2, y1, y2;
	float			*tempBuff = al_malloc(BUFFER_SIZE * sizeof(float));

	al_lock_mutex(mutex);
	if (mainBuffer.index == 0)
		// memcpy(tempBuff, mainBuffer.buffer1, BUFFER_SIZE * sizeof(float));
		tempBuff = mainBuffer.buffer1;
	else
		// memcpy(tempBuff, mainBuffer.buffer2, BUFFER_SIZE * sizeof(float));
		tempBuff = mainBuffer.buffer2;

	for (int i = 0; i < SCREEN_WIDTH; i++) {

		x1 = i*1.f;
		x2 = x1 + 1.f;
		y1 = tempBuff[i] * scale;
		y2 = tempBuff[i + 1] * scale;
		
		al_draw_line(x1, y1, x2, y2, red, 1);
	}
	al_unlock_mutex(mutex);
	al_free(tempBuff);
	plot_axis();
}