#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <vector>
#include <cmath>

Actor::Actor(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr, bool Human, bool isBoulder): GraphObject(image, x, y, d, size, depth)
{
    m_ticks = 0;
    m_world = ptr;
    setVisible(true);
    m_dead = false;
    m_human = Human;
    m_boulder = isBoulder;
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

NonHuman::NonHuman(int image, int x, int y, Direction d, float size, unsigned int depth, StudentWorld* ptr, bool isBoulder): Actor(image, x, y, d, size, depth, ptr, false, isBoulder)
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

bool Goodies::itemProximity()
{
    if (getWorld()->distance(getX(), getY(), getWorld()->frackRef()) <= 3)
    {
        setDead();
        setVisible(false);
        getWorld()->increaseScore(m_score);
        return true;
    }
    else if (getWorld()->distance(getX(), getY(), getWorld()->frackRef()) <= 4)
    {
        setVisible(true);
    }
    
    return false;
}

//----------------------------------------------------------------------//

Boulder::Boulder(int x, int y, StudentWorld* ptr): NonHuman(IID_BOULDER, x, y, down, 1, 1, ptr, true)
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
    
    if (m_stable)
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
    else
    {
        if (getTick() < 30)
        {
            setTick(getTick()+1);
            if (getTick() == 30)
                getWorld()->playSound(SOUND_FALLING_ROCK);
        }
        else    //Falling action
        {
            bool dirty = true;
            for (int i = getX(); i <= getX()+3; i++)
            {
                if (getWorld()->isDirty(i, getY()-1))   //If there is at least one dirt underneath, set to fall to false
                {
                    dirty = false;
                    break;
                }
            }
            
            if (getWorld()->distance(getX(), getY(), getWorld()->frackRef()) <= 3)
            {
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                getWorld()->frackRef()->setDead();  //Bumped into frackman, kill em
                return;
            }
            
            getWorld()->humanBoulder(this);     //Check if collides with human
            
            if (getY() == 0 || ! dirty || getWorld()->boulThere(down, getX(), getY()))
            {
                setVisible(false);
                setDead();
            }
            else
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
    
    if (itemProximity())
    {
        getWorld()->reduceOil();
        getWorld()->playSound(SOUND_FOUND_OIL);
    }
    
}

//----------------------------------------------------------------------//

Gold::Gold (int x, int y, StudentWorld* ptr, bool pickUp): Goodies(IID_GOLD, x, y, 2, ptr, 10)
{
    pickAble = pickUp;
    
    if (pickAble)
        setVisible(false);
    else
    {
        setVisible(true);
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
    
    if (pickAble)
    {
        if (itemProximity())
        {
            getWorld()->frackRef()->adjustGold(1);
            getWorld()->playSound(SOUND_GOT_GOODIE);
        }
    }
    else
    {
        if (getTick() >= lifeTime)
        {
            setDead();
            setVisible(false);
        }
        else
        {
            setTick(getTick()+1);
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
        getWorld()->frackRef()->adjustSonar(1);
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

Human::Human(int image, int x, int y, Direction d, int health, StudentWorld* ptr): Actor(image, x, y, d, 1, 0, ptr, true, false)
{
    m_health = health;
}

Human::~Human() {}

void Human::annoy(int power, bool boulder)
{
    m_health -= power;
    if (m_health <= 0)
        setDead();
}

int Human::health() const
{
    return m_health;
}

//----------------------------------------------------------------------//
void FrackMan::adjustGold(int amount)
{
    m_gold += amount;
}
void FrackMan::adjustWater(int amount)
{
    m_water += amount;
}
void FrackMan::adjustSonar(int amount)
{
    m_sonar += amount;
}

int FrackMan::goldCount() const {return m_gold;}
int FrackMan::waterCount() const {return m_water;}
int FrackMan::sonarCount() const {return m_sonar;}

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
                if (getDirection() == left && getX() > 0 && !getWorld()->boulThere(left, getX(), getY()))
                    moveTo(getX()-1, getY());
                else if (getDirection() != left)
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() == right && getX() < 60 && !getWorld()->boulThere(right, getX(), getY()))
                    moveTo(getX()+1, getY());
                else if (getDirection() != right)
                    setDirection(right);
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() == down && getY() > 0 && !getWorld()->boulThere(down, getX(), getY()))
                    moveTo(getX(), getY()-1);
                else if (getDirection() != down)
                    setDirection(down);
                break;
            case KEY_PRESS_UP:
                if (getDirection() == up && getY() < 60 && !getWorld()->boulThere(up, getX(), getY()))
                    moveTo(getX(), getY()+1);
                else if (getDirection() != up)
                    setDirection(up);
                break;
            case KEY_PRESS_ESCAPE:
                setDead();
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
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
        }
        
        if(getWorld()->clearDirt(getX(), getY()))
           getWorld()->playSound(SOUND_DIG);   //Checks if there is dirt in the player's sprite
    }
}

//----------------------------------------------------------------------//

Protester::Protester(StudentWorld* ptr, bool hard): Human(IID_PROTESTER, 60, 60, left, 5, ptr)
{
    int T = 3 - getWorld()->getLevel()/4;
    if (T > 0)
        waitTicks = T;
    else
        waitTicks = 0;
    leavingField = false;
    notShouted = true;
    m_hard = hard;
}

Protester::~Protester() {}

void Protester::doSomething()
{
    if (isDead())
        return;
    
    if (waitStatus < waitTicks)
    {
        waitStatus++;
        return;
    }
    else
    {
        waitStatus = 0;
        if (! notShouted)
            shoutWait--;
    }
    
    if (leavingField)
    {
        
    }
    
    if (shoutWait == 0)     //Can shout
    {
        if (shoutProtest())     //Within radius 4 and facing f
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->frackRef()->annoy(2, false);
            shoutWait = 15;
            return;
        }
    }
}

bool Protester::shoutProtest()
{
    if(getWorld()->distance(getX(), getY(), getWorld()->frackRef()) <= 4)
    {
        if (getX()==getWorld()->frackRef()->getX() && getY()==getWorld()->frackRef()->getY())
            return true;
        
        switch (getDirection())
        {
            case right:
                if (getX() < getWorld()->frackRef()->getX())
                    return true;
                break;
            case left:
                if (getX() > getWorld()->frackRef()->getX())
                    return true;
                break;
            case up:
                if (getY() < getWorld()->frackRef()->getY())
                    return true;
                break;
            case down:
                if (getY() > getWorld()->frackRef()->getY())
                    return true;
                break;
            default:
                break;
        }
    }
    return false;
}













