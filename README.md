# foldermonitor
Foldermonitor is daemon program which monitors chosen folder, reads files, parses data from it(expects one key:value data inside single file) and writes parsed data to database. Also used docker container for deploying purpose with centOS7 as base and SQLite as database.

## Installation
Clone repository
```bash
git clone https://github.com/mrGoncharuk/foldermonitor.git
```
Build image with application
```bash
docker-compose build
```

## Hints
Check database records
```bash
sqlite3 /home/foldermonitor/database/foldermonitor.db "select * from filedata"
```

If you accidentally deleted your database you can easily restore it.
```bash
cd foldermonitor/database
cat db_creation_script.sql | sqlite3 foldermonitor.db
```

Check log
```bash
cat /var/log/maillog
```
Change devtoolset(i.e to build daemon)
```bash
scl enable devtoolset-7 bash
``` 

If you don't see any logs in /var/log/maillog, restart rsyslog
```bash
systemctl restart rsyslog
```
