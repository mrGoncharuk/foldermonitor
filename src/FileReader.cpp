//
// Created by mhonchar on 24.11.19.
//

#include "../includes/FileReader.hpp"
#include <unistd.h>
FileReader::FileReader()
{}

FileReader::~FileReader()
{}

std::string FileReader::extractData(const std::string &fname)
{
	syslog(LOG_NOTICE, "FR> Start processing <%s>", fname.c_str());
	std::string		query = "INSERT INTO filedata VALUES('";
	std::ifstream 	ifs(fname.c_str());
	std::string		data;
	if (ifs)
	{
		std::getline(ifs, data, ':');
		query = query + data + "','";
		std::getline(ifs, data, ':');
		query = query + data + "');";
		ifs.close();
		syslog(LOG_NOTICE, "FR> first query <%s>", query.c_str());
		syslog(LOG_NOTICE, "FR> File <%s>, processed successfully.", fname.c_str());
	}
	else
		syslog(LOG_ERR, "FR> Error while opening file <%s>", fname.c_str());
	return (query);
}
