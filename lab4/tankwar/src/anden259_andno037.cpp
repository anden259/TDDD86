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

    if ((s.myMines != 0) && (mineMyBaseLocations(s).size() > 0)) {
        return mineLocation(s, mineMyBaseLocations(s).front());
    } else {
        return goToLocation(s, s.oppBase);
        //return goToOppBase(s);
    }
    //return goToMyBase(s);

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

bool anden259_andno037::isOkToMove(const location &to)
{
    return isOkToMove(to.r, to.c);
}

bool anden259_andno037::isOkToMove(const int r, const int c)
{
    const viewElements loc = board.getView(r, c);

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
        //return goToLocationStupid(s, to);
        return goToLocation(s, to);
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
list<location> anden259_andno037::reconstructPath(map<location, location, classCompLocation> &cameFrom, const location &current) {
    bool is=false;
    list<location> locationList;
//    for (auto llit : cameFrom){
//        if(llit.first == current){
//            //cout <<"tjena !!!\n";
//            is=true;
//            break;
//        }
//    }
    if(cameFrom.find(current)!=cameFrom.end()){
        is=true;
    }
    if(is){
        locationList = reconstructPath(cameFrom, cameFrom[current]);

    }

    locationList.push_back(current);
    return locationList;
}



list<location> anden259_andno037::aStar(const sensors &s, const location& to)
{
    set<location, classCompLocation> closedset;
    set<location, classCompLocation> openset {s.me};
    // <location,location> cameFrom;
    map< location, location, classCompLocation> cameFrom;

    map< location, int, classCompLocation> gScore {pair<location, int>(s.me, 0)};
    map< location, int, classCompLocation> fScore {pair<location, int>(s.me , gScore[s.me] + Board::getLine(s.me, to).size())};

    while (!openset.empty()) {
        location current = *(openset.begin());
        for (auto loc : openset) {
            if (gScore[loc] < gScore[current]) {
                current = loc;
            }
        }

        if (current == to) {

            return reconstructPath(cameFrom, to);
        }

        openset.erase(current);
        closedset.insert(current);
        location neighbor;
        for (int r = -1; r <= 1 ; ++r) {
            for (int c = -1; c <= 1; ++c) {
                neighbor.r = current.r + r;
                neighbor.c = current.c + c;
                if (!(r == 0 && c == 0) && isOkToMove(neighbor)) {

                    if (closedset.find(neighbor) != closedset.end()) {
                        continue;
                    }
                    int tempGscore = gScore[current] + 1;

                    if ((openset.find(neighbor) == openset.end()) || (tempGscore < gScore[neighbor])) {
                        if(s.me != current)
                            cameFrom[neighbor] = current;
                        gScore[neighbor] = tempGscore;
                        fScore[neighbor] = gScore[neighbor] + Board::getLine(neighbor, to).size();
                        if (openset.find(neighbor) == openset.end()) {
                            openset.insert(neighbor);
                        }
                    }
                }
            }
        }
    }
    cout << "no path\n";
    return list<location> {};

}

action anden259_andno037::goToLocation(const sensors &s, const location& to)
{
    action move;
    list<location> myPath = aStar(s, to);
    //myPath.reverse();

    //myPath.pop_front();


    //cout <<"me "<< s.me.r << "  " << s.me.c <<"\n";

//    for (auto x:myPath){
//    cout << x.r << "  " << x.c <<"\n";
//    }



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
