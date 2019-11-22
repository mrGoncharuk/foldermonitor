//
// Created by mhonchar on 21.11.19.
//

#include "DbWriter.hpp"
#include <iostream>
DBWriter::DBWriter(std::string const &dbfilename, std::string const &pathtomon)
	: dbName(dbfilename)
	, pathToMonitor(pathtomon)
	, db(nullptr)
{
	syslog(LOG_NOTICE, "Database writer created.");
}

DBWriter::~DBWriter()
{
	syslog(LOG_NOTICE, "Database writer destroyed.");
}
void 	DBWriter::initDBWriter()
{
	int status;
	status = sqlite3_open(dbName.c_str(), &db);
	if (status != SQLITE_OK)
		throw "Error occured while opening database.";
}

void 	DBWriter::migrateData(const std::string &fname)
{
	std::ifstream ifs(fname.c_str());
	std::string query = "INSERT INTO filedata VALUES('";
	if (ifs)
	{
		std::string data;
		std::getline(ifs, data, ':'); // get key
		query += data;
		query += "','";
		data = "";
		std::getline(ifs, data, ':'); // get value
		query += data;
		query += "');";
	}
	else
		syslog(LOG_ERR, "Error while opening file <%s>", fname.c_str());

	sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
	std::cout << query;
}

void	DBWriter::startWriting(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	std::string currFileName;

	while (isRunning)
	{
		if (filenames.empty() == false)
		{
			std::lock_guard<std::mutex> lock(list_mutex);
			currFileName = filenames.front();
			filenames.pop_front();
			migrateData(pathToMonitor + currFileName);
		}
	}
}


