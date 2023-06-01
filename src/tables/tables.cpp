/**
 * @file tables.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Implementation of Tables class
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef TABLES_CPP
#define TABLES_CPP
#include "tables.h"
#include "../line/line.h"
#include "../graph/graph.h"
#include "../help/help.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
#include <deque>
#include <cmath>

Tables::Tables() : m_Table(0), maxLineSize(0) {}

Tables::Tables(const Tables &src) : m_Table(src.m_Table) {}

Tables::~Tables() {}

void Tables::changeSize(const int &newSize)
{
    if ((size_t)newSize > m_Table.size())
    {
        m_Table.resize(newSize);
    }
    this->changeLineSize((int)maxLineSize);
}

void Tables::changeLineSize(const int &newSize)
{
    if ((int)maxLineSize < newSize)
        maxLineSize = newSize;
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        m_Table[i].changeSize(newSize);
    }
}

void Tables::setValue(const int &row, const int &column, const std::string &input)
{
    this->changeSize(row + 1);
    this->changeLineSize(column + 1);
    Cell *newCell = m_Table[row].getCell(column);
    if (newCell != nullptr)
    {
        if (newCell->whatIs() == "CellFunc")
        {
            for (size_t i = 0; i < m_Formula.size(); i++)
            {
                if (m_Formula[i].first == row && m_Formula[i].second == column)
                {
                    m_Formula.erase(m_Formula.begin() + i);
                    break;
                }
            }
        }
    }
    m_Table[row].setValue(column, input);
}

void Tables::copyValue(const int &row1, const int &column1, const int &row2, const int &column2)
{
    if (row2 >= (int)m_Table.size() || column2 >= (int)maxLineSize)
        throw std::logic_error("Source cell is empty");
    Cell *src = m_Table[row2].getCell(column2);
    if (src == nullptr)
        throw std::logic_error("Source cell is empty");

    this->changeSize(row1 + 1);
    this->changeLineSize(column1 + 1);

    if (src->whatIs() == "CellFunc")
    {
        std::stringstream s;
        src->printFunc(s);
        this->addFormula(row1, column1, s.str());
        return;
    }

    std::stringstream os;
    src->print(os);
    this->setValue(row1, column1, os.str());
}

void Tables::printInd(const size_t &lineLenth, const std::vector<size_t> &CellWidth, int startWith)
{
    for (size_t i = startWith; i < lineLenth; i++)
    {
        std::deque<int> pow27(0);
        int num = (int)i;
        for (int j = 0; num > 0; j++)
        {
            pow27.push_back(num % 26);
            num = num / 26;
        }
        if (pow27.empty())
            pow27.push_back(0);

        std::cout << "|" << std::setw((int)CellWidth[i]);
        std::string output;
        for (size_t j = 0; j < pow27.size(); j++)
        {
            output.insert(output.begin(), (char)(pow27[j] - (int)j + 65));
        }
        std::cout << output;
    }
    std::cout << "|" << std::endl;
}

void Tables::exportTable(std::ofstream &outFile) const
{
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        m_Table[i].exportLine(outFile);
        outFile << std::endl;
    }
    outFile << "Function:" << std::endl;
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        m_Table[i].exportLineFunc(outFile);
        outFile << std::endl;
    }
}

void Tables::importTable(std::ifstream &inFile)
{
    this->deleteAll();
    std::string line;
    bool formula = false;
    int row = 0;
    while (std::getline(inFile, line))
    {
        std::stringstream sstream;
        sstream.str(line);
        std::string value;
        if (line == "Function:")
        {
            formula = true;
            row = 0;
            continue;
        }
        if (!formula)
            m_Table.push_back(Line());
        int ind = 0;
        row++;
        while (std::getline(sstream, value, ','))
        {
            ind++;
            if (!formula)
            {
                value.erase(value.begin());
                value.erase(value.end() - 1);
                m_Table[m_Table.size() - 1].changeSize(ind);
                if (!value.empty())
                {
                    this->setValue((int)m_Table.size() - 1, ind - 1, value);
                }
            }
            else
            {
                value.erase(value.begin());
                value.erase(value.end() - 1);
                if (!value.empty())
                {
                    m_Table[row - 1].setFormula(ind - 1, value);
                    m_Formula.push_back(std::pair<int, int>(row - 1, ind - 1));
                }
            }
        }
    }
}

void Tables::printLine(const size_t &fullSize, const size_t &maxInd)
{
    for (size_t j = 0; j < fullSize + maxInd + 3; j++)
        std::cout << '-';
    std::cout << std::endl;
}

void Tables::printTable(bool function) const
{
    if (maxLineSize == 0)
    {
        std::cout << "|-> EMPTY TABLE" << std::endl;
        return;
    }

    //Get size of a current opened terminal
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    std::vector<size_t> CellWidth(maxLineSize);
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        for (size_t j = 0; j < maxLineSize; j++)
        {
            if (CellWidth[j] < m_Table[i].getCellWidth(j))
                CellWidth[j] = m_Table[i].getCellWidth(j);
        }
    }

    size_t fullSize = 1;
    for (size_t i = 0; i < maxLineSize; i++)
        fullSize += CellWidth[i] + 1;

    size_t maxInd = (int)m_Table.size();
    maxInd = std::to_string(maxInd).length();
    if (maxInd < 3)
        maxInd = 3;

    size_t size = w.ws_col;
    if (size <= fullSize + maxInd + 3)
    {
        std::cout << "PRINT MAY BE INCORRECT. CHOOSE EXPORT FUNCTION" << std::endl;
        return;
    }

    std::cout << "|-> YOUR TABLE:" << std::endl;
    Tables::printLine(fullSize, maxInd);
    std::cout << "|" << std::setw((int)maxInd + 1) << "IND" << "|";
    Tables::printInd(maxLineSize, CellWidth);

    for (size_t j = 0; j < fullSize + maxInd + 3; j++)
        std::cout << '=';
    std::cout << std::endl;

    for (size_t i = 0; i < m_Table.size(); i++)
    {
        std::cout << "|" << std::setw((int)maxInd + 1) << i + 1 << "|";
        m_Table[i].print(std::cout, CellWidth);
        Tables::printLine(fullSize, maxInd);
    }

    if (function)
    {
        std::cout << "FUNCTIONS:" << std::endl;
        for (size_t i = 0; i < m_Table.size(); i++)
        {
            if (m_Table[i].hasFormula())
            {
                m_Table[i].printFormula(i);
            }
        }
    }
}

bool Tables::isEmpty() const
{
    if (m_Table.empty())
        return true;
    return false;
}

void Tables::printCell(const int &row1, const int &column1, bool function) const
{
    if (row1 >= (int)m_Table.size() || column1 >= (int)maxLineSize)
        throw std::out_of_range("Cell is empty");

    Cell *src = m_Table[row1].getCell(column1);
    if (src == nullptr)
        throw std::out_of_range("Cell is empty");

    std::cout << "|-> DATA = ";
    src->print(std::cout);
    if (src->whatIs() == "CellFunc" && function)
    {
        std::cout << " || FORMULA = ";
        src->printFunc(std::cout);
    }
    std::cout << std::endl;
}

void Tables::printRange(const int &row1, const int &column1, const int &row2, const int &column2, bool function) const
{
    if (row2 >= (int)m_Table.size() || column2 >= (int)maxLineSize || row1 >= (int)m_Table.size() || column1 >= (int)maxLineSize)
        throw std::logic_error("Range is bigger than table itself");

    if (row2 < row1 || column2 < column1)
        throw std::logic_error("Starting cell's index is smaller than a ending's cell index");

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    std::vector<size_t> CellWidth(maxLineSize);
    for (int i = row1; i <= row2; i++)
    {
        for (int j = column1; j <= column2; j++)
        {
            if (CellWidth[j] < m_Table[i].getCellWidth(j))
                CellWidth[j] = m_Table[i].getCellWidth(j) + 1;
        }
    }

    size_t fullSize = 1;
    for (int i = column1; i <= column2; i++)
        fullSize += CellWidth[i] + 1;

    size_t size = w.ws_col;
    if (size < fullSize)
    {
        std::cout << "PRINT MAY BE INCORRECT. CHOOSE EXPORT FUNCTION" << std::endl;
        return;
    }

    size_t maxInd = std::to_string(row2).length();
    if (maxInd < 3)
        maxInd = 3;

    std::cout << "|-> YOUR TABLE:" << std::endl;
    Tables::printLine(fullSize, maxInd);
    std::cout << "|" << std::setw((int)maxInd + 1) << "IND"
              << "|";
    Tables::printInd(column2 + 1, CellWidth, (size_t)column1);

    for (size_t j = 0; j < fullSize + maxInd + 3; j++)
        std::cout << '=';
    std::cout << std::endl;

    for (int i = row1; i <= row2; i++)
    {
        std::cout << "|" << std::setw((int)maxInd + 1) << i + 1 << "|";
        m_Table[i].printRange(std::cout, CellWidth, column1, column2);
        Tables::printLine(fullSize, maxInd);
    }

    if (function)
    {
        std::cout << "FUNCTIONS:" << std::endl;
        for (int i = row1; i <= row2; i++)
        {
            if (m_Table[i].hasFormula())
            {
                m_Table[i].printFormula(i, column1, column2);
            }
        }
    }
}

void Tables::deleteAll()
{
    this->m_Table.clear();
    this->m_Formula.clear();
    this->maxLineSize = 0;
}

void Tables::deleteEmpty()
{
    for (int i = (int)m_Table.size() - 1; i >= 0; i--)
    {
        if (m_Table[i].isEmpty())
            m_Table.erase(m_Table.begin() + i);
        else
            break;
    }

    for (size_t i = 0; i < m_Table.size(); i++)
        m_Table[i].delEmpty();

    maxLineSize = 0;
    for (size_t i = 0; i < m_Table.size(); i++)
        if (maxLineSize < m_Table[i].getSize())
            maxLineSize = m_Table[i].getSize();

    this->changeLineSize((int)maxLineSize);
}

void Tables::deleteDepended(const int &row1, const int &column1)
{
    std::stringstream s;
    translateRow(s, row1 + 1, column1);
    std::string cell = s.str();
    std::transform(cell.begin(), cell.end(), cell.begin(), ::tolower);
    for (size_t i = 0; i < m_Table.size(); i++)
    {
        std::vector<int> depend = m_Table[i].deleteDepend(cell);
        for (size_t j = 0; j < depend.size(); j++)
        {
            Cell *delCell = m_Table[i].getCell(depend[j]);
            if (delCell == nullptr)
                continue;
            this->deleteCell((int)i, depend[j]);
        }
    }
}

void Tables::deleteCell(const int &row1, const int &column1)
{
    if (row1 >= (int)m_Table.size() || column1 >= (int)maxLineSize)
        throw std::out_of_range("Cell is empty");

    Cell *src = m_Table[row1].getCell(column1);
    if (src == nullptr)
        throw std::out_of_range("Cell is empty");

    m_Table[row1].delCell(column1);
    this->deleteDepended(row1, column1);
    for (size_t i = 0; i < m_Formula.size(); i++)
        if (m_Formula[i].first == row1 && m_Formula[i].second == column1)
        {
            m_Formula.erase(m_Formula.begin() + i);
            break;
        }
    this->deleteEmpty();
}

void Tables::deleteRange(const int &row1, const int &column1, const int &row2, const int &column2)
{
    if (row2 >= (int)m_Table.size() || column2 >= (int)maxLineSize || row1 >= (int)m_Table.size() || column1 >= (int)maxLineSize)
        throw std::logic_error("Range is bigger than table itself");

    if (row2 < row1 || column2 < column1)
        throw std::logic_error("Starting cell's index is smaller than a ending's cell index");

    for (int i = row1; i <= row2; i++)
    {
        for (int j = column1; j <= column2; j++)
            m_Table[i].delCell(j);
    }
    this->deleteEmpty();
}

bool Tables::checkCycle()
{
    std::vector<std::pair<int, std::string>> indFunc;
    for (size_t i = 0; i < m_Formula.size(); i++)
    {
        std::stringstream s;
        translateRow(s, m_Formula[i].first + 1, m_Formula[i].second);
        std::string tran = s.str();
        std::transform(tran.begin(), tran.end(), tran.begin(), ::tolower);
        indFunc.push_back(std::pair<int, std::string>(i, tran));
    }

    Graph g((int)m_Formula.size());
    for (size_t i = 0; i < indFunc.size(); i++)
    {
        std::vector<std::string> formula = m_Table[m_Formula[i].first].getCell(m_Formula[i].second)->getFormula();
        for (size_t j = 0; j < formula.size(); j++)
            if (detectIfIsCell(formula[j]))
            {
                for (size_t k = 0; k < indFunc.size(); k++)
                {
                    if (indFunc[k].second == formula[j])
                        g.addEdge((int)i, (int)k);
                }
            }
    }
    return g.isCyclic();
}

std::vector<int> Tables::topoSort()
{
    std::vector<std::pair<int, std::string>> indFunc;
    for (size_t i = 0; i < m_Formula.size(); i++)
    {
        std::stringstream s;
        translateRow(s, m_Formula[i].first + 1, m_Formula[i].second);
        std::string tran = s.str();
        std::transform(tran.begin(), tran.end(), tran.begin(), ::tolower);
        indFunc.push_back(std::pair<int, std::string>(i, tran));
    }

    Graph g((int)m_Formula.size());
    for (size_t i = 0; i < indFunc.size(); i++)
    {
        std::vector<std::string> formula = m_Table[m_Formula[i].first].getCell(m_Formula[i].second)->getFormula();
        for (size_t j = 0; j < formula.size(); j++)
            if (detectIfIsCell(formula[j]))
            {
                for (size_t k = 0; k < indFunc.size(); k++)
                {
                    if (indFunc[k].second == formula[j])
                        g.addEdge((int)i, (int)k);
                }
            }
    }
    return g.topologicalSort();
}

void Tables::addFormula(const int &row, const int &column, const std::string &src)
{
    this->changeSize(row + 1);
    this->changeLineSize(column + 1);
    m_Table[row].setValueFormula(column, src);
    Cell *newCell = m_Table[row].getCell(column);
    std::vector<std::string> formula = newCell->getFormula();
    for (size_t i = 0; i < formula.size(); i++)
    {
        if (detectIfIsCell(formula[i]))
        {
            std::pair<int, int> cord = translateCell(formula[i]);
            if ((cord.first == row && cord.second == column))
            {
                deleteCell(row, column);
                throw std::logic_error("Cell formula cannot content itself");
            }
            else if (((size_t)cord.first >= m_Table.size() || (size_t)cord.second >= this->maxLineSize))
            {
                deleteCell(row, column);
                throw std::logic_error("Cell in formula doesn't exist");
            }
            Cell *check = m_Table[cord.first].getCell(cord.second);
            if (check == nullptr)
            {
                deleteCell(row, column);
                std::transform(formula[i].begin(), formula[i].end(), formula[i].begin(), ::toupper);
                throw std::logic_error(formula[i] + " is empty");
            }
        }
    }
    for (size_t i = 0; i < m_Formula.size(); i++)
    {
        if (m_Formula[i].first == row && m_Formula[i].second == column)
        {
            m_Formula.erase(m_Formula.begin() + i);
            break;
        }
    }
    m_Formula.push_back(std::pair<int, int>(row, column));
    if (checkCycle())
    {
        m_Formula.pop_back();
        m_Table[row].delCell(column);
        setValue(row, column, "0");
        std::stringstream s;
        translateRow(s, row + 1, column);
        throw std::logic_error("Cycle detected. " + s.str() + "'s value is set to 0");
    }
    this->updateInsideFormula();
}

std::string Tables::operation(const std::string &operation, const std::string &firstOp, const std::string &secondOp)
{
    std::string res;
    Cell *src1 = nullptr, *src2 = nullptr;
    if (detectIfIsCell(firstOp))
    {
        std::pair<int, int> cellCoordinates = translateCell(firstOp);
        src1 = m_Table[cellCoordinates.first].getCell(cellCoordinates.second);
    }
    if (detectIfIsCell(secondOp))
    {
        std::pair<int, int> cellCoordinates = translateCell(secondOp);
        src2 = m_Table[cellCoordinates.first].getCell(cellCoordinates.second);
    }

    if (src1 == nullptr && src2 == nullptr)
    {
        bool isNumbers;
        if (isNum(firstOp))
            isNumbers = true;
        if (isNumbers)
        {
            NumCell src;
            src.setValue(std::stod(firstOp));
            res = src.operation(operation, secondOp, true);
        }
        else
        {
            StringCell src;
            src.setValue(firstOp);
            res = src.operation(operation, secondOp, true);
        }
    }
    else if (src1 != nullptr && src2 == nullptr)
    {
        res = src1->operation(operation, secondOp, true);
    }
    else if (src2 != nullptr && src1 == nullptr)
    {
        res = src2->operation(operation, firstOp, false);
    }
    else
    {
        std::stringstream s;
        src2->print(s);
        std::string operand = s.str();
        res = src1->operation(operation, operand, true);
    }

    return res;
}

std::string Tables::function(const std::string &operation, const std::string &firstOp)
{
    std::string res;
    Cell *src1 = nullptr;
    if (detectIfIsCell(firstOp))
    {
        std::pair<int, int> cellCoordinates = translateCell(firstOp);
        src1 = m_Table[cellCoordinates.first].getCell(cellCoordinates.second);
    }

    if (src1 == nullptr)
    {
        if (isNum(firstOp))
        {
            NumCell src;
            src.setValue(std::stod(firstOp));
            res = src.function(operation);
        }
        else
        {
            throw std::logic_error("Cannot execute " + operation + " on a line");
        }
    }
    else
    {
        res = src1->function(operation);
    }
    return res;
}

void Tables::updateInsideFormula()
{
    std::vector<int> arr = this->topoSort();
    for (int i = (int)arr.size() - 1; i >= 0; i--)
    {
        int k = arr[i];
        Cell *newCell = m_Table[m_Formula[k].first].getCell(m_Formula[k].second);
        std::vector<std::string> formula = newCell->getFormula();
        for (size_t j = 0; j < formula.size(); j++)
        {
            if (isOperation(formula[j]))
            {
                if (isFunc(formula[j]))
                {
                    if (j == 0)
                    {
                        formula.insert(formula.begin() + j, "0");
                        j++;
                    }
                    try
                    {
                        formula[j] = this->function(formula[j], formula[j - 1]);
                        formula.erase(formula.begin() + j - 1);
                    }
                    catch (const std::exception &ex)
                    {
                        this->deleteCell(m_Formula[k].first, m_Formula[k].second);
                        throw std::logic_error(ex.what());
                    }
                    continue;
                }
                if (j < 2)
                {
                    if (j == 0)
                    {
                        this->deleteCell(m_Formula[k].first, m_Formula[k].second);
                        throw std::logic_error("Not correct formula");
                    }
                    while (j < 2)
                    {
                        formula.insert(formula.begin() + j, " ");
                        j++;
                    }
                }
                try
                {
                    formula[j] = this->operation(formula[j], formula[j - 2], formula[j - 1]);
                }
                catch (const std::exception &ex)
                {
                    this->deleteCell(m_Formula[k].first, m_Formula[k].second);
                    throw std::logic_error(ex.what());
                }
                formula.erase(formula.begin() + j - 2, formula.begin() + j);
                j = 0;
            }
        }
        newCell->setInside(formula[0]);
    }
}
#endif // TABLES_CPP