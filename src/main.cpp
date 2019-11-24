#include <iostream>
#include "../includes/DirectoryMonitor.hpp"
#include "../includes/DbWriter.hpp"
#include <string>
#include <syslog.h>
#include <mutex>
#include <list>
#include <atomic>
#include <sys/stat.h>

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


	std::list<std::string> filenames;
	std::mutex l_mutex;
	const std::string watching_folder = "/home/foldermonitor/monitorme/";
	const std::string database_folder = "/home/foldermonitor/database/foldermonitor.db";

	DirectoryMonitor dm(watching_folder);
	DBWriter dbw(database_folder, watching_folder);
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

	dbw.startWriting(l_mutex, filenames, isRunning);
	threadFWatcher.join();
	syslog(LOG_NOTICE, "Stopping daemon-foldermonitor.");
	closelog();
	exit(EXIT_SUCCESS);
}
