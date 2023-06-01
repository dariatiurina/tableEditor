/**
 * @file commands.cpp
 * @author Daria Tiurina (tiuridar@fit.cvut.cz)
 * @brief Implementation of class Commands
 * @version 0.9
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef COMMANDS_CPP
#define COMMANDS_CPP
#include "commands.h"
#include "../help/help.h"
#include <iostream>
#include <algorithm>
#include <sstream>

Commands::Commands() : m_Commands(0) {}

Commands::Commands(const Commands &src) : m_Commands(src.m_Commands), m_HelpString(src.m_HelpString) {}

Commands::~Commands() {}

std::string Commands::convertCommand(const std::string &line, const size_t &position)
{
    std::string tmp = line;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    if (position == 0)
    {
        if (tmp == "exit" || tmp == "print" || tmp == "export" || tmp == "import" || tmp == "formula")
            return tmp;
        else if (detectIfIsCell(tmp))
        {
            m_HelpString.push_back(tmp);
            return "cellnum";
        }
        else if (tmp == "delete" || tmp == "del")
            return "del";
        return "";
    }
    else if (position == 1)
    {
        if (tmp == "all" || tmp == "formula" || tmp == "=")
            return tmp;
        else if (detectIfIsCell(tmp))
        {
            this->m_HelpString.push_back(tmp);
            return "cellnum";
        }
        else if (detectIfIsRange(tmp))
        {
            this->m_HelpString.push_back(tmp);
            return "cellrange";
        }

        this->m_HelpString.push_back(line);
        return "src";
    }
    else if (position == 2)
    {
        if (tmp == "all" || tmp == "=")
            return tmp;
        else if (detectIfIsRange(tmp))
        {
            this->m_HelpString.push_back(tmp);
            return "cellrange";
        }
        else if (detectIfIsCell(tmp))
        {
            this->m_HelpString.push_back(tmp);
            return "cellnum";
        }
        this->m_HelpString.push_back(line);
        return "src";
    }
    this->m_HelpString.push_back(line);
    return "src";
}

void Commands::checkCommand()
{
    this->m_HelpString.clear();
    m_Commands.clear();
    //Checks if EOF => ends program
    if (m_Input.empty())
        m_Commands.push_back("exit");

    std::string tmp;
    std::stringstream os(m_Input);
    while (std::getline(os, tmp, ' '))
    {
        if(tmp.empty())
            continue;
        std::string check = convertCommand(tmp, this->m_Commands.size());
        if (check.empty())
        {
            this->m_Commands.clear();
            throw std::logic_error("Unknown command");
        }
        else
            this->m_Commands.push_back(check);
        tmp.clear();
    }
}

void Commands::checkSequence()
{
    bool hasCommand = false;

    //If command doesn't have at least one command => it is not a valid command
    for (size_t i = 0; i < m_Commands.size(); i++)
        if (m_Commands[i] == "print" || m_Commands[i] == "=" || m_Commands[i] == "exit" || m_Commands[i] == "import" || m_Commands[i] == "export" || m_Commands[i] == "del")
            hasCommand = true;

    if (!hasCommand)
        throw std::logic_error("Unknown command");

    if (m_Commands.size() == 1)
    {
        //adds helping words for processing
        if (m_Commands[0] == "print")
            m_Commands.push_back("all");
        else if (m_Commands[0] == "del")
            m_Commands.push_back("all");
        else if (m_Commands[0] == "export" || m_Commands[0] == "immport")
        {
            m_Commands.push_back("src");
            m_HelpString.push_back("table.csv");
        }
        else if (m_Commands[0] != "exit")
        {
            throw std::logic_error("Unknown command");
        }
    }
    else if (m_Commands.size() == 2)
    {
        if (m_Commands[0] == "print" && m_Commands[1] == "formula")
        {
            m_Commands.push_back("all");
        }
        else if ((m_Commands[0] == "print" || m_Commands[0] == "del") && (m_Commands[1] != "cellnum" && m_Commands[1] != "cellrange" && m_Commands[1] != "all"))
        {
            throw std::logic_error("Unknown command");
        }
        else if(m_Commands[1] == "=" && m_Commands[0] == "cellnum"){
            m_Commands.push_back("src");
            m_HelpString.push_back(" ");
        }
        else
        {
            for (size_t i = 0; i < m_Commands.size(); i++)
                if (m_Commands[i] == "exit" || m_Commands[i] == "=")
                    throw std::logic_error("Unknown command");
        }
    }
    else if (m_Commands.size() == 3)
    {
        if (m_Commands[0] == "print" && m_Commands[1] != "formula")
            throw std::logic_error("Unknown command");
        else if (m_Commands[1] == "=" && (m_Commands[0] != "cellnum" || (m_Commands[2] != "src" && m_Commands[2] != "cellnum")))
            throw std::logic_error("Unknown command");
        for (size_t i = 0; i < m_Commands.size(); i++)
            if (m_Commands[i] == "export" || m_Commands[i] == "import" || m_Commands[i] == "exit")
                throw std::logic_error("Unknown command");
    }
    else
    {
        if (m_Commands[1] != "=" && m_Commands[0] != "formula" && m_Commands[0] != "export" && m_Commands[0] != "import")
            throw std::logic_error("Unknown command");
    }
}

std::vector<std::string> Commands::getCommands() const
{
    return this->m_Commands;
}

std::vector<std::string> Commands::getString() const
{
    return m_HelpString;
}

std::istream &operator>>(std::istream &is, Commands &dest)
{
    try
    {
        std::getline(std::cin, dest.m_Input);
    }
    catch (const std::exception &ex)
    {
        throw std::logic_error("Input is wrong. Please, try shorter command");
    }
    return is;
}

#endif // COMMANDS_CPP