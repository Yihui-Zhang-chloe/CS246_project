#include "edge.h"

Edge::Edge(){
    ID = 0;
    edgeColor = None;
}

Colour Edge::getColor() noexcept { return edgeColor; }

string Edge::show() noexcept {
    switch (edgeColor){
        case Blue:
            return "BR";
        case Red:
            return "RR";
        case Orange:
            return "OR";
        case Yellow:
            return "YR";
        default:{
            return (ID > 9)? to_string(ID) : " " + to_string(ID);
        }
    }
}

int Edge::getID() noexcept { return ID; }

void Edge::setValue(int id,enum Colour color) noexcept {
    ID = id;
    edgeColor = color;
}

bool Edge::BulidingRoadByColor(enum Colour Color) noexcept{
    if ((edgeColor != None) && (Color != None)){
        return false;
    }else {
        edgeColor = Color;
        return true;
    }
}

