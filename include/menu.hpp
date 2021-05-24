// Copyright 2020 Andreytorix
#pragma once
#include <boost/program_options.hpp>
#include <string>
class EApplication {
	char** argv1;
	int argc1;
public:

	void menu(int argc, const char** argv);
	int exec();
	std::string path_to_programm;
private:
	boost::program_options::positional_options_description p;

	boost::program_options::options_description m_desk{ "доступные опции" };
	boost::program_options::variables_map m_vm;
	std::string conf;      
	bool insta;
	bool pack;
	int timeout;
};
