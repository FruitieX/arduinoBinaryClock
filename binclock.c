#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>
#include <termios.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: binclock /path/to/ttyUSBx\n");
        exit(EXIT_FAILURE);
    }

    int ttyfd = open(argv[1], O_WRONLY);
    speed_t baud = B9600;

    struct termios settings;
    tcgetattr(ttyfd, &settings);
    cfsetospeed(&settings, baud);

    uint32_t timeInt;
    char timeStr[6] = {'\0'};
    time_t rawtime;
    struct tm *info;

    while(1) {
        time(&rawtime);
        info = localtime(&rawtime);

        timeInt = info->tm_hour * 60 * 60 + info->tm_min * 60 + info->tm_sec;
        snprintf(timeStr, 6, "%u\n", timeInt);
        write(ttyfd, timeStr, strlen(timeStr) + 1); // include null byte
        usleep(1000 * 1000); // 500 ms
    }
};
