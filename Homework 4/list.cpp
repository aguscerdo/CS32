////
////  list.cpp
////  Homework 4
////
////  Created by Agustin Marinovic on 2/27/16.
////  Copyright © 2016 Agustin Marinovic. All rights reserved.
////
//
//#include <stdio.h>
//
//#include <iostream>
//#include <string>
//#include <vector>
//
//using namespace std;
//
//class Class
//{
//public:
//    Class(string nm) : m_name(nm) {}
//    string name() const { return m_name; }
//    const vector<Class*>& subclasses() const { return m_subclasses; }
//    void add(Class* d) { m_subclasses.push_back(d); }
//    ~Class();
//private:
//    string m_name;
//    vector<Class*> m_subclasses;
//};
//
//Class::~Class()
//{
//    for (size_t k = 0; k < m_subclasses.size(); k++)
//        delete m_subclasses[k];
//}
//
void listAll(const Class* c, string path)  // two-parameter overload
{
    if (path != "")
    {
        path += "=>";
    }
    path += c->name();
    cout << path << endl;
    
    //if ( ! c->subclasses().empty())
    {
        for (vector<Class*>::const_iterator i = c->subclasses().begin(); i != c->subclasses().end(); i++)
        {
            listAll(*i, path);
        }
    }
}
//
//void listAll(const Class* c)  // one-parameter overload
//{
//    if (c != nullptr)
//        listAll(c, "");
//}
//
//int main()
//{
//    Class* d1 = new Class("HardcoreProtester");
//    listAll(d1);
//    cout << "====" << endl;
//    d1->add(new Class("ExtremeProtester"));
//    Class* d2 = new Class("Protester");
//    d2->add(new Class("RegularProtester"));
//    d2->add(d1);
//    Class* d3 = new Class("ActivatingObject");
//    d3->add(new Class("OilBarrel"));
//    d3->add(new Class("GoldNugget"));
//    d3->add(new Class("SonarKit"));
//    listAll(d3);
//    cout << "====" << endl;
//    Class* d4 = new Class("Actor");
//    d4->add(d2);
//    d4->add(new Class("Squirt"));
//    d4->add(d3);
//    listAll(d4);
//    delete d4;
//}
//
