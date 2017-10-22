#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr, bool Human = false, bool isBoulder = false);
    virtual ~Actor();
        
    virtual void doSomething() = 0;
    
    virtual void annoy(int power, bool boulder) {};
    void setDead();
    bool isDead() const;
    StudentWorld* getWorld();
    int getTick() const {return m_ticks;}
    void setTick(int nTicks) {m_ticks = nTicks;}
    bool isBoulder() const {return m_boulder;}
    bool isHuman() const {return m_human;}
    virtual bool isHard() {return false;}
    virtual void leaveField() {}
    virtual void bribeHard() {}
    
    
private:
    bool m_boulder;
    bool m_human;
    int m_ticks;
    StudentWorld* m_world;
    bool m_dead;
};

//----------------------------------------------------//

class NonHuman: public Actor
{
public:
    NonHuman(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr, bool isBoulder = false);
    virtual ~NonHuman();

    virtual void doSomething() {}    //Do nothing by default
};

//----------------------------------------------------//

class Goodies: public NonHuman
{
public:
    Goodies(int image, int x, int y, unsigned int depth, StudentWorld* ptr, int score);
    virtual ~Goodies();
    
    bool itemProximity();
    
private:
    int m_score;
};

//----------------------------------------------------//


class Dirt: public NonHuman
{
public:
    Dirt(int x, int y, StudentWorld* ptr);
    ~Dirt();
};

//----------------------------------------------------//

class Boulder: public NonHuman
{
public:
    Boulder(int x, int y, StudentWorld*ptr);
    ~Boulder();
    
    void doSomething();
    
private:
    bool m_stable;
};

//----------------------------------------------------//

class Barrel: public Goodies
{
public:
    Barrel (int x, int y, StudentWorld* ptr);
    ~Barrel();
    
    void doSomething();
};

//----------------------------------------------------//

class Gold: public Goodies
{
public:
    Gold(int x, int y, StudentWorld* ptr, bool pickUp);
    ~Gold();
    
    void doSomething();
    
private:
    bool pickAble;
    int lifeTime;
};

//----------------------------------------------------//

class Sonar: public Goodies
{
public:
    Sonar(StudentWorld* ptr);
    ~Sonar();
    
    void doSomething();
};

//----------------------------------------------------//

class Human: public Actor
{
public:
    Human(int image, int x, int y, Direction d, int health, StudentWorld* ptr);
    virtual ~Human();
    
    virtual void doSomething() = 0;
    void annoy(int power, bool boulder);
    int health() const;
    
private:
    int m_health;
};

//----------------------------------------------------//

class FrackMan: public Human
{
public:
    FrackMan(StudentWorld* ptr);
    ~FrackMan();
    
    void doSomething();
    void adjustGold(int amount);
    int goldCount() const;
    void adjustWater(int amount);
    int waterCount() const;
    void adjustSonar(int amount);
    int sonarCount() const;
    void sonarWave();
    
private:
    int m_sonar;
    int m_gold;
    int m_water;
};

//----------------------------------------------------//

class Protester: public Human
{
public:
    Protester(StudentWorld* ptr, bool hard = false);
    ~Protester();
    
    int movesInD();
    void doSomething();
    bool isHard() {return m_hard;}
    void leaveField() {leavingField = true;}
    bool shoutProtest();
    
private:
    int stepsLeft;
    Direction dirToMove;
    bool leavingField;
    int waitTicks;
    int waitStatus;
    
    int shoutWait;
    bool notShouted;
    bool m_hard;
    
};

//----------------------------------------------------//

class HardCore: public Protester
{
public:
    HardCore(StudentWorld* ptr);
    void bribeHard();
    
};

#endif // ACTOR_H_
