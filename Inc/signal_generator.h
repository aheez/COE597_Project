#ifndef __SIG_GEN_H
#define __SIG_GEN_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265359f
#define TWOPI 6.28318530718f
#define SAMPLE_FREQUENCY 44100U
#define Ts 0.00002267573f

#define DEG2RAD(x) (x * PI / 180)

typedef enum Waveform{
	SINE,
	SQUARE,
	TRIANGLE,
	SAWTOOTH
}Waveform;

typedef struct Oscillator{
	Waveform type;
	float frequency;
	float phase;
	float amplitude;
}Oscillator;


void GenerateWaveform(uint32_t  buff_size, float* buff, Oscillator osc);
void Sine(uint32_t buff_size, float* buff, Oscillator osc);
void Square(uint32_t buff_size, float* buff, Oscillator osc);
void Triangle(uint32_t buff_size, float* buff, Oscillator osc);
void Sawtooth(uint32_t buff_size, float* buff, Oscillator osc);
void ClearBuffer(uint32_t buff_size, float* buff);
void PrintBuffer(uint32_t buff_size, float* buff);
void AddedNoise (uint32_t buff_size, float* buff);

#endif
