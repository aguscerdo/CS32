#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

//-----------------------------------//

StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    unsigned int sr = getLevel()*getLevel()+getScore()+rand()%(getLevel()+getScore()+100);
    srand(sr);
    
    for (int x = 0; x != 64; x++)
        for (int y = 0; y != 64; y++)       //Initialize the dirt
        {
            if (y >= 60)
            {
                dirty[x][y] = nullptr;
            }
            else if (x >= 30 && x <= 33 && y >= 4)
            {
                dirty[x][y] = nullptr;
            }
            else
            {
                dirty[x][y] = new Dirt(x, y, this);
            }
        }
    
    player = new FrackMan(this);        //Allocate player
    player->setVisible(true);
    
    int x = -1, y = -1;
    
    int B = getLevel()/ 2 + 2;      //Boulders
    B = min(B, 6);
    for (int i = 0; i < B; i++)
    {
        randAllocateBig(x, y);
        actors.push_back(new Boulder(x, y, this));
    }
    
    int G = 5-getLevel()/ 2;        //Gold
    G = max(G, 2);
    for (int k = 0; k < G; k++)
    {
        randAllocateBig(x, y);
        actors.push_back(new Gold(x, y, this, true));
    }
    
    int L = getLevel() + 2;      //Barrels
    L = min (L, 20);
    oilCount = L;
    for (int k = 0; k < L; k++)
    {
        randAllocateBig(x, y);
        actors.push_back(new Barrel(x, y, this));
    }
    
    int T = 200 - getLevel();
    T = max(T, 25);
    protestTickWait = T;
    
    int P = 2 + getLevel()*1.5;
    P = max(P, 15);
    maxProtest = P;
    
    protestWaitState = 0;
    protestCount = 0;
    
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    string stringer = statString();
    setGameStatText(stringer);
    
    //--------------------------// Do u wanna play a game?
    
    player->doSomething();
    
    for (vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)       //Ask rest to do something
    {
        (*i)->doSomething();
    }
    
    //--------------------------//  A new actor joins smash!
    
    if (rand()%(getLevel()*25+300) == 1)   //Chance of adding water or sonar
    {
        if (rand()%5 == 1) // 1/5 chances to add a Sonar Kit
        {
            actors.push_back(new Sonar (this));
        }
        else
        {
            int x1, y1;
            allocateWater(x1, y1);
            actors.push_back(new WaterPool(x1, y1, this));
        }
    }
    
    if (protestWaitState > 0)
    {
        protestWaitState--;
    }
    else
    {
         if (protestCount < maxProtest)
         {
             int P = (getLevel() * 10 + 30);
             P = min(90, P);
             int r = rand()%100+1;
             
             if (r > P)
             {
                 actors.push_back(new Protester(this));
             }
             else
             {
                 actors.push_back(new HardCore(this));
             }
             
             protestCount++;
             protestWaitState = protestTickWait;
         }
    }
    
    //--------------------------// I see dead people O.o
    
    if (player->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    else if (oilCount == 0)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    vector<Actor*>::iterator i = actors.begin();
    while (i != actors.end())
    {
        if((*i)->isDead())
        {
            delete *i;
            *i = nullptr;
            i = actors.erase(i);
        }
        else
            i++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int x = 0; x != 64; x++)       //Clear Dirt
        for (int y = 0; y != 64; y++)
        {
            delete dirty[x][y];
            dirty[x][y] = nullptr;
        }
    
    delete player;      //Delete player
    player = nullptr;
    
    while (!actors.empty())           //Clear all the rest
    {
        delete actors[0];
        actors[0] = nullptr;
        actors.erase(actors.begin());
    }
}

bool StudentWorld::clearDirt(int x, int y)
{
    bool returner = false;
    
    for (int x1 = x; x1 <= x+3 && x1 < 64; x1++)
        for (int y1 = y; y1 <= y+3 && y1 < 64; y1++)
            if (dirty[x1][y1] != nullptr)
            {
                delete dirty[x1][y1];
                dirty[x1][y1] = nullptr;
                returner = true;
            }
    
    return returner;
}

void StudentWorld::randAllocateBig(int& x, int& y)
{
    do
    {
        x = rand()%61;
        y = rand()%57;
        
    } while (x<0 || y<20 || (x>=27 && x<=33) || ! goodCreationD(x, y));
}

bool StudentWorld::goodCreationD(int x, int y)
{
    if ( ! actors.empty())
    {
        for (vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
        {
            double dx = (x - (*i)->getX());
            double dy = (y - (*i)->getY());
            double distance = sqrt(dx*dx + dy*dy);
        
            if (distance < 6.0)
                return false;
        }
    }
    
    return true;
}

double StudentWorld::distance (Actor* ptr1, Actor* ptr2)
{
    double dx = ptr1->getX() - ptr2->getX();
    double dy = ptr1->getY() - ptr2->getY();
    
    return sqrt(dx*dx + dy*dy);
}

bool StudentWorld::boulThere (Actor::Direction d, Actor* ptr)
{
    if ( ! actors.empty())
    {
        for (vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
        {
            if ((*i)->isBoulder())
                switch (d)
                {
                    case Actor::right:
                        if (ptr->getX() == (*i)->getX()-4 && distance(ptr, *i) <= 5)
                            return true;
                        break;
                    case Actor::left:
                        if (ptr->getX() == (*i)->getX()+4 && distance(ptr, *i) <= 5)
                            return true;
                        break;
                    case Actor::up:
                        if (ptr->getY() == (*i)->getY()-4 && distance(ptr, *i) <= 5)
                            return true;
                        break;
                    case Actor::down:
                        if (ptr->getY() == (*i)->getY()+4 && distance(ptr, *i) <= 5)
                            return true;
                        break;
                    default:
                        break;
                }
        }
    }
    return false;
}

bool StudentWorld::boulThere (Actor::Direction d, int x, int y)
{
    if ( ! actors.empty())
    {
        for (vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
        {
            if ((*i)->isBoulder())
            {
                if (d == Actor::none && (*i)->getX() == x && (*i)->getY() == y)
                    return true;
                
                int dx = (*i)->getX() - x;
                int dy = (*i)->getY() - y;
                double distance = sqrt(dx*dx + dy*dy);
//                if (distance <= 3)
//                    return true;
                
                switch (d)
                {
                    case Actor::right:
                        if (x == (*i)->getX()-4 && distance <= 5)
                            return true;
                        break;
                    case Actor::left:
                        if (x == (*i)->getX()+4 && distance <= 5)
                            return true;
                        break;
                    case Actor::up:
                        if (y == (*i)->getY()-4 && distance <= 5)
                            return true;
                        break;
                    case Actor::down:
                        if (y == (*i)->getY()+4 && distance <= 5)
                            return true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return false;
}

std::string StudentWorld::statString() const
{
    std::string text = "Scr: ";
    
    std::string score = intToStr(getScore());
    std::string lives = intToStr(getLives());
    std::string level = intToStr(0+getLevel());
    std::string health = intToStr(player->health()*10);
    std::string water = intToStr(player->waterCount());
    std::string gold = intToStr(player->goldCount());
    std::string sonar = intToStr(player->sonarCount());
    std::string oil = intToStr(oilCount);
    
    for (int k = 0; k < 6-score.length(); k++)      //Add score to 7 digits
        text += '0';
    text += score;
    text += "  ";
    
    text += "Lvl: ";
    for (int k = 0; k < 2-level.length(); k++)
        text += ' ';
    text += level;
    text += "  ";
    
    text += "Lives: ";
    text += lives;
    text += "  ";
    
    text += "Hlth: ";
    for (int k = 0; k < 3-health.length(); k++)
        text += ' ';
    text += health;
    text += "%  ";
    
    text += "Wtr: ";
    for (int k = 0; k < 2-water.length(); k++)
        text += ' ';
    text += water;
    text += "  ";
    
    text += "Gld: ";
    for (int k = 0; k < 2-gold.length(); k++)
        text += ' ';
    text += gold;
    text += "  ";
    
    text += "Sonar: ";
    for (int k = 0; k < 2-sonar.length(); k++)
        text += ' ';
    text += sonar;
    text += "  ";
    
    text += "Oil Left: ";
    for (int k = 0; k < 2-oil.length(); k++)
        text += ' ';
    text += oil;
    
    return text;
}

std::string StudentWorld::intToStr(int n) const
{
    if (n == 0)
        return "0";
    
    std::string number = "";
    char ch;
    int size = log10(n);
    
    for (int k = size; k >= 0; k--)
    {
        int simple = n/pow(10, k);
        ch = simple%10 + '0';
        number += ch;
    }
    
    return number;
}

bool StudentWorld::isDirty (int x, int y) const
{
    if (x > 63 || y > 63 || x < 0 || y < 0)
        return false;
    
    if (dirty[x][y] != nullptr)
        return true;
    
    return false;
}

void StudentWorld::sonarReveal()
{
    player->adjustSonar(-1);
    playSound(SOUND_SONAR);
    
    for (std::vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
        if (distance(player, (*i)) <= 12)
            (*i)->setVisible(true);
}

void StudentWorld::dropGold()
{
    actors.push_back(new Gold (player->getX(), player->getY(), this, false));
}

bool StudentWorld::greedyB(Actor* gold)
{
    for (vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
    {
        if ((*i)->isHuman())
            if (distance(*i, gold) <= 3)
            {
                playSound(SOUND_PROTESTER_FOUND_GOLD);
                
                if ((*i)->isHard())
                {
                    (*i)->bribeHard();
                    increaseScore(50);
                }
                else
                {
                    (*i)->leaveTheField();
                    increaseScore(25);
                }
                return true;
            }
    }
    
    return false;
}

void StudentWorld::humanBoulder(Actor* ptrB)
{
    if ( ! actors.empty())
    {
        for (std::vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
        {
            if ((*i) != nullptr && (*i)->isHuman())
                if (distance(ptrB, (*i)) <= 3)
                {
                    (*i)->annoy(100, true);
                }
        }
    }
    
    
}

void StudentWorld::allocateWater(int& x, int& y)
{
    bool dirtyB;
    
    do
    {
        dirtyB = false;
        
        x = rand()%60;
        y = rand()%60;
        
        for (int x0 = x; x0 < x+4; x0++)
            for (int y0 = y; y0 < y+4; y0++)
                if (isDirty(x0, y0))
                {
                    dirtyB = true;
                }
    } while (dirtyB);
}

bool StudentWorld::squirtMan(Actor* squirt)
{
    
    for (vector<Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
    {
        if ((*i)->isHuman())
        {
            if (distance(squirt, *i) <= 3)
            {
                (*i)->annoy(2, false);
                return true;
            }
        }
    }
    
    return false;
}

void StudentWorld::allocateSquirt()
{
    bool mayCreate = true;
    
    switch (player->getDirection())
    {
        case Actor::up:
            if (player->getY() >= 60)
                return;
            for (int x = player->getX(); x < player->getX()+3; x++)
                for (int y = player->getY()+3; y < player->getY()+7; y++)
                    if (isDirty(x, y) || boulThere(Actor::up, player))
                        mayCreate = false;
            if (mayCreate)
                actors.push_back(new Squirt(player->getX(), player->getY()+3, Actor::up, this));
            return;
        case Actor::down:
            if (player->getY() <= 0)
                return;
            for (int x = player->getX(); x < player->getX()+3; x++)
                for (int y = player->getY()-3; y < player->getY(); y++)
                    if (isDirty(x, y) || boulThere(Actor::down, player))
                        mayCreate = false;
            if (mayCreate)
                actors.push_back(new Squirt(player->getX(), player->getY()-3, Actor::down, this));
            return;
        case Actor::right:
            if (player->getX() >= 60)
                return;
            for (int x = player->getX()+3; x < player->getX()+7; x++)
                for (int y = player->getY(); y < player->getY()+3; y++)
                    if (isDirty(x, y) || boulThere(Actor::up, player))
                        mayCreate = false;
            if (mayCreate)
                actors.push_back(new Squirt(player->getX()+3, player->getY(), Actor::right, this));
            return;
        case Actor::left:
            if (player->getX() <= 0)
                return;
            for (int x = player->getX()-3; x < player->getX(); x++)
                for (int y = player->getY(); y < player->getY()+3; y++)
                    if (isDirty(x, y) || boulThere(Actor::down, player))
                        mayCreate = false;
            if (mayCreate)
                actors.push_back(new Squirt(player->getX()-3, player->getY(), Actor::left, this));
            return;
        default:;
    }
}
