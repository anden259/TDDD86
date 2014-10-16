#include "view.h"

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

void TView::displayView(const sensors &s)
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
