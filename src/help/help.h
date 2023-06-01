/**
 * @file help.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of helping functions
 * @version 1.0
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef HELP_H
#define HELP_H
#include <string>

/**
 * @brief Helping function which detects if given line is CellRange
 *
 * @param line Input, which is needed to be checked
 * @return true line is Range
 * @return false line is not Range
 */
bool detectIfIsRange(const std::string &line);

/**
 * @brief Helping function which detects if given line is CellNum
 *
 * @param line Input, which is needed to be checked
 * @return true line is CellNum
 * @return false line is not CellNum
 */
bool detectIfIsCell(const std::string &line);

/**
 * @brief Helping function, which translates std::string with a Cell to a pair of indexes in Tables
 *
 * @param line std::string, which must be translated
 * @return std::pair<int, int> pair of indexes of translated line
 */
std::pair<int, int> translateCell(const std::string &line);

/**
 * @brief Helping function, which detetcts if line is a function
 * 
 * @param line string, which may be function
 * @return true line is function
 * @return false line is not function
 */
bool isFunc(const std::string & line);

/**
 * @brief Helping function, which detetcts if line is math operation 
 * 
 * @param line string, which may be function
 * @return true line is math operation 
 * @return false line is not math operation 
 */
bool isOperation(const std::string &line);

/**
 * @brief Helping function, which detects if formula is valid
 * 
 * @param line string with formula in need of validation
 * @return true formula is valid
 * @return false formula is not valid
 */
bool checkFormula(const std::string &line);

/**
 * @brief Helping function, which detects if string can be converted to a double
 * 
 * @param src string which will be checked
 * @return true is number
 * @return false is not number
 */
bool isNum(const std::string &src);

/**
 * @brief Helping funtion, which translates cell's index in Tables to a string
 * 
 * @param os stream, where will be printed output
 * @param row cell's row index
 * @param column cell's column index
 */
void translateRow(std::ostream & os, const size_t & row, const size_t & column);

#endif // HELP_H