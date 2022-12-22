#include "CImg.h" // Path to the previous download directory
using namespace cimg_library;


__kernel void flip_image(__global CImg<unsigned char> img){
	int W = img.width();
	int H = img.height();
	int j = get_global_id(0);
	
	for (int i = 0; i < W/2;i++){
			int aux_red = (int) img(i, j, 0, 0) ;
			int aux_green = (int) img(i, j, 0, 1) ;
			int aux_blue = (int) img(i, j, 0, 2) ;	
			img(i,j,0,0) = img(W-i, j,0,0);
			img(i,j,0,1) = img(W-i, j,0,1);
			img(i,j,0,2) = img(W-i, j,0,2);
			img(W-i, j,0,0)  = aux_red;
			img(W-i, j,0,1) = aux_green;
			img(W-i, j,0,2) = aux_blue;		
	}  

  return 0;

}
