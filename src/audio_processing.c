#include "audio_processing.h"
#include <signal.h>
#include <unistd.h>

<<<<<<< HEAD
short band_1[BUFFER_SIZE];
short band_2[BUFFER_SIZE];
short band_3[BUFFER_SIZE];
short band_4[BUFFER_SIZE];
short band_5[BUFFER_SIZE];
short band_6[BUFFER_SIZE];
short output_buffer[BUFFER_SIZE];

void combine_bands(snd_pcm_t *handle, int frames)
{
    memset(output_buffer, 0, BUFFER_SIZE * sizeof(short));

    for (int i=0; i < frames; i++)
    {
        float acc = 0.0f;
        acc += band_1[i];
        acc += band_2[i];
        acc += band_3[i];
        acc += band_4[i];
        acc += band_5[i];
        acc += band_6[i];

        if (acc > MAX_SAMP) acc = MAX_SAMP;
        if (acc < N_MAX_SAMP) acc = N_MAX_SAMP;

        output_buffer[i] = (short) acc;
    }

    int err = snd_pcm_writei(handle, output_buffer, frames);
    if (err == -EPIPE)
    {
        fprintf(stderr, "Error: buffer overrun occurred\n");
        snd_pcm_prepare(handle);
    }


}
=======
unsigned char *mp3_buffer_full;
size_t mp3_buffer_full_size = 0;
size_t mp3_buffer_capacity = 1024 * 1024;
>>>>>>> origin/without-fftw

// Function to handle SIGINT (Ctrl+C)
void handle_sigint(int sig)
{
    FILE *output_file = fopen("final_output.mp3", "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: unable to open output MP3 file.\n");
        exit(1);
    }

    fwrite(mp3_buffer_full, sizeof(unsigned char), mp3_buffer_full_size, output_file);
    fclose(output_file);

    free(mp3_buffer_full);
    exit(0);
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

<<<<<<< HEAD
// Function to process FFT and reconstruct audio
// Takes raw audio buffer and number of frames as input
void process_audio_bands(short *buffer, int frames)
{
    int N = frames * CHANNELS; // Total number of samples
    double *in = fftw_malloc(sizeof(double) * N); // Real-values time domain samples
    fftw_complex *out = fftw_malloc(sizeof(fftw_complex) * (N / 2 + 1)); // Complex frequency domainn data
    fftw_plan plan_forward = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE); // forward transform
    fftw_plan plan_inverse = fftw_plan_dft_c2r_1d(N, out, in, FFTW_ESTIMATE); // inverse transform 

    // Copy audio data into the FFT input buffer
    for (int i = 0; i < N; i++)
=======
// Perform inverse FFT (Inverse Fast Fourier Transform)
void ifft(double complex *input, double complex *output, int n)
{
    for (int i = 0; i < n; i++)
>>>>>>> origin/without-fftw
    {
        input[i] = conj(input[i]);
    }

<<<<<<< HEAD

    // Perform FFT
    fftw_execute(plan_forward);

    // Frequency band definitions
    int band_limits[] = {BAND_1_MAX, BAND_2_MAX, BAND_3_MAX, BAND_4_MAX, BAND_5_MAX, BAND_6_MAX};
    // Defines maximum frequency for each band
    int band_count = 6;
    double bin_size = (double)SAMPLE_RATE / N;
=======
    fft(input, output, n);
>>>>>>> origin/without-fftw

    for (int i = 0; i < n; i++)
    {
<<<<<<< HEAD
        int lower_bin = (band == 0) ? 0 : (int)(band_limits[band - 1] / bin_size);
        int upper_bin = (int)(band_limits[band] / bin_size);
        // For each, calculates which FFT bins correspond to its frequency range

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
=======
        output[i] = conj(output[i]) / n;
    }
}
>>>>>>> origin/without-fftw

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
void process_audio()
{
    snd_pcm_t *handle; /* Pointer to manage PCM (Pulse Code Modulation) device*/
    snd_pcm_t *playback_handle; //output
    snd_pcm_hw_params_t *params; /* Pointer for hardware parameters */
    short *buffer; /* Array to store audio samples*/
    snd_pcm_uframes_t frames; /* number of frames to process */
    int err; 
    int sample_rate = SAMPLE_RATE; /* Audio sampling rate */

<<<<<<< HEAD
    // Open the default PCM device for capturing audio (input)
    // Capture mode (recording)
    // Default mode : 0
=======
    // Open PCM device for capture
>>>>>>> origin/without-fftw
    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to open PCM device: %s\n", snd_strerror(err));
        exit(1);
    }

<<<<<<< HEAD
    if ((err = snd_pcm_open(&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to open PCM device: %s\n", snd_strerror(err));
        exit(1);
    }

    // Allocate memory for ALSA hardware parameters
=======
>>>>>>> origin/without-fftw
    snd_pcm_hw_params_malloc(&params);


    // Fills params with default values for the PCM device
    if ((err = snd_pcm_hw_params_any(handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to initialize capture hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }
    if ((err = snd_pcm_hw_params_any(playback_handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to initialize playback hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

<<<<<<< HEAD
    // Set the hardware parameters (16-bit signed little-endian format, 2 channels, and sample rate)
    // Sets interleaved access mode (left and right channel samples are alternated)
    if ((err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        fprintf(stderr, "Error: unable to set access type capture: %s\n", snd_strerror(err));
        exit(1);
    }
    if ((err = snd_pcm_hw_params_set_access(playback_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        fprintf(stderr, "Error: unable to set access type playback: %s\n", snd_strerror(err));
        exit(1);
    }


    // Sets 16-bit signed little-endian sample format.
    if ((err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        fprintf(stderr, "Error: unable to set sample format capture: %s\n", snd_strerror(err));
        exit(1);
    }
    if ((err = snd_pcm_hw_params_set_format(playback_handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        fprintf(stderr, "Error: unable to set sample format playback: %s\n", snd_strerror(err));
        exit(1);
    }

    // Sets the number of channels, 2.
    if ((err = snd_pcm_hw_params_set_channels(handle, params, CHANNELS)) < 0)
=======
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
>>>>>>> origin/without-fftw
    {
        bands[i] = (double complex *)malloc(frames * sizeof(double complex));
    }
    if ((err = snd_pcm_hw_params_set_channels(playback_handle, params, CHANNELS)) < 0)
    {
        fprintf(stderr, "Error: unable to set channels: %s\n", snd_strerror(err));
        exit(1);
    }

<<<<<<< HEAD
    // Pass a pointer to the sample_rate variable
    if ((err = snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to set sample rate: %s\n", snd_strerror(err));
        exit(1);
    }
    if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, params, &sample_rate, 0)) < 0)
    {
        fprintf(stderr, "Error: unable to set sample rate: %s\n", snd_strerror(err));
        exit(1);
    }


    // Apply all hardware parameters
    if ((err = snd_pcm_hw_params(handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to set hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }
    if ((err = snd_pcm_hw_params(playback_handle, params)) < 0)
    {
        fprintf(stderr, "Error: unable to set hardware parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    // Allocate the buffer for audio data
    // Gets the period size (frames per period) and allocated buffer memory accordingly.
    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    buffer = (short *)malloc(frames * sizeof(short) * CHANNELS);


    // Keep capturing the audio
=======
    // Initialize LAME for final MP3 encoding
    lame_t lame_encoder = lame_init();
    lame_set_in_samplerate(lame_encoder, SAMPLE_RATE);
    lame_set_num_channels(lame_encoder, CHANNELS);
    lame_set_brate(lame_encoder, 128);
    lame_init_params(lame_encoder);

    signal(SIGINT, handle_sigint);
    mp3_buffer_full = (unsigned char *)malloc(mp3_buffer_capacity);

    unsigned char mp3_buffer[BUFFER_SIZE];

>>>>>>> origin/without-fftw
    while (1)
    {
        //Reads audio data from the device into buffer
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

<<<<<<< HEAD
        //processes the captured audio data.
        process_audio_bands(buffer, frames);

        // Modify the six frequency bands.
        for (int i = 0; i < 6; i++)
=======
        // Ensure FFT size is a power of two
        int fft_size = frames;
        if (!is_power_of_two(fft_size))
        {
            fft_size = next_power_of_two(frames);
        }

        // Prepare the FFT input
        prepare_fft_input(buffer, fft_data, fft_size);

        // FFT and band processing
        fft(fft_data, fft_data, fft_size);
        split_into_bands(fft_data, bands, fft_size);

        // Combine all bands into a single waveform
        for (int i = 0; i < fft_size; i++)
>>>>>>> origin/without-fftw
        {
            ifft_data[i] = 0;
            for (int j = 0; j < 6; j++)
            {
                ifft_data[i] += bands[j][i];
            }
        }

<<<<<<< HEAD
        combine_bands(playback_handle, frames);
=======
        // Inverse FFT to get time-domain waveform
        ifft(ifft_data, fft_data, fft_size);

        // Convert complex waveform to PCM format
        for (int i = 0; i < frames; i++)
        {
            buffer[i] = (short)creal(fft_data[i]);
        }

        // Encode to MP3
        int mp3_size = lame_encode_buffer_interleaved(lame_encoder, buffer, frames, mp3_buffer, BUFFER_SIZE);

        if (mp3_buffer_full_size + mp3_size > mp3_buffer_capacity)
        {
            mp3_buffer_capacity *= 2;
            mp3_buffer_full = (unsigned char *)realloc(mp3_buffer_full, mp3_buffer_capacity);
        }

        memcpy(mp3_buffer_full + mp3_buffer_full_size, mp3_buffer, mp3_size);
        mp3_buffer_full_size += mp3_size;
>>>>>>> origin/without-fftw
    }

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
    snd_pcm_close(playback_handle);
    snd_pcm_hw_params_free(params);
}
