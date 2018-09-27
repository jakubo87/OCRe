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

using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;

//to be in a separate task before rest begins -> thread parallel and joined before all the recognition stuff begins (after glyphing)
auto make_masks(){ //TODO
  // for both jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
  trans_tab trans;
  std::vector<char> chars;
  std::vector<matrix> masks;
  std::string path = "../Trainingimages";

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
    int len=path.length();
    chars.push_back(path[len-5]);
    //std::cout << "mask finished for" << path << " -> "<< path[len-5] << "\n";
  }
  trans.first=masks;
  trans.second=chars;
  return trans;
}


double T_a=0.05; //threshault for accuracy

auto similarity(matrix input,matrix comp){
  int H=input.size();
  int W=input[0].size();
  int result=0;
  for (int i=0;i<H;++i){
    for (int j=0;j<W;++j){
      //std::cout <<"input: "<< input[i][j] <<"\n";
      //std::cout <<"mask: " << comp[i][j] <<"\n";
      //distance
      result+=(input[i][j]-comp[i][j])*(input[i][j]-comp[i][j]);
	  }
	}
  //result*=100;
  result/=H*H*W*W;//norm
  return -(std::sqrt(result));
}

//using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;

std::string recognise(gly_string & gly_s, const trans_tab & tran){
  std::vector<xy_char> pos_c;
  for (auto g : gly_s){ //for each glyph in the sequence
    auto c= g.recognize(tran); //c is pair of char and confidence
    if (c.second>-100){
     pos_c.push_back(xy_char{g.left(),g.bottom(),c.first});
     std::cout << "added new char: " << c.first << "\n";
    }
  }
  //sort by x for each line - assuming there is only one line to translate
  std::sort(pos_c.begin(),pos_c.end(), [&](auto & m, auto & n){return m.x<n.x;});

  //assuming multiple lines are read at once ...
  //TODO eventually, possibly never

  std::string result;
  for (auto & e : pos_c)
    result+=e.c;
  return result;
  //TODO finding new lines, empty lines and empty spaces.
}



#endif