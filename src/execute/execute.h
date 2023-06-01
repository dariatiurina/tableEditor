/**
 * @file execute.h
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Declaration of a class Execute
 * @version 1.0
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef EXECUTE_H
#define EXECUTE_H

#include "../commands/commands.h"
#include "../tables/tables.h"
#include "../operators/operators.h"

/**
 * @brief class Execute, which connects class Commands and Tables and execute given Commands on a given Tables
 */
class Execute
{
public:
    /**
     * @brief Construct a new Execute object
     * @param command Commands which is needed to be executed
     * @param src On what Tables will be Commands executed
     */
    Execute(const Commands &command, Tables *src);

    /**
     * @brief Destroy the Execute object
     */
    ~Execute();

    /**
     * @brief Executes Command
     * @return true continue program
     * @return false exit program
     */
    bool executeCommand();

private:
    //!> Commands, which will be executed
    const Commands m_Command;

    //!> Tables, on which Commands will be executed
    Tables *m_Table;
};

#endif // EXECUTE_H
