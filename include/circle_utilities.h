#include <bmpfile.h>
#include "./../include/log_handle.h"
#define radius 30

// Typedef for circle center
typedef struct {
    int x,y;
}coordinate;

 /* Instantiate bitmap, passing three parameters:
    *   - width of the image (in pixels)
    *   - Height of the image (in pixels)
    *   - Depth of the image (1 for greyscale images, 4 for colored images)
*/
int width = 1600;
int height = 600;
int depth = 4;

// Data type for defining pixel colors (BGRA)
rgb_pixel_t pixel = {255, 0, 0, 0};
rgb_pixel_t pixel_w = {255, 255, 255, 0};

// draw a circle in the center (cx,cy) for a bitmap
void circle_draw(int cx, int cy, bmpfile_t *bmp){
  for(int x = -radius; x <= radius; x++) {
    for(int y = -radius; y <= radius; y++) {
      // If distance is smaller, point is within the circle
      if(sqrt(x*x + y*y) < radius) {
          /*
          * Color the pixel at the specified (x,y) position
          * with the given pixel values
          */
          bmp_set_pixel(bmp, cx + x, cy + y, pixel);
      }
    }
  }
}

// draw a circle for the shared memory
void circle_drawAOS(bmpfile_t *bmp, rgb_pixel_t *ptr){

    rgb_pixel_t* p;
    
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){

        p = bmp_get_pixel(bmp,j,i);

        
        ptr[i+j*height].alpha = p->alpha;
        ptr[i+j*height].blue = p->blue;
        ptr[i+j*height].green = p->green;
        ptr[i+j*height].red = p->red;

      }
    }

}


// delete the old circle by colouring everything white for the bitmap
void delete(int cx, int cy, bmpfile_t *bmp){
  for(int x = -radius; x <= radius; x++) {
    for(int y = -radius; y <= radius; y++) {
      // If distance is smaller, point is within the circle
      if(sqrt(x*x + y*y) < radius) {
          /*
          * Color the pixel at the specified (x,y) position
          * with the given pixel values
          */
          bmp_set_pixel(bmp, cx + x, cy + y, pixel_w);
      }
    }
  }
}

// delete the old circle by colouring everything white in the shared memory
void deleteAOS(rgb_pixel_t *ptr){
    
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
        ptr[i+j*height].alpha = pixel_w.alpha;
        ptr[i+j*height].blue = pixel_w.blue;
        ptr[i+j*height].green = pixel_w.green;
        ptr[i+j*height].red = pixel_w.red;
      }
    }
}

// function to find the center of the shared memory
coordinate find_center(bmpfile_t *bmp, rgb_pixel_t *ptr){
  
        int first = 0, last = 0;

        char msg[100];

        coordinate center;

        sprintf(msg," AHAHAHAH loooosers");

         for(int i = 0; i < height; i++){
          for(int j = 0; j < width; j++){
             if(ptr[i+j*height].blue == pixel.blue && first == 0){
              first = j;
             }
             if(ptr[i+j*height].blue != pixel.blue && first != 0){
              last = j-1;
              break;
             }
      }

      if(last - first == 2*radius){
                center.x = (last-first)/2; // cx
                center.y = i; // cy

                sprintf(msg," AHAHAHAH first %d last %d", first, last);
                sprintf(msg," AHAHAHAH cx %d cy %d", center.x,center.y);
                file_logG("./logs/prova.txt", msg);

                break;
            }
      first = 0;
      last = 0;
    }


    circle_draw(center.x,center.y,bmp);

    return center;





}




