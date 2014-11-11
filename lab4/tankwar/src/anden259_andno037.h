/*
 * anden259
 * andno037
 *
 */

#ifndef ANDEN259_ANDNO037_H
#define ANDEN259_ANDNO037_H

#include<vector>
#include "Board.h"

namespace anden259_andno037_N {

struct viewElements {
    bool unknown{true};
    bool mine{false};
    bool op_path{false};
    bool pot_mine{false};
    status stat;
};

class TView
{
    std::vector<std::vector<viewElements> > board;
public:
    TView() = default;
    ~TView() = default;
    void setStatus(const location& loc, const status &s);
    void setStatus(int r, int c, const status &s);
    void setMine(int r,int c);
    void setMine(const location& loc);

    void setPotentialMine(int r,int c);
    void setPotentialMine(const location& loc);

    void setOppPath(int r,int c);
    void setOppPath(const location& loc);

    void displayView(const sensors);
    void clear();
    void initView();
    status getStatus(int r,int c);
    status getStatus(const location& loc);

    viewElements getView(const location& loc);
    viewElements getView(int r,int c);
};

}

// ---------------------------------------------------------------------------- //

#include <map>
#include <vector>
#include <set>
#include <random>

#include "Board.h"
#include "Tank.h"


using namespace anden259_andno037_N;

struct classCompLocation {
    bool operator()(const location& lhs, const location& rhs) const
    {
        return pair<int, int>(lhs.r, lhs.c) < pair<int, int>(rhs.r, rhs.c);
    }
};


class anden259_andno037 : public Tank
{

public:
    anden259_andno037();
    ~anden259_andno037() = default;

    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;

private:

    void update_status(const sensors &s);

    list<location> mineMyBaseLocations(const sensors &s);
    list<location> aStar(const sensors &s, const location& loc);
    list<location> reconstructPath(map<location, location, classCompLocation> &cameFrom, const location& current);

    list<location> okNeighbors(const location& loc, const sensors& s);

    action randomStep(const sensors &s);
    action pillageAndDodge(const sensors &s);

    action goToLocationStupid(const sensors &s, const location& to);
    action goToLocation(const sensors &s, const location &to);
    action goToMyBase(const sensors &s);
    action mineLocation(const sensors &s, location to);

    bool isOkToMove(const location &to, const sensors& s);
    bool isOkToMove(const int r, const int c, const sensors& s);
    bool isMoveable(const int r, const int c, const sensors& s);
    bool isMoveable(const location &loc, const sensors& s);

    static int calcDistance(const location& from, const location& to);
    int cost(const int r, const int c);
    int cost(const location &loc);

    TView board;
    size_t matchNumber {0};
    map<size_t , map<size_t , const sensors> > historyMap;
};

#endif // ANDEN259_ANDNO037_H
