#include <iostream>
#include "../includes/DirectoryMonitor.hpp"
#include "../includes/DbWriter.hpp"
#include "../includes/MigrationManager.hpp"
#include <string>
#include <syslog.h>
#include <mutex>
#include <list>
#include <atomic>
#include <sys/stat.h>

static unsigned int		countThreadAmount()
{
	unsigned int threads = std::thread::hardware_concurrency();
	syslog(LOG_NOTICE, "There are %d threads available in system.", threads);
	if (threads <= 3)
		return (1);
	else
		return (threads - 2);
}

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
	std::atomic<bool> isRunning;
	std::mutex l_mutex;
	const std::string watching_folder = "/home/mhonchar/Documents/fm_test/monitorme/";
	const std::string database_folder = "/home/mhonchar/Documents/fm_test/database/foldermonitor.db";
	DirectoryMonitor dm(watching_folder);
	DBWriter *dbw = new DBWriter(database_folder);
	MigrationManager migManager(dbw, countThreadAmount());

	if (dm.initWatcher() == false)
		exit(EXIT_FAILURE);
	if (dbw->initDB() == false)
		exit(EXIT_FAILURE);
	isRunning = true;

	std::thread threadFWatcher
		( &DirectoryMonitor::startWatching
			, std::ref(dm)
			, std::ref(l_mutex)
			, std::ref(filenames)
			, std::ref(isRunning)
		);
	migManager.migrateData(l_mutex, filenames, isRunning);
	threadFWatcher.join();
	dbw->closeDB();
	delete dbw;
	syslog(LOG_NOTICE, "Stopping daemon-foldermonitor.");
	closelog();
	exit(EXIT_SUCCESS);
}
