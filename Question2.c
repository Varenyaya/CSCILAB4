#include <stdio.h> // For printf, fopen, fclose, fgets, sscanf, fscanf, fprintf, fwrite
#include <stdlib.h> // For malloc, free

// Global constants for image dimensions
#define WIDTH 512 // Width of the image
#define HEIGHT 512 // Height of the image

// Function prototypes
int readPGMText(const char *filename, unsigned char *pixels, int width, int height); // Function to read an ASCII PGM file
int writePGMText(const char *filename, unsigned char *pixels, int width, int height); // Function to write an ASCII PGM file
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height); // Function to write a binary PGM file
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height); // Function to embed a secret image using 4-bit LSB
void extractLSB(unsigned char *stegoPixels, unsigned char *outputPixels, int width, int height); // Function to extract the secret image from a stego image

// Main function
int main() { 
    // File paths
    const char *coverImageFile = "../baboon.pgm"; // Cover image file path
    const char *secretImageFile = "../farm.pgm"; // Secret image file path
    const char *stegoImageFile = "stego_image_bin.pgm"; // Stego image file path
    const char *extractedSecretFile = "extracted_secret.pgm"; // Extracted secret image file path

    // Dynamically allocate memory for images
    unsigned char *coverPixels = (unsigned char *)malloc(WIDTH * HEIGHT); // Cover image pixels
    unsigned char *secretPixels = (unsigned char *)malloc(WIDTH * HEIGHT); // Secret image pixels
    unsigned char *stegoPixels = (unsigned char *)malloc(WIDTH * HEIGHT); // Stego image pixels
    unsigned char *outputPixels = (unsigned char *)malloc(WIDTH * HEIGHT); // Extracted secret image pixels

    // Check memory allocation
    if (!coverPixels || !secretPixels || !stegoPixels || !outputPixels) { // If memory allocation failed
        printf("Error: Memory allocation failed.\n"); // Print an error message
        free(coverPixels); // Free allocated memory
        free(secretPixels); // Free allocated memory
        free(stegoPixels); // Free allocated memory
        free(outputPixels); // Free allocated memory
        return 1; // Return 1 to indicate failure
    }

    // Read cover and secret images
    if (readPGMText(coverImageFile, coverPixels, WIDTH, HEIGHT) != 0) { // If reading the cover image failed
        printf("Error: Could not read cover image.\n"); // Print an error message
        free(coverPixels); // Free allocated memory
        free(secretPixels); // Free allocated memory
        free(stegoPixels); //  Free allocated memory
        free(outputPixels); // Free allocated memory
        return 1; // Return 1 to indicate failure
    }
    if (readPGMText(secretImageFile, secretPixels, WIDTH, HEIGHT) != 0) { // If reading the secret image failed
        printf("Error: Could not read secret image.\n"); // Print an error message
        free(coverPixels); // Free allocated memory
        free(secretPixels); // Free allocated memory
        free(stegoPixels); // Free allocated memory
        free(outputPixels); // Free allocated memory
        return 1; // Return 1 to indicate failure
    }

    // Embed secret image into cover image
    embedLSB(coverPixels, secretPixels, WIDTH, HEIGHT); 

    // Save the stego image in binary format
    if (writePGMBinary(stegoImageFile, coverPixels, WIDTH, HEIGHT) != 0) { // If writing the stego image failed
        printf("Error: Could not save stego image.\n"); // Print an error message
        free(coverPixels); // Free allocated memory
        free(secretPixels); // Free allocated memory
        free(stegoPixels); // Free allocated memory
        free(outputPixels); // Free allocated memory
        return 1; // Return 1 to indicate failure
    }

    // Extract the secret image from the stego image
    extractLSB(coverPixels, outputPixels, WIDTH, HEIGHT);

    // Save the extracted secret image in text format
    if (writePGMText(extractedSecretFile, outputPixels, WIDTH, HEIGHT) != 0) {
        printf("Error: Could not save extracted secret image.\n");
    }

    // Free allocated memory
    free(coverPixels); 
    free(secretPixels);
    free(stegoPixels);
    free(outputPixels);

    return 0; // Return 0 to indicate successful completion
}

// Function to read an ASCII PGM file
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) { // Function to read an ASCII PGM file
    FILE *file = fopen(filename, "r"); // Open the file in read mode
    if (!file) return 1; // If the file could not be opened, return 1 to indicate failure

    char buffer[16]; // Buffer to store the header information
    if (!fgets(buffer, sizeof(buffer), file) || buffer[0] != 'P' || buffer[1] != '2') { // If the file is not a P2 PGM file
        fclose(file); // Close the file
        return 1; // Return 1 to indicate failure
    }

    do { // Skip comments
        if (!fgets(buffer, sizeof(buffer), file)) { // If the file does not contain valid data
            fclose(file); // Close the file
            return 1; // Return 1 to indicate failure
        }
    } while (buffer[0] == '#'); // Continue reading until a non-comment line is found

    int w, h, maxVal; // Variables to store width, height, and maximum pixel value
    sscanf(buffer, "%d %d", &w, &h); // Read width and height from the buffer
    if (fscanf(file, "%d", &maxVal) != 1 || w != width || h != height || maxVal != 255) { // If the width, height, or maximum pixel value is invalid
        fclose(file); // Close the file
        return 1; // Return 1 to indicate failure
    }

    for (int i = 0; i < width * height; i++) { // Read pixel values
        if (fscanf(file, "%hhu", pixels + i) != 1) { // If reading the pixel value failed
            fclose(file); // Close the file
            return 1; // Return 1 to indicate failure
        }
    }

    fclose(file); // Close the file
    return 0; // Return 0 to indicate success
}

// Function to write an ASCII PGM file
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) { // Function to write an ASCII PGM file
    FILE *file = fopen(filename, "w"); // Open the file in write mode
    if (!file) return 1; // If the file could not be opened, return 1 to indicate failure

    fprintf(file, "P2\n%d %d\n255\n", width, height); // Write the header information
    for (int i = 0; i < width * height; i++) { // Write pixel values
        fprintf(file, "%d\n", *(pixels + i)); // Write the pixel value
    }

    fclose(file); // Close the file
    return 0; // Return 0 to indicate success
}

// Function to write a binary PGM file
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) { // Function to write a binary PGM file
    FILE *file = fopen(filename, "wb"); // Open the file in binary write mode
    if (!file) return 1; // If the file could not be opened, return 1 to indicate failure

    fprintf(file, "P5\n%d %d\n255\n", width, height); // Write the header information
    fwrite(pixels, sizeof(unsigned char), width * height, file); // Write pixel values

    fclose(file); // Close the file
    return 0; // Return 0 to indicate success
}

// Function to embed a secret image using 4-bit LSB
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) { // Function to embed a secret image using 4-bit LSB
    for (int i = 0; i < width * height; i++) { // Iterate over all pixels
        unsigned char coverMSB = *(coverPixels + i) & 0xF0; // Extract upper 4 bits of the cover pixel
        unsigned char secretMSB = (*(secretPixels + i) & 0xF0) >> 4; // Extract upper 4 bits of the secret pixel
        *(coverPixels + i) = coverMSB | secretMSB; // Combine both to form the stego pixel
    }
}

// Function to extract the secret image from a stego image
void extractLSB(unsigned char *stegoPixels, unsigned char *outputPixels, int width, int height) { // Function to extract the secret image from a stego image
    for (int i = 0; i < width * height; i++) { // Iterate over all pixels
        unsigned char secretMSB = (*(stegoPixels + i) & 0x0F) << 4; // Extract lower 4 bits and shift to upper
        *(outputPixels + i) = secretMSB; // Store reconstructed secret pixel
    }
}