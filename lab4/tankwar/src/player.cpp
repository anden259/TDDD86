/*
 * Implements the behavior in player.h
 */

#include "player.h"

player::player() {
    name = "Turd Furgason";
}

action player::doYourThing (const sensors &s) {
    action move;


    if(s.turn%2==0){
        move.theMove=mine;
    }else{
        move.theMove=moveS;
    }

    return move;


    //return fireAtOpp(s);  // right now calls base class fireAtOpp method -
                            // feel free to override that method
                            // with one that fires at where you think they will be
}

string player::taunt(const string &otherguy) const{
    return "You couldn't code your way out of a wet paper bag, " + otherguy;
}
