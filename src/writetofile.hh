#ifndef __WRITETOFILE_H_INCLUDED__
#define __WRITETOFILE_H_INCLUDED__



#include <vector>
#include "recognition_test.hh"
#include <iostream>
#include <fstream>


void writetofile(const std::vector<xy_char> & vec){
  std::ofstream foutput;
  foutput.open ("output.txt");
  int x,y=0;
  for (xy_char e : vec){
    foutput << e.value;
  }
  foutput.close();
  std::cout << "File written successfully.\n";

}

#endif 