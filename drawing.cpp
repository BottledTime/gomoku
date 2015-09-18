// This file is corresponding to drawing.h and describes the functions
// about drawing about the game.

#include <Arduino.h>
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "math.h"
#include "board_values.h"

const int JOYSTICK_HORIZ = 0; // Horizontal control line for joystick
const int JOYSTICK_VERT = 1; // Vertical control line for joystick

const int size_of_board = 19; // Side length of the board

// Function that draws the map array with 19 horizontal lines and 19 vertical
// lines coloured black with an interval of 6 pixels between each 
void draw_board(Adafruit_ST7735 tft){
  tft.fillScreen(ST7735_WHITE);
  for (int16_t y=0; y <= 108; y+=6) {
      tft.drawFastHLine(10, (y+10), 108, ST7735_BLACK);
  }
  for (int16_t x=0; x <= 108; x+=6) {
      tft.drawFastVLine((x+10), 10, 109, ST7735_BLACK);
  }
}

// Put black text on the screen below the map array
void draw_text(Adafruit_ST7735 tft, char *text) {
  tft.setCursor(10, 128);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print(text);
}

// Draw black piece as a circle filled with black at location x, y on
// the map array with certain radius
void draw_black_piece(Adafruit_ST7735 tft, int x, int y, int radius, 
                      int map_array[19][19]){
    map_array[x][y] = black_piece;
    tft.fillCircle((6*x+10), (6*y+10), radius, ST7735_BLACK);
}

// Draw white piece as a circle filled with black at location x, y on
// the map array with certain radius
void draw_white_piece(Adafruit_ST7735 tft, int x, int y, int radius,
                      int map_array[19][19]){
    map_array[x][y] = white_piece;
    tft.fillCircle((6*x+10), (6*y+10), radius, ST7735_WHITE);
    tft.drawCircle((6*x+10), (6*y+10), radius, ST7735_BLACK);
}

// Draw the cursor as a black rectangle with white filling at location x, y
// on the map array with certain side length
void draw_cursor(Adafruit_ST7735 tft, int x, int y, int side){
    tft.fillRect((6*x+8), (6*y+8), side, side, ST7735_WHITE);
    tft.drawRect((6*x+8), (6*y+8), side, side, ST7735_BLACK);
}

// Redraw the previous location of the cursor on the map array
// when the cursor moves
void redraw_board(Adafruit_ST7735 tft, int x, int y, int side){

    // Use an all white rectangle to fill the cursor
    tft.fillRect((6*x+8), (6*y+8), side, side, ST7735_WHITE);

    // If not at boundary, the line length adjust factor is len_x and len_y
    // and the position variable adjust factor is pos_x and pox_y
    int pos_x = 2;
    int len_x = 2;
    int pos_y = 2;
    int len_y = 2;

    // When at left boundary, redraw vertical line and right
    // part of horizontal line
    if(x == 0){
        pos_x = 0;
        len_x = 0;
    }

    // When at right boundary, redraw vertical line and left 
    // part of horizontal line
    else if(x == 18){
        pos_x = 2;
        len_x = 0;
    }


    // When at top boundary, redraw horizontal line and bottom
    // part of vertical line
    if(y == 0){
        pos_y = 0;
        len_y = 0;
    }

    // When at bottom boundary, redraw horizontal line and top
    // part of vertical line
    else if(y == 18){
        pos_y = 2;
        len_y = 0;
    }

    // Draw one horizontal and one vertical line with settings set above to 
    // complete the map.
    tft.drawFastHLine((6*x+10-pos_x), (6*y+10), 3+len_x, ST7735_BLACK);
    tft.drawFastVLine((6*x+10), (6*y+10-pos_y), 3+len_y, ST7735_BLACK);
}

// Redraw the part of board where a winning move was displayed
void wipe_clean(Adafruit_ST7735 tft, int side, int map_array[19][19]){
    for(int i=0; i<19; ++i){
        for(int j=0; j<19; ++j){
            if(map_array[i][j] == white_win || map_array[i][j] == black_win ||
               map_array[i][j] == both_win){
                redraw_board(tft,i,j,side);
            }
        }
    }
}

// Function that sets the rules of piece putting and game ending 
void record(Adafruit_ST7735 tft, int x, int y, int x1, int y1, int radius, 
            int side, int map_array[19][19], int * player, int * count){

    // If the user wants to put a piece on another piece, display an error
    // message. After one second, use a white rectangle to erase the error
    // message.
    if(map_array[x][y] == white_piece || map_array[x][y] == black_piece){
        // Print an error message
        draw_text(tft, "You can not put a \n  piece here");
        delay(1000);
        tft.fillRect(0, 128, 128, 32, ST7735_WHITE);
    }

    // If a piece is put on a winning move displayed, display the winning
    // message, erase the winning moves and go to an infinite while loop to 
    // terminate the game.
    else if(map_array[x][y] == (*player + 2) || map_array[x][y] == both_win){
        if(*player == white){
            draw_white_piece(tft, x, y, radius, map_array);
            draw_text(tft,"WHITE WINS!");
            wipe_clean(tft, side, map_array);
            while(1){}
        }
        else if(*player == black){
            draw_black_piece(tft, x, y, radius, map_array);
            draw_text(tft,"BLACK WINS!");
            wipe_clean(tft, side, map_array);
            while(1){}
        }
    }

    // Otherwise, draw the piece, change the map array value and change 
    // the player. If it's a black piece, count it.
    else{
        if(*player == black){
            draw_black_piece(tft, x, y, radius, map_array);
            record_win_move(*player, x, y, x1, y1, map_array);
            *player = white;            
            (*count)++;
        }
        else if(*player == white){
            draw_white_piece(tft, x, y, radius, map_array);
            record_win_move(*player, x, y, x1, y1, map_array);
            *player = black;
        }
    }   

    // If the board is fully filled which means 181 black pieces are put,
    // display DRAW message and go into a infinite loop to terminate the 
    // game
    if((*count) == (size_of_board*size_of_board+1)/2){
        draw_text(tft, "DRAW!");
        while(1){}
    }
}

// Decide whether to redraw pieces, winning moves, or that part of the 
// board when the cursor moves 
void decide(Adafruit_ST7735 tft, int x, int y, int radius, int side, 
            int map_array[19][19], int player){

    // If the value of map array is white_piece, draw white piece.
    if(map_array[x][y] == white_piece){ 
        draw_white_piece(tft, x, y, radius, map_array);
    }

    // If the value of map array is black_piece, draw black piece.
    else if(map_array[x][y] == black_piece){
        draw_black_piece(tft, x, y, radius, map_array);
    }

    // If the value of map array is black_win or both_win, draw red
    // rectangle as winning move hint.
    else if(map_array[x][y] == (player+2) || map_array[x][y] == both_win){
        tft.fillRect(6*x+8, 6*y+8, side, side, ST7735_RED);
    }

    // If that part of the board is empty, then redraw the board there
    else if(map_array[x][y] == empty || 
            (player == white && map_array[x][y] == black_win) ||
            (player == black && map_array[x][y] == white_win)){
        redraw_board(tft, x, y, side);
    }
}

// Draws the cursor in a new position, depending on joystick movement
void move_cursor(Adafruit_ST7735 tft, int start_vertical, int start_horizontal,
                 int sensitivity, int * x, int * y, int radius, int side, 
                 int map_array[19][19], int player, int amount_slowed){

    // Read the current horizontal and vertical values from the joystick
    // These values will be between 0 and 1023
    int vertical = analogRead(JOYSTICK_VERT);
    int horizontal = analogRead(JOYSTICK_HORIZ);
 
    // Move cursor up if vertical analog change over a certain threshold
    // is detected, until the top edge of the map
    if(vertical < (start_vertical - sensitivity) && (*y) > 0) {
        decide(tft, (*x), (*y), radius, side, map_array, player);

        // Move the position of y up one by subtracting one
        --(*y);
    
        draw_cursor(tft, (*x), (*y), side);
    }
 
    // Move cursor down if vertical analog change over a certain threshold
    // is detected, until the bottom edge of the map
    if(vertical > (start_vertical + sensitivity) && (*y) < 18) {
        decide(tft, (*x), (*y), radius, side, map_array, player);
    
        // Move the position of y down one by adding one
        ++(*y);

        draw_cursor(tft, (*x), (*y), side);
    }
 
    // Move cursor left if horizontal analog change over a certain
    // threshold is detected, until the left edge of the map
    if(horizontal < (start_horizontal - sensitivity) && (*x) > 0) {
        decide(tft, (*x), (*y), radius, side, map_array, player);      
        // Move the position of x left one by subtracting one
        --(*x);
    
        draw_cursor(tft, (*x), (*y), side);
    }
 
    // Move cursor right if horizontal analog change over a certain
    // threshold is detected, until the right edge of the map
    if(horizontal > (start_horizontal + sensitivity) && (*x) < 18) {
        decide(tft, (*x), (*y), radius, side, map_array, player);
    
        // Move the position of x right one by adding one
        ++(*x);

        // Draw cursor at certain location
        draw_cursor(tft, (*x), (*y), side);
    }

    // Delay to slow down the move of cursor
    delay(amount_slowed);
}

// Draw winning moves for black and erase black's winning moves
void draw_white_win(Adafruit_ST7735 tft, int x, int y, int side, 
                    int map_array[19][19]){

    // If the value of map array is black_win or both_win, draw red 
    // rectangle as winning move hint.
    if(map_array[x][y] == white_win || map_array[x][y] == both_win){
        tft.fillRect(6*x+8, 6*y+8, side, side, ST7735_RED);
    }

    // If it's the current user doesn't put a piece to win, then redraw
    // the board at where the winning move hint was.
    else if(map_array[x][y] == black_win){
        redraw_board(tft, x, y, side);
    }
}


// Draw winning moves for black and erase white's winning moves 
void draw_black_win(Adafruit_ST7735 tft, int x, int y, int side, 
                    int map_array[19][19]){

    // If the value of map array is black_win or both_win, draw red 
    // rectangle as winning move hint.
    if(map_array[x][y] == black_win || map_array[x][y] == both_win){
        tft.fillRect(6*x+8, 6*y+8, side, side, ST7735_RED);
    }

    // If it's the current user doesn't put a piece to win, then redraw
    // the board at where the winning move hint was.
    else if(map_array[x][y] == white_win){
        redraw_board(tft, x, y, side);
    }
}
