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
    //0: solve sudoku, -2 : contradiction, -1 : cannot solve
    
    void cellStatus(short r, short c);
    void cellStatus(unsigned short *posCellptr);
    //show a cell's status
    
private:
    void initPosCell();
    //initialize posCell from cell
    
    void initPotSets();
    //initialize potential sets from posCells and cells
    //store only the unknowns in them
    
    bool initContradictionTest();
    //see if there is contradiction in the given sudoku (before solving)
    
    int setInvestigation(std::unordered_set<unsigned short*> &potSet);
    //investigate a potSets and update Cell accordingly,
    //return the number of correctly updated cell.
    //note: if found, calls updateCell to mutates posCells & other potSets
    
    int updateCell(unsigned short* address, unsigned short bit);
    //[used only calculate()]: with the correct value (bit)
    //makes necc. change on cells, posCells, potSets for calculate to
    //work properly.
    //return -1 if set w/ empty address is found after updating; then
    //address will point to problematic address at hand
    //return -2 if row, col box has repetition of number after updating
    
    bool repetitionCheck(int bit, int row, int col, int box);
    //see if a value (decimal) is repeated more than once in given row, col ,box (true : repeated)
    
    bool emptyAddressCheck(int row, int col, int box);
    //see if PotSets with following index contain address with value of first 1-9th little endian values as all 0s. true : empty address
    
    unsigned short bitCount9(unsigned short bit);
    //count how many bits are on in the first 9 bit.
    
    unsigned short b2d(unsigned short bit);
    //convert bitWise to decimal (Ex: 4 -> 3, 16-> 5)
    
    unsigned short cells[81];
    //actual value in bits (zero means unknown)
    
    unsigned short posCells[81];
    //read initPosCell description

    int error;
    //way to update calculate() whether there is an error or not
    //introduced to make calculation faster...
    //0: no error, -1: emtpy address error, -2: repetition error
    //should this be on, calculate should stop and exit

    std::unordered_set<unsigned short*> rowPotSet[9];
    std::unordered_set<unsigned short*> colPotSet[9];
    std::unordered_set<unsigned short*> boxPotSet[9];
    
    
};

#endif /* board_hpp */
