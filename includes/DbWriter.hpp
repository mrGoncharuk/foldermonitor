//
// Created by mhonchar on 21.11.19.
//

#ifndef FOLDERMONITORING_DBWRITER_HPP
#define FOLDERMONITORING_DBWRITER_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <list>
# include <atomic>
# include <mutex>
# include <thread>
# include <syslog.h>
# include <sqlite3.h>
# include "FileReader.hpp"
#include "IDB.hpp"

class DBWriter: public IDB
{
private:

	sqlite3 *db;
	std::string dbName;
	std::mutex dbMutex;
public:
	explicit DBWriter(std::string const &dbfilename);
	virtual ~DBWriter();
	bool 	initDB() override;
	void	executeQuery(const std::string &query) override;
	void 	closeDB() override;
};

#endif //FOLDERMONITORING_DBWRITER_HPP
