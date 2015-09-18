// This is the main file that describes the order in which things are
// called

#include <Arduino.h>
#include "drawing.h"
#include "board_values.h"
#include <Adafruit_ST7735.h> // Hardware-specific library

// standard U of A library settings, assuming Atmel Mega SPI pins
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const int JOYSTICK_HORIZ = 0; // Horizontal control line for joystick
const int JOYSTICK_VERT = 1; // Vertical control line for joystick
const int JOYSTICK_SEL = 9; // Select line for joystick

void setup() {

    // The least change of joystick position required to move cursor
    int sensitivity = 300;

    // The time delay
    int amount_slowed = 200;

    // Starting position of joystick
    int start_vertical;
    int start_horizontal;
    
    // Radius of pieces
    int radius = 3;

    // Initial position of cursor at center of board
    int x = 9;
    int y = 9;

    // Variables of where on the board to start checking both diagonal
    // directions
    int x1;
    int y1;

    // Set starting player; 1 = white, 2 = black; Black goes first
    int player = black;

    // Number of black pieces placed
    int count = 0;
    
    // Values recording the state of location on board
    int map_array[19][19] = {0};
    // map_array = 0 (empty) means nothing is there
    // map_array = 1 (white) means a white piece is there
    // map_array = 2 (black) means a black piece is there
    // map_array = 3 (white_wins) means this is a winning spot for white
    // map_array = 4 (black_wins)means this is a winning spot for black
    // map_array = 5 (both_win) means this is a winning spot for both
  
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735R chip, black tab
  
    digitalWrite(JOYSTICK_SEL, HIGH); // Initialize the button status
  
    draw_board(tft); // Draw the board

    // Read the initial vertical and horizontal values of the joystick
    // These values will be between 0 and 1023
    start_vertical = analogRead(JOYSTICK_VERT);
    start_horizontal = analogRead(JOYSTICK_HORIZ);

    int side = 2*radius - 1; // Side length of the cursor

    draw_cursor(tft, x, y, side); // Draw the cursor in its starting place

    // Constantly do the following steps until a winning move is 
    // made. 
    while(1){

        // When the button is not pressed, the cursor is free to move
        while(digitalRead(JOYSTICK_SEL) == HIGH){
            move_cursor(tft, start_vertical, start_horizontal, sensitivity, &x, 
                        &y, radius, side, map_array, player, amount_slowed);
        }

        // Stop program until the joystick is released to avoid multiple
        // clicks being read from one long click
        while(digitalRead(JOYSTICK_SEL) == LOW){
        }
        
        // Record the player's move and change map array values
        record(tft, x, y, x1, y1, radius, side, map_array, &player, &count);

        // Redraw the cursor
        draw_cursor(tft, x, y, side);

        // Go through the whole board if white, drawing white winning moves 
        // and erasing black's, according to map array values
        if(player == white){
            for(int i=0; i<19; ++i){
                for(int j=0; j<19; ++j){
                    draw_white_win(tft, i, j, side, map_array);
                }
            }
        }

        // Go through the whole board if black, drawing black winning moves 
        // and erasing white's, according to map array values 
        else if(player == black){
            for(int i=0; i<19; ++i){
                for(int j=0; j<19; ++j){
                    draw_black_win(tft, i, j, side, map_array);
                }
            }
        }
    }
}

void loop() {
}
