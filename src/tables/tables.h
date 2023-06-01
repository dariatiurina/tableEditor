/**
 * @file tables.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of a Tables class
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef TABLES_H
#define TABLES_H

#include <vector>
#include "../cell/cell.h"
#include "../line/line.h"
#include <iostream>

/**
 * @brief Class Tables, which is Tables itself with Cells in them
 */
class Tables
{
public:
    /**
     * @brief Construct a new Tables object
     */
    Tables();

    /**
     * @brief Construct a new Tables object copied from a source Tables
     * @param src Tables which needs to be copied
     */
    Tables(const Tables &src);

    /**
     * @brief Destroy the Tables object
     */
    ~Tables();

    /**
     * @brief Print full table to a console
     */
    void printTable(bool function = false) const;

    /**
     * @brief Print one Cell data to a console
     * @param row1 Row, where Cell is situated
     * @param column1 Column, where Row is situated
     */
    void printCell(const int &row1, const int &column1, bool function = false) const;

    /**
     * @brief Print CellRange to a console
     * @param row1 Row, where starting Cell is situated of a CellRange
     * @param column1 Column, where starting Cell is situated of a CellRange
     * @param row2 Row, where ending Cell is situated of a CellRange
     * @param column2 Column, where ending Cell is situated of a CellRange
     */
    void printRange(const int &row1, const int &column1, const int &row2, const int &column2, bool function = false) const;

    /**
     * @brief Static function, which will print Indexes (upper line with letters)
     * @param lineLenth Number of elements in a Line
     * @param CellWidth std::vector<size_t> with maxWidth of every Column
     * @param startWith starting index
     */
    static void printInd(const size_t &lineLenth, const std::vector<size_t> &CellWidth, int startWith = 0);

    /**
     * @brief Prints line ('-')
     * @param fullSize Number of elements*CellWidth in a Line
     * @param maxInd Max row index in a Table
     */
    static void printLine(const size_t &fullSize, const size_t &maxInd);

    /**
     * @brief Set the value to a Cell in the Table
     * @param row Row, where Cell is situated
     * @param column Column, where Cell is situated
     * @param input Value, which will be set
     */
    void setValue(const int &row, const int &column, const std::string &input);

    /**
     * @brief Exports Table toa  given std::ofstream
     * @param outFile std::ofstream, where Tables ought to be exported to
     */
    void exportTable(std::ofstream &outFile) const;

    /**
     * @brief Import Table from a std::ifstream
     * @param inFile std::ifstream, where source Table is
     */
    void importTable(std::ifstream &inFile);

    /**
     * @brief Delete Table
     */
    void deleteAll();

    /**
     * @brief Delete value from one Cell from a Table
     * @param row1 Row, where Cell is situated
     * @param column1 Column, where Cell is situated
     */
    void deleteCell(const int &row1, const int &column1);

    /**
     * @brief Delete value from a CellRange from a Table
     * @param row1 Row, where starting Cell is situated of a CellRange
     * @param column1 Column, where starting Cell is situated of a CellRange
     * @param row2 Row, where ending Cell is situated of a CellRange
     * @param column2 Column, where ending Cell is situated of a CellRange
     */
    void deleteRange(const int &row1, const int &column1, const int &row2, const int &column2);

    /**
     * @brief Delete empty Lines at the bottom of a Table
     */
    void deleteEmpty();

    /**
     * @brief Detects if Table is empty
     * @return true Table is empty
     * @return false Table has at least one Cell
     */
    bool isEmpty() const;

    /**
     * @brief Copies values from one Cell in a Table to another
     * @param row1 Row, where destination Cell is situated
     * @param column1 Column, destination where Cell is situated
     * @param row2 Row, where source Cell is situated
     * @param column2 Column, source where Cell is situated
     */
    void copyValue(const int &row1, const int &column1, const int &row2, const int &column2);

    /**
     * @brief Change numbers of rows
     * @param newSize new number of Lines
     */
    void changeSize(const int &newSize);

    /**
     * @brief Change numbers of columns
     * @param newSize new number of columns
     */
    void changeLineSize(const int &newSize);

    /**
     * @brief Add CellFunc in a Table
     * 
     * @param row row's index, where CellFunc will be
     * @param column column's index, where CellFunc will be
     * @param src function, which will be set
     */
    void addFormula(const int &row, const int &column, const std::string &src);

    /**
     * @brief Countes formulas
     * 
     */
    void updateInsideFormula();

    /**
     * @brief Deletes cells, which are depended on source cell
     * 
     * @param row1 row, where cell is situated
     * @param column1 column, where cell is situated
     */
    void deleteDepended(const int &row1, const int &column1);

    /**
     * @brief Detects, if formulas have cycles
     * 
     * @return true Formulas does contain cycle
     * @return false Formulas doesn't contain cycle
     */
    bool checkCycle();

    /**
     * @brief Topological sort of formulas
     * 
     * @return std::vector<int> order, in which formulas will be counted
     */
    std::vector<int> topoSort();

    /**
     * @brief Execute math operation
     * 
     * @param operation math operation which will be executed
     * @param firstOp first operand
     * @param secondOp second operand
     * @return std::string result of an operation
     */
    std::string operation(const std::string &operation, const std::string &firstOp, const std::string &secondOp);

    /**
     * @brief Execute function
     * 
     * @param operation function which will be executed
     * @param firstOp operand on which function will be executed
     * @return std::string result of a function
     */
    std::string function(const std::string &operation, const std::string &firstOp);

private:
    //!> Table itself with rows and columns
    std::vector<Line> m_Table;

    //!> number of Cells in a line (basically a size of a Line)
    size_t maxLineSize;

    //!> indexes of CellFunc in table
    std::vector<std::pair<int, int>> m_Formula;
};

#endif // TABLES_H
