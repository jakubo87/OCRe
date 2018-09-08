#ifndef __RECOGNITION_H_INCLUDED__
#define __RECOGNITION_H_INCLUDED__


#include "structures.hh"
#include <vector>
#include <string>
#include "gly_scan.hh"


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

std::string recognise(gly_string g){
  std::string result="test";




  return result;
}



#endif