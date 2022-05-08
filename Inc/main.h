#ifndef __MAIN_FILE__
#define __MAIN_FILE__

#include<math.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_font.h"
#include "key_osc_map.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CENTER_X SCREEN_WIDTH/2.0f
#define CENTER_Y SCREEN_HEIGHT/2.0f

#define BUFFER_SIZE (1024U)
#define MAX_OSCILLATORS (4U)		// Maximum number of oscillators active at once

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

typedef struct mainBufferStruct {
	float * buffer1;
	float * buffer2;
	uint8_t index;
}mainBufferStruct;

#endif
// EOF