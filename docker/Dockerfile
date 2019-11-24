FROM centos:centos6
LABEL The CentOS Project <cloud-ops@centos.org>

RUN yum -y update; yum clean all
RUN yum -y install git
RUN yum -y install vim
RUN yum -y install sqlite-devel.x86_64
RUN yum -y install centos-release-scl
RUN yum -y install rsyslog
RUN yum -y install -y devtoolset-7
RUN git clone https://github.com/mrGoncharuk/foldermonitor.git /home/foldermonitor
RUN rsyslogd
RUN yum -y install gcc-c++
RUN yum -y install wget
WORKDIR /home
RUN wget http://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
RUN yum -y upgrade
RUN yum -y install devtoolset-2-gcc devtoolset-2-binutils devtoolset-2-gcc-c++
# RUN yum -y update scl-utils
# RUN echo 'source scl_source enable devtoolset-2' >> ~/.bashrc
# CMD scl enable devtoolset-2 bash
WORKDIR /home/foldermonitor
RUN source /opt/rh/devtoolset-2/enable
# RUN /opt/tools/rh/devtools-2/root/usr/bin/g++ --version
RUN rm -rf /home/foldermonitor
RUN git clone https://github.com/mrGoncharuk/foldermonitor.git /home/foldermonitor
RUN ls /opt/rh
RUN echo "source /opt/rh/devtoolset-2/enable" >> ~/.bashrc
# RUN source /opt/rh/devtoolset-2/enable
# RUN scl enable devtoolset-7 bash
RUN make -C /home/foldermonitor
RUN pwd
RUN ls
CMD ./daemon-foldermonitor

#RUN bash