/**
 * @file mission.h
 * @author kim jea ha kim jin seok
 * @brief 
 * @version 0.1
 * @date 2021-06-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __MISSION_H__
#define __MISSION_H__

/**
 * @brief 
 * 
 */
class Mission 
{
    int MaxBodyLength;
    int NumberGrowthItem;
    int NumberPoisonItem;
    int NumberGate;


    public:
    int currentMax;
    int consumedGrowthItem;
    int consumedPoisonItem;
    int GatePassed;
    bool isMaxBodyLength;



    Mission(int bodyLength = 0, int grothItem = 0, int poisonItem = 0, int gate = 0) : MaxBodyLength(bodyLength), NumberGrowthItem(grothItem), NumberPoisonItem(poisonItem), NumberGate(gate)
    {
        currentMax = 0;
        consumedGrowthItem = 0;
        consumedPoisonItem = 0;
        GatePassed = 0;
    }

    bool isPassGrowthItem()
    {
        return consumedGrowthItem >= NumberGrowthItem;
    }
    
    bool isPassPoisonItem() 
    {
        return consumedPoisonItem >= NumberPoisonItem;
    }
    
    bool isPassNumberGate() 
    {
        return GatePassed >= NumberGate;
    }

    bool isPassMaxBodyLength() 
    {
        return currentMax >= MaxBodyLength;
    }

    bool isClear()
    {
        if(isPassGrowthItem() && isPassMaxBodyLength() && isPassPoisonItem() && isPassNumberGate()) 
        {
            return true;
        }    
        return false;
    }

    int getMaxBodyLenght() 
    {
        return MaxBodyLength;
    }

    int getNumberGrothItem() 
    {
        return NumberGrowthItem;
    }

    int getNumberPoisonItem() 
    {
        return NumberPoisonItem;
    }
    
    int getNumberGate() 
    {
        return NumberGate;
    }



    

};
#endif