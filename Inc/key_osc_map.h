#ifndef _KEY_OSC_MAP_H_
#define _KEY_OSC_MAP_H_

#include "allegro5/keycodes.h"
#include "signal_generator.h"

/**
 * @brief The KeyOscMap Lookup Table Structure
 */
struct KeyOscMap {
	int8_t key;
	Waveform type;
	float freq;
};

/* 
  A3: 220Hz
  A3#: 233Hz
  B3: 247Hz
  C4: 262Hz
  C4#: 277Hz
  D4: 294Hz
  D4#: 311Hz
  E4: 330Hz
  F4: 349Hz
  F4#: 370Hz
  G4: 392Hz
  G4#: 415Hz
  A4: 440Hz
*/


/**
 * @brief The KeyOscMap Lookup Table
 */
struct KeyOscMap osc_conf[] = {
	// Sine
	{ALLEGRO_KEY_A, SINE, 220.0f},
	{ALLEGRO_KEY_S, SINE, 233.0f},
	{ALLEGRO_KEY_D, SINE, 247.0f},
	{ALLEGRO_KEY_F, SINE, 262.0f},
	{ALLEGRO_KEY_G, SINE, 277.0f},
	{ALLEGRO_KEY_H, SINE, 294.0f},
	{ALLEGRO_KEY_J, SINE, 311.0f},
	{ALLEGRO_KEY_K, SINE, 330.0f},
	{ALLEGRO_KEY_L, SINE, 349.0f},
	{ALLEGRO_KEY_SEMICOLON, SINE, 370.0f},
	{ALLEGRO_KEY_QUOTE, SINE, 392.0f},

	// Square
	{ALLEGRO_KEY_Z, SQUARE, 220.0f},
	{ALLEGRO_KEY_X, SQUARE, 233.0f},
	{ALLEGRO_KEY_C, SQUARE, 247.0f},
	{ALLEGRO_KEY_V, SQUARE, 262.0f},
	{ALLEGRO_KEY_B, SQUARE, 277.0f},
	{ALLEGRO_KEY_N, SQUARE, 294.0f},
	{ALLEGRO_KEY_M, SQUARE, 311.0f},
	{ALLEGRO_KEY_COMMA, SQUARE, 330.0f},
	{ALLEGRO_KEY_FULLSTOP, SQUARE, 349.0f},
	{ALLEGRO_KEY_SLASH, SQUARE, 370.0f},

	// Sawtooth
	{ALLEGRO_KEY_Q, SAWTOOTH, 220.0f},
	{ALLEGRO_KEY_W, SAWTOOTH, 233.0f},
	{ALLEGRO_KEY_E, SAWTOOTH, 247.0f},
	{ALLEGRO_KEY_R, SAWTOOTH, 262.0f},
	{ALLEGRO_KEY_T, SAWTOOTH, 277.0f},
	{ALLEGRO_KEY_Y, SAWTOOTH, 294.0f},
	{ALLEGRO_KEY_U, SAWTOOTH, 311.0f},
	{ALLEGRO_KEY_I, SAWTOOTH, 330.0f},
	{ALLEGRO_KEY_O, SAWTOOTH, 349.0f},
	{ALLEGRO_KEY_P, SAWTOOTH, 370.0f}
};


#endif