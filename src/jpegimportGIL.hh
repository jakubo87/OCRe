#ifndef __JPEFIMPORTGIL_H_INCLUDED__
#define __JPEFIMPORTGIL_H_INCLUDED__

  #include <iostream>
  #include <boost/gil/extension/io/jpeg/old.hpp> //<--- this???
  //probably missing:
  //#include <boost/gil/extension/io/jpeg/tags.hpp>
  //#include <boost/gil/gil_all.hpp>
  #include <array>
  #include <string>
  //#include "recognition.hh"
  #include "structures.hh"

//Globals


using namespace boost::gil;

//read and scale image
//TODO improve contrast + other stuff
//TODO generalize to rgb pictures! atm only grayscale 8bit

decltype(auto) boost_gil_read_img(const std::string & fname)
{
  gray8_image_t img;
  //jpeg_read_image(fname, img); //jpeg_read_image() ?
  read_and_convert_image(fname, img, jpeg_tag());

//  std::cout << "Read complete, got an image " << img.width()
//            << " by " << img.height() << " pixels\n";
//
//  gil::gray8_pixel_t px = *const_view(img).at(33, 33);
//  std::cout << "The pixel at 33,33 is "
//            << (int)px[0] << '\n';

  int const height=img.height();
  int const width=img.width();

  //instantiating the matrix

  matrix pixels;//={ {(width)} };
  //copying the pixeldata (in case of there being some sort of proxy - thx c++)
  for (int i=0; i<height; ++i){
    std::vector<int> line (width);
    for (int j=0; j<width; ++j){
      line[j]=*const_view(img).at(j,i);
    }
    pixels.push_back(line);
  }

  return pixels;//std::move(pixels); Move will probably not do, as the problem size is unknown at compile time, but who knows...?
}

int count=0;

void matrix_to_image(matrix input){

  std::string filename( "../debug-images/image" );
  filename+=std::to_string(count++);
  filename+=".jpg";
  int H=input.size();
  int W=input[0].size();
  gray8_image_t img ( W, H );

// write data into image
  for (int row=0;row<H;++row){
    for (int col=0;col<W;++col){
      *view(img).at(col, row)=input[row][col];
    }
}

 // write_view( filename, view( img ), jpeg_tag());
  jpeg_write_view( filename, view( img ));
  //std::cout << "turned a matrix to an image.\n";

}
    //fucking include -ljpeg at compiler!



#endif