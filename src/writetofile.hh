#ifndef __WRITETOFILE_H_INCLUDED__
#define __WRITETOFILE_H_INCLUDED__



#include <vector>
#include "recognition.hh"
#include <iostream>
#include <fstream>
#include <string>

void writetofile(const std::string text){
  std::ofstream foutput;
  foutput.open ("output.txt");

  /*
  for (auto e : vec){
    foutput << e.value;
  }
  */
  foutput << text;


  foutput.close();
  std::cout << "File written successfully.\n";

}

#endif 