#include "pixutils.h"
	//check for flags -i -o -r -g - can be in any order
  //-i is followed by the input png
  //-o is followed the output png
  //-r is followd by the rotation angle in degrees (float) <optional for user>
  //-g is whether th png should be grayed <optional for user>

int main(int argc, char *argv[]){
  char *inputfile=0,*outputfile=0;
  float degrees=0, grayFlag=0, bitFlag=0, sortFlag=0;

  int count = 1;
  while(argv[count]) {
    if(strcmp(argv[count],"-i") == 0) {
      if(argv[++count]) inputfile = argv[count];
    }
    if(strcmp(argv[count], "-o") == 0) {
      if(argv[++count]) outputfile = argv[count];
    }
    if(argc > 5) {
      if(strcmp(argv[count], "-r") == 0) {
        sscanf(argv[++count],"%f",&degrees);
      }
      if(strcmp(argv[count], "-g") == 0) {
        grayFlag = 1;
      }
      if(strcmp(argv[count], "-b") == 0) {
        bitFlag = 1;
      }
      if(strcmp(argv[count], "-s") == 0) {
        sortFlag = 1;
      }
    }
    count++;
  }

  pixMap *p=pixMap_init_filename(inputfile);
  if(degrees) pixMap_rotate(p,degrees);
  if(grayFlag) pixMap_gray(p);
  if(sortFlag) pixMap_sort(p);
  if(bitFlag) pixMap_write_bmp16(p, outputfile);
  if(!bitFlag) pixMap_write(p,outputfile);
  pixMap_destroy(p);

  return 0;
}
