FROM centos:centos6
MAINTAINER The CentOS Project <cloud-ops@centos.org>

#RUN yum -y update; yum clean all
RUN yum -y install git
RUN yum -y install gcc-c++
RUN yum -y install vim
#sudo apt-get install sqlite3 libsqlite3-dev
RUN git clone https://github.com/mrGoncharuk/foldermonitor.git home/deamon
RUN cd home/deamon
RUN make
# RUN ./folderdeamon

#RUN bash