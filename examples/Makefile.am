AM_CPPFLAGS = $(CXX11_FLAGS) -I$(srcdir)/../src
AM_CXXFLAGS = $(PICKY_CXXFLAGS)
LDADD = ../src/libsourdough.a -lpthread

bin_PROGRAMS = udpclient udpserver

udpclient_SOURCES = udpclient.cc

udpserver_SOURCES = udpserver.cc
