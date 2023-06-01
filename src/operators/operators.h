/**
 * @file operators.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of class Operators, which converts formula to RPN
 * @version 1.0
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef OPERATORS_H
#define OPERATORS_H
#include <stack>
#include <vector>
#include <string>

/**
 * @brief Class which converts formula to RPN
 *
 */
class Operators
{
public:
    /**
     * @brief Construct a new Operators object
     *
     */
    Operators();

    /**
     * @brief Destroy the Operators object
     * 
     */
    ~Operators();

    /**
     * @brief Converts line into RPN
     * 
     * @param src line, which is needed to be converted
     */
    void convertLine(const std::string &src);

    /**
     * @brief returns converted formula
     * 
     * @return std::vector<std::string> formula in RPN 
     */
    std::vector<std::string> returnLine();

private:
    
    //!> result of convertion in RPN
    std::vector<std::string> m_Numbers;
};

#endif