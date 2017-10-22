//
//  main.cpp
//  Homework 2
//
//  Created by Agustin Marinovic on 1/27/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

void print (string maze[])
{
    for (int i = 0; maze[i][0] == 'X'; i++)
    {
        cerr << maze[i] << endl;
    }
    
    cerr << endl << endl << endl;
}



bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> path;
    path.push(Coord(sr,sc));
    maze[sr][sc] = 'S';
    print(maze);
    

    while( ! path.empty())
    {
        
        Coord actual(path.top());
        //cerr << " - " << actual.r() << ", " << actual.c() << endl;
        path.pop();
        if (actual.r() == er && actual.c() == ec)
        {
            maze[er][ec] = 'E';
            print(maze);
            return true;
        }
        
        if ((actual.r() - 1) >= 0 && maze[actual.r() - 1][actual.c()] == '.')        //NORTH
        {
            maze[actual.r()-1][actual.c()] = ' ';
            path.push(Coord(actual.r() - 1, actual.c()));
        }
        if ((actual.r() + 1) < nRows && maze[actual.r() + 1][actual.c()] == '.')        //SOUTH
        {
            maze[actual.r()+1][actual.c()] = ' ';
            path.push(Coord(actual.r() + 1, actual.c()));
        }
        if ((actual.c() + 1) < nCols && maze[actual.r()][actual.c() + 1] == '.')        //EAST
        {
            maze[actual.r()][actual.c() + 1] = ' ';
            path.push(Coord(actual.r(), actual.c()+1));
        }
        if ((actual.c() - 1) >= 0 && maze[actual.r()][actual.c() - 1] == '.')        //WEST
        {
            maze[actual.r()][actual.c()-1] = ' ';
            path.push(Coord(actual.r(), actual.c() - 1));
        }
        
        //print(maze);
    }
    return false;
    
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
