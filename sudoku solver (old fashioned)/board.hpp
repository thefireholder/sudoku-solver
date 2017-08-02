//
//  board.hpp
//  sudoku solver (old fashioned)
//
//  Created by Sangjoon Lee on 7/11/17.
//  Copyright © 2017 Sangjoon Lee. All rights reserved.
//

#ifndef board_hpp
#define board_hpp

#include <unordered_set>

class Board
{
public:
    void initialize(unsigned short* board_copy, int row, int col);
    //initialize
    
    void printCells();
    //print the whole sudoku page stored in cells
    
    int calculate();
    //solve sudoku, -2 : contradiction, -1 : cannot solve
    
    void cellStatus(short r, short c);
    //show a cell's status
    
private:
    
    
    void initPosCell();
    //initialize posCell from cell
    
    void initPotSets();
    //initialize potential sets from posCells and cells
    //store only the unknowns in them
    
    unsigned short bitCount9(unsigned short bit);
    //count how many bits are on in the first 9 bit.
    
    unsigned short b2d(unsigned short bit);
    //convert bitWise to decimal (Ex: 4 -> 3, 16-> 5)
    
    unsigned short cells[81];
    //actual value in decimal (zero means unknown)
    
    unsigned short posCells[81];
    //read initPosCell description
    

    std::unordered_set<unsigned short*> rowPotSet[9];
    std::unordered_set<unsigned short*> colPotSet[9];
    std::unordered_set<unsigned short*> boxPotSet[9];
    
    
};

#endif /* board_hpp */
