//
//  Map.cpp
//  HW1
//
//  Created by Agustin Marinovic on 1/15/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include "newMap.h"

Map::Map(int aSize)
{
    if (aSize < 0)
    {
        std::cerr << "Size of the Map must be non-negative" << std::endl;
        exit(1);
        
    }
    m_size = 0;
    m_maxSize = aSize;
    m_array = new KeyVal[aSize];
}

Map::Map(const Map& copyCat)
{
    m_size = copyCat.size();
    m_array = new KeyVal[m_size];
    for (int i = 0; i < m_size; i++)
    {
        m_array[i] = copyCat.m_array[i];
    }
    m_maxSize = copyCat.m_maxSize;
}

Map& Map::operator=(const Map& copyCat)
{
    if ( &copyCat == this)
        return *this;
    
    delete [] m_array;
    m_array = new KeyVal[copyCat.m_maxSize];
    for (int i = 0; i < m_size; i++)
    {
        m_array[i] = copyCat.m_array[i];
    }
    m_size = copyCat.size();
    m_maxSize = copyCat.m_maxSize;
    
    return *this;
}

Map::~Map()
{
    delete [] m_array;
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
    Map potato = other;
    other = *this;
    *this = potato;
    3;
}











