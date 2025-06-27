#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE "/dev/driver"

int main() {
    int fd = open(DEVICE_FILE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }
    
    char input;
    printf("Press 0 to turn ON, 1 to turn OFF, q to quit:\n");
    while (1) {
        input = getchar();
        if (input == '0') {
            write(fd, "bat", 3);
            printf("\nGPIO turned ON\n");
        } else if (input == '1') {
            write(fd, "tat", 3);
            printf("\nGPIO turned OFF\n");
			        } else if (input == 'q') {
            printf("\nExiting...\n");
            break;
        }
    }
    
    close(fd);
    return 0;
}

