# !/bin/bash

cd /home/foldermonitor && ./foldermonitor
cp -a /home/foldermonitor/testfiles/test* /home/foldermonitor/monitorme
sqlite3 /home/foldermonitor/database/foldermonitor.db "select * from filedata"