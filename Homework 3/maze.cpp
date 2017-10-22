//
//  maze.cpp
//  Homework 3
//
//  Created by Agustin Marinovic on 2/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cassert>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = ' ';
    
    if (sr > 0 && maze[sr-1][sc] == '.')      //NORTH
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    if (sr+1 < nRows && maze[sr+1][sc] == '.')      //South
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    if (sc+1 < nCols && maze[sr][sc+1] == '.')      //East
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    if (sc > 0 && maze[sr][sc-1] == '.')      //West
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    
    return false;
}
