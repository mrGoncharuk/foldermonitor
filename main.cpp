#include <iostream>
#include "DirectoryMonitor.hpp"
#include "DbWriter.hpp"
#include <string>
#include <syslog.h>
#include <mutex>
#include <list>
#include <atomic>

int main()
{
	std::list<std::string> filenames;
	std::mutex l_mutex;
	DirectoryMonitor dm("/home/mhonchar/Documents/foldermonitor/monitorme");
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
	int a;
	std::cin >> a;
	isRunning = false;
	threadFWatcher.detach();
	threadDBWriter.detach();
	return 0;
}

/*
// For security purposes, we don't allow any arguments to be passed into the daemon
int main(void)
{
	// Define variables
	pid_t pid, sid;

	// Fork the current process
	pid = fork();
	// The parent process continues with a process ID greater than 0
	if(pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
		// A process ID lower than 0 indicates a failure in either process
	else if(pid < 0)
	{
		exit(EXIT_FAILURE);
	}
	// The parent process has now terminated, and the forked child process will continue
	// (the pid of the child process was 0)

	// Since the child process is a daemon, the umask needs to be set so files and logs can be written
	umask(0);

	// Open system logs for the child process
	openlog("daemon-monitor", LOG_NOWAIT | LOG_PID, LOG_USER);
	syslog(LOG_NOTICE, "Successfully started daemon-name");

	// Generate a session ID for the child process
	sid = setsid();
	// Ensure a valid SID for the child process
	if(sid < 0)
	{
		// Log failure and exit
		syslog(LOG_ERR, "Could not generate session ID for child process");

		// If a new session ID could not be generated, we must terminate the child process
		// or it will be orphaned
		exit(EXIT_FAILURE);
	}

	// Change the current working directory to a directory guaranteed to exist
	if((chdir("/")) < 0)
	{
		// Log failure and exit
		syslog(LOG_ERR, "Could not change working directory to /");

		// If our guaranteed directory does not exist, terminate the child process to ensure
		// the daemon has not been hijacked
		exit(EXIT_FAILURE);
	}

	// A daemon cannot use the terminal, so close standard file descriptors for security reasons
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Daemon-specific intialization should go here
	const int SLEEP_INTERVAL = 5;

	// Enter daemon loop
	while(1)
	{
		// Execute daemon heartbeat, where your recurring activity occurs
		for (int i = 0; i < 1000000; i++)
			i++;
		// Sleep for a period of time
		sleep(SLEEP_INTERVAL);
		syslog(LOG_NOTICE, "End of loopa");
		syslog(LOG_ALERT, "Alert log");
		syslog(LOG_DAEMON, "Deamon_LOGggg");
		syslog(LOG_SYSLOG, "syslog log");

	}

	// Close system logs for the child process
	syslog(LOG_NOTICE, "Stopping daemon-name");
	closelog();

	// Terminate the child process when the daemon completes
	exit(EXIT_SUCCESS);
}
*/