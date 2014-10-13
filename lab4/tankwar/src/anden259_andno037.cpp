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


    board.setStatus(s.me.r - 1, s.me.c - 1, s.look[0]);
    board.setStatus(s.me.r - 1, s.me.c, s.look[1]);
    board.setStatus(s.me.r - 1, s.me.c + 1, s.look[2]);


    board.setStatus(s.me.r, s.me.c - 1, s.look[3]);
    board.setStatus(s.me.r, s.me.c, s.look[4]);
    board.setStatus(s.me.r, s.me.c + 1, s.look[5]);

    board.setStatus(s.me.r + 1, s.me.c - 1, s.look[6]);
    board.setStatus(s.me.r + 1, s.me.c, s.look[7]);
    board.setStatus(s.me.r + 1, s.me.c + 1, s.look[8]);


}



action anden259_andno037::doYourThing(const sensors &s)
{
    update_status(s);
    //board.display_view();
    if (mineMyBaseLocations(s).size() > 0) {
        return mineLocation(s, mineMyBaseLocations(s).front());
    } else {
        return goToOppBase(s);
    }


    // right now calls base class fireAtOpp method -
    // feel free to override that method
    // with one that fires at where you think they will be
}

string anden259_andno037::taunt(const string &otherguy) const
{
    return "TJENA!! " + otherguy;
}


list<location> anden259_andno037::mineMyBaseLocations(const sensors &s)
{
    list<location> mineLocations;
    location tmp;
    // TODO värdera om det är värt att lägga minna på obs.
    for (int r = -1; r <= 1; ++r) {
        for (int c = -1; c <= 1 ; ++c) {
            if (isOkToMove(s.myBase.r + r, s.myBase.c + c) && !(r == 0 && c == 0)) {
                tmp.r = s.myBase.r + r;
                tmp.c = s.myBase.c + c;
                mineLocations.push_back(tmp);
            }
        }
    }

    return mineLocations;
}

bool anden259_andno037::isOkToMove(const location to)
{
    return isOkToMove(to.r, to.c);
}
bool anden259_andno037::isOkToMove(int r, int c)
{
    viewElements loc = board.getView(r, c);

    if (loc.unknown) return true;
    if (loc.mine) return false;

    if (loc.pot_mine)return false;//TODO !!!

    switch (loc.stat) {
    case edge:
        return false;
        break;
    case obs:
        return false;
        break;
    default:
        return true;
        break;
    }
    return true;

}



action anden259_andno037::mineLocation(const sensors &s, location to)
{
    action doMine;
    list<location> myPath;
    myPath = Board::getLine(s.me, to);

    // TODO check if mined
    if (myPath.size() == 0) {
        doMine.theMove = mine;
        board.setMine(s.me);
        return doMine;
    } else {
        return goToLocationStupid(s, to);
    }

}

action anden259_andno037::goToLocationStupid(const sensors &s, const location& to)
{
    action move;
    list<location> myPath;
    myPath = Board::getLine(s.me, to);
    list<location>::iterator i;

    if (s.me != to) {
        i = myPath.begin();
        if ((i->c == s.me.c - 1) && (i->r == s.me.r - 1)) move.theMove = moveNW;
        if ((i->c == s.me.c + 1) && (i->r == s.me.r - 1)) move.theMove = moveNE;
        if ((i->c == s.me.c) && (i->r == s.me.r - 1)) move.theMove = moveN;
        if ((i->c == s.me.c - 1) && (i->r == s.me.r + 1)) move.theMove = moveSW;
        if ((i->c == s.me.c + 1) && (i->r == s.me.r + 1)) move.theMove = moveSE;
        if ((i->c == s.me.c) && (i->r == s.me.r + 1)) move.theMove = moveS;
        if ((i->c == s.me.c - 1) && (i->r == s.me.r)) move.theMove = moveW;
        if ((i->c == s.me.c + 1) && (i->r == s.me.r)) move.theMove = moveE;
    } else {
        move.theMove = sit;
    }
    return move;
}

list<location> anden259_andno037::aStar(const sensors &s, const location& to)
{
    vector<location> closedset;
    vector<location> openset{s.me};
    map <location,location> cameFrom;


}

action anden259_andno037::goToLocation(const sensors &s, const location& to)
{
    action move;
    list<location> myPath = aStar(s,to);



    list<location>::iterator i;

    if (s.me != to) {
        i = myPath.begin();
        if ((i->c == s.me.c - 1) && (i->r == s.me.r - 1)) move.theMove = moveNW;
        if ((i->c == s.me.c + 1) && (i->r == s.me.r - 1)) move.theMove = moveNE;
        if ((i->c == s.me.c) && (i->r == s.me.r - 1)) move.theMove = moveN;
        if ((i->c == s.me.c - 1) && (i->r == s.me.r + 1)) move.theMove = moveSW;
        if ((i->c == s.me.c + 1) && (i->r == s.me.r + 1)) move.theMove = moveSE;
        if ((i->c == s.me.c) && (i->r == s.me.r + 1)) move.theMove = moveS;
        if ((i->c == s.me.c - 1) && (i->r == s.me.r)) move.theMove = moveW;
        if ((i->c == s.me.c + 1) && (i->r == s.me.r)) move.theMove = moveE;
    } else {
        move.theMove = sit;
    }
    return move;
}





action anden259_andno037::goToMyBase(const sensors &s)
{

    return goToLocation(s, s.myBase);
}
