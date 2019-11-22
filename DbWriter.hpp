//
// Created by mhonchar on 21.11.19.
//

#ifndef FOLDERMONITORING_DBWRITER_HPP
#define FOLDERMONITORING_DBWRITER_HPP

#include "sqlite3.h"

class DBWriter
{
private:
	sqlite3* DB;
public:
	DBWriter();
	~DBWriter();

};


#endif //FOLDERMONITORING_DBWRITER_HPP
