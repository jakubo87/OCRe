#ifndef __RECOGNITION_H_INCLUDED__
#define __RECOGNITION_H_INCLUDED__


#include "structures.hh"
#include <vector>
#include <string>
#include "gly_scan.hh"
#include <algorithm>


//to be in a separate task before rest begins -> thread parallel and joined before all the recognition stuff begins (after glyphing)
void make_masks(){
  // for both jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
}

matrix glyph_to_matrix(glyph g){
    //offsets
    X left=g.left();
    Y top=g.top();
    Y bottom=g.bottom();
    X right=g.right();

  //initialize the matrix containing only the glyph
  matrix m={std::vector<int> (right-left)};
  for (int i=1;i<20;++i) //add 19 more lines
    m.push_back(std::vector<int> (right-left));

  std::for_each(g.data().begin(),g.data().end(),[&](auto i){
    m[i.y-top][i.x-left]=1;
  });
  return m;
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