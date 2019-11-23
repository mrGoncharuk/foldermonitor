//
// Created by mhonchar on 21.11.19.
//

#include "DbWriter.hpp"
#include <iostream>
const int DBWriter::threadAmount = 3;

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
	{
		syslog(LOG_ERR, "Error occured while opening database.");
		throw "Error occured while opening database.";
	}
	syslog(LOG_NOTICE, "Database writer initialized successfully.");
}

void 	DBWriter::migrateData(const std::string fname, std::atomic<bool> &isDone)
{
	int ret;
	char *errmsg;
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
		ifs.close();
		std::lock_guard<std::mutex> lock(dbMutex);
		ret = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errmsg);
		if (ret == SQLITE_OK)
			syslog(LOG_NOTICE, "Data from file <%s> migrated to database successfully.", fname.c_str());
		else
		{
			syslog(LOG_ERR, "Error while migrating data from <%s>. Query: [%s]. Error message: %s.", fname.c_str(), query.c_str(), errmsg);
			sqlite3_free(errmsg);
		}
	}
	else
		syslog(LOG_ERR, "Error while opening file <%s>", fname.c_str());
	isDone = true;
}

void	DBWriter::startWriting(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	std::string currFileName;
	std::thread migrateThreads[threadAmount];
	std::atomic<bool> isThreadFinished[threadAmount];

	int i = 0;
	while (i < threadAmount)
	{
		std::lock_guard<std::mutex> lock(list_mutex);
		if (!filenames.empty())
		{
			isThreadFinished[i] = false;
			migrateThreads[i] = std::thread(&DBWriter::migrateData, std::ref(*this), pathToMonitor + filenames.front(),
											std::ref(isThreadFinished[i]));
			syslog(LOG_NOTICE, "Start processing <%s>.", filenames.front().c_str());
			filenames.pop_front();
			i++;
		}
	}
	while (isRunning)
	{
		for (int i = 0; i < threadAmount; i++)
		{
			if (isThreadFinished[i])
			{
				std::lock_guard<std::mutex> lock(list_mutex);
				if (!filenames.empty())
				{
					migrateThreads[i].join();
					migrateThreads[i] = std::thread(&DBWriter::migrateData, std::ref(*this), pathToMonitor + filenames.front(),
											  std::ref(isThreadFinished[i]));
					syslog(LOG_NOTICE, "Start processing <%s>.", filenames.front().c_str());
					filenames.pop_front();
				}
			}
		}
	}
}


