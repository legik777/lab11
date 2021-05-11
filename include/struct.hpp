// Copyright 2020 Andreytorix
#pragma once
#include <string>
#include <vector>
#include "rocksdb/db.h"
struct soursedb{
	std::string key, value;
	int family;
};
std::vector<soursedb> src;
struct newdb {
	std::string key, value;
	int family;
};
std::vector<newdb> ndb;
struct dbinfo {
	std::vector<rocksdb::ColumnFamilyHandle*> handles;
};
dbinfo idb;
rocksdb::DB* nrdb;
