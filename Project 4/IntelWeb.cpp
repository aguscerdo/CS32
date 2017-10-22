//
//  IntelWeb.cpp
//  Project 4 full
//
//  Created by Agustin Marinovic on 3/8/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include "IntelWeb.h"
#include <queue>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>

IntelWeb::IntelWeb()
{
    
}
IntelWeb::~IntelWeb()
{
    
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    unsigned int nBuckets = maxDataItems*10/7;
    
    fileName[0] = filePrefix + "-Implication";
    fileName[1] = filePrefix + "-Implied";
    
    if ( !dmm0.createNew(fileName[0], nBuckets))
        return false;
    if ( !dmm1.createNew(fileName[1], nBuckets))
        return false;
    
    return true;
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
    fileName[0] = filePrefix + "-Implication";
    fileName[1] = filePrefix + "-Implied";
    if ( !dmm0.openExisting(fileName[0]) || !dmm1.openExisting(fileName[1]))
    {
        cerr << "Cant reopen" << endl;
        dmm0.close();
        dmm1.close();
        return false;
    }
    
    return true;
}

void IntelWeb::close()
{
    dmm0.close();
    dmm1.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
    ifstream infile(telemetryFile);
    if (!infile)
        return false;
    std::string s;
    MultiMapTuple m, mdmm1, mdmm2;
    
    while (getline(infile, s))
    {
        m = logToTuple(s);
    
        mdmm1.key = m.value;
        mdmm1.value = m.context;
        mdmm1.context = m.key;
        
        mdmm2.key = m.context;
        mdmm2.value = m.value;
        mdmm2.context = m.key;
    
    if ( !dmm0.insert(mdmm1.key, mdmm1.value, mdmm1.context))     //Implication file
        return false;
        
    if ( !dmm1.insert(mdmm2.key, mdmm2.value, mdmm2.context))     //Implied file
        return false;
    }
    
    return true;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators,
                             unsigned int minPrevalenceToBeGood,
                             std::vector<std::string>& badEntitiesFound,
                             std::vector<InteractionTuple>& interactions
                             )
{
    if (indicators.empty())    //No indicators
        return 0;
    
    while ( !badEntitiesFound.empty())      //Clear previous bad entities
        badEntitiesFound.pop_back();
    while ( !interactions.empty())
        interactions.pop_back();
    
    std::map<std::string, bool> uMap;     //Keeps track of all the elements checked, so they don't get rechecked
    
    
    std::queue<std::string> q;
    for (vector<std::string>::const_iterator i = indicators.begin(); i != indicators.end(); i++)
    {
        uMap[*i] = false;
        q.push(*i);
    }
    
    std::string suspect;
    MultiMapTuple tuple;
    InteractionTuple iTuple;
    
    while ( !q.empty())
    {
        suspect = q.front();
        q.pop();
        
        {       //Examine first file from -> to
            DiskMultiMap::Iterator i1 = dmm0.search(suspect);       //Implication dmm
        
            while (i1.isValid())
            {
                tuple = *i1;
                
                iTuple.from = tuple.key;    //Write interaction down into a log
                iTuple.to = tuple.value;
                iTuple.context = tuple.context;
                interactions.push_back(iTuple);
                
                if (lowPrevalence(tuple.value, minPrevalenceToBeGood, uMap))        //If prevalence is big and item has been examined before
                {
                    q.push(tuple.value);        //Value hasn't been examined, add it for relations
                    badEntitiesFound.push_back(tuple.value);    //Add it to the list
                }
                
                ++i1;
            }
        }
    
        
        {       //Examine first file to -> from
            DiskMultiMap::Iterator i2 = dmm1.search(suspect);       //Implication dmm
            
            while (i2.isValid())
            {
                tuple = *i2;
                
                iTuple.from = tuple.value;
                iTuple.to = tuple.key;
                iTuple.context = tuple.context;
                interactions.push_back(iTuple);
                
                if (lowPrevalence(tuple.value, minPrevalenceToBeGood, uMap))        //If prevalence is small
                {
                    q.push(tuple.value);
                    badEntitiesFound.push_back(tuple.value);
                }
                
                ++i2;
            }
        }
    }
    
    sort(interactions.begin(), interactions.end(), compareTuple);   //Sort logs
    sort(badEntitiesFound.begin(), badEntitiesFound.end()); //Sort entities found
    
    for (vector<InteractionTuple>::iterator j = interactions.begin(), h; j != interactions.end();)      //Delete duplicates
    {
        h = j;
        h++;
        if (h != interactions.end() && (*h).context == (*j).context && (*h).from == (*j).from && (*h).to == (*j).to)
        {
            interactions.erase(h);
        }
        else
        {
            j++;
        }
        
    }
    return badEntitiesFound.size() + indicators.size();
}


bool IntelWeb::purge(const std::string& entity)
{
    bool returner = false;
    
    std::queue<MultiMapTuple> q;
    DiskMultiMap::Iterator i0 = dmm0.search(entity);      //Check first file
    DiskMultiMap::Iterator i1 = dmm1.search(entity);      //Check Second file
    MultiMapTuple m;
    
    while (i0.isValid())        //Check first file
    {
        q.push(*i0);
        ++i0;
    }
    while (i1.isValid())
    {
        q.push(*i1);
        ++i1;
    }
    
    if (!q.empty())
        returner = true;
    
    while (!q.empty())
    {
        m = q.front();
        q.pop();
        dmm1.erase(m.key, m.value, m.context);
        dmm0.erase(m.value, m.key, m.context);
        dmm0.erase(m.key, m.value, m.context);
        dmm1.erase(m.value, m.key, m.context);
    }
    
    return returner;
}


//------------------------------------------------------------------------------//

MultiMapTuple IntelWeb::logToTuple(std::string s)
{
    int k = 0;
    std::string i[3];
    MultiMapTuple m;
    
    while (s[k] != ' ')
    {
        i[0] += s[k];
        k++;
    }
    m.key = i[0];
    k++;
    
    while (s[k] != ' ')
    {
        i[1] += s[k];
        k++;
    }
    m.value = i[1];
    k++;
    
    while (k < s.size())
    {
        i[2] += s[k];
        k++;
    }
    m.context = i[2];
    
    return m;
}

std::string IntelWeb::logEncoder(MultiMapTuple m)
{
    std::string s = "";
    
    s = m.key + " " + m.value + " " + m.context;
    
    return s;
}

bool IntelWeb::compareTuple(InteractionTuple i1, InteractionTuple i2)
{
    if (i1.context > i2.context)
        return false;
    else if (i1.context < i2.context)
        return true;
    else
    {
        if (i1.from > i2.from)
            return false;
        else if (i1.from < i2.from)
            return true;
        else
            return i1.to > i2.to;
    }
    
}

bool IntelWeb::lowPrevalence(const std::string& key, const unsigned int& minP, std::map<std::string, bool>& maps)
{
    map<std::string, bool>::iterator iter = maps.find(key);
    
    if (iter != maps.end())
        return false;
    
    DiskMultiMap::Iterator i = dmm0.search(key);
    unsigned int n = 0;
    while (i.isValid())
    {
        if (n++ >= minP)
        {
            maps[key] = true;
            return false;
        }
        ++i;
    }
    
    i = dmm1.search(key);
    
    while (i.isValid())
    {
        if (n++ >= minP)
        {
            maps[key] = true;
            return false;
        }
        ++i;
    }
    
    maps[key] = n < minP;
    
    return n < minP;
}