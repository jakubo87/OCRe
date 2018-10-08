#include <iostream>
#include "images.hh"
#include "gly_scan.hh"
#include "writetofile.hh"
//#include "recognition.hh"
#include <future>

int main(){

//prepare masks for recognition
  auto trans = std::async(std::launch::async, make_masks);
//  auto trans = make_masks();

//read the greyscale image
  //auto pixels=boost_gil_read_img("../Testimages/test_text.jpg");  //7007.jpg"); //
//do stuff to the image

//extract glyphs from the image
  //auto glyphs= gly_scan(pixels);
//write glyphs to vector
  writetofile(
    find_lines(
      read_img_to_matrix("../Testimages/test_text3.jpg"),
      trans.get()
//    trans
    )
  );
  return 0;
}

