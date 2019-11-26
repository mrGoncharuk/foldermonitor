//
// Created by mhonchar on 25.11.19.
//

#ifndef FOLDERMONITOR_IDB_HPP
#define FOLDERMONITOR_IDB_HPP

#include <string>

class IDB {
public:
	virtual bool initDB() = 0;
	virtual void executeQuery(const std::string &query) = 0;
	virtual void closeDB() = 0;
};

#endif //FOLDERMONITOR_IDB_HPP
