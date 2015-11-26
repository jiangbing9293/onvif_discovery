GSOAP_ROOT = /home/admin/onvif/gsoap-2.8/gsoap

CC = gcc -g -DWITH_NONAMESPACES

INCLUDE = -I$(GSOAP_ROOT)

SERVER_OBJS = soapC.o stdsoap2.o  soapClient.o soapServer.o deviceserver.o  wsddapi.o  wsaapi.o

CLIENT_OBJS = soapC.o stdsoap2.o soapClient.o deviceprobe.o 

all: server client

server: $(SERVER_OBJS)
	$(CC) $(INCLUDE) -o deviceserver $(SERVER_OBJS)
        
client: $(CLIENT_OBJS)
	$(CC) $(INCLUDE) -o deviceprobe $(CLIENT_OBJS)
        
clean: 
	rm -f *.o deviceprobe deviceserver
