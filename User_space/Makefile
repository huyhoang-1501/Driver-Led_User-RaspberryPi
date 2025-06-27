CROSS_COMPILE = aarch64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
TARGET = user_control

# File nguồn
SRC = user_control.c

all:
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

deploy:
	scp $(TARGET) pi@192.168.137.155:/home/pi
