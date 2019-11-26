//
// Created by mhonchar on 21.11.19.
//

#include "../includes/DbWriter.hpp"
#include <iostream>

DBWriter::DBWriter(std::string const &dbfilename)
	: db(nullptr)
	, dbName(dbfilename)
{
	syslog(LOG_NOTICE, "DW> Database writer created.");
}

DBWriter::~DBWriter()
{
	syslog(LOG_NOTICE, "DW> Database writer destroyed.");
}
bool 	DBWriter::initDB()
{
	int status;

	status = sqlite3_open(dbName.c_str(), reinterpret_cast<sqlite3 **>(&db));
	if (status != SQLITE_OK)
	{
		syslog(LOG_ERR, "DW> Error occured while opening database.");
		syslog(LOG_NOTICE, "DW> Daemon stops.");
		return false;
	}
	syslog(LOG_NOTICE, "DW> Database writer initialized successfully.");
	return true;
}

void 	DBWriter::executeQuery(const std::string &query)
{
	int ret;
	char *errmsg;

	std::lock_guard<std::mutex> lock(dbMutex);
	ret = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errmsg);
	if (ret == SQLITE_OK)
		syslog(LOG_NOTICE, "DW> Query executed successfully.");
	else
	{
		syslog(LOG_ERR, "DW> Error while migrating data from. Query: [%s]. Error message: %s.", query.c_str(), errmsg);
		sqlite3_free(errmsg);
	}
}

void 	DBWriter::closeDB()
{
	sqlite3_close(db);
	syslog(LOG_NOTICE, "DW> Database closed.");
}
