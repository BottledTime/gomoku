// This file is corresponding to the math.h file
// This file describes the functions about the main rules of 
// the game.

#include <Arduino.h>
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "drawing.h"
#include "board_values.h"

// Store the relative values of certain location x, y to map array,
// depending on the player
// map_array = empty means nothing is there
// map_array = white_piece means a white piece is there
// map_array = black_piece means a black piece is there
// map_array = white_win means this is a winning spot for white
// map_array = black_win means this is a winning spot for black
// map_array = both_win means this is a winning spot for both
void store(int player, int x, int y, int map_array[19][19]){

    // When it's black's move
    if(player == black){

        // If there's nothing at the location x, y change the value 
        // to black_win
        if(map_array[x][y] == empty){
            map_array[x][y] = black_win;
        }

        // If it is already a winning spot for white
        // change the value to both_win
        else if(map_array[x][y] == white_win){
            map_array[x][y] = both_win;
        }
    }
    
    // When it's the move of white
    else if(player == white){

        // If there's nothing at the location x, y change the value
        // to white_win
        if(map_array[x][y] == empty){
            map_array[x][y] = white_win;
        }

        // If it is already a winning spot for black
        // change the value to both_win
        else if(map_array[x][y] == black_win){
            map_array[x][y] = both_win;
        }
    }
}

// Check for a winning move in the vertical direction
void check_vertical(int player, int x, int map_array[19][19]){

    // Find adjacent pieces of same colour in vertical direction.
    // Find whether there's a blank that can make 5 pieces in a 
    // column and use store function to change the value of map
    // array.
    for(int j=0; j<=15; ++j){

        // Keep checking adjacent vertical pieces of same colour
        if(map_array[x][j] == player){
            ++j;
            if(map_array[x][j] == player){
                ++j;
                if(map_array[x][j] == player){
                    ++j;
                    if(map_array[x][j] == player){

                        // When there are four adjacent pieces 
                        // Use store function to check the two
                        // ends and stores relative values to 
                        // the map array.
                        if(j >= 4){
                            store(player, x, j-4, map_array);
                        }
                        if(j < 18){
                            store(player, x, j+1, map_array);
                        }
                    }

                    // When there are three adjacent pieces, 
                    // check if it precedes a blank and a piece
                    // that can make 5. If so, change the value
                    // of blank in map array using "store".
                    else if(map_array[x][j+1] == player){
                        store(player, x, j, map_array);
                    }
                }

                // When there are two adjacent pieces, check if 
                // it precedes a blank and two pieces that can 
                // make 5. If so, change the value of blank in 
                // map array using "store".
                else if(map_array[x][j+1] == player){
                    if(map_array[x][j+2] == player){
                        store(player, x, j, map_array);
                    }
                }
            }

            // When there's one piece, check if it precedes a blank and 
            // three pieces. If so, change the value of blank 
            // in map array using "store".
            else if(map_array[x][j+1] == player){
                if(map_array[x][j+2] == player){
                    if(map_array[x][j+3] == player){
                        store(player, x, j, map_array);
                    }
                }
            }
        }
    }
}

// Check for a winning move in the horizontal direction
void check_horizontal(int player, int y, int map_array[19][19]){

    // Find adjacent pieces of same colour in horizontal direction.
    // Find whether there's a blank that can make 5 pieces in a 
    // row and use store function to change the value of map
    // array.
    for(int i=0; i<=15; ++i){

        // Keep checking adjacent pieces of same colour
        if(map_array[i][y] == player){
            ++i;
            if(map_array[i][y] == player){
                ++i;
                if(map_array[i][y] == player){
                    ++i;
                    if(map_array[i][y] == player){

                        // When there are four adjacent pieces 
                        // Use store function to check the two
                        // ends and store relative values to 
                        // the map array.
                        if(i >= 4){
                            store(player, i-4, y, map_array);
                        }
                        if(i < 18){
                            store(player, i+1, y, map_array);
                        }
                    }

                    // When there are three adjacent pieces, 
                    // check if it precedes a blank and a piece
                    // that can make 5. If so, change the value
                    // of blank in map array using "store".
                    else if(map_array[i+1][y] == player){
                        store(player, i, y, map_array);
                    }
                }

                // When there are two adjacent pieces, check if 
                // it precedes a blank and two pieces that can 
                // make 5. If so, change the value of blank in 
                // map array using "store".
                else if(map_array[i+1][y] == player){
                    if(map_array[i+2][y] == player){
                        store(player, i, y, map_array);                       
                    }
                }
            }

            // Check if there's one piece followed by a blank and 
            // three pieces. If so, change the value of blank 
            // in map array using "store".
            else if(map_array[i+1][y] == player){
                if(map_array[i+2][y] == player){
                    if(map_array[i+3][y] == player){
                        store(player, i, y, map_array);
                    }
                }
            }
        }
    }
}

// Find and store the beginning coordinate of the diagonal from 
// top left to bottom right which contains the cursor position. 
// Use pointer as input, and change the value it points to so that
// we can keep the value outside the function.
void find_diagonal1(int * x1, int * y1, int x, int y){

    (*x1) = x; // Set the value the pointer x1 points to as x
    (*y1) = y; // Set the value the pointer y1 points to as y

    // Find the beginning coordinate of the diagonal.
    if((*x1) >= (*y1)){
        (*x1) = (*x1) - (*y1);
        (*y1) = 0;
    }
    else if((*x1) < (*y1)){
        (*y1) = (*y1) - (*x1);
        (*x1) = 0;
    }
}

// Check for a winning move in the top left to bottom right diagonal
// direction
void check_diagonal1(int x1, int y1, int x, int y, int player, 
                     int map_array[19][19]){

    // Find and store the beginning coordinate of the diagonal
    find_diagonal1(&x1, &y1, x, y);
    
    // Keep checking adjacent pieces of same colour
    while(x1 <= 15 && y1 <= 15){
        if(map_array[x1][y1] == player){
            ++x1;
            ++y1;
            if(map_array[x1][y1] == player){
                ++x1;
                ++y1;
                if(map_array[x1][y1] == player){
                    ++x1;
                    ++y1;
                    if(map_array[x1][y1] == player){
                        
                        // When there are four adjacent pieces 
                        // Use store function to check the two
                        // ends and store relative values to 
                        // the map array.
                        if(x1 >= 4 && y1 >= 4){
                            store(player, x1-4, y1-4, map_array);
                        }
                        if(x1 < 18 && y1 < 18){
                            store(player, x1+1, y1+1, map_array);
                        }
                    }
                    
                    // When there are three adjacent pieces, 
                    // check if it precedes a blank and a piece
                    // that can make 5. If so, change the value
                    // of blank in map array using "store".
                    else if(map_array[x1+1][y1+1] == player){
                        store(player, x1, y1, map_array);
                    }
                }
                
                // When there are two adjacent pieces, check if 
                // it precedes a blank and two pieces that can 
                // make 5. If so, change the value of blank in 
                // map array using "store".
                else if(map_array[x1+1][y1+1] == player){
                    if(map_array[x1+2][y1+2] == player){
                        store(player, x1, y1, map_array);
                    }
                }
            }
            
            // Check if there's one piece followed by a blank and 
            // three pieces. If so, change the value of blank 
            // in map array using "store".
            else if(map_array[x1+1][y1+1] == player){
                if(map_array[x1+2][y1+2] == player){
                    if(map_array[x1+3][y1+3] == player){
                        store(player, x1, y1, map_array);
                    }
                }
            }
        }
        
        // Increment x1 and y1 to keep searching
        ++x1;
        ++y1;
    }
}

// Find and store the beginning coordinate of the diagonal from 
// top right to bottom left which contains the cursor position. 
// Use pointer as input, and change the value it points to so that
// we can keep the value outside the function.
void find_diagonal2(int * x1, int * y1, int x, int y){

    (*x1) = x; // Set the value the pointer x1 points to as x
    (*y1) = y; // Set the value the pointer y1 points to as y

    // Find the beginning coordinate of the diagonal.
    if((*x1) <= (18 - (*y1))){
        (*x1) = (*x1) + (*y1);
        (*y1) = 0;
    }
    else if((*x1) > (18 - (*y1))){
        (*y1) = (*y1) - (18 - (*x1));
        (*x1) = 18;
    }
}

// Check for a winning move in the top right to bottom left diagonal
// direction
void check_diagonal2(int x1, int y1, int x, int y, int player, 
                     int map_array[19][19]){

    // Find and store the beginning coordinate of the diagonal
    find_diagonal2(&x1, &y1, x, y);

    // Keep checking adjacent pieces of same colour
    while(x1 >= 3 && y1 <= 15){
        if(map_array[x1][y1] == player){
            --x1;
            ++y1;
            if(map_array[x1][y1] == player){
                --x1;
                ++y1;
                if(map_array[x1][y1] == player){
                    --x1;
                    ++y1;
                    if(map_array[x1][y1] == player){

                        // When there are four adjacent pieces 
                        // Use store function to check the two
                        // ends and stores relative values to 
                        // the map array.
                        if(x1 < 15 && y >= 4){
                            store(player, x1+4, y1-4, map_array);
                        }
                        if(x1 >= 1 && y1 < 18){
                            store(player, x1-1, y1+1, map_array);
                        }   
                    }

                    // When there are three adjacent pieces, 
                    // check if it precedes a blank and a piece
                    // that can make 5. If so, change the value
                    // of blank in map array using "store".
                    else if(map_array[x1-1][y1+1] == player){
                        store(player, x1, y1, map_array);
                    }
                }

                // When there are two adjacent pieces, check if 
                // it precedes a blank and two pieces that can 
                // make 5. If so, change the value of blank in 
                // map array using "store".
                else if(map_array[x1-1][y1+1] == player){
                    if(map_array[x1-2][y1+2] == player){
                        store(player, x1, y1, map_array);
                    }
                }
            }

            // Check if there's one piece followed by a blank and 
            // three pieces. If so, change the value of blank 
            // in map array using "store".
            else if(map_array[x1-1][y1+1] == player){
                if(map_array[x1-2][y1+2] == player){
                    if(map_array[x1-3][y1+3] == player){
                        store(player, x1, y1, map_array);
                    }
                }
            }
        }

        // Decrement x1 and increment y1 to keep searching
        --x1;
        ++y1;
    }
}

// Function that records the winning moves in four directions
void record_win_move(int player, int x, int y, int x1, int y1, 
                     int map_array[19][19]){
    check_vertical(player, x, map_array);
    check_horizontal(player, y, map_array);
    check_diagonal1(x1, y1, x, y, player, map_array);
    check_diagonal2(x1, y1, x, y, player, map_array);
}

