#include "NeuroNet.hh"
#include <iostream>
#include <vector>


int main(){

  //training with expected hyperplane with weigths (2,1,0)
  input v1 {1,3,1}; //c1 or 1
  input v2 {1,1,-3};//c2 or 0
  input v3 {1,4,7};      //1
  input v4 {1,4,-7};      //1
  input v5 {1,-5,3};     //0
  input v6 {1,0,0};       //0
  input v7 {1,-4,10};     //0
  input v8 {1,-2,0};       //0
  input v9 {1,1,-7};     //0
  input v10 {1,-4,4};    //0
  input v11 {1,3,0};      //1
  input v12 {1,6,-4};    //1
  input v13 {1,3,-1};    //1

  true_res y {1,0,1,1,0,0,0,0,0,0,1,1,1}; //length 12

  train_data data;
  data.push_back(v1);
  data.push_back(v2);
  data.push_back(v3);
  data.push_back(v4);
  data.push_back(v5);
  data.push_back(v6);
  data.push_back(v7);
  data.push_back(v8);
  data.push_back(v9);
  data.push_back(v10);
  data.push_back(v11);
  data.push_back(v12);
  data.push_back(v13);


  //weights w {-2,1,0}; //shouldnt be even touched..
  weights w {0,0,0}; //random..

//NN suck, when data varies strongly in one component which shouldnt be even touched -> use finer grained learning factor


  train(data,w,y);



  return 0;
}
