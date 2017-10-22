//
//  Player.hpp
//  Project 1 - Marinovic
//
//  Created by Agustin Marinovic on 1/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
//#include "Arena.h"

class Arena;

class Player
{
public:
    // Constructor
    Player(Arena *ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;
    
    // Mutators
    void   stand();
    void   moveOrAttack(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
};




#endif /* Player_hpp */
