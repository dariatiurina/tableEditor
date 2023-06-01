/**
 * @file cell.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of classes Cell, NumCell and StringCell
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CELL_H
#define CELL_H

#include <vector>
#include <cstring>
#include <set>
#include <iostream>

/**
 * @brief Class Cell, which defines one empty cell in a table
 *
 */
class Cell
{
public:
    /**
     * @brief Construct a new Cell object
     */
    Cell();

    /**
     * @brief Destroy the Cell object
     */
    virtual ~Cell();

    /**
     * @brief Virtual function, which writes Cell's data to a ostream
     * @param os Ostream, where Cell is needed to be printed
     */
    virtual void print(std::ostream &os) const;

    /**
     * @brief Print formula
     *
     * @param os Ostream, where formula will be printed
     */
    virtual void printFunc(std::ostream &os) const;

    /**
     * @brief Get the Length object
     * @return size_t
     */
    virtual size_t getLength() const;

    /**
     * @brief Get the formula from a Cell
     *
     * @return std::vector<std::string> formula in RPN
     */
    virtual std::vector<std::string> getFormula() const;

    /**
     * @brief Set the inside in Cell
     *
     * @param src line, which needs to be set in a Cell
     */
    virtual void setInside(const std::string &src);

    /**
     * @brief Detects which Cell it is
     *
     * @return std::string which Cell is it
     */
    virtual std::string whatIs() const;

    /**
     * @brief Execute function on a Cell
     *
     * @param operation which function needs to be executed
     * @return std::string result of an operation
     * @exception if operation cannot be executed throws an exception
     */
    virtual std::string function(const std::string &operation) const;

    /**
     * @brief Execute math operation on a Cell
     *
     * @param operation operation which function needs to be executed
     * @param operand second number or line, which will be in function
     * @param isFirst true if Cell is first in operation
     * @return std::string std::string result of an operation
     */
    virtual std::string operation(const std::string &operation, const std::string &operand, const bool &isFirst) const;

    /**
     * @brief Output operator to a given ostream
     * @param os ostream, where insides will be printed
     * @param src Cell which needs to be printed
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os, const Cell &src);
};

/**
 * @brief Child class from class Cell. Defines Cell with a number written inside
 */
class NumCell : public Cell
{
public:
    /**
     * @brief Construct a new Num Cell object
     */
    NumCell();

    /**
     * @brief Construct a new Num Cell object copied from a source NumCell
     * @param src NumCell, which is needed to be copied
     */
    NumCell(const NumCell &src);

    NumCell(const int &src);

    /**
     * @brief Destroy the Num Cell object
     */
    ~NumCell();

    /**
     * @brief Get the length of a number inside a NumCell
     * @return size_t number of symbols inside NumCell's data
     */
    size_t getLength() const override;

    /**
     * @brief Prints NumCell's data to a given ostream
     * @param os ostream, where data need to be printed
     */
    void print(std::ostream &os) const override;

    /**
     * @brief Returns that this is NumCell
     *
     * @return std::string NumCell
     */
    std::string whatIs() const override;

    /**
     * @brief Execute math operation on a Cell
     *
     * @param operation operation which function needs to be executed
     * @param operand second number or line, which will be in function
     * @param isFirst true if Cell is first in operation
     * @return std::string std::string result of an operation
     */
    std::string operation(const std::string &operation, const std::string &operand, const bool &isFirst) const override;

    /**
     * @brief Return the number from a NumCell
     * @return double data inside NumCell
     */
    double getValue();

    /**
     * @brief Execute function on a Cell
     *
     * @param operation which function needs to be executed
     * @return std::string result of an operation
     * @exception if operation cannot be executed throws an exception
     */
    std::string function(const std::string &operation) const override;

    /**
     * @brief Set value to a NumCell
     * @param newValue number, which is needed to be inside a NumCell
     */
    void setValue(const double &newValue);

private:
    double m_Inside; //!< data's inside a NumCell
};

/**
 * @brief Child class from class Cell. Defines Cell with a word or sentence written inside
 */
class StringCell : public Cell
{
public:
    /**
     * @brief Construct a new String Cell object
     */
    StringCell();

    /**
     * @brief Construct a new String Cell object copied from a source StringCell
     * @param src source StringCell
     */
    StringCell(const StringCell &src);

    /**
     * @brief Destroy the String Cell object
     */
    ~StringCell();

    /**
     * @brief Get length of a StringCell's data
     * @return size_t number of symbols in StringCell's data
     */
    size_t getLength() const override;

    /**
     * @brief Get the value inside a StringCell
     * @return std::string data's inside a StringCell
     */
    std::string getValue();

    /**
     * @brief Prints StringCell's data to a given ostream
     * @param os ostream, where data need to be printed
     */
    void print(std::ostream &os) const override;

    /**
     * @brief Set value to a StringCell
     * @param newValue word or sentence, which is needed to be inside a StringCell
     */
    void setValue(const std::string &newValue);

    /**
     * @brief Execute math operation on a Cell
     *
     * @param operation operation which function needs to be executed
     * @param operand second number or line, which will be in function
     * @param isFirst true if Cell is first in operation
     * @return std::string std::string result of an operation
     */
    std::string operation(const std::string &operation, const std::string &operand, const bool &isFirst) const override;

    /**
     * @brief Execute function on a Cell
     *
     * @param operation which function needs to be executed
     * @return std::string result of an operation
     * @exception if operation cannot be executed throws an exception
     */
    std::string function(const std::string &operation) const override;

    /**
     * @brief Returns that this is StringCell
     *
     * @return std::string StringCell
     */
    std::string whatIs() const override;

private:
    std::string m_Inside; //!< data's inside a StringCell
};

/**
 * @brief Child class from class Cell. Defines Cell with a formula inside
 * 
 */
class CellFunc : public Cell
{
public:
    /**
     * @brief Construct a new Cell Func object
     *
     */
    CellFunc();

    /**
     * @brief Construct a new Cell Func object from an existing CellFunc
     *
     * @param src source CellFunc
     */
    CellFunc(const CellFunc &src);

    /**
     * @brief Destroy the Cell Func object
     *
     */
    ~CellFunc();

    /**
     * @brief Set the formula inside CellFunc
     *
     * @param line formula, which will be than translated
     */
    void setFormula(const std::string &line);

    /**
     * @brief Prints CellFunc's data to a given ostream
     * @param os ostream, where data need to be printed
     */
    void print(std::ostream &os) const override;

    /**
     * @brief Get the formula from a Cell
     *
     * @return std::vector<std::string> formula in RPN
     */
    std::vector<std::string> getFormula() const;

    /**
     * @brief Get the length of a number inside a NumCell
     * @return size_t number of symbols inside NumCell's data
     */
    size_t getLength() const;

    /**
     * @brief Execute math operation on a Cell
     *
     * @param operation operation which function needs to be executed
     * @param operand second number or line, which will be in function
     * @param isFirst true if Cell is first in operation
     * @return std::string std::string result of an operation
     */
    std::string operation(const std::string &operation, const std::string &operand, const bool &isFirst) const override;

    /**
     * @brief Execute function of a Cell
     *
     * @param operation which function needs to be executed
     * @return std::string result of an operation
     * @exception if operation cannot be executed throws an exception
     */
    std::string function(const std::string &operation) const override;

    /**
     * @brief Returns that this is CellFunc
     *
     * @return std::string CellFunc
     */
    std::string whatIs() const override;

    /**
     * @brief Set the inside in Cell
     *
     * @param src line, which needs to be set in a Cell
     */
    void setInside(const std::string &src);

    /**
     * @brief Prints formula to a given ostream
     * @param os ostream, where data need to be printed
     */
    void printFunc(std::ostream &os) const;

private:
    //!> formula in normal rotation
    std::string m_FormulaPrint;
    //!> result of formula
    std::string m_Inside;
    //!> formula in RPN after convertion
    std::vector<std::string> m_Formula;
};

#endif // CELL_H
