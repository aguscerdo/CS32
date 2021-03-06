//
//  utilities.hpp
//  Project 1 - Marinovic
//
//  Created by Agustin Marinovic on 1/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#ifndef globals_h
#define globals_h

//#include <iostream>
//#include <string>
//#include <random>
//#include <utility>
//#include <cstdlib>


#include <stdio.h>

const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 30;             // max number of columns in the arena
const int MAXROBOTS = 100;          // max number of robots allowed
const int INITIAL_ROBOT_HEALTH = 2;

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

int randInt(int min, int max);

void clearScreen();

int decodeDirection(char dir);

#endif /* globals_h */
