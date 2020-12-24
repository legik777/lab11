// Copyright 2020 Your Name <your_email>

#include <boostf.hpp>

std::vector<std::string> recdir(std::string path_to_ftp) {
    boost::filesystem::recursive_directory_iterator end;
    std::string dir;
    std::vector<std::string> ret;
    int countv = 0;
    for (boost::filesystem::recursive_directory_iterator
         it(path_to_ftp); it != end; ++it) { //перебор файлов и каталогов
        boost::filesystem::file_status
            fs = boost::filesystem::status(*it); //информация о файле
        switch (fs.type()) { //
        case boost::filesystem::regular_file:
            break;
        case boost::filesystem::symlink_file:
            dir = it->path().stem().string();
            break;
        case boost::filesystem::directory_file:
            dir = it->path().stem().string();
            break;
        default:
            break;
        }
        std::string fi = it->path().stem().string();
        if (!boost::algorithm::ends_with(fi, ".old")
            && boost::algorithm::starts_with(fi, "balance_")) { //
            countv++;
            std::string brok = dir + " " + it->path().stem().string();
            Broker::AddBroker(brok);
            ret.push_back(brok);
        }
    }
    ret.push_back(std::to_string(countv));
    return ret;
}

void show_vector(std::vector<std::string> vec) {
    for (std::vector<std::string>::iterator
         it = vec.begin(); it != vec.end(); ++it)
        std::cout << *it << std::endl;
}
void show_broker(std::vector<Broker> vec) {
    for (Broker b : vec) {
        std::cout << b.toString() << std::endl;
    }
}
