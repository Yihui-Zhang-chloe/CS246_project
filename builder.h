#ifndef __BUILDER_H__
#define __BUILDER_H__

#include "basic.h"
#include <string>
using namespace std;

class Builder{
    Colour BuilderColor; // color
    RES Res;             // resources
    bool loaded;
    public:
    Builder();
    Builder(Colour color);
    bool getLoaded() noexcept;
    void setLoaded(bool bLoad) noexcept;
    string showColor() noexcept; // returns the color
    Colour getColor() noexcept;
    void setColor(Colour color) noexcept;
    void addRes(RES res) noexcept; // increase resources
    bool removeRes(RES res) noexcept; // decrease resources
    RES getRes() noexcept; // returns current state of resource
    bool haveRes() noexcept;
};

#endif

