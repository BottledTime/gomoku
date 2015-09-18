// This file is corresponding to the drawing.cpp file and declares
// the functions described in drawing.cpp that are used in other files

#ifndef DRAWING_H
#define DRAWING_H
#include <Adafruit_ST7735.h> // Hardware-specific library

void draw_board(Adafruit_ST7735 tft);

void draw_cursor(Adafruit_ST7735 tft, int x, int y, int side);

void record(Adafruit_ST7735 tft, int x, int y, int x1, int y1, int radius, 
            int side, int map_array[19][19], int * player, int * count);

void move_cursor(Adafruit_ST7735 tft, int start_vertical, int start_horizontal,
                 int sensitivity, int * x, int * y, int radius, int side, 
                 int map_array[19][19], int player, int amount_slowed);

void draw_white_win(Adafruit_ST7735 tft, int x, int y, int side, 
                    int map_array[19][19]);

void draw_black_win(Adafruit_ST7735 tft, int x, int y, int side, 
                    int map_array[19][19]);
#endif
