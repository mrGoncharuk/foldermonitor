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
# include "FileReader.hpp"
class DBWriter
{
private:
	sqlite3* db;
	std::string dbName;
	std::string pathToMonitor;
	const int threadAmount;
	std::mutex dbMutex;
public:
	explicit DBWriter(std::string const &dbfilename, std::string const &pathtomon);
	~DBWriter();
	bool 	initDBWriter();
	void	startWriting(std::mutex &p_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning);
	void	sendQuery(const std::string &fname, std::atomic<bool> &isDone);
};


#endif //FOLDERMONITORING_DBWRITER_HPP
