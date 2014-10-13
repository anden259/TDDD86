/*
 * anden259
 * andno037
 *
 */
#include "anden259_andno037.h"
#include "view.h"


anden259_andno037::anden259_andno037():
    Tank {} {
    this->name = "anden259_andno037";
}

void anden259_andno037::update_view(const sensors &s)
{


}

void anden259_andno037::update_status(const sensors &s)
{


    board.set_status(s.me.r-1,s.me.c-1,s.look[0]);
    board.set_status(s.me.r-1,s.me.c,s.look[1]);
    board.set_status(s.me.r-1,s.me.c+1,s.look[2]);


    board.set_status(s.me.r,s.me.c-1,s.look[3]);
    board.set_status(s.me.r,s.me.c,s.look[4]);
    board.set_status(s.me.r,s.me.c+1,s.look[5]);

    board.set_status(s.me.r+1,s.me.c-1,s.look[6]);
    board.set_status(s.me.r+1,s.me.c,s.look[7]);
    board.set_status(s.me.r+1,s.me.c+1,s.look[8]);


}



action anden259_andno037::doYourThing(const sensors &s)
{
    update_status(s);
    board.display_view();
    return goToMyBase(s);


    // right now calls base class fireAtOpp method -
    // feel free to override that method
    // with one that fires at where you think they will be
}

string anden259_andno037::taunt(const string &otherguy) const
{
    return "TJENA!! " + otherguy;
}

action anden259_andno037::mineLocation(const sensors &s,location to) {
    list<location> myPath;
    myPath = Board::getLine(s.me, to);

    // TODO check if mined
    if(myPath.size()==0){
        // TODO set mine
    }else{
        // TODO move to location
    }


}


action anden259_andno037::goToMyBase(const sensors &s) {
    action move;
    list<location> myPath;
    myPath = Board::getLine(s.me, s.myBase);
    list<location>::iterator i;

    if (s.me != s.myBase) {
        i = myPath.begin();
        if ((i->c == s.me.c-1) && (i->r == s.me.r-1)) move.theMove = moveNW;
        if ((i->c == s.me.c+1) && (i->r == s.me.r-1)) move.theMove = moveNE;
        if ((i->c == s.me.c) && (i->r == s.me.r-1)) move.theMove = moveN;
        if ((i->c == s.me.c-1) && (i->r == s.me.r+1)) move.theMove = moveSW;
        if ((i->c == s.me.c+1) && (i->r == s.me.r+1)) move.theMove = moveSE;
        if ((i->c == s.me.c) && (i->r == s.me.r+1)) move.theMove = moveS;
        if ((i->c == s.me.c-1) && (i->r == s.me.r)) move.theMove = moveW;
        if ((i->c == s.me.c+1) && (i->r == s.me.r)) move.theMove = moveE;
    }
    else {
        move.theMove = sit;
    }
    return move;
}
