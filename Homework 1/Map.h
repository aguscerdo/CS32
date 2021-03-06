//
//  Map.hpp
//  HW1
//
//  Created by Agustin Marinovic on 1/15/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <stdio.h>
#include <iostream>

typedef std::string KeyType;
typedef double ValueType;
const int DEFAULT_MAX_ITEMS = 200;

struct KeyVal
{
    KeyType key;
    ValueType val;
};


class Map
{
public:
    Map();
    
    bool empty();
    
    int size() const;
    
    bool insert(const KeyType& key, const ValueType& value);
    
    bool update(const KeyType& key, const ValueType& value);
    
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    
    bool erase(const KeyType& key);
    
    bool contains(const KeyType& key) const;
    
    bool get(const KeyType& key, ValueType& value) const;
    
    bool get(int i, KeyType& key, ValueType& value) const;
    
    void swap(Map& other);
    
private:
    KeyVal m_array[DEFAULT_MAX_ITEMS];
    
    int m_size;
    
    int m_maxSize;
};





#endif /* Map_hpp */
