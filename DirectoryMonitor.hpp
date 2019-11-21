//
// Created by mhonchar on 21.11.19.
//

#ifndef DIRECTORYMONITOR_HPP
# define DIRECTORYMONITOR_HPP

# include <string>
# include <sys/inotify.h>
# include <unistd.h>
# include <iostream>
class DirectoryMonitor
{
private:
	int		fileDescriptor;
	int		watchDescriptor;
	std::string	fname;
public:
	DirectoryMonitor();
	~DirectoryMonitor();
	DirectoryMonitor(DirectoryMonitor const &rhs);
	DirectoryMonitor &operator=(DirectoryMonitor const &rhs);
	DirectoryMonitor(const std::string path);
	bool startWatching();

	std::string getFname() const;
	void	setFname(std::string const p_fname);
public:
	static const int MaxEventMonitor;
	static const int MaxFilenameSize;
	static const int MonitorEventSize;
	static const int BufferSize;
};


#endif //DIRECTORYMONITOR_HPP
