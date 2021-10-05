FROM centos:centos7

# System setup
RUN yum -y update; yum clean all
RUN yum -y install centos-release-scl
RUN yum -y install devtoolset-7
SHELL [ "/usr/bin/scl", "enable", "devtoolset-7" ]

# Dependency packages install
RUN yum -y install sqlite-devel.x86_64
RUN yum -y install rsyslog && systemctl enable rsyslog

# Utilities install
RUN yum -y install git
RUN yum -y install vim

RUN /home/foldermonitor/build.sh
RUN /home/foldermonitor/test.sh
#RUN bash