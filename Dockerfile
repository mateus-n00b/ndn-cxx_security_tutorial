FROM ubuntu:18.04

RUN apt-get update -y &&\
	apt install doxygen git sudo graphviz python3-pip libpcap-dev libsystemd-dev -y &&\
	apt install g++ pkg-config python3-minimal libboost-all-dev libssl-dev libsqlite3-dev -y &&\
	pip3 install sphinx sphinxcontrib-doxylink 

RUN git clone https://github.com/named-data/ndn-cxx 
RUN git clone --recursive https://github.com/named-data/NFD.git


RUN cd ndn-cxx && ./waf configure --with-examples && ./waf && ./waf install
RUN cd NFD && ./waf configure && ./waf && ./waf install ; ldconfig
RUN cp /usr/local/etc/ndn/nfd.conf.sample /usr/local/etc/ndn/nfd.conf

WORKDIR ndn-cxx

RUN apt-get autoremove && apt-get clean 	

RUN nfd-start
