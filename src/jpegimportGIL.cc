//source    https://www.quora.com/How-can-we-convert-a-png-jpeg-image-into-a-2D-array-in-C++-If-possible-please-attach-code-snippets

#include <iostream>
#include <boost/gil/extension/io/jpeg_io.hpp>
//  #include <boost/gil/gil_all.hpp>
#include "jpegimportGIL.hh"

namespace gil = boost::gil;

//shows height and width of a jpeg
int main(){

  auto result=boost_gil_read_img("Testimages/7007.jpg");

  return 0;
}







     /*old code

     //coloured
     gil::rgb8_image_t img;
     gil::jpeg_read_image("demo.jpg", img);
     std::cout << "Read complete, got an image " << img.width()
               << " by " << img.height() << " pixels\n";

     gil::rgb8_pixel_t px = *const_view(img).at(5, 10);
     std::cout << "The pixel at 5,10 is "
               << (int)px[0] << ','
               << (int)px[1] << ','
               << (int)px[2] << '\n';


    //greyscale

     gil::gray8_image_t img2;
     gil::jpeg_read_image("Testimages/7007.jpg", img2);
     std::cout << "Read complete, got an image " << img2.width()
               << " by " << img2.height() << " pixels\n";

     gil::gray8_pixel_t px2 = *const_view(img2).at(5, 10);
     std::cout << "The pixel at 5,10 is "
               << (int)px2[0] << '\n';

*/


    //fucking include -ljpeg at compiler bitch!
