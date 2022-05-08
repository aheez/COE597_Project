#include "signal_generator.h"


void GenerateWaveform(uint32_t buff_size, float* buff, Oscillator osc) {

	switch (osc.type)
	{
	case SINE:
		Sine(buff_size, buff, osc);
		break;
	case SQUARE:
		Square(buff_size, buff, osc);
		break;
	case TRIANGLE:
		Triangle(buff_size, buff, osc);
		break;
	case SAWTOOTH:
		Sawtooth(buff_size, buff, osc);
		break;
	default:
		break;
	}

	// AddedNoise(buff_size, buff);
}

void Sine(uint32_t buff_size, float* buff, Oscillator osc) {
	for(int i = 0; i<buff_size; i++)
		buff[i] = - osc.amplitude * sinf(i*Ts*TWOPI*osc.frequency+DEG2RAD(osc.phase));
}

void Square(uint32_t buff_size, float* buff, Oscillator osc) {
	for(int i = 0; i<buff_size; i++)
		buff[i] = (sinf(i*Ts*TWOPI*osc.frequency+ DEG2RAD(osc.phase)) >= 0.0f) ? 1.0f * osc.amplitude : -1.0f * osc.amplitude;
}

void Triangle(uint32_t buff_size, float* buff, Oscillator osc) {
	float p = 1.0f / osc.frequency;
	float shift = (1.0f/(4.0f*osc.frequency)) + (DEG2RAD(osc.phase)/TWOPI*osc.frequency);
	for(int i = 0; i<buff_size; i++)
		buff[i] = osc.amplitude * (2.0f * abs(2.0f * ((Ts * i + shift) / p) - floor((Ts * i + shift) / p + 0.5)) - 1);
}

void Sawtooth(uint32_t buff_size, float* buff, Oscillator osc) {
	for(int i= 0; i<buff_size; i++)
		buff[i] = osc.amplitude * (2.0f * (fmodf(Ts*i*osc.frequency, 1.0f) - 0.5f));
}

void ClearBuffer(uint32_t buff_size, float* buff) {
	for(int i = 0; i<buff_size; i++)
		buff[i] = 0;
}

void PrintBuffer(uint32_t buff_size, float* buff) {
	printf("\n[");
	for (int i=0; i<buff_size; i++)
		printf("%f, ", buff[i]);
	printf("]\n");
}

void AddedNoise (uint32_t buff_size, float* buff) {
	for(int i = 0; i<buff_size; i++)
		buff[i] += (rand() % 100) / 10000.0f;
}