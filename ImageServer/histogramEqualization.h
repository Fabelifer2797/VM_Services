#ifndef IMAGESERVER_HISTOGRAMEQUALIZATION_H
#define IMAGESERVER_HISTOGRAMEQUALIZATION_H

int COLS;
int ROWS;
char *INPUT_FILE_NAME;
char *OUTPUT_FILE_NAME;
void histogramEqualization(int cols, int rows, char *input_file_name, char *output_file_name);

#endif
