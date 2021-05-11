// Copyright 2020 Andreytorix
#include "../include/menu.hpp"
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "rocksdb/db.h"
#include <assert.h>
#include <string>
#include "../include/struct.hpp"
#include <condition_variable>
#include "../include/pico.h"
#include "boost/log/trivial.hpp"
#include "boost/log/core.hpp"
#include <boost/log/sources/logger.hpp>

namespace po = boost::program_options;
boost::log::trivial::severity_level log_lvl =
    boost::log::trivial::error;
std::condition_variable heshFunction_cv, writeFunction_cv;
std::mutex m;
bool hesh_work = true;
bool write_work = true;

void log(std::string mes) {
  if (log_lvl == boost::log::trivial::error) {
    BOOST_LOG_TRIVIAL(error) << mes;
  }
  if (log_lvl == boost::log::trivial::info) {
    BOOST_LOG_TRIVIAL(info) << mes;
  }
  if (log_lvl == boost::log::trivial::warning) {
    BOOST_LOG_TRIVIAL(warning) << mes;
  }
}
void readFileFunction(std::string input_file) {
  rocksdb::DB* db;  //ïåðåìåííàÿ äëÿ õðàíåíèÿ ÁÄ
  rocksdb::Status status;
  rocksdb::Options options;  //îïöèè
  std::vector<std::string> column_families;
  std::vector<rocksdb::ColumnFamilyDescriptor> column_familiesD;
  db->ListColumnFamilies(options, input_file,
                         &column_families);  //ñ÷èòûâàåì ñåìüè
  while (!column_families.empty()) {  //ñîçäà¸ì äåñêðèïòîð ñåìüè
    std::string t1 = std::move(column_families.front());
    log("Ñ÷èòûâàíèå äàííûõ...");
    column_families.erase(column_families.begin());
    column_familiesD.push_back(
        rocksdb::ColumnFamilyDescriptor(t1, rocksdb::ColumnFamilyOptions()));
  }
  status = rocksdb::DB::Open(
      rocksdb::DBOptions(), input_file, column_familiesD, &idb.handles,
      &db);  //îòêðûâàåì èñõîäíîå õðàíëèùå â ðåæèìå òîëüêî äëÿ ÷òåíèÿ
  for (uint64_t i = 0; i < idb.handles.size(); i++) {
    rocksdb::Iterator* ter =
        db->NewIterator(rocksdb::ReadOptions(), idb.handles[i]);
    for (ter->SeekToFirst(); ter->Valid(); ter->Next()) {
      soursedb temp;
      temp.family = i;
      temp.key = ter->key().ToString();
      temp.value = ter->value().ToString();
      log(" key: " + temp.key + "; value: " + temp.value);
      src.push_back(temp);
    }
  }
}

void hashFunction() {
  while (hesh_work) {
    if (src.empty()) {
      hesh_work = false;
    } else {
      std::unique_lock<std::mutex> lk(m);
      newdb temp;
      temp.family = src.back().family;
      temp.key = src.back().key;
      temp.value = src.back().value;
      src.pop_back();
      lk.unlock();
      temp.value = picosha2::hash256_hex_string(temp.value);
      log("key: " + temp.key + " hash:" + temp.value);
      ndb.push_back(temp);
    }
  }
}

void writeFileFunction(std::string path) {
    n = 1;
    path = "";
  while (write_work) {
    std::unique_lock<std::mutex> lk(m);
    if (ndb.size() != 0) {
      rocksdb::Status status;
      // std::cout<< ndb.back().family<< std::endl<< ndb.back().key<<
      // std::endl<< ndb.back().value << std::endl;

      status = nrdb->Put(
          rocksdb::WriteOptions(), idb.handles[ndb.back().family],
          rocksdb::Slice(ndb.back().key), rocksdb::Slice(ndb.back().value));
      ndb.pop_back();
    } else {
      writeFunction_cv.wait(lk, [] { return ndb.empty(); });
      if (!src.empty()) {
      } else {
        write_work = false;
      }
    }
    lk.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
  }
}

void createFileFunction(std::string path) {
  rocksdb::Status status;
  rocksdb::Options options;  //îïöèè
  options.create_if_missing = true;
  options.compression = rocksdb::CompressionType::kNoCompression;
  std::vector<rocksdb::ColumnFamilyDescriptor> column_familiesD;
  for (uint64_t i = 0; i < idb.handles.size(); i++) {
    if (idb.handles[i]->GetName() != "default") {
      column_familiesD.push_back(rocksdb::ColumnFamilyDescriptor(
          idb.handles[i]->GetName(), rocksdb::ColumnFamilyOptions()));
      // std::cout << idb.handles[i]->GetName() << std::endl;
    }
  }
  status = rocksdb::DB::Open(options, path, &nrdb);
  if (status.ok()) {
    log("Ôàéë ñîçäàí");
    status = nrdb->CreateColumnFamilies(column_familiesD, &idb.handles);

    log(status.ToString());
  } else {
    // std::cout << status.ToString() << std::endl;
    std::vector<std::string> column_families;
    std::vector<rocksdb::ColumnFamilyHandle*> handles;
    nrdb->ListColumnFamilies(options, path, &column_families);
    //std::vector<rocksdb::ColumnFamilyDescriptor> column_familiesD;
    while (!column_families.empty()) {
      std::string t1 = column_families.back();  //èìÿ ñåìüè
      column_families.pop_back();
      // std::cout << t1 << std::endl;
      column_familiesD.push_back(
          rocksdb::ColumnFamilyDescriptor(t1, rocksdb::ColumnFamilyOptions()));
    }
    status = rocksdb::DB::Open(rocksdb::DBOptions(), path, column_familiesD,
                               &handles, &nrdb);
    log("Ñòàòóñ: " + status.ToString());
  }
}

void EApplication::menu(int argc, const char** argv) {
  m_desk.add_options()("help", "âûçîâ ñïðàâêè")(
      "log-level", boost::program_options::value<std::string>(&log_level)
                       ->composing()
                       ->default_value("error"),
      "\"info\"|\"warning\"|\"error\"")(
      "thread-count", boost::program_options::value<int>(&thread_count)
                          ->composing()
                          ->default_value(std::thread::hardware_concurrency()),
      "êîëè÷åñòâî ïîòîêîâ")("output",
                            boost::program_options::value<std::string>(&path)
                                ->composing()
                                ->default_value("dbcs-storage.db"),
                            "Ïóòü ê íîâîìó õðàíèëèùó")(
      "input-file", boost::program_options::value<std::string>(&input_path)
                        ->composing()
                        ->default_value("source.db"),
      "Ïóòü ê õðàíèëèùó");
  p.add("input-file", -1);
  boost::program_options::store(
      boost::program_options::command_line_parser(argc, argv)
          .options(m_desk)
          .positional(p)
          .run(),
      m_vm);
  boost::program_options::notify(m_vm);
  path_to_programm = argv[0];
}

int EApplication::exec() {
  if (m_vm.count("help")) {
    // std::cout << m_desk << std::endl;

    return 1;
  }
  if (m_vm.count("log-level") && m_vm.count("thread-count") &&
      m_vm.count("output") && m_vm.count("input-file")) {
    if (log_level == "info") log_lvl = boost::log::trivial::info;
    if (log_level == "warning") log_lvl = boost::log::trivial::warning;

    if (m_vm["input-file"].as<std::string>() != "source.db") {
      std::cout << m_vm["input-file"].as<std::string>() << std::endl;
      readFileFunction(m_vm["input-file"].as<std::string>());
      createFileFunction(m_vm["output"].as<std::string>());
      if (src.size() != 0) {
        std::vector<std::thread> hesh;
        std::vector<std::thread> write;
        std::cout << thread_count << std::endl;
        for (int i = 0; i < thread_count; i++) {
          hesh.push_back(std::thread(hashFunction));
        }
        for (int i = 0; i < thread_count; i++) {
          write.push_back(std::thread(writeFileFunction,
                                      m_vm["output"].as<std::string>()));
        }
        for (int i = 0; i < thread_count; i++) {
          hesh[i].join();
        }
        for (int i = 0; i < thread_count; i++) {
          write[i].join();
        }
      }
    } else {
      std::cout << "Îøèáêà ïóòè" << std::endl;
    }
  } else {
    std::cout << "Ïîæàëóéñòà, èñïîëüçóéòå --help îïöèþ äë¤ âûâîäà ñïðàâêè"
              << std::endl;
    return 1;
  }
  return 0;
}
