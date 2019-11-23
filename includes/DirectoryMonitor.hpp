//
// Created by mhonchar on 21.11.19.
//

#ifndef DIRECTORYMONITOR_HPP
# define DIRECTORYMONITOR_HPP

# include <string>
# include <sys/inotify.h>
# include <unistd.h>
# include <iostream>
# include <thread>
# include <mutex>
# include <syslog.h>
# include <list>
# include <atomic>

class DirectoryMonitor
{
private:
	int		fileDescriptor;
	int		watchDescriptor;
	std::string	fname;

public:
	DirectoryMonitor(const std::string &path);
	~DirectoryMonitor();
	void startWatching(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning);
	bool initWatcher();

private:
	DirectoryMonitor();
	DirectoryMonitor(DirectoryMonitor const &rhs);
	DirectoryMonitor &operator=(DirectoryMonitor const &rhs);

public:
	static const int MaxEventMonitor;
	static const int MaxFilenameSize;
	static const int MonitorEventSize;
	static const int BufferSize;
};


#endif //DIRECTORYMONITOR_HPP
