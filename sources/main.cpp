// Copyright 2020 Andreytorix
#include <async++.h>
#include <iostream>
#include <memory>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "menu.hpp"
int main(int argc, const char *argv[]) {
    setlocale(LC_ALL, "Russian");
    EApplication obj;
    obj.menu(argc, argv);
    return obj.exec();
}
