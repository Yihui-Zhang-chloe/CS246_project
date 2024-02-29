#include "game.h"
#include <sstream>
#include <fstream>
using namespace std;

void Game::getInputLine(bool * bEOF){
    if (cin.eof()){ 
       * bEOF = true;
       cout <<"End of file reached."<<endl;
       return;
    }
    string input = "";
    getline(cin,input);
    stringstream inPutSS(input);
    string strTMP;
    while(inPutSS>>strTMP){
        strList.push_back(strTMP);
    }
}

Game::Game(){
    curTurn = Blue;
}

void Game::turn(bool b){
    if (b){
        switch (curTurn){
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
    }else {
        switch (curTurn){
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
}

string Game::showcurTurn() noexcept { return theBoard.showColor(curTurn); }

bool Game::boardFormFile(string fileName){
    string line="";
    string buf="";
    stringstream ss;
    ifstream  loadfile( fileName,ios::in);
    if (! loadfile){
        return false;
    }else {
        getline(loadfile, line);
        ss.clear();
        ss.str(line);
        int j = 0;

        while (ss >> buf){
            int id = stoi(buf);
            ss >> buf;
            int rNum = stoi(buf);
            Resource rID;
            switch(id){
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
    return true;
}

bool Game::saveToFile(string fileName){

    ofstream outfile;
    outfile.open(fileName);
    if(! outfile){
        outfile << "Open File Fail:" << fileName << endl;
        return false;
    }
    outfile << to_string(int(curTurn)-1) << endl;
    for (int i = 0; i < 4; i++){
        outfile << theBoard.getBuilderSaveDate(Colour (i+1)) << endl;
    }
    outfile << theBoard.getTileSaveData() << endl;  //board
    if (((theBoard.getGeese()) >= 0) && (theBoard.getGeese() <TILENUM)){
        outfile << to_string(theBoard.getGeese()) << endl;
    }
    outfile.close();
    return true;
}

bool Game::loadFormFile(string fileName){
    string line="";
    string buf="";
    bool b_h = false; // load from save
    ifstream  loadfile(fileName,ios::in);
    if ( !loadfile){
        return false;
    }else {
        stringstream ss;
        for (int i = 0; i < 7; i++){
            getline(loadfile, line);
            switch(i){
                case 0:
                    curTurn = Colour( stoi (line)+1);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:{
                    RES res={0};
                    ss.clear();
                    ss.str(line);
                    int j = 0;
                    b_h = false;
                    while (ss >> buf){
                        switch(j){
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
                            default:{
                                if (buf == "r") break;
                                if (buf == "h") {
                                    b_h = true;
                                    break;
                                }
                                if (! b_h){
                                    theBoard.setRoads(stoi(buf),Colour(i));
                                }else {
                                    int id = stoi(buf);
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
                        }
                        j++;
                    }
                }
                break;
                case 5:{
                    ss.clear();
                    ss.str(line);
                    int j = 0;
                    while (ss >> buf){
                        int id = stoi(buf);
                        ss >> buf;
                        int rNum = stoi(buf);
                        Resource rID;
                        switch(id){
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
                case 6:{
                    ss.clear();
                    ss.str(line);
                    ss >> buf;
                    if (buf.length() > 0){
                        theBoard.initGeese(stoi(buf));
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
}

bool Game::init(bool * bEOF){
    int VertexID;
    theBoard.show();
    for (int i = 1; i <= 8; i++){
        cout<<"Builder "<< this->showcurTurn()<<", where do you want to build a basement?"<<endl;
        while (true){
            VertexID = getAInt(bEOF);
            if (*bEOF) return false;
            if (VertexID== -1){
                cout<< "ERROR: not an integer" <<endl;
                cout << "Builder " << this->showcurTurn() <<" where do you want to build a basement?" <<endl;
            }else if ((VertexID >= 0)&&(VertexID < VERTEXNUM)){
                if (theBoard.BuildingHouse(curTurn, VertexID,true)){
                    break;
                }else {
                    cout<< "You cannot build here." <<endl;
                    cout << "Basements already exist as locations:" << theBoard.showExistHouse() <<endl;
                    cout << "Builder " << this->showcurTurn() <<" where do you want to build a basement?" <<endl;
                }
            }else {
                cout<< "You cannot build here." <<endl;
                cout<< "location does not exist." <<endl;
                cout << "Builder " << this->showcurTurn() <<" where do you want to build a basement?" <<endl;
            }
        }
        if (i % 4 != 0){
            this->turn(i < 5);
        }
    }
    return true;
}

int Game::roll(bool * bEOF){
    int dice = 0;
    while (true){
        string input = "";
        input = getACommand(bEOF);
        if (*bEOF) return false;
        if (input == "LOAD"){
            cout<< "Builder " << this->showcurTurn() << " now has loaded Dice"<<endl;
            theBoard.setLoaded(curTurn, true);
        }else if (input == "FAIR"){
            cout<< "Builder " << this->showcurTurn() << " now has fair Dice."<<endl;
            theBoard.setLoaded(curTurn, false);
        }else if (input == "ROLL"){
            if (theBoard.getLoaded(curTurn)){
                cout << "Input a roll between 2 and 12:" << endl;
                while (true){
                    int in = 0;
                    cout<< ">";
                    in = getAInt(bEOF);
                    if (*bEOF) return false;
                    if (in == -1){
                        cout << "ERROR:  isn't a valid integer." << endl;
                    }else if ((in < 2) || (in > 12)){
                        cout << "Invalid roll " << to_string(in) << endl;
                        cout << "Input a roll between 2 and 12:" << endl;
                    }else {
                        dice = in;
                        break;
                    }
                }
            }else {
                dice = (rand()% 6 + 1) + (rand()% 6 + 1);
                cout << "The number you rolled is " << to_string(dice) << endl;
            }
            break;
        }else if (input == "STATUS"){
            cout << theBoard.showBuilderStatus(Blue) << endl;
            cout << theBoard.showBuilderStatus(Red) << endl;
            cout << theBoard.showBuilderStatus(Orange) << endl;
            cout << theBoard.showBuilderStatus(Yellow) << endl;
        }else if (input == "HELP"){
            cout << "Valid commands:" <<endl;
            cout << "~ load : changes current builder's dice type to 'loaded'" << endl;
            cout << "~ fair : changes current builder's dice type to 'fair'" << endl;
            cout << "~ roll : rolls the dice and distributes resources." << endl;
            cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
            cout << "~ help : prints out the list of commands." << endl;
        }else {
            cout << "Invalid command." << endl;
            cout << "Please enter 'help' for a list of valid commands." << endl;
        }
    }
    return dice;
}

string Game::getACommand(bool * bEOF,bool big) noexcept{
    string str="";
    cout << ">";
    while (true){
        if (!strList.empty()){
            str = strList.front();
            strList.erase(strList.begin());
            if (big){
                for (int i = 0; i < str.length(); i++){
                    if ((str[i] >= 97)&&(str[i] <= 122)){
                        str[i] -= 32;
                    }
                }
            }
            return str;
        }else {
            getInputLine(bEOF);
            if(*bEOF) return "";
        }
    }
    return "";
}

int Game::getAInt(bool * bEOF) noexcept {
    string str="";
    cout << ">";
    while (true){
        if (!strList.empty()){
            str = strList.front();
            strList.erase(strList.begin());
            break;
        }else {
            getInputLine(bEOF);
            if(*bEOF) return -1;
        }
    }
    for (int i = 0; i < str.length(); i++){
        if (str[i] < 48 || str[i] > 57){
            return -1;
        }
    }
    return stoi(str);
}

bool Game::beginTurn(bool * bEOF){
    int input = 0;
    theBoard.show();
    cout<<"Builder "<<this->showcurTurn()<<"’s turn."<<endl;
    int dice = this->roll(bEOF);
    if (*bEOF) return false;
    if (dice == 7){
        theBoard.dropResToGeese(); // lose resources
        cout<<"Choose where to place the GEESE."<<endl;
        while (true){
            string strOut = "";
            input = getAInt(bEOF);
            if (*bEOF) return false;
            if ((input < 0) ||(input > TILENUM) || (input == theBoard.getGeese())){
                cout << "Geese can't move here." << endl;
                cout << "Choose where to place the GEESE." << endl;
            }else {
                strOut = theBoard.setGeese(curTurn,input);
                if (strOut.length() == 0){
                    cout << "Builder " << showcurTurn()<< " has no builders to steal from."<<endl;
                }else { // have someone to steal
                    cout<< "Builder " << showcurTurn()<< " can choose to steal from " <<strOut<<endl;
                    while (true){
                        cout << "Choose a builder to steal from." << endl;
                        string inStr = getACommand(bEOF);
                        if (*bEOF) return false;
                        if (inStr == "B") inStr ="BLUE";
                        if (inStr == "R") inStr = "RED";
                        if (inStr == "O") inStr = "ORANGE";
                        if (inStr == "Y") inStr = "YELLOW";
                        if ((inStr == "BLUE") || (inStr == "RED") || (inStr == "ORANGE") || (inStr == "YELLOW")){
                            string strOutBig = strOut;
                            for (int i = 0;i < strOutBig.length(); i++){
                                if ((strOutBig[i] >= 97)&&(strOutBig[i] <= 122)){
                                    strOutBig[i] -=32;
                                }
                            }
                            if (strOutBig.find(inStr) != string::npos){
                                theBoard.stolen(curTurn,theBoard.strToColour(inStr));
                                break;
                            }else {
                                cout << "They can't be stolen from." << endl;
                            }
                        }else {
                            cout << "They can't be stolen from." << endl;
                        }
                    }
                }
                break;
            }
        }
    }else { // rolling a non-seven
        theBoard.addResByDice(dice);
    }
    return true;
}

void Game::help() noexcept{
    cout << "Valid commands:" << endl;
    cout << "~ board : prints the current board." << endl;
    cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
    cout << "~ residences : prints the residences the current builder has currently completed." << endl;
    cout << "~ build-road <road#> : attempts to builds the road at <road#>." << endl;
    cout << "~ build-res <housing#> : attempts to builds a basement at <housing#>." << endl;
    cout << "~ improve <housing#> : attempts to improve the residence at <housing#>." << endl;
    cout << "~ trade <colour> <give> <take> : attempts to trade with builder <colour>, giving one resource of type <give> and receiving one resource of type <take>." << endl;
    cout << "~ next : passes control onto the next builder in the game." << endl;
    cout << "~ save <file> : saves the current game state to <file>." << endl;
    cout << "~ help : prints out the list of commands." << endl;
    cout  << endl;
}

bool Game::duringTurn(bool * bEOF){
    string strCommand = "";
    while (true){
        cout << "Enter a command:" << endl;
        strCommand = getACommand(bEOF);
        if (*bEOF) return false;
        if (strCommand == "HELP"){
            help();
        }else if (strCommand == "BOARD"){
            theBoard.show();
        }else if (strCommand == "STATUS"){
            cout << theBoard.showBuilderStatus(Blue) << endl;
            cout << theBoard.showBuilderStatus(Red) << endl;
            cout << theBoard.showBuilderStatus(Orange) << endl;
            cout << theBoard.showBuilderStatus(Yellow) << endl;
        }else if (strCommand == "RESIDENCES"){
            theBoard.showBuilderResidnce(curTurn);
        }else if (strCommand == "TRADE"){
            Colour tradeColor;
            Resource giveRes;
            Resource takeRes ;
            string strTake;
            string strGive;
            string strTrade;
            string strTradeShow;
            while (true){
                strTradeShow = getACommand(bEOF,false);
                if (*bEOF) return false;
                if (strTradeShow == "B") strTradeShow ="Blue";
                if (strTradeShow == "R") strTradeShow = "Red";
                if (strTradeShow == "O") strTradeShow = "Orange";
                if (strTradeShow == "Y") strTradeShow = "Yellow";
                if (strTradeShow == "b") strTradeShow ="Blue";
                if (strTradeShow == "r") strTradeShow = "Red";
                if (strTradeShow == "o") strTradeShow = "Orange";
                if (strTradeShow == "y") strTradeShow = "Yellow";
                strTrade = strTradeShow;
                for (int i = 0; i < strTrade.length(); i++){
                    if ((strTrade[i] >= 97) && (strTrade[i] <= 122)){
                        strTrade[i] -= 32;
                    }
                }
                tradeColor = theBoard.strToColour( strTrade );
                if (tradeColor == None){
                    cout << "Invalid colour." << endl;
                }else {
                    break;
                }
            }
            while (true){
                strGive = getACommand(bEOF);
                if (*bEOF) return false;
                giveRes = theBoard.strToResource( strGive );
                if( giveRes == PARK){
                    cout << "Invalid item." << endl;
                }else {
                    break;
                }
            }
            while (true){
                strTake = getACommand(bEOF);
                if (*bEOF) return false;
                takeRes = theBoard.strToResource( strTake );
                if( takeRes == PARK){
                    cout << "Invalid item." << endl;
                }else {
                    break;
                }
            }
            if (tradeColor == curTurn){
                cout << "Can't trade with yourself." << endl;
            }else if (takeRes == giveRes){
                cout << "Why are you trading for the same resource..." << endl;
            }else if (theBoard.getBuilderResNum(curTurn,giveRes) < 1){
                cout << "You don't have enough " << strGive <<endl;
            }else if (theBoard.getBuilderResNum(tradeColor,takeRes) < 1){
                cout << strTrade << " doesn't have enough " << strTake << "." <<endl;
            }else { // can trade
                cout << showcurTurn()<<" offers "<<strTradeShow<<" one "<<strGive<<" for one "<<strTake<<"." <<endl;
                while (true){
                    cout <<"Does " << strTradeShow<<" accept this offer?" << endl;
                    string strAnswer = getACommand(bEOF);
                    if (*bEOF) return false;
                    if (strAnswer == "YES"){
                        theBoard.trade(curTurn,tradeColor,giveRes,takeRes);
                        cout << showcurTurn() <<" gains one " << strTake <<" and loses one "<< strGive <<endl;
                        cout << ", " << strTradeShow <<" gains one "<< strGive <<" and loses one " << strTake <<endl;
                        break;
                    }else if (strAnswer == "NO"){
                        cout << strTradeShow << " declined the trade." <<endl;
                        break;
                    }else {
                        cout << "Invalid command." << endl;
                    }
                }
            }
        }else if (strCommand == "BUILD-ROAD"){
            int roadID;
            while (true){
                roadID = getAInt(bEOF);
                if (*bEOF) return false;
                if (roadID < 0){
                    cout << "ERROR:  isn't a valid integer." << endl;
                }else if (roadID >= EDGENUM){
                    cout << "ERROR:  You cannot Build here. location does not exist." << endl;
                }else {
                    break;
                }
            }
            if (theBoard.canBuildroad(curTurn,roadID)){ // can build road 
                if (theBoard.enoughToBuildRoad(curTurn)){ // enough resource
                    if (theBoard.BulidingRoad(curTurn,roadID)){ // build successfully
                        theBoard.payForBuildRoad(curTurn); // update the resource
                        cout << "Builder " << showcurTurn() << " successfully built a Road at " << to_string(roadID) <<endl;
                    }else {
                        cout << "You cannot build here."<< endl;
                    }
                }else { // resources are not enough
                    cout << "You do not have enough resources." <<endl;
                    cout << endl;
                    cout << "The cost of a Road is one HEAT and one WIFI resource." <<endl;
                }
            }else {
                cout << "You cannot build here."<< endl;
            }
        }else if (strCommand == "BUILD-RES"){
            int resourceID;
            while (true){
                resourceID = getAInt(bEOF);
                if (*bEOF) return false;
                if (resourceID < 0){
                    cout << "ERROR:  isn't a valid integer." << endl;
                }else if (resourceID >= VERTEXNUM){
                    cout << "ERROR:  You cannot Build here. location does not exist." << endl;
                }else {
                    break;
                }
            }
            if (Nothing == theBoard.getVertexLevel(resourceID)){
                if (theBoard.canBuildHouse(curTurn,resourceID)){ // can build residences
                    if (theBoard.enoughToBuildHouse(curTurn, resourceID)){ // enough resources
                        if (theBoard.BuildingHouse(curTurn, resourceID)){ // build successfully
                            theBoard.payForBuildHouse(curTurn, resourceID); // update resources
                            cout << "Builder "<< showcurTurn() << " successfully built a "<< theBoard.showVertexLevel(resourceID) <<" at "<< to_string(resourceID)<<"." << endl;
                        }else {
                            cout << "You cannot build here."<< endl;
                        }
                    }else { // resources are not enough
                        cout << endl;
                        cout << "You do not have enough resources." <<endl;
                        cout << endl;
                        cout << "The cost of a Basement is one BRICK, one ENERGY, one GLASS, and one WIFI resource." <<endl;
                    }
                }else {
                    cout << "You cannot build here."<< endl;
                }
            }else {
                cout << "You cannot build here."<< endl;
            }
        }else if (strCommand == "IMPROVE"){
            int resourceID;
            while (true){
                resourceID = getAInt(bEOF);
                if (*bEOF) return false;
                if (resourceID < 0){
                    cout << "ERROR:  isn't a valid integer." << endl;
                }else if (resourceID >= VERTEXNUM){
                    cout << "ERROR:  You cannot improve here. location does not exist." << endl;
                }else {
                    break;
                }
            }
            if (Nothing != theBoard.getVertexLevel(resourceID)){
                if ( theBoard.canBuildHouse(curTurn,resourceID)){  // can build residences
                    if (theBoard.enoughToBuildHouse(curTurn,resourceID)){ // enough resources
                        if (theBoard.BuildingHouse(curTurn, resourceID)){ // build successfully 
                            theBoard.payForBuildHouse(curTurn, resourceID) ;// updates resources
                            cout << "Builder "<< showcurTurn() << " successfully built a "<< theBoard.showVertexLevel(resourceID) <<" at "<< to_string(resourceID)<<"." << endl;
                        }else{
                            cout << "You can't improve that building."<< endl;
                        }
                    }else { //resources are not enough
                        cout << "You do not have enough resources." <<endl;
                        cout << endl;
                        cout << "The cost to improve a Basement to a House is two GLASS and three HEAT resource." <<endl;
                        cout << "The cost to improve a House to a Tower is three BRICK, two ENERGY, two GLASS, one WIFI, and two HEAT." <<endl;
                    }
                }else{
                    cout << "You can't improve that building."<< endl;
                }
            }else {
                cout << "Invalid residence."<< endl;
            }
        }else if (strCommand == "SAVE"){
            string strFileName;
            strFileName =  getACommand(bEOF,false);
            if (*bEOF) return false;
            cout << "Saving to " << strFileName <<endl;
            if (!saveToFile(strFileName)){
                cout << "Error: save file error" << endl;
            }
        }else if (strCommand == "NEXT"){
            break;
        }else {
            cout << "Invalid command." << endl;
            cout << "Please enter 'help' for a list of valid commands." << endl;
        }
    }
    return true;
}

bool Game::gameIsOver() noexcept{
    return theBoard.getBuilderResPoint(curTurn) >= 10;
}

bool Game::playagain(bool * bEOF){
    string strAnswer;
    bool bPlay;
    cout << "Congratulations!! " << showcurTurn() << " wins!!" << endl;
    while(true){
        cout << "Would you like to play again?" <<endl;
        strAnswer = getACommand(bEOF);
        if (*bEOF) return false;
        if (strAnswer == "YES"){
            bPlay = true;
            break;
        }else if (strAnswer == "NO"){
            bPlay = false;
            break;
        }else{
            cout << "Invalid command." << endl;
            break;
        }
    }
    return bPlay;
}

