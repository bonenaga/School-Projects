#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input.wav> <output.wav>\n", argv[0]);
        return 1;
    }

    // Open input file for reading
    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // Read header
    WAVHEADER header;
    if (fread(&header, sizeof(WAVHEADER), 1, inputFile) != 1)
    {
        fprintf(stderr, "Failed to read header from %s.\n", argv[1]);
        fclose(inputFile);
        return 3;
    }

    // Use check_format to ensure WAV format
    if (check_format(header) != 0)
    {
        fprintf(stderr, "%s is not a valid WAV file.\n", argv[1]);
        fclose(inputFile);
        return 4;
    }

    // Open output file for writing
    FILE *outputFile = fopen(argv[2], "wb");
    if (outputFile == NULL)
    {
        fprintf(stderr, "Could not create %s.\n", argv[2]);
        fclose(inputFile);
        return 5;
    }

    // Write header to file
    if (fwrite(&header, sizeof(WAVHEADER), 1, outputFile) != 1)
    {
        fprintf(stderr, "Failed to write header to %s.\n", argv[2]);
        fclose(inputFile);
        fclose(outputFile);
        return 6;
    }

    // Read the entire audio data
    char *data = malloc(header.subchunk2Size);
    if (data == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(inputFile);
        fclose(outputFile);
        return 7;
    }

    if (fread(data, 1, header.subchunk2Size, inputFile) != header.subchunk2Size)
    {
        fprintf(stderr, "Failed to read audio data from %s.\n", argv[1]);
        free(data);
        fclose(inputFile);
        fclose(outputFile);
        return 8;
    }

    // Reverse the entire audio data
    int sampleSize = header.bitsPerSample / 8 * header.numChannels;
    for (int i = 0; i < header.subchunk2Size / 2; i += sampleSize)
    {
        for (int j = 0; j < sampleSize; j++)
        {
            char temp = data[i + j];
            data[i + j] = data[header.subchunk2Size - i - sampleSize + j];
            data[header.subchunk2Size - i - sampleSize + j] = temp;
        }
    }

    // Write the reversed audio data
    if (fwrite(data, 1, header.subchunk2Size, outputFile) != header.subchunk2Size)
    {
        fprintf(stderr, "Failed to write reversed audio data to %s.\n", argv[2]);
        free(data);
        fclose(inputFile);
        fclose(outputFile);
        return 9;
    }

    // Clean up
    free(data);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

int check_format(WAVHEADER header)
{
    // Check for 'RIFF' in chunkID and 'WAVE' in format
    if (strncmp((const char *) header.chunkID, "RIFF", 4) != 0 || strncmp((const char *) header.format, "WAVE", 4) != 0)
    {
        return 1; // Incorrect format
    }
    return 0; // Correct format
}

int get_block_size(WAVHEADER header)
{
    // For simplicity, assuming block align is the block size
    return header.blockAlign;
}
