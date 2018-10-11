#include "Game.h"
#include "Board.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>

using namespace std;

class Ship
{
public:
    Ship(int length, char symbol, string name);
    int length() const;
    char symbol() const;
    string name() const;
    
private:
    int m_length;
    char m_symbol;
    string m_name;
};

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    
private:
    int m_rows;
    int m_cols;
    int m_nShips;
    vector <Ship> shipcollection;
};

// Non-member Function
void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

/////////////////////////////////////////////////////////////////////////
// Ship Functions
Ship::Ship(int length, char symbol, string name){
    m_length = length;
    m_symbol = symbol;
    m_name = name;
}

int Ship::length() const {
    return m_length;
}

char Ship::symbol() const {
    return m_symbol;
}

string Ship::name() const {
    return m_name;
}

/////////////////////////////////////////////////////////////////////////
// GameImpl Functions
GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    m_nShips = 0;
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship add(length, symbol, name);
    shipcollection.push_back(add);
    m_nShips++;
    
    return true;
}

int GameImpl::nShips() const
{
    return m_nShips;
    // return shipcollection.size() + 1;
}

int GameImpl::shipLength(int shipId) const
{
    return shipcollection[shipId].length();
}

char GameImpl::shipSymbol(int shipId) const
{
    return shipcollection[shipId].symbol();
}

string GameImpl::shipName(int shipId) const
{
    return shipcollection[shipId].name();
}

// shouldPause defaults to true if not included
Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    // p1 will have b1
    // p2 will have b2
    
    /*
     * sets each player's ships
     * might fail because:
        - no configuration of ships will fit
        - mediocre player unable to place ships due to blocked area
     * if ships could not be placed on the board before the game began (placeShips from player class)
     */
    
    if (!(p1->placeShips(b1))){
        // cout << endl << "b1: " << endl;
        b1.display(false);
        cout << "ERROR: Ships cannot be placed for P1, Game cannot start" << endl;
        return nullptr;
    }
    
    if (!(p2->placeShips(b2))){
        // cout << endl << "b2: " << endl;
        b2.display(false);
        cout << "ERROR: Ships cannot be placed for P2, Game cannot start" << endl;
        return nullptr;
    }
   
    /*
     b1.display(0);
     cout << endl;
     b2.display(0);
     cout << "after placing ships in game.cpp";*/
    
    /////////////////////////////////////////////////////////////////////////
    bool hit = 0; // shotHIT
    bool shot = 0;  // validSHOT
    bool destroy = 0;
    int id = -1;
    Point P;
    
    // loop until someone wins (i.e., have no more ships)
    // while(p1->game().nShips() != 0 || p2->game().nShips() != 0){
    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {
        
        // P1 = HUMAN
        // if p1 is human, do not display undamaged segments
        if (p1->isHuman()){
            
            cout << p1->name() << "'s turn. Board for " << p2->name() << ": " << endl;
            b2.display(true);  // displaying p2's board
            
            P = p1->recommendAttack();
            
            // p1 attack p2
            // will say if attack is successful or not (i.e. if attack point is outside the board or attack is made on a previously attacked location)
            if (b2.attack(P, hit, destroy, id)){
                if (!hit){
                    cout << p1->name() << " attacked (" << P.r << "," << P.c << ") and missed, resulting in: " << endl;
                }
                
                else if (hit && destroy){
                    cout << p1->name() << " attacked (" << P.r << "," << P.c << ") and destroyed the " << shipName(id) << ", resulting in: " << endl;
                }
                
                else if (hit){
                    cout << p1->name() << " attacked (" << P.r << "," << P.c << ") and hit something, resulting in: " << endl;
                }
                // if attack was successfully made...
                shot = true;
                b2.display(true);  // displaying resulting attack
            }
            
            else {
                shot = false;
                destroy = false;
                hit = false;
                cout << p1->name() << " wasted a shot at (" << P.r << ", " << P.c << ")." << endl;
            }
            
            if (shouldPause){
                waitForEnter();
            }
            
            // attacker needs to know the results of his/her attack
            p1->recordAttackResult(P, shot, hit, destroy, id);
            
        } // end of if Human
        
        // P1 != HUMAN
        if (!p1->isHuman()){
            cout << p1->name() << "'s turn. Board for " << p2->name() << ": " << endl;
            b2.display(false);  // displaying p2's board
            
            P = p1->recommendAttack();
            
            // p1 attack p2
            // will say if attack is successful or not (i.e. if attack point is outside the board or attack is made on a previously attacked location)
            if (b2.attack(P, hit, destroy, id)){
                if (!hit){
                    cout << p1->name() << " attacked (" << P.r << "," << P.c << ") and missed, resulting in: " << endl;
                }
                
                else if (hit && destroy){
                    cout << p1->name() << " attacked (" << P.r << "," << P.c << ") and destroyed the " << shipName(id) << ", resulting in: " << endl;
                }
                
                else if (hit){
                    cout << p1->name() << " attacked (" << P.r << "," << P.c << ") and hit something, resulting in: " << endl;
                }
                shot = true;
                b2.display(false);  // displaying resulting attack
            }
            else {
                shot = false;
                destroy = false;
                hit = false;
                cout << p1->name() << " wasted a shot at (" << P.r << ", " << P.c << ")." << endl;
            }
            
            if (shouldPause){
                waitForEnter();
            }
            
            // opponent needs to know where the attack was made on his/her board
            p2->recordAttackByOpponent(P);
            
            // attacker needs to know the results of his/her attack
            p1->recordAttackResult(P, shot, hit, destroy, id);
        }
        
        if (b2.allShipsDestroyed()){
            break;
        };
        
        /////////////////////////////////////////////////////////////////////////
        // P2 = HUMAN
        if (p2->isHuman()){
            
            cout << p2->name() << "'s turn. Board for " << p1->name() << ": " << endl;
            b1.display(true);  // displaying p1's board
            
            P = p2->recommendAttack();
            
            // p2 attack p1
            // will say if attack is successful or not (i.e. if attack point is outside the board or attack is made on a previously attacked location)
            if (b1.attack(P, hit, destroy, id)){
                if (!hit){
                    cout << p2->name() << " attacked (" << P.r << "," << P.c << ") and missed, resulting in: " << endl;
                }
                else if (hit && destroy){
                    cout << p2->name() << " attacked (" << P.r << "," << P.c << ") and destroyed the " << shipName(id) << ", resulting in: " << endl;
                }
                else if (hit){
                    cout << p2->name() << " attacked (" << P.r << "," << P.c << ") and hit something, resulting in: " << endl;
                }
                shot = true;
                b1.display(true);  // displaying resulting attack
            }
            else {
                shot = false;
                destroy = false;
                hit = false;
                cout << p2->name() << " wasted a shot at (" << P.r << ", " << P.c << ")." << endl;
            }
            
            if (shouldPause){
                waitForEnter();
            }
            
            // opponent needs to know where the attack was made on his/her board
            p1->recordAttackByOpponent(P);
            
            // attacker needs to know the results of his/her attack
            p2->recordAttackResult(P, shot, hit, destroy, id);
            
        }
        
        // P2 != HUMAN
        if (!p2->isHuman()){
            
            cout << p2->name() << "'s turn. Board for " << p1->name() << ": " << endl;
            b1.display(false);  // displaying p1's board
            
            P = p2->recommendAttack();
            
            // p2 attack p1
            // will say if attack is successful or not (i.e. if attack point is outside the board or attack is made on a previously attacked location)
            if (b1.attack(P, hit, destroy, id)){
                if (!hit){
                    cout << p2->name() << " attacked (" << P.r << "," << P.c << ") and missed, resulting in: " << endl;
                }
                else if (hit && destroy){
                    cout << p2->name() << " attacked (" << P.r << "," << P.c << ") and destroyed the " << shipName(id) << ", resulting in: " << endl;
                }
                else if (hit){
                    cout << p2->name() << " attacked (" << P.r << "," << P.c << ") and hit something, resulting in: " << endl;
                }
                shot = true;
                b1.display(false);  // displaying resulting attack
            }
            else {
                shot = false;
                destroy = false;
                hit = false;
                cout << p2->name() << " wasted a shot at (" << P.r << ", " << P.c << ")." << endl;
            }
            
            if (shouldPause){
                waitForEnter();
            }
            
            // opponent needs to know where the attack was made on his/her board
            p1->recordAttackByOpponent(P);
           
            // attacker needs to know the results of his/her attack
            p2->recordAttackResult(P, shot, hit, destroy, id);
        }
        
        
    } // end of while
    
    // p1 is the winner
    if (b2.allShipsDestroyed()){
        cout << p1->name() << " wins!" << endl;
        
        if (p2->isHuman()){
            b2.display(false);
        }
        
        return p1;
    }
    
    // p2 is the winner
    if (b1.allShipsDestroyed()){
        cout << p2->name() << " wins!" << endl;
        
        if (p1->isHuman()){
            b1.display(false);
        }
        
        return p2;
    }
    
    // ONLY RETURN NULL IF:
    // no configuration of ships will fit
    // OR mediocre player is unable to place all of its ships
    
    return nullptr;
    
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
        << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
        << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
        << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
            << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    // if either player is invalid or ships have not been placed yet
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0){
        cout << "NONE";
        return nullptr;
    }
    // creates 2 board objects
    Board b1(*this);
    Board b2(*this);
    
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

