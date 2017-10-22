//
//  linear.cpp
//  Homework 3
//
//  Created by Agustin Marinovic on 2/6/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cassert>
using namespace std;

//bool somePredicate(double x)
//{
//    return x < 10.5; // include <cmath> for std::sin, etc.
//}



bool anyTrue(const double a[], int n)
{

    if (n <= 0)     //If the index is 0 or less, return false to the recurrsion
        return false;
    else
    {
        if (somePredicate(a[n-1]))      //Check if the last item in the arra satisfies
        {
            return true;
        }
        else        //Nope, call the recursion again
        {
            return anyTrue(a, n-1);
        }
    }
    
    return false;  // This is not always correct.
}


int countTrue(const double a[], int n)
{
    if (n <= 0)     //Stop if n is not natural
        return 0;
    int omega = countTrue(a, n-1);      //Call for recurrsion, how many below current
    if (somePredicate(a[n-1]))      //Predicate true, add 1
        omega++;

    return omega;
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if (n <= 0)     //Stop if n is not natural
        return -1;
    int omega = countTrue(a, n-1);
    
    if (omega > 0)
        return omega;       //*********CHECK IF SUBSCRIPT IS SAME AS INDEX!!!!
    else if (somePredicate(a[n-1]))
        return n-1;
    else
        return -1;
}

// Return the subscript of the smallest element in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n <= 0)     //Stop if n is not natural
        return -1;
    int omega = indexOfMin(a, n-1);
    
    if (n == 1)
        return 0;
    else if (a[omega] <= a[n])
        return omega;
    else
        return n;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return true;
    if (n1 <= 0)
        return a1[n1]==a2[n2];
    
    
    if (a1[n1-1] == a2[n2-1])
    {
        return includes(a1, n1-1, a2, n2-1);
    }
    else
    {
        return includes(a1, n1-1, a2, n2);
    }
    
    return false;
    
}


