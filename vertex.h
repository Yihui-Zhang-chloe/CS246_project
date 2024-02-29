#ifndef __VERTEX_H__
#define __VERTEX_H__

using namespace std;
#include <string>
#include "basic.h"

class Vertex{
    int ID;                             //id
    enum Colour VertexColor;           //colour of owner
    enum Residence ResidenceLevel;     //level of residence
    public:
    Vertex();
    Colour getColour() noexcept;
    string show() noexcept;
    string showResidence() noexcept;
    int getID() noexcept;
    Residence getResidenceLevel() noexcept;
    void setValue(int id,Colour Color,Residence level) noexcept;
    bool bulidingByColor(enum Colour Color);
};

#endif

