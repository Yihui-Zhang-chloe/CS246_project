//
//  main.cpp
//  catan
//
//  Created by Chaos on 2020/12/9.
//


#include <cstddef>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

using namespace std;


#define MYERROR "输入错误，请重新输入"

#define VERTEXNUM  54
#define EDGENUM  72
#define TILENUM  19


enum Colour{None=0,Blue, Red, Orange, Yellow };//四个玩家
enum Residence{Nothing=0,Basement,House,Tower};//三个等级
enum Resource{PARK=0,BRICK,ENERGY,GLASS,HEAT,WIFI};//五种资源
enum Commands{ERR=0,HELP,BOARD,STATUS,RESIDENCES,BUILDROAD,BUILDRES,IMPROVE,TRADE,NEXT,SAVE};//各种命令


//每个Tile的6个顶点ID数据,从左上角顺时针保存
const int Vertexdata[19][6]=
{
    {0,1,4,9,8,3},
    {2,3,8,14,13,7},
    {4,5,10,16,15,9},
    {6,7,13,19,18,12},
    {8,9,15,21,20,14},
    {10,11,17,23,22,16},
    {13,14,20,26,25,19},
    {15,16,22,28,27,21},
    {18,19,25,31,30,24},
    {20,21,27,33,32,26},
    {22,23,29,35,34,28},
    {25,26,32,38,37,31},
    {27,28,34,40,39,33},
    {30,31,37,43,42,36},
    {32,33,39,45,44,38},
    {34,35,41,47,46,40},
    {37,38,44,49,48,43},
    {39,40,46,51,50,45},
    {44,45,50,53,52,49}
};
//每个Tile的6个边ID数据,从左上角顺时针保存
const int Edgedata[19][6]=
{
    {0,2,7,10,6,1},
    {3,6,14,18,13,5},
    {4,8,16,19,15,7},
    {9,13,21,26,20,12},
    {10,15,23,27,22,14},
    {11,17,25,28,24,16},
    {18,22,31,35,30,21},
    {19,24,33,36,32,23},
    {26,30,38,43,37,29},
    {27,32,40,44,39,31},
    {28,34,42,45,41,33},
    {35,39,48,52,47,38},
    {36,41,50,53,49,40},
    {43,47,55,60,54,46},
    {44,49,57,61,56,48},
    {45,51,59,62,58,50},
    {52,56,64,67,63,55},
    {53,58,66,68,65,57},
    {61,65,70,71,69,64}
};

bool strToInt(int *out,string input)
{
    for (int i=0;i<input.length();i++)
    {
        if ( input[i] < 48 || input[i] >57 ) return false;
    }
    *out = stoi(input);
    return true;
};






//各种资源结构体
struct RES
{
    int numBrick;
    int numEnergy;
    int numGlass;
    int numHeat;
    int numWifi;
};

class Builder
{
    private:
        Colour BuilderColor; //颜色
        RES Res;             //拥有的资源
        bool loaded;
        
    public:
        Builder()
        {
            BuilderColor=None;
            Res = {0};
            loaded = true;

        };
        Builder(Colour color)
        {
            BuilderColor=color;
            Res = {0};
        };
        bool getLoaded()
        {
            return loaded;
        }
        void setLoaded(bool bLoad)
        {
            loaded = bLoad;
        }
        string showColor() //输出自身颜色字符串
        {
            switch (BuilderColor)
            {
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
        };
        Colour getColor()
        {
            return BuilderColor;
        }
        void setColor(Colour color)
        {
                BuilderColor = color; 
        };
        void addRes(RES res)              //获得资源
        {
            Res.numBrick += res.numBrick;
            Res.numEnergy += res.numEnergy;
            Res.numGlass += res.numGlass;
            Res.numHeat += res.numHeat;
            Res.numWifi += res.numWifi;
        };
        bool removeRes(RES res)           //减少资源
        {
    

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
        };

        RES getRes()             //返回资源情况
        {
            return Res;
        };

        bool haveRes()
        {
            int n=0;
            n += Res.numBrick ;
            n += Res.numEnergy;
            n += Res.numGlass ;
            n += Res.numHeat;
            n += Res.numWifi;
            return (n > 0);

        };
};

//顶点
class Vertex
{
    private:
        int ID;                             //自身ID
        enum Colour VertexColor;           //拥有者颜色
        enum Residence ResidenceLevel;     //建筑等级

    public:
        Vertex()
        {
            ID=0;
            VertexColor = None;
            ResidenceLevel = Nothing;
        };
        Colour getColour()
        {
            return VertexColor;
        };
        string show()                           //画图用,输出拥有者颜色和建筑等级
        {
            string str1 = "";
            switch (VertexColor)
            {
                case Blue:
                    str1+="B";
                    break;
                case Red:
                    str1+="R";
                    break;
                case Orange:
                    str1+="O";
                    break;
                case Yellow:
                    str1+="Y";
                    break;
                default:
                    if (ID>9)
                    {
                        str1=to_string(ID);
                    }
                    else
                    {
                        str1=" "+to_string(ID);
                    }
                    break;
            }
            switch (ResidenceLevel)
            {
            case Basement:
                str1+="B";
                break;
            case House:
                str1+="H";
                break;
            case Tower:
                str1+="T";
                break;
            default:
                break;
            }
            return  str1;
        };
        string showResidence()
        {
            switch (ResidenceLevel)
            {
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
            return  "";
        }
    
        int getID()
        {
            return ID;
        };
    
        Residence getResidenceLevel()
        {
            return ResidenceLevel;
        };
    
        void setValue(int id,Colour Color,Residence level)
        {
            ID = id;
            VertexColor = Color;
            ResidenceLevel = level;
        };
    

        bool bulidingByColor(enum Colour Color)       //建筑房子
        {
             switch (ResidenceLevel)
            {
            case Nothing:
                {
                    VertexColor = Color;
                    ResidenceLevel = Basement;
                    return true;
                }
            case Basement:
                {
                    if (Color == VertexColor)
                    {
                        ResidenceLevel = House;
                        return true;
                    }
                    break;
                }
            case House:
                {
                    if (Color == VertexColor)
                    {
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

};

//边

class Edge
{
    private:

        int ID;
        enum Colour edgeColor;    //拥有者颜色
    
    public:
        Edge()
        {
            ID = 0;
            edgeColor = None;
        };
        Colour getColor(){return edgeColor;};
    
        string show()       //画图用,输出ID或颜色
        {
            switch (edgeColor)
            {
            case Blue:
                return "BR";
            case Red:
                return  "RR";
            case Orange:
                return "OR";
            case Yellow:
                return  "YR";
            default:
                {
                    if (ID>9)
                    {
                        return to_string(ID);
                    }
                    else
                    {
                        return " "+to_string(ID);
                    }
                }
            }
        };
        int getID()
        {
            return ID;
        };
        void setValue(int id,enum Colour color)
        {
            ID = id;
            edgeColor = color;
        };
       

        bool BulidingRoadByColor(enum Colour Color)      //修路
        {
            if ( (edgeColor != None) && (Color != None) )
            {
                return false;
            }else
            {
                edgeColor=Color;
                return  true;
            }
        }
};



//块

class Tile
{
    private:

        int ID;
        Vertex *  ptheVertex[6];    //指向6个顶点的指针数组
        Edge *  ptheEdge[6];        //指向6条边的指针数组
        enum Resource ResourceID;   //本区块的资源ID
        int ResourceNum;            //本区块资源数量

    public:
        Tile()
        {
            ID = 0;
            ResourceID = PARK ;
            ResourceNum = 0 ;
            for (int i=0; i<6; i++) {
                ptheVertex[i] = NULL;
                ptheEdge[i] = NULL;
            }
            
        
        };
    
        string showID()
        {
            if (ID>9)
            {
                return to_string(ID);
            }
            else{
                return " "+to_string(ID);
            }
            
            
        };

        string showResourceNum()
        {
            if (ResourceID == PARK)
            {
                return "  ";
            }
            if (ResourceNum < 10)
            {
                return " " + to_string(ResourceNum);
            }
            return to_string(ResourceNum);
        };
    
        int getResourceNum()
        {
            return ResourceNum;
        };
    
        Resource getResourceID()
        {
            return ResourceID;
        };

        string showResource()
        {
            switch (ResourceID)
            {
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
        
        };

        void setValue(int id,Resource rID,int rNum)
        {   
            ID = id;
            ResourceID = rID;
            ResourceNum = rNum;
        };

        void init(int id,Vertex* v,Edge* e,Resource rID,int rNum) //初始化,主要是指针指向正确的位置
        {
            ID = id;
            ResourceID = rID;
            ResourceNum = rNum;
            for (int i = 0; i < 6; i++)
            {
               ptheVertex[i] = v+Vertexdata[id][i];
               ptheEdge[i] = e+Edgedata[id][i];
            }
        };
};

class Board
{
    private:

        Vertex  theVertex[VERTEXNUM];    //全部54个顶点数组, 每个Tile对象里面的指针就指向这里面的其中6个
        Edge    theEdge[EDGENUM];        //全部72条边数组,每个Tile对象里面的指针就指向这里面的其中6个
        Tile theTile[TILENUM];             //19个Tile的数组
        int Geese;                          //鹅的位置
    
        Builder theBuilder[4];              //4个玩家
        
        RES resRoad;       //修路需要的资源
        RES resBasement;   //一级需要的资源
        RES resHouse;          //二级需要的资源
        RES resTower;           //三级需要的资源

 

    public:
        Board()
        {
            for (int i = 0; i < 54; i++)
            {
               theVertex[i].setValue(i,None,Nothing);
            }
            for (int i = 0; i < 72; i++)
            {
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
            
            //随机初始化棋盘
            Resource place[19]={PARK,WIFI,WIFI,WIFI,HEAT,HEAT,HEAT,BRICK,BRICK,BRICK,BRICK,ENERGY,ENERGY,ENERGY,ENERGY,GLASS,GLASS,GLASS,GLASS};
            int num[19]={2,3,4,5,6,7,8,9,10,11,12}; //保证每个数字都有
            
            num[11] = rand()%4+3; //凑足2个3-6
            num[12] = rand()%4+8; //凑足2个8-11
            
            for (int i=13;i<19;i++)
            {
                int n = 7;
                while (n == 7)
                {
                    n = rand()%9+3; //剩下6个随机填充3-11
                }
                num[i] = n;
            }

            
            
            Resource tmpR;
            int tmp = 0;
            
            for (int j=0;j<100;j++) //随机排上100遍
            {
                for (int i=0; i<19; i++)
                {
                    int n=0;
                    
                    n =rand()%(19-i);
                
                    tmpR = place[18-i];
                    place[18-i] = place[n];
                    place[n] = tmpR;
                    
                    n =rand()%(19-i);
                    
                    tmp = num[18-i];
                    num[18-i] = num[n];
                    num[n] = tmp;
                }//for
            }//for
            
            for (int i=0; i<19; i++) //把PARK与7对应起来
            {
                if ( PARK == place[i] )
                {
              //      Geese = i;//鹅初始化到park
                    for (int j=0;j<19;j++)
                    {
                     if (7 == num[j])
                     {
                         num[j] = num[i];
                         num[i] = 7;
                         
                     }
                        
                    }
                }
            }//for
            
            for (int i=0;i<19;i++) //把排好序的数据填入Tile
            {
                theTile[i].init(i,theVertex,theEdge,place[i],num[i]);
            }
        };
    
        bool enoughToBuildRoad(Colour color)
        {
            RES resTmp = {0};
            
            for (int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    resTmp = theBuilder[i].getRes();
                    break;
                }
            }
            
            if (resTmp.numBrick < resRoad.numBrick )    return  false;
            if (resTmp.numEnergy < resRoad.numEnergy )  return  false;
            if (resTmp.numGlass < resRoad.numGlass)     return  false;
            if (resTmp.numHeat < resRoad.numHeat)       return  false;
            if (resTmp.numWifi < resRoad.numWifi)      return  false;
            return true;
        };
        bool enoughToBuildHouse(Colour color,int vertexID)
        {
            RES resTmp = {0};
            
            for (int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    resTmp = theBuilder[i].getRes();
                    break;
                }
            }
            
            switch ( theVertex[vertexID].getResidenceLevel() )
            {
                case Nothing:
                    {
                        if (resTmp.numBrick < resBasement.numBrick )    return  false;
                        if (resTmp.numEnergy < resBasement.numEnergy )  return  false;
                        if (resTmp.numGlass < resBasement.numGlass)     return  false;
                        if (resTmp.numHeat < resBasement.numHeat)       return  false;
                        if (resTmp.numWifi < resBasement.numWifi)      return  false;
                        return true;
                    }
                    break;
                case Basement:
                    {
                        if (resTmp.numBrick < resHouse.numBrick )    return  false;
                        if (resTmp.numEnergy < resHouse.numEnergy )  return  false;
                        if (resTmp.numGlass < resHouse.numGlass)     return  false;
                        if (resTmp.numHeat < resHouse.numHeat)       return  false;
                        if (resTmp.numWifi < resHouse.numWifi)      return  false;
                        return true;
                    }
                    break;
                case House:
                    {
                        if (resTmp.numBrick < resTower.numBrick )    return  false;
                        if (resTmp.numEnergy < resTower.numEnergy )  return  false;
                        if (resTmp.numGlass < resTower.numGlass)     return  false;
                        if (resTmp.numHeat < resTower.numHeat)       return  false;
                        if (resTmp.numWifi < resTower.numWifi)      return  false;
                        return true;
                    }
                default:
                    break;
            }
            return  false;
        };
    
        bool payForBuildRoad(Colour color)
        {
            for (int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    return  theBuilder[i].removeRes(resRoad);
                }
            }
            return false;
        
        };
        
        bool payForBuildHouse(Colour color,int vertexID)
        {
            if (theVertex[vertexID].getColour() != color) return false;
            
            for (int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    switch ( theVertex[vertexID].getResidenceLevel() )
                    {
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
        };
    
    
    
    
        void setLoaded(Colour color,bool bLoaded)
        {
            for ( int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    theBuilder[i].setLoaded(bLoaded);
                    return;
                }
            }
        };
    
        bool getLoaded(Colour color)
        {
            for ( int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    
                    return theBuilder[i].getLoaded();
                }
            }
            return true;
        };

        int getBuilderResNum(Colour color,Resource aResId)
        {
            RES resTMP = {0};
            for ( int i=0;i<4;i++)
            {
                if (theBuilder[i].getColor() == color )
                {
                    
                    resTMP = theBuilder[i].getRes();
                }
            }
            switch (aResId)
            {
            case BRICK:
                return resTMP.numBrick;
                break;
            case ENERGY:
                return resTMP.numEnergy;
            case GLASS:
                return resTMP.numGlass;
            case HEAT:
                return resTMP.numHeat;
            case WIFI:
                return resTMP.numWifi;
            default:
                break;
            }
            return -1;

        };

        void setTile(int id,Resource rID,int rNum)
        {
            theTile[id].setValue(id,rID,rNum);
        };

        void setHouse(int id,Colour Color,Residence level)
        {
            //不判断条件，用于load
            theVertex[id].setValue(id,Color,level);

        };
        void setRoads(int id,Colour color)
        {
            //不判断条件，用于load
            theEdge[id].setValue(id,color);

        };
        Resource strToResource(string str)
        {
            if ( str == "BRICK") return BRICK;
            if ( str == "ENERGY") return ENERGY;
            if ( str == "GLASS") return GLASS;
            if ( str == "HEAT")     return HEAT;
            if ( str == "WIFI")     return WIFI;
            return PARK;
        }
        Colour strToColour(string str)
        {
            if (str == "BLUE") return Blue;
            if (str == "RED")   return Red;
            if (str == "ORANGE")    return Orange;
            if (str == "YELLOW")    return Yellow;
            return None;

        };
        int getGeese()
            {
                return Geese;
            }
        string showGeese(int g)
        {
            
            if ( Geese == g )
            {
            //    if ( ! (theTile[g].getResourceID() == PARK) )
                {
                    return "  GEESE  ";
                }
            }
            return "         ";
        };

        bool stolen(Colour inColor,Colour outColor)
        {
            RES tmpRES = {0};
            RES outRES = {0};

            if ( inColor == outColor )
            {
                return false;
            }


            for (int i=0;i<4;i++)
            {
             if  ( theBuilder[i].getColor() == outColor )
              {
                  outRES = theBuilder[i].getRes();
              }
            }

            int r1,r2,r3,r4,r5 = 0;
            r1 = outRES.numBrick;
            r2 = outRES.numEnergy;
            r3 = outRES.numGlass;
            r4 = outRES.numHeat;
            r5 = outRES.numWifi;
            
            int n = rand()%(r1+r2+r3+r4+r5);

            if ( n < r1 )
            {
                tmpRES.numBrick = 1;
                cout << "Builder "<<showColor(inColor)<<" steals BRICK from builder "<<showColor(outColor)<<"."<<endl;
            }
            else if ( n < r2 )
            {
                tmpRES.numEnergy = 1;
                 cout << "Builder "<<showColor(inColor)<<" steals ENERGY from builder "<<showColor(outColor)<<"."<<endl;
            }else if (n<r3)
            {
                tmpRES.numGlass= 1;
                 cout << "Builder "<<showColor(inColor)<<" steals GLASS from builder "<<showColor(outColor)<<"."<<endl;
            }else if (n<r4)
            {
                tmpRES.numHeat = 1;
                 cout << "Builder "<<showColor(inColor)<<" steals HEAT from builder "<<showColor(outColor)<<"."<<endl;
            }else
            {
                tmpRES.numWifi = 1;
                 cout << "Builder "<<showColor(inColor)<<" steals WIFI from builder "<<showColor(outColor)<<"."<<endl;
            }
            for (int i=0;i<4;i++)
            {
             if  ( theBuilder[i].getColor() == outColor )
              {
                  theBuilder[i].removeRes(tmpRES);
              }
              if  ( theBuilder[i].getColor() == inColor )
              {
                  theBuilder[i].addRes(tmpRES);
              }
            }
            return true;
        };
        void addRes(Colour color,RES res)
        {
            for (int i=0;i<4;i++)
            {
                if ( theBuilder[i].getColor() == color )
                {
                    theBuilder[i].addRes(res);
                }
            }
        };

        string showColor(Colour color)
        {

            switch (color)
            {
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
        void initGeese(int geese)
        {
            Geese = geese;
        };
    
        string setGeese(Colour curColor,int geese)
        {
            //返回可以偷窃的所有字串
            string str="";
            if (geese < 0) return str;
            if (geese >= TILENUM) return str;
            if (Geese == geese) return str;

            Geese = geese;  //设置鹅的位置
            Colour tmpColor = None;
            bool bBlue =false;
            bool bRed = false;
            bool bOrange = false;
            bool bYellow =false;

            for (int i=0;i<6;i++)
            {
                tmpColor = theVertex[Vertexdata[geese][i]].getColour();

                switch ( tmpColor )
                {
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

           switch ( curColor )  //排除自身
                {
                case Blue:
                    bBlue = false;
                    break;
                case Red:
                    bRed = false;
                case Orange:
                    bOrange = false;
                case Yellow:
                    bYellow = false;
                default:
                    break;
                }

            
            for (int i=0;i<4;i++)
            {
                if (bBlue)
                {
                    if (( theBuilder[i].getColor() == Blue ) && ( theBuilder[i].haveRes() ))
                    {
                        str += "Blue,";
                    }
                }
                if (bRed)
                {
                    if (( theBuilder[i].getColor() == Red ) && ( theBuilder[i].haveRes() ))
                    {
                        str += "Red,";
                    }
                }
                if (bOrange)
                {
                    if (( theBuilder[i].getColor() == Orange ) && ( theBuilder[i].haveRes() ))
                    {
                        str += "Orange,";
                    }
                }
                if (bYellow)
                {
                    if (( theBuilder[i].getColor() == Yellow ) && ( theBuilder[i].haveRes() ))
                    {
                        str += "Yellow,";
                    }
                }
            }//for
            //去掉最后逗号
            if (str.length()>0)
            {
                str = str.substr(0,str.length()-1);
                str += ".";
            }
            //
            return str;
        };
    
        void dropResToGeese()        //投掷到7的时候,资源总量》=10的,随机丢掉一半资源
        {
 
            for (int i=0;i<4;i++)
            {
                int n=0;
                RES bRes={0};
                bRes = theBuilder[i].getRes();
                n += bRes.numBrick;
                n += bRes.numEnergy;
                n += bRes.numGlass;
                n += bRes.numHeat;
                n += bRes.numWifi;

                if (n >= 10)
                {
                    int m = n/2;
                    RES tRes={0};

                    for (int a=0;a<m;a++)
                    {
                        int r1=bRes.numBrick - tRes.numBrick;
                        int r2=bRes.numEnergy - tRes.numEnergy;
                        int r3=bRes.numGlass - tRes.numGlass;
                        int r4=bRes.numHeat - tRes.numHeat;
                        int r5=bRes.numWifi - tRes.numWifi;

                        int p = rand()%(r1+r2+r3+r4+r5);
                        if (p < r1)
                        {
                            tRes.numBrick +=1;
                        }
                        else if (p<(r1+r2))
                        {
                            tRes.numEnergy += 1;
                        }else if (p<(r1+r2+r3))
                        {
                            tRes.numGlass += 1;
                        }else if (p<(r1+r2+r3+r4))
                        {
                            tRes.numHeat +=1;
                        }else
                        {
                            tRes.numWifi += 1;
                        }
                    }

                    theBuilder[i].removeRes(tRes);

                    //输出信息

                    cout <<"Builder "<<theBuilder[i].showColor()<<" Loses "<<to_string(m)<<" Resources to the geese. They loss:"<<endl;
                    if  (tRes.numBrick > 0  )  cout<<to_string(tRes.numBrick)<<" BRICK"<<endl;
                    if  (tRes.numEnergy > 0  )   cout<<to_string(tRes.numEnergy)<<" ENERGY"<<endl;
                    if  (tRes.numGlass > 0  )   cout<<to_string(tRes.numGlass)<<" GLASS"<<endl;
                    if  (tRes.numHeat > 0  )   cout<<to_string(tRes.numHeat)<<" HEAT"<<endl;
                    if  (tRes.numWifi > 0  )   cout<<to_string(tRes.numWifi)<<" WIFI"<<endl;

                }//if
   
            }

        };

        void addResByDice(int dice)
        {
            if (dice == 7)
            {
                //r如果是7单独处理
                 return;
            }
            
            Builder tmpBuilder[4];
            tmpBuilder[0].setColor(Blue);
            tmpBuilder[1].setColor(Red);
            tmpBuilder[2].setColor(Orange);
            tmpBuilder[3].setColor(Yellow);


            for (int i=0;i<TILENUM;i++)
            {
              if (theTile[i].getResourceNum() == dice)
                {
                    if (Geese != i)  //如果鹅不在这里
                    {
                        RES res={0};
                        switch (theTile[i].getResourceID())
                        {
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

                        for (int j=0;j<6;j++)
                        {

                            for (int k = 0; k < theVertex[Vertexdata[i][j]].getResidenceLevel(); k++)
                            {//根据建筑等级确定循环次数  
                                switch (theVertex[Vertexdata[i][j]].getColour())
                                {
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
                            }//for
                          
                        }
                    }
  
                }
            }//for

            bool bNoBuilder = true;

            for (int i=0;i<4;i++)
            {
                if (tmpBuilder[i].haveRes())
                {
                    bNoBuilder = false;
                    theBuilder[i].addRes(tmpBuilder[i].getRes());
                    //输出信息
                    cout << "Builder "<<theBuilder[i].showColor()<<" gained:"<< endl;
                    if (tmpBuilder[i].getRes().numBrick > 0 ) cout << to_string(tmpBuilder[i].getRes().numBrick) << " BRICK" <<endl;
                    if (tmpBuilder[i].getRes().numEnergy > 0 ) cout << to_string(tmpBuilder[i].getRes().numEnergy) << " ENERGY" <<endl;
                    if (tmpBuilder[i].getRes().numGlass > 0 ) cout << to_string(tmpBuilder[i].getRes().numGlass) << " GLASS" <<endl;
                    if (tmpBuilder[i].getRes().numHeat> 0 ) cout << to_string(tmpBuilder[i].getRes().numHeat) << " HEAT" <<endl;
                    if (tmpBuilder[i].getRes().numWifi > 0 ) cout << to_string(tmpBuilder[i].getRes().numWifi) << " WIFI" <<endl;
                } 
            }
            if (bNoBuilder)
            {
                cout << "No builders gained resources." << endl;
            }
            return ;
        };

        bool BulidingRoad(Colour color,int id)
        {
            if ( canBuildroad(color,id) )
            {
               return theEdge[id].BulidingRoadByColor(color);
            }
            return false;
        };

        bool canBuildroad(Colour color,int id)
        {
            if (id >= EDGENUM) return false;
            if (id < 0) return false;
            if (int(color) >= 5) return false;
            if (int(color) <= 0) return false;

            if (theEdge[id].getColor() != None)
            {
                return false;
            }

            for (int i=0;i<TILENUM;i++)
            {
                for (int j=0;j<6;j++)
                {
                    if (id == Edgedata[i][j] )
                    {
                        //
                       int m=0;
                       int n =5;

                       if (5 > j)
                       {
                           m=j+1;
                       }
                       if (0 < j)
                       {
                           n=j-1;
                       }

                       if ( theVertex[Vertexdata[i][j]].getColour() == color )
                       {
                            return true;
                       }
                       if ( theVertex[Vertexdata[i][m]].getColour() == color )
                       {
                            return true;
                       }

                       if ( theEdge[Edgedata[i][n]].getColor() == color )
                       {
                           if ( theVertex[ Vertexdata[i][j]].getColour() == color  )
                           {
                               return true;
                           }
                           if ( theVertex[ Vertexdata[i][j]].getColour() == None  )
                           {
                               return true;
                           }
                       }
                       if ( theEdge[Edgedata[i][m]].getColor() == color )
                       {
                            if ( theVertex[ Vertexdata[i][m]].getColour() == color  )
                           {
                               return true;
                           }
                           if ( theVertex[ Vertexdata[i][m]].getColour() == None  )
                           {
                               return true;
                           }
                       }

                    }

                }

            }
            return false;
        };

        bool trade(Colour giveColor,Colour takeColor,Resource give,Resource take)
        {
    
            RES resGive = {0};
            RES resTake= {0};

            switch (give)
            {
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
            default:
                break;
            }

            switch (take)
            {
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
            default:
                break;
            }

            for (int i = 0; i < 4; i++)
            {
                if ( theBuilder[i].getColor() == giveColor )
                {
                    theBuilder[i].removeRes(resGive);
                    theBuilder[i].addRes(resTake);
                }
                if ( theBuilder[i].getColor() == takeColor )
                {
                    theBuilder[i].removeRes(resTake);
                    theBuilder[i].addRes(resGive); 
                }
            }
            
            return true;
        };

        bool BuildingHouse(Colour color,int id,bool bFirst = false)
        {
            if (canBuildHouse(color,id,bFirst) )
            {
                return theVertex[id].bulidingByColor(color);
            }
            return false;
        };


        bool canBuildHouse(Colour color,int id,bool bFirst = false)
        {
            if (id >= VERTEXNUM) return false;
            if (id < 0) return false;
            if (int(color) >= 5) return false;
            if (int(color) <= 0) return false;

    
          

            if( theVertex[id].getResidenceLevel() != Nothing )  //有建筑
            {
                if ( theVertex[id].getResidenceLevel() == Tower )  //已经顶级
                {
                    return false;
                }
                if ((theVertex[id].getColour() != color)) //不是自己的
                {
                    return false;
                }
                if (bFirst)  //有建筑但是开局
                {
                    return false;
                }
                return true;
            }
            else  //空地
            {
                bool bRoad = false ; //相邻是否有自己的路
            
                for (int  i = 0; i < TILENUM; i++)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        if (id == Vertexdata[i][j])
                        {
                            int m=0; //+
                            int n =5; //-

                            if (5 > j)
                            {
                                m=j+1;
                            }
                            if (0 < j)
                            {
                                n=j-1;
                            }

                            if ( theEdge[Edgedata[i][n]].getColor() == color )  //有自己的路
                            {
                                    bRoad = true;
                            }
                            if ( theEdge[Edgedata[i][j]].getColor() == color )  //有自己的路
                            {
                                    bRoad = true;
                            }
                                
                            if ( theVertex[Vertexdata[i][m]].getColour() != None)  //相邻不是空的
                            {
                                return false;
                            } 
                            if ( theVertex[Vertexdata[i][n]].getColour() != None) //相邻不是空的
                            {
                                return false;
                            }                      
                        }
                    }  
                }//for
                if ( bRoad )
                {
                    return true;;
                }
                else if (bFirst )
                {
                    return true;;
                }
                return false;

            }//if

 
        };
    
        void show()
        {
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

            for (int n=0;n<3;n++)
            {
                cout<<"      |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
                cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
                cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
                cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
                cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;

                for (int a=0;;a++)
                {
                    if (a == 0)
                    {
                        cout<<"        ";
                    }
                    cout<<"|";
                    if (a>=5)
                    {
                        cout<<endl;
                        break;
                    }
                    if (a == 1 || a == 3)
                    {
                        cout<<showGeese(g++);
                        continue;
                    }
                    cout<<"         "; // 可能有鹅
                }
                for (int a=0;;a++)
                {
                    if (a == 0)
                    {
                        cout<<"       ";
                    }
                    cout<<theEdge[j++].show()<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show();
                    if (a>=2)
                    {
                        cout<<endl;
                        break;
                    }
                    cout<<"        ";
                }
                 for (int a=0;;a++)
                {
                    if(a == 0)
                    {
                        cout<<"        ";
                    }
                    cout<<"|"<<theTile[m++].showResource()<<"|";
                    if (a>=2)
                    {
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

                for (int a=0;;a++)
                {
                    if (a > 5)
                    {
                        cout<<endl;
                        break;
                    }
                    if (a == 0)
                    {
                        cout<<"        |";
                        continue;
                    }
                    if (a % 2 == 1)
                    {
                        cout<<showGeese(g++)<<"|";// 鹅
                        continue;
                    }
                    cout<<"         |";
                }
                for (int a=0;;a++)
                {
                    if (a == 0)
                    {
                        cout<<"       ";
                    }
                    cout<<theEdge[j++].show()<<"        "<<theEdge[j++].show();
                    if (a>=2)
                    {
                        cout<<endl;
                        break;
                    }
                    cout<<"   "<<theTile[l++].showID()<<"   ";;
                }
                for (int a=0;;a++)
                {
                    if (a == 0)
                    {
                        cout<<"        ";
                    }
                    cout<<"|         |";
                    if (a>=2)
                    {
                        cout<<endl;
                        break;
                    }
                    cout<<theTile[m++].showResource();
                }
            }

            //最后
            cout<<"      |"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
            cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
            cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
            cout<<"  "<<theTile[k++].showResourceNum()<<"  ";
            cout<<"|"<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;

            cout<<"         ";
            for (int a=0;a<4;a++)
                {
                    if ( a % 2 == 1)
                    {
                        cout<<showGeese(g++)<<"|";
                        continue;
                    }
                    cout<<"         |"; // 可能有鹅
                }
                cout<<endl;

//          
            cout<<"                 "<<theEdge[j++].show();
            cout<<"        "<<theEdge[j++].show();
            cout<<"   "<<theTile[l++].showID()<<"   "<<theEdge[j++].show();
            cout<<"        "<<theEdge[j++].show();
            cout<<endl;
//
            cout<<"                  |         |";
            cout<<theTile[m++].showResource()<<"|         |"<<endl;
//
            cout<<"                |";
            cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
            cout<<"  "<<theTile[k++].showResourceNum()<<"  |";
            cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|";
            cout<<endl;
            //
            cout<<"                            |"<<showGeese(g++)<<"|"<<endl; // 可能有鹅
            //
            cout<<"                           ";
            cout<<theEdge[j++].show()<<"        "<<theEdge[j++].show()<<endl;

            //
            cout<<"                            |         |"<<endl;

            //
            cout<<"                          |";
            cout<<theVertex[i++].show()<<"|--"<<theEdge[j++].show()<<"--|"<<theVertex[i++].show()<<"|"<<endl;

        };
        string showExistHouse()
        {
            string str="";
            for (int i=0;i<VERTEXNUM;i++)
            {
                if ( theVertex[i].getColour() != None )
                {
                    str += " ";
                    str += to_string(i);
                }
            }
            return  str;
        };
        void showBuilderResidnce(Colour color)
        {
            for (int i=0;i<4;i++)
            {
                if ( theBuilder[i].getColor() == color )
                {
                    cout << theBuilder[i].showColor() << " has built:" << endl; 
                }
            }
            for (int i=0;i<VERTEXNUM;i++)
            {
                if ( theVertex[i].getColour() == color )
                {
                    cout << to_string(i) << " " << theVertex[i].showResidence() <<endl;
                }

            }
            
        };
        string showVertexLevel(int id)
        {
            switch ( theVertex[id].getResidenceLevel() )
            {
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
        };
        Residence getVertexLevel(int vID)
        {
            return theVertex[vID].getResidenceLevel() ;
        };
        int getBuilderResPoint(Colour color)
        {
            int p = 0; //资源点

            for (int i=0;i<VERTEXNUM;i++)
            {
                if (theVertex[i].getColour() == color)
                {
                    switch (theVertex[i].getResidenceLevel())
                    {
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
            }//for
            return p;

        };
        string showBuilderStatus(Colour color)
        {
            string str="";
            int p = 0; //资源点
            RES res = {0};
            
            p = getBuilderResPoint(color);
            
            int i = 0;
            for (i=0;i<4;i++)
            {
                if ( theBuilder[i].getColor() == color )
                {
                    res = theBuilder[i].getRes();
                    break;
                }
            }
            //Builder Blue    has 7 building points, 0 BRICK, 0 ENERGY, 0 GLASS, 5 HEAT, 0 WIFI.
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
        };
        string getTileSaveData()
        {
            string str = "";
            int n;

            for (int i=0; i<TILENUM;i++)
            {
                n = theTile[i].getResourceNum();
               switch (theTile[i].getResourceID())
               {
               case BRICK:
                   {
                       str += "0";
                       str += " ";
                       str += to_string(n);
                       str += " ";
                   }
                   break;
                case ENERGY:
                    {
                        str += "1";
                        str += " ";
                        str += to_string(n);
                        str += " ";
                    } 
                    break;
                case GLASS:
                    {
                        str += "2";
                        str += " ";
                        str += to_string(n);
                        str += " ";
                    }
                    break;
                case HEAT:
                    {
                        str += "3";
                        str += " ";
                        str += to_string(n);
                        str += " ";
                    }
                    break;
                case WIFI:
                    {
                        str += "4";
                        str += " ";
                        str += to_string(n);
                        str += " ";
                    }
                    break;
                case PARK:
                    {
                        str += "5";
                        str += " ";
                        str += "7";
                        str += " ";
                    }
                    break;
               
               default:
                   break;
               } 

            }//for
            if (str.length()>0)
            {
                str = str.substr(0,str.length()-1);
            }
            return str;
        };
        string getBuilderSaveDate(Colour color)
        {
            string str ="";
            RES res ={0};

            for (int i=0;i<4;i++)
            {
                if ( theBuilder[i].getColor() == color )
                {
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

            for (int i=0;i<EDGENUM;i++)
            {
                if (theEdge[i].getColor() == color)
                {
                    str += to_string(i);
                    str += " ";
                }
            }
            str += "h";
            for (int i=0;i<VERTEXNUM;i++)
            {
                if (theVertex[i].getColour() == color)
                {
                    str += " ";
                    str += to_string(i);
                    str += " ";
                    str += theVertex[i].showResidence();
                }
            }

            return str;
        };

};





class game
{
    private:
        Board theBoard;
        Colour curTurn;
    
        vector<string> strList;
    
        void getInputLine()
        {
            string input="";
            
            if (cin.eof())  //EOF标志
            {
                saveToFile("backup.sv");
                exit(10);
            }
            getline(cin, input);

            stringstream inPutSS(input);
            string strTMP;
            while(inPutSS>>strTMP)
            {
                strList.push_back(strTMP);
            }
        };
  
    public:
        game()
        {
           
            curTurn = Blue;
        }

        void turn(bool b=true)
        {
            if (b)
            {
                switch (curTurn)
                {
                    case Blue:
                        curTurn = Red;
                        break;
                    case Red:
                        curTurn = Orange;
                        break;
                    case Orange:
                        curTurn = Yellow;
                        break;
                    case Yellow:
                        curTurn = Blue;
                        break;
                    default:
                        break;
                }
            }else
            {
                switch (curTurn)
                {
                    case Blue:
                        curTurn = Yellow;
                        break;
                    case Yellow:
                        curTurn = Orange;
                        break;
                    case Orange:
                        curTurn = Red;
                        break;
                    case Red:
                        curTurn = Blue;
                        break;
                    default:
                        break;
                }
            }
        };
        string showcurTurn()
        {
            return theBoard.showColor(curTurn);
        };
        bool boardFormFile(string fileName)
        {
            string line="";
            string buf="";
            stringstream ss;
    
            //读取board文件
            ifstream  loadfile( fileName,ios::in);
            if ( !loadfile)
            {
               // cout << "can not open this file" <<endl;
                return false;
            }
            else
            {
                getline (loadfile, line);
                ss.clear();
                ss.str(line);
                int j=0;

                while (ss >> buf)
                {
                    int id = stoi(buf);
                    ss >> buf;
                    int rNum = stoi(buf);
                    Resource rID;
                                

                    switch (id)
                    {
                        case 0:
                            rID = BRICK;
                            break;
                        case 1:
                            rID = ENERGY;
                            break;
                        case 2:
                            rID = GLASS;
                            break;
                        case 3:
                            rID = HEAT;
                            break;
                        case 4:
                            rID = WIFI;
                            break;
                        case 5:
                            rID = PARK;
                           // theBoard.initGeese(j);  //把鹅放到park
                            break;
                        default:
                            break;
                        }

                        theBoard.setTile(j,rID,rNum);

                        j++;
                    }//while
            }//if
            return true;
        };
        bool saveToFile(string fileName)
        {
            RES res = {0};
            ofstream outfile;
            outfile.open(fileName);


            if(! outfile)
            {
                outfile <<"Open File Fail:"<<fileName<<endl;
                return false;  
            }

            outfile << to_string(int(curTurn)-1) <<endl;  //第一行curTurn

            for (int i=0;i<4;i++)   //四个玩家数据
            {
                outfile << theBoard.getBuilderSaveDate(Colour (i+1)) << endl;
            }
            outfile << theBoard.getTileSaveData() << endl;  //地图数据
            
            if (( (theBoard.getGeese()) >=0 ) && ( theBoard.getGeese() <TILENUM ))
            {
                outfile << to_string(theBoard.getGeese()) << endl;
            }
            

            outfile.close();
            return true;
        };
        bool loadFormFile(string fileName)
        {

            string line="";
            string buf="";
            
            bool b_h = false;
            //读取save文件
            ifstream  loadfile( fileName,ios::in);
            if ( !loadfile)
            {
               // cout << "can not open this file" <<endl;
                return false;
            }
            else
            {
                stringstream ss;

                for (int i=0;i<7;i++)
                {
                    getline (loadfile, line);

                    switch (i)
                    {
                    case 0:
                        curTurn = Colour( stoi (line)+1);
                        break;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        {
                            RES res={0};
                            ss.clear();
                            ss.str(line);
                            int j=0;
                            b_h = false;

                            while (ss >> buf)
                            {
                                switch (j)
                                {
                                case 0:
                                    res.numBrick = stoi(buf);
                                    break;
                                case 1:
                                    res.numEnergy = stoi(buf);
                                    break;
                                case 2:
                                    res.numGlass = stoi(buf);
                                    break;
                                case 3:
                                    res.numHeat = stoi(buf);
                                    break;
                                case 4:
                                    res.numWifi = stoi(buf);
                                    theBoard.addRes( Colour(i),res);                                    
                                    break;
                                default: //第5列以后都放到默认
                                    {
                                        if (buf == "r")  
                                        {
                                            break;
                                        }
                                        if (buf == "h")  //遇到h后置b_h为true
                                        {
                                            b_h = true;
                                            break;
                                        }
                                        if (! b_h) //如果在h之前
                                        {
                                            theBoard.setRoads(stoi(buf),Colour(i));  //
                                        }
                                        else
                                        {
                                            int id=stoi(buf);
                                            Residence level = Nothing;
                                            ss >> buf;

                                            if (buf == "B") level = Basement;
                                            if (buf == "H") level = House;
                                            if (buf == "T") level = Tower;

                                            theBoard.setHouse(id,Colour(i),level);
                                            break;
                                        }
                                    }
                                    break;
                                }//switch
                                j++;
                            }
                        }
                        break;
                    case 5:
                        {
                            ss.clear();
                            ss.str(line);
                            int j=0;
                            

                            while (ss >> buf)
                            {
                                int id = stoi(buf);
                                ss >> buf;
                                int rNum = stoi(buf);
                                Resource rID;
                                

                                switch (id)
                                {
                                case 0:
                                    rID = BRICK;
                                    break;
                                case 1:
                                    rID = ENERGY;
                                    break;
                                case 2:
                                    rID = GLASS;
                                    break;
                                case 3:
                                    rID = HEAT;
                                    break;
                                case 4:
                                    rID = WIFI;
                                    break;
                                case 5:
                                    rID = PARK;
                                    break;
                                
                                default:
                                    break;
                                }

                                theBoard.setTile(j,rID,rNum);

                                j++;
                            }
                        }
                        break;
                    case 6:
                        {
                            ss.clear();
                            ss.str(line);
                           
                            ss >> buf;
                            if ( buf.length() > 0 )
                            {
                                theBoard.initGeese( stoi (buf));
                            }
                        }
                        break;

                    default:
                        break;
                    }

                }  
            }

            loadfile.close();
            return true;
        };
        
        bool init()
        {
            int VertexID;

            theBoard.show();
            

            for (int i=1;i<=8;i++)
            {
                cout<<"Builder "<< this->showcurTurn()<<", where do you want to build a basement?"<<endl;

                while (true)
                {
                    VertexID = getAInt();
                    
                    if (VertexID== -1)
                    {

                        cout<< "ERROR: not an integer" <<endl;
                        cout << "Builder " << this->showcurTurn() <<" where do you want to build a basement?" <<endl;

                    }else  if ((VertexID >= 0)&&(VertexID < VERTEXNUM))
                    {
                        if (theBoard.BuildingHouse(curTurn, VertexID,true))
                        {
                            break;
                        }
                        else
                        {
                            cout<< "You cannot build here." <<endl;
                            cout << "Basements already exist as locations:" << theBoard.showExistHouse() <<endl;
                            cout << "Builder " << this->showcurTurn() <<" where do you want to build a basement?" <<endl;
                        }
                    }
                    else
                    {
                        cout<< "You cannot build here." <<endl;
                        cout<< "location does not exist." <<endl;
                        cout << "Builder " << this->showcurTurn() <<" where do you want to build a basement?" <<endl;
                    }
                }
                if (i%4 != 0)
                {
                    this->turn(i <5 );
                }
            }
            return true;
        };
    
        int roll()
        {
            int dice = 0;
            
            while (true)
            {
                string input = "";
                
                input = getACommand();
        
                if (input == "LOAD")
                {
                    cout<< "Builder " << this->showcurTurn() << " now has loaded Dice"<<endl;
            
                    theBoard.setLoaded(curTurn, true);
                }
                else if (input == "FAIR")
                {
                    cout<< "Builder " << this->showcurTurn() << " now has fair Dice."<<endl;
                    theBoard.setLoaded(curTurn, false);
                }
                else  if (input == "ROLL")
                {
                    if (theBoard.getLoaded(curTurn))
                    {
                        cout << "Input a roll between 2 and 12:" << endl;
                        
                        while (true) //输入骰子数字
                        {
                            int in = 0;
                            cout<< ">";
                            
                            in = getAInt();
                            if ( in == -1 )
                            {
                                cout << "ERROR:  isn't a valid integer." << endl;
                            }
                            else if (( in <2 ) || (in > 12) )
                            {
                                cout << "Invalid roll " << to_string(in) << endl;
                                cout << "Input a roll between 2 and 12:" << endl;
                            }
                            else
                            {
                                dice = in;
                                break;
                            }
                        }
                    }
                    else //正常扔骰子
                    {
                        dice = ( rand()%6+1 )+( rand()%6+1 );
                        cout << "The number you rolled is " << to_string(dice) << endl;
                    }
                    break;
                }
                else if (input == "STATUS")
                {
                    cout << theBoard.showBuilderStatus(Blue) << endl;
                    cout << theBoard.showBuilderStatus(Red) << endl;
                    cout << theBoard.showBuilderStatus(Orange) << endl;
                    cout << theBoard.showBuilderStatus(Yellow) << endl;
                }
                else if (input == "HELP")
                {
                    cout << "Valid commands:" <<endl;
                    cout << "~ load : changes current builder's dice type to 'loaded'" << endl;
                    cout << "~ fair : changes current builder's dice type to 'fair'" << endl;
                    cout << "~ roll : rolls the dice and distributes resources." << endl;
                    cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
                    cout << "~ help : prints out the list of commands." << endl;
                }
                else
                {
                    cout << "Invalid command." << endl;
                    cout << "Please enter 'help' for a list of valid commands." << endl;
                }
            }
            
            return dice;
        };

    string getAFileName()
    {
        string str="";
        cout << ">";

        while (true)
        {
            if ( ! strList.empty())
            {
                str = strList.front();
                strList.erase(strList.begin());
                
                return str;
            }
            else
            {
                getInputLine();
            }
        }
        return "";
    };
    
        string getACommand(bool big = true)
        {
            string str="";
            cout << ">";

            while (true)
            {
                if ( ! strList.empty())
                {
                    str = strList.front();
                    strList.erase(strList.begin());
                    
                    //转换成大写
                    if (big)
                    {
                        for (int i=0;i<str.length();i++)
                        {
                            if ((str[i] >= 97)&&(str[i] <= 122) )
                            {
                                str[i] -=32;
                            }
                        }
                    }
                    
                    return str;
                }
                else
                {
                    getInputLine();
                }
            }
            return "";
        };

        int getAInt()
        {
            string str = "";
            cout << ">";

             while (true)
            {
                if ( ! strList.empty())
                {
                    str = strList.front();
                    strList.erase(strList.begin());
                
                    break;
                }
                else
                {
                    getInputLine();
                }
            }
            

            for (int i = 0; i < str.length(); i++)
            {
                if ( str[i] < 48 || str[i] >57 )
                {
                   return -1;//返回错误
                }
            }
            return stoi(str);
        };
    

        bool beginTurn()
        {
            int input=0;

            theBoard.show();
            
            cout<<"Builder "<<this->showcurTurn()<<"’s turn."<<endl;
            
            
            int dice =this->roll();
            if ( dice== 7)
            {
              
              theBoard.dropResToGeese();//丢失资源
              cout<<"Choose where to place the GEESE."<<endl;
              //移动鹅
                while (true)
                {
                    string strOut = "";
     
                    input= getAInt();
                    
                    if ((input < 0 ) ||( input >TILENUM) || (input == theBoard.getGeese()))
                    {
                        cout << "Geese can't move here." << endl;
                        cout << "Choose where to place the GEESE." << endl;
                    }
                    else
                    {
                        strOut = theBoard.setGeese(curTurn,input);
                        
                        if ( strOut.length() == 0 )
                        {
                           cout << "Builder " << showcurTurn()<< " has no builders to steal from."<<endl;
                        }
                        else  //有东西可偷
                        {
                            cout<< "Builder " << showcurTurn()<< " can choose to steal from " <<strOut<<endl;
                            while (true)
                            {
                                cout << "Choose a builder to steal from." << endl;
                                string inStr = getACommand();

                                if ( inStr == "B" ) inStr ="BLUE";
                                if ( inStr == "R" ) inStr = "RED";
                                if ( inStr == "O" ) inStr = "ORANGE";
                                if ( inStr == "Y" ) inStr = "YELLOW";
                                
                                
                                if ( (inStr == "BLUE") || (inStr == "RED") || ( inStr == "ORANGE") || (inStr == "YELLOW") )
                                {
                                    string strOutBig = strOut;
                                    
                                    for (int i=0;i<strOutBig.length();i++)
                                    {
                                        if ((strOutBig[i] >= 97)&&(strOutBig[i] <= 122) )
                                        {
                                            strOutBig[i] -=32;
                                        }
                                    }

                                    if ( strOutBig.find(inStr) != string::npos )  //结果中包含
                                    {
                                        theBoard.stolen(curTurn,theBoard.strToColour(inStr));
                                        break;
                                    }
                                    else
                                    {
                                         cout << "They can't be stolen from." << endl;
                                    }
                                }
                                else
                                {

                                    cout << "They can't be stolen from." << endl;
                                }
           
                            }//while
                        }
                        break;
                    }
                }//while
          }
          else //roll不等7,增加资源
          {
              theBoard.addResByDice(dice);
          }
          return true;
        };

    

    
        
        void help()
        {
            cout << "Valid commands:" << endl;
            cout << "~ board : prints the current board." << endl;
            cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
            cout << "~ residences : prints the residences the current builder has currently completed." << endl;
            cout << "~ build - road <road#> : attempts to builds the road at <road#>." << endl;
            cout << "~ build - res <housing#> : attempts to builds a basement at <housing#>." << endl;
            cout << "~ improve <housing#> : attempts to improve the residence at <housing#>." << endl;
            cout << "~ trade <colour> <give> <take> : attempts to trade with builder <colour>, giving one resource of type <give> and receiving one resource of type <take>." << endl;
            cout << "~ market <sell> <buy> : attempts to sell resources on the market, giving four resource of type <sell> and receiving one resource of type <buy>." << endl;
            cout << "~ next : passes control onto the next builder in the game." << endl;
            cout << "~ save <file> : saves the current game state to <file>." << endl;
            cout << "~ help : prints out the list of commands." << endl;
            cout  << endl;
        };
 


    bool duringTurn()
    {
        
            string strCommand="";
        
            while (true)
            {
                cout << "Enter a command:" << endl;
                
                strCommand = getACommand();
                
                if ( strCommand == "HELP")
                {
                    help();
                }
                else if ( strCommand == "BOARD")
                {
                        theBoard.show();
                }
                else if ( strCommand == "STATUS")
                {
                    cout << theBoard.showBuilderStatus(Blue) << endl;
                    cout << theBoard.showBuilderStatus(Red) << endl;
                    cout << theBoard.showBuilderStatus(Orange) << endl;
                    cout << theBoard.showBuilderStatus(Yellow) << endl;

                }
                else if ( strCommand == "RESIDENCES")
                {
                    theBoard.showBuilderResidnce(curTurn);
                }
                else if ( strCommand == "TRADE")
                {
                    Colour tradeColor;
                    Resource giveRes;
                    Resource takeRes ;
                    string strTake;
                    string strGive;
                    string strTrade;
                    string strTradeShow;

                    while (true)
                    {
                        strTradeShow = getACommand(false);

                        if ( strTradeShow == "B" ) strTradeShow ="Blue";
                        if ( strTradeShow == "R" ) strTradeShow = "Red";
                        if ( strTradeShow == "O" ) strTradeShow = "Orange";
                        if ( strTradeShow == "Y" ) strTradeShow = "Yellow";
                        if ( strTradeShow == "b" ) strTradeShow ="Blue";
                        if ( strTradeShow == "r" ) strTradeShow = "Red";
                        if ( strTradeShow == "o" ) strTradeShow = "Orange";
                        if ( strTradeShow == "y" ) strTradeShow = "Yellow";

                        strTrade = strTradeShow;

                        for (int i=0;i<strTrade.length();i++)
                        {
                            if ((strTrade[i] >= 97)&&(strTrade[i] <= 122) )
                            {
                                strTrade[i] -=32;
                            }

                        }

                        

                        tradeColor = theBoard.strToColour( strTrade );
                        if(  tradeColor == None )
                        {
                            cout << "Invalid colour." << endl;
                        }
                        else
                        {
                            break;
                        }
                    }

                    while (true)
                    {
                        strGive = getACommand();
                        giveRes =theBoard.strToResource( strGive );
                        if(  giveRes == PARK )
                        {
                            cout << "Invalid item." << endl;
                        }
                        else
                        {
                            break;
                        }
                    }
                     while (true)
                    {
                        strTake =  getACommand();
                        takeRes =theBoard.strToResource( strTake );
                        if(  takeRes == PARK )
                        {
                            cout << "Invalid item." << endl;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if ( tradeColor == curTurn) //
                    {
                        cout << "Can't trade with yourself." << endl;
                    }
                    else if (takeRes == giveRes)
                    {
                        cout << "Why are you trading for the same resource..." << endl;
                    }
                    else if ( theBoard.getBuilderResNum(curTurn,giveRes) < 1  )
                    {
                        cout << "You don't have enough " << strGive <<endl;
                    }
                    else if ( theBoard.getBuilderResNum(tradeColor,takeRes) < 1  )
                    {
                        cout << strTrade << " doesn't have enough " << strTake << "." <<endl;
                    }
                    else  //满足条件
                    {
                        cout << showcurTurn()<<" offers "<<strTradeShow<<" one "<<strGive<<" for one "<<strTake<<"." <<endl;
                        while (true)
                        {
                            
                            cout <<"Does " << strTradeShow<<" accept this offer?" << endl;
                            string strAnswer = getACommand();

                            if ( strAnswer == "YES" )
                            {

                                theBoard.trade(curTurn,tradeColor,giveRes,takeRes);

                                cout << showcurTurn() <<" gains one " << strTake <<" and loses one "<< strGive <<endl;
                                cout << ", " << strTradeShow <<" gains one "<< strGive <<" and loses one " << strTake <<endl;

                                break;
                            }
                            else if ( strAnswer == "NO" )
                            {
                                cout << strTradeShow << " declined the trade." <<endl;
                                break;
                            }
                            else
                            {
                                cout << "Invalid command." << endl;
                            }
                        }
                    }
                }//trade
                else if ( strCommand == "BUILD-ROAD")
                {
                    int roadID;
                    
                    while (true)
                    {
                        roadID = getAInt();
                        if (roadID < 0 )
                        {
                            cout << "ERROR:  isn't a valid integer." << endl;
                        }
                        else if ( roadID >= EDGENUM )
                        {
                            cout << "ERROR:  You cannot Build here. location does not exist." << endl;

                        }
                        else
                        {
                            break;
                        }

                    }

                    if ( theBoard.canBuildroad(curTurn,roadID) )  //这地方能修路
                    {

                        if (theBoard.enoughToBuildRoad(curTurn)) //资源够用
                        {
                            if (theBoard.BulidingRoad(curTurn, roadID)) //修路成功
                            {
                                theBoard.payForBuildRoad(curTurn); //修建成功后才能支付费用，要不然结果不对
                                cout << "Builder " << showcurTurn() << " successfully built a Road at " << to_string(roadID) <<endl;
                            }
                            else
                            {
                                cout << "You cannot build here."<< endl;
                            }
                                
                        }
                        else //资源不够用
                        {
                            cout << "You do not have enough resources." <<endl;
                            cout << endl;
                            cout << "The cost of a Road is one HEAT and one WIFI resource." <<endl;
                        }
                    }//if
                    else
                    {
                        cout << "You cannot build here."<< endl;
                    }
                }//build-road
                else if ( strCommand == "BUILD-RES")
                {
                    int resourceID;
                    
                    while (true)
                    {
                        resourceID = getAInt();
                        if ( resourceID < 0 )
                        {
                            cout << "ERROR:  isn't a valid integer." << endl;
                        }
                        else if ( resourceID >= VERTEXNUM )
                        {
                            cout << "ERROR:  You cannot Build here. location does not exist." << endl;

                        }
                        else
                        {
                            break;
                        }

                    }

                    if ( Nothing == theBoard.getVertexLevel(resourceID))
                    {
                        if ( theBoard.canBuildHouse(curTurn, resourceID ) ) //这地方能修房子
                        {
                            //资源判断
                            if (theBoard.enoughToBuildHouse(curTurn, resourceID ) )
                            {
                                //资源修改
                                if (theBoard.BuildingHouse(curTurn, resourceID)) //修建成功
                                {
                                    theBoard.payForBuildHouse(curTurn, resourceID) ;//修建成功后才能支付费用，要不然结果不对

                                    cout << "Builder "<< showcurTurn() << " successfully built a "<< theBoard.showVertexLevel(resourceID) <<" at "<< to_string(resourceID)<<"." << endl;
                                }
                                else
                                {
                                    cout << "You cannot build here."<< endl;
                                }
                                        
                            }
                            else //资源不够
                            {
                                cout << endl;
                                cout << "You do not have enough resources." <<endl;
                                cout << endl;
                                cout << "The cost of a Basement is one BRICK, one ENERGY, one GLASS, and one WIFI resource." <<endl;
                            }
                        }
                        else
                        {
                            cout << "You cannot build here."<< endl;
                        }
                    }
                    else //此地已经有建筑
                    {
                        cout << endl;
                        cout << "You cannot build here."<< endl;
                    }
                }//build-res
                else if ( strCommand == "IMPROVE")
                {
                    int resourceID;
                    
                    while (true)
                    {
                        resourceID = getAInt();
                        if ( resourceID < 0 )
                        {
                            cout << "ERROR:  isn't a valid integer." << endl;
                        }
                        else if  ( resourceID >= VERTEXNUM ) 
                        {
                            cout << "ERROR:  You cannot improve here. location does not exist." << endl;

                        }
                        else
                        {
                            break;
                        }

                    }
                    
                    if ( Nothing != theBoard.getVertexLevel(resourceID))
                    {
                        if ( theBoard.canBuildHouse(curTurn, resourceID ) )  //这地方能盖房子
                        {
                             //资源判断
                            if (theBoard.enoughToBuildHouse(curTurn, resourceID ) )
                            {
                                if (theBoard.BuildingHouse(curTurn, resourceID)) //修建成功
                                {
                                    theBoard.payForBuildHouse(curTurn, resourceID) ;//支付费用
    
                                    cout << "Builder "<< showcurTurn() << " successfully built a "<< theBoard.showVertexLevel(resourceID) <<" at "<< to_string(resourceID)<<"." << endl;
                                }
                                else
                                {
                                    cout << "You can't improve that building."<< endl;
                                }
                                        
                            }
                            else //资源不够
                            {
                                cout << "You do not have enough resources." <<endl;
                                cout << endl;
                                cout << "The cost to improve a Basement to a House is two GLASS and three HEAT resource." <<endl;
                                cout << "The cost to improve a House to a Tower is three BRICK, two ENERGY, two GLASS, one WIFI, and two HEAT." <<endl;
                            }
                        }
                        else
                        {
                            cout << "You can't improve that building."<< endl;
                        }
                    }
                    else //此地没有建筑无法升级
                    {
                        cout << "Invalid residence."<< endl;
                    }
                }//improve
                else if ( strCommand == "SAVE")
                {
                    string strFileName;

                    strFileName =  getACommand(false);
                    cout << "Saving to " << strFileName <<endl;
                   if ( !saveToFile(strFileName) )
                   {
                       cout << "Error: save file error" << endl;
                   }

                }//save
                else if ( strCommand == "NEXT")
                {                              
                    break;

                }
                else
                {
                    cout << "Invalid command." << endl;
                    cout << "Please enter 'help' for a list of valid commands." << endl;
                }

            } //while
        
            return true;
    };

    
   
    bool gameIsOver()
    {
        return (theBoard.getBuilderResPoint(curTurn) >=10 );
    };
    
    bool playagain()
    {
        string strAnswer;
        bool bPlay;

        cout << "Congratulations!! " << showcurTurn() << " wins!!" << endl;
                        
        while (true)
        {
            cout << "Would you like to play again?" <<endl;

            strAnswer = getACommand();
            if ( strAnswer == "YES" )
            {
                bPlay = true;
                break;
            }
            else if ( strAnswer == "NO" )
            {
                bPlay = false;
                break;
            }
            else
            {
                cout << "Invalid command." << endl;
            }
        }
        
        return bPlay;
    };


};






int main(int argc, const char * argv[]) {


    int seed = (unsigned int) time(NULL);  //默认用当前时间作为随机数种子
    string fileName="";
    string boardfile="layout.txt";
    Builder loadBuilder[4]; 
    
    bool bRandom = false;


    for(int i=1;i<argc;i++)  
    {
        string strInput ="";
        string strTmp ="";
          
        strInput = argv[i];
        strTmp = argv[i-1];

        if (strInput == "-seed")
        {
            if (! strToInt(&seed,argv[i+1]))  //如果有正确的参数重新设置种子
            {
                cout << "ERROR: -seed missing seed argument" << endl;
                return 1;
            }
        }else if (strInput == "-random-board")
        {
            bRandom = true;
            boardfile = ""; 

        }else if (strInput == "-load")
        {
            if (bRandom)
            {
                cout <<"ERROR: already specified -random, can't also specify -load"<<endl;
                return 2;
            } 
            if (boardfile != "layout.txt")
            {
                cout <<"ERROR: already specified -board, can't also specify -load"<<endl;
                return 3;
            }
            fileName = argv[i+1]; //取save文件名
        }else if (strInput == "-board")
        {
            if (bRandom)
            {
                cout <<"ERROR: already specified -random, can't also specify -board"<<endl;
                return 4;
            }
            if (fileName.length() > 0)
            {
                cout <<"ERROR: already specified -load, can't also specify -board"<<endl;
                return 5;
            }
            boardfile = argv[i+1]; //取board文件名
        }else if ( (strTmp != "-board") && (strTmp != "-load") &&  (strTmp != "-seed") )
        {
            cout <<"ERROR: unrecognized argument "<<strInput<<endl;
            return 6;
        }
    }//for

    srand(seed);  //设置随机数种子

   
    while (true)
    {
        game mygame;
        
        //判断文件存在
        if ( fileName.length()>0 ) //从文件load
        {
            if (!mygame.loadFormFile(fileName))
            {
                cout << "ERROR: Unable to open file "<< fileName <<" for board layout." <<endl;
                return 8;
            }
        }
        else 
        {
            if (boardfile.length() > 0) //从文件初始化board
            {
                if (!mygame.boardFormFile(boardfile))  
                {
                    if ( boardfile == "layout.txt" )
                    {
                        cout << "ERROR: Unable to open file "<< boardfile <<" for default board layout." <<endl;
                    }
                    else
                    {
                        cout << "ERROR: Unable to open file "<< boardfile <<" for board layout." <<endl;
                    }
                  return 8;
                } 
            }
            mygame.init();  //随机开局需要前两轮
        }

        while (true)
        {
            mygame.beginTurn();      //前半回合掷骰子获得资源
            mygame.duringTurn();   //后半回合各种建造命令
             
            if (mygame.gameIsOver())  //判断是否gameover
            {
                if (mygame.playagain()) //再来一局
                {
                    fileName = ""; //把load文件名置空,随机新开局
                    break;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                mygame.turn(); //下一人
            }
            
        }//while

    }//while

    return 0;
    
}

