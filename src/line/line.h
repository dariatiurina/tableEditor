/**
 * @file line.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of class Line
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef LINE_H
#define LINE_H

#include <vector>
#include "../cell/cell.h"
#include <iostream>
#include <fstream>

/**
 * @brief Class Line which defines 1 line in a class Tables
 */
class Line
{
public:
    /**
     * @brief Construct a new Line object
     */
    Line();

    /**
     * @brief Construct a new Line object copied from a source Line
     * @param src source Line, which is needed to be copied from
     */
    Line(const Line &src);

    /**
     * @brief Destroy the Line object
     */
    ~Line();

    /**
     * @brief Changes width of a Line
     * @param newWidth width, which will be set
     */
    void changeSize(const size_t &newWidth);

    /**
     * @brief Get the Cell object
     * @param ind where Cell is situated
     * @return Cell* needed Cell
     */
    Cell *getCell(const size_t &ind) const;

    /**
     * @brief Print full Line to a given std::ostream
     * @param os std::ostream, where Line will be printed
     * @param CellWidth std::vector<size_t> with maxWidth of every Column
     */
    void print(std::ostream &os, const std::vector<size_t> CellWidth) const;

    /**
     * @brief Print givenRange Line to a given std::ostream
     * @param os std::ostream, where Line will be printed
     * @param CellWidth std::vector<size_t> with maxWidth of every Column
     * @param column1 starting Range column
     * @param column2 ending Range column
     */
    void printRange(std::ostream &os, const std::vector<size_t> CellWidth, const size_t &column1, const size_t &column2) const;

    /**
     * @brief Export Line to a given std::ofstream
     * @param of std::ofstream where Line will be exported to
     */
    void exportLine(std::ofstream &of) const;

    /**
     * @brief exports formula
     * 
     * @param of file, where formula will be exported
     */
    void exportLineFunc(std::ofstream &of) const;

    /**
     * @brief change maxWidth size parametr
     * @param newSize new maxSize
     */
    void changeSizeCell(const size_t &newSize);

    /**
     * @brief Sets the CellFunc
     * 
     * @param column index, where CellFunc will be set
     * @param src line which will be set as formula
     */
    void setFormula(const int & column, const std::string & src);

    /**
     * @brief Delete Cell from a Line
     * @param ind index, where needed Cell is
     */
    void delCell(const int &ind);

    /**
     * @brief Checks if Line is Empty
     * @return true Line is empty
     * @return false Line has at least one not empty Cell
     */
    bool isEmpty();

    /**
     * @brief Delete all empty columns at the back of a Line
     */
    void delEmpty();

    /**
     * @brief Get the CellWidth of a Cell with ind
     * @param ind Index of a needed Cell
     * @return size_t return either CellWidth of a given Cell or const MaxWidthCell, if CellWidth is smaller than MaxWidthCell
     */
    size_t getCellWidth(const size_t &ind) const;

    /**
     * @brief print formula containing in Cell
     * 
     * @param row row, in which line is in
     * @param column_min from which column start
     * @param column_max on which column end
     */
    void printFormula(const size_t & row, const size_t & column_min = 0, const size_t & column_max = 0) const;

    /**
     * @brief Return size of a Line
     * @return size_t number of columns in a Line
     */
    size_t getSize() const;

    /**
     * @brief Sets value to a given Cell
     * @param ind Index of needed Cell
     * @param newValue value, which will be set
     */
    void setValue(const int &ind, const std::string &newValue);

    /**
     * @brief Set the formula
     * 
     * @param ind index where CellFunc will be
     * @param newValue formula which will be set
     */
    void setValueFormula(const int &ind, const std::string &newValue);

    /**
     * @brief return indexes of depended cells
     * 
     * @param childCell which child cell needs to be find in formulas
     * @return std::vector<int> vector of indexes
     */
    std::vector<int> deleteDepend(const std::string &childCell);

    /**
     * @brief Returns whether line has formulas
     * 
     * @return true line has at least 1 CellFunc
     * @return false line doesn't have ane CellFunc
     */
    bool hasFormula() const;

private:
    //!> std::vector of pointers to Cells
    std::vector<Cell *> m_Line;

    //!> const maxWidthCell, which is used while printing Line
    size_t maxWidthCell;
};

#endif // LINE_H
