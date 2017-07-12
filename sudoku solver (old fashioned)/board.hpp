//
//  board.hpp
//  sudoku solver (old fashioned)
//
//  Created by Sangjoon Lee on 7/11/17.
//  Copyright © 2017 Sangjoon Lee. All rights reserved.
//

#ifndef board_hpp
#define board_hpp

class Board
{
public:
    void initialize(unsigned short* board_copy, int row, int col);
    //initialize
    
    void printCells();
    //print the whole sudoku page stored in cells
    
    int calculate();
    //solve sudoku, -2 : contradiction, -1 : cannot solve 
    
private:
    
    
    void initPosCell();
    //initialize posCell from cell
    
    unsigned short bitCount9(unsigned short bit);
    //count how many bits are on in the first 9 bit.
    
    unsigned short b2d(unsigned short bit);
    //convert bitWise to decimal (Ex: 4 -> 3, 16-> 5)
    
    
    unsigned short cells[81];
    
    unsigned short posCells[81];
    
    unsigned short rowPot[9];
    unsigned short colPot[9];
    unsigned short boxPot[9];
};

#endif /* board_hpp */
