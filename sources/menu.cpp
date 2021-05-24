// Copyright 2020 Andreytorix
#include "menu.hpp"
#include <iostream>
#include "async++.h"
#include <thread>
#include <boost/process.hpp>
#include <boost/bind.hpp>
namespace po = boost::program_options;
namespace bp = boost::process;

const std::string dbuild = ".build";
const std::string dinstall = ".install";

void EApplication::menu(int argc, const char** argv) {
    m_desk.add_options()("help", "����� �������")
        ("config", boost::program_options::value<std::string>(&conf)->composing()->default_value("Debug"), "\"Release\"|\"Debug\"")
        ("install", "�������� ���� ���������")
        ("pack", "�������� ���� ��������")
        ("timeout", boost::program_options::value<int>(&timeout)->composing(), "����� �������� � ��������");
    //p.add("input-file", -1);
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(m_desk).positional(p).run(), m_vm);
    boost::program_options::notify(m_vm);
}

/*void timer() {
   // std::this_thread::sleep_for(milliseconds{ DATA_TIMEOUT });
}*/


int EApplication::exec() {
    bp::pipe pip;
    bp::child *ch;


    if (m_vm.count("help")) {
        std::cout << m_desk << std::endl;
        return 1;
    } else {

        int t = timeout;
        if (t !=0){
            auto t1 = async::spawn([ t, &ch] {
                std::this_thread::sleep_for(std::chrono::seconds(t));
                ch->terminate();
                std::cout << "cansel building, timeout" << std::endl;
            });
        }
        std::string c = conf;
        auto t2 = async::spawn([&ch, c]{ //cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Debug
            ch = new bp::child("cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE="+c, bp::std_out > stdout);
            ch->wait();
        });
        auto t3 = t2.then([&ch]{

            ch = new bp::child("cmake --build _builds", bp::std_out > stdout);
            ch->wait();
        });


        if(m_vm.count("install") && m_vm.count("pack")){
            auto t4 = t3.then([&ch]{
                ch = new bp::child("cmake --build _builds --target install", bp::std_out > stdout);
                ch->wait();
            });

            auto t5 = t4.then([&ch]{
                ch = new bp::child("cmake --build _builds --target package", bp::std_out > stdout);
                ch->wait();
            });

        } else {
            if (m_vm.count("install")) {
                auto t4 = t3.then([&ch]{
                    ch = new bp::child("cmake --build _builds --target install", bp::std_out > stdout);
                    ch->wait();
                });
            }
            if (m_vm.count("pack")){
                auto t4 = t3.then([&ch]{
                    ch = new bp::child("cmake --build _builds --target package", bp::std_out > stdout);
                    ch->wait();
                });
            }
        }

       /* if(m_vm.count("install")){
            auto t4 = t3.then([&ch]{
                ch = new bp::child("cmake --build _builds --target install", bp::std_out > stdout);
                ch->wait();
            });
        }
        if(m_vm.count("pack")){
            auto t4 = t3.then([&ch]{
                ch = new bp::child("cmake --build _builds --target install", bp::std_out > stdout);
                ch->wait();
            });
        }*/
       /* else
            auto t5 = t4.then([&ch]{
                ch = new bp::child("cmake --build _builds --target package", bp::std_out > stdout);
                ch->wait();
            });
        }*/

        //cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Debug

        /*auto timer = async::spawn([] {
            std::cout << "test2" << std::endl;
        });*/




        //std::cout << "test1"<<std::endl;
        //bp::system("C:\\Windows\\System32\\cmd.exe", " /K test ping ya.ru");


        /*if (m_vm.count("config") && m_vm.count("install") && m_vm.count("pack") && m_vm.count("timeout")) {

        }
        else {
            std::cout << "����������, ����������� --help ����� �� ������ �������"
                << std::endl;
            return 1;
        }*/
    }



    return 0;
}


/*
bp::opstream in;
bp::ipstream out;

bp::child c("c++filt", std_out > out, std_in < in);

in << "_ZN5boost7process8tutorialE" << endl;
std::string value;
out >> value;

c.terminate();
*/


/*
std::vector<std::string> read_demangled_outline(const std::string & file)
{
    bp::pipe p;
    bp::ipstream is;

    std::vector<std::string> outline;

    //we just use the same pipe, so the
    bp::child nm(bp::search_path("nm"), file,  bp::std_out > p);
    bp::child filt(bp::search_path("c++filt"), bp::std_in < p, bp::std_out > is);

    std::string line;
    while (filt.running() && std::getline(is, line)) //when nm finished the pipe closes and c++filt exits
        outline.push_back(line);

    nm.wait();
    filt.wait();
}
*/
