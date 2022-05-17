#include "signal_generator.h"

static uint64_t index = 0;

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

}

void Sine(uint32_t buff_size, float* buff, Oscillator osc) {
	for(int i = 0; i<buff_size; i++) {
		buff[i] = - osc.amplitude * sinf(index*Ts*TWOPI*osc.frequency+DEG2RAD(osc.phase));
		index++;
	}
}

void Square(uint32_t buff_size, float* buff, Oscillator osc) {
	for(int i = 0; i<buff_size; i++) {
		buff[i] = (sinf(index*Ts*TWOPI*osc.frequency+ DEG2RAD(osc.phase)) >= 0.0f) ? 1.0f * osc.amplitude : -1.0f * osc.amplitude;
		index++;
	}
}

void Triangle(uint32_t buff_size, float* buff, Oscillator osc) {
	float p = 1.0f / osc.frequency;
	float shift = (1.0f/(4.0f*osc.frequency)) + (DEG2RAD(osc.phase)/TWOPI*osc.frequency);
	for(int i = 0; i<buff_size; i++) {
		buff[i] = osc.amplitude * (2.0f * abs(2.0f * ((Ts * index + shift) / p) - floor((Ts * index + shift) / p + 0.5)) - 1);
		index++;
	}
}

void Sawtooth(uint32_t buff_size, float* buff, Oscillator osc) {
	for(int i= 0; i<buff_size; i++) {
		buff[i] = osc.amplitude * (2.0f * (fmodf(Ts*index*osc.frequency, 1.0f) - 0.5f));
		index++;
	}
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