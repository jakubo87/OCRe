#include <iostream>
#include "jpegimportGIL.hh"
#include "gly_scan.hh"
#include "writetofile.hh"
#include <future>

int main(){

//prepare masks for recognition

//read the gre  yscale image
  auto pixels=boost_gil_read_img("../Testimages/7007.jpg");
//do stuff to the image

//extract glyphs from the image
  auto glyphs= gly_scan(pixels);
//write glyphs to ve  ctor
  auto  text =recognise(glyphs);
//write to file
  writetofile(text);
  return 0;
}

