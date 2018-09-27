#ifndef __RECOGNITION_H_INCLUDED__
#define __RECOGNITION_H_INCLUDED__


#include "structures.hh"
#include <vector>
#include <string>
#include "gly_scan.hh"
#include <algorithm>
#include <numeric>
#include "jpegimportGIL.hh"
#include <filesystem>
#include <boost/filesystem.hpp>
#include <utility>

using gly_string = std::vector<glyph>;

using matrix = std::vector<std::vector<int>>;
using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;

//to be in a separate task before rest begins -> thread parallel and joined before all the recognition stuff begins (after glyphing)
trans_tab make_masks(){
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

template<class M, class C>
auto similarity(M && input,C && comp){
  int H=input.size();
  int W=input[0].size();
  int result=0;

  /* the easy way
  for (int i=0;i<H;++i){
    for (int j=0;j<W;++j){
      //std::cout <<"input: "<< input[i][j] <<"\n";
      //std::cout <<"mask: " << comp[i][j] <<"\n";
      //distance
      result+=(input[i][j]-comp[i][j])*(input[i][j]-comp[i][j]);
	  }
	}
	*/
  //the hard way
	//used 2 accumulates here to not get race conditions from simultaneously writing to accum value
	//hopefully to be used in a parallel way-> execution policy parallel: std::execution::par_unseq
	//in more civilised days...
  std::vector<int> rows(input.size());
  std::transform(input.begin(), input.end(), comp.begin(), rows.begin() ,
    //lambda to fill row sum vector
    [&](const auto & /*vector*/ v1, const auto & v2){
      std::vector<int> cols(v1.size()); //temporary vector
      std::transform(v1.begin(),v1.end(),v2.begin(), cols.begin(),
        //lambda to fill col sum vector
        [&](const auto & i1, const auto & i2){
          return (i1-i2)*(i1-i2);
        }
      );
      return std::accumulate(cols.begin(), cols.end(),0);
      }
    );
  result= std::accumulate(rows.begin(), rows.end(), 0);




  //result*=100;
  result/=H*H*W*W;//norm
  return -(std::sqrt(result));
}

//using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;
template<class G, class T>
decltype(auto) recognise(G && gly_s, T && tran){

//first find composite glyphs to be fused and resize the container

// secondly sort the string. It's unsorted due to scanning order (linewise top->bottom), so the taller letters always go first
  //sort glyphs by x
  std::sort(gly_s.begin(),gly_s.end(), [&](auto & m, auto & n){return m.left()<n.left();});
  std::string res;
  res.reserve(gly_s.size());

//lastly translate the glyphs to chars
  //std::transform(std::execution::par_unseq,gly_s.begin(), gly_s.end(), s.begin() //since c++17
  std::transform(gly_s.begin(), gly_s.end(), res.begin(),                           //before c++17
    [&](auto & g){
      const auto & c= g.recognize(tran); //c is pair of char and confidence
      if (c.second>-100){
        return c.first;
      }
      return '_';
    }
  );
  return std::move(res);
  //TODO finding new lines, empty lines and empty spaces.
}



#endif