using namespace std;
#include <string>
#include <iostream>
#include "builder.h"
#include "game.h"

int main(int argc, const char * argv[]) {

    bool bEOF = false;
    int seed = (unsigned int) time(NULL);  // seed is current time
    string fileName = "";
    string boardfile = "layout.txt";
    bool bRandom = false;
    for(int i = 1; i < argc; i++){
        string strInput = "";
        string strTmp = "";
        strInput = argv[i];
        strTmp = argv[i-1];
        if (strInput == "-seed"){
            if (!strToInt(&seed,argv[i+1])){  // reset seed if there is a valid input
                cout << "ERROR: -seed missing seed argument" << endl;
                return 1;
            }
        }else if (strInput == "-random-board"){
            bRandom = true;
            boardfile = ""; 
        }else if (strInput == "-load"){
            if (bRandom){
                cout <<"ERROR: already specified -random, can't also specify -load"<<endl;
                return 2;
            } 
            if (boardfile != "layout.txt"){
                cout <<"ERROR: already specified -board, can't also specify -load"<<endl;
                return 3;
            }
            fileName = argv[i+1]; // read the name of the file 
        }else if (strInput == "-board"){
            if (bRandom){
                cout <<"ERROR: already specified -random, can't also specify -board"<<endl;
                return 4;
            }
            if (fileName.length() > 0){
                cout <<"ERROR: already specified -load, can't also specify -board"<<endl;
                return 5;
            }
            boardfile = argv[i+1]; // read the name of the board
        }else if ((strTmp != "-board") && (strTmp != "-load") &&  (strTmp != "-seed")){
            cout <<"ERROR: unrecognized argument "<<strInput<<endl;
            return 6;
        }
    }
    srand(seed);  // set a random seed

    while (true){
        Game mygame;
        if (fileName.length() > 0){ 
            if (!mygame.loadFormFile(fileName)){ // check loadfile
                cout << "ERROR: Unable to open file "<< fileName <<" for board layout." <<endl;
                return 8;
            }
        }else{
            if (boardfile.length() > 0){ // initialize the board from the boardfile
                if (!mygame.boardFormFile(boardfile)){
                    if ( boardfile == "layout.txt"){
                        cout << "ERROR: Unable to open file "<< boardfile <<" for default board layout." <<endl;
                    }else{
                        cout << "ERROR: Unable to open file "<< boardfile <<" for board layout." <<endl;
                    }
                return 8;
                } 
            }
            mygame.init(&bEOF);  // first two turn 
            if (bEOF){
                return (0);
            } 
        }

        while (true){
            mygame.beginTurn(&bEOF);      // rolling dice and gain resource
            if (bEOF){
                mygame.saveToFile("backup.sv");
                return (0);
            } 
            mygame.duringTurn(&bEOF);   // construction and other commands
            if (bEOF){
                mygame.saveToFile("backup.sv");
                return (0);
            } 
             
            if (mygame.gameIsOver()){ // check whether the game is over
                if (mygame.playagain(&bEOF)){ // check whether play again
                    if (bEOF){
                        mygame.saveToFile("backup.sv");
                        return (0);
                    } 
                    fileName = ""; // start another game and make the filename blank
                    break;
                }else{
                    if (bEOF){
                        mygame.saveToFile("backup.sv");
                        return (0);
                    } 
                    return 0;
                }
            }else{
                mygame.turn(); // next player
            }
        }
    }
    return 0;
}


