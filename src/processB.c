#include <bmpfile.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "./../include/processB_utilities.h"
#include "./../include/circle_utilities.h"
#define NAME "out/prova"

int main(int argc, char const *argv[])
{
    // instantiation of the shared memory
    const char * shm_name = "/AOS";
    const int SIZE = width*height*sizeof(rgb_pixel_t);
    int shm_fd;
    rgb_pixel_t * ptr;
    shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == 1) {
        printf("Shared memory segment failed\n");
        exit(1);
    }

    ptr = (rgb_pixel_t *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return 1;
    }

    // DA GESTIRE IN VERSIONE FINALE, QUANDO PROGRAMMA CHIUDE
    //munmap(ptr, SIZE);


    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // array for the center
    coordinate *center = NULL;

    center = (coordinate*) malloc(10 * sizeof (coordinate));

    center[0].x = width/2;
    center[0].y = height/2;

    

    // normalized center for the ncurse window
    int cx, cy;

    // Initialize UI
    init_console_ui();

    // Data structure for storing the bitmap file
    bmpfile_t *bmp;
    
    bmp = bmp_create(width, height, depth);

    circle_draw(800, 300,bmp);

    bmp_save(bmp, "out/SNOP.bmp");

    
    // Infinite loop
    while (TRUE) {

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
        else {
            delete(center[0].x, center[0].y,bmp);
            center[0] = find_center(bmp, ptr);
            bmp_save(bmp, "out/SNOP.bmp");
            cx = center[0].x/20;
            cy = center[0].y/20;
            mvaddch(center[0].y, center[0].x, '0');
            refresh();
            sleep(10);
        }
    }

    endwin();
    return 0;
}
