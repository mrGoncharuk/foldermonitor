//
// Created by mhonchar on 21.11.19.
//

#include "../includes/DirectoryMonitor.hpp"
#include <chrono>

const int DirectoryMonitor::MaxEventMonitor = 256;
const int DirectoryMonitor::MaxFilenameSize = 32;
const int DirectoryMonitor::MonitorEventSize = sizeof(struct inotify_event);
const int DirectoryMonitor::BufferSize = MaxEventMonitor * (MonitorEventSize  + MaxFilenameSize);

DirectoryMonitor::DirectoryMonitor()
	: fileDescriptor(-1)
	, watchDescriptor(-1)
	, fname("")
{}

DirectoryMonitor::~DirectoryMonitor()
{
	syslog(LOG_NOTICE, "DM> File watcher destroyed.");
	if (fileDescriptor > 0 && watchDescriptor > 0)
		inotify_rm_watch(fileDescriptor, watchDescriptor);
	if (fileDescriptor > 0)
		close(fileDescriptor);
}

DirectoryMonitor::DirectoryMonitor(DirectoryMonitor const &rhs)
{
	*this = rhs;
}

DirectoryMonitor &DirectoryMonitor::operator=(DirectoryMonitor const &rhs)
{
	this->fileDescriptor = rhs.fileDescriptor;
	this->watchDescriptor = rhs.watchDescriptor;
	this->fname = rhs.fname;
	return (*this);
}

DirectoryMonitor::DirectoryMonitor(const std::string &path)
	: fileDescriptor(-1)
	, watchDescriptor(-1)
	, fname(path)
{
	syslog(LOG_NOTICE, "DM> File watcher created.");
}

bool DirectoryMonitor::initWatcher()
{
	fileDescriptor = inotify_init();
	if (fileDescriptor < 0)
	{
		syslog (LOG_ERR, "DM> Error while inotify initialization.");
		syslog (LOG_NOTICE, "DM> Daemon stops.");
		return false;
	}
	watchDescriptor = inotify_add_watch(fileDescriptor, fname.c_str(), IN_CREATE);
	if (watchDescriptor < 0)
	{
		syslog (LOG_ERR, "DM> Error while starting watching folder.");
		syslog (LOG_NOTICE, "DM> Daemon stops.");
		return false;
	}
	syslog(LOG_NOTICE, "DM> File watcher initialized successfully.");
	return true;
}

void	DirectoryMonitor::startWatching(std::mutex &p_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	char buffer[BufferSize];
	int i;
	int total_read;
	while (isRunning)
	{
		i = 0;
		total_read = read(fileDescriptor, buffer, BufferSize);
		if (total_read < 0)
			syslog (LOG_ERR, "DM> Error while reading folder events.");
		while (i < total_read)
		{
			inotify_event *event = reinterpret_cast<inotify_event *>(buffer + i);
			if (event->len)
			{
				if (!(event->mask & IN_ISDIR))
				{
					std::lock_guard<std::mutex> lock(p_mutex);
					syslog (LOG_INFO, "DM> File created <%s>", event->name);

					filenames.push_back(event->name);
				}
			}
			i += MonitorEventSize + event->len;
		}
	}
}
