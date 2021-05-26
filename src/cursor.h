#include<iostream>
#include<ncurses.h>
#include<vector>
#include<Point.h>
#include<Direct.h>

using namespace std;
// 만들어 둬야 할것들 : 포인트 , direct


class Snake{
    private:
        int bodyLength;
        vector<Point> body;
        Direct direct;
        bool isColide = false;
    
    public:
     Snake(){ bodyLength = 3; body.push_back(Point(0,0)); }

        void move(){

            Point temp = body.front();

            if(direct ==  N){ temp.y++; }
            else if(direct ==  S){ temp.y--; }
            else if(direct ==  W){ temp.x++; }
            else if(direct ==  E){ temp.x--; }

            for(int i = 1; i < bodyLength; i++){
                body.at(i) = body.at(i-1);
                if(body.at(i) == temp){ isColide = true; return ; }
            }

            body.front() = temp;

        }

        void changeDirect(Direct d){ direct = d; }
        bool isDead(){ return isColide; }

    friend class Board;
};