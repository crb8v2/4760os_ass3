CC = gcc
CFLAGS = -g -pthread

TARGET1 = oss
TARGET2 = user
OBJS1 = oss.o
OBJS2 = user.o

all: $(TARGET1) $(TARGET2)

oss: $(OBJS1)
	$(CC) $(CFLAGS) -o oss oss.o

user: $(OBJS2)
	$(CC) $(CFLAGS) -o user user.o

clean:
	/bin/rm -f *.o
	/bin/rm -f $(TARGET1)
	/bin/rm -f $(TARGET2)
	/bin/rm -f  text.txt
