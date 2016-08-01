#include "pixutils.h"
	//check for flags -i -o -r -g - can be in any order
  //-i is followed by the input png
  //-o is followed the output png
  //-r is followd by the rotation angle in degrees (float) <optional for user>
  //-g is whether th png should be grayed <optional for user>

int main(int argc, char *argv[]){
  char *inputfile=0,*outputfile=0;
  float degrees=0, grayFlag=0;

  int count = 1;
  while(argv[count]) {
    if(strcmp(argv[count],"-i") == 0) {
      if(argv[++count]) inputfile = argv[count];
      // printf("inputfile: %s\n", inputfile);
    }
    if(strcmp(argv[count], "-o") == 0) {
      if(argv[++count]) outputfile = argv[count];
      // printf("outputfile: %s\n", outputfile);
    }
    if(argc > 5) {
      if(strcmp(argv[count], "-r") == 0) {
        sscanf(argv[++count],"%f",&degrees);
        // printf("degrees: %f\n", degrees);
      }
      if(strcmp(argv[count], "-g") == 0) {
        grayFlag = 1;
        // printf("gray: %f\n", grayFlag);
      }
    }
    count++;
  }

  pixMap *p=pixMap_init_filename(inputfile);
  if(degrees)pixMap_rotate(p,degrees);
  if(grayFlag)pixMap_gray(p);
  pixMap_write(p,outputfile);
  pixMap_destroy(p);

  return 0;
}
