#include "builder.h"

Builder::Builder(){
    BuilderColor=None;
    Res = {0};
    loaded = true;
}

Builder::Builder(Colour color){
    BuilderColor=color;
    Res = {0};
}

bool Builder::getLoaded() noexcept { return loaded; }

void Builder::setLoaded(bool bLoad) noexcept { loaded = bLoad; }

string Builder::showColor() noexcept{
    switch (BuilderColor){
        case Blue:
            return "Blue";
        case Red:
            return "Red";
        case Orange:
            return "Orange";
        case Yellow:
            return "Yellow";
            break;
        default:
            break;
    }
    return "";
}

Colour Builder::getColor() noexcept { return BuilderColor; }

void Builder::setColor(Colour color) noexcept { BuilderColor = color; }

void Builder::addRes(RES res) noexcept {
    Res.numBrick += res.numBrick;
    Res.numEnergy += res.numEnergy;
    Res.numGlass += res.numGlass;
    Res.numHeat += res.numHeat;
    Res.numWifi += res.numWifi;
}

bool Builder::removeRes(RES res) noexcept {
    if (Res.numBrick < res.numBrick) return false;
    if (Res.numEnergy < res.numEnergy) return false;
    if(Res.numGlass < res.numGlass) return false;
    if(Res.numHeat < res.numHeat) return false;
    if(Res.numWifi < res.numWifi) return false;
    Res.numBrick -= res.numBrick;
    Res.numEnergy -= res.numEnergy;
    Res.numGlass -= res.numGlass;
    Res.numHeat -= res.numHeat;
    Res.numWifi -= res.numWifi;
    return true;
}

RES Builder::getRes() noexcept { return Res; }

bool Builder::haveRes() noexcept {
    int n = 0;
    n += Res.numBrick ;
    n += Res.numEnergy;
    n += Res.numGlass ;
    n += Res.numHeat;
    n += Res.numWifi;
    return (n > 0);
}

