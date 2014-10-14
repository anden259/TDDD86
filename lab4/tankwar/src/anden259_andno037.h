#ifndef ANDEN259_ANDNO037_H
#define ANDEN259_ANDNO037_H

/*
 * anden259
 * andno037
 *
 */

#include <map>
#include <vector>
#include <set>


#include "Board.h"
#include "Tank.h"
#include "view.h"

using namespace anden259_andno037_N;

struct classCompLocation {
  bool operator() (const location& lhs, const location& rhs) const
  {return pair<int,int>(lhs.r,lhs.c) < pair<int,int>(rhs.r,rhs.c);}
};


class anden259_andno037 : public Tank
{

public:
    anden259_andno037();
    ~anden259_andno037() = default;

    /*
     * this simple player class fires at the enemy every turn (regardless of ammo remaining)
     * and misses every time the other player is moving since it fires at where the enemy is, and
     * not where the enemy will be. Keep in mind that after you fire, the opponent gets a turn
     * (to possibly move out of the way), and then the bullet hits. This class uses the base class
     * fireAtOpp(sensors) method to fire at the current position of the enemy. Please note that
     * this only has the possibility of hitting the enemy if they 'sit' on their subsequent turn.
     */
    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;


private:

    void update_view(const sensors &s);
    void update_status(const sensors &s);
    action goToLocation(const sensors &s, const location &to);
    action goToMyBase(const sensors &s);
    action mineLocation(const sensors &s, location to);
    list<location> mineMyBaseLocations(const sensors &s);
    list<location> aStar(const sensors &s, const location& loc);
    action goToLocationStupid(const sensors &s, const location& to);
    list<location> reconstructPath(map<location, location, classCompLocation> &cameFrom, const location& current);

    bool isOkToMove(const location &to);
    bool isOkToMove(const int r, const int c);

    // any data that I want to retain for my tank - probably want to store some tank state information
    TView board;



};






#endif // ANDEN259_ANDNO037_H
