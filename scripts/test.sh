# !/bin/bash

cd /home/foldermonitor && ./daemon-foldermonitor
cp -a /home/foldermonitor/testfiles/test* /home/foldermonitor/monitorme
sleep 3
sqlite3 /home/foldermonitor/database/foldermonitor.db "select * from filedata"