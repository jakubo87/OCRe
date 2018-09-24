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
#include <utility>


using trans_tab=std::pair<std::vector<matrix>,std::vector<char>>;


//to be in a separate task before rest begins -> thread parallel and joined before all the recognition stuff begins (after glyphing)
auto make_masks(){ //TODO
  // for both jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
  trans_tab trans;
  std::vector<char> chars;
  std::vector<matrix> masks;
  std::string path = "../Trainingimages";
  //for all images in ../Testimages
  for (auto & p : boost::filesystem::directory_iterator(path)){ //C++17 & -lstc++fs for linking
    std::string path=p.path().string();
    masks.push_back(
      resize_matrix(
        gly_scan(
          boost_gil_read_img(path)).back().to_matrix(),
        MaskW,
        MaskH
      )
    );
    std::cout << "mask finished for" << path << "!\n";
    int len=path.length();
    chars.push_back(path[len-5]);
  }
  trans.first=masks;
  trans.second=chars;
  return trans;
}




auto similarity(matrix input,matrix comp){
  int H=input.size();
  int W=input[0].size();
  int result=0;
  for (int i=0;i<H;++i){
    for (int j=0;j<W;++j){
      //std::cout <<"input: "<< input[i][j] <<"\n";
      //std::cout <<"mask: " << comp[i][j] <<"\n";
      //epsilon
      if (std::abs(input[i][j]-comp[i][j])<T*255){
        //std::cout<< "scored a point!\n";
        ++result;
	    }
	  }
	}
  result*=100;
  result/=H*W;//norm
  return result;
}

std::string recognise(gly_string gly_s, trans_tab trans){
  std::string result;
  for (auto g : gly_s){ //for each glyph in the sequence
    char best;
    double score=0;
    for (int i=0;i<trans.first.size();++i){
      int curr= similarity(
        resize_matrix(
          g.to_matrix(),MaskW,MaskH),
          trans.first[i]);
      std::cout << "char scored " << curr << "\n";
      if (curr>score){
        score =curr; //max
        best=trans.second[i]; //TODO identify the jpeg/mask as a char
      }
    }
    if (score>70){
     result.push_back(best);
     std::cout << "added new char: " << best << "\n";
    }
  }
  return result;
  //TODO finding new lines, empty lines and empty spaces.
}



#endif