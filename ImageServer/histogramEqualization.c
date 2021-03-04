// C program to perform histogram equalization to adjust contrast levels in pictures with raw format

#include "histogramEqualization.h"
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to perform histogram equalization on an image
// Function takes total rows, columns, input file name and output file name as parameters

void histogramEqualization(int cols, int rows, char *input_file_name, char *output_file_name)
{
    // Creating image pointer
    unsigned char *image;

    // Declaring 2 arrays for storing histogram values (frequencies) and
    // new gray level values (newly mapped pixel values as per algorithm)
    int hist[256] = { 0 };
    int new_gray_level[256] = { 0 };

    // Declaring other important variables
    int input_file, output_file, col, row, total, curr, i;

    // Allocating image array the size equivalent to number of columns
    // of the image to read one row of an image at a time
    image = (unsigned char*)calloc(cols, sizeof(unsigned char));

    // Opening input file in Read Only Mode
    input_file = open(input_file_name, O_RDONLY);
    if (input_file < 0) {
        printf("Error opening input file\n");
        exit(1);
    }

    // Creating output file that has write and read access
    output_file = creat(output_file_name, 0666);
    if (output_file < 0) {
        printf("Error creating output file\n");
        exit(1);
    }

    // Calculating frequency of occurrence for all pixel values
    for (row = 0; row < rows; row++) {
        // Reading a row of image
        read(input_file, &image[0], cols * sizeof(unsigned char));

        // Logic for calculating histogram
        for (col = 0; col < cols; col++)
            hist[(int)image[col]]++;
    }

    // Calculating total number of pixels
    total = cols * rows;

    curr = 0;

    // Calculating cumulative frequency and new gray levels
    for (i = 0; i < 256; i++) {
        // Cumulative frequency
        curr += hist[i];

        // Calculating new gray level after multiplying by
        // maximum gray count which is 255 and dividing by
        // total number of pixels
        new_gray_level[i] = round((((float)curr) * 255) / total);
    }

    // Closing file
    close(input_file);

    // Reopening file in Read Only Mode
    input_file = open(input_file_name, O_RDONLY);

    // Performing histogram equalization by mapping new gray levels
    for (row = 0; row < rows; row++) {
        // Reading a row of image
        read(input_file, &image[0], cols * sizeof(unsigned char));

        // Mapping to new gray level values
        for (col = 0; col < cols; col++)
            image[col] = (unsigned char)new_gray_level[image[col]];

        // Writing new gray level mapped
        write(output_file, &image[0], cols * sizeof(unsigned char));
    }

    // Freeing dynamically allocated memory
    free(image);

    // Closing input and output files
    close(input_file);
    close(output_file);
}

// Code example for testing the equalization algorithm in the main function
// COLS = Width, ROWS = Height (All of that in pixels unit)

/*
    COLS = 512;
    ROWS = 512;
    INPUT_FILE_NAME = "/home/fabelifer2797/VM_Services/ImageServer/lena_gray.raw";
    OUTPUT_FILE_NAME = "/home/fabelifer2797/VM_Services/ImageServer/lena_Histogram_Equalized.raw";
    histogramEqualization(COLS,ROWS,INPUT_FILE_NAME,OUTPUT_FILE_NAME);
 */

