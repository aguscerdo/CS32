//
//  Map.hpp
//  Project 2 - Marinovic
//
//  Created by Agustin Marinovic on 1/20/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <stdio.h>
#include <iostream>
#include <string>

typedef std::string KeyType;
typedef double ValueType;



class Map
{
public:
    Map();
    Map(const Map& copycat);
    Map& operator=(const Map& copycat);
    ~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    
    void dump() const;
private:
    
    struct Node
    {
        Node();
        Node(KeyType key, ValueType val, Node* ptr_n = nullptr, Node* ptr_p = nullptr){
            m_key = key;
            m_val = val;
            m_next = ptr_n;
            m_previous = ptr_p;
        }
        
        KeyType m_key;
        ValueType m_val;
        Node* m_next;
        Node* m_previous;
    };
    int m_size;
    //Node m_node;
    Node* m_first;
    Node* m_last;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_h */
