#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Actor.h"
#include <vector>
#include <list>

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr);
    virtual ~Actor();
        
    virtual void doSomething() = 0;
    virtual void annoy(int power, bool boulder) {};
    void setDead();
    bool isDead() const;
    StudentWorld* getWorld();
    int getTick() const {return m_ticks;}
    void setTick(int nTicks) {m_ticks = nTicks;}
    virtual bool isBoulder() const {return false;}
    virtual bool isHuman() const {return false;}
    virtual bool isHard() const {return false;}
    virtual void leaveTheField() {}
    virtual bool leaving() const {return false;}
    virtual void bribeHard() {}
    virtual void stun(int num) {}
    //virtual void recordMove() {}
    virtual void leaveTheFieldRightNow() {}
    
private:
    int m_ticks;
    StudentWorld* m_world;
    bool m_dead;
};

//----------------------------------------------------//

class NonHuman: public Actor
{
public:
    NonHuman(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr);
    virtual ~NonHuman();

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
    void doSomething() {}
};

//----------------------------------------------------//

class Boulder: public NonHuman
{
public:
    Boulder(int x, int y, StudentWorld*ptr);
    ~Boulder();
    
    void doSomething();
    bool isBoulder() const {return true;}
    
private:
    bool m_stable;
};

//----------------------------------------------------//

class Squirt: public NonHuman
{
public:
    Squirt(int x, int y, Direction dir, StudentWorld* ptr);
    ~Squirt() {}
    
    void doSomething();
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

class WaterPool: public Goodies
{
public:
    WaterPool(int x, int y, StudentWorld* ptr);
    ~WaterPool() {}
    
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
    bool isHuman() const {return true;}
    
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
    void adjustGold(int amount) {m_gold += amount;}
    int goldCount() const {return m_gold;}
    void adjustWater(int amount) {m_water += amount;}
    int waterCount() const {return m_water;}
    void adjustSonar(int amount) {m_sonar += amount;}
    int sonarCount() const {return m_sonar;}
    
private:
    int m_sonar;
    int m_gold;
    int m_water;
};

//----------------------------------------------------//

class Protester: public Human
{
public:
    Protester(StudentWorld* ptr, int image = IID_PROTESTER, int health = 5);
    virtual ~Protester();
    
    Direction chooseD(Direction avoid1 = none, Direction avoid2 = none);
    virtual void doSomething();
    void leaveTheField() {leavingField = true;}
    bool shoutProtest();
    bool checkSquareDirty(int x, int y);
    Actor::Direction lineWithFrack();
    bool leaving() const {return leavingField;}
    bool intersection();
    virtual void stun(int num) {waitStatus = num + waitTicks;}
    void recordMove();
    void leaveTheFieldRightNow();
    
private:
    struct Coord
    {
        Coord(int x0, int y0) {x = x0; y = y0;}
        Coord(int x0, int y0, int md) {x = x0; y = y0; deep = md;}
        int x;
        int y;
        int deep;
        
        bool compare (Coord other)
        {
            return (other.x == x && other.y == y);
        }
    };
    std::list<Coord*> path;
    int dirChangeTicks;
    int stepsLeft;
    Direction dirToMove;
    bool leavingField;
    int waitTicks;
    int waitStatus;
    int shoutWait;
    bool notShouted;
    
};

//----------------------------------------------------//

class HardCore: public Protester
{
public:
    HardCore(StudentWorld* ptr);
    ~HardCore() {}
    
    void doSomething();
    void bribeHard();
    bool isHard() const {return true;}
    void createMaze (char maze[][64]);
    bool solveMazeinRange (int sx, int sy, int ex, int ey, char map[][64], int& maxDepth, int M = 64*64);
    bool mazeSquareCheck (int x, int y, char maze[][64]);
    bool findPath(int x, int y, int ex, int ey, char maze[][64], int m, int depth = 0, Direction di = none);
    void moveCloser1313(char maze[][64]);
    void stun(int num) {waitStatus = num;}
    
private:
    struct Coord
    {
        Coord(int x0, int y0) {x = x0; y = y0;}
        Coord(int x0, int y0, int md) {x = x0; y = y0; deep = md;}
        int x;
        int y;
        int deep;
        
        bool compare (Coord other)
        {
            return (other.x == x && other.y == y);
        }
    };
    
    int dirChangeTicks;
    int stepsLeft;
    int waitTicks;
    int waitStatus;
    int shoutWait;
    bool notShouted;
};

#endif // ACTOR_H_
