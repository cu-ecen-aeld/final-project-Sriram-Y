#include "audio_processing.h"

short band_1[BUFFER_SIZE];
short band_2[BUFFER_SIZE];
short band_3[BUFFER_SIZE];
short band_4[BUFFER_SIZE];
short band_5[BUFFER_SIZE];
short band_6[BUFFER_SIZE];

void modify_band(int band_number)
{
    // TODO: Modify each band here and store the modified band back in its respective buffer
    short *band_buffer;
    float gain = 1.0;
    float distortion = 0.0; // Distort amount

    // Select the appropriate buffer based on band number
    switch (band_number)
    {
    case 0:
        band_buffer = band_1;
        break;
    case 1:
        band_buffer = band_2;
        break;
    case 2:
        band_buffer = band_3;
        break;
    case 3:
        band_buffer = band_4;
        break;
    case 4:
        band_buffer = band_5;
        break;
    case 5:
        band_buffer = band_6;
        break;
    default:
        band_buffer = band_1;
        break; // Invalid band number
    }

    switch (band_number)
    {
    case 0:         // low frequencies
        gain = 1.2; // Boost bass by a small amount
        break;
    case 1: // Low to mid frequencies
        gain = 1.1;
        break;
    case 2: // mid frequencies
        gain = 1.0;
        break;
    case 3:               // mid to upper frequencies frequencies
        gain = 0.9;       // Reduce slightly
        distortion = 0.1; // Add slight distortion
        break;
    case 4: // upper frequencies
        gain = 0.8;
        break;
    case 5: // high frequencies
        gain = 0.7;
        break;
    }

    // Process each sample in the buffer
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        float sample = band_buffer[i];

        sample *= gain; // amplify/reduce gain value

        if (distortion > 0.0)
        { // apply distortion if needed
            sample = tanh(sample * (1.0 + distortion * 3.0)) / (1.0 + distortion);
        }

        // prevent overflow for the edited samples
        if (sample > 32767)
            sample = 32767;
        if (sample < -32767)
            sample = -32767;

        // save back into buffer
        band_buffer[i] = (short)sample;
    }
}

// Function to process FFT and reconstruct audio
void process_audio_bands(short *buffer, int frames)
{
    int N = frames * CHANNELS; // Total number of samples
    double *in = fftw_malloc(sizeof(double) * N);
    fftw_complex *out = fftw_malloc(sizeof(fftw_complex) * (N / 2 + 1));
    fftw_plan plan_forward = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
    fftw_plan plan_inverse = fftw_plan_dft_c2r_1d(N, out, in, FFTW_ESTIMATE);

    // Copy audio data into the FFT input buffer
    for (int i = 0; i < N; i++)
    {
        in[i] = (double)buffer[i];
    }

    // Perform FFT
    fftw_execute(plan_forward);

    // Frequency band definitions
    int band_limits[] = {BAND_1_MAX, BAND_2_MAX, BAND_3_MAX, BAND_4_MAX, BAND_5_MAX, BAND_6_MAX};
    int band_count = 6;
    double bin_size = (double)SAMPLE_RATE / N;

    // Process each band and store the result in the global arrays
    for (int band = 0; band < band_count; band++)
    {
        int lower_bin = (band == 0) ? 0 : (int)(band_limits[band - 1] / bin_size);
        int upper_bin = (int)(band_limits[band] / bin_size);

        // Zero out frequencies outside this band
        for (int i = 0; i < N / 2 + 1; i++)
        {
            if (i < lower_bin || i > upper_bin)
            {
                out[i][0] = 0.0;
                out[i][1] = 0.0;
            }
        }

        fftw_execute(plan_inverse);

        // Normalize and store the result in the corresponding global array
        short *band_buffer = NULL;
        switch (band)
        {
        case 0:
            band_buffer = band_1;
            break;
        case 1:
            band_buffer = band_2;
            break;
        case 2:
            band_buffer = band_3;
            break;
        case 3:
            band_buffer = band_4;
            break;
        case 4:
            band_buffer = band_5;
            break;
        case 5:
            band_buffer = band_6;
            break;
        }

        // Store the processed audio band into its corresponding global array
        for (int i = 0; i < frames; i++)
        {
            band_buffer[i] = (short)(in[i] / N); // Normalize and store
        }
    }

    // Cleanup FFTW resources
    fftw_destroy_plan(plan_forward);
    fftw_destroy_plan(plan_inverse);
    fftw_free(in);
    fftw_free(out);
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

    // Allocate the buffer for audio data
    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    buffer = (short *)malloc(frames * sizeof(short) * CHANNELS);

    // Keep capturing the audio
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

        process_audio_bands(buffer, frames);

        // TODO: Here is where you will call the modify_bands function
        for (int i = 0; i < 6; i++)
        {
            modify_band(i);
        }
    }

    // Clean up
    free(buffer);
    snd_pcm_close(handle);
}
