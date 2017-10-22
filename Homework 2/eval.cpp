//
//  eval.cpp
//  Homework 2
//
//  Created by Agustin Marinovic on 1/28/16.
//  Copyright © 2016 Agustin Marinovic. All rights reserved.
//

#include <stdio.h>

#include <iostream>
#include <stack>
#include <cassert>
#include "Map.h"
using namespace std;

bool operators(char op);
bool precedence(char nChar, char oChar);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    stack<char> op;
    string infixed = "";
    int nLetters = 0, nSymbols = 0, nOpen = 0, nClose = 0;
    if (infix == "")
        return 1;
    
    for (int k = 0; k < infix.length(); k++)
    {
        if (infix[k] == ' ')
            continue;
        else if (isalpha(infix[k]) && islower(infix[k]))        //Check if all characters are lowercase letters, operators or spaces
        {
            nLetters++;
            continue;
        }
        else if (operators(infix[k]))
        {
            if (infix[k] != '(' && infix[k] != ')')
                nSymbols++;
            else if (infix[k] == '(')
                nOpen++;
            else if (infix[k] == ')')
                nClose++;
            continue;
        }
        //Bad item encountered
        
        cerr << "Encountered: " << infix[k] << ", returning" << endl;
        return 1; // !!!!!!!!!!!!!!!!!!!!!!!!!!
        
    }
    
    if (nSymbols+1 != nLetters || nOpen != nClose)
        return 1;
    

    
    cerr << "Text is good\n";
    
    //--------------------------------------------------// END STRING CHECK
    
    for (int k = 0; k < infix.length(); k++)        //Go through all the letters on the original string
    {
        if (infix[k] == ' ')        //if the char is a space, skip it
            continue;
        
        if (isalpha(infix[k]))      //If the char is an operand, append it to the infixed
            infixed += infix[k];
        else if (infix[k] == ')')       //If the char is a ')'
        {
            while (op.top() != '(')     //Pop all the opperands until you reach a '('
            {
                if (op.top() != '(')
                    infixed += op.top();
                op.pop();
                if(op.empty())      //If the stacks happens to be empty, stop
                    break;
            }
            if (! op.empty())
                op.pop();       //Get rid of the '(' on the stack
        }
        else if (operators(infix[k]))       //If the char is an operator
        {
            if (op.empty())     //Empty stack --> push it
                op.push(infix[k]);

            else if (precedence(infix[k], op.top()))        //If the operator has lower precedence than the top of the stack
            {
                while (op.top() != '(' && precedence(infix[k], op.top()))       //Until you reach a '(', pop all operators with lower precedence and stop
                {
                    if (op.top() != '(')
                        infixed += op.top();
                    op.pop();
                    if(op.empty())
                        break;
                }
                op.push(infix[k]);      //Push the new operator to the stack
            }
            else        //The operator has higher precedence, just push it
            {
                op.push(infix[k]);
            }
            
        }

        
    }       //End for
    while ( ! op.empty())   //Pop the remaining operators
    {
        if (op.top() != '(')
        {
            infixed += op.top();
        }
        op.pop();
    }
    
    postfix = infixed;
    cerr << postfix << endl;
    //-----------------------------------------------------------// END POSTFIXING
    

    
    cerr << infix << "  ---  " << infixed << endl;
    
    for (int k = 0; k < infixed.length(); k++)      //Check for the letters on the map
    {
        if (isalpha(infixed[k]))
            if ( ! values.contains(infixed[k]))     //A char wasn't found, return 2
                return 2;
                
    }
    
    //-------------------------------------------------------------// CHECK IF IN MAP
    
    stack<int> algebra;
    int operand1, operand2;
    int val;
    
    for (int k = 0; k < infixed.length(); k++)
    {
        if (isalpha(infixed[k]))        //If a letter, push it's value from the map onto the stack
        {
            values.get(infixed[k], val);
            algebra.push(val);
        }
        else        //If it's an operand, pop the top 2 values, and do the correct operation. Push the result to the stack
        {
            operand2 = algebra.top();
            algebra.pop();
            operand1 = algebra.top();
            algebra.pop();
            
            switch (infixed[k])
            {
                case '+':
                    val = operand1 + operand2;
                    algebra.push(val);
                    break;
                case '-':
                    val = operand1 - operand2;
                    algebra.push(val);
                    break;
                case '/':
                    if (operand2 != 0)      //No division by 0!!!
                        val = operand1 / operand2;
                    else
                        return 3;
                    algebra.push(val);
                    break;
                case '*':
                    val = operand1 * operand2;
                    algebra.push(val);
                    break;
                    
            }
            cerr << operand1 << " " << infixed[k] << " " << operand2 << " = " << algebra.top() << endl;
        }
            
    }
    
    //cerr << "The final answer is: " << algebra.top() << endl;
    result = algebra.top();
    return 0;
}

bool operators(char op)     //Returns true if the char is an operator, false if not
{
    if (op == '/' || op == '*' || op == '+' || op == '-' || op == '(' || op == ')')
        return true;
    else
        return false;
}

bool precedence(char nChar, char oChar)     //Checks precedence of symbols. Returns false if the new char has lower precedence and true otherwise
{
    if ((nChar == '+' || nChar == '-') && oChar != '(')
        return true;    //Pop
    else if ((nChar == '+' || nChar == '-') && oChar == '(')
        return false;
    
    if ((nChar == '/' || nChar == '*') && (oChar == '+' || oChar == '-'))
        return false;
    else if ((nChar == '/' || nChar == '*') && oChar != '(')
        return true;
    
    if (nChar == '(')
        return false;
    
    return false;
}

int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
    
    //----------------My Testing----------------//
    
    Map omicorn;
    string s0 = "a+b+c/5";
    string sf = ":(";
    int result = 999;

    assert (evaluate(s0, omicorn, sf, result) == 1);
    assert (sf == ":(" && result == 999);
    
    string s1 = "a+b+c+d";
    
    assert (evaluate(s1, omicorn, sf, result) == 2);
    assert(sf == "ab+c+d+" && result == 999);
    
    string s2 = "a*(b+c)/d";
    assert (evaluate(s2, omicorn, sf, result) == 2);
    assert (sf == "abc+*d/");
    
    for (int i = 1; i < 15; i++)
    {
        char a = 'a' + i - 1;
        omicorn.insert(a, i);
    }
    
    omicorn.insert('z', 0);
    
    string s3 = "a/z";
    assert (evaluate(s3, omicorn, sf, result) == 3);
    assert (sf == "az/");
    
    
    string s4 = "f/g*h/(i+j*k/l)";
    
    assert (evaluate(s4, omicorn, sf, result) == 0);
    assert (sf == "fg/h*ijk*l/+/" && result == 0);

    
    cerr << "--- Success! ---" << endl;
}
