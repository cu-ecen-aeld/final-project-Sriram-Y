#include "audio_processing.h"

// Perform FFT
void fft(short *input, double complex *output, int n)
{
    if (n == 1)
    {
        output[0] = input[0];
        return;
    }

    double complex even[n / 2];
    double complex odd[n / 2];
    for (int i = 0; i < n / 2; i++)
    {
        even[i] = input[2 * i];
        odd[i] = input[2 * i + 1];
    }

    double complex even_fft[n / 2];
    double complex odd_fft[n / 2];
    fft((short *)even, even_fft, n / 2);
    fft((short *)odd, odd_fft, n / 2);

    for (int k = 0; k < n / 2; k++)
    {
        double complex t = cexp(-2.0 * I * M_PI * k / n) * odd_fft[k];
        output[k] = even_fft[k] + t;
        output[k + n / 2] = even_fft[k] - t;
    }
}

// Perform inverse FFT
void ifft(double complex *input, double complex *output, int n)
{
    for (int i = 0; i < n; i++)
    {
        input[i] = conj(input[i]);
    }

    fft((short *)input, output, n);

    for (int i = 0; i < n; i++)
    {
        output[i] = conj(output[i]) / n;
    }
}

// Split into frequency bands
void split_into_bands(double complex *fft_data, double complex *bands[6], int n)
{
    int band_limits[6];
    band_limits[0] = (int)((BAND_1_MAX / (double)SAMPLE_RATE) * n);
    band_limits[1] = (int)((BAND_2_MAX / (double)SAMPLE_RATE) * n);
    band_limits[2] = (int)((BAND_3_MAX / (double)SAMPLE_RATE) * n);
    band_limits[3] = (int)((BAND_4_MAX / (double)SAMPLE_RATE) * n);
    band_limits[4] = (int)((BAND_5_MAX / (double)SAMPLE_RATE) * n);
    band_limits[5] = (int)((BAND_6_MAX / (double)SAMPLE_RATE) * n);

    // Loop through each band and assign corresponding frequency data
    for (int band = 0; band < 6; band++)
    {
        // Starting index for the band
        int lower_limit;
        if (band == 0)
        {
            lower_limit = 0; // For the first band, the lower limit is 0
        }
        else
        {
            lower_limit = band_limits[band - 1]; // For other bands, the lower limit is the previous band's upper limit
        }
        int upper_limit = band_limits[band]; // Ending index for the band

        // Initialize the band's frequency data to 0
        for (int i = lower_limit; i < upper_limit; i++)
        {
            bands[band][i] = fft_data[i];
        }

        // Zero out the frequencies outside the band range
        for (int i = 0; i < n; i++)
        {
            if (i < lower_limit || i >= upper_limit)
            {
                bands[band][i] = 0;
            }
        }
    }
}

void process_audio()
{
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    short *buffer;
    snd_pcm_uframes_t frames;
    int err;
    int sample_rate = SAMPLE_RATE;

    // Open PCM device for capture
    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to open PCM device: %s\n", snd_strerror(err));
        exit(1);
    }

    snd_pcm_hw_params_malloc(&params);
    if ((err = snd_pcm_hw_params_any(handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to initialize hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    // Set the hardware parameters
    if ((err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0 ||
        (err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0 ||
        (err = snd_pcm_hw_params_set_channels(handle, params, CHANNELS)) < 0 ||
        (err = snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate, 0)) < 0 ||
        (err = snd_pcm_hw_params(handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to set hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    buffer = (short *)malloc(frames * sizeof(short) * CHANNELS);

    double complex *fft_data = (double complex *)malloc(frames * sizeof(double complex));
    double complex *ifft_data = (double complex *)malloc(frames * sizeof(double complex));
    double complex *bands[6];
    for (int i = 0; i < 6; i++)
    {
        bands[i] = (double complex *)malloc(frames * sizeof(double complex));
    }

    // Initialize LAME for final MP3 encoding
    lame_t lame_encoder = lame_init();
    lame_set_in_samplerate(lame_encoder, SAMPLE_RATE);
    lame_set_num_channels(lame_encoder, CHANNELS);
    lame_set_brate(lame_encoder, 128);
    lame_init_params(lame_encoder);

    FILE *output_file = fopen("final_output.mp3", "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: unable to open output MP3 file.\n");
        exit(1);
    }

    unsigned char mp3_buffer[BUFFER_SIZE];

    while (1)
    {
        err = snd_pcm_readi(handle, buffer, frames);
        if (err == -EPIPE)
        {
            fprintf(stderr, "Error: buffer overrun occurred\n");
            snd_pcm_prepare(handle);
            continue;
        }
        else if (err < 0)
        {
            fprintf(stderr, "Error: failed to read audio data: %s\n", snd_strerror(err));
            break;
        }
        else if (err != frames)
        {
            fprintf(stderr, "Error: short read, read %d frames instead of %d\n", err, frames);
            continue;
        }

        // FFT and band processing
        fft(buffer, fft_data, frames);
        split_into_bands(fft_data, bands, frames);

        // Combine all bands into a single waveform
        for (int i = 0; i < frames; i++)
        {
            ifft_data[i] = 0;
            for (int j = 0; j < 6; j++)
            {
                ifft_data[i] += bands[j][i];
            }
        }

        // Inverse FFT to get time-domain waveform
        ifft(ifft_data, fft_data, frames);

        // Convert complex waveform to PCM format
        for (int i = 0; i < frames; i++)
        {
            buffer[i] = (short)creal(fft_data[i]);
        }

        // Encode to MP3
        int mp3_size = lame_encode_buffer_interleaved(lame_encoder, buffer, frames, mp3_buffer, BUFFER_SIZE);
        fwrite(mp3_buffer, sizeof(unsigned char), mp3_size, output_file);
    }

    // Flush and close MP3 file
    int final_mp3_size = lame_encode_flush(lame_encoder, mp3_buffer, BUFFER_SIZE);
    fwrite(mp3_buffer, sizeof(unsigned char), final_mp3_size, output_file);
    fclose(output_file);

    // Clean up
    free(buffer);
    free(fft_data);
    free(ifft_data);
    for (int i = 0; i < 6; i++)
    {
        free(bands[i]);
    }
    lame_close(lame_encoder);
    snd_pcm_close(handle);
}
