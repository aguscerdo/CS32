#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <vector>
#include <cmath>
#include <queue>
#include <stack>

Actor::Actor(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr): GraphObject(image, x, y, d, size, depth)
{
    m_ticks = 0;
    m_world = ptr;
    setVisible(true);
    m_dead = false;
}

Actor::~Actor() {}

void Actor::setDead()
{
    m_dead = true;
}

bool Actor::isDead() const
{
    return m_dead;
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

//----------------------------------------------------------------------//

NonHuman::NonHuman(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr): Actor(image, x, y, d, size, depth, ptr)
{
}

NonHuman::~NonHuman() {}

//----------------------------------------------------------------------//

Dirt::Dirt(int x, int y, StudentWorld* ptr): NonHuman(IID_DIRT, x, y, right, 0.25, 3, ptr)
{
}

Dirt::~Dirt() {}

//----------------------------------------------------------------------//

Goodies::Goodies(int image, int x, int y, unsigned int depth, StudentWorld* ptr, int score): NonHuman (image, x, y, right, 1.0, depth, ptr)
{
    m_score = score;
    
}

Goodies::~Goodies() {}

bool Goodies::itemProximity()   //If the player is in distance 4, reveal, distance 3 then pick it up
{
    if (getWorld()->distance(this, getWorld()->frackRef()) <= 3)
    {
        setDead();
        setVisible(false);
        getWorld()->increaseScore(m_score);
        return true;
    }
    else if (getWorld()->distance(this, getWorld()->frackRef()) <= 4)
    {
        setVisible(true);
    }
    
    return false;
}

//----------------------------------------------------------------------//

Boulder::Boulder(int x, int y, StudentWorld* ptr): NonHuman(IID_BOULDER, x, y, down, 1, 1, ptr)
{
    getWorld()->clearDirt(x, y);
    m_stable = true;
    setVisible(true);
    
}

Boulder::~Boulder() {}

void Boulder::doSomething()
{
    if (isDead())
        return;
    
    if (m_stable)   //Stable, check if it can fall
    {
        bool free4all = true;
        for (int i = getX(); i <= getX()+3; i++)
        {
            if (getWorld()->isDirty(i, getY()-1))   //If there is at least one dirt underneath, set fall to false
            {
                free4all = false;
                break;
            }
        }
        if (free4all)
            m_stable = false;
    }
    else    //Unstable
    {
        if (getTick() < 30)     //If 30 ticks haven't ellapsed, wait
        {
            setTick(getTick()+1);
            if (getTick() == 30)
                getWorld()->playSound(SOUND_FALLING_ROCK);
        }
        else    //Wait ellapsed, fall
        {

            
            if (getWorld()->distance(this, getWorld()->frackRef()) <= 3)      //If it's in distance 3 of frackman, kill him
            {
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                getWorld()->frackRef()->setDead();
                return;
            }
            
            getWorld()->humanBoulder(this);     //Check if collides with human
            
            bool dirty = true;
            for (int i = getX(); i <= getX()+3; i++)
            {
                if (getWorld()->isDirty(i, getY()-1))   //If there is at least one dirt underneath, set to fall to false
                {
                    dirty = false;
                    break;
                }
            }
            
            if (getY() == 0 || ! dirty || getWorld()->boulThere(down, this))  //If it can't keep moving, remove
            {
                setVisible(false);
                setDead();
            }
            else    //Can keep moving, go down by one
            {
                moveTo(getX(), getY() -1);
            }
        }
    }
}
//----------------------------------------------------------------------//

Barrel::Barrel(int x, int y, StudentWorld* ptr): Goodies(IID_BARREL, x, y, 2, ptr, 1000)
{
    setVisible(false);
}

Barrel::~Barrel() {}

void Barrel::doSomething()
{
    if (isDead())
        return;
    
    if (itemProximity())    //Check proximity and reduce oil count if close enough
    {
        getWorld()->reduceOil();
        getWorld()->playSound(SOUND_FOUND_OIL);
    }
    
}

//----------------------------------------------------------------------//

Gold::Gold (int x, int y, StudentWorld* ptr, bool pickUp): Goodies(IID_GOLD, x, y, 2, ptr, 10)
{
    pickAble = pickUp;
    
    if (pickAble)       //Hidden state
        setVisible(false);
    else
    {
        setVisible(true);   //Player dropped state
        lifeTime = 100;
        setTick(0);
    }
}

Gold::~Gold()
{
    
}

void Gold::doSomething()
{
    if (isDead())
        return;
    
    if (pickAble)       //If hidden, wait for player to pick it
    {
        if (itemProximity())
        {
            getWorld()->frackRef()->adjustGold(1);
            getWorld()->playSound(SOUND_GOT_GOODIE);
        }
    }
    else        //Dropped by player
    {
        if (getTick() >= lifeTime)      //Too long out there, remove it
        {
            setDead();
            setVisible(false);
        }
        else
        {
            setTick(getTick()+1);       //Increase tick by 1
        }
        
        if (getWorld()->greedyB(this))  //Check if protester in proximity
        {
            setVisible(false);
            setDead();
        }
    }
}

//----------------------------------------------------------------------//

Sonar::Sonar(StudentWorld* ptr): Goodies(IID_SONAR, 0, 60, 2, ptr, 75)
{
    setVisible(true);
    int T = 300 - 10*(getWorld()->getLevel());
    if (100 < T)
        setTick(100);
    else
        setTick(T);
    
}

Sonar::~Sonar()
{
}

void Sonar::doSomething()
{
    if (isDead())
        return;
    
    if (itemProximity())
    {
        getWorld()->frackRef()->adjustSonar(2);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    else
    {
        if (getTick() <= 0)
        {
            setDead();
            setVisible(false);
        }
        else
            setTick(getTick()-1);
    }
    
}

//----------------------------------------------------------------------//

WaterPool::WaterPool(int x, int y, StudentWorld* ptr): Goodies(IID_WATER_POOL, x, y, 2, ptr, 100)
{
    setTick(100);
}

void WaterPool::doSomething()
{
    if (isDead())
        return;
    if (itemProximity())
    {
        getWorld()->frackRef()->adjustWater(5);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    else
    {
        if (getTick() <= 0)
        {
            setDead();
            setVisible(false);
        }
        else
            setTick(getTick()-1);
    }
    
    
}

//----------------------------------------------------------------------//

Squirt::Squirt(int x, int y, Direction dir, StudentWorld* ptr): NonHuman(IID_WATER_SPURT, x, y, dir, 1, 1, ptr)
{
    setTick(4);
}

void Squirt::doSomething()
{
    if (isDead())
        return;
    
    if (getWorld()->squirtMan(this))    //Check if it can annoy someone
    {
        setDead();
        setVisible(false);
        return;
    }
    
    if (getTick() == 0)
    {
        setDead();
        setVisible(false);
        return;
    }
    else
        setTick(getTick()-1);
    
    switch (getDirection())
    {
        case up:
            for (int k = getX(); k < getX()+4; k++)
            {
                if (getY()+1 > 60 || getWorld()->isDirty(k, getY()+5) || getWorld()->boulThere(getDirection(), this))
                {
                    setDead();
                    setVisible(false);
                    return;
                }
            }
            moveTo(getX(), getY()+1);
            break;
        case down:
            for (int k = getX(); k < getX()+4; k++)
            {
                if (getY()-1 < 0 || getWorld()->isDirty(k, getY()-1) || getWorld()->boulThere(getDirection(), this))
                {
                    setDead();
                    setVisible(false);
                    return;
                }
            }
            moveTo(getX(), getY()-1);
            break;
        case right:
            for (int k = getY(); k < getY()+4; k++)
            {
                if (getX()+1 > 60 || getWorld()->isDirty(getX()+5, k) || getWorld()->boulThere(getDirection(), this))
                {
                    setDead();
                    setVisible(false);
                    return;
                }
            }
            moveTo(getX()+1, getY());
            break;
        case left:
            for (int k = getY(); k < getY()+4; k++)
            {
                if (getX()-1 < 0 || getWorld()->isDirty(getX()-1, k) || getWorld()->boulThere(getDirection(), this))
                {
                    setDead();
                    setVisible(false);
                    return;
                }
            }
            moveTo(getX()-1, getY());
            break;
        default:;
    }
    
    
}


//----------------------------------------------------------------------//

Human::Human(int image, int x, int y, Direction d, int health, StudentWorld* ptr): Actor(image, x, y, d, 1, 0, ptr)
{
    m_health = health;
}

Human::~Human() {}

void Human::annoy(int power, bool boulder)
{
    if (leaving()) //Avoid increased score if protester is already leaving
        return;
    
    m_health -= power;
    
    if (this == getWorld()->frackRef())    //Frackman
    {
        if (m_health <= 0)  //Died :(
        {
            setDead();
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        }
        else
            getWorld()->playSound(SOUND_PLAYER_ANNOYED);
    }
    else     //Protester
    {
        if (m_health > 0)       //Still Alive
        {
            int N = 100 - getWorld()->getLevel()*10;
        
            if (N < 50)
                stun(-N);
            else
                stun(-50);
            
            getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        }
        else        //Dead x_x
        {
            leaveTheField();
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            
            if (boulder)    //Killed by boulder
            {
                getWorld()->increaseScore(500);
            }
            else    //Squirted to death ;)
            {
                if (isHard())
                    getWorld()->increaseScore(250);
                else
                    getWorld()->increaseScore(100);
            }
        }
    }
}

int Human::health() const
{
    return m_health;
}

//----------------------------------------------------------------------//

FrackMan::FrackMan(StudentWorld* ptr): Human(IID_PLAYER, 30, 60, right, 10, ptr)
{
    m_sonar = 1;
    m_gold = 0;
    m_water = 5;
}

FrackMan::~FrackMan()
{
}

void FrackMan::doSomething()
{
    if (isDead())      //Oh no!, no health :(
        return;
    
    int ch;
    if (getWorld()->getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                if (getDirection() == left && getX() > 0 && !getWorld()->boulThere(left, this))
                    moveTo(getX()-1, getY());
                else if (getDirection() != left)
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() == right && getX() < 60 && !getWorld()->boulThere(right, this))
                    moveTo(getX()+1, getY());
                else if (getDirection() != right)
                    setDirection(right);
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() == down && getY() > 0 && !getWorld()->boulThere(down, this))
                    moveTo(getX(), getY()-1);
                else if (getDirection() != down)
                    setDirection(down);
                break;
            case KEY_PRESS_UP:
                if (getDirection() == up && getY() < 60 && !getWorld()->boulThere(up, this))
                    moveTo(getX(), getY()+1);
                else if (getDirection() != up)
                    setDirection(up);
                break;
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case 'Z':
            case 'z':
                if (m_sonar > 0)
                    getWorld()->sonarReveal();
                break;
            case KEY_PRESS_TAB:
                if (m_gold > 0)
                {
                    adjustGold(-1);
                    getWorld()->dropGold();
                }
                break;
            case KEY_PRESS_SPACE:
                if (m_water > 0)
                {
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    adjustWater(-1);
                    getWorld()->allocateSquirt();
                }
                break;
        }
        
        if(getWorld()->clearDirt(getX(), getY()))
           getWorld()->playSound(SOUND_DIG);   //Checks if there is dirt in the player's sprite
    }
}

//----------------------------------------------------------------------//

Protester::Protester(StudentWorld* ptr, int image, int health): Human(image, 60, 60, left, health, ptr)
{
    int T = 3 - getWorld()->getLevel()/4;
    T = std::max(T, 0);
    waitTicks = T;
    leavingField = false;
    dirChangeTicks = 0;
    path.push_back(new Coord(getX(), getY()));
}

Protester::~Protester() {}

void Protester::doSomething()
{
    if (isDead())       //Dead, return
        return;
    
    if (waitStatus < waitTicks)     //Waiting ticks
    {
        waitStatus++;       //Gotta wait
        return;
    }
    else
    {
        waitStatus = 0;     //GTG
        if (dirChangeTicks > 0)
            dirChangeTicks--;
        
        if (shoutWait > 0)
            shoutWait--;
    }
    
    if (leavingField)       //Leaving field, dont even bother
    {
        leaveTheFieldRightNow();
        return;
    }
    
    if (shoutWait <= 0)     //Can shout
    {
        if (shoutProtest())     //Within radius 4 and facing f
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);    //Shouted
            getWorld()->frackRef()->annoy(2, false);
            shoutWait = 15;
            return;
        }
    }
    else if (getWorld()->distance(this, getWorld()->frackRef()) <= 4)
        return;
    
//    else if (getWorld()->distance(this, getWorld()->frackRef()) <= 4)
//    {
//        Actor* f = getWorld()->frackRef();
//        
//        if (getX() > f->getX())
//            setDirection(left);
//        else if (getX() < f->getX())
//            setDirection(right);
//        else if (getY() > f->getY())
//            setDirection(down);
//        else if (getY() < f->getY())
//            setDirection(up);
//        return;     //Close enough
//    }
    
    Direction align = lineWithFrack();      //Straight line from frack
    if (align != none)
    {
        setDirection(align);
        switch (align)
        {
            case right:
                moveTo(getX()+1, getY());
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            default:;
        }
        recordMove();
        stepsLeft = 0;
        return;
    }
    
    stepsLeft--;        //One big step, I get a little bit closer
    if (stepsLeft <= 0)     //New D
    {
        setDirection(chooseD());    //Change D
        stepsLeft = rand()%53 + 8;      //Move nSteps [8, 60]
    }
    
    if (intersection() && dirChangeTicks <= 0)    //Check if intersection
    {
        dirChangeTicks = 200;
        switch (getDirection()) //Perpendicular Direction and new nSteps
        {
            case up:
            case down:
                setDirection(chooseD(up, down));
                stepsLeft = rand()%53 + 8;
                break;
            case left:
            case right:
                setDirection(chooseD(right, left));
                stepsLeft = rand()%53 + 8;
                break;
            default:;
        }
    }
    
    switch (getDirection())     //One big step, I move a little bit closer
    {
        case up:
            if ( getY() < 60 && ! checkSquareDirty(getX(), getY()+1) && !getWorld()->boulThere(up, this))
                moveTo(getX(), getY()+1);
            else
                stepsLeft = 0;
            break;
        case down:
            if ( getY() > 0 && ! checkSquareDirty(getX(), getY()-1) && !getWorld()->boulThere(down, this))
                moveTo(getX(), getY()-1);
            else
                stepsLeft = 0;
            break;
        case left:
            if ( getX() > 0 && ! checkSquareDirty(getX()-1 , getY()) && !getWorld()->boulThere(left, this))
                moveTo(getX()-1, getY());
            else
                stepsLeft = 0;
            break;
        case right:
            if ( getX() < 60 && ! checkSquareDirty(getX()+1 , getY()) && !getWorld()->boulThere(right, this))
                moveTo(getX()+1, getY());
            else
                stepsLeft = 0;
            break;
        default:
            stepsLeft = 0;
    }
    recordMove();

}

Actor::Direction Protester::lineWithFrack()
{
    int frackX = getWorld()->frackRef()->getX();
    int frackY = getWorld()->frackRef()->getY();
    
    bool straightLine = true;
    
    if (getX() < frackX && (getY() <= frackY+4 && getY() >= frackY-4))    //Straight right
    {
        for (int x = getX(); x != frackX; x++)
        {
            if (checkSquareDirty(x, getY()) || getWorld()->boulThere(right, x, getY()))
            {
                straightLine = false;
                break;
            }
        }
        if (straightLine)
            return right;
    }
    else if (getX() > frackX && (getY() <= frackY+4 && getY() >= frackY-4))   //Straight left
    {
        for (int x = getX(); x != frackX; x--)
        {
            if (checkSquareDirty(x, getY()) || getWorld()->boulThere(left, x, getY()))
            {
                straightLine = false;
                break;
            }
        }
        if (straightLine)
            return left;
    }
    else if (getY() < frackY && (getX() >= frackX-4 && getX() <= frackX+4))   //Stright up
    {
        for (int y = getY(); y != frackY; y++)
        {
            if (checkSquareDirty(getX(), y) || getWorld()->boulThere(up, getX(), y))
            {
                straightLine = false;
                break;
            }
        }
        if (straightLine)
            return up;
    }
    else if (getY() > frackY && (getX() >= frackX-4 && getX() <= frackX+4))   //Stright down
    {
        for (int y = getY(); y != frackY; y--)
        {
            if (checkSquareDirty(getX(), y) || getWorld()->boulThere(down, getX(), y))
            {
                straightLine = false;
                break;
            }
        }
        if (straightLine)
            return down;
    }
    return none;
}

bool Protester::shoutProtest()
{
    int fX = getWorld()->frackRef()->getX();
    int fY = getWorld()->frackRef()->getY();
    
    if(getWorld()->distance(this, getWorld()->frackRef()) <= 4)
    {
        if (getWorld()->distance(this, getWorld()->frackRef()) == 0)
            return true;
        
        if (getX() < fX && (getY() >= fY-4 && getY() <= fY+4))
        {
            if (getDirection() == right)
                return true;
            else
                setDirection(right);
        }
        else if (getX() > fX && getY() >= fY-4 && getY() <= fY+4)
        {
            if (getDirection() == left)
                return true;
            else
                setDirection(left);
        }
        else if (getY() < fY && (getX() >= fX-4 && getX() <= fX+4))
        {
            if (getDirection() == up)
                return true;
            else
                setDirection(up);
        }
        else if (getY() > fY && getX() >= fX-4 && getX() <= fX+4)
        {
            if (getDirection() == down)
                return true;
            else
                setDirection(down);
        }
    }
    
    return false;
}

Actor::Direction Protester::chooseD(Direction avoid1, Direction avoid2)
{
    int r;
    
    do
    {
        r = rand()%4;
        
        if (r == 0 && avoid1 != up && avoid2 != up)
        {
            if ( getY() < 60 && ! checkSquareDirty(getX(), getY()+1) && !getWorld()->boulThere(up, this))
                return up;
        }
        else if (r == 1 && avoid1 != down && avoid2 != down)
        {
            if ( getY() > 0 && ! checkSquareDirty(getX(), getY()-1) && !getWorld()->boulThere(down, this))
                return down;
        }
        else if (r == 2 && avoid1 != left && avoid2 != left)
        {
            if ( getX() > 0 && ! checkSquareDirty(getX()-1 , getY()) && !getWorld()->boulThere(left, this))
                return left;
        }
        else if (r == 3 && avoid1 != right && avoid2 != right)
        {
            if ( getX() < 60 && ! checkSquareDirty(getX()+1 , getY()) && !getWorld()->boulThere(right, this))
                return right;
        }
    } while (true);
    
}

bool Protester::checkSquareDirty(int x, int y)
{
    for (int x0 = x; x0 < x+4; x0++)
        for (int y0 = y; y0 < y+4; y0++)
            if (getWorld()->isDirty(x0, y0))
                return true;
    
    return false;
}

bool Protester::intersection()
{
    bool paths[4] = {false, false, false, false};
    
    if ( getY() < 60 && ! checkSquareDirty(getX(), getY()+1) && !getWorld()->boulThere(up, this))   //Check UP
        paths[0] = true;
    if ( getY() > 0 && ! checkSquareDirty(getX(), getY()-1) && !getWorld()->boulThere(down, this))  //Check down
        paths[1] = true;
    if ( getX() > 0 && ! checkSquareDirty(getX()-1 , getY()) && !getWorld()->boulThere(left, this)) //Check left
        paths[2] = true;
    if ( getX() < 60 && ! checkSquareDirty(getX()+1 , getY()) && !getWorld()->boulThere(right, this))   //Check right
        paths[3] = true;
    
    return (paths[0] || paths[1]) && (paths[2] || paths[3]);
}

void Protester::recordMove()
{
    Coord c(getX(), getY());
    bool gate = false;
    
    if ( ! path.empty())
    {
        std::list<Coord*>::iterator k = path.end();
        k--;
        
        while (k != path.begin())
        {
            if (c.compare(**k))
            {
                gate = true;        //I've been here before
                break;
            }
            k--;
        }
        
        if (gate)       //Pop all items after actual position
        {
            while (! c.compare(*path.back()))
            {
                path.pop_back();
            }
            
            return;
        }
    }
    //This is a new position, much much interesting
    
    path.push_back(new Coord(getX(), getY()));
    
}

void Protester::leaveTheFieldRightNow()
{
    if ( ! path.empty())
    {
        int x = path.back()->x;
        int y = path.back()->y;
        
        if (getX() < x)
            setDirection(right);
        else if (getX() > x)
            setDirection(left);
        else if (getY() < y)
            setDirection(up);
        else if (getY() > y)
            setDirection(down);
        
        path.pop_back();
        moveTo(x, y);
    }
    if (getX()==60 && getY()==60)
        setDead();
}

//-----------------------------------------------------------------------//

HardCore::HardCore(StudentWorld* ptr): Protester(ptr, IID_HARD_CORE_PROTESTER, 20)
{
    int T = 3 - getWorld()->getLevel()/4;
    T = std::max(T, 0);
    waitTicks = T;
    dirChangeTicks = 0;
    setVisible(true);
}

void HardCore::doSomething()
{
    if (isDead())       //Dead, return
        return;
    
    if (waitStatus < waitTicks)     //Waiting ticks
    {
        waitStatus++;       //Gotta wait
        return;
    }
    else
    {
        waitStatus = 0;     //GTG
        if (dirChangeTicks > 0)
            dirChangeTicks--;
        
        if (shoutWait > 0)
            shoutWait--;
    }
    
    if (leaving())       //Leaving field, dont even bother
    {
        leaveTheFieldRightNow();
        return;
    }
    
    if (shoutWait <= 0)    //Can shout
    {
        if (shoutProtest())     //Within radius 4 and facing f
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);    //Shouted
            getWorld()->frackRef()->annoy(2, false);
            shoutWait = 15;
            return;
        }
    }
    else if (getWorld()->distance(this, getWorld()->frackRef()) <= 4)
        return;
    
//    else if (getWorld()->distance(this, getWorld()->frackRef()) <= 4)
//    {
//        Actor* f = getWorld()->frackRef();
//
//        if (getX() > f->getX())
//            setDirection(left);
//        else if (getX() < f->getX())
//            setDirection(right);
//        else if (getY() > f->getY())
//            setDirection(down);
//        else if (getY() < f->getY())
//            setDirection(up);
//        return;
//    }
    
    Direction align = lineWithFrack();      //Straight line from frack
    if (align != none)
    {
        setDirection(align);
        switch (align)
        {
            case right:
                moveTo(getX()+1, getY());
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            default:;
        }
        recordMove();
        stepsLeft = 0;
        return;
    }
    
    int M = 16 + getWorld()->getLevel()*2;        //Cellphone signal
    int maxDepth = -1;
    char maze[64][64];
    createMaze(maze);   //Create a string Maze
    if (solveMazeinRange(getX(), getY(), getWorld()->frackRef()->getX(), getWorld()->frackRef()->getY(), maze, maxDepth, M))     //I feel you bro
    {
        maze[getWorld()->frackRef()->getX()][getWorld()->frackRef()->getY()] = ' ';
        maze[getX()][getY()] = '@';
        
        findPath(getX(), getY(), getWorld()->frackRef()->getX(), getWorld()->frackRef()->getY(),maze, maxDepth, 0);
        
        //        for (int i = 0; i != 64; i++)
        //        {
        //            for (int k = 0; k != 64; k++)
        //                std::cerr << maze[i][k];
        //            std::cerr << std::endl;
        //        }
        //        std::cerr << "\n\n";
        //        getWorld()->playSound(SOUND_PROTESTER_YELL);
        
        moveCloser1313(maze);
        return;
    }

    stepsLeft--;        //One big step, I get a little bit closer
    if (stepsLeft <= 0)     //New D
    {
        setDirection(chooseD());    //Change D
        stepsLeft = rand()%53 + 8;      //Move nSteps [8, 60]
    }
    
    if (dirChangeTicks <= 0 && intersection())    //Check if intersection
    {
        dirChangeTicks = 200;
        switch (getDirection()) //Perpendicular Direction and new nSteps
        {
            case up:
            case down:
                setDirection(chooseD(up, down));
                stepsLeft = rand()%53 + 8;
                break;
            case left:
            case right:
                setDirection(chooseD(right, left));
                stepsLeft = rand()%53 + 8;
                break;
            default:;
        }
    }
    
    switch (getDirection())     //One big step, I move a little bit closer
    {
        case up:
            if ( getY() < 60 && ! checkSquareDirty(getX(), getY()+1) && !getWorld()->boulThere(up, this))
                moveTo(getX(), getY()+1);
            else
                stepsLeft = 0;
            break;
        case down:
            if ( getY() > 0 && ! checkSquareDirty(getX(), getY()-1) && !getWorld()->boulThere(down, this))
                moveTo(getX(), getY()-1);
            else
                stepsLeft = 0;
            break;
        case left:
            if ( getX() > 0 && ! checkSquareDirty(getX()-1 , getY()) && !getWorld()->boulThere(left, this))
                moveTo(getX()-1, getY());
            else
                stepsLeft = 0;
            break;
        case right:
            if ( getX() < 60 && ! checkSquareDirty(getX()+1 , getY()) && !getWorld()->boulThere(right, this))
                moveTo(getX()+1, getY());
            else
                stepsLeft = 0;
            break;
        default:
            stepsLeft = 0;
    }
    
    recordMove();
}

void HardCore::bribeHard()
{
    waitStatus = -50 + waitTicks;
}


void HardCore::createMaze (char map[][64])
{
    for (int i = 0; i != 64; i++)
    {
        for (int j = 0; j != 64; j++)
        {
            if (getWorld()->isDirty(i, j))
            {
                map [i][j] = 'X';
                
            }
            else if (getWorld()->boulThere(none, i, j))
            {
                for (int a = i; a < i+4; a++)
                    for (int b = j; b < j+4; b++)
                        map [a][b] = 'X';
            }
            else if (map[i][j] != 'X')
                map [i][j] = 'O';
            
            //std::cerr << map[i][j];
        }
        //std::cerr << "\n";
    }
    //std::cerr << "\n";std::cerr << "\n";std::cerr << "\n";
}

bool HardCore::solveMazeinRange(int sx, int sy, int ex, int ey, char maze[][64], int& maxDepth, int M)
{
    std::queue<Coord> Q;
    Q.push(Coord(sx, sy, 0));
    
    while ( ! Q.empty())
    {
        Coord C = Q.front();
        Q.pop();
        
        if (C.deep > M)
            return false;
        
        if (C.compare(Coord(ex, ey)))
        {
            maxDepth = C.deep;
            return true;
        }
        
        if (mazeSquareCheck(C.x+1, C.y, maze))    //Right
        {
            maze[C.x+1][C.y] = ' ';
            Q.push(Coord(C.x+1, C.y, C.deep+1));
        }
        if (mazeSquareCheck(C.x-1, C.y, maze))     //Left
        {
            maze[C.x-1][C.y] = ' ';
            Q.push(Coord(C.x-1, C.y, C.deep+1));
        }
        if (mazeSquareCheck(C.x, C.y+1, maze))    //Up
        {
            maze[C.x][C.y+1] = ' ';
            Q.push(Coord(C.x, C.y+1, C.deep+1));
        }
        if (mazeSquareCheck(C.x, C.y-1, maze))     //Down
        {
            maze[C.x][C.y-1] = ' ';
            Q.push(Coord(C.x, C.y-1, C.deep+1));
        }
    }
    
    return false;
}

bool HardCore::findPath(int x, int y, int ex, int ey, char maze[][64], int m, int depth, Direction di)
{
    if (x == ex && y == ey)
        return true;
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
    else if (depth > m)
        return false;
    
    bool a(false), b(false), c(false), d(false);
    
    if (maze[x+1][y] == ' ' && di != left)        //Right
    {
        //maze[x+1][y] = '~';
        a = findPath(x+1, y, ex, ey, maze, m, depth+1, right);
        if (a == true)
            maze[x+1][y] = '?';
    }
    
    if (maze[x-1][y] == ' ' && di != right)        //Left
    {
        //maze[x-1][y] = '~';
        b = findPath(x-1, y, ex, ey, maze, m, depth+1, left);
        if (b == true)
            maze[x-1][y] = '?';
    }
    
    if (maze[x][y+1] == ' ' && di != down)        //Up
    {
        //maze[x][y+1] = '~';
        c = findPath(x, y+1, ex, ey, maze, m, depth+1, up);
        if (c == true)
            maze[x][y+1] = '?';
    }
    
    if (maze[x][y-1] == ' ' && di != up)        //Down
    {
        //maze[x][y-1] = '~';
        d = findPath(x, y-1, ex, ey, maze, m, depth+1, down);
        if (d == true)
            maze[x][y-1] = '?';
    }
    
    return a || b || c || d;
}

bool HardCore::mazeSquareCheck(int x, int y, char maze[][64])
{
    if ((x < 0 || x > 63 || y < 0 || y > 63) || maze[x][y] != 'O')
        return false;
    
    for (int i = x; i < x+4; i++)
        for (int j = y; j < y+4; j++)
            if ((i < 0 || i > 63 || j < 0 || j > 63) || toupper(maze[i][j]) == 'X')
                return false;
    
    return true;
}

void HardCore::moveCloser1313(char maze[][64])
{
    if (maze[getX()+1][getY()] == '?')
    {
        setDirection(right);
        moveTo(getX()+1, getY());
    }
    else if (maze[getX()-1][getY()] == '?')
    {
        setDirection(left);
        moveTo(getX()-1, getY());
    }
    else if (maze[getX()][getY()+1] == '?')
    {
        setDirection(up);
        moveTo(getX(), getY()+1);
    }
    else if (maze[getX()][getY()-1] == '?')
    {
        setDirection(down);
        moveTo(getX(), getY()-1);
    }
    
    recordMove();
}

