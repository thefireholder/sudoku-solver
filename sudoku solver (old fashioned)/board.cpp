//
//  board.cpp
//  sudoku solver (old fashioned)
//
//  Created by Sangjoon Lee on 7/11/17.
//  Copyright © 2017 Sangjoon Lee. All rights reserved.
//

#include "board.hpp"
#include <iostream>


void Board::initialize(unsigned short* board_copy, int row, int col)
{
    //initizliaze cells
    for (int i = 0; i < row * 9; i +=9 )
    {
        for (int j = 0; j < col; j++)
        {
            unsigned short value = board_copy[i+j];
            
            if (value != 0)
                value = 1 << (value-1);
                
            board_copy[i+j] = value;
            cells[i+j] = value;
        }
    }
    
    initPosCell();
}

void Board::initPosCell()
{
    unsigned short rowPotential[9];
    unsigned short colPotential[9];
    unsigned short boxPotential[9];
    
    //start with all of them set to zero
    for (int i = 0; i < 9; i++)
    {
        rowPotential[i] = 0;
        colPotential[i] = 0;
        boxPotential[i] = 0;
    }
    
    //initialize rowPotential
    for (int i = 0; i < 9; i++)
    {
        //or (|) all the element in a row and store it in rowPotential
        int i9 = i * 9;
        for (int j = 0; j < 9; j++)
            rowPotential[i] |= cells[i9+j];
        
    }
    
    //initialize col Potential
    for (int i = 0; i < 9; i++)
    {
        //or (|) all the element in a col and store it in rowPotential
        int i9 = i*9;
        for (int j = 0; j < 9; j++)
            colPotential[j] |= cells[i9+j];
    }
    
    //initialize box Potential (incomplete)
    for (int k = 0; k < 9; k +=3)
    {
        for (int i = 0; i < 3; i++)
        {
            int i9 = i*3 + k;
            for (int j = 0; j < 3; j++)
            {
                int index = i9 + j*3;
                boxPotential[j+k] |= cells[index];
                boxPotential[j+k] |= cells[index+1];
                boxPotential[j+k] |= cells[index+2];
            }
        }
    }
    
    //check
    for (int i = 0; i < 9; i++)
        std::cout << boxPotential[i] << " ";

}

int Board::calculate()
{
    
    
    return -1;
}


void Board::printCells()
{
    for (int i = 0; i < 81;)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << b2d(cells[i]) << b2d(cells[i+1]) << b2d(cells [i+2]);
            std::cout << ' '<< b2d(cells[i+3]) << b2d(cells[i+4]) << b2d(cells [i+5]);
            std::cout << ' ' << b2d(cells[i+6]) << b2d(cells[i+7]) << b2d(cells [i+8]);
            std::cout << std::endl;
            
            i+=9;
        }
        std::cout << std::endl;
    }
}


unsigned short bitCount9(unsigned short bit)
{
    unsigned short temp = (bit & 85) + ((bit & 170) >> 1);
    temp = (temp & 51) + ((temp & 204) >> 2);
    temp = (temp & 15) + ((temp & 240) >> 4);
    temp += (bit & 256) >> 8;
    
    return temp;
}

unsigned short Board::b2d(unsigned short bit)
{
    switch(bit)
    {
        case 4: return 3;
        case 8: return 4;
        case 16: return 5;
        case 32: return 6;
        case 64: return 7;
        case 128: return 8;
        case 256: return 9;
        default:
            return bit;
    }
}
