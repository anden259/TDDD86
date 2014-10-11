#include "view.h"

TView::TView()
{

}




void TView::set_status(const location& loc, const status& s)
{

    board[loc.r][loc.c].stat = s;

}

void TView::set_status(int r,int c, const status& s)
{

    board[r][c].stat = s;

}

status TView::get_status(const location& loc)
{
    return board[loc.r][loc.c].stat;
}
