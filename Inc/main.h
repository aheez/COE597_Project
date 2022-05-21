#ifndef __MAIN_FILE__
#define __MAIN_FILE__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_font.h"
#include "key_osc_map.h"
#include "signal_generator.h"

#define SCREEN_WIDTH ( 1024U )
#define SCREEN_HEIGHT ( 576U )

#define CENTER_X ( SCREEN_WIDTH/2.0f )
#define CENTER_Y ( SCREEN_HEIGHT/2.0f )

#define BUFFER_SIZE ( 1024U )
#define MAX_OSCILLATORS ( 4U )		// Maximum number of oscillators active at once

/**
 * @brief Structure for an Audio Signal
 * 
 * An Audio Signal is a combination of an oscillator and a buffer.
 * 		  	
 */	
typedef struct audioSignal {
	Oscillator * osc;
	float * buffer;
	uint8_t status;
}audioSignal;

/*	
	main buffer structure with 2 segments since double buffering is needed for audio processing
*/
typedef struct mainBufferStruct {
	float * buffer1;
	float * buffer2;
	uint8_t index;
}mainBufferStruct;

/*	
	Functions prototypes
*/
static void must_init(bool test, const char * msg);
static bool init_oscillators();
static bool init_active_signals();
static bool init_main_buffer();
static void update_main_buffer(ALLEGRO_AUDIO_STREAM * stream);
static void key_down(int keycode);
static void clear_active_signals();
static void HandleAudio();
static void plot_axis();
static void plot_waveform();
#endif
// EOF