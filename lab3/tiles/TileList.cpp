// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"

TileList::TileList()
{
    allocSize=10;
    size=0;
    myTile = new Tile[allocSize];
    first=nullptr;
    end=nullptr;

}

TileList::TileList(const TileList &other)
{
    // TODO: write this member
}

TileList::~TileList()
{
    // TODO: write this member
}

void TileList::addTile(Tile tile)
{
    if(size==allocSize){
        expand();
    }

    if(end == nullptr){
        first = myTile;
        end = myTile;
    }else{
        ++end;
    }

    *end = tile;
    ++size;

    return;
}

void TileList::drawAll(QGraphicsScene* scene)
{
    for(Tile* current = first; current <= end; ++current){
        current->draw(scene);
    }
}

int TileList::indexOfTopTile(int x, int y)
{
    // TODO: write this member
}

void TileList::raise(int x, int y)
{
    // TODO: write this member
}

void TileList::lower(int x, int y)
{
    // TODO: write this member
}

void TileList::remove(int x, int y)
{
    // TODO: write this member
}

void TileList::removeAll(int x, int y)
{
    // TODO: write this member
}

void TileList::expand(){
    int newAllocSize = allocSize*2;
    Tile* newTile = new Tile[newAllocSize];
    Tile* newCurrent = newTile;
    Tile* current = first;
    for(; current <= end; ++current){
        newCurrent=std::move(current);
    }

    delete[] myTile;
    myTile = newTile;
    first = myTile;
    end = current;
    return;
}
