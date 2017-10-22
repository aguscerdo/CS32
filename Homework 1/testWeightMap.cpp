#include "WeightMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    WeightMap a;
    
    assert (a.enroll("Julia", 140) && ! a.enroll("Julia", 141));
    assert (! a.enroll("Jordan", -5));
    
    assert (a.weight("Julia") == 140 && a.weight("Jordan") == -1);
    
    assert (a.adjustWeight("Julia", -1) && a.weight("Julia") == 139);
    assert (! a.adjustWeight("Julia", -10000) && a.weight("Julia") == 139);
    assert (! a.adjustWeight("Jordan", 100));
    
    assert (a.size() == 1);
    
    a.print();
    
    std::cerr << "Success" << std::endl;
}