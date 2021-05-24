// Copyright 2021 Andreytorix
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

void EApplication::menu(int argc,
  const char ** argv) {
  m_desk.add_options()("help", "����� �������")
    ("config", boost::program_options::value < std::string > ( & conf) -> composing() -> default_value("Debug"), "\"Release\"|\"Debug\"")
    ("install", "�������� ���� ���������")
    ("pack", "�������� ���� ��������")
    ("timeout", boost::program_options::value < int > ( & timeout) -> composing(), "����� �������� � ��������");
  //p.add("input-file", -1);
  boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(m_desk).positional(p).run(), m_vm);
  boost::program_options::notify(m_vm);
}
int EApplication::exec() {
  bp::pipe pip;
  bp::child * ch;

  if (m_vm.count("help")) {
    std::cout << m_desk << std::endl;
    return 1;
  } else {
    int t = timeout;
    if (t != 0) {
      auto t1 = async ::spawn([t, & ch] {
        std::this_thread::sleep_for(std::chrono::seconds(t));
        ch -> terminate();
        std::cout << "cansel building, timeout" << std::endl;
      });
    }
    std::string c = conf;
    auto t2 = async ::spawn([ & ch, c] { //cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Debug
      ch = new bp::child("cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=" + c, bp::std_out > stdout);
      ch -> wait();
    });
    auto t3 = t2.then([ & ch] {
      ch = new bp::child("cmake --build _builds", bp::std_out > stdout);
      ch -> wait();
    });
    if (m_vm.count("install") && m_vm.count("pack")) {
      auto t4 = t3.then([ & ch] {
        ch = new bp::child("cmake --build _builds --target install", bp::std_out > stdout);
        ch -> wait();
      });
      auto t5 = t4.then([ & ch] {
        ch = new bp::child("cmake --build _builds --target package", bp::std_out > stdout);
        ch -> wait();
      });
    } else {
      if (m_vm.count("install")) {
        auto t4 = t3.then([ & ch] {
          ch = new bp::child("cmake --build _builds --target install", bp::std_out > stdout);
          ch -> wait();
        });
      }
      if (m_vm.count("pack")) {
        auto t4 = t3.then([ & ch] {
          ch = new bp::child("cmake --build _builds --target package", bp::std_out > stdout);
          ch -> wait();
        });
      }
    }
  }
  return 0;
}
