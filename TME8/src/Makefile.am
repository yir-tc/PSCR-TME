# noinst = no install, on peut utiliser bin_PROGRAMS sinon
noinst_PROGRAMS = client server server2 ftp_server ftp_client

INC_DIR = $(top_builddir)/include
SRC_DIR = $(top_builddir)/src

# Flags to compiler
AM_CPPFLAGS =  -I $(INC_DIR) -g -Wall -std=c++1y

# global LDFLAG  = Linker flags
AM_LDFLAGS = -pthread

# version locale a une cible
#cible_LDFLAGS = -pthread 

server_SOURCES = Socket.h Socket.cpp ServerSocket.h ServerSocket.cpp server.cpp

client_SOURCES = Socket.h Socket.cpp client.cpp
 
server2_SOURCES = Socket.h Socket.cpp TCPServer.h TCPServer.cpp ServerSocket.h ServerSocket.cpp server2.cpp

ftp_server_SOURCES = Socket.cpp Socket.h TCPServer.cpp ServerSocket.cpp FTPServer.cpp

ftp_client_SOURCES = FTPClient.cpp Socket.cpp



