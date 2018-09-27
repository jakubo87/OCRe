#ifndef __WRITETOFILE_H_INCLUDED__
#define __WRITETOFILE_H_INCLUDED__



#include <vector>
//#include "recognition.hh"
#include <iostream>
#include <fstream>
#include <string>

template<class S>
void writetofile(S && text){
  std::ofstream foutput;
  foutput.open ("output.txt");
  foutput << text;
  foutput.close();
  std::cout << "File written successfully.\n";
}

#endif 