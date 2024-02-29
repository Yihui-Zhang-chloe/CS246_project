#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "board.h"

class Game{
    Board theBoard;
    Colour curTurn;
    vector<string> strList;
    void getInputLine(bool *bEOF);
    public:
    Game();
    void help() noexcept;
    void turn(bool b=true);
    string showcurTurn() noexcept;
    bool boardFormFile(string fileName);
    bool saveToFile(string fileName);
    bool loadFormFile(string fileName);
    bool init(bool * bEOF);
    int roll(bool *bEOF);
    string getACommand(bool*bEOF,bool big = true) noexcept;
    int getAInt(bool *bEOF) noexcept;
    bool beginTurn(bool*bEOF);
    bool duringTurn(bool * bEOF);
    bool gameIsOver() noexcept;
    bool playagain(bool* bEOF);
};
#endif

