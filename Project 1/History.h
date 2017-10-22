//
//  History.hpp
//  Project 1 - Marinovic
//
//  Created by Agustin Marinovic on 1/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#ifndef History_h
#define History_h



#include <stdio.h>
//#include "Arena.h"
#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    int m_rows;
    int m_cols;
    int m_grid[MAXROWS+1][MAXCOLS+1];
    
};


#endif /* History_hpp */
