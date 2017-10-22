//
//  History.cpp
//  Project 1 - Marinovic
//
//  Created by Agustin Marinovic on 1/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    for(int i(0); i < nRows && i < MAXROWS; i++) //Create a grid and fill it with '0', aka number of deaths on the spot
    {
        for(int j(0); j < nCols && j < MAXCOLS; j++)
        {
            m_grid[i][j] = 0;
        }
    }
}

bool History::record(int r, int c)
{
    if (r < 1 || c < 1 || r > m_rows || c > m_cols) //If not a valid pos return false
        return false;
    
    m_grid[r-1][c-1]++; //Add 1 to the number of deaths on the spot
    return true;
}


void History::display() const
{
    char letters;
    clearScreen();
    for(int i(0); i < m_rows; i++)
    {
        for(int j(0); j < m_cols; j++)
        {
            if (m_grid[i][j] == 0) //If no robots have been kiled, display '.'
                cout << '.';
            else
            {
                if (m_grid[i][j] < 26) //Write the correct letter (Z for 26 or more)
                    letters = m_grid[i][j] + 'A' - 1;
                else
                    letters = 'Z';
                cout << letters; //If 1, display 'A', keep inncreasing if more
            }
        }
        cout << endl;
    }
    cout << endl;
}