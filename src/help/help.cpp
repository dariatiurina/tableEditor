/**
 * @file help.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Helping functions for classes
 * @version 1.0
 * @date 2023-05-18
 *
 * @copyright Copyright (c) 2023
 */
#ifndef HELP_CPP
#define HELP_CPP
#include <string>
#include <stdexcept>
#include <sstream>
#include <deque>
#include <cmath>
#include "help.h"

bool detectIfIsRange(const std::string &line)
{
    std::string column, row, column2, row2;
    bool valid = true;
    bool hasColon = false;
    size_t lastInd = 0;
    for (size_t i = 0; i < line.size() && valid; i++)
    {
        if (isalpha(line[i]))
            column += line[i];
        else if (isdigit(line[i]))
            row += line[i];
        else if (line[i] == ':')
        {
            hasColon = true;
            lastInd = i;
            break;
        }
        else
            valid = false;
    }

    for (size_t i = lastInd + 1; i < line.size() && valid; i++)
    {
        if (isalpha(line[i]))
            column2 += line[i];
        else if (isdigit(line[i]))
            row2 += line[i];
        else
            valid = false;
    }

    if (column + row + ":" + column2 + row2 != line || row.empty() || column.empty() || row2.empty() || column2.empty())
        valid = false;

    return (valid && hasColon);
}

bool detectIfIsCell(const std::string &line)
{
    std::string column, row;
    bool valid = true;
    for (size_t i = 0; i < line.size() && valid; i++)
    {
        if (isalpha(line[i]))
            column += line[i];
        else if (isdigit(line[i]))
            row += line[i];
        else
            valid = false;
    }

    if (column + row != line || column.empty() || row.empty())
        valid = false;

    return valid;
}

std::pair<int, int> translateCell(const std::string &line)
{
    int rowId = 0, columnId = 0;
    std::string row, column;
    std::pair<int, int> ret;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (isalpha(line[i]))
            column += line[i];
        else if (isdigit(line[i]))
            row += line[i];
    }

    try
    {
        rowId = stoi(row) - 1;
    }
    catch (const std::exception &ex)
    {
        throw std::out_of_range("Cell Index is out of range");
    }

    for (size_t i = 0; i < column.size(); i++)
    {
        columnId += ((int)column[i] - 96) * (int)pow(26, (int)(column.size() - i - 1));
    }
    columnId--;
    ret.first = rowId;
    ret.second = columnId;

    return ret;
}

bool isOperation(const std::string &line)
{
    if (line == "+" || line == "-" || line == "*" || line == "/" || line == "(" || line == ")" || isFunc(line))
        return true;
    return false;
}

bool checkFormula(const std::string &line)
{
    size_t numberOfClosing = 0, numberOfOpenning = 0;
    std::stringstream os;
    os.str(line);
    std::string tmp;
    while (std::getline(os, tmp, ' '))
    {
        if (tmp == "(")
            numberOfOpenning++;
        else if (tmp == ")")
            numberOfClosing++;
    }
    if (numberOfClosing != numberOfOpenning)
        return false;
    return true;
}

bool isNum(const std::string &src)
{
    bool hasLetter = false;
    bool alreadyHadDot = false;
    for (size_t i = 0; i < src.size(); i++)
    {
        if ((!isdigit(src[i]) && src[i] != '.' && src[i] != '-') || (alreadyHadDot && src[i] == '.') || (src[i] == '-' && i != 0))
            hasLetter = true;
        if (src[i] == '.')
            alreadyHadDot = true;
    }
    return !hasLetter;
}

void translateRow(std::ostream &os, const size_t &row, const size_t &column)
{
    std::deque<int> pow27(0);
    int num = (int)column;
    for (int j = 0; num > 0; j++)
    {
        pow27.push_back(num % 26);
        num = num / 26;
    }
    if (pow27.empty())
        pow27.push_back(0);

    std::string output;
    for (size_t j = 0; j < pow27.size(); j++)
    {
        output.insert(output.begin(), (char)(pow27[j] - (int)j + 65));
    }
    os << output << row;
}

bool isFunc(const std::string &line)
{
    if (line == "cos" || line == "sin" || line == "sqrt")
        return true;
    return false;
}

#endif // HELP_CPP