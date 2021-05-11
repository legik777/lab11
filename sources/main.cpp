#include <iostream>
#include "menu.hpp"
int main(int argc, const char* argv[])
{
    setlocale(LC_ALL, "Russian");
    EApplication obj;
    obj.menu(argc, argv);
    return obj.exec();
}