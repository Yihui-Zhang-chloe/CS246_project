#ifndef __EDGE_H__
#define __EDGE_H__

#include <string>
#include "basic.h"
using namespace std;

class Edge{
    int ID;
    enum Colour edgeColor;
    public:
    Edge();
    Colour getColor() noexcept;
    string show() noexcept;
    int getID() noexcept;
    void setValue(int id,enum Colour color) noexcept;
    bool BulidingRoadByColor(enum Colour Color) noexcept;
};

#endif

