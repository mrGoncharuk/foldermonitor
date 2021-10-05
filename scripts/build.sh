# !/bin/bash

# Build daemon
cd /home/foldermonitor
make

# Create database
cd /home/foldermonitor/database && \
    cat db_creation_script.sql | sqlite3 foldermonitor.db
