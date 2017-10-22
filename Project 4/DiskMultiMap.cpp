//
//  DiskMultiMap.cpp
//  Project 4 full
//
//  Created by Agustin Marinovic on 3/7/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <stdio.h>
#include "DiskMultiMap.h"

DiskMultiMap::DiskMultiMap()
{
    
}

DiskMultiMap::~DiskMultiMap()
{
    bf.close();
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    bf.close();
    BinaryFile::Offset nullnull = 0;
    
    if ( !bf.createNew(filename))
    {
        cerr << "Cant create " << filename << endl;
        return false;
    }
    bf.write(numBuckets, 0);        //Number of Buckets
    bf.write(nullnull, sizeof(unsigned int));      //First empty space is 0

    firstBucket = sizeof(unsigned int) + sizeof(BinaryFile::Offset);
    
    for (int k = 0; k < numBuckets; k++)
    {
        if( !bf.write(nullnull, firstBucket + k*sizeof(BinaryFile::Offset)))    //Creat N empty spots and fill them all with "null"
            return false;
    }
    return true;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
    bf.close();
    return bf.openExisting(filename);
}

void DiskMultiMap::close()
{
    bf.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
    if ( !bf.isOpen())
    {
        cerr << "Not Opened" << endl;
        return false;
    }
    
    if ((key.size()+value.size()+context.size() + 2) > 120)
    {
        cerr << "Too big" << endl;
        return false;
    }
    
    
    std::string stringer = key + " " + value + " " + context;
    DiskNode dn;
    dn.off = 0;
    dn.stringTheDisk(stringer);
    unsigned int nBuckets;
    bf.read(nBuckets, 0);
    unsigned int bucket = Hashing(key, nBuckets);       //Get the appropiate bucket
    BinaryFile::Offset putItIn = sizeof(unsigned int) + (1+bucket)*sizeof(BinaryFile::Offset), readBucket;
    bf.read(readBucket, putItIn);
    BinaryFile::Offset location = goodInsert();
    
    dn.off = readBucket;       //Make the new first node point to the former first
    if (!bf.write(dn, location) || !bf.write(location, putItIn))
    {
        cerr << "Couldn't write" << endl;
        return false;
    }
    
    return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    if ( !bf.isOpen())
        return DiskMultiMap::Iterator();
    
    unsigned int nBuckets;
    bf.read(nBuckets, 0);
    unsigned int bucket = Hashing(key, nBuckets);
    BinaryFile::Offset bucketLocation = sizeof(unsigned int) + sizeof(BinaryFile::Offset)*(1+bucket), bucketContent;
    
    bf.read(bucketContent, bucketLocation);     //Read bucket of the key
    
    if (bucketContent == 0)     //Bucket is empty, nothing to look
        return Iterator();
    else        //Leggo
    {
        DiskMultiMap::DiskNode dn;
        dn.off = 0;
        MultiMapTuple m;
        do
        {
            bf.read(dn, bucketContent);
            m = multiMapping(dn);
            if (m.key == key)
                return Iterator (bucketContent, &bf);     //Found a key :)
            
            bucketContent = dn.off;     //Move to whatever dn was 'pointing' to
            
        } while (bucketContent != 0);
        
    }
    return Iterator();
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    if ( !bf.isOpen())
        return -1;
    
    int numOfDeletions = 0;

    unsigned int nBuckets;
    bf.read (nBuckets, 0);
    unsigned int bucket = Hashing(key, nBuckets);
    
    DiskNode dn;
    MultiMapTuple m;
    
    BinaryFile::Offset current, bLoc = sizeof(unsigned int) + (bucket+1)*sizeof(BinaryFile::Offset), prev = current;
    
    bf.read(current, bLoc); //Location Bucket points to of first node
    if (current == 0)
        return 0;
    while (current != 0)       //First item matches
    {
        bf.read(dn, current);       //Read the link
        m = multiMapping(dn);       //Create a multimap from the link
        
        
        if (m.context == context && m.value == value && m.key == key)       //Found a match
        {
            bf.write(dn.off, bLoc);     //De-link first item
            
            DiskNode delNode;
            BinaryFile::Offset delHead;
            bf.read(delHead, sizeof(unsigned int));     //Get head of del list
            
            delNode.off = delHead;                  //Create dn pointing to the head
            bf.write(delNode, current);             //OverWrite the deleted location
            bf.write(current, sizeof(unsigned int));//Chnge the header of the del-list to point to the newly deleted item
            numOfDeletions++;
        }
        else
        {
            prev = current;
            current = dn.off;
            break;
        }
        prev = current;
        current = dn.off;
    }
        DiskNode prevNode = dn;     //Make "previous node" of last non deleted node
    while (current != 0)        //Look further in the list
    {
        
        bf.read(dn, current);       //Read the new link
        m = multiMapping(dn);       //Create a multimap from the link
        
        if (m.context == context && m.value == value && m.key == key)       //Found a match
        {
            prevNode.off = dn.off;      //Adjust previous node
            bf.write(prevNode, prev);   //Update previous node on file
            
            DiskNode delNode;
            BinaryFile::Offset delHead;
            bf.read(delHead, sizeof(unsigned int));     //Get head of del list
            
            delNode.off = delHead;                  //Create dn pointing to the head
            bf.write(delNode, current);             //OverWrite the deleted location
            bf.write(current, sizeof(unsigned int));//Chnge the header of the del-list to point to the newly deleted item
            numOfDeletions++;
        }
        else
        {
            prev = current;
            prevNode = dn;
        }
        
        current = dn.off;
        
    }

    return numOfDeletions;
}

//--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--//

MultiMapTuple DiskMultiMap::multiMapping(const DiskNode& dn)
{
    MultiMapTuple m;
    int k = 0;
    std::string s1 = "", s2 = "", s3 = "";
    
    
    while (dn.text[k] != ' ' && k < strlen(dn.text))
    {
        s1 += dn.text[k];
        k++;
        
    }
    m.key = s1;
    k++;
    while (dn.text[k] != ' ' && k < strlen(dn.text))
    {
        s2 += dn.text[k];
        k++;
    }
    m.value = s2;
    k++;
    while (k < strlen(dn.text))
    {
        s3 += dn.text[k];
        k++;
    }
    m.context = s3;
    
    return m;
}

unsigned int DiskMultiMap::Hashing(const std::string& key, unsigned int nBuckets)
{
    hash<std::string> k;
    size_t s = k(key);
    unsigned int a = s % nBuckets;
    cerr << "Hash: " << a << endl;
    return a;
}

BinaryFile::Offset DiskMultiMap::goodInsert()
{
    BinaryFile::Offset start;
    bf.read(start, sizeof(unsigned int));
    
    if (start == 0)
        return bf.fileLength();
    else
    {
        DiskNode D;
        D.off = 0;
        bf.read(D, start);
        bf.write(D.off, sizeof(unsigned int));
        
        return start;
    }
}

//--------------------------------------------------------------------------------------------------//

DiskMultiMap::Iterator::Iterator()
{
    valid = false;
    location = 0;
    key = "";
    bfp = nullptr;
}

DiskMultiMap::Iterator::Iterator(const Iterator& c, BinaryFile* b)
{
    valid = c.valid;
    location = c.location;
    key = c.key;
    m_m.key = c.m_m.key;
    m_m.value = c.m_m.value;
    m_m.context = c.m_m.context;
    bfp = c.bfp;
}

DiskMultiMap::Iterator::Iterator(BinaryFile::Offset o, BinaryFile* b)
{
    bfp = b;
    location = o;
    DiskNode dn;
    if (bfp != nullptr)
        bfp->read(dn, o);
    m_m = multiMap(dn);
    key = m_m.key;
    valid = true;
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator=(const Iterator& c){
    valid = c.valid;
    location = c.location;
    key = c.key;
    m_m = c.m_m;
    bfp = c.bfp;
    return *this;
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
    
    if (!valid || bfp == nullptr)
        return *this;
    DiskNode dn;
    dn.off = 0;
    while (location != 0)
    {
        bfp->read (dn, location);
        location = dn.off;
        
        if (keyring())
        {
            if (key == m_m.key)     //Another match
            {
                multiMapper();
                valid = true;
                return *this;
            }
        }
    }
    
    valid = false;
    return *this;
}

bool DiskMultiMap::Iterator::isValid() const
{
    return valid && location != 0;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    if (valid)
        return m_m;
    else
        return MultiMapTuple();
}

//----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----//

bool DiskMultiMap::Iterator::multiMapper()
{
    if (location == 0 || !bfp->isOpen())
        return false;
    
    DiskNode dn;
    dn.off = 0;
    bfp->read(dn, location);
    
    MultiMapTuple m = multiMap(dn);
    //key = m.key;
    m_m = m;
    
    return true;
}

bool DiskMultiMap::Iterator::keyring()
{
    if (location == 0 || !bfp->isOpen())
        return false;
    
    DiskNode dn;
    dn.off = 0;
    bfp->read(dn, location);
    
    MultiMapTuple m = multiMap(dn);
    key = m.key;
    
    return true;
}

MultiMapTuple DiskMultiMap::Iterator::multiMap(const DiskNode& dn)
{
    MultiMapTuple m;
    int k = 0;
    std::string s1 = "", s2 = "", s3 = "";
    
    
    while (dn.text[k] != ' ' && k < strlen(dn.text))
    {
        s1 += dn.text[k];
        k++;
        
    }
    m.key = s1;
    k++;
    while (dn.text[k] != ' ' && k < strlen(dn.text))
    {
        s2 += dn.text[k];
        k++;
    }
    m.value = s2;
    k++;
    while (k < strlen(dn.text))
    {
        s3 += dn.text[k];
        k++;
    }
    m.context = s3;
    
    return m;
}

