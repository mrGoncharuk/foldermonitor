FROM centos:centos6
MAINTAINER The CentOS Project <cloud-ops@centos.org>

RUN yum -y update; yum clean all
RUN yum -y install git
RUN yum -y install vim
RUN yum -y install sqlite-devel.x86_64
RUN yum -y install centos-release-scl
RUN yum -y install rsyslog
RUN yum -y install -y devtoolset-7
# RUN scl enable devtoolset-7 bash
RUN git clone https://github.com/mrGoncharuk/foldermonitor.git /home/foldermonitor
RUN bash /home/foldermonitor/docker/prepairing_script
# RUN cd /home/foldermonitor/database
# RUN cat /home/foldermonitor/database/db_creation_script.sql | sqlite3 /home/foldermonitor/database/foldermonitor.db
# RUN cd ..
# RUN rsyslogd
# RUN mkdir monitorme
# RUN make


#RUN bash