#include "Game.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <cassert>


using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
    g.addShip(4, 'B', "battleship")  &&
    g.addShip(3, 'D', "destroyer")  &&
    g.addShip(3, 'S', "submarine")  &&
    g.addShip(2, 'P', "patrol boat");
}

int main()
{
    const int NTRIALS = 10;
    
    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
    << "-game match between a mediocre and an awful player, with no pauses"
    << endl;
    cout << "  4.  A single game match between a good player and a mediocre player, with no pauses" << endl;
    cout << "  5.  My Own Game for Testing" << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        g.addShip(1, 'T', "TEE");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;
        
        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
            << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("awful", "Awful Audrey", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                              g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
        << NTRIALS << " games." << endl;
        // We'd expect a mediocre player to win most of the games against
        // an awful player.  Similarly, a good player should outperform
        // a mediocre player.
    }
    
    else if (line[0] == '4'){
        Game battleship(10, 10);
        addStandardShips(battleship);
        
        Player* p1 = createPlayer("good", "Good G", battleship);
        Player* p2 = createPlayer("mediocre", "med A", battleship);
        
        battleship.play(p1, p2, false);
        
        delete p1;
        delete p2;
    }
    
    // My Own Game to Test
    else if (line[0] == '5'){
        Game battleship(6, 7);
        
        /* Testing Board Initialization
         assert(!battleship.addShip(-5, 'B', "Badlength"));
         assert(!battleship.addShip(0, 'Z', "Badlength2"));
         assert(!battleship.addShip(2, '.', "Badsymbol"));
         assert(!battleship.addShip(2, 'X', "Badsymbol2"));
         // assert(!battleship.addShip(5, 'A', "Badair"));
         battleship.addShip(3, 'A', "Air");
         battleship.addShip(3, 'J', "Jet");
         battleship.addShip(2, 'C', "Car");
         battleship.addShip(5, 'S', "Sub");
         battleship.addShip(4, 'L', "L");
         
         Player* p1 = createPlayer("awful", "Awful Rip", battleship);
         Player* p2 = createPlayer("mediocre", "Mediocre Kookie", battleship);
         
         battleship.play(p1, p2);
         delete p1;
         delete p2;
         */
        
         /*
         // Testing
         Player* p1 = createPlayer("awful", "Popeye", g);
         Player* p2 = createPlayer("awful", "Bluto", g);
         cout << g.nShips() << endl;
         cout << g.shipLength(0) << " " << g.shipSymbol(0) << " " << g.shipName(0) << " " << endl;
         cout << g.shipLength(1) << endl;
         
        
        battleship.addShip(3, 'A', "Air");
        battleship.addShip(2, 'C', "Car");
        battleship.addShip(4, 'L', "L");
        // battleship.addShip(3, 'J', "Jet");
        // battleship.addShip(5, 'S', "Sub");
        
        Player* p1 = createPlayer("mediocre", "Med Mimi", battleship);
        Player* p2 = createPlayer("mediocre", "Med Meimei", battleship);
        // Player* p2 = createPlayer("human", "Human Hehe", battleship);
        
        battleship.play(p1, p2);
        delete p1;
        delete p2;
        */
        
        /* Testing Board Functionality
         // Board b(battleship);
         
         cout << endl;
         b.display(true);
         cout << endl;
         b.block();
         b.display(false);
         cout << endl;
         // b.unblock();
         b.clear();
         b.display(false);
         cout << endl;
         Point p;    // should have coords of origin
         Point pp (2, 3);
         Point ppp (1, 0);
         Point out (-1, -4);
         b.placeShip(p, 0, HORIZONTAL);   // true
         // b.placeShip(pp, 0, HORIZONTAL);  // false
         b.placeShip(pp, 2, VERTICAL);     // true
         // b.placeShip(pp, 0, VERTICAL);   // false
         b.placeShip(ppp, 3, VERTICAL);   // true
         
         assert(b.unplaceShip(p, 0, HORIZONTAL));
         assert(!b.unplaceShip(out, 0, HORIZONTAL));
         assert(!b.unplaceShip(p, 1, HORIZONTAL));
         assert(!b.unplaceShip(p, 0, VERTICAL));
         assert(!b.unplaceShip(ppp, 0, HORIZONTAL));
         assert(b.unplaceShip(pp, 2, VERTICAL));
         
         // g.addShip(4, 'U', "IU");    // invalid ship, cannot be added
         // cout << b.placeShip(p, 0, VERTICAL) << endl;
         // cout << b.placeShip(ppp, 1, HORIZONTAL) << endl;
         */
    }
    
    else
    {
        cout << "That's not one of the choices." << endl;
    }
}
