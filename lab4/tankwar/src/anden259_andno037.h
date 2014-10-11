#ifndef ANDEN259_ANDNO037_H
#define ANDEN259_ANDNO037_H

/*
 * anden259
 * andno037
 *
 */


#include "Board.h"
#include "Tank.h"
#include "view.h"

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

    // any data that I want to retain for my tank - probably want to store some tank state information
    TView board;



};






#endif // ANDEN259_ANDNO037_H