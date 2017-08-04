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
//next 4 bit: (box index 0-8)

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
    
    //**use the row, col, box potential to store values that are already taken:
    
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
                int temp = boxPotential[i + k]; //boxPotential
                temp |= (i+k) << 9; //box index
                posCells[ipos++] |= temp;
                posCells[ipos++] |= temp;
                posCells[ipos++] |= temp;
            }
    
        //also store box index during (// for box) ^
    
    
    //**inverse the result to find values that are NOT taken
    
    unsigned short mask = 511;
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
                unsigned short *address = &posCells[index];
                short ibox = (posCells[index] >> 9) & 15;
                
                rowPotSet[i].insert(address);
                colPotSet[j].insert(address);
                boxPotSet[ibox].insert(address);
            }
        }
    }
    

}

int Board::calculate()
{
    int count, result;
    do
    {
        count = 0;
        //check if a cell in a set contains a value that is unique to itself
        //or a value that has bitCount = 1
        for (int i = 0; i < 9; i++)
            count += setInvestigation(boxPotSet[i]);
        
        for (int i = 0; i < 9; i++)
            count += setInvestigation(colPotSet[i]);
        
        for (int i = 0; i < 9; i++)
            count += setInvestigation(rowPotSet[i]);
    }
    while(count);
    
    return -1;
}

int Board::setInvestigation(unordered_set<unsigned short *> &potSet)
//investigate a given set for 2 case
//case 1: identify cells with potential value that is unique to that cell.
//case 2: identify cells with bitCount = 1
//such cells will be given that number as its true value. and that value
//(+address) will be removed from its column, row and box
//
//returns number of correctly identified cells (returns -1 if there is error)
{
    bool identified;
    int nidentified = 0;
    do{
        identified = false;
        
    //case 1:
        //testing 0-9 (bit wise) on all cells within a set
        for (unsigned short testCase = 1; testCase <= 256; testCase <<= 1)
        {
            //future: see if rejecting known 0 makes it faster or not
            int count = 0;
            for (auto && s : potSet)
                count += !!(*s & testCase);
            
            //if it is unique
            if (count == 1)
            {
                //find the address
                unsigned short* address;
                for (auto && s : potSet)
                {
                    if (*s & testCase)
                    {
                        address = s;
                        break;
                    }
                }
                
                //updates Cell with value (bit), (doesn't update posCell)
                //remove that value from cells in all the relevant potSets
                //remove that address from all the potSets
                int result = updateCell(address, testCase);
                
                //if the result is bad, must report
                if(result < 0)
                {
                    cout << "something's wrong after we updated address:" << address-posCells << "with value" << b2d(testCase);
                    return result;
                }
                
                //flag
                identified = true;
                nidentified++;
            }
        }
        
    //case 2:
        for (auto && s : potSet)
            if (bitCount9(*s)==1)
            {
                unsigned short bit = (*s)&511;
                int result = updateCell(s, bit);
                
                if(result < 0)
                {
                    cout << "something's wrong after we updated cell[" << s-posCells << "] with value " << b2d(bit) << endl;
                    return result;
                }
                
                //flag
                identified = true;
                nidentified++;
            }
    }
    while(identified);
    return nidentified;
}

int Board::updateCell(unsigned short *address, unsigned short bit)
//updates Cell with value (bit), (doesn't update posCell)
//remove that value from cells in all the relevant potSets
//remove that address from all the potSets
{
    //preparing necc info.
    unsigned short value = b2d(bit); //decimal value
    int index = (int)(address - posCells); //0-80
    int irow = index/9; //0-8
    int icol = index%9; //0-8
    int ibox = (*address >> 9) & 15; //0-8
    
    //update Cell
    cells[index] = bit;
    
    //remove that address from all the potSets
    rowPotSet[irow].erase(address);
    colPotSet[icol].erase(address);
    boxPotSet[ibox].erase(address);
    
    //remove that value from cells in all the relevant potSets
    unsigned short mask = ~bit;
    for(auto &&s : rowPotSet[irow])
        *s &= mask;
    for(auto &&s : colPotSet[icol])
        *s &= mask;
    for(auto &&s : boxPotSet[ibox])
        *s &= mask;
    
    //check (this section can be remove in the future)
    cout << "cell[" << irow << "][" << icol << "]: updated " << value <<endl;
    
    bool result = repetitionCheck(value, irow, icol, ibox);
    if(result) {cout<< "repeated" <<endl; return -1;}
//    else cout << "not repeated"<< endl;
    result = emptyAddressCheck(irow, icol, ibox);
    if(result) {cout<< "empty address found" <<endl; return -1;}
//    else cout << "no empty address"<< endl;
    
    return 0;
    
}

bool Board::repetitionCheck(int value, int row, int col, int box)
//see if a value is repeated more than once in given row, col ,box
//if repeated: true
{
    //row check
    int counter = 0;
    for (int i = 0; i < 9; i++)
        counter += (cells[row*9+i] == value);
    if (counter > 1) return true;
    
    //col check
    counter = 0;
    for (int i = 0; i < 81; i+=9)
        counter += (cells[col+i]==value);
    if (counter > 1) return true;
    
    //box check
    counter = 0;
    int threshold;
    
    if (box < 3){threshold=27; box=box*3;}
    else if (box < 6){threshold=54; box=(box-3)*3;}
    else {threshold=81; box = (box-6)*3;}
    
    for (int i = threshold-27; i < threshold; i+=9)
    {
        counter += (cells[box+i]==value);
        counter += (cells[box+i+1]==value);
        counter += (cells[box+i+2]==value);
    }
    if (counter > 1) return true;
    
    return false;
}

bool Board::emptyAddressCheck(int row, int col, int box)
//see if PotSets with following index contain address with value of first 1-9th little endian values as all 0s
//if empty Address found: true
{
    for(auto &&s : rowPotSet[row])
        if(!((*s)&511)) return true;
    for(auto &&s : colPotSet[col])
        if(!((*s)&511)) return true;
    for(auto &&s : boxPotSet[box])
        if(!((*s)&511)) return true;
    return false;
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

void Board::cellStatus(unsigned short *posCellptr)
{
    int index = int(posCellptr - posCells);
    unsigned short content = cells[index];
    cout << "cell[" << index/9 << "][" << index%9 << "]: ";
    if (content)
        cout << b2d(content) << endl;
    else
    {
        content = *posCellptr;
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
