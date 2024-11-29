#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void processFileWithFseek(const char *inputFile, const char *outputFile);

int main() {
    const char *inputFile = "../data.txt";
    const char *outputFile = "valid_data.txt";

    processFileWithFseek(inputFile, outputFile);
    return 0;
}

void processFileWithFseek(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "r");
    FILE *outFile = fopen(outputFile, "w");

    // Verify file access
    if (inFile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    if (outFile == NULL) {
        perror("Error opening output file");
        fclose(inFile);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    int invalidCount = 0;

    while (1) {
        long currentPos = ftell(inFile); // Get current position
        if (fscanf(inFile, "%255s", buffer) == 1) {
            char *endPtr;
            float value = strtof(buffer, &endPtr);

            // Check if the entire buffer is a valid float
            if (*endPtr == '\0') {
                fprintf(outFile, "%f\n", value);
            } else {
                // Invalid data: reset file pointer to the next token
                fseek(inFile, currentPos + strlen(buffer) + 1, SEEK_SET);
                invalidCount++;
            }
        } else {
            if (feof(inFile)) {
                break; // End of file reached
            }
            if (ferror(inFile)) {
                perror("Error reading input file");
                break;
            }
        }
    }

    // Close files
    fclose(inFile);
    fclose(outFile);

    // Display results
    printf("Number of invalid float values: %d\n", invalidCount);
}