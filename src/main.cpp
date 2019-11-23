#include <iostream>
#include "../includes/DirectoryMonitor.hpp"
#include "../includes/DbWriter.hpp"
#include <string>
#include <syslog.h>
#include <mutex>
#include <list>
#include <atomic>
#include <sys/stat.h>
/*
int main()
{
	std::list<std::string> filenames;
	std::mutex l_mutex;
	DirectoryMonitor dm("/home/mhonchar/Documents/foldermonitor/monitorme/");
	DBWriter dbw("/home/mhonchar/Documents/foldermonitor/database/foldermonitor.db", "/home/mhonchar/Documents/foldermonitor/monitorme/");
	std::atomic<bool> isRunning;
	try
	{
		dm.initWatcher();
		dbw.initDBWriter();
	}
	catch (std::string e)
	{
		std::cout << "Error occured: " << e << std::endl;
		return (-1);
	}
	isRunning = true;
	std::thread threadFWatcher
		( &DirectoryMonitor::startWatching
		, std::ref(dm)
		, std::ref(l_mutex)
		, std::ref(filenames)
		, std::ref(isRunning)
		);
	std::thread threadDBWriter
		( &DBWriter::startWriting
		, std::ref(dbw)
		, std::ref(l_mutex)
		, std::ref(filenames)
		, std::ref(isRunning)
		);

	dbw.startWriting(l_mutex, filenames, isRunning);

	isRunning = false;
	threadFWatcher.join();
	threadDBWriter.join();
	return 0;
}
*/

// For security purposes, we don't allow any arguments to be passed into the daemon
int main(void)
{
	pid_t pid, sid;

	pid = fork();
	if(pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if(pid < 0)
	{
		exit(EXIT_FAILURE);
	}
	umask(0);


	openlog("daemon-foldermonitor", LOG_NOWAIT | LOG_PID, LOG_MAIL);
	syslog(LOG_NOTICE, "Successfully started daemon-foldermonitor");

	sid = setsid();
	if(sid < 0)
	{
		syslog(LOG_ERR, "Could not generate session ID for child process");
		exit(EXIT_FAILURE);
	}

	if((chdir("/")) < 0)
	{
		syslog(LOG_ERR, "Could not change working directory to /");
		exit(EXIT_FAILURE);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	const int SLEEP_INTERVAL = 5;
	std::list<std::string> filenames;
	std::mutex l_mutex;
	DirectoryMonitor dm("/home/deamon/monitorme");
	DBWriter dbw("/home/deamon/database/foldermonitor.db", "/home/deamon/monitorme/");
	std::atomic<bool> isRunning;
	if (dm.initWatcher() == false)
		exit(EXIT_FAILURE);
	if (dbw.initDBWriter() == false)
		exit(EXIT_FAILURE);
	isRunning = true;
	std::thread threadFWatcher
		( &DirectoryMonitor::startWatching
			, std::ref(dm)
			, std::ref(l_mutex)
			, std::ref(filenames)
			, std::ref(isRunning)
		);
	std::thread threadDBWriter
		( &DBWriter::startWriting
			, std::ref(dbw)
			, std::ref(l_mutex)
			, std::ref(filenames)
			, std::ref(isRunning)
		);
	while(isRunning)
	{
		sleep(SLEEP_INTERVAL);
	}
	threadFWatcher.join();
	threadDBWriter.join();
	syslog(LOG_NOTICE, "Stopping daemon-foldermonitor.");
	closelog();
	exit(EXIT_SUCCESS);
}
