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
	static std::string extractData(const std::string &fname);
};


#endif //FOLDERMONITOR_FILEREADER_HPP
