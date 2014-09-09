// This is the .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header

#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

class TileList {
public:
    TileList();
    TileList(const TileList & other);
    ~TileList();
    void addTile(Tile tile);
    void drawAll(QGraphicsScene* scene);
    int indexOfTopTile(int x, int y);
    void lower(int x, int y);
    void raise(int x, int y);
    void remove(int x, int y);
    void removeAll(int x, int y);


private:
    Tile* myTile;
    Tile* last;
    size_t allocSize;
    size_t size;

    void expand();
    void removeByIndex(int index);
    void addTileFirst(Tile tile);
};

#endif // TILELIST_H
