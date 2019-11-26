//
// Created by mhonchar on 26.11.19.
//

#ifndef FOLDERMONITOR_MIGRATIONMANAGER_HPP
# define FOLDERMONITOR_MIGRATIONMANAGER_HPP


# include <mutex>
# include <list>
# include <thread>
# include <chrono>
# include "FileReader.hpp"
# include "IDB.hpp"

class MigrationManager
{
private:
	IDB *dbwriter;
	const unsigned int threadAmount;
	void 	migrateThreaded(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning);
	void 	migrateSolo(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning);
public:
	explicit MigrationManager(IDB *db, unsigned int availableThreads);
	~MigrationManager();
	void 	migrateData(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning);
};


#endif //FOLDERMONITOR_MIGRATIONMANAGER_HPP
