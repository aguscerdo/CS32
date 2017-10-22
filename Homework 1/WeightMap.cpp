//
//  WeightMap.cpp
//  HW1
//
//  Created by Agustin Marinovic on 1/16/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include "WeightMap.h"
#include <iostream>

WeightMap::WeightMap()
{
    
}

bool WeightMap::enroll(std::string name, double startWeight)
{
    if (startWeight < 0)
        return false;
    return m_map.insert(name, startWeight);
    
}

double WeightMap::weight(std::string name) const
{
    ValueType a;
    if (m_map.get(name, a))
        return a;
    else
        return -1;
}

bool WeightMap::adjustWeight(std::string name, double amt)
{
    ValueType a;
    if (! m_map.get(name, a))
        return false;
    amt += a;
    if (amt < 0)
        return false;
    else
        m_map.update(name, amt);
    return true;
}

int WeightMap::size() const
{
    return m_map.size();
}

void WeightMap::print() const
{
    KeyType name;
    ValueType weight;
    for (int i = 0; i < m_map.size(); i++)
    {
        m_map.get (i, name, weight);
        std::cout << name << " " << weight << std::endl;
    }
}