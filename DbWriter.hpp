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
# include "sqlite3.h"

class DBWriter
{
private:
	DBWriter();
	sqlite3* db;
	std::string dbName;
	std::string pathToMonitor;
	static const int threadAmount;
	std::mutex dbMutex;
public:
	DBWriter(std::string const &dbfilename, std::string const &pathtomon);
	~DBWriter();
	void 	initDBWriter();
	void	startWriting(std::mutex &p_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning);
	void	migrateData(const std::string fname, std::atomic<bool> &isDone);
};


#endif //FOLDERMONITORING_DBWRITER_HPP
