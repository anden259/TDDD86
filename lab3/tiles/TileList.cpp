// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"

TileList::TileList()
{
    allocSize = 10;
    size = 0;
    myTile = new Tile[allocSize];
    first = nullptr;
    last = nullptr;

}

TileList::TileList(const TileList &other)
{
    *this = other;
}

TileList::~TileList()
{
    // TODO: write this member
}

void TileList::addTile(Tile tile)
{
    if(size == allocSize){
        expand();
    }

    if(last == nullptr){
        first = myTile;
        last = myTile;
    }else{
        ++last;
    }

    ++size;
    *last = tile;

    return;
}

void TileList::drawAll(QGraphicsScene* scene)
{
    for(Tile* current = first; current <= last; ++current){
        current->draw(scene);
    }
}

int TileList::indexOfTopTile(int x, int y)
{
    int index = -1;
    for(size_t i = 0; i < size; ++i){
        if(first[i].contains(x, y)){
            //++index;
            index = i;
        }
    }
    std::cout << "indexOf: " << size << std::endl;
    return index;
}

void TileList::raise(int x, int y)
{
    int i = indexOfTopTile(x, y);

    if (i == -1){
        return;
    }

    Tile tile = myTile[i];
    removeByIndex(i);
    addTile(tile);
    return;
}

void TileList::lower(int x, int y)
{
    // TODO: write this member
}

void TileList::remove(int x, int y)
{
    removeByIndex(indexOfTopTile(x, y));
    return;
}

void TileList::removeAll(int x, int y)
{
    int i = indexOfTopTile(x, y);
    while(i != -1){
        removeByIndex(i);
        i = indexOfTopTile(x, y);
    }
}

void TileList::expand(){
    size_t newAllocSize = allocSize*2;
    Tile* newTile = new Tile[newAllocSize];

    for(size_t i = 0; i < allocSize; ++i){
        newTile[i] = std::move(myTile[i]);
    }

    last = &newTile[allocSize-1];
    allocSize = newAllocSize;
    delete[] myTile;
    myTile = newTile;
    first = newTile;
    return;
}

void TileList::removeByIndex(int index){
    if(index < 0){
        return;
    }
    int prevIndex = index;
    for(size_t currentIndex = index+1; currentIndex < size; ++currentIndex){
        myTile[prevIndex] = myTile[currentIndex];
    }
    --last;
    --size;
}
