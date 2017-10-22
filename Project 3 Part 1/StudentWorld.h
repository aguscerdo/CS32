#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    //virtual unsigned int getLevel() const;
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    std::string statString() const;
    std::string intToStr(int n) const;
    
    bool clearDirt (int x, int y);
    void randAllocateBig(int& x, int& y);
    double distance(int x, int y, Actor* ptr);
    bool goodCreationD(int x, int y);
    bool boulThere (Actor::Direction d, int x, int y);
    bool isDirty (int x, int y) const;
    FrackMan* frackRef() {return player;}
    std::vector<Actor*> actorsRef() { return actors;}
    void reduceOil() {oilCount--;}
    void sonarReveal();
    void dropGold();
    bool greedyB(Actor* gold);
    void humanBoulder(Actor* ptrB);
    
private:
    int oilCount;
    std::vector<Actor*> actors;
    Dirt* dirty[64][64];
    FrackMan* player;
};

#endif // STUDENTWORLD_H_
