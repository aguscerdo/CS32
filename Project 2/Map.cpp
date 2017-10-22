//
//  Map.cpp
//  Project 2 - Marinovic
//
//  Created by Agustin Marinovic on 1/20/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include "Map.h"

Map::Map()
{
    
    m_first = nullptr;
    m_last = nullptr;
    m_size = 0;
}

Map::Map(const Map& copycat)
{
    Node* ptr_this = nullptr;
    Node* ptr_copy = copycat.m_first;
    for (int i = 0; i < copycat.size(); i++)
    {
        if (ptr_this == nullptr)    //First item to be created
        {
            m_first = new Node (ptr_copy->m_key, ptr_copy->m_val);      //Create new Node for m_first
            ptr_this = m_first;
        }
        else
        {
            ptr_this->m_next = new Node(ptr_copy->m_key, ptr_copy->m_val, nullptr, ptr_this);       //Create nodes linked to the previous one
            ptr_this = ptr_this->m_next;
        }
        
        ptr_copy = ptr_copy->m_next;
        
    }
    m_last = ptr_this;
    m_size = copycat.size();
}

Map& Map::operator=(const Map& copycat)
{
    if (this == &copycat)   //DONT DELETE YOURSELF
        return *this;
    
    Node* Hippies = nullptr;
    Node* ptr = m_first;
    
    while (ptr != nullptr)  //Delete this' allocated memory
    {
        std::cerr << "Cleaning Map\n";
        Hippies = ptr;
        ptr = ptr->m_next;
        delete Hippies;
    }

    Node* ptr_this = nullptr;       //Implement the same as copy constructor
    Node* ptr_copy = copycat.m_first;
    for (int i = 0; i < copycat.size(); i++)
    {
        if (ptr_this == nullptr)    //First item to be created
        {
            m_first = new Node (ptr_copy->m_key, ptr_copy->m_val);
            ptr_this = m_first;
        }
        else        //Other items to be created
        {
            ptr_this->m_next = new Node(ptr_copy->m_key, ptr_copy->m_val, nullptr, ptr_this);   //By default the ptr m_next is null
            ptr_this = ptr_this->m_next;
        }
        
        ptr_copy = ptr_copy->m_next;
        
    }
    m_last = ptr_this;
    m_size = copycat.size();
    
    return *this;
}

Map::~Map()
{
    Node* Hippies = nullptr;
    Node* ptr = m_first;
    
    while (ptr != nullptr)  //Delete this' allocated memory
    {
        //std::cerr << "Destructor called, deleting objects.\n";
        Hippies = ptr;
        ptr = ptr->m_next;
        delete Hippies;
    }
}

bool Map::empty() const
{
    return m_size == 0;
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if(m_first == nullptr) //Check if its gonna be the first item of the list
    {
        m_first = new Node(key, value); //Create a new Node, assigned to m_first and m_last
        std::cerr << key << " successfuly insertd.\n";
        m_last = m_first;
        m_size++;
        return true;
        
    }
    else        //CHECK****************************
    {
        Node* curr;
        Node* prev = nullptr;
        for(curr = m_first; curr != nullptr && curr->m_key != key; prev = curr, curr = curr->m_next); //Lookup the name on the list
        
        if (curr != nullptr)   //if found, p != nullptr
            return false;
        else                    // Not found, create new Node, increase m_size
        {
            prev->m_next = new Node(key, value, nullptr, prev);     //Create a new Node
            std::cerr << key << " successfully insertd.\n";
            (prev->m_next)->m_next = nullptr;
            m_size++;
            m_last = prev->m_next;
            return true;
        }
    }
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* curr;
    for(curr = m_first; curr != nullptr && curr->m_key != key; curr = curr->m_next);
    
    if (curr == nullptr)        //Key not found
        return false;
    else
    {
        std::cerr << key << "'s value updated.\n";
        //curr->m_key = key;      //Key found, update the value
        curr->m_val = value;
        return true;
    }
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if(m_first == nullptr) //Check if its gonna be the first item of the list
    {
        m_first = new Node(key, value); //Create a new Node, assigned to m_first and m_last
        std::cerr << key << " successfully inserted.\n";
        m_last = m_first;
        return true;
        
    }
    else        //CHECK****************************
    {
        Node* curr;
        Node* prev = nullptr;
        for(curr = m_first; curr != nullptr && curr->m_key != key; prev = curr, curr = curr->m_next); //Lookup the name on the list
        
        if (curr != nullptr)   //if found, update
        {
            //curr->m_key = key;
            std::cerr << key << "'s value updated.\n";
            curr->m_val = value;
            return true;
        }
        else                    // Not found, create new Node, increase m_size
        {
            prev->m_next = new Node(key, value, nullptr, prev);
            std::cerr << key << " successfully inserted.\n";
            m_size++;
            m_last = prev->m_next;
            return true;
        }
    }
}

bool Map::erase(const KeyType& key)
{
    
    Node* curr;
    Node* prev = nullptr;
    
    for(curr = m_first; curr != nullptr && curr->m_key != key; prev = curr, curr = curr->m_next);       //Look for the Key
    
    if (curr == nullptr)    //Not Found
        return false;
    
    else if (m_size == 1 && m_first->m_key == key)    //One element list
    {
        std::cerr << key << " found, erasing.\n";
        curr = m_first;
        delete curr;
        m_first = nullptr;
        m_last = nullptr;
        m_size--;
        return true;
    }
    else if (curr->m_next == nullptr)   //Deleting last element
    {
        std::cerr << key << " found, erasing.\n";
        prev->m_next = nullptr;
        m_last = prev;
        delete curr;
        m_size--;
        return true;
        
    }
    else if (curr->m_previous == nullptr && m_first->m_key == key)   //Deleting first element
    {
        std::cerr << key << " found, erasing.\n";
        curr = m_first;
        //(curr->m_next)->m_previous = nullptr;
        m_first->m_next->m_previous = nullptr;
        m_first = m_first->m_next;
        delete curr;
        m_size--;
        return true;
    }
    else //if (curr != nullptr)
    {
        std::cerr << key << " found, erasing.\n";
        (curr->m_previous)->m_next = curr->m_next;  //Connect items before and after curr
        (curr->m_next)->m_previous = curr->m_previous;
        delete curr;
        m_size--;
        return true;
    }
}

bool Map::contains(const KeyType& key) const
{
    for (Node* alpha = m_first; alpha != nullptr; alpha = alpha->m_next) //Go through every item of the list, if found return true
        if (alpha->m_key == key)
            return true;
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    for (Node* alpha = m_first; alpha != nullptr; alpha = alpha->m_next) //Go through every item of the list, if found return true
    {
        if (alpha->m_key == key)
        {
            std::cerr << "Get2, found " << alpha->m_key << std::endl;
            value = alpha->m_val;
            return true;
        }
    }
    
    std::cerr << "Get2, " << key << " not found\n";
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const  ///**** Make more efficent *******************************
{
    if (i >= m_size || m_size == 0 || i < 0)
        return false;
    
    Node* omicron = m_first;
    //for (int e = 0; e != i; omicron = omicron->m_next, e++);
    
    int e = 0;
    while (e != i)      //Move i times through the linked list
    {
        omicron = omicron->m_next;
        e++;
    }
    
    key = omicron->m_key;
    std::cerr << "Get3 found: " << key << "\n";
    value = omicron->m_val;     //Update values
    return true;
    
        
}

void Map::swap(Map& other)
{
    Node* ptr;
    
    ptr = m_first;  //Swap initial pointers
    m_first = other.m_first;
    other.m_first = ptr;
    
    ptr = m_last;   //Switch end pointers
    m_last = other.m_last;
    other.m_last = ptr;
    
    int s = m_size; //Switch sizes
    m_size = other.m_size;
    other.m_size = s;
}

void Map::dump() const  //display all values
{
    for (Node* ptr = m_first; ptr != nullptr; ptr = ptr->m_next)
        std::cerr << ptr->m_key << " --- " << ptr->m_val << std::endl;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    bool Trouble = true;
    KeyType K;
    ValueType V, V2;
    
    std::cerr << "Combine!\n";
    
    if (&m1 != &result && &m2 != &result)
    {
        while ( ! result.empty())       //Clean the result Map
        {
            result.get(0, K, V);
            std::cerr << "Deleting " << K << "\n";
            result.erase(K);
        }
    }
    
    for (int i = 0; i < m1.size(); i++)     //Go through all the items in m1
    {
        m1.get(i, K, V);
        std::cerr << "Got: " << K << std::endl;
        if (result.get(K, V2))      //If the item is already on result, check that the values match
        {
            if (V != V2)
            {
                Trouble = false;      //Values don't match, delete the key from the list, set returner to false
                std::cerr << K << " isn't consistent, deleting.\n";
                result.erase(K);
            }
        }
        else        //No such Key found on result
        {
            result.insert(K, V);
            std::cerr << K << " inserted.\n";
        }
    }
    
    for (int i = 0; i < m2.size(); i++)     //Go through all items in m2
    {
        m2.get(i, K, V);
        std::cerr << "Got: " << K << std::endl;
        if (result.get(K, V2))      //Check if a key with the same name is already in result
        {
            if (V != V2)
            {
                Trouble = false;      //Values not consistent, delete key from list
                std::cerr << K << " isn't consistent, deleting.\n";
                result.erase(K);
            }
        }
        else
        {
            result.insert(K, V);        //No matching Key, insert it to result list
            std::cerr << K << " inserted.\n";
        }
    }
    
    return Trouble;
}


void subtract(const Map& m1, const Map& m2, Map& result)
{
    KeyType K;
    ValueType V;
    int code = 0;
    Map alpha;
    
    std::cerr << "Subtract!\n";
    
    if (&m1 == &result)     //If m1 is the same as result, create temporary for m1 and clear result
    {
        alpha = m1;
        code = 1;
    }
    else if (&m2 == &result)     //If m2 is the same as result, create temporary for m2 and clear result
    {
        alpha = m2;
        code = 2;
    }

    
    while ( ! result.empty())       //Clean the result Map
    {
        
        result.get(0, K, V);
        std::cerr << "Deleting " << K << "\n";
        result.erase(K);
    }
    
    if (code == 0)      //Check for scenarios in which result is the same as m1 or m2
    {
        for (int e = 0; e < m1.size(); e++)
        {
            m1.get(e, K, V);
            if (! m2.contains(K))       //Check if m2 has the element from m1, if not insert it to result
            {
                std::cerr << K << " not duplicated, inserting.\n";
                result.insert(K, V);
            }
            else
                std::cerr << K << " is duplicated, NOT inserting.\n";
        }
    }
    else if (code == 1)
    {
        for (int e = 0; e < alpha.size(); e++)
        {
            alpha.get(e, K, V);
            if (! m2.contains(K))       //Check if m2 has the element from alpha, if not insert it to result
            {
                std::cerr << K << " not duplicated, inserting.\n";
                result.insert(K, V);
            }
            else
                std::cerr << K << " is duplicated, NOT inserting.\n";
        }
    }
    else if (code == 2)
    {
        for (int e = 0; e < m1.size(); e++)
        {
            m1.get(e, K, V);
            if (! alpha.contains(K))       //Check if alpha has the element from m1, if not insert it to result
            {
                std::cerr << K << " not duplicated, inserting.\n";
                result.insert(K, V);
            }
            else
                std::cerr << K << " is duplicated, NOT inserting.\n";
        }
    }
}












