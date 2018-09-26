#include <iostream>
#include "jpegimportGIL.hh"
#include "gly_scan.hh"
#include "writetofile.hh"
#include <future>

int main(){

//prepare masks for recognition
  auto trans = std::async(std::launch::async, make_masks);

//read the greyscale image
  auto pixels=boost_gil_read_img("../Testimages/test_text.jpg");  //7007.jpg"); //
//do stuff to the image

//extract glyphs from the image
  auto glyphs= gly_scan(pixels);
//write glyphs to vector
  auto  text =recognise(glyphs, trans.get());
//write to file
  writetofile(text);
  return 0;
}

