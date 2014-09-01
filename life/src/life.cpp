// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "grid.h"
#include "lifeutil.h"
#include <fstream>
using namespace std;

template <typename T>
void print_grid(Grid<T>& grid){
    for(int r=0;r<grid.numRows();r++){
        for(int c=0;c<grid.numCols();c++){
            cout << grid.get(r,c);
        }
        cout <<endl;
    }

}

template <typename T>
void fill_grid(Grid<T>& grid, T filler){
    for(int r=0;r<grid.numRows();r++){
        for(int c=0;c<grid.numCols();c++){
            grid.set(r,c, filler);
        }
    }
}

template <typename T>
void evolve(Grid<T>& grid){
    int numRows = grid.numRows();
    int numCols = grid.numCols();
    Grid<int> tmp_grid(numRows,numCols);
    fill_grid(tmp_grid,0);

    for(int r=0;r<numRows;r++){
        for(int c=0;c<numCols;c++){
            if(grid.get(r,c) == 'X'){
                for(int i=-1; i<=1; i++){
                    for(int j=-1;j<=1;j++){
                        if(!((i==0) && (j==0)) && tmp_grid.inBounds(r+i,c+j)){
                            tmp_grid.set(r+i,c+j,tmp_grid.get(r+i,c+j)+1);
                        }
                    }
                }
            }
        }
    }

    for(int r=0;r<numRows;r++){
        for(int c=0;c<numCols;c++){
            switch (tmp_grid.get(r,c)) {
            case 2:
                break;
            case 3:
                grid.set(r,c,'X');
                break;
            default:
                grid.set(r,c,'-');
                break;
            }
        }
    }
}
template <typename T>
void animation(Grid<T>& grid){
    while(true){
    print_grid(grid);
    pause(150);
    clearConsole();
    evolve(grid);
    }
}


int main() {
    string filename,line;
    int row,col;
    string number;
    ifstream myReader;
    // TODO: Finish the program!

    cout << "Welcome to the TDDD86 Game of Life,\n";
    cout << "a simulation of the lifecycle of a bacteria colony.\n";
    cout << "Cells (X) live and die by the following rules:\n";
    cout << " - A cell with 1 or fewer neighbours dies.\n";
    cout << " - Location with 2 neighbours remain stable.\n";
    cout << " - Location with 3 neighbours will crerat life.\n";
    cout << " - A cell with 4 or more neighbours dies.\n";
    cout << "\n";

    cout << "Grid input file name? ";
    cin >> filename;
    //filename="simple.txt";
    myReader.open(filename);

    getline(myReader,line);
    row=atoi(&line[0]);
    getline(myReader,line);
    col=atoi(&line[0]);


    Grid<char> myGrid(row,col);

    for(int r=0;r<row;r++){
        getline(myReader,line);
        for(int c=0;c<col;c++){
            myGrid.set(r,c,line[c]);
        }
    }


    print_grid(myGrid);
    char input;
    while(cout << "a)nimate, t)ick, q)uit? " &&cin >> input && input!='q'){

        switch (input) {
        case 't':
            evolve(myGrid);
            print_grid(myGrid);
            break;
        case 'q':
            return 0;
        case 'a':
            animation(myGrid);
            break;
        default:
            break;
        }

    }



    myReader.close();
    cout << "Have a nice Life!" << std::endl;
    return 0;
}





