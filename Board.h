#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include "globals.h"

class Game;
class BoardImpl;

class Board
{
public:
    Board(const Game& g);
    ~Board();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;
    
    // We prevent a Board object from being copied or assigned
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    
private:
    BoardImpl* m_impl;
};

#endif // BOARD_INCLUDED
//
//  Board.hpp
//  Assignments
//
//  Created by Cynthia Zhou on 6/15/17.
//  Copyright © 2017 Cynthia Zhou. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <stdio.h>

#endif /* Board_hpp */
