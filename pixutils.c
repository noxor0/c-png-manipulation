#include "pixutils.h"
#include "lodepng.h"

//private methods
static pixMap *pixMap_init(); //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
static void pixMap_reset();  //free the allocated memoray and set to zero but do not free memory for pixMap
static void pixMap_copy(pixMap *dest,pixMap *source); //copy one pixmap to another
static int pixMap_read(pixMap *p,char *filename); //read in a pixmap from file

static pixMap* pixMap_init() {
 //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
	pixMap *ptr;
	ptr = malloc(sizeof(pixMap));
	ptr->image = 0;
	ptr->pixArray = 0;
	ptr->width = 0;
	ptr->height = 0;
	return ptr;
}
pixMap* pixMap_init_filename(char *filename) {
	//use pixMap_int to create the pixMap object
	//use pixMap read to fill the fields of the object
	//return the pointer to the new object
	pixMap *ptr;
	ptr = pixMap_init();
	pixMap_read(ptr, filename);
	return ptr;

}
static int pixMap_read(pixMap *p,char *filename) {
	unsigned error;
	unsigned char* image;
	unsigned width, height;
 	//read and allocate image, read in width and height using using lodepng_decode32_file
 	//example is in lodepng/examples - one liner
	//then allocate p->pixArray to hold p->height pointers
	//set p->pixArray[0] to p->image and p->pixArray[i]=p->pixArray[i-1]+p->width
	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
	p->image = image;
	p->width = width;
	p->height = height;
	p->pixArray = malloc(p->height * sizeof(p->pixArray));
	p->pixArray[0] = (rgba*) p->image;
	int i;
  for(i = 1; i < p->height; i++) {
      p->pixArray[i] = malloc(sizeof(p->width) * sizeof(p->pixArray));
      p->pixArray[i] = p->pixArray[i - 1] + p->width;
  }
	// free(image);
	return 0;
}
static void pixMap_copy(pixMap *dest,pixMap *source) {
	// if source image is zero then reset dest and copy width and height -
	// if source image is not zero -
	//   if width or height are different -
	//     if width*height is different then we need to allocate dest->image -
	//       if dest->image is zero use malloc to allocate memory for dest->image
	//       else use realloc to allocate memory dest->image
	//
	//     if dest->height is different
	//       malloc or realloc dest->pixArray depending on whether dest->pixArray is zero
	//
	//     even if the height is the same set dest->pixArray[0] to dest->image
	// 		and dest->pixArray[i]=dest->pixArray[i-1]+source->width
	//
	//   do a memcpy from source->image to dest->image
	//   set dest->width and dest->height to source values

	if(source->image == 0) {
		pixMap_reset(dest);
		printf("source image is 0\n");
	} else {
		if(dest->width != source ->width || dest->height != source->height) {
			if(dest->width * dest->height != source->width * source->height) {
				dest->image = malloc(sizeof(rgba) * source->height * source->width);
			} else {
				dest->image = realloc(dest->image, sizeof(rgba) * source->height * source->width);
			}
			if(dest->height != source->height) {
				if (dest->pixArray == 0) {
					dest->pixArray = malloc(sizeof(dest) * source->height);
				} else {
					dest->pixArray = realloc(dest->pixArray, sizeof(dest) * source->height);
				}
			}
			dest->pixArray[0] = (rgba*) source->image;
			int i;
			for(i = 1; i < dest->height; i++) {
					dest->pixArray[i] = malloc(sizeof(dest->width) * sizeof(dest));
					dest->pixArray[i] = dest->pixArray[i - 1] + dest->width;
			}
		}
		memcpy(dest->image, source->image, sizeof(rgba)*source->height);
		dest->width = source->width;
		dest->height = source->width;
	}
	// printf("dest: %d\nsource: %d\n", dest->pixArray[0]->r, source->pixArray[0]->r);
}

static void pixMap_reset(pixMap *p) {
	//free all the memory in the fields and set everything to zero

	p->width = 0;
	p->height = 0;
	free(p->pixArray);
	free(p->image);
}


void pixMap_destroy(pixMap *p) {
	//reset pixMap and free the object p

	pixMap_reset(p);
	free(p);
}


void pixMap_rotate (pixMap *p, float theta) {
	pixMap *temp = pixMap_init();
	pixMap_copy(temp, p);

 //make a new temp blank pixMap structure
 //copy p to temp
 //set the values in the image to zero using memset - Note that the 3rd argument of memset is the size in BYTES

 //calculate the coordinates ox and oy of the middle of the png graphic
 //calculate the values of sine and cosine used by the rotation formula for -theta

	//for(int y=0;y<p->height;y++){   //two for loops to loop through each pixel in the original
	 //for(int x=0;x<p->width;x++){

	    //calculate the old coordinates rotx roty by rotating by -theta and using the formula described here
	    //http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	    //use the answer from stackoverflowery

	    //However this answer assumes that y is going from the bottom to the top (mathematical convention)
	    //but the pixmap starts at the upper left hand corner and height grows DOWN (scan order)
	    //so use this formula instead where c is cos(degreesToRadians(-theta)) and s is sin(degreeToRadians(-theta))
	    //    float rotx = c*(x-ox) - s * (oy-y) + ox;
      //    float roty = -(s*(x-ox) + c * (oy-y) - oy);

 	   //round the coordinates to the nearest integer in your calculations (add 0.5 and cast to integer)

	    //if old coordinates are within the height and width limits
	      //copy the pixel at the old coords to the pixel to the temporary copy using memcpy
				//memcpy(temp->pixArray[y]+x,p->pixArray[roty]+rotx,sizeof(rgba))
	    //

	  //
	//
	//copy the temp pixMap to the original
	//destroy the temp;
}

void pixMap_gray (pixMap *p){
	//for() loop through pixels using two for loops
	  //for()
	    //calculate average value of r and g and b values (assign to a float variable)
	    //for example the red value of for the pixel at height i, width j would be p->pixel[i][j].r
	    //round float variable to integer (add 0.5 before casting to integer)
	    //assign the rounded value to r,g and b values
	  //
	//
	int row;
	int col;
	for(row = 0; row < p->height; row++) {
		for(col = 0; col < p->width; col++) {
			// float avg = (p->pixArray[row][col]->r + p->pixArray[row][col]->g + p->pixArray[row][col]->b) / 3;
			// p->pixArray[row][col]->r = greyval;
			// p->pixArray[row][col]->g = greyval;
			// p->pixArray[row][col]->b = greyval;
		}
	}
			float avg = (1 + 2 + 3) / 3;
			avg += 0.5;
			int greyval = (int) avg;
			printf("%d", greyval);
}
int pixMap_write(pixMap *p,char *filename){
 	//write out to filename using lodepng_encode32_file
	//example is in lodepng/examples - one liner
	lodepng_encode32_file(filename, p->image, p->width, p->height);
	return 0;
}
