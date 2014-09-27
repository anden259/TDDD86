#include "TileList.h"

TileList::TileList()
{
    allocSize = 10;
    size = 0;
    myTile = new Tile[allocSize];
    last = nullptr;

}

TileList::~TileList()
{
    delete[] myTile;
}

// add a tile to the end of the list.
void TileList::addTile(Tile tile)
{
    // if full, expand.
    if(size == allocSize){
        expand();
    }

    // if empty, special case.
    if(last == nullptr){
        last = myTile;
    }else{
        ++last;
    }

    ++size;
    *last = tile;

    return;
}

// draw all tiles in the list to the scene
void TileList::drawAll(QGraphicsScene* scene) const
{
    for(Tile* current = myTile; current <= last; ++current){
        current->draw(scene);
    }
}

// get the index of the top most tile on coordinates x,y
int TileList::indexOfTopTile(int x, int y) const
{
    int index = -1;
    for(size_t i = 0; i < size; ++i){
        if(myTile[i].contains(x, y)){
            index = i;
        }
    }
    return index;
}

// raise the toptile on x,y to the top of the list (the end of the list)
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

// lower the top tile on x,y to the bottom (the start of the list)
void TileList::lower(int x, int y)
{
    int i = indexOfTopTile(x, y);

    if (i == -1){
        return;
    }

    Tile tile = myTile[i];
    removeByIndex(i);
    addTileFirst(tile);
    return;
}

// remove the toptile on x,y
void TileList::remove(int x, int y)
{
    removeByIndex(indexOfTopTile(x, y));
    return;
}

// remove all tiles on x,y
void TileList::removeAll(int x, int y)
{
    int i = indexOfTopTile(x, y);
    while(i != -1){
        removeByIndex(i);
        i = indexOfTopTile(x, y);
    }
}

// expand the array to make room for more tiles.
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
    return;
}

// remove specific tile by index.
void TileList::removeByIndex(int index){
    if((index < 0) || ((size_t) index >= size)){
        return;
    }
    if(size == 1){
        size = 0;
        last = nullptr;
        return;
    }

    for(size_t currentIndex = index+1; currentIndex < size; ++currentIndex){
        myTile[currentIndex-1] = myTile[currentIndex];
    }
    --size;
    --last;
}

// add a tile to the begining of the list.
void TileList::addTileFirst(Tile tile){
    if(size == allocSize){
        expand();
    }

    size_t currentIndex = size - 1;
    for(size_t next = size; next > 0; --next){
        myTile[next] = myTile[currentIndex];
        --currentIndex;
    }

    myTile[0] = tile;
    ++last;
    ++size;
}
