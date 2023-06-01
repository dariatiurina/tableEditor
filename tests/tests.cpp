#include <iostream>
#include <assert.h>
#include "../src/cell/cell.h"
#include "../src/commands/commands.h"
#include "../src/tables/tables.h"

int main()
{
    Tables testTable;
    assert(testTable.isEmpty() == true);

    testTable.setValue(0, 0, "Ahoj");
    assert(testTable.isEmpty() == false);

    bool exceptionThrown = false;

    try
    {
        StringCell stringTest;
        stringTest.function("sin");
    }
    catch (const std::exception &ex)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    exceptionThrown = false;
    try
    {
        NumCell numTest;
        numTest.operation("/", "a", false);
    }
    catch (const std::exception &ex)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "EVERYTHING IS CORRECT!" << std::endl;
    return EXIT_SUCCESS;
}