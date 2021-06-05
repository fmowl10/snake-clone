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
 * @brief mission class
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


    /**
     * @brief Construct a new Mission object
     * 
     * @param bodyLength 
     * @param grothItem 
     * @param poisonItem 
     * @param gate 
     */
    Mission(int bodyLength = 0, int grothItem = 0, int poisonItem = 0, int gate = 0) : MaxBodyLength(bodyLength), NumberGrowthItem(grothItem), NumberPoisonItem(poisonItem), NumberGate(gate)
    {
        currentMax = 0;
        consumedGrowthItem = 0;
        consumedPoisonItem = 0;
        GatePassed = 0;
    }

    /**
     * @brief returns is consumeGroth greater than NUmberGrothItem
     * 
     * @return true 
     * @return false 
     */
    bool isPassGrowthItem()
    {
        return consumedGrowthItem >= NumberGrowthItem;
    }
    
    /**
     * @brief returns is consumedPoisonItem greater than NumberPoisonItem
     * 
     * @return true 
     * @return false 
     */
    bool isPassPoisonItem() 
    {
        return consumedPoisonItem >= NumberPoisonItem;
    }

    /**
     * @brief returns is GatePassed greater than NumberGate
     * 
     * @return true 
     * @return false 
     */
    bool isPassNumberGate() 
    {
        return GatePassed >= NumberGate;
    }
    
    /**
     * @brief returns is currentMax greater than MaxBodyLength
     * 
     * @return true 
     * @return false 
     */
    bool isPassMaxBodyLength() 
    {
        return currentMax >= MaxBodyLength;
    }

    /**
     * @brief return clear condition
     * 
     * @return true 
     * @return false 
     */
    bool isClear()
    {
        if(isPassGrowthItem() && isPassMaxBodyLength() && isPassPoisonItem() && isPassNumberGate()) 
        {
            return true;
        }    
        return false;
    }

    /**
     * @brief Get the Max Body Length object
     * 
     * @return int 
     */
    int getMaxBodyLength() 
    {
        return MaxBodyLength;
    }

    /**
     * @brief Get the Number Groth Item object
     * 
     * @return int 
     */
    int getNumberGrothItem() 
    {
        return NumberGrowthItem;
    }

    /**
     * @brief Get the Number Poison Item object
     * 
     * @return int 
     */
    int getNumberPoisonItem() 
    {
        return NumberPoisonItem;
    }
    
    /**
     * @brief Get the Number Gate object
     * 
     * @return int 
     */
    int getNumberGate() 
    {
        return NumberGate;
    }
};
#endif