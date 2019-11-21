//
// Created by mhonchar on 21.11.19.
//

#include "DirectoryMonitor.hpp"

const int DirectoryMonitor::MaxEventMonitor = 1024;
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
	std::cout << "file closed\n";
	inotify_rm_watch(fileDescriptor, watchDescriptor);
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
}

DirectoryMonitor::DirectoryMonitor(const std::string path)
	: fileDescriptor(-1)
	, watchDescriptor(-1)
	, fname(path)
{}

bool	DirectoryMonitor::startWatching()
{
	fileDescriptor = inotify_init();
	if (fileDescriptor < 0)
		throw "Error while inotify initialization.";		//TODO: Replace by own exception
	watchDescriptor = inotify_add_watch(fileDescriptor, fname.c_str(), IN_CREATE);
	if (watchDescriptor < 0)
		throw "Error while starting watching folder.";		//TODO: Replace by own exception


	char buffer[BufferSize];
	int i;
	int total_read;
	while (true)
	{
		i = 0;
		total_read = read(fileDescriptor, buffer, BufferSize);
		if (total_read < 0)
			throw "Error while reading.";					//TODO: Replace by own exception
		while (i < total_read)
		{
			inotify_event *event = reinterpret_cast<inotify_event *>(buffer + i);
			if (event->len)
			{
				if (!(event->mask & IN_ISDIR))
					std::cout << "File was created: " << event->name << std::endl;
			}
			i += MonitorEventSize + event->len;
		}
	}
}

std::string DirectoryMonitor::getFname() const { return (fname); }
void	DirectoryMonitor::setFname(std::string const p_fname) { fname = p_fname; }

