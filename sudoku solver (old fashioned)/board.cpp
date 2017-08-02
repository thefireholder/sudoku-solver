//
//  board.cpp
//  sudoku solver (old fashioned)
//
//  Created by Sangjoon Lee on 7/11/17.
//  Copyright © 2017 Sangjoon Lee. All rights reserved.
//

#include "board.hpp"
#include <iostream>
using namespace std;


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
    initPotSets();
}

void Board::initPosCell()
//each cell in PosCell will be assigned the following:
//(little Endian)
//first 9 bit: (possible sudoku value: 1-9)
//      taken from surrounding row, col, box.
//      written even for the known values.
//next 7 bit: (sudoku index 0-80) 81 index total

{
    //following 3 arrays store numbers that are already taken across
    //row, col, box. these arrays will be used to find numbers
    //that are NOT taken for each cell (for first 9 bit in posCells)
    
    unsigned short rowPotential[9];
    unsigned short colPotential[9];
    unsigned short boxPotential[9];
    
    //start with all of them set to zero
    for (int i = 0; i < 9; i++)
    {
        //rowPotential[i] = 0; //this is done while init rowPotential
        colPotential[i] = 0;
        boxPotential[i] = 0;
    }
    
    //initialize row Potential
    for (int i = 0; i < 9; i++)
    {
        //or (|) all the element in a row and store it in rowPotential
        int sum = 0, i9 = i*9;
        for (int j = 0; j < 9; j++)
            sum |= cells[i9+j];
        
        rowPotential[i] = sum;
        
    }
    
    //initialize col Potential
    for (int i = 0; i < 9; i++)
    {
        //or (|) all the element in a col and store it in rowPotential
        int i9 = i*9;
        for (int j = 0; j < 9; j++)
            colPotential[j] |= cells[i9+j];
    }
    
    //initialize box Potential
    int cellIndex = 0;
    int boxIndex = 0;
    for (; boxIndex < 9; boxIndex += 3) //box Index rise every 3 rows
    {
        for (int i =0; i < 3; i++) //for each of that row
        {
            for (int j = 0; j < 3; j++) //there is 3 col (for each col:)
            {
                int temp = boxPotential[boxIndex + j];
                
                temp |= cells[cellIndex++];
                temp |= cells[cellIndex++];
                temp |= cells[cellIndex++];
                
                boxPotential[boxIndex + j] = temp;
            }
        }
    }
    
    //**use the row, col, box potential to store values that are NOT taken:
    
       //  for row & col
    for (int i = 0; i < 9; i++) //for row, col
        for (int j = 0; j < 9; j++)
            posCells[i*9+j] = rowPotential[i] | colPotential[j];
    
       //  for box
    int ipos = 0; //for box
    for (int i = 0; i < 9; i+=3) //for every 3 rows
        for (int j = 0; j < 3; j++) //for a row in those 3 rows
            for (int k = 0; k < 3; k++) //for 3 cols in each row
            {
                int temp = boxPotential[i + k];
                posCells[ipos++] |= temp;
                posCells[ipos++] |= temp;
                posCells[ipos++] |= temp;
            }
    
        //inverse the result to find values that are NOT taken
    
    short mask = 511;
    for (int i = 0; i < 81; i++)
        posCells[i] ^= mask;
    
    
    //** assign zero to values that are known AND store index
    //May not be necessary. address work as index, cells shows unknown
//    for (int i = 0; i < 81; i++)
//    {
//        if (cells[i]) posCells[i] = 0;
//        posCells[i] |= (i << 9);
//    }
    
    cellStatus(8,5); cout << endl;
    cellStatus(0,2); cout << endl;
    cellStatus(1,0); cout << endl;
}


void Board::initPotSets()
//initialize those unorderd set to hold address to posCells for cells of
//which value is unknown
{
    //store address of unknown sudoku cells into row & col Sets.
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int index = i*9+j;
            if (!cells[index])
            {
                rowPotSet[i].insert(&cells[index]);
                colPotSet[j].insert(&cells[index]);
            }
        }
    }
    
    //store address of unkown cells into box Sets
    int ipos = -1; //for box
    for (int i = 0; i < 9; i+=3) //for every 3 rows
        for (int j = 0; j < 3; j++) //for a row in those 3 rows
            for (int k = 0; k < 3; k++) //for 3 cols in each row
            {
                if (!cells[++ipos])
                    boxPotSet[i+k].insert(&posCells[ipos]);
                if (!cells[++ipos])
                    boxPotSet[i+k].insert(&posCells[ipos]);
                if (!cells[++ipos])
                    boxPotSet[i+k].insert(&posCells[ipos]);
            }
}

int Board::calculate()
{
    //start with box since it's easiest
    for (auto && s : boxPotSet[0])
    {
        cout << s << "|";
        cout << *s<< "*";
        cout << endl;
    }
    cout << endl;
    
    //check if a cell in a set contains a value that is unique to itself
    for (unsigned short testCase = 1; testCase < 256; testCase <<= 1)
    {
        //future: see if rejecting known 0 makes it faster or not
        int count = 0;
        for (auto && s : boxPotSet[0])
            count += !!(*s & testCase);
        
        //if it is unique
        if (count == 1)
        {
            //find the address
            unsigned short* address;
            for (auto && s : boxPotSet[0])
            {
                if (*s & testCase)
                {
                    address = s;
                    cout << address << ": " << testCase << endl;
                    break;
                }
            }
            
            //fix Cells & posCells
            //remove that value from cells in all the relevant potSets
            //remove that address from all the potSets
        }
    }
    
    
    return -1;
}


void Board::printCells()
{
    for (int i = 0; i < 81;)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << b2d(cells[i]) << b2d(cells[i+1]) << b2d(cells [i+2]);
            cout << ' '<< b2d(cells[i+3]) << b2d(cells[i+4]) << b2d(cells [i+5]);
            cout << ' ' << b2d(cells[i+6]) << b2d(cells[i+7]) << b2d(cells [i+8]);
            cout << endl;
            
            i+=9;
        }
        cout << endl;
    }
}

void Board::cellStatus(short r, short c)
{
    unsigned short content = cells[9*r+c];
    cout << "cell[" << r << "][" << c << "]: ";
    if (content)
        cout << b2d(content) << endl;
    else
    {
        content = posCells[9*r+c];
        unsigned short nChoice = bitCount9(content);
        cout << "unknown" << endl;
        cout << "out of "<< nChoice << " possible choice:";
        
        //print all choices
        int index = 1;
        for (int i = 1; i < 512; i *= 2)
        {
            if (content & i) cout << ' ' << index;
            index++;
        }
        
        cout << endl;
    }
}


unsigned short Board::bitCount9(unsigned short bit)
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
