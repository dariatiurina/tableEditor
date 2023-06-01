/**
 * @file cell.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Implementation of a Cell, NumCell and StringCell
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CELL_CPP
#define CELL_CPP
#include "cell.h"
#include "../help/help.h"
#include "../operators/operators.h"
#include <cmath>
#include <sstream>

Cell::Cell() {}

Cell::~Cell() {}

void Cell::setInside(const std::string &src)
{
    std::string copy = src;
    return;
}

void Cell::print(std::ostream &os) const
{
    os << "";
}

void Cell::printFunc(std::ostream &os) const
{
    os << "";
}

std::ostream &operator<<(std::ostream &os, const Cell &src)
{
    src.print(os);
    return os;
}

size_t Cell::getLength() const
{
    return 0; //!< Cell is default empty Cell => lentgh is 0
}

std::vector<std::string> Cell::getFormula() const
{
    return std::vector<std::string>();
}

std::string Cell::whatIs() const
{
    return "Cell";
}

std::string Cell::operation(const std::string &operation, const std::string &operand, const bool &isFirst) const
{
    return operation + operand + std::to_string(isFirst);
}

std::string Cell::function(const std::string &operation) const
{
    return operation;
}

NumCell::NumCell() : Cell(), m_Inside(0) {}

NumCell::NumCell(const NumCell &src) : Cell(), m_Inside(src.m_Inside) {}

NumCell::NumCell(const int &src) : Cell(), m_Inside(src) {}

NumCell::~NumCell() {}

size_t NumCell::getLength() const
{
    double intpart, fpart;
    fpart = modf(m_Inside, &intpart);
    long int intp = (long int)intpart;
    if (fpart != 0)
        return (std::to_string(intp) + std::to_string(fpart)).length() - 1;

    return std::to_string(intp).length();
}

double NumCell::getValue()
{
    return m_Inside;
}

void NumCell::print(std::ostream &os) const
{
    double intpart, fpart;
    fpart = modf(m_Inside, &intpart);
    if (fpart != 0)
    {
        os << m_Inside;
    }
    else
    {
        long int intp = (long int)intpart;
        os << intp;
    }
}

void NumCell::setValue(const double &newValue)
{
    m_Inside = newValue;
}

std::string NumCell::whatIs() const
{
    return "NumCell";
}

std::string NumCell::operation(const std::string &operation, const std::string &operand, const bool &isFirst) const
{
    std::string ret;
    if (isNum(operand))
    {
        double num = std::stod(operand);
        if (operation == "+")
            ret = std::to_string(num + m_Inside);
        else if (operation == "-" && isFirst)
            ret = std::to_string(m_Inside - num);
        else if (operation == "-" && !isFirst)
            ret = std::to_string(num - m_Inside);
        else if (operation == "*")
            ret = std::to_string(num * m_Inside);
        else if (operation == "/" && isFirst)
            ret = std::to_string(m_Inside / num);
        else if (operation == "/" && !isFirst)
            ret = std::to_string(num / m_Inside);
    }
    else
    {
        std::stringstream s;
        this->print(s);
        std::string thisCell = s.str();
        if (operation == "+" && !isFirst)
            ret = operand + thisCell;
        else if (operation == "+" && isFirst)
            ret = thisCell + operand;
        else if (operation == "-" && isFirst)
            ret = operand;
        else if (operation == "-" && !isFirst)
            ret = thisCell;
        else if (operation == "*")
        {
            for (int i = 0; i < (int)m_Inside; i++)
                ret += operand;
        }
        else
            throw std::logic_error("Cannot execute " + operation + " on a line");
    }
    return ret;
}

std::string NumCell::function(const std::string &operation) const
{
    std::string res;
    if (operation == "sin")
        res = std::to_string(::sin(m_Inside));
    else if (operation == "cos")
        res = std::to_string(::cos(m_Inside));
    else if (operation == "sqrt")
    {
        if (m_Inside < 0)
            throw std::logic_error("Cannot execute sqrt on a negative number");
        res = std::to_string(::sqrt(m_Inside));
    }
    return res;
}

StringCell::StringCell() : Cell(), m_Inside("") {}

StringCell::StringCell(const StringCell &src) : Cell(), m_Inside(src.m_Inside) {}

StringCell::~StringCell() {}

size_t StringCell::getLength() const
{
    return m_Inside.size();
}

void StringCell::print(std::ostream &os) const
{
    os << m_Inside;
}

void StringCell::setValue(const std::string &newValue)
{
    m_Inside = newValue;
}

std::string StringCell::getValue()
{
    return m_Inside;
}

std::string StringCell::whatIs() const
{
    return "StringCell";
}

std::string StringCell::operation(const std::string &operation, const std::string &operand, const bool &isFirst) const
{
    std::string ret;
    if (isNum(operand))
    {
        double num = std::stod(operand);
        if (operation == "+" && isFirst)
            ret = m_Inside + operand;
        else if (operation == "+" && !isFirst)
            ret = operand + m_Inside;
        else if (operation == "-" && isFirst)
            ret = m_Inside;
        else if (operation == "-" && !isFirst)
            ret = operand;
        else if (operation == "*")
        {
            for (int i = 0; i < (int)num; i++)
                ret += m_Inside;
        }
        else
            throw std::logic_error("Cannot execute" + operation + "on a line");
    }
    else
    {
        if (operation == "+" && isFirst)
            ret = m_Inside + operand;
        else if (operation == "+" && !isFirst)
            ret = operand + m_Inside;
        else if (operation == "-" && isFirst)
            ret = m_Inside;
        else if (operation == "-" && !isFirst)
            ret = operand;
        else
            throw std::logic_error("Cannot execute" + operation + "on a line");
    }
    return ret;
}

std::string StringCell::function(const std::string &operation) const
{
    throw std::logic_error("Cannot execute " + operation + " on a line");
    return operation;
}

CellFunc::CellFunc() : Cell(), m_FormulaPrint(""), m_Inside(""), m_Formula(0) {}

CellFunc::~CellFunc() {}

CellFunc::CellFunc(const CellFunc &src) : Cell(), m_FormulaPrint(src.m_FormulaPrint), m_Inside(src.m_Inside), m_Formula(src.m_Formula) {}

void CellFunc::printFunc(std::ostream &os) const
{
    os << m_FormulaPrint;
}

void CellFunc::print(std::ostream &os) const
{
    if (isNum(m_Inside))
    {
        NumCell cell;
        cell.setValue(std::stod(m_Inside));
        cell.print(os);
        return;
    }
    os << m_Inside;
}

void CellFunc::setFormula(const std::string &line)
{
    m_FormulaPrint = line;
    Operators op;
    op.convertLine(line);
    m_Formula = op.returnLine();
}

std::string CellFunc::whatIs() const
{
    return "CellFunc";
}

size_t CellFunc::getLength() const
{
    if (isNum(m_Inside))
    {
        NumCell cell;
        cell.setValue(std::stod(m_Inside));
        return cell.getLength();
    }
    return m_Inside.length();
}

void CellFunc::setInside(const std::string &src)
{
    m_Inside = src;
}

std::vector<std::string> CellFunc::getFormula() const
{
    return m_Formula;
}

std::string CellFunc::operation(const std::string &operation, const std::string &operand, const bool &isFirst) const
{
    std::string res;
    if (isNum(m_Inside))
    {
        NumCell help;
        help.setValue(std::stod(m_Inside));
        res = help.operation(operation, operand, isFirst);
    }
    else
    {
        StringCell help;
        help.setValue(m_Inside);
        res = help.operation(operation, operand, isFirst);
    }
    return res;
}

std::string CellFunc::function(const std::string &operation) const
{
    std::string res;
    if (isNum(m_Inside))
    {
        NumCell help;
        help.setValue(std::stod(m_Inside));
        res = help.function(operation);
    }
    else
    {
        StringCell help;
        help.setValue(m_Inside);
        res = help.function(operation);
    }
    return res;
}

#endif // CELL_CPP