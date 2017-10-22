//
//  Map.cpp
//  HW1
//
//  Created by Agustin Marinovic on 1/15/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include "Map.h"

Map::Map()
{
    m_size = 0;
    m_maxSize = DEFAULT_MAX_ITEMS;
}

bool Map::empty()
{
    return m_size == 0;
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (m_size >= m_maxSize)
    {
        std::cerr << "The Map is full" << std::endl;
        return false;
    }
    
    for (int i(0); i < m_size; i++)
        if (key == m_array[i].key)
        {
            std::cerr << "Name already on the Map" << std::endl;
            return false;
        }
    
    m_array[m_size].key = key;
    m_array[m_size].val = value;
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    for (int i(0); i < m_size; i++)
        if (key == m_array[i].key)
        {
            m_array[i].val = value;
            return true;
        }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    for (int i(0); i < m_size; i++)
        if (key == m_array[i].key)
        {
            m_array[i].val = value;
            return true;
        }
    return this->insert(key, value);
//    if (m_size >= m_maxSize)
//        return false;
//    
//    for (int i(0); i < m_size; i++)
//        if (key == m_array[i].key)
//            return false;
//    
//    m_array[m_size].key = key;
//    m_array[m_size].val = value;
//    m_size++;
//    return true;
}

bool Map::erase(const KeyType& key)
{

    for (int i(0); i < m_size; i++)
        if (key == m_array[i].key)
        {
            if (m_size == 1)
            {
                m_size--;
                return true;
            }
            m_array[i].key = m_array[m_size-1].key;
            m_array[i].val = m_array[m_size-1].val;
            m_size--;
            return true;
        }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    for (int i(0); i < m_size; i++)
        if (key == m_array[i].key)
            return true;
    return  false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    for (int i(0); i < m_size; i++)
        if (key == m_array[i].key)
        {
            value = m_array[i].val;
            return true;
        }
    
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 && i >= m_size)
        return false;
        
    key = m_array[i].key;
    value = m_array[i].val;
    return true;
}

void Map::swap(Map& other)
{
    KeyType placeK;
    ValueType placeV;
    int small, big, origin;
    
    if (m_size > other.size())
        big = m_size, small = other.size(), origin = 1;
    else
        small = m_size, big = other.size(), origin = 0;
    
    for (int i = 0; i < small; i++) //Exchange until the smallest one runs out
    {
        placeK = m_array[i].key;
        m_array[i].key = other.m_array[i].key;
        other.m_array[i].key = placeK;
        
        placeV = m_array[i].val;
        m_array[i].val = other.m_array[i].val;
        other.m_array[i].val = placeV;
    }
    
    if (origin) //If this.size() > other.size()
        for (int i = small; i < big; i++)
        {
            other.m_array[i].key = m_array[i].key;
            other.m_array[i].val = m_array[i].val;
        }
    else
        for (int i = small; i < big; i++) //this.size()n < other.size()
        {
           m_array[i].key = other.m_array[i].key;
           m_array[i].val = other.m_array[i].val;
        }
    int placeS = m_size;
    m_size = other.size();
    other.m_size = placeS;
}











