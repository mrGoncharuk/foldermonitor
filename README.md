# foldermonitor

## Installation
Clone repository
```bash
- git clone https://github.com/mrGoncharuk/foldermonitor.git
```
Open repository directory
```bash
- cd foldermonitor
```
Open docker folder
```bash
- cd docker
```
Build docker image
```bash
- sudo docker build -t <ImageName>:<ImageTag> .
```
After building start docker container
```bash
- sudo docker run --name <ContainerName> -it <ImageName>:<ImageTag> bash
```
## Prepairing system to start daemon

Change devtoolset
```bash
- scl enable devtoolset-7 bash
```

Start rsyslogd
```bash
- rsyslogd
```

Open daemon directory. Its already cloned into /home/foldermonitor
```bash
- cd /home/foldermonitor
```

Build
```bash
- make
```

Run
```bash
- ./daemon-foldermonitor
```

## Hints
Check database records
```bash
- cd foldermonitor/database
- sqlite3 foldermonitor.db
- select * from filedata
```

If you accidentally deleted your database you can easily restore it.
```bash
- cd foldermonitor/database
- cat db_creation_script.sql | sqlite3 foldermonitor.db
```

Check log
```bash
- cat /var/log/maillog
```

If daemon doesn't compile run(make: g++: Command not found), change devtoolset
```bash
- scl enable devtoolset-7 bash
```

If you don't see any logs in /var/log/maillog, start rsyslogd
```bash
- rsyslogd
```


