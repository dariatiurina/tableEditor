/**
 * @file operators.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Implementation of class Operators
 * @version 1.0
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef OPERATORS_CPP
#define OPERATORS_CPP
#include "operators.h"
#include "../help/help.h"
#include <sstream>
#include <iostream>
#include <algorithm>

Operators::Operators() {}

Operators::~Operators() {}

void Operators::convertLine(const std::string &src)
{
    //Classical Shunting-Yard
    std::stack<std::pair<std::string, int>> m_Stack;
    if (!checkFormula(src))
        throw std::logic_error("Not correct formula");
    std::stringstream os;
    os.str(src);
    std::string tmp;
    short int checkNum = 0;
    while (std::getline(os, tmp, ' '))
    {
        std::string op = tmp;
        std::transform(op.begin(), op.end(), op.begin(), ::tolower);
        if (tmp.empty())
            continue;
        if (isOperation(op))
        {
            checkNum = 0;
            if (op == "(")
            {
                checkNum = 0;
                m_Stack.push(std::pair<std::string, int>("(", -10));
                continue;
            }
            else if (op == ")")
            {
                checkNum = 0;
                bool check = false;
                while (!m_Stack.empty() && !check)
                {
                    if (m_Stack.top().first == "(")
                        check = true;
                    else
                        m_Numbers.push_back(m_Stack.top().first);
                    m_Stack.pop();
                }
                if (!m_Stack.empty())
                {
                    if (m_Stack.top().first == "sin" || m_Stack.top().first == "cos" || m_Stack.top().first == "sqrt")
                    {
                        m_Numbers.push_back(m_Stack.top().first);
                        m_Stack.pop();
                    }
                }
                continue;
            }
            if (op == "+" || op == "-")
            {
                if (m_Stack.empty())
                {
                    m_Stack.push(std::pair<std::string, int>(op, 1));
                    continue;
                }
                if (m_Stack.top().second >= 1)
                {
                    m_Numbers.push_back(m_Stack.top().first);
                    m_Stack.pop();
                }
                m_Stack.push(std::pair<std::string, int>(op, 1));
            }
            else if (op == "/" || op == "*")
            {
                if (m_Stack.empty())
                {
                    m_Stack.push(std::pair<std::string, int>(op, 2));
                    continue;
                }
                if (m_Stack.top().second >= 2)
                {
                    m_Numbers.push_back(m_Stack.top().first);
                    m_Stack.pop();
                }
                m_Stack.push(std::pair<std::string, int>(op, 2));
            }
            else
            {
                checkNum = 0;
                if (m_Stack.empty())
                {
                    m_Stack.push(std::pair<std::string, int>(op, 0));
                    continue;
                }
                m_Stack.push(std::pair<std::string, int>(op, 0));
            }
        }
        else
        {
            //If two operands one after another => put them together with space in between
            if (checkNum == -1)
            {
                m_Numbers[m_Numbers.size() - 1] += " ";
                m_Numbers[m_Numbers.size() - 1] += tmp;
            }
            else
                m_Numbers.push_back(tmp);
            checkNum = -1;
        }
    }
    while (!m_Stack.empty())
    {
        m_Numbers.push_back(m_Stack.top().first);
        m_Stack.pop();
    }
}

std::vector<std::string> Operators::returnLine()
{
    return m_Numbers;
}

#endif