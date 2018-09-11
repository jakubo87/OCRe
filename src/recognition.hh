#ifndef __RECOGNITION_H_INCLUDED__
#define __RECOGNITION_H_INCLUDED__


#include "structures.hh"
#include <vector>
#include <string>
#include "gly_scan.hh"


//to be in a separate task before rest begins -> thread parallel and joined before all the recognition stuff begins (after glyphing)
void make_mask(){
  // for all jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
}


double similarity(matrix input,matrix comp){
  int H=input.size();
  int W=input[0].size();
  int result=0;
  for (int i=0;i<H;++i)
    for (int j=0;j<W;++j)
      if (input[i][j]==comp[i][j])
        ++result;
  result=result/(H*W);
  return result;
}

std::string recognise(gly_string gly_s){
  std::string result;
  for (auto g : gly_s){ //for each glyph in the sequence
    char best;
    double score=0;
    for (auto e: masks){
      int curr= similarity(g, e);
      if (curr>score) score =curr; //max
      best='7';
    }
    if (score>0.8) result.push_back(best);
  }
  return result;
}



#endif