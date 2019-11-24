//
// Created by mhonchar on 24.11.19.
//

#include "../includes/FileReader.hpp"

FileReader::FileReader()
{}

FileReader::~FileReader()
{}

void FileReader::extractData(const std::string &fname,
							std::string &key,
							std::string &value)
{
	syslog(LOG_NOTICE, "FR> Start processing <%s>", fname.c_str());
	std::ifstream ifs(fname.c_str());
	if (ifs)
	{
		std::getline(ifs, key, ':'); // get key
		std::getline(ifs, value, ':'); // get key
		ifs.close();
		syslog(LOG_NOTICE, "FR> File <%s>, processed successfully.", fname.c_str());

	}
	else
		syslog(LOG_ERR, "FR> Error while opening file <%s>", fname.c_str());
}
