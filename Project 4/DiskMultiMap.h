#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include <cstring>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        //Iterator(BinaryFile* b);
        Iterator(BinaryFile::Offset o, BinaryFile* b);
        Iterator(const Iterator& c, BinaryFile* b);
        Iterator& operator= (const Iterator& c);
        
        // You may add additional constructors
        bool isValid() const;
        Iterator& operator++();
        MultiMapTuple operator*();
        
    private:
        // Your private member declarations will go here
        struct DiskNode
        {
        public:
            char text[121];
            BinaryFile::Offset off;
            
            void stringTheDisk(std::string s)
            {
                strcpy(text, s.c_str());
            }
        };
        
        BinaryFile::Offset location;
        MultiMapTuple m_m;
        std::string key;
        bool valid;
        //DiskMultiMap& parent;
        BinaryFile* bfp;
        
        bool multiMapper();
        bool keyring();
        MultiMapTuple multiMap(const DiskNode& dn);
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    
    struct DiskNode
    {
    public:
        char text[121];
        BinaryFile::Offset off;
        
        void stringTheDisk(std::string s)
        {
            strcpy(text, s.c_str());
        }
    };
    
    unsigned int numBuckets;
    BinaryFile bf;
    BinaryFile::Offset emptySpot;
    BinaryFile::Offset firstBucket;
    
    
    unsigned int Hashing (const std::string& key, unsigned int nBuckets);
    BinaryFile::Offset goodInsert();
    void linkDeleted (const BinaryFile::Offset& takenLocation, const BinaryFile::Offset& tNext);     //Links node after a deletion
    MultiMapTuple multiMapping(const DiskNode& dn);
};



#endif // DISKMULTIMAP_H_