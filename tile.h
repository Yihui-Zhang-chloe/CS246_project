#ifndef __TILE_H__
#define __TILE_H__

using namespace std;
#include <string>
#include "vertex.h"
#include "edge.h"

class Tile{
    int ID;
    Vertex*  ptheVertex[6];    
    Edge*  ptheEdge[6];        
    enum Resource ResourceID;   
    int ResourceNum;
    public:
    Tile();
    string showID() noexcept;
    string showResourceNum() noexcept;
    int getResourceNum() noexcept;
    Resource getResourceID() noexcept;
    string showResource() noexcept;
    void setValue(int id,Resource rID,int rNum);
    void init(int id,Vertex* v,Edge* e,Resource rID,int rNum);
};
#endif

