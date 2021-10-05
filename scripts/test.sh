# !/bin/bash

# Start daemon
cd /home/foldermonitor && ./daemon-foldermonitor

## Perform tests
# Copy files to folder which under investigation
cp -a /home/foldermonitor/testfiles/test* /home/foldermonitor/monitorme
# Wait until daemon finish processing datafiles and writing to database 
sleep 3
sqlite3 /home/foldermonitor/database/foldermonitor.db "select * from filedata"
