// Copyright 2020 Andreytorix

#include <boostf.hpp>
std::vector<Broker> Broker::brokerVector;
int main(int n, char* arr[]) {
    std::string path_to_ftp;
    if (n != 1){
    path_to_ftp = arr[1];
    } else {
    path_to_ftp = boost::filesystem::current_path().string();
    }
    if (boost::filesystem::exists(path_to_ftp)) {
    std::cout << path_to_ftp << std::endl;
    std::vector<std::string> vec;
    vec = recdir(path_to_ftp);
    std::cout << "number of files found " + vec.back() << std::endl;
    vec.pop_back();
    show_vector(vec);
    show_broker(Broker::ListBrokersv());
    } else {
    std::cout << "dir not exist" << std::endl;
    }
}

