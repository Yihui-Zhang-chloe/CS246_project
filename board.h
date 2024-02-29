#ifndef __BOARD_H__
#define __BOARD_H__

#include "tile.h"
#include "builder.h"
#include <iostream>

class Board{
    Vertex  theVertex[VERTEXNUM];    //全部54个顶点数组, 每个Tile对象里面的指针就指向这里面的其中6个
    Edge    theEdge[EDGENUM];
    Tile theTile[TILENUM];             //19个Tile的数组
    int Geese;
    Builder theBuilder[4];
    RES resRoad;
    RES resBasement;
    RES resHouse;
    RES resTower;
    public:
    Board();
    bool enoughToBuildRoad(Colour color) noexcept;
    bool enoughToBuildHouse(Colour color,int vertexID) noexcept;
    bool payForBuildRoad(Colour color) noexcept;
    bool payForBuildHouse(Colour color,int vertexID) noexcept;
    void setLoaded(Colour color,bool bLoaded) noexcept;
    bool getLoaded(Colour color) noexcept;
    int getBuilderResNum(Colour color,Resource aResId) noexcept;
    void setTile(int id,Resource rID,int rNum) noexcept;
    void setHouse(int id,Colour Color,Residence level) noexcept;
    void setRoads(int id,Colour color) noexcept;
    Resource strToResource(string str) noexcept;
    Colour strToColour(string str) noexcept;
    int getGeese() noexcept;
    string showGeese(int g) noexcept;
    bool stolen(Colour inColor,Colour outColor) noexcept;
    void addRes(Colour color,RES res) noexcept;
    string showColor(Colour color) noexcept;
    void initGeese(int geese) noexcept;
    string setGeese(Colour curColor,int geese) noexcept;
    void dropResToGeese();
    void addResByDice(int dice);
    bool BulidingRoad(Colour color,int id) noexcept;
    bool canBuildroad(Colour color,int id) noexcept;
    bool trade(Colour giveColor,Colour takeColor,Resource give,Resource take);
    bool canBuildHouse(Colour color,int id,bool bFirst = false);
    bool BuildingHouse(Colour color,int id,bool bFirst = false);
    void show() noexcept;
    string showExistHouse() noexcept;
    void showBuilderResidnce(Colour color) noexcept;
    string showVertexLevel(int id) noexcept;
    Residence getVertexLevel(int vID) noexcept;
    int getBuilderResPoint(Colour color) noexcept;
    string showBuilderStatus(Colour color) noexcept;
    string getTileSaveData() noexcept;
    string getBuilderSaveDate(Colour color);
};

#endif

