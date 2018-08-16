#ifndef __JPEFIMPORTGIL_H_INCLUDED__
#define __JPEFIMPORTGIL_H_INCLUDED__

  #include <iostream>
  #include <boost/gil/extension/io/jpeg_io.hpp>
  //  #include <boost/gil/gil_all.hpp>
  #include <array>
  #include <string>
  #include "recognition_test.hh"

//Globals


namespace gil = boost::gil;

//read and scale image
//TODO improve contrast + other stuff
//TODO generalize to rgb pictures! atm only grayscale 8bit

decltype(auto) boost_gil_read_img(std::string & fname)
{
  gil::gray8_image_t img;
  gil::jpeg_read_image(fname, img);
  std::cout << "Read complete, got an image " << img.width()
            << " by " << img.height() << " pixels\n";

  gil::gray8_pixel_t px = *const_view(img).at(5, 10);
  std::cout << "The pixel at 5,10 is "
            << (int)px[0] << '\n';

  int const height=img.height();
  int const width=img.width();

  //generating an array

  std::array<int,height*width> pixels;
  //copying the pixeldata (in case of there being some sort of proxy - thx c++)
  for (int i=0; i<height; ++i){
    for (int j=0; j<width; ++j){
      pixels[i*width+j]=*const_view(img).at(j,i);
    }
  }

  //generating the matrix
  matrix m{std::move(pixels),height,width}
  //return result
  return std::move(m);

}

    //fucking include -ljpeg at compiler bitch!






#endif