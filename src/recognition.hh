#ifndef __RECOGNITION_H_INCLUDED__
#define __RECOGNITION_H_INCLUDED__


#include "structures.hh"
#include <vector>
#include <string>
#include "gly_scan.hh"
#include <algorithm>
#include "jpegimportGIL.hh"
#include <filesystem>
#include <boost/filesystem.hpp>


//to be in a separate task before rest begins -> thread parallel and joined before all the recognition stuff begins (after glyphing)
auto make_masks(){ //TODO
  // for both jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
  std::vector<matrix> masks;
  std::string path = "../Testimages";
  //for all images in ../Testimages
  for (auto & p : boost::filesystem::directory_iterator(path)){ //C++17 & -lstc++fs for linking
    masks.push_back(
      resize_matrix(
        boost_gil_read_img(p.path().string()),
        MaskW,
        MaskH
      )
    );
    std::cout << "mask finished!\n";
  }
  return masks;
}




double similarity(matrix input,matrix comp){
  int H=input.size();
  int W=input[0].size();
  int result=0;
  for (int i=0;i<H;++i)
    for (int j=0;j<W;++j)
      if (input[i][j]==comp[i][j])
        ++result;
  result=static_cast<double>(result)/(H*W);
  return result;
}

std::string recognise(gly_string gly_s, std::vector<matrix> masks){
  std::string result;
  for (auto g : gly_s){ //for each glyph in the sequence
    char best;
    double score=0;
    for (auto e: masks){
      int curr= similarity(
        resize_matrix(
          g.to_matrix(),MaskW,MaskH),
          e);
      std::cout << "char scored " << curr << "\n";
      if (curr>score) score =curr; //max
      best='7'; //TODO identify the jpeg/mask as a char
    }
    if (score>0.8){
     result.push_back(best);
     std::cout << "added new char: " << best << "\n";
    }
  }
  return result;
  //TODO finding new lines, empty lines and empty spaces.
}



#endif