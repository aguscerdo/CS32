//
//  Arena.hpp
//  Project 1 - Marinovic
//
//  Created by Agustin Marinovic on 1/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#ifndef Arena_h
#define Arena_h

#include <stdio.h>

#include "History.h"
#include "globals.h"

class Player;
class Robot;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    
    // Mutators
    bool   addRobot(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackRobotAt(int r, int c, int dir);
    bool   moveRobots();
    
    History& history();
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
    History m_history;
};


#endif /* Arena_h */
