#ifndef VIEW_H
#define VIEW_H

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


    void displayView();
    void displayView(const sensors& s);
    void clear();
    void initView();
    status getStatus(int r,int c);
    status getStatus(const location& loc);

    viewElements getView(const location& loc);
    viewElements getView(int r,int c);


};

}
#endif // VIEW_H
