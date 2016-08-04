#include "pixutils.h"
#include "lodepng.h"
#include "bmp.h"

//private methods
static pixMap *pixMap_init(); //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
static void pixMap_reset();  //free the allocated memoray and set to zero but do not free memory for pixMap
static void pixMap_copy(pixMap *dest,pixMap *source); //copy one pixmap to another
static int pixMap_read(pixMap *p,char *filename); //read in a pixmap from file

static pixMap* pixMap_init() {
	pixMap *ptr;
	ptr = malloc(sizeof(pixMap));
	ptr->image = 0;
	ptr->pixArray = 0;
	ptr->width = 0;
	ptr->height = 0;
	return ptr;
}
pixMap* pixMap_init_filename(char *filename) {
	pixMap *ptr;
	ptr = pixMap_init();
	pixMap_read(ptr, filename);
	return ptr;

}
static int pixMap_read(pixMap *p,char *filename) {
	unsigned error;
	unsigned char* image;
	unsigned width, height;
	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
	p->image = image;
	p->width = width;
	p->height = height;
	p->pixArray = malloc(p->height * sizeof(p->pixArray));
	p->pixArray[0] = (rgba*) p->image;
	int i;
  for(i = 1; i < p->height; i++) {
      p->pixArray[i] = p->pixArray[i - 1] + p->width;
  }
	return 0;
}
static void pixMap_copy(pixMap *dest,pixMap *source) {
	if(source->image == 0) {
		pixMap_reset(dest);
	} else {
		if(dest->width != source ->width || dest->height != source->height) {
			if(dest->width * dest->height != source->width * source->height) {
				if (dest -> image == 0) {
					dest->image = malloc(sizeof(rgba) * source->height * source->width);
				} else {
					dest -> image = realloc(dest -> image, sizeof(rgba) * source->height * source->width);
				}
		}
		if(dest->height != source->height) {
			if (dest->pixArray == 0) {
				dest -> pixArray = malloc(sizeof(rgba)*source->height*source->width);
			} else {
				dest -> pixArray = realloc(dest -> pixArray, sizeof(rgba) * source->height * source -> width);
			}
		}
		dest->pixArray[0] = (rgba*) dest->image;
		for(int i = 1; i < source->height; i++) {
			dest -> pixArray[i] = dest->pixArray[i-1] + source -> width;
		}
	}
	memcpy(dest->image, source->image, sizeof(rgba) * source->height * source -> width);
	dest->width = source->width;
	dest->height = source->height;
	}
}

static void pixMap_reset(pixMap *p) {
	p->width = 0;
	p->height = 0;
	free(p->pixArray);
	free(p->image);
}

void pixMap_destroy(pixMap *p) {
	pixMap_reset(p);
	free(p);
}

void pixMap_rotate (pixMap *p, float theta) {
	pixMap *temp = pixMap_init();
	pixMap_copy(temp, p);
  memset(temp->image, 0, temp->height*temp->width*sizeof(rgba));

  float ox = p->width / 2;
  float oy = p->height / 2;
  float s = sin(degreesToRadians(theta));
  float c = cos(degreesToRadians(theta));

  for (int y = 0; y < p->height; y++) {
  	for (int x = 0; x < p->width; x++) {
    	int rotx = (int) (.5) + c*(x-ox) - s * (oy-y) + ox;
      int roty = (int) (.5) + -(s*(x-ox) + c * (oy-y) - oy);
    	if (rotx < p->width && roty < p->height) {
				memcpy(temp->pixArray[y]+x, p->pixArray[roty]+rotx, sizeof(rgba));
       }
     }
  }

  pixMap_copy(p, temp);
  pixMap_destroy(temp);
}

void pixMap_write_bmp16(pixMap *p,char *filename) {
	BMP16_map *b = BMP16_map_init(p->height, p->width, 0, 5, 6, 5);
	int row;
	int col;
	for(row = 0; row < p->height; row++) {
		for(col = 0; col < p->width; col++) {
			unsigned short colorVal = 0;
			int red5 = (p->pixArray[row][col].r & 0xF8) >> 3;
			int green6 = (p->pixArray[row][col].g & 0xFC) >> 2;
			int blue5 = (p->pixArray[row][col].b & 0xF8) >> 3;
			colorVal |= (red5 << 11);
			colorVal |= (green6 << 5);
			colorVal |= (blue5);
			b->pixArray[(p->height - row) - 1][col] = colorVal;
		}
	}
	BMP16_write(b, filename);
	BMP16_map_destroy(b);
}

static int sum(rgba *p) {
	return p->r + p->b + p->g;
}

static int compare(const void *a, const void *b) {
	rgba *arr1 = (rgba *)a;
  rgba *arr2 = (rgba *)b;
	//ret is a negative value, then first is smaller than second
	//ret is a positive value, then second is smaller than first
	//ret is zero, then values are equal
	int sum1 = sum(arr1);
	int sum2 = sum(arr2);
 	return sum1-sum2;
}

// void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));
// qsort(values, 5, sizeof(int), cmpfunc);
void pixMap_sort(pixMap *p) {
	compare(&(p->pixArray[0][0]), &(p->pixArray[1][0]) );
		// qsort(&p->pixArray, 10, sizeof(rgba), compare);

}

void pixMap_gray (pixMap *p){
	int row;
	int col;
	for(row = 0; row < p->height; row++) {
		for(col = 0; col < p->width; col++) {
			float avg = (p->pixArray[row][col].r + p->pixArray[row][col].g + p->pixArray[row][col].b) / 3;
			avg += 0.5;
			int greyval = (int) avg;
			p->pixArray[row][col].r = greyval;
			p->pixArray[row][col].g = greyval;
			p->pixArray[row][col].b = greyval;
		}
	}
}
int pixMap_write(pixMap *p,char *filename){
	lodepng_encode32_file(filename, p->image, p->width, p->height);
	return 0;
}
