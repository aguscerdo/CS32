// main.cpp

//#include <iostream>
//#include <string>
//#include <random>
//#include <utility>
//#include <cstdlib>
//using namespace std;

#include "Game.h"

//#include "Arena.h"
//#include "Robot.h"
//#include "Player.h"
//#include "History.h"

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(5, 5, 10);
    
    // Play the game
    g.play();
}
