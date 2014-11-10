/*
 * anden259
 * andno037
 *
 */
#include "anden259_andno037.h"


namespace anden259_andno037_N
{

void TView::initView()
{
    board.reserve(BOARD_ROWS);
    board.clear();
    for (int r = 0; r < BOARD_ROWS; ++r) {
        board.push_back(std::vector<viewElements> {});
        board[r].reserve(BOARD_COLS);
        for (int c = 0; c < BOARD_COLS; ++c) {
            board[r].push_back(viewElements {});
            if ((r == 0) || (c == 0) || (r == (BOARD_ROWS - 1)) || (c == (BOARD_COLS - 1))) {
                board[r][c].unknown = false;
                board[r][c].stat = edge;
            } else {
                board[r][c].stat = nada;
            }
        }
    }
}


void TView::setStatus(const location& loc, const status& s)
{
    setStatus(loc.r, loc.c, s);
}

void TView::setStatus(int r, int c, const status& s)
{
    board[r][c].stat = s;
    board[r][c].unknown = false;
}

status TView::getStatus(const location& loc)
{
    return getStatus(loc.r, loc.c);
}

status TView::getStatus(int r, int c)
{
    return board[r][c].stat;
}

void TView::setMine(const location& loc)
{
    setMine(loc.r, loc.c);
}

void TView::setMine(int r, int c)
{
    board[r][c].mine = true;
}
void TView::setPotentialMine(const location& loc)
{
    setPotentialMine(loc.r, loc.c);
}

void TView::setPotentialMine(int r, int c)
{
    board[r][c].unknown = false;
    board[r][c].pot_mine = true;
}

void TView::setOppPath(const location& loc)
{
    setOppPath(loc.r, loc.c);
}

void TView::setOppPath(int r, int c)
{
    board[r][c].unknown = false;
    board[r][c].op_path = true;
}

viewElements TView::getView(const location& loc)
{
    return getView(loc.r, loc.c);
}

viewElements TView::getView(int r, int c)
{
    return board[r][c];
}

void TView::displayView(const sensors)
{
    for (int row = 0; row < BOARD_ROWS; row++) {
        for (int col = 0; col < BOARD_COLS; col++) {

            if (board[row][col].mine) {
                cout << '^';
            } else if (board[row][col].pot_mine) {
                cout << 'v';
            } else if (board[row][col].op_path) {
                cout << 'X';
            } else if (board[row][col].unknown) {
                cout << '.';
            } else {

                switch (board[row][col].stat) {
                case  edge     : {
                    std::cout << "=";
                } break;
                case  obs      : {
                    std::cout << "*";
                } break;
                case  nada     : {
                    std::cout << " ";
                } break;
                case pu_ammo   : {
                    std::cout << "b";
                } break;
                case pu_mines  : {
                    std::cout << "m";
                } break;
                case pu_points : {
                    std::cout << "p";
                } break;
                default: std::cout << "ERROR displaying board - an unknown status was stored in square " << row << "," << col;

                }
            }
        }
        std::cout << "\n";
    }
    std::cout << endl;
}




void TView::clear()
{
    initView();
}


}

// ---------------------------------------------------------------------------- //

anden259_andno037::anden259_andno037():
    Tank {} {
    this->name = "anden259_andno037";
}

// update the view of the bord
// and save the info of this round for later use
void anden259_andno037::update_status(const sensors &s)
{

    historyMap[matchNumber].insert(pair<size_t, const sensors>(s.turn, s));
    board.setOppPath(s.opp);
// to see if the oppnent is siting
    if (s.turn != 1 && historyMap[matchNumber][s.turn - 1].opp == s.opp) {
        board.setPotentialMine(s.opp);
    }

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
// things the tank can do
action anden259_andno037::doYourThing(const sensors &s)
{
    if (s.turn == 1) {
        ++matchNumber;
        historyMap[matchNumber] = map<size_t, const sensors> {};
        board.clear();
    }

    // TODO: pot_mine, opp_path
    update_status(s);
    //board.displayView(s);
    if ((s.myMines != 0) && (mineMyBaseLocations(s).size() > 0)) {
        return mineLocation(s, mineMyBaseLocations(s).front());
    } else {

        return pillageAndDodge(s);
    }
}

string anden259_andno037::taunt(const string &otherguy) const
{
    return "TJENA!! " + otherguy;
}

// return a list of all the neighbors a round the base you can put a mine
list<location> anden259_andno037::mineMyBaseLocations(const sensors &s)
{
    return okNeighbors(s.myBase, s);
}


// return a list of all the neighbor a round loc you can move to
list<location> anden259_andno037::okNeighbors(const location& loc, const sensors& s)
{
    list<location> locations;
    location tmp;
    for (int r = -1; r <= 1; ++r) {
        for (int c = -1; c <= 1 ; ++c) {
            if (isOkToMove(loc.r + r, loc.c + c, s) && !(r == 0 && c == 0)) {
                tmp.r = loc.r + r;
                tmp.c = loc.c + c;
                locations.push_back(tmp);
            }
        }
    }

    return locations;
}

// is the location moveable at all
bool anden259_andno037::isMoveable(const int r, const int c, const sensors &s)
{
    if (s.opp.r == r && s.opp.c == c) return false;
    //if (board.getView(r, c).unknown) return true;
    if (board.getView(r, c).stat == edge) return false;
    return true;
}

bool anden259_andno037::isMoveable(const location &loc, const sensors &s)
{
    return isMoveable(loc.r, loc.c, s);
}

int anden259_andno037::cost(const location &loc)
{
    return cost(loc.r, loc.c);
}

// wat is the cost to walk on this location
int anden259_andno037::cost(const int r, const int c)
{
    const viewElements loc = board.getView(r, c);

    if (loc.pot_mine) {
        return 5000;
    } else if (loc.mine) {
        return 5000;
    } else if (loc.unknown) {
        return 1;
    } else {

        switch (loc.stat) {
        case  edge     : {
            return 1000000000;
        } break;
        case  obs      : {
            return 100;
        } break;
        case  nada     : {
            return 2;
        } break;
        case pu_ammo   : {
            return -5;
        } break;
        case pu_mines  : {
            return -5;
        } break;
        case pu_points : {
            return -5;
        } break;
        default: std::cout << "ERROR displaying board - an unknown status was stored in square " << r << "," << c;
            return 0;
        }
    }
}


bool anden259_andno037::isOkToMove(const location &to, const sensors &s)
{
    return isOkToMove(to.r, to.c, s);
}

// is it ok to move to this location
bool anden259_andno037::isOkToMove(const int r, const int c, const sensors &s)
{
    const viewElements loc = board.getView(r, c);

    if (!isMoveable(r,c ,s)){
        return false;
    }
    if (loc.mine) return false;
    if (loc.pot_mine)return false;//TODO !!!
    if (loc.unknown) return true;

    switch (loc.stat) {
    case obs:
        return false;
        break;
    default:
        return true;
        break;
    }

    return true;
}


// mine this location if not there go to the location.
action anden259_andno037::mineLocation(const sensors &s, location to)
{
    // TODO check if mined
    if (s.me == to) {
        action doMine;
        doMine.theMove = mine;
        board.setMine(s.me);
        return doMine;
    } else {
        //return goToLocationStupid(s, to);
        return goToLocation(s, to);
    }

}

// go to a location in a line
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

// creating a list of the locations to your goal.
list<location> anden259_andno037::reconstructPath(map<location, location, classCompLocation> &cameFrom, const location &current)
{
    bool is = false;
    list<location> locationList;
    if (cameFrom.find(current) != cameFrom.end()) {
        is = true;
    }
    if (is) {
        locationList = reconstructPath(cameFrom, cameFrom[current]);
    }

    locationList.push_back(current);
    return locationList;
}


int anden259_andno037::calcDistance(const location& from, const location& to)
{
    return max((from.r - to.r) * (from.r - to.r), (from.c - to.c) * (from.c - to.c));
}

// astar implementation to find you way.
list<location> anden259_andno037::aStar(const sensors &s, const location& to)
{
    set<location, classCompLocation> closedset;
    set<location, classCompLocation> openset {s.me};
    map< location, location, classCompLocation> cameFrom;

    map< location, int, classCompLocation> gScore {pair<location, int>(s.me, 0)};
    map< location, int, classCompLocation> fScore {pair<location, int>(s.me , gScore[s.me] +  calcDistance(s.me, to))}; // Board::getLine(s.me, to).size())};

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
                if (!(r == 0 && c == 0) && isMoveable(neighbor, s)) {

                    if (closedset.find(neighbor) != closedset.end()) {
                        continue;
                    }
                    int tempGscore = gScore[current] + cost(neighbor);

                    if ((openset.find(neighbor) == openset.end()) || (tempGscore < gScore[neighbor])) {
                        if (s.me != current)
                            cameFrom[neighbor] = current;
                        gScore[neighbor] = tempGscore;
                        fScore[neighbor] = gScore[neighbor] + calcDistance(s.me, to); // Board::getLine(neighbor, to).size();
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

// move with a star.
action anden259_andno037::goToLocation(const sensors &s, const location& to)
{
    action move;
    list<location> myPath = aStar(s, to);

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

// move to opponent base in random step
action anden259_andno037::pillageAndDodge(const sensors &s)
{
    int ran = (rand() % 100) + 1;

    if (calcDistance(s.me,s.myBase)<10 || ran > 35) {
        return goToLocation(s, s.oppBase);
    } else {
        return randomStep(s);
    }
}

// move a random step
action anden259_andno037::randomStep(const sensors &s)
{
    list<location> neighborList = okNeighbors(s.me, s);
    if (neighborList.empty()) {        
        return goToLocation(s, s.me);
    }
    int ran = rand() % neighborList.size();

    for (int i = 0; i < ran; ++i) {
        neighborList.pop_back();
    }
    return goToLocation(s, neighborList.back());
}

action anden259_andno037::goToMyBase(const sensors &s)
{

    return goToLocation(s, s.myBase);
}
