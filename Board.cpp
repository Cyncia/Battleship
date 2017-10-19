#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;
    
private:
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    const Game& m_game;
    int m_ships;
    char m_board[MAXROWS][MAXCOLS];
};

// game already checks for valid board size
BoardImpl::BoardImpl(const Game& g)
: m_game(g)
{
    // number of ships for the board is given by g
    m_ships = g.nShips();
    
    // initialize board to dots
    for (int r = 0; r < g.rows(); r++){
        for (int c = 0; c < g.cols(); c++){
            m_board[r][c] = '.';
        }
    }
    
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++){
        for (int c = 0; c < m_game.cols(); c++){
            if (m_board[r][c] != '.'){
                m_board[r][c] = '.';
            }
        }
    }
}

void BoardImpl::block()
{
    
    // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                // block cell (r,c) with #
                m_board[r][c] = '#';
            }
    
    
    //TESTING BOARDS
    /*
     for (int r = 0; r < 2; r++){
     for (int c = 0; c < m_game.cols(); c++)
     { m_board[r][c] = '#'; }
     }
     m_board[2][0] = '#';
     m_board[2][2] = '#';
     m_board[2][3] = '#';
     m_board[2][4] = '#';
     m_board[2][5] = '#';
     m_board[2][6] = '#';
     
     m_board[3][0] = '#';
     m_board[3][2] = '#';
     m_board[3][3] = '#';
     m_board[3][4] = '#';
     m_board[3][5] = '#';
     m_board[3][6] = '#';
     
     m_board[4][0] = '#';
     m_board[5][0] = '#';
     m_board[5][1] = '#';
     m_board[5][2] = '#';
     m_board[5][3] = '#';
     m_board[5][4] = '#';
     m_board[5][5] = '#';
     
     
     
     for (int r = 0; r < m_game.cols(); r++){
     for (int c = 0; c < m_game.cols(); c++)
     { m_board[r][c] = '#'; }
     }
     
     m_board[1][3] = '.';
     m_board[2][3] = '.';
     m_board[3][3] = '.';
     m_board[4][3] = '.';
     m_board[4][2] = '.';
     m_board[4][4] = '.'; //
     m_board[4][5] = '.';
     m_board[3][5] = '.';
     m_board[2][5] = '.';
     */
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            // unblock cell (r,c) if blocked
            if (m_board[r][c] == '#'){
                m_board[r][c] = '.';
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    // invalid ship ID
    if (shipId < 0 || shipId > m_ships -1){
        // cout << "1";
        return false;
    }
    
    // if point is out of bounds
    if (!m_game.isValid(topOrLeft)){
        // cout << "2";
        return false;
    }
    
    int opencounter = 0;
    // current board should not have any characters of the new ship that will be added
    for (int r = 0; r < m_game.rows(); r++){
        for (int c = 0; c < m_game.cols(); c++){
            
            if (m_board[r][c] == '.'){
                opencounter++;
            }
            
            if (m_board[r][c] == m_game.shipSymbol(shipId)){
                // cout << "3";
                return false;
            }
        }
    }
    
    /*
     int totallength = 0;
     for (int i = 0; i < m_game.nShips(); i++){
     totallength += m_game.shipLength(i);
     }
     
     if (opencounter > totallength){
     return false;
     }
     */
    
    switch(dir){
        case HORIZONTAL:
            // check out of bounds
            if (topOrLeft.c < 0 || topOrLeft.c + m_game.shipLength(shipId) > m_game.cols() || topOrLeft.r < 0 || topOrLeft.r > m_game.rows()){
                // cout << "4";
                return false;
            }
            
            // if position is blocked or has another ship
            for (int c = topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId); c++){
                if (m_board[topOrLeft.r][c] != '.'){
                    // cout << "5";
                    return false;
                }
            }
            
            // if program reaches this point, it is safe to place the ship onto the board
            // replace board characters with appropriate ship letter
            for (int c = topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId); c++){
                m_board[topOrLeft.r][c] = m_game.shipSymbol(shipId);
            }
            
            break;
            
            
        case VERTICAL:
            // check out of bounds
            if (topOrLeft.r < 0 || topOrLeft.r + m_game.shipLength(shipId) > m_game.rows() || topOrLeft.c < 0 || topOrLeft.c > m_game.cols()){
                // cout << "6";
                return false;
            }
            
            // if position is blocked or has another ship
            for (int r = topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId); r++){
                if (m_board[r][topOrLeft.c] != '.'){
                    // cout << "7";
                    return false;
                }
            }
            
            // if program reaches this point, it is safe to place the ship onto the board
            // replace board characters with appropriate ship letter
            for (int r = topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId); r++){
                m_board[r][topOrLeft.c] = m_game.shipSymbol(shipId);
            }
            
            break;
    }
    
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    // invalid ship ID
    if (shipId < 0 || shipId > m_ships -1){
        return false;
    }
    
    // if point is out of bounds
    if (!m_game.isValid(topOrLeft)){
        return false;
    }
    
    switch (dir){
        case HORIZONTAL:
            for (int c = topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId); c++){
                if (m_board[topOrLeft.r][c] != m_game.shipSymbol(shipId)){
                    return false;
                }
            }
            
            break;
            
        case VERTICAL:
            for (int r = topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId); r++){
                if (m_board[r][topOrLeft.c] != m_game.shipSymbol(shipId)){
                    return false;
                }
            }
            break;
    }
    
    switch (dir){
        case HORIZONTAL:
            for (int c = topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId); c++){
                m_board[topOrLeft.r][c] = '.';
            }
            
            break;
            
        case VERTICAL:
            for (int r = topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId); r++){
                m_board[r][topOrLeft.c] = '.';
            }
            
            break;
    }
    
    
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    // o = missed attacks
    // X = damaged ship segments
    // . = no attacks/untouched
    
    // if shots only F -> use ship's symbol to display undamaged ship segments
    // if shots only T -> use period to display undamaged ship segment
    //********************************************
    // print board
    
    // print spaces
    cout << "  ";
    
    // print col nums
    for (int i = 0; i < m_game.cols(); i++){
        cout << i;
    }
    
    // new line to print rows
    cout << endl;
    
    // print rows and row content
    for (int r = 0; r < m_game.rows(); r++){
        cout << r << " ";
        
        for (int c = 0; c < m_game.cols(); c++){
            
            if (shotsOnly){
                if (m_board[r][c] != 'X' && m_board[r][c] != 'o' && m_board[r][c] != '#'){
                    cout << '.';
                }
                else {
                    cout << m_board[r][c];
                }
            }
            
            else {
                cout << m_board[r][c];
            }
        }
        cout << endl;
    }
    
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    // cout << p.r << p.c;
    char temp;
    int id = -1;
    
    // check if point is valid/within bounds
    if(!m_game.isValid(p)){
        shotHit = false;
        return false;
    }
    
    // check if spot has been previously attacked before
    if (m_board[p.r][p.c] == 'o' || m_board[p.r][p.c] == 'X'){
        shotHit = false;
        return false;
    }
    
    // if water is hit, return o
    if (m_board[p.r][p.c] == '.'){
        shotHit = false;
        m_board[p.r][p.c] = 'o';
    }
    
    // if any undamaged segment of a ship is at position p on the board...
    // considered as a damaged segment
    else if (m_board[p.r][p.c] != '.' && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X'){
        
        for (int i = 0; i < m_ships; i++){
            if (m_board[p.r][p.c] == m_game.shipSymbol(i)){
                temp = m_game.shipSymbol(i);
                id = i;
            }
        }
        
        m_board[p.r][p.c] = 'X';
        shotHit = true;
    }
    
    shipDestroyed = true;
    
    if (shotHit){
        for (int r = 0; r < m_game.rows(); r++){
            for (int c = 0; c < m_game.cols(); c++){
                if (m_board[r][c] == temp && shipDestroyed){
                    shipDestroyed = false;
                }
            }
        }
        
        if (shipDestroyed && id != -1){
            shipId = id;
        }
    }
    
    if (!shotHit){
        shipDestroyed = false;
    }
    
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    
    for (int i = 0; i < m_ships; i++){
        
        for (int r = 0; r < m_game.rows(); r++){
            for (int c = 0; c < m_game.cols(); c++){
                
                if (m_board[r][c] == m_game.shipSymbol(i)){  // if (m_board[r][c] != '.', 'X', 'o', '#'
                    return false;
                }
                
            }
        }
    }
    
    
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
