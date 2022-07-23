#pragma once

#include "global.h"
#include "sprite.h"
#include "dropPattern.h"

namespace ppvs
{

class game;

class characterSelect
{
    public:
        characterSelect(game*);
        virtual ~characterSelect();

        void draw();
        void prepare();
        void play();
        void end();

        void setCharacter(int playernum,int selection,bool choice);

    private:
        void setDropset(int x,int y,int pl);
        int findCurrentCharacter(int i);

        int timer;
        game* currentgame;
        gameData *data;
        puyoCharacter order[40];
        sprite background;
        sprite holder[40];
        sprite charSprite[40];
        fimage *backgroundImage;

        //player stuff
        sprite *selectSprite;
        sprite *selectedCharacter;
        sprite *dropset;
        sprite *name;
        sprite *nameHolder;
        sprite *nameHolderNumber;
        sprite *playernumber;
        int *sel;
        bool *madeChoice;
        float scale;
        int Nplayers;
        bool firstStart;
};
}
