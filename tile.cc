#include "tile.h"

Tile::Tile(): ID{0}, ResourceID{PARK}, ResourceNum{0}{
    for (int i = 0; i < 6; i++) {
        ptheVertex[i] = NULL;
        ptheEdge[i] = NULL;
        }
}

string Tile::showID() noexcept {
    return (ID > 9) ? to_string(ID) : " " + to_string(ID);
}

string Tile::showResourceNum() noexcept {
    if (ResourceID == PARK) return "  ";
    return (ResourceNum < 10) ? " " + to_string(ResourceNum) : to_string(ResourceNum);
}

int Tile::getResourceNum() noexcept { return ResourceNum; }

Resource Tile::getResourceID() noexcept { return ResourceID; }

string Tile::showResource() noexcept {
    switch(ResourceID){
        case BRICK:
            return "  BRICK  ";
        case ENERGY:
            return "  ENERGY ";
        case GLASS:
            return "  GLASS  ";
        case HEAT:
            return "  HEAT   ";
        case WIFI:
            return "  WIFI   ";
        default:
            return "  PARK   ";
    }
}

void Tile::setValue(int id,Resource rID,int rNum){
    ID = id;
    ResourceID = rID;
    ResourceNum = rNum;
}

void Tile::init(int id,Vertex* v,Edge* e,Resource rID,int rNum){
    ID = id;
    ResourceID = rID;
    ResourceNum = rNum;
    for (int i = 0; i < 6; i++){
        ptheVertex[i] = v+Vertexdata[id][i];
        ptheEdge[i] = e+Edgedata[id][i];
    }
}

