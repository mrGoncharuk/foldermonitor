//
// Created by mhonchar on 21.11.19.
//

#include "../includes/DbWriter.hpp"
#include <iostream>
#include <sstream>

const int DBWriter::threadAmount = 3;

DBWriter::DBWriter(std::string const &dbfilename, std::string const &pathtomon)
	: db(nullptr)
	, dbName(dbfilename)
	, pathToMonitor(pathtomon)

{
	syslog(LOG_NOTICE, "DW> Database writer created.");
}

DBWriter::~DBWriter()
{
	syslog(LOG_NOTICE, "DW> Database writer destroyed.");
}
bool 	DBWriter::initDBWriter()
{
	int status;
	status = sqlite3_open(dbName.c_str(), &db);
	if (status != SQLITE_OK)
	{
		syslog(LOG_ERR, "DW> Error occured while opening database.");
		syslog(LOG_NOTICE, "DW> Daemon stops.");
		return false;
	}
	syslog(LOG_NOTICE, "DW> Database writer initialized successfully.");
	return true;
}

void 	DBWriter::sendQuery(const std::string &fname, std::atomic<bool> &isDone)
{
	std::string key, value;
	std::stringstream ss;
	int ret;
	char *errmsg;

	FileReader::extractData(fname, key, value);
	ss << "INSERT INTO filedata VALUES('" << key << "','" << value << "');";

	std::lock_guard<std::mutex> lock(dbMutex);
	ret = sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, &errmsg);
	if (ret == SQLITE_OK)
		syslog(LOG_NOTICE, "DW> Query with <%s> data done successfully.", fname.c_str());
	else
	{
		syslog(LOG_ERR, "DW> Error while migrating data from <%s>. Query: [%s]. Error message: %s.", fname.c_str(), ss.str().c_str(), errmsg);
		sqlite3_free(errmsg);
	}
	isDone = true;
}

void	DBWriter::startWriting(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	std::string currFileName;
	std::thread migrateThreads[threadAmount];
	std::atomic<bool> isThreadFinished[threadAmount];
	int i = 0;

	while (i < threadAmount && isRunning)
	{
		std::lock_guard<std::mutex> lock(list_mutex);
		if (!filenames.empty())
		{
			isThreadFinished[i] = false;
			migrateThreads[i] = std::thread(&DBWriter::sendQuery, std::ref(*this), pathToMonitor + filenames.front(),
											std::ref(isThreadFinished[i]));
			filenames.pop_front();
			i++;
		}
	}
	while (isRunning)
	{
		for (i = 0; i < threadAmount; i++)
		{
			if (isThreadFinished[i])
			{
				std::lock_guard<std::mutex> lock(list_mutex);
				if (!filenames.empty())
				{
					migrateThreads[i].join();
					isThreadFinished[i] = false;
					migrateThreads[i] = std::thread(&DBWriter::sendQuery, std::ref(*this), pathToMonitor + filenames.front(),
											  std::ref(isThreadFinished[i]));
					filenames.pop_front();
				}
			}
		}
	}
}


