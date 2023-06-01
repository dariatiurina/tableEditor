/**
 * @file main.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Main file, which connects all of classes
 * @version 0.8
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include "execute/execute.h"
#include "tables/tables.h"
#include "commands/commands.h"

int main()
{
    Tables t;
    Commands c;
    while (true)
    {
        std::cout << "|-> ENTER YOUR COMMAND:" << std::endl;
        std::cin >> c;
        try
        {
            c.checkCommand();
            c.checkSequence();
            Execute newCommand(c, &t);
            if (!newCommand.executeCommand())
                break;
        }
        catch (const std::exception &ex)
        {
            std::cout << "|-> ERROR DETECTED: " << ex.what() << std::endl;
            t.deleteEmpty();
            continue;
        }
    }
    return EXIT_SUCCESS;
}
