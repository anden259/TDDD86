#ifndef VIEW_H
#define VIEW_H

#include<vector>
#include "Board.h"

namespace anden259_andno037_N {




struct view_elements {
    bool unknown{true};
    bool mine{false};
    bool op_path{false};
    bool pot_mine{false};
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
    void set_mine(int r,int c);
    void display_view();
    status get_status(const location& loc);

};

}
#endif // VIEW_H
