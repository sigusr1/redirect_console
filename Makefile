GCC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread 

TARGET = server client
SERVER_DEP = test.o server.o
CLIENT_DEP = client.o

%.o : %.c
	$(GCC) -c $(CFLAGS) $^ -o $@
	
all: $(TARGET)

server: $(SERVER_DEP)
	$(GCC) $(SERVER_DEP) $(LDFLAGS) -o server


client:$(CLIENT_DEP)
	$(GCC) $(CLIENT_DEP) $(CFLAGS) -o client
	
clean :
	rm -f $(TARGET) $(SERVER_DEP) $(CLIENT_DEP)
