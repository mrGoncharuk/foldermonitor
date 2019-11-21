#include <iostream>
#include "DirectoryMonitor.hpp"

int main()
{
	std::cout << "Hello, World!" << std::endl;

	DirectoryMonitor dm("/home/mhonchar/Documents/GeeksTest/monitorMe");
	try
	{
		dm.startWatching();
	}
	catch (std::string e)
	{
		std::cout << "Error occured: " << e << std::endl;
	}
	return 0;
}


/*
 * 	void start()
	{
		mWatching   = true;
		mThread     = std::unique_ptr<std::thread>( new std::thread( [this](){
			// keep watching for modifications every ms milliseconds
			auto ms = std::chrono::milliseconds( 500 );
			while( mWatching ) {
				do {
					// iterate through each watcher and check for modification
					std::lock_guard<std::mutex> lock( mMutex );
					auto end = mFileWatchers.end();
					for( auto it = mFileWatchers.begin(); it != end; ++it ) {
						it->second.watch();
					}
					// lock will be released before this thread goes to sleep
				} while( false );

				// make this thread sleep for a while
				std::this_thread::sleep_for( ms );
			}
		} ) );
	}
 */