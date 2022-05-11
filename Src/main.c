#include "main.h"

const char * waveforms [] = {
	"SINE",
	"SQUARE",
	"TRIANGLE",
	"SAWTOOTH"
};

ALLEGRO_DISPLAY * display;

static int no_actSig = 0;					// Number of active signals
static int no_osc = 0;						// Number of oscillators
static Oscillator * oscillators = NULL;		// Array of all oscillators
static audioSignal * active_signals = NULL;	// Array of active signals
static struct mainBufferStruct mainBuffer;		// Main buffer
static bool mainThread = false;				// Main thread flag

static void must_init(bool test, const char * msg) {
	
	if (!test) {
		fprintf(stderr, "Error: Could not initialize %s\n", msg);
		exit(1);
	}

}

// Generate Oscillator list
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

// Generate Active audioSignal list
static bool init_active_signals() {
	
	active_signals = malloc(MAX_OSCILLATORS * sizeof(audioSignal));
	for (int i = 0; i < MAX_OSCILLATORS; i++) {
		active_signals[i].osc = &oscillators[i];
		active_signals[i].buffer = malloc(BUFFER_SIZE * sizeof(float));
		active_signals[i].status = 0;
	}
	return true;
}

static bool init_main_buffer() {
	mainBuffer.index = 0;
	return true;
}


/**
 * @brief Update the main buffer (mix all active signals)
 */
static void update_main_buffer(ALLEGRO_AUDIO_STREAM * stream) {

	printf("Updating main buffer\nNo of active signals: %d\n", no_actSig);

	al_set_audio_stream_playing(stream, false);
	float *temp = malloc(BUFFER_SIZE * sizeof(float));
	ClearBuffer(BUFFER_SIZE, temp);
	for (int i = 0; i < BUFFER_SIZE; i++) 
		for (int j = 0; j < no_actSig; j++) {
			if (active_signals[j].status == 1) {
				temp[i] += active_signals[j].buffer[i] / no_actSig;
			}
		}
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
	al_set_audio_stream_playing(stream, true);
}

static void key_down(int keycode) {
	printf("Key pressed: %s\n", al_keycode_to_name(keycode));
	if (no_actSig == MAX_OSCILLATORS) {
		fprintf(stderr, "Error: Maximum number of active signals reached\n");
		return;
	}
	int index = 0;
	no_actSig++;
	for (int i = 0; i < no_osc; i++) {
		if (osc_conf[i].key == keycode) {
			index = i;
			break;
		}
	}
	active_signals[no_actSig - 1].status = 1;
	active_signals[no_actSig - 1].osc = &oscillators[index];
	GenerateWaveform(BUFFER_SIZE, active_signals[no_actSig - 1].buffer, * active_signals[no_actSig - 1].osc);
}

static void clear_active_signals() {

	printf("Clearing active signals\n");

	for (int i = 0; i < MAX_OSCILLATORS; i++) {
		active_signals[i].status = 0;
		no_actSig = 0;
	}
	no_actSig = 0;
}


static void HandleAudio() {

	printf("Main thread running\n");

	ALLEGRO_TIMER * timer = al_create_timer(1.0 / 100);
	ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
	ALLEGRO_EVENT event;
	ALLEGRO_KEYBOARD_STATE kst;
	ALLEGRO_AUDIO_STREAM * stream = al_create_audio_stream	(2, 
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

int main(int argc, char ** argv) {
	
	must_init(al_init(), "allegro");
	must_init(al_install_audio(), "audio");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_init_primitives_addon(), "primitives");
	must_init(init_main_buffer(), "main buffer");
	must_init(init_oscillators(), "oscillators");
	must_init(init_active_signals(), "active signals");
	ALLEGRO_THREAD * audioThread = al_create_thread(HandleAudio, NULL);
	must_init(audioThread, "audio thread");
	
	ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	must_init(display, "display");

	al_register_event_source(queue, al_get_display_event_source(display));
	// al_register_event_source(queue, al_get_keyboard_event_source());

	ALLEGRO_EVENT event;

	plot_axis();

	mainThread = true;
	al_start_thread(audioThread);

	while (1) {

		al_wait_for_event(queue, &event);

		switch (event.type) {

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			goto finish;
			break;
		
		case ALLEGRO_EVENT_KEY_DOWN:
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			event.keyboard.keycode = 0;
				goto finish;
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
	free(oscillators);
	free(active_signals);
	printf("Finished\n");
	return 0;
}

/* static ALLEGRO_DISPLAY *display1;

static void redraw(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2) {

	al_set_target_backbuffer(display1);
	al_clear_to_color(color1);
	al_flip_display();

}

int main(int argc, char **argv){
	printf("Version: %s\n",ALLEGRO_VERSION_STR);

	// Initialize allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	// initialize the keyboard
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}
	// intialize the mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}
	// Initialize the display
	display1 = al_create_display(640, 480);
	if(!display1) {
		al_destroy_display(display1);
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);

	// mouse state
	ALLEGRO_MOUSE_STATE mst0;
	ALLEGRO_MOUSE_STATE mst;
	// keyboard state
	ALLEGRO_KEYBOARD_STATE kst;

	memset(&mst0, 0, sizeof(mst0));

	while (1)
	{
		al_get_mouse_state(&mst);

		if (mst.display != mst0.display || mst.x != mst0.x || mst.y != mst0.y)
		{
			if (mst.display == NULL)
				printf("Mouse is not on display\n");
			else if (mst.display == display1)
				printf("Mouse is on display\n");
			else
				printf("Uknown Display");
			
			mst0 = mst;
		}
		
		if (mst.display == display1)
		{
			redraw(red, black);
		}
		else
		{
			redraw(white, black);
		}

		al_rest(0.1);

		al_get_keyboard_state(&kst);
		if (al_key_down(&kst, ALLEGRO_KEY_ESCAPE))
			break;
	}

	// allegro distroy and exit
	al_destroy_display(display1);
	al_uninstall_mouse();
	al_uninstall_keyboard();

	return 0;
} */