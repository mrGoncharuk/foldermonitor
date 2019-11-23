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
- - ./daemon-foldermonitor
```

## Hints
If you accidentally deleted your database inside /database/ folder
```bash
- cd database
- cat db_creation_script.sql | sqlite3 foldermonitor.db
```
Check log
```bash
- cat /var/log/maillog
```
