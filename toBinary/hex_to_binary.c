#include "hex_to_binary.h"

int main()
{
    FILE *input;
    FILE *output;
    char readingHex;
    char outHex;

    input = fopen(INPUT_FILE, "r");
    output = fopen(OUTPUT_FILE, "w");
    while (1)
    {
        readingHex = fgetc(input);
        outHex = strtol(&readingHex, NULL, 16) << 4;
        readingHex = fgetc(input);
        outHex += strtol(&readingHex, NULL, 16);
        if (feof(input))
            break;
        putc(outHex, output);
    }
    fclose(input);
    fclose(output);
}