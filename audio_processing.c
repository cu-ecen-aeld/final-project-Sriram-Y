#include "audio_processing.h"

void apply_gain_to_band(double complex *band, int band_size, double gain)
{
    for (int i = 0; i < band_size; i++)
    {
        band[i] *= gain;
    }
}

// Check if a number is a power of two
int is_power_of_two(int n)
{
    return (n > 0) && ((n & (n - 1)) == 0);
}

// Get the next power of two greater than or equal to n
int next_power_of_two(int n)
{
    int power = 1;
    while (power < n)
    {
        power *= 2;
    }
    return power;
}

// Prepare FFT input (convert short to double complex)
void prepare_fft_input(short *input, double complex *output, int n)
{
    for (int i = 0; i < n; i++)
    {
        output[i] = (double)input[i]; // Convert short to double
    }
}

// Perform FFT (Fast Fourier Transform)
void fft(double complex *input, double complex *output, int n)
{
    if (n == 1)
    {
        output[0] = input[0];
        return;
    }

    if (n <= 0)
    {
        fprintf(stderr, "Error: Invalid FFT size %d\n", n);
        exit(1);
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
    fft(even, even_fft, n / 2);
    fft(odd, odd_fft, n / 2);

    for (int k = 0; k < n / 2; k++)
    {
        double complex t = cexp(-2.0 * I * M_PI * k / n) * odd_fft[k];
        output[k] = even_fft[k] + t;
        output[k + n / 2] = even_fft[k] - t;
    }
}

// Perform inverse FFT (Inverse Fast Fourier Transform)
void ifft(double complex *input, double complex *output, int n)
{
    for (int i = 0; i < n; i++)
    {
        input[i] = conj(input[i]);
    }

    fft(input, output, n);

    for (int i = 0; i < n; i++)
    {
        output[i] = conj(output[i]) / n;
    }
}

// Split the FFT data into frequency bands
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
        int lower_limit = (band == 0) ? 0 : band_limits[band - 1];
        int upper_limit = band_limits[band]; // Ending index for the band

        // Initialize the band's frequency data to 0
        for (int i = lower_limit; i < upper_limit; i++)
        {
            bands[band][i] = fft_data[i];
        }
    }
}

// Process the audio (main function)
void process_audio(double gains[6], bool willExport)
{
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *params;
    short *buffer;
    snd_pcm_uframes_t frames;
    int err;
    unsigned int sample_rate = SAMPLE_RATE;

    lame_t lame_encoder;
    FILE *output_file;
    unsigned char mp3_buffer[BUFFER_SIZE];

    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *playback_params;

    // Open PCM device for capture
    if ((err = snd_pcm_open(&capture_handle, "hw:Loopback,1,0", SND_PCM_STREAM_CAPTURE, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to open PCM device: %s\n", snd_strerror(err));
        exit(1);
    }

    snd_pcm_hw_params_malloc(&params);
    if ((err = snd_pcm_hw_params_any(capture_handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to initialize hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    // Set the hardware parameters
    if ((err = snd_pcm_hw_params_set_access(capture_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0 ||
        (err = snd_pcm_hw_params_set_format(capture_handle, params, SND_PCM_FORMAT_S16_LE)) < 0 ||
        (err = snd_pcm_hw_params_set_channels(capture_handle, params, CHANNELS)) < 0 ||
        (err = snd_pcm_hw_params_set_rate_near(capture_handle, params, &sample_rate, 0)) < 0 ||
        (err = snd_pcm_hw_params(capture_handle, params)) < 0)
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

    if (!willExport)
    {
        if ((err = snd_pcm_open(&playback_handle, "hw:1,0", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
        {
            fprintf(stderr, "Error: unable to open PCM playback device: %s\n", snd_strerror(err));
            exit(1);
        }

        // Set the hardware parameters for playback
        snd_pcm_hw_params_malloc(&playback_params);
        if ((err = snd_pcm_hw_params_any(playback_handle, playback_params)) < 0 ||
            (err = snd_pcm_hw_params_set_access(playback_handle, playback_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0 ||
            (err = snd_pcm_hw_params_set_format(playback_handle, playback_params, SND_PCM_FORMAT_S16_LE)) < 0 ||
            (err = snd_pcm_hw_params_set_channels(playback_handle, playback_params, CHANNELS)) < 0 ||
            (err = snd_pcm_hw_params_set_rate_near(playback_handle, playback_params, &sample_rate, 0)) < 0 ||
            (err = snd_pcm_hw_params(playback_handle, playback_params)) < 0)
        {
            fprintf(stderr, "Error: unable to set playback hardware parameters: %s\n", snd_strerror(err));
            exit(1);
        }
        snd_pcm_hw_params_malloc(&playback_params);
    }
    else
    {
        // Initialize LAME for final MP3 encoding
        lame_encoder = lame_init();
        lame_set_in_samplerate(lame_encoder, SAMPLE_RATE);
        lame_set_num_channels(lame_encoder, CHANNELS);
        lame_set_brate(lame_encoder, 128);
        lame_init_params(lame_encoder);

        output_file = fopen("final_output.mp3", "wb");
        if (!output_file)
        {
            fprintf(stderr, "Error: unable to open output MP3 file.\n");
            exit(1);
        }
    }

    while (1)
    {
        err = snd_pcm_readi(capture_handle, buffer, frames);
        if (err == -EPIPE)
        {
            fprintf(stderr, "Error: buffer overrun occurred\n");
            snd_pcm_prepare(capture_handle);
            continue;
        }
        else if (err < 0)
        {
            fprintf(stderr, "Error: failed to read audio data: %s\n", snd_strerror(err));
            break;
        }
        else if (err != frames)
        {
            fprintf(stderr, "Error: short read, read %d frames instead of %ld\n", err, frames);
            continue;
        }

        fprintf(stdout, "Captured buffer values:");

        for (int i = 0; i < CHANNELS * frames; i++)
        {
            fprintf(stdout, "%d, ", buffer[i]);
            if ((i+1) % 20 == 0) fprintf(stdout, "\n");
        }

        fprintf(stdout, "\n");

        // Ensure FFT size is a power of two
        int fft_size = frames;
        if (willExport)
        {
            if (!is_power_of_two(fft_size))
            {
                fft_size = next_power_of_two(frames);
            }
        }

        // Prepare the FFT input
        prepare_fft_input(buffer, fft_data, fft_size);

        // FFT and band processing
        fft(fft_data, fft_data, fft_size);
        split_into_bands(fft_data, bands, fft_size);

        for (int j = 0; j < 6; j++)
        {
            apply_gain_to_band(bands[j], fft_size, gains[j]);
        }

        // Combine all bands into a single waveform
        for (int i = 0; i < fft_size; i++)
        {
            ifft_data[i] = 0;
            for (int j = 0; j < 6; j++)
            {
                ifft_data[i] += bands[j][i];
            }
        }

        // Inverse FFT to get time-domain waveform
        ifft(ifft_data, fft_data, fft_size);

        // Convert complex waveform to PCM format
        for (int i = 0; i < frames; i++)
        {
            buffer[i] = (short)creal(fft_data[i]);
        }

        if (!willExport)
        {
            err = snd_pcm_writei(playback_handle, buffer, frames);
            if (err == -EPIPE)
            {
                fprintf(stderr, "Error: buffer underrun occurred\n");
                snd_pcm_prepare(playback_handle);
            }
            else if (err < 0)
            {
                fprintf(stderr, "Error: failed to write audio data: %s\n", snd_strerror(err));
                break;
            }
        }
        else
        {
            // Encode to MP3
            int mp3_size = lame_encode_buffer_interleaved(lame_encoder, buffer, frames, mp3_buffer, BUFFER_SIZE);
            fwrite(mp3_buffer, sizeof(unsigned char), mp3_size, output_file);
        }
    }

    // Clean up
    free(buffer);
    free(fft_data);
    free(ifft_data);
    for (int i = 0; i < 6; i++)
    {
        free(bands[i]);
    }
    snd_pcm_close(capture_handle);

    if (!willExport)
    {
        snd_pcm_close(playback_handle);
    }
    else
    {
        // Flush and close MP3 file
        int final_mp3_size = lame_encode_flush(lame_encoder, mp3_buffer, BUFFER_SIZE);
        fwrite(mp3_buffer, sizeof(unsigned char), final_mp3_size, output_file);
        fclose(output_file);
        lame_close(lame_encoder);
    }
}
