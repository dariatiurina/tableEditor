/**
 * @file execute.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Implementation of class Execute
 * @version 0.8
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 */
#ifndef EXECUTE_CPP
#define EXECUTE_CPP
#include "execute.h"
#include "../help/help.h"
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <cmath>

Execute::Execute(const Commands &command, Tables *src) : m_Command(command), m_Table(src) {}

Execute::~Execute() {}

bool Execute::executeCommand()
{
    std::vector<std::string> doCommand = m_Command.getCommands();
    if (doCommand[0] == "exit")
    {
        std::cout << "|-> GOODBYE!" << std::endl;
        return false;
    }

    std::vector<std::string> help = m_Command.getString();
    int ind = (int)help.size() - 1;
    //Adds together help string, which are one after another
    for (int i = (int)doCommand.size() - 1; i > 0 && ind > 0; i--)
    {
        if (doCommand[i - 1] == "=")
            break;
        else if ((doCommand[i] == "src" || doCommand[i] == "cellnum" || doCommand[i] == "cellrange") && (doCommand[i - 1] == "src" || doCommand[i - 1] == "cellnum" || doCommand[i - 1] == "cellrange"))
        {
            help[ind - 1] += " ";
            help[ind - 1] += help[ind];
            doCommand.erase(doCommand.begin() + i);
            doCommand[i - 1] = "src";
            help.erase(help.begin() + ind);
            ind--;
        }
    }

    //Depends on command, which operations class Table will execute
    if (doCommand[0] == "print")
    {
        m_Table->updateInsideFormula();
        if (doCommand[1] == "all")
            m_Table->printTable();
        else if (doCommand[1] == "cellnum")
        {
            std::pair<int, int> cell = translateCell(help[0]);
            m_Table->printCell(cell.first, cell.second);
        }
        else if (doCommand[1] == "cellrange")
        {
            std::string line1;
            std::string line2;
            line1 = help[0].substr(0, help[0].find(':'));
            line2 = help[0].substr(help[0].find(':'));
            std::pair<int, int> cell1 = translateCell(line1);
            std::pair<int, int> cell2 = translateCell(line2);
            m_Table->printRange(cell1.first, cell1.second, cell2.first, cell2.second);
        }
        if (doCommand[1] == "formula")
        {
            if (doCommand[2] == "all")
            {
                m_Table->printTable(true);
            }
            else if (doCommand[2] == "cellnum")
            {
                std::pair<int, int> cell = translateCell(help[0]);
                m_Table->printCell(cell.first, cell.second, true);
            }
            else if (doCommand[2] == "cellrange")
            {
                std::string line1;
                std::string line2;
                line1 = help[0].substr(0, help[0].find(':'));
                line2 = help[0].substr(help[0].find(':'));
                std::pair<int, int> cell1 = translateCell(line1);
                std::pair<int, int> cell2 = translateCell(line2);
                m_Table->printRange(cell1.first, cell1.second, cell2.first, cell2.second, true);
            }
        }
    }
    else if (doCommand[0] == "del")
    {
        if (doCommand[1] == "all")
            m_Table->deleteAll();
        else if (doCommand[1] == "cellnum")
        {
            std::pair<int, int> cell = translateCell(help[0]);
            m_Table->deleteCell(cell.first, cell.second);
        }
        else if (doCommand[1] == "cellrange")
        {
            std::string line1;
            std::string line2;
            line1 = help[0].substr(0, help[0].find(':'));
            line2 = help[0].substr(help[0].find(':'));
            std::pair<int, int> cell1 = translateCell(line1);
            std::pair<int, int> cell2 = translateCell(line2);
            m_Table->deleteRange(cell1.first, cell1.second, cell2.first, cell2.second);
        }
    }
    else if (doCommand[0] == "export")
    {
        std::ofstream fileOut("examples/" + help[0], std::ios::trunc);
        if (fileOut.is_open())
            m_Table->exportTable(fileOut);
        else
            throw std::logic_error("File cannot be made");

        fileOut.close();
    }
    else if (doCommand[0] == "import")
    {
        bool ok = true;
        std::ifstream fileIn("examples/" + help[0]);
        if (!fileIn.is_open())
            throw std::logic_error("File cannot be open");
        if (!m_Table->isEmpty())
        {
            std::cout << "|-> Table is not empty. Continue? y/n" << std::endl;
            ok = false;
            std::string line;
            for (int i = 0; i < 3; i++)
            {
                std::getline(std::cin, line);
                std::transform(line.begin(), line.end(), line.begin(), ::tolower);
                if (line == "y" || line == "yes")
                {
                    ok = true;
                    break;
                }
                if (line == "n" || line == "no")
                    return true;
            }
        }
        if (!ok)
            return true;
        m_Table->deleteAll();
        m_Table->importTable(fileIn);
        fileIn.close();
    }
    else if (doCommand[0] == "formula")
    {
        std::pair<int, int> cell = translateCell(help[0]);
        m_Table->addFormula(cell.first, cell.second, help[1]);
    }
    else if (doCommand[1] == "=")
    {
        std::pair<int, int> cell = translateCell(help[0]);
        if (doCommand[2] != "cellnum")
            m_Table->setValue(cell.first, cell.second, help[1]);
        else if (doCommand[2] == "cellnum")
        {
            std::pair<int, int> cell2 = translateCell(help[1]);
            m_Table->copyValue(cell.first, cell.second, cell2.first, cell2.second);
        }
        m_Table->updateInsideFormula();
    }
    else
        throw std::logic_error("Unknown command");
    return true;
}

#endif // EXECUTE_CPP