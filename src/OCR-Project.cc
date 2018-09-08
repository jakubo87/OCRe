#include <iostream>
#include "jpegimportGIL.hh"
#include "gly_scan.hh"

int main(){

//soon

//read the greyscale image
  auto pixels=boost_gil_read_img("Testimages/7007.jpg");
//do stuff to the image

//extract glyphs from the image
  auto glyphs= gly_scan(pixels);
//write glyphs to vector
  auto  text =recognise(glyphs);
//write to file
  writetofile(text);


  return 0;
}

