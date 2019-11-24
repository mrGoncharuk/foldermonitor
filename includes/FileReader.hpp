//
// Created by mhonchar on 24.11.19.
//

#ifndef FOLDERMONITOR_FILEREADER_HPP
#define FOLDERMONITOR_FILEREADER_HPP

# include <fstream>
# include <string>
# include <atomic>
# include <syslog.h>

class FileReader
{
public:
	FileReader();
	~FileReader();
	static void extractData(const std::string &fname,
							std::string &key,
							std::string &value);
};


#endif //FOLDERMONITOR_FILEREADER_HPP
