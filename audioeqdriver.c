#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "audio_processing.h"

#define MAX_LINE_LENGTH 100

bool check_export(const char *filename);

int main()
{
    printf("Audio EQ Driver is running!!!\n");

    double gains[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // Default gains
    char* filename = "gains.txt";
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open gains file. Using default gains.\n");
    }
    else
    {
        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, file))
        {
            if (line[0] == 'B' && line[2] == '=')
            {
                int band = line[1] - '1';
                if (band >= 0 && band < 6)
                {
                    double gain = atof(&line[3]);
                    gains[band] = gain;
                }
            }
        }
        fclose(file);
    }

    process_audio(gains, check_export(filename));

    return 0;
}

bool check_export(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s for EXPORT check.\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        // Look for the string EXPORT
        if (strstr(line, "EXPORT") != NULL)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
