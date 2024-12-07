#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <complex.h>
#include <lame/lame.h>

#define SAMPLE_RATE 48000
#define CHANNELS 2
#define BUFFER_SIZE 1024

#define BAND_LOW 20
#define BAND_1_MAX 60
#define BAND_2_MAX 250
#define BAND_3_MAX 500
#define BAND_4_MAX 2000
#define BAND_5_MAX 6000
#define BAND_6_MAX 20000

// Function declarations
void fft(double complex *input, double complex *output, int n);
void ifft(double complex *input, double complex *output, int n);
void split_into_bands(double complex *fft_data, double complex *bands[6], int n);
void process_audio();

// Helper functions to ensure power of two for FFT size
int is_power_of_two(int n);
int next_power_of_two(int n);
void prepare_fft_input(short *input, double complex *output, int n);

#endif // AUDIO_PROCESSING_H
