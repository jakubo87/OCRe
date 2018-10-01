#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

  #include <iostream>
  #include <boost/gil/extension/io/jpeg/old.hpp> //<--- this???
  //probably missing:
  //#include <boost/gil/extension/io/jpeg/tags.hpp>
  //#include <boost/gil/gil_all.hpp>
  #include <array>
  #include <string>
  //#include "recognition.hh"
  #include "structures.hh"
  #include "gly_scan.hh"

//Globals


//using namespace boost::gil;

//read and scale image
//TODO improve contrast + other stuff

//decltype(auto)
matrix read_img_to_matrix(const std::string & fname){
  boost::gil::gray8_image_t img;
  //jpeg_read_image(fname, img); //jpeg_read_image() ?
  boost::gil::read_and_convert_image(fname, img, boost::gil::jpeg_tag());

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

  return pixels; //Move will probably not do, as the problem size is unknown at compile time, but who knows...?
}

int count=0;

template<class M>
void matrix_to_image(M && input){

  std::string filename( "../debug-images/image" );
  filename+=std::to_string(count++);
  filename+=".jpg";
  int H=input.size();
  int W=input[0].size();
  boost::gil::gray8_image_t img ( W, H );

// write data into image
  for (int row=0;row<H;++row){
    for (int col=0;col<W;++col){
      *boost::gil::view(img).at(col, row)=input[row][col];
    }
}

 // write_view( filename, view( img ), jpeg_tag());
  boost::gil::jpeg_write_view( filename, view( img ));
  //std::cout << "turned a matrix to an image.\n";

}
    //fucking include -ljpeg at compiler!



template<class M, class T>
std::string find_lines(M && m, T && t){
  std::string s;
  s+=recognise(point{0,0},point{static_cast<X>(m[0].size()-1),static_cast<Y>(m.size()-1)},std::forward<M>(m), t); //dummy

  std::cout << s << "\n";
  return s;

}



#endif