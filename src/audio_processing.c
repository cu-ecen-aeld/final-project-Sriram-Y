#include "audio_processing.h"
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define BUFFER_SIZE 1024

void print_waveform(short *buffer, int frames)
{
    for (int i = 0; i < frames; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

void process_audio()
{
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    short *buffer;
    snd_pcm_uframes_t frames;
    int err;
    int sample_rate = SAMPLE_RATE;

    // Open the default PCM device for capturing audio (input)
    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to open PCM device: %s\n", snd_strerror(err));
        exit(1);
    }

    // Allocate memory for ALSA hardware parameters
    snd_pcm_hw_params_malloc(&params);
    if ((err = snd_pcm_hw_params_any(handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to initialize hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    // Set the hardware parameters (16-bit signed little-endian format, 2 channels, and sample rate)
    if ((err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        fprintf(stderr, "Error: unable to set access type: %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        fprintf(stderr, "Error: unable to set sample format: %s\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_channels(handle, params, CHANNELS)) < 0)
    {
        fprintf(stderr, "Error: unable to set channels: %s\n", snd_strerror(err));
        exit(1);
    }

    // Pass a pointer to the sample_rate variable
    if ((err = snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to set sample rate: %s\n", snd_strerror(err));
        exit(1);
    }

    // Apply hardware parameters
    if ((err = snd_pcm_hw_params(handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to set hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    // allocate the buffer audio data
    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    buffer = (short *)malloc(frames * sizeof(short) * CHANNELS);

    // keep capturing the audio
    while (1)
    {
        err = snd_pcm_readi(handle, buffer, frames);
        if (err == -EPIPE)
        {
            fprintf(stderr, "Error: buffer overrun occurred\n");
            snd_pcm_prepare(handle);
        }
        else if (err < 0)
        {
            fprintf(stderr, "Error: failed to read audio data: %s\n", snd_strerror(err));
        }
        else if (err != frames)
        {
            fprintf(stderr, "Error: short read, read %d frames instead of %d\n", err, frames);
        }

        print_waveform(buffer, frames);
    }

    // Clean up
    free(buffer);
    snd_pcm_close(handle);
}
