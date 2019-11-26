//
// Created by mhonchar on 26.11.19.
//

#include "../includes/MigrationManager.hpp"

MigrationManager::MigrationManager(IDB *db,  unsigned int availableThreads)
	: dbwriter(db)
	, threadAmount(availableThreads)
{
	syslog(LOG_NOTICE, "MM> Migration manager created.");
}

MigrationManager::~MigrationManager()
{}

void 	MigrationManager::migrateSolo(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	std::string currFileName;

	while (isRunning)
	{
		list_mutex.lock();
		if (filenames.empty())
			list_mutex.unlock();
		else
		{
			currFileName = filenames.front();
			filenames.pop_front();
			list_mutex.unlock();
			std::string query = FileReader::extractData(currFileName);
			dbwriter->executeQuery(query);
		}
	}
}

void	MigrationManager::migrateThreaded(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	std::string currFileName;
	std::thread migrateThreads[threadAmount];
	std::atomic<bool> isThreadFinished[threadAmount];
	unsigned int i = 0;

	while (i < threadAmount && isRunning)
	{
		std::lock_guard<std::mutex> lock(list_mutex);
		if (!filenames.empty())
		{
			isThreadFinished[i] = false;
			currFileName = filenames.front();
			filenames.pop_front();
			migrateThreads[i] = std::thread(
				[currFileName](IDB *idb, std::atomic<bool> &isReady)
				{
					std::string query = FileReader::extractData(currFileName);
					idb->executeQuery(query);
					isReady = true;
				},
				this->dbwriter, std::ref(isThreadFinished[i]));
			i++;
		}
	}
	while (isRunning)
	{
		for (i = 0; i < threadAmount; i++)
		{
			if (isThreadFinished[i])
			{
				std::lock_guard<std::mutex> lock(list_mutex);
				if (!filenames.empty())
				{
					migrateThreads[i].join();
					isThreadFinished[i] = false;
					currFileName = filenames.front();
					filenames.pop_front();
					migrateThreads[i] = std::thread(
						[currFileName](IDB *idb, std::atomic<bool> &isReady)
						{
							std::string query = FileReader::extractData(currFileName);
							idb->executeQuery(query);
							isReady = true;
						},
						this->dbwriter, std::ref(isThreadFinished[i]));
					i++;
				}
			}
		}
	}
	syslog(LOG_NOTICE, "MM> Waiting for end of threads task.");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	for (i = 0; i < threadAmount; i++)
	{
		if (isThreadFinished[i])
			migrateThreads[i].join();
	}
}

void	MigrationManager::migrateData(std::mutex &list_mutex, std::list<std::string> &filenames, std::atomic<bool> &isRunning)
{
	if (threadAmount == 1)
	{
		syslog(LOG_NOTICE, "MM> Data migration will be executed in solo thread mode.");
		migrateSolo(list_mutex, filenames, isRunning);
	}
	else
	{
		syslog(LOG_NOTICE, "MM> Data migration will be executed in <%d> threads.", threadAmount);
		migrateThreaded(list_mutex, filenames, isRunning);
	}
}
