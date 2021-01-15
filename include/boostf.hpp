// Copyright 2020 Andreytorix
#pragma once
#include <limits.h>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include "boost/algorithm/string.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <any>

class Broker {
public:
    static std::vector<Broker> brokerVector;
    std::string catalog;
    std::string id;
    int date;
    int counter;
    static void AddBroker(std::string inputStringBroker) {
        std::vector<std::string> v;
        boost::split(v, inputStringBroker, boost::is_any_of(" ,._"));
        checkBroker(v);
    }
    static std::vector<Broker> ListBrokersv() {
        return (brokerVector);
    }
    std::string toString() {
        return("broker:"+ this -> catalog + " account:" + this -> id +
               " files:" + std::to_string(this ->counter) +
               " lastdate:" + std::to_string(this -> date));
    }
   static void clearVector() {
        brokerVector.clear();
    }

private:
    static void checkBroker(std::vector<std::string> inputBroker) {
        bool exists = false;
        int lvec = brokerVector.size();
        for (int i = 0; i < lvec; ++i) {
            Broker* br = &brokerVector[i];
            if (br->id == inputBroker[2]) {
                exists = true;
                if (std::stoi(inputBroker[3]) > br->date) {
                    br->date = std::stoi(inputBroker[3]);
                }
                br->counter++;
            }
        }
        if (!exists) {
            Broker broker;
            broker.counter = 1;
            broker.catalog = inputBroker[0];
            broker.id = inputBroker[2];
            broker.date = std::stoi(inputBroker[3]);
            brokerVector.push_back(broker);
        }
    }
};
std::vector<std::string> recdir(std::string);
void show_vector(std::vector<std::string> vec);
void show_broker(std::vector<Broker> vec);
