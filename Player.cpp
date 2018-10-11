#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

#include <random>
#include <vector>
#include <list>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}



//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}


class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual ~HumanPlayer() {}
    virtual bool placeShips(Board& b);
    virtual bool isHuman() const { return true; }
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " the Human must place " << game().nShips() << " ships." << endl;
    
    int row, col;
    
    for (int i = 0; i < game().nShips(); i++){
        // setting the board
        b.display(false);
        
        bool check1 = false;
        bool check2 = false;
        string dir = "";
        Direction way;
        
        // get direction
        while (!check1){
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            getline(cin, dir);
            
            if (dir.empty()) {
                cout << "Direction must be h or v." << endl;
                check1 = false;
            }
            
            // caps 'H' and 'V' were not considered valid with sample program
            else if (dir[0] != 'h' && dir[0] != 'v'){
                cout << "Direction must be h or v." << endl;
                check1 = false;
            }
            
            else if (dir[0] == 'h'){
                way = HORIZONTAL;
                check1 = true;
            }
            
            else if (dir[0] == 'v'){
                way = VERTICAL;
                check1 = true;
            }
            
        }
        
        // get row and col number
        while (!check2) {
            cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
            check2 = true;
            
            // bad input
            bool input = getLineWithTwoIntegers(row, col);
            
            // if not inbounds
            Point p(row, col);
            
            if (!input){
                cout << "The ship can not be placed there." << endl;
                check2 = false;
            }
            
            else if (!game().isValid(p)){
                cout << "The ship can not be placed there." << endl;
                check2 = false;
            }
            
            else if (!b.placeShip(p, i, way)){
                cout << "The ship can not be placed there." << endl;
                check2 = false;
            }
        }
    } // end of for
    
    // b.display(false);
    // cout << endl;
    
    return true;
}

Point HumanPlayer::recommendAttack()
{
    int row, col;
    cout << "Enter the row and column to attack (e.g, 3 5): ";
    getLineWithTwoIntegers(row, col);
    Point p(row, col);
    
    return p;
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
    // don't need to do anything
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
    // don't need to do anything
}



//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual ~MediocrePlayer() {}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool pathExists(vector<Point>&all, Board& b, int ship);
    bool backtrack(vector<Point>&all, Board& b, int ship);
    bool repeat(Point rand);
    
    // Return a uniformly distributed random int from 0 to limit-1
    int randInt(int start, int limit);
    
private:
    Point m_lastCellAttacked;
    Point m_transition;
    int m_state = 1;
    vector <Point> m_points;
    vector <Point> cross;
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0), m_transition(0, 0)
{}

int MediocrePlayer::randInt(int start, int limit){
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distro(start, limit-1);
    return distro(generator);
}

bool MediocrePlayer::placeShips(Board& b)
{
    // Remember that Mediocre::placeShips(Board& b) must start by calling
    // b.block(), and must call b.unblock() just before returning.
    b.block();
    vector <Point> all;
    all.clear();
    
    int ship = 0;
    for (int i = 0; i < 50; i++){
        if (pathExists(all, b, ship)){
            b.unblock();
            return true;
        }
    }
    
    // b.unblock();
    return false;
}


bool MediocrePlayer::backtrack(vector<Point>&all, Board& b, int ship){
    
    // base case
    if (ship == 0){
        return false;
    }
    
    Point temp(all.back().r, all.back().c);
    
    // called when tempship = ship
    if (b.unplaceShip(all.back(), ship-1, HORIZONTAL)){
        ship--;
        all.pop_back();
        
        // first see if the ship can be flipped the other way before moving it to a new spot (H -> V)
        if (b.placeShip(temp, ship, VERTICAL)){
            all.push_back(temp);
            if (ship < game().nShips()){
                ship++;
            }
            pathExists(all, b, ship);
            return true;
        }
    }
    
    else if (b.unplaceShip(all.back(), ship-1, VERTICAL)){
        ship--;
        all.pop_back();
    }
    
    for (int nonfirstc = temp.c + 1; nonfirstc < game().cols(); nonfirstc++){
        Point thisrow(temp.r, nonfirstc);
        
        if (b.placeShip(thisrow, ship, HORIZONTAL) || b.placeShip(thisrow, ship, VERTICAL)){
            all.push_back(thisrow);
            if (ship < game().nShips()){
                ship++;
            }
            if (pathExists(all, b, ship)){
                // pathExists(all, b, ship);
                return true;
            }
            else {
                return false;
            }
        }
        
    }// end of for
    
    if (all.back().r < game().rows() - 1) {
        // + 1 to avoid ship being placed in the same position
        for (int r = temp.r + 1; r < game().rows(); r++){
            for (int c = 0; c < game().cols(); c++){
                Point newcur(r, c);
                
                // re-place last placed ship
                if (b.placeShip(newcur, ship, HORIZONTAL) || b.placeShip(newcur, ship, VERTICAL)){
                    // if it was sucessfully placed, continue placing ships
                    all.push_back(newcur);
                    if (ship < game().nShips()){
                        ship++;
                    }
                    if (pathExists(all, b, ship)){
                        //pathExists(all, b, ship);
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
        }
        
    } // end of if
    
    if(!backtrack(all, b, ship)){
        return false;
    }
    
    return true;
}

bool MediocrePlayer::pathExists(vector<Point>&all, Board& b, int ship){
    
    // base
    // if (ship < 0 || ship > game().nShips()){
    if (ship == game().nShips()){
        return true;
    }
    
    int tempship = ship;
    bool done = false;
    
    for (int r = 0; r < game().rows() && !done; r++){
        for (int c = 0; c < game().cols(); c++){
            Point cur(r, c);
            if (b.placeShip(cur, ship, HORIZONTAL) || b.placeShip(cur, ship, VERTICAL)){
                all.push_back(cur);

                // ALL SIZE SHOULD BE THE SAME OR EQUAL TO NUMBER OF SHIPS***
                if (ship < game().nShips()){
                    ship++;
                }
                
                if (!pathExists(all, b, ship)){
                    return false;
                }
                done = true;
                break;
            }
        }
        
    } // end of last for loop
    
    // if (tempship == ship && ship != game().nShips()){
    if (tempship == ship){
        if(!backtrack(all, b, ship)){
            return false;
        }
    }
   
    return true;
    
}

bool MediocrePlayer::repeat(Point rand){
    for (int i = 0; i < m_points.size(); i++){
        if (rand.r == m_points[i].r && rand.c == m_points[i].c){
            return true;
        }
    }
    
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    for (int a = 0; a < game().nShips(); a++){
        if (game().shipLength(a) > 5){
            m_state = 2;
            break;
        }
    }
    
    // NOTE: STATE 2 CHECK FOR NO REPEATS EITHER (IN THE CASE OF LENGTH 6 SHIPS)
    if (m_state == 2){
        Point randinbounds(-1, -1);
        
        // try every row
        for (int mr = m_transition.r-4; mr < 8; mr++){
            randinbounds.r = mr;
            randinbounds.c = m_transition.c;
            if (game().isValid(randinbounds)){
                cross.push_back(randinbounds);
            }
        }
        
        // try every col
        for (int mc = m_transition.c-4; mc < 8; mc++){
            randinbounds.c = mc;
            randinbounds.r = m_transition.r;
            if (game().isValid(randinbounds)){
                cross.push_back(randinbounds);
            }
        }
        
        /* Testing
         for (int k = 0; k < cross.size(); k++){
         cout << "cross " << cross[k].r << cross[k].c << " ";
         }
         
         cout << endl << endl;
         
         for (int h = 0; h < m_points.size(); h++){
         cout << "points " << m_points[h].r << m_points[h].c << " ";
         }
         */
        
        for (int j = 0; j < cross.size(); j++){
            if (!repeat(cross[j])){
                m_points.push_back(cross[j]);
                return cross[j];
            }
        }
    }
    
    cross.clear();
    
    // if state 1, return a random point that has not been chosen before
    // if everything in the cross was hit, revert to state 1
    // if (m_state == 1) {
    Point rand = game().randomPoint();
    int counter = 0;
    
    for (int i = 0; i < m_points.size(); i++){
        // keeps generating a new point if point is already present
        while (rand.r == m_points[i].r && rand.c == m_points[i].c){
            rand = game().randomPoint();
            i = 0;
            counter++;
        }
    }
    
    m_points.push_back(rand);
    
    return rand;
    
    // return m_lastCellAttacked;
    
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    // P would be the rand point returned from recommendAttack
    
    // if it is a valid shot and it missed
    if (validShot && !shotHit){
        m_state = 1;
    }
    
    if (validShot && shotHit && shipDestroyed){
        m_state = 1;
    }
    
    // hit a ship but did not destroy it
    if (validShot && shotHit && !shipDestroyed){
        
        // point that caused the transition from state 1 to state 2
        if (m_state == 1){
            m_transition = p;
        }
        
        m_state = 2;
    }
    
    // as long as there is one ship with length 6, follow this procedure
    // only after each position within a radius of 4 was hit, switch to case 1
    for (int i = 0; i < game().nShips(); i++){
        if (game().shipLength(i) > 5){
            
        }
    }
    
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
    // Function does NOTHING for a Mediocre Player
}



//*********************************************************************
//  GoodPlayer
//*********************************************************************
//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer() {}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
    bool putintoquad(int quad, int midrow, int midcol, int rows, int cols, int shipid, Board& b);
    int randInt(int start, int limit);
    bool repeat(Point rand);
    int getEvenNum (int randStart, int randEnd);
    Point generateEvenPoint (int rStart, int rEnd, int cStart, int cEnd);
    int findDirection (Point one, Point two);
private:
    Point m_lastCellAttacked;
    Point m_lastCellTried;
    int m_state;
    int m_try;
    int m_firsttry;
    int nextPoint;
    Point m_transition;
    vector <Point> m_points;
    vector <Point> cross;
    bool firstattempt;
    Point middle;
    Point consecutivepoint;
    bool switchdir;
    bool didhit;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0), m_state(1), nextPoint(1), m_transition(0, 0), firstattempt(true), m_try(0), m_lastCellTried(0,0), switchdir(false), didhit(false)
{}

int GoodPlayer::randInt(int start, int limit){
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distro(start, limit-1);
    return distro(generator);
}

// a call to recommendAttack, then Board::attack, then recordAttackResult must not take more than 5 seconds

bool GoodPlayer::placeShips(Board& b)
{
    int rows = game().rows();
    int cols = game().cols();
    int ships = game().nShips();
    Direction dir;
    list <int> quad;
    bool check = 0;
    int count = 0;

    // ?
    int num = 0;
    int midrow = (rows / 2);
    int midcol = (cols / 2);
    
    for (int i = 1; i < 5; i ++){
        quad.push_back(i);  // 4 quads
    }
    
    for (int i = 0; i < game().nShips(); i++){
        check = 0;
        
        if (count < 100){
            while (!check){
                Point initialPlacement(randInt(0, rows), randInt(0, cols));
                int d = randInt(0, 2);
                if (d == 0){
                    dir = HORIZONTAL;
                }
                else if (d == 1){
                    dir = VERTICAL;
                }
                
                if (b.placeShip(initialPlacement, i, dir)){
                    check = 1;
                }
                count++;
            }
        }
        
    }
    
    return true;
}

bool GoodPlayer::repeat(Point rand){
    for (int i = 0; i < m_points.size(); i++){
        if (rand.r == m_points[i].r && rand.c == m_points[i].c){
            return true;
        }
    }
    
    return false;
}

int GoodPlayer::findDirection (Point one, Point two){
    if (one.r == two.r && one.c > two.c){
        //EAST
        return 2;
    }
    
    else if (one.r == two.r && one.c < two.c){
        // WEST
        return 4;
    }
    
    else if (one.c == two.c && one.r < two.r){
        // NORTH
        return 1;
    }
    
    else if (one.c == two.c && one.r > two.c){
        // SOUTH
        return 3;
    }
    
    // need to fix***
    return randInt(1, 5);
    
}

int GoodPlayer::getEvenNum (int randStart, int randEnd) {
    int num = 1;
    
    while (num % 2 != 0 ) {
        num = randInt(randStart, randEnd);
    }
    
    return num;
}

Point GoodPlayer::generateEvenPoint (int rStart, int rEnd, int cStart, int cEnd) {
    Point temp(getEvenNum(rStart, rEnd), getEvenNum(cStart, cEnd));
    
    while (repeat(temp)) {
        temp.r = getEvenNum(rStart, rEnd);
        temp.c = getEvenNum(cStart, cEnd);
    }
    
    return temp;
}

Point GoodPlayer::recommendAttack()
{
    // think of board as a checkerboard, pick even coords
    int midrow = (game().rows() / 2);
    int midcol = (game().cols() / 2);
    int row = game().rows();
    int col = game().cols();
    
    // valid shot but missed or just destroyed a ship
    if (m_state == 1) {

        Point inQuad(0, 0);
        
        switch (nextPoint) {
            case 1:
                inQuad = generateEvenPoint(0, midrow, 0, midcol);
                nextPoint = 2;
                break;
                
            case 2:
                inQuad = generateEvenPoint(midrow, row, 0, midcol);
                nextPoint = 3;
                break;
                
            case 3:
                inQuad = generateEvenPoint(midrow, row, midcol, col);
                nextPoint = 4;
                break;
                
            case 4:
                inQuad = generateEvenPoint(0, midrow, midcol, col);
                nextPoint = 1;
                break;
                
            default:
                cout << "Error: GoodPlayer recommendAttack() nextPoint out of bounds" << endl;
                break;
        }
        
        m_points.push_back(inQuad);
        return inQuad;
    }
    
    // hit ship but not destroyed
    else if (m_state == 2) { }
    Point temp(0, 0);
    
    return temp;
    
}


void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId )
{
    // P would be the rand point returned from recommendAttack
    
    // if it is a valid shot and it missed
    if (validShot && !shotHit){
        m_state = 1;
    }
    
    if (validShot && shotHit && shipDestroyed){
        m_state = 1;
    }
 
    // hit a ship but did not destroy it
    if (validShot && shotHit && !shipDestroyed){
        
        // point that caused the transition from state 1 to state 2 (first hit of a ship)
        if (m_state == 1){
            
            // find direction and record point
            m_transition = p;
        }
        
        if (m_state == 2) {
            firstattempt = true;
            didhit = true;
            // DO NOT CHANGE DIRECTIONS
            consecutivepoint = p;
        }
        
        m_state = 2;
    }
    
    // ----------------------------------------------------
    
    // as long as there is one ship with length 6, follow this procedure
    // only after each position within a radius of 4 was hit, switch to case 1
    for (int i = 0; i < game().nShips(); i++){
        if (game().shipLength(i) > 5){
            
        }
    }
    
    
    /*
     // if it is a valid shot and it missed
     if (validShot && !shotHit){
     
     // hit before, missed next
     if (m_state == 2){
     // go back and try another direction
     p = m_lastCellAttacked;
     m_try++;
     switchdir = true;
     return;
     }
     
     if (m_firsttry > 12){
     m_firsttry = 0;
     }
     
     m_state = 1;
     firstattempt = false;
     m_firsttry++;
     }
     
     // if it hit and destroyed a ship
     if (validShot && shotHit && shipDestroyed){
     m_state = 3;
     // reset middle, cause will be going back to state 1
     middle = m_lastCellTried;
     m_try = 0;
     }
     
     // hit a ship but did not destroy it
     if (validShot && shotHit && !shipDestroyed){
     
     if (m_try > 3){
     m_try = 0;
     }
     
     // point that caused the transition from state 1 to state 2
     if (m_state == 1 || m_state == 3){
     m_transition = p;
     }
     
     if (m_state == 2) {
     firstattempt = true;
     didhit = true;
     // DO NOT CHANGE DIRECTIONS
     consecutivepoint = p;
     }
     
     m_state = 2;
     // if came from state 1 --> lastcelltried is the one that was from the formation???
     // we basically need the last point returned before the call
     m_lastCellTried = p;
     // reset
     m_try = 0;
     firstattempt = true;
     switchdir = false;
     
     
     }
     
     
     */
}

void GoodPlayer::recordAttackByOpponent(Point p)
{

}




//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
         type != types[pos]; pos++)
        ;
    switch (pos)
    {
        case 0:  return new HumanPlayer(nm, g);
        case 1:  return new AwfulPlayer(nm, g);
        case 2:  return new MediocrePlayer(nm, g);
        case 3:  return new GoodPlayer(nm, g);
        default: return nullptr;
    }
}
