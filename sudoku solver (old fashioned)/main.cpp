//
//  main.cpp
//  sudoku solver (old fashioned)
//
//  Created by Sangjoon Lee on 7/11/17.
//  Copyright © 2017 Sangjoon Lee. All rights reserved.
//

#include <iostream>
#include "board.hpp"
using namespace std;

//global constants
const int C_SIZE = 81;
const int ROW_SIZE = 9;
const int COL_SIZE = 9;
const int BOX_SIZE = 9;




int main() {
    
    //board set up
    unsigned short board[C_SIZE] = {2,0,0,  8,0,4,  0,0,6,
                                    0,0,6,  0,0,0,  5,0,0,
                                    0,7,4,  0,0,0,  9,2,0,
        
                                    3,0,0,  0,4,0,  0,0,7,
                                    0,0,0,  3,0,5,  0,0,0,
                                    4,0,0,  3,6,0,  0,0,9,
        
                                    0,1,9,  0,0,0,  7,4,0,
                                    0,0,8,  0,0,0,  2,0,0,
                                    5,0,0,  6,0,8,  0,0,1 };
    
    Board bd;
    bd.initialize(board, ROW_SIZE, COL_SIZE);
    bd.calculate();
    cout << endl;
    bd.printCells();
}

void beta(){
    char rowIndex[ROW_SIZE];
    char colIndex[COL_SIZE];
    char boxIndex[BOX_SIZE];
    
    //initial sums
    for(int i = 0; i < ROW_SIZE; i++)
    {
        rowIndex[i] = 0;
        colIndex[i] = 0;
        boxIndex[i] = 0;
    }
    
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            //if (board[9*i+j])
            {
                rowIndex[i]++;
                colIndex[j]++;
            }
        }
    }
}
