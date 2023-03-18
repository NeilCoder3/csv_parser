#pragma once
#include <stdio.h>
#include <string>

#define SIMPLE_VAR_NAME_PRINTER(v) #v
#define VAR_NAME_PRINTER(name) varNamePrinter(#name, (name))

std::string varNamePrinter(const char* name, int value)
{
    std::string output(name);
    printf("name: %s\tvalue: %d\n", name, value);
    return output;
}