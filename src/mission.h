#ifndef __MISSION_H__
#define __MISSION_H__

class Mission {
    public:
    int MaxBodyLength;
    int NumberGrowthItem;
    int NumberPoisonItem;
    int NumberGate;
    bool isPassBody;
    bool isPassGrowth;
    bool isPassPoison;
    bool isPassGate;
    Mission() {
        MaxBodyLength = 0;
        NumberGrowthItem = 0;
        NumberPoisonItem =0;
        NumberGate = 0;
        isPassBody = false;
        isPassGrowth = false;
        isPassPoison = false;
        isPassGate = false;
    }
    bool isClear() {
        if(isPassBody && isPassGrowth && isPassPoison && isPassGate) {
            return true;
        }
        return false;
    }
};
#endif