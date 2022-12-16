#include <stdio.h>
#include <stdlib.h>
#include <bmpfile.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "./../include/processA_utilities.h"
#include "./../include/circle_utilities.h"
#define NAME "out/snap"

// Free resources before termination
// bmp_destroy(bmp);
// TO BE HANDLED IF A SIGNAL ARRIVES (CLOSE OF THE PROGRAM)



int main(int argc, char *argv[])
{
    // instantiation of the shared memory
    const char * shm_name = "/AOS";
    const int SIZE = width*height*sizeof(rgb_pixel_t);
    int shm_fd;
    rgb_pixel_t* ptr;
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == 1) {
        printf("Shared memory segment failed\n");
        exit(1);
    }

    ftruncate(shm_fd,SIZE);

    ptr = (rgb_pixel_t *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return 1;
    }

    // DA GESTIRE IN VERSIONE FINALE, QUANDO PROGRAMMA CHIUDE
    //munmap(ptr, SIZE);
    
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // variable to name the copies of the bitmap saved from time to time
    int val = 0;

    // variable to store the old center of the circle (delete function)
    int cx = width/2, cy = height/2;

    // variable to name the bitmap file to be saved
    char msg[100];

    // Initialize UI
    init_console_ui();

    // Data structure for storing the bitmap file
    bmpfile_t *bmp;
    
    bmp = bmp_create(width, height, depth);

    circle_draw(cx,cy,bmp);
    bmp_save(bmp, NAME);
    circle_drawAOS(bmp, ptr); 

    // Infinite loop
    while (TRUE)
    {
        // Get input in non-blocking mode
        int cmd = getch();

        // If user resizes screen, re-draw UI...
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }

        // Else, if user presses print button...
        else if(cmd == KEY_MOUSE) {
            if(getmouse(&event) == OK) {
                if(check_button_pressed(print_btn, &event)) {
                    mvprintw(LINES - 1, 1, "Print button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS - BTN_SIZE_X - 2; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                    sprintf(msg,"%s%d.bmp",NAME,val);
                    val++;
                    // Save image as .bmp file
                    bmp_save(bmp, msg);
                }
            }
        }

        // If input is an arrow key, move circle accordingly...
        else if(cmd == KEY_LEFT || cmd == KEY_RIGHT || cmd == KEY_UP || cmd == KEY_DOWN) {
            move_circle(cmd);
            draw_circle();
            delete(cx,cy,bmp);
            deleteAOS(ptr);
            cx = circle.x*20;
            cy = circle.y*20;
            circle_draw(cx,cy,bmp);
            circle_drawAOS(bmp,ptr);
        }
    }
    
    endwin();
    return 0;
}
