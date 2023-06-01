/**
 * @file commands.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of a class Commands
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <string>

/**
 * @brief Class Commands, which defines transleted commands from a user
 *
 */
class Commands
{
public:
    /**
     * @brief Construct a new Commands object
     */
    Commands();

    /**
     * @brief Construct a new Commands object as a copy from a source Commands
     * @param src source Commands, which must be copied
     */
    Commands(const Commands &src);

    /**
     * @brief Destroy the Commands object
     */
    ~Commands();

    /**
     * @brief Input operator
     * @param is input stream, from where m_Input is brought
     * @param dest Commands class, where line is needed to be written
     * @return std::istream& original std::istreams
     */
    friend std::istream &operator>>(std::istream &is, Commands &dest);

    /**
     * @brief Function checks what was written by a user
     */
    void checkCommand();

    /**
     * @brief Returns std::vector of indexes of Commands
     * @return std::vector<int> std::vector of indexes of Commands
     */
    std::vector<std::string> getCommands() const;

    /**
     * @brief Returns std::vector of std::strings from a Commands
     * @return std::vector<std::string> helpStrings which user has written
     */
    std::vector<std::string> getString() const;

    /**
     * @brief Function converts a std::string line, which was written by user, to an index of a command
     * @param line word, which user has written
     * @param position number of a word
     * @return int returns -1 if command is not found, and positive number if command is found
     */
    std::string convertCommand(const std::string &line, const size_t &position);

    /**
     * @brief Checks if given sequence of a Commands exists and can be Executed
     */
    void checkSequence();

private:
    //!> User's input
    std::string m_Input;
    //!> Decoded Commands
    std::vector<std::string> m_Commands;
    //!> helpStrings, which user has given (ex. CellNum or newData)
    std::vector<std::string> m_HelpString;
};

#endif // COMMANDS_H
