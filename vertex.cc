#include "vertex.h"

Vertex::Vertex(){
    ID = 0;
    VertexColor = None;
    ResidenceLevel = Nothing;
}

Colour Vertex::getColour() noexcept { return VertexColor; }

string Vertex::showResidence() noexcept {
    switch(ResidenceLevel){
        case Basement:
            return "B";
            break;
        case House:
            return "H";
            break;
        case Tower:
            return "T";
            break;
        default:
            break;
    }
    return "";
}

string Vertex::show() noexcept {
    string str1 = "";
    switch (VertexColor){
        case Blue:
            str1 += "B";
            break;
        case Red:
            str1 += "R";
            break;
        case Orange:
            str1 += "O";
            break;
        case Yellow:
            str1 += "Y";
            break;
        default:
        if (ID > 9){
            str1 = to_string(ID);
        }else {
            str1 = " " + to_string(ID);
        }
        break;
    }
    switch(ResidenceLevel){
        case Basement:
            str1 += "B";
            break;
        case House:
            str1 += "H";
            break;
        case Tower:
            str1 += "T";
            break;
        default:
            break;
    }
    return str1;
}

int Vertex::getID() noexcept { return ID; }

Residence Vertex::getResidenceLevel() noexcept { return ResidenceLevel; }

void Vertex::setValue(int id,Colour Color,Residence level) noexcept {
    ID = id;
    VertexColor = Color;
    ResidenceLevel = level;
}

bool Vertex::bulidingByColor(enum Colour Color){
    switch(ResidenceLevel){
        case Nothing:{
            VertexColor = Color;
            ResidenceLevel = Basement;
            return true;
        }
        case Basement:{
            if (Color == VertexColor){
                ResidenceLevel = House;
                return true;
            }
            break;
        }
        case House:{
            if (Color == VertexColor){
                ResidenceLevel = Tower;
                return true;
            }
            break;
        }
        default:
            return false;
    }
    return false;
}

