#include "board.h"

Board::Board(){
    for (int i = 0; i < 54; i++){
        theVertex[i].setValue(i,None,Nothing);
    }
    for (int i = 0; i < 72; i++){
        theEdge[i].setValue(i,None);
    }
    theBuilder[0].setColor(Blue);
    theBuilder[1].setColor(Red);
    theBuilder[2].setColor(Orange);
    theBuilder[3].setColor(Yellow);
    resRoad = {0,0,0,1,1}; //one HEAT and one WIFI resource  
    resBasement = {1,1,1,0,1}; //one BRICK, one ENERGY, one GLASS, and one WIFI to build.
    resHouse = {0,0,2,3,0};  //two GLASS, and three HEAT to build.
    resTower = {3,2,2,2,1}; //three BRICK, two ENERGY, two GLASS, one WIFI and two HEAT to build.
    Geese = 100;
            
    // initialize the board randomly
    Resource place[19]={PARK,WIFI,WIFI,WIFI,HEAT,HEAT,HEAT,BRICK,BRICK,BRICK,BRICK,ENERGY,ENERGY,ENERGY,ENERGY,GLASS,GLASS,GLASS,GLASS};
    int num[19]={2,3,4,5,6,7,8,9,10,11,12}; // 
    num[11] = rand()% 4 + 3; // 2 3-6
    num[12] = rand()% 4 + 8; // 2 8-11
            
    for (int i = 13; i < 19; i++){
        int n = 7;
        while (n == 7){
            n = rand() % 9 + 3; // fill 3-11 in rest 6
        }
        num[i] = n;
    }
    Resource tmpR;
    int tmp = 0;
    for (int j = 0; j < 100; j++){
        for (int i = 0; i < 19; i++){
            int n = 0;
            n = rand() % (19 - i);
            tmpR = place[18-i];
            place[18-i] = place[n];
            place[n] = tmpR;
            n = rand() % (19 - i);
            tmp = num[18-i];
            num[18-i] = num[n];
            num[n] = tmp;
        }
    } //use for loop to randomly run 100 times

    for (int i = 0; i < 19; i++){
        if (place[i] == PARK){
            for (int j = 0; j < 19; j++){
                if (num[j] == 7){
                    num[j] = num[i];
                    num[i] = 7;
                }
            }
        }
    }

    // init every tile
    for (int i = 0; i < 19; i++){
        theTile[i].init(i,theVertex,theEdge,place[i],num[i]);
    }
}

bool Board::enoughToBuildRoad(Colour color) noexcept {
    RES resTmp = {0};
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            resTmp = theBuilder[i].getRes();
            break;
        }
    }
    if (resTmp.numBrick < resRoad.numBrick) return false;
    if (resTmp.numEnergy < resRoad.numEnergy) return false;
    if (resTmp.numGlass < resRoad.numGlass) return false;
    if (resTmp.numHeat < resRoad.numHeat) return false;
    if (resTmp.numWifi < resRoad.numWifi) return false;
    return true;
}

bool Board::enoughToBuildHouse(Colour color,int vertexID) noexcept{
    RES resTmp = {0};
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            resTmp = theBuilder[i].getRes();
            break;
        }
    }
    switch (theVertex[vertexID].getResidenceLevel()){
        case Nothing:{
            if (resTmp.numBrick < resBasement.numBrick) return false;
            if (resTmp.numEnergy < resBasement.numEnergy) return false;
            if (resTmp.numGlass < resBasement.numGlass) return false;
            if (resTmp.numHeat < resBasement.numHeat) return false;
            if (resTmp.numWifi < resBasement.numWifi) return false;
            return true;
        }
        break;
        case Basement:{
            if (resTmp.numBrick < resHouse.numBrick) return false;
            if (resTmp.numEnergy < resHouse.numEnergy) return false;
            if (resTmp.numGlass < resHouse.numGlass) return false;
            if (resTmp.numHeat < resHouse.numHeat) return false;
            if (resTmp.numWifi < resHouse.numWifi) return false;
            return true;
        }
        break;
        case House:{
            if (resTmp.numBrick < resTower.numBrick) return false;
            if (resTmp.numEnergy < resTower.numEnergy) return false;
            if (resTmp.numGlass < resTower.numGlass) return false;
            if (resTmp.numHeat < resTower.numHeat) return false;
            if (resTmp.numWifi < resTower.numWifi) return false;
            return true;
        }
        default:
            break;
    }
    return false;
}

bool Board::payForBuildRoad(Colour color) noexcept{
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            return theBuilder[i].removeRes(resRoad);
        }
    }
    return false;
}

bool Board::payForBuildHouse(Colour color,int vertexID) noexcept{
    if (theVertex[vertexID].getColour() != color) return false;
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            switch (theVertex[vertexID].getResidenceLevel()){
                case Basement:
                    return theBuilder[i].removeRes(resBasement);
                case House:
                    return theBuilder[i].removeRes(resHouse);
                case Tower:
                    return theBuilder[i].removeRes(resTower);
                default:
                    break;
            }
        }
    }
    return false;
}

void Board::setLoaded(Colour color,bool bLoaded) noexcept {
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            theBuilder[i].setLoaded(bLoaded);
            return;
        }
    }
}

bool Board::getLoaded(Colour color) noexcept{
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            return theBuilder[i].getLoaded();
        }
    }
    return true;
}

int Board::getBuilderResNum(Colour color,Resource aResId) noexcept {
    RES resTmp = {0};
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            resTmp = theBuilder[i].getRes();
        }
    }
    switch(aResId){
        case BRICK:
            return resTmp.numBrick;
        case ENERGY:
            return resTmp.numEnergy;
        case GLASS:
            return resTmp.numGlass;
        case HEAT:
            return resTmp.numHeat;
        case WIFI:
            return resTmp.numWifi;
        default:
            break;
    }
    return -1;
}

void Board::setTile(int id,Resource rID,int rNum) noexcept {
    theTile[id].setValue(id,rID,rNum);
}

void Board::setHouse(int id,Colour Color,Residence level) noexcept {
    theVertex[id].setValue(id,Color,level);
}

void Board::setRoads(int id,Colour color) noexcept {
    theEdge[id].setValue(id,color);
}

Resource Board::strToResource(string str) noexcept {
    if (str == "BRICK") return BRICK;
    if (str == "ENERGY") return ENERGY;
    if (str == "GLASS") return GLASS;
    if (str == "HEAT") return HEAT;
    if (str == "WIFI") return WIFI;
    return PARK;
}

Colour Board::strToColour(string str) noexcept{
    if (str == "BLUE") return Blue;
    if (str == "RED") return Red;
    if (str == "ORANGE") return Orange;
    if (str == "YELLOW") return Yellow;
    return None;
}

int Board::getGeese() noexcept { return Geese; }

string Board::showGeese(int g) noexcept{
    return (Geese == g) ? "  GEESE  " : "         ";
}

bool Board::stolen(Colour inColor,Colour outColor) noexcept{
    RES tmpRES = {0};
    RES outRES = {0};
    if (inColor == outColor) return false;
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == outColor){
            outRES = theBuilder[i].getRes();
        }
    }
    int r1,r2,r3,r4,r5 = 0;
    r1 = outRES.numBrick;
    r2 = outRES.numEnergy;
    r3 = outRES.numGlass;
    r4 = outRES.numHeat;
    r5 = outRES.numWifi;

    int n = rand()% (r1 + r2 + r3 + r4 + r5);
    if (n < r1){
        tmpRES.numBrick = 1;
        cout << "Builder "<<showColor(inColor)<<" steals BRICK from builder "<<showColor(outColor)<<"."<<endl;
    }else if (n < (r1 + r2)){
        tmpRES.numEnergy = 1;
        cout << "Builder "<<showColor(inColor)<<" steals ENERGY from builder "<<showColor(outColor)<<"."<<endl;
    }else if (n < (r1 + r2 + r3)){
        tmpRES.numGlass = 1;
        cout << "Builder "<<showColor(inColor)<<" steals GLASS from builder "<<showColor(outColor)<<"."<<endl;
    }else if (n < (r1 + r2 + r3 + r4)){
        tmpRES.numHeat = 1;
        cout << "Builder "<<showColor(inColor)<<" steals HEAT from builder "<<showColor(outColor)<<"."<<endl;
    }else {
        tmpRES.numWifi = 1;
        cout << "Builder "<<showColor(inColor)<<" steals WIFI from builder "<<showColor(outColor)<<"."<<endl;
    }

    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == outColor){
            theBuilder[i].removeRes(tmpRES);
        }
        if (theBuilder[i].getColor() == inColor){
            theBuilder[i].addRes(tmpRES);
        }
    }
    return true;
}

void Board::addRes(Colour color,RES res) noexcept{
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            theBuilder[i].addRes(res);
        }
    }
}

string Board::showColor(Colour color) noexcept{
    switch (color){
        case Blue:
            return "Blue";
        case Red:
            return "Red";
        case Orange:
            return "Orange";
        case Yellow:
            return "Yellow";
        default:
            return "None";
    }
}

void Board::initGeese(int geese) noexcept { Geese = geese; }

string Board::setGeese(Colour curColor,int geese) noexcept{
    string str = "";
    if (geese < 0) return str;
    if (geese >= TILENUM) return str;
    if (Geese == geese) return str;

    Geese = geese;
    Colour tmpColor = None;
    bool bBlue =false;
    bool bRed =false;
    bool bOrange =false;
    bool bYellow =false;

    for (int i = 0; i < 6; i++){
        tmpColor = theVertex[Vertexdata[geese][i]].getColour();
        switch(tmpColor){
            case Blue:
                bBlue = true;
                break;
            case Red:
                bRed = true;
                break;
            case Orange:
                bOrange = true;
                break;
            case Yellow:
                bYellow = true;
                break;
            default:
                break;
        }
    }
    switch(curColor){
        case Blue:
            bBlue = false;
            break;
        case Red:
            bRed = false;
            break;
        case Orange:
            bOrange = false;
            break;
        case Yellow:
            bYellow = false;
            break;
        default:
            break;
    }

    for (int i = 0; i < 4; i++){
        if (bBlue){
            if ((theBuilder[i].getColor() == Blue) && ( theBuilder[i].haveRes())){
                str += "Blue,";
            }
        }
        if (bRed){
            if ((theBuilder[i].getColor() == Red) && ( theBuilder[i].haveRes())){
                str += "Red,";
            }
        }
        if (bOrange){
            if ((theBuilder[i].getColor() == Orange) && ( theBuilder[i].haveRes())){
                str += "Orange,";
            }
        }
        if (bYellow){
            if ((theBuilder[i].getColor() == Yellow) && ( theBuilder[i].haveRes())){
                str += "Yellow,";
            }
        }
    }

    if (str.length() > 0){
        str = str.substr(0,str.length()-1);
        str += ".";
    }
    return str;
}

void Board::dropResToGeese(){
    for (int i = 0; i < 4; i++){
        int n = 0;
        RES bRes={0};
        bRes = theBuilder[i].getRes();
        n += bRes.numBrick;
        n += bRes.numEnergy;
        n += bRes.numGlass;
        n += bRes.numHeat;
        n += bRes.numWifi;

        if (n >= 10){
            int m = n / 2;
            RES tRes = {0};

            for (int a = 0; a < m; a++){
                int r1 = bRes.numBrick - tRes.numBrick;
                int r2 = bRes.numEnergy - tRes.numEnergy;
                int r3 = bRes.numGlass - tRes.numGlass;
                int r4 = bRes.numHeat - tRes.numHeat;
                int r5 = bRes.numWifi - tRes.numWifi;

                int p = rand()% (r1 + r2 + r3 + r4 + r5);
                if (p < r1){
                    tRes.numBrick +=1;
                }else if (p < (r1 + r2)){
                    tRes.numEnergy += 1;
                }else if (p < (r1 + r2 + r3)){
                    tRes.numGlass += 1;
                }else if (p < (r1 + r2 + r3 + r4)){
                    tRes.numHeat += 1;
                }else{
                    tRes.numWifi += 1;
                }
            }
            theBuilder[i].removeRes(tRes);
            cout << "Builder " << theBuilder[i].showColor() << " Loses " << to_string(m); 
            cout << " Resources to the geese. They loss:" << endl;
            if (tRes.numBrick > 0) cout << to_string(tRes.numBrick) << " BRICK" << endl;
            if (tRes.numEnergy > 0) cout << to_string(tRes.numEnergy) << " Energy" << endl;
            if (tRes.numGlass > 0) cout << to_string(tRes.numGlass) << " GLASS" << endl;
            if (tRes.numHeat > 0) cout << to_string(tRes.numHeat) << " Heat" << endl;
            if (tRes.numWifi > 0) cout << to_string(tRes.numWifi) << " WIFI" << endl;
        }
    }
}

void Board::addResByDice(int dice){
    // when rolling a seven, deal it with other methods
    if (dice == 7){
        return;
    }
    Builder tmpBuilder[4];
    tmpBuilder[0].setColor(Blue);
    tmpBuilder[1].setColor(Red);
    tmpBuilder[2].setColor(Orange);
    tmpBuilder[3].setColor(Yellow);

    for (int i = 0; i < TILENUM; i++){
        if (theTile[i].getResourceNum() == dice){
            if (Geese != i){ // geese is not at i
            RES res = {0};
            switch (theTile[i].getResourceID()){
                case BRICK:
                    res.numBrick = 1;
                    break;
                case ENERGY:
                    res.numEnergy = 1;
                    break;
                case GLASS:
                    res.numGlass = 1;
                    break;
                case HEAT:
                    res.numHeat = 1;
                    break;
                case WIFI:
                    res.numWifi = 1;
                    break;
                default:
                    break;
            }

            for (int j = 0; j < 6; j++){
                for (int k = 0; k < theVertex[Vertexdata[i][j]].getResidenceLevel(); k++){
                    switch (theVertex[Vertexdata[i][j]].getColour()){
                        case Blue:
                            tmpBuilder[0].addRes(res);
                            break;
                        case Red:
                            tmpBuilder[1].addRes(res);
                            break;
                        case Orange:
                            tmpBuilder[2].addRes(res);
                            break;
                        case Yellow:
                            tmpBuilder[3].addRes(res);
                            break;
                        default:
                            break;
                    }
                }
            }
            }
        }
    }
    bool bNoBuilder = true;
    for (int i = 0; i < 4; i++){
        if (tmpBuilder[i].haveRes()){
            bNoBuilder = false;
            theBuilder[i].addRes(tmpBuilder[i].getRes());
            cout << "Builder "<<theBuilder[i].showColor()<<" gained:"<< endl;
            if (tmpBuilder[i].getRes().numBrick > 0 ) cout << to_string(tmpBuilder[i].getRes().numBrick) << " BRICK" <<endl;
            if (tmpBuilder[i].getRes().numEnergy > 0 ) cout << to_string(tmpBuilder[i].getRes().numEnergy) << " ENERGY" <<endl;                if (tmpBuilder[i].getRes().numGlass > 0 ) cout << to_string(tmpBuilder[i].getRes().numGlass) << " GLASS" <<endl;
            if (tmpBuilder[i].getRes().numHeat> 0 ) cout << to_string(tmpBuilder[i].getRes().numHeat) << " HEAT" <<endl;
            if (tmpBuilder[i].getRes().numWifi > 0 ) cout << to_string(tmpBuilder[i].getRes().numWifi) << " WIFI" <<endl;
        }
    }
    if (bNoBuilder){
        cout << "No builders gained resources." << endl;
    }
    return;
}

bool Board::BulidingRoad(Colour color,int id) noexcept{
    if (canBuildroad(color,id)){
        return theEdge[id].BulidingRoadByColor(color);
    }
    return false;
}

bool Board::canBuildroad(Colour color,int id) noexcept{
    if (id >= EDGENUM) return false;
    if (id < 0) return false;
    if (int(color) >= 5) return false;
    if (int(color) <= 0) return false;
    if (theEdge[id].getColor() != None) return false;

    for (int i = 0; i < TILENUM; i++){
        for (int j = 0; j < 6; j++){
            if (id == Edgedata[i][j]){
                int m = 0;
                int n = 5;
                if (5 > j){
                    m = j + 1;
                }
                if (0 < j){
                    n = j - 1;
                }
                if (theVertex[Vertexdata[i][j]].getColour() == color){
                    return true;
                }
                if (theVertex[Vertexdata[i][m]].getColour() == color){
                    return true;
                }
                if (theEdge[Edgedata[i][n]].getColor() == color){
                    if (theVertex[Vertexdata[i][j]].getColour() == color){
                        return true;
                    }
                    if (theVertex[Vertexdata[i][j]].getColour() == None){
                        return true;
                    }
                }
                if (theEdge[Edgedata[i][m]].getColor() == color){
                    if (theVertex[Vertexdata[i][m]].getColour() == color){
                        return true;
                    }
                    if (theVertex[Vertexdata[i][m]].getColour() == None){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::trade(Colour giveColor,Colour takeColor,Resource give,Resource take){
    RES resGive = {0};
    RES resTake = {0};
    switch(give){
        case BRICK:
            resGive.numBrick = 1;
            break;
        case ENERGY:
            resGive.numEnergy = 1;
            break;
        case GLASS:
            resGive.numGlass = 1;
            break;
        case HEAT:
            resGive.numHeat = 1;
            break;
        case WIFI:
            resGive.numWifi = 1;
            break;
        default:
            break;
    }

    switch(take){
        case BRICK:
            resTake.numBrick = 1;
            break;
        case ENERGY:
            resTake.numEnergy = 1;
            break;
        case GLASS:
            resTake.numGlass = 1;
            break;
        case HEAT:
            resTake.numHeat = 1;
            break;
        case WIFI:
            resTake.numWifi = 1;
            break;
        default:
            break;
    }

    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == giveColor){
            theBuilder[i].removeRes(resGive);
            theBuilder[i].addRes(resTake);
        }
        if (theBuilder[i].getColor() == takeColor){
            theBuilder[i].removeRes(resTake);
            theBuilder[i].addRes(resGive); 
        }
    }
    return true;
}

bool Board::canBuildHouse(Colour color,int id,bool bFirst){
    if (id >= VERTEXNUM) return false;
    if (id < 0) return false;
    if (int(color) >= 5) return false;
    if (int(color) <= 0) return false;
    if (theVertex[id].getResidenceLevel() != Nothing){ // has residences
        if (theVertex[id].getResidenceLevel() == Tower){
            return false;
        }
        if ((theVertex[id].getColour() != color)){
            return false;
        }
        if (bFirst){
            return false;
        }
        return true;
    }else { // no residences
        bool bRoad = false;
        for (int i = 0; i < TILENUM; i++){
            for (int j = 0; j < 6; j++){
                if (id == Vertexdata[i][j]){
                    int m = 0;
                    int n = 5;
                    if (5 > j){
                        m = j + 1;
                    }
                    if (0 < j){
                        n = j - 1;
                    }
                    if (theEdge[Edgedata[i][n]].getColor() == color){
                        bRoad = true;
                    }
                    if (theEdge[Edgedata[i][j]].getColor() == color){
                        bRoad = true;
                    }
                    if (theVertex[Vertexdata[i][m]].getColour() != None){
                        return false;
                    }
                    if (theVertex[Vertexdata[i][n]].getColour() != None){
                        return false;
                    }
                }
            }
        }
        if (bRoad){
            return true;
        }else if (bFirst){
            return true;
        }
        return false;
    }  
}

bool Board::BuildingHouse(Colour color,int id,bool bFirst){
    if (canBuildHouse(color,id,bFirst)){
        return theVertex[id].bulidingByColor(color);
    }
    return false;
}

void Board::show() noexcept{
    int i=0; 
    int j=0;
    int k=0;
    int l=0;
    int m=0;
    int g=0;
    cout<<"                          |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;
    cout<<"                            |         |"<<endl;
    cout<<"                           "<<theEdge[j++].show()<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show()<<endl;
    cout<<"                            |"<<theTile[m++].showResource()<<"|"<<endl;
    cout<<"                |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<"  "<<theTile[k++].showResourceNum()<<"  |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;
    cout<<"                  |         |"<<showGeese(g++)<<"|         |"<<endl;//鹅
    cout<<"                 "<<theEdge[j++].show()<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show()<<"        "<<theEdge[j++].show()<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show()<<endl;
    cout<<"                  |"<<theTile[m++].showResource()<<"|         |"<<theTile[m++].showResource()<<"|"<<endl;

    for (int n = 0; n < 3; n++){
        cout<<"      |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
        cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
        cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
        cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
        cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;

        for (int a = 0;; a++){
            if (a == 0){
                cout<<"        ";
            }
            cout<<"|";
            if (a >= 5){
                cout<<endl;
                break;
            }
            if (a == 1 || a == 3){
                cout<<showGeese(g++);
                continue;
            }
            cout<<"         "; // mitght have a geese
        }
        for (int a = 0;; a++){
            if (a == 0){
                cout<<"       ";
            }
            cout<<theEdge[j++].show()<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show();
            if (a >= 2){
                cout<<endl;
                break;
            }
            cout<<"        ";
        }
        for (int a = 0;; a++){
            if(a == 0){
                cout<<"        ";
            }
            cout<<"|"<<theTile[m++].showResource()<<"|";
            if (a >= 2){
                cout<<endl;
                break;
            }
            cout<<"         ";
        }
    {
    cout<<"      |"<<theVertex[i++].show()<<"|  "<<theTile[k++].showResourceNum()<<"  |";
    cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|";
    cout<<theVertex[i++].show()<<"|  "<<theTile[k++].showResourceNum()<<"  |";
    cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|";
    cout<<theVertex[i++].show()<<"|  "<<theTile[k++].showResourceNum()<<"  |"<<theVertex[i++].show()<<"|"<<endl;
    }

    for (int a = 0;; a++){
        if (a > 5){
            cout<<endl;
            break;
        }
        if (a == 0){
            cout<<"        |";
            continue;
        }
        if (a % 2 == 1){
            cout<<showGeese(g++)<<"|"; // geese
            continue;
        }
        cout<<"         |";
    }
    for (int a = 0;; a++){
        if (a == 0){
            cout<<"       ";
        }
        cout<<theEdge[j++].show()<<"        "<<theEdge[j++].show();
        if (a >= 2){
            cout << endl;
            break;
        }
        cout<<"   "<<theTile[l++].showID()<<"   ";;
    }
    for (int a = 0;; a++){
        if (a == 0){
            cout<<"        ";
        }
        cout<<"|         |";
        if (a >= 2){
            cout<<endl;
            break;
        }
        cout<<theTile[m++].showResource();
    }
    }

    // last 
    cout<<"      |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
    cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
    cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
    cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
    cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;
    cout<<"         ";
    for (int a = 0; a < 4; a++){
        if ( a % 2 == 1){
            cout<<showGeese(g++)<<"|";
            continue;
        }
        cout<<"         |"; // might have a geese
    }
    cout<<endl;   
    cout<<"                 "<<theEdge[j++].show();
    cout<<"        "<<theEdge[j++].show();
    cout<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show();
    cout<<"        "<<theEdge[j++].show();
    cout<<endl;
    cout<<"                  |         |";
    cout<<theTile[m++].showResource()<<"|         |"<<endl;
    cout<<"                |";
    cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
    cout<<"  "<<theTile[k++].showResourceNum()<<"  |";
    cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
    cout<<endl;
    cout<<"                            |"<<showGeese(g++)<<"|"<<endl; // might have a geese
    cout<<"                           ";
    cout<<theEdge[j++].show()<<"        "<<theEdge[j++].show()<<endl;
    cout<<"                            |         |"<<endl;
    cout<<"                          |";
    cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;
}

string Board::showExistHouse() noexcept{
    string str="";
    for (int i = 0; i < VERTEXNUM; i++){
        if (theVertex[i].getColour() != None){
            str += " ";
            str += to_string(i);
        }
    }
    return str;
}

void Board::showBuilderResidnce(Colour color) noexcept{
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            cout << theBuilder[i].showColor() << " has built:" << endl; 
        }
    }
    for (int i = 0; i < VERTEXNUM; i++){
        if (theVertex[i].getColour() == color){
            cout << to_string(i) << " " << theVertex[i].showResidence() <<endl;
        }
    }
}

string Board::showVertexLevel(int id) noexcept{
    switch (theVertex[id].getResidenceLevel()){
        case Basement:
            return "Basement";
        case House:
            return "House";
        case Tower:
            return "Tower";
        default:
            break;
    }
    return "Nothing";
}

Residence Board::getVertexLevel(int vID) noexcept { return theVertex[vID].getResidenceLevel(); }

int Board::getBuilderResPoint(Colour color) noexcept{
    int p = 0;
    for (int i = 0; i < VERTEXNUM; i++){
        if (theVertex[i].getColour() == color){
            switch (theVertex[i].getResidenceLevel()){
                case Basement:
                    p += 1;
                    break;
                case House:
                    p += 2;
                    break;
                case Tower:
                    p += 3;
                    break;
                default:
                    break;
            }
        }
    }
    return p;
}

string Board::showBuilderStatus(Colour color) noexcept{
    string str = "";
    int p = 0; //point
    RES res = {0};
    p = getBuilderResPoint(color);
    int i = 0;
    for (i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            res = theBuilder[i].getRes();
            break;
        }
    }
    str += "Builder ";
    str += theBuilder[i].showColor();
    if (str.substr(8) == "Blue") str += "	";
    if (str.substr(8) == "Red") str += "	";
    if (str.substr(8) == "Orange" || str.substr(8) == "Yellow") str += "  ";
    str += "has ";
    str += to_string(p);
    str += " building points, ";
    str += to_string(res.numBrick);
    str += " BRICK, ";
    str += to_string(res.numEnergy);
    str += " ENERGY, ";
    str += to_string(res.numGlass);
    str += " GLASS, ";
    str += to_string(res.numHeat);
    str += " HEAT, ";
    str += to_string(res.numWifi);
    str += " WIFI.";
    return str;
}

string Board::getTileSaveData() noexcept {
    string str = "";
    int n;
    for (int i = 0; i < TILENUM; i++){
        n = theTile[i].getResourceNum();
        switch (theTile[i].getResourceID()){
            case BRICK:{
                str += "0 ";
                str += to_string(n);
                str += " ";
            }
            break;
            case ENERGY:{
                str += "1 ";
                str += to_string(n);
                str += " ";
            }
            break;
            case GLASS:{
                str += "2 ";
                str += to_string(n);
                str += " ";
            }
            break;
            case HEAT:{
                str += "3 ";
                str += to_string(n);
                str += " ";
            }
            break;
            case WIFI:{
                str += "4 ";
                str += to_string(n);
                str += " ";
            }
            break;
            case PARK:{
                str += "5 ";
                str += "7 ";
            }
            break;
            default:
                break;
        }
    }
    if (str.length() > 0){
        str = str.substr(0,str.length() - 1);
    }
    return str;
}

string Board::getBuilderSaveDate(Colour color){
    string str = "";
    RES res = {0};
    for (int i = 0; i < 4; i++){
        if (theBuilder[i].getColor() == color){
            res = theBuilder[i].getRes();
            break;
        }
    }
    str += to_string(res.numBrick);
    str += " ";
    str += to_string(res.numEnergy);
    str += " ";
    str += to_string(res.numGlass);
    str += " ";
    str += to_string(res.numHeat);
    str += " ";
    str += to_string(res.numWifi);
    str += " ";
    str += "r ";
    for (int i = 0; i < EDGENUM; i++){
        if (theEdge[i].getColor() == color){
            str += to_string(i);
            str += " ";
        }
    }
    str += "h";
    for (int i = 0; i < VERTEXNUM; i++){
        if (theVertex[i].getColour() == color){
            str += " ";
            str += to_string(i);
            str += " ";
            str += theVertex[i].showResidence();
        }
    }
    return str;
}


