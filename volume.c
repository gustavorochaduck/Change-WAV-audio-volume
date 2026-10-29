
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int HEADER_SIZE = 44;
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    float factor = atof(argv[3]);
    char header[44];
    size_t InputHeader = fread(header, 1, 44, input);
    if (InputHeader != 44)
    {
        fclose(input);
        fclose(output);
        return 1;
    }
    size_t OutputHeader = fwrite(header, 1, 44, output);
    int16_t buffer[4096];
    size_t n;
    while ((n = fread(buffer, sizeof(int16_t), 4096, input)) > 0)
    {
        for (size_t i = 0; i < n; i++)
        {
            int32_t demo = buffer[i];
            demo = demo * atof(argv[3]);
            buffer[i] = (int16_t) demo;
        }
        if (fwrite(buffer, sizeof(int16_t), n, output) != n)
            break;
    }
    fclose(input);
    fclose(output);
}
