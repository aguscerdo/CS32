//
//  main.cpp
//  Homework 3
//
//  Created by Agustin Marinovic on 2/5/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

//Your declarations and implementations would go here

class Landmark
{
public:
    Landmark(string name);
    virtual ~Landmark();
    virtual string color() const;
    virtual string icon() const = 0;
    string name() const;
private:
    string m_color;
    string m_icon;
    string m_name;
};

class Hotel: public Landmark
{
public:
    Hotel(string name);
    ~Hotel();
    string icon() const;
    //string color() const;
};

class Restaurant: public Landmark
{
public:
    Restaurant(string name, int capacity);
    ~Restaurant();
    string icon() const;
    //string color() const;
private:
    int m_capacity;
};

class Hospital: public Landmark
{
public:
    Hospital(string name);
    ~Hospital();
    string icon() const;
    string color() const;
};

//------------------------------------------------------------------------------------------------------------------------------------//
//CONSTRUCTORS / Destructor

Landmark::Landmark(string name): m_name(name) {}

Landmark::~Landmark() {cout << m_name << "." << endl;}

Hotel::Hotel(string name): Landmark(name) {}

Hotel::~Hotel() {cout << "Destroying the hotel ";}

Restaurant::Restaurant(string name, int capacity): Landmark(name), m_capacity(capacity) {}

Restaurant::~Restaurant() {cout << "Destroying the restaurant ";}

Hospital::Hospital(string name): Landmark(name) {}

Hospital::~Hospital() {cout << "Destroying the hospital ";}



//RETURNERS

string Landmark::name() const {return m_name;}

string Restaurant::icon() const
{
    if (m_capacity < 40)
        return "small knife/fork";
    else
        return "large knife/fork";
}

string Hospital::icon() const
{
    return "H";
}

string Hotel::icon() const
{
    return "bed";
}

string Landmark::color() const
{
    return "yellow";
}

string Hospital::color() const
{
    return "blue";
}

//void display(const Landmark* lm)
//{
//    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
//    << lm->name() << "." << endl;
//}
//
//int main()
//{
//    Landmark* landmarks[4];
//    landmarks[0] = new Hotel("Westwood Rest Good");
//    // Restaurants have a name and seating capacity.  Restaurants with a
//    // capacity under 40 have a small knife/fork icon; those with a capacity
//    // 40 or over have a large knife/fork icon.
//    landmarks[1] = new Restaurant("Bruin Bite", 30);
//    landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
//    landmarks[3] = new Hospital("UCLA Medical Center");
//    
//    cout << "Here are the landmarks." << endl;
//    for (int k = 0; k < 4; k++)
//        display(landmarks[k]);
//    
//    // Clean up the landmarks before exiting
//    cout << "Cleaning up." << endl;
//    for (int k = 0; k < 4; k++)
//        delete landmarks[k];
//}