/**
 * @file line.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Implementation of Line class
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef LINE_CPP
#define LINE_CPP
#include "line.h"
#include "../help/help.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include <deque>

Line::Line() : m_Line(0), maxWidthCell(4) {}

Line::Line(const Line &src) : maxWidthCell(src.maxWidthCell)
{
    this->m_Line.resize(src.m_Line.size());
    for (size_t i = 0; i < m_Line.size(); i++)
    {
        if (src.m_Line[i] == nullptr)
            continue;
        if (src.m_Line[i]->whatIs() == "CellFunc")
        {
            std::stringstream s;
            src.m_Line[i]->printFunc(s);
            this->setFormula((int)i, s.str());
        }
        else
        {
            std::stringstream s;
            src.m_Line[i]->print(s);
            this->setValue((int)i, s.str());
        }
    }
}

Line::~Line()
{
    for (size_t i = 0; i < m_Line.size(); i++)
        delete m_Line[i];
}

size_t Line::getCellWidth(const size_t &ind) const
{
    if (m_Line[ind] == nullptr)
        return maxWidthCell;
    size_t ret = m_Line[ind]->getLength();
    if (ret == 0 || ret < maxWidthCell)
        return maxWidthCell;
    return ret;
}

void Line::exportLine(std::ofstream &of) const
{
    for (size_t i = 0; i < m_Line.size(); i++)
    {
        std::stringstream os;
        if (m_Line[i] != nullptr)
            m_Line[i]->print(os);
        else
            os << "";
        if (i == 0)
        {
            of << "\"" << os.str() << "\"";
        }
        else
        {
            of << ",\"" << os.str() << "\"";
        }
    }
}

void Line::exportLineFunc(std::ofstream &of) const
{
    for (size_t i = 0; i < m_Line.size(); i++)
    {
        std::stringstream os;
        if (m_Line[i] != nullptr)
            m_Line[i]->printFunc(os);
        else
            os << "";
        if (i == 0)
        {
            of << "\"" << os.str() << "\"";
        }
        else
        {
            of << ",\"" << os.str() << "\"";
        }
    }
}

void Line::print(std::ostream &os, const std::vector<size_t> CellWidth) const
{
    if (m_Line.size() > 0)
    {
        os << "|";
        for (size_t i = 0; i < m_Line.size(); i++)
        {
            os << std::setw((int)CellWidth[i]);
            if (m_Line[i] != nullptr)
            {
                m_Line[i]->print(os);
            }
            else
                os << " ";
            os << "|";
        }
    }
    os << std::endl;
}

void Line::printRange(std::ostream &os, const std::vector<size_t> CellWidth, const size_t &column1, const size_t &column2) const
{
    if (m_Line.size() > 0)
    {
        os << "|";
        for (size_t i = column1; i <= column2; i++)
        {
            os << std::setw((int)CellWidth[i]);
            if (m_Line[i] != nullptr)
            {
                m_Line[i]->print(os);
            }
            else
                os << " ";
            os << "|";
        }
    }
    os << std::endl;
}

void Line::changeSize(const size_t &newWidth)
{
    if (newWidth > m_Line.size())
    {
        m_Line.resize(newWidth);
    }
}

size_t Line::getSize() const
{
    return m_Line.size();
}

bool Line::isEmpty()
{
    for (size_t i = 0; i < m_Line.size(); i++)
        if (m_Line[i] != nullptr)
            return false;
    return true;
}

Cell *Line::getCell(const size_t &ind) const
{
    if (ind >= m_Line.size())
        return nullptr;
    return m_Line[ind];
}

void Line::printFormula(const size_t &row, const size_t &column_min, const size_t &column_max) const
{
    size_t max = column_max + 1;
    if (column_max == 0)
        max = m_Line.size();
    for (size_t i = column_min; i < max; i++)
    {
        if (m_Line[i] == nullptr)
            continue;
        std::string res = m_Line[i]->whatIs();
        if (res != "CellFunc")
            continue;
        translateRow(std::cout, row + 1, i);
        std::cout << " = ";
        m_Line[i]->printFunc(std::cout);
        std::cout << std::endl;
    }
}

void Line::setValue(const int &ind, const std::string &newValue)
{
    if (isNum(newValue))
    {
        NumCell *newCell = new NumCell();
        double value = stod(newValue);
        newCell->setValue(value);
        if (m_Line[ind] == nullptr)
            m_Line[ind] = newCell;
        else
        {
            delete m_Line[ind];
            m_Line[ind] = newCell;
        }
    }
    else
    {
        StringCell *newCell = new StringCell();
        newCell->setValue(newValue);
        if (m_Line[ind] == nullptr)
            m_Line[ind] = newCell;
        else
        {
            delete m_Line[ind];
            m_Line[ind] = newCell;
        }
    }
}

void Line::setFormula(const int &column, const std::string &src)
{
    if (m_Line[column] != nullptr)
        delete m_Line[column];
    CellFunc *newCell = new CellFunc();
    newCell->setFormula(src);
    m_Line[column] = newCell;
}

void Line::delEmpty()
{
    for (int i = (int)m_Line.size() - 1; i >= 0; i--)
    {
        if (m_Line[i] == nullptr)
            m_Line.erase(m_Line.begin() + i);
        else
            break;
    }
}

void Line::delCell(const int &ind)
{
    if (m_Line[ind] == nullptr)
        return;

    delete m_Line[ind];
    m_Line[ind] = nullptr;
    this->delEmpty();
}

void Line::setValueFormula(const int &ind, const std::string &newValue)
{
    CellFunc *newCell = new CellFunc();
    try
    {
        newCell->setFormula(newValue);
    }
    catch (const std::exception &ex)
    {
        delete newCell;
        throw std::logic_error(ex.what());
    }

    if (m_Line[ind] != nullptr)
        delete m_Line[ind];
    m_Line[ind] = newCell;
}

bool Line::hasFormula() const
{
    for (size_t i = 0; i < m_Line.size(); i++)
        if (m_Line[i] != nullptr)
        {
            std::string res = m_Line[i]->whatIs();
            if (res == "CellFunc")
                return true;
        }
    return false;
}

std::vector<int> Line::deleteDepend(const std::string &childCell)
{
    std::vector<int> ret;
    for (size_t i = 0; i < m_Line.size(); i++)
    {
        if (m_Line[i] == nullptr)
            continue;
        std::vector<std::string> formula = m_Line[i]->getFormula();
        for (size_t j = 0; j < formula.size(); j++)
        {
            if (formula[j] == childCell)
            {
                ret.push_back((int)i);
                break;
            }
        }
    }
    return ret;
}

#endif // LINE_CPP