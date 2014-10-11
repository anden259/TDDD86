#ifndef VIEW_H
#define VIEW_H

#include<vector>
#include "Board.h"


struct view_elements {
    bool unknown;
    bool mine;
    bool op_path;
    bool pot_mine;
    status stat;

};

class TView
{
    std::vector<std::vector<view_elements> > board;
public:
    TView();
    ~TView() = default;
    void set_status(const location& loc, const status &s);
    void set_status(int r, int c, const status &s);
    status get_status(const location& loc);

};


#endif // VIEW_H
