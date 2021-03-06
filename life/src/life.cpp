// andno037, anden259

#include  <iostream>
#include "grid.h"
#include "lifeutil.h"
#include  <fstream>
using namespace std;

// Prints the grid to cout
template  <typename T>
void print_grid(Grid <T>& grid)
{
    for(int r = 0; r < grid.numRows(); r++) {
        for(int c = 0; c < grid.numCols(); c++) {
            cout  <<  grid.get(r,c);
        }
        cout  << endl;
    }
}

// Filles the grid
template  <typename T>
void fill_grid(Grid <T>& grid, T filler)
{
    for(int r = 0; r < grid.numRows(); r++) {
        for(int c = 0; c < grid.numCols(); c++) {
            grid.set(r,c, filler);
        }
    }
}

// Add 1 to the namber count, to all the cells around the living cell and not the cell it self
void addMeAsNamber(int r,int c,Grid<int>& NumberOfNambers){
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(!((i == 0) && (j == 0)) && NumberOfNambers.inBounds(r + i,c + j)) {
                NumberOfNambers.set(r + i,c + j,NumberOfNambers.get(r + i,c + j) + 1);
            }
        }
    }
}



// Evolves the grid to the next generation.
template  <typename T>
void evolve(Grid <T>& grid)
{
    int numRows  =  grid.numRows();
    int numCols  =  grid.numCols();
    // a grid to count how many nambers that cell has.
    Grid < int> NumberOfNambers(numRows,numCols);
    fill_grid(NumberOfNambers,0);

    for(int r = 0; r < numRows; r++) {
        for(int c = 0; c < numCols; c++) {
            if(grid.get(r,c)  ==  'X') {
                addMeAsNamber(r,c,NumberOfNambers);
            }
        }
    }

    for(int r = 0; r < numRows; r++) {
        for(int c = 0; c < numCols; c++) {
            switch (NumberOfNambers.get(r,c)) {
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

// Starts an auto evalve of the grid.
template  <typename T>
void animation(Grid <T>& grid)
{
    while(true) {
        print_grid(grid);
        pause(150);
        clearConsole();
        evolve(grid);
    }
}


int main()
{
    string filename,line;
    int row,col;
    string number;
    ifstream myReader;

    cout  <<  "Welcome to the TDDD86 Game of Life,\n";
    cout  <<  "a simulation of the lifecycle of a bacteria colony.\n";
    cout  <<  "Cells (X) live and die by the following rules:\n";
    cout  <<  " - A cell with 1 or fewer neighbours dies.\n";
    cout  <<  " - Location with 2 neighbours remain stable.\n";
    cout  <<  " - Location with 3 neighbours will crerat life.\n";
    cout  <<  " - A cell with 4 or more neighbours dies.\n";
    cout  <<  "\n";

    cout  <<  "Grid input file name? ";
    cin >> filename;
    //filename = "simple.txt";
    myReader.open(filename);

    getline(myReader,line);
    row = atoi(&line[0]);
    getline(myReader,line);
    col = atoi(&line[0]);

    Grid <char> myGrid(row,col);

    for(int r = 0; r < row; r++) {
        getline(myReader,line);
        for(int c = 0; c < col; c++) {
            myGrid.set(r,c,line[c]);
        }
    }

    print_grid(myGrid);
    char input;
    while(cout  <<  "a)nimate, t)ick, q)uit? " &&cin >> input && input!= 'q') {

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
    cout  <<  "Have a nice Life!"  <<  std::endl;
    return 0;
}





