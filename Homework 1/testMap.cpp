#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map a;
    assert (a.empty());
    
    assert (a.size() == 0);
    
    assert (a.insert("Julia", 3.90) && a.size() == 1);
    
    assert (! a.empty());
    
    assert (a.update("Julia", 4.00));
    assert (! a.update("Jordan", 1.00));
    
    assert (a.insertOrUpdate("Jordan", 3.60) && a.size() == 2);
    assert (a.insertOrUpdate("Jordan", 3.70));
    
    assert (a.erase("Jordan") && a.size() == 1);
    assert (! a.erase("Jordan"));
    
    assert (a.contains("Julia") && ! a.contains("Jordan"));
    
    ValueType julez = 0.1;
    ValueType daddy = 3.14;
    assert (a.get("Julia", julez) && julez == 4.00); //Succeed
    assert (! a.get("Jordan", daddy) && daddy == 3.14); //Fail
    
    KeyType luhOG;
    ValueType vStick;
    assert (a.get(0, luhOG, vStick) && luhOG == "Julia" && vStick == 4.00);
    
    Map b;
    b.insert("A", 1);
    b.insert("B", 2);
    a.swap(b);
    assert (a.size() == 2 && b.size() == 1);
    
    std::cerr << "Success" << std::endl;
}