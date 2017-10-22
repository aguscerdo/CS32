#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <string>
#include <vector>
#include <map>

class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    bool ingest(const std::string& telemetryFile);
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& interactions
                       );
    bool purge(const std::string& entity);
    
private:
    // Your private member declarations will go here
    DiskMultiMap dmm0;
    DiskMultiMap dmm1;
    std::string fileName[2];
    
    MultiMapTuple logToTuple(std::string s);
    std::string logEncoder (MultiMapTuple m);
    static bool compareTuple (InteractionTuple i1, InteractionTuple i2);
    bool lowPrevalence(const std::string& key, const unsigned int& minP, std::map<std::string, bool>& maps);
    
};

#endif // INTELWEB_H_
