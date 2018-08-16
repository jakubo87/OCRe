#ifndef __NEURONET_H_INCLUDED__
#define __NEURONET_H_INCLUDED__

#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <cmath>

//here is where i will make tests of implementing a deep neural network
//
//1st approach is of course the naivest possible

const int M=3; //feature dimensions with offset/bias


using weights = std::vector<double>;   //length M+1
using input = std::vector<int>;     //length M+1
using true_res = std::vector<int>;  //length N (datapoints)
using train_data = std::vector<input>; //length N

//dot- or inner-product with input_0=1
double dot(const input in, const weights & w){
  auto res=std::inner_product(in.begin(),in.end(), w.begin(),0);
  std::cout << "dot prod " << res<<"\n";
  return res;
}

// N-D-sigmoidal activation function
double sig(double x){
  return 1/(std::exp(x));
}

bool h(double x){ //heaviside activation function
  if (x<0) return 0;
  return 1;
}

//training a perceptron over data
void train(const train_data& data, weights & w,const true_res & y){

  for (int n=0;n<100;++n){

    for (int i=0;i< data.size();++i) //for each data point
    {
      if (h(dot(data[i],w))!=y[i]){
        for (int j=0;j<M;++j)
          w[j]= 0.01*(h(data[i][j]*w[j])-y[i])*data[i][j];

        std::cout << "calculated weights after data point " << i << "\n";
        for (auto _w : w)
          std::cout << _w << " ";
        std::cout << "\n";
      }
    }
    std::cout << "calculated weights after epoch "<< n+1<< "\n";
      for (auto _w : w)
        std::cout << _w << " ";
      std::cout << "\n";
  }
}

#endif


/*
int main(int argc, const char * argv[]) {
    for (unsigned i = 0; i != 50; ++i) {
        vector<float> pred = sigmoid(dot(X, W, 4, 4, 1 ) );
        vector<float> pred_error = y - pred;
        vector<float> pred_delta = pred_error * sigmoid_d(pred);
        vector<float> W_delta = dot(transpose( &X[0], 4, 4 ), pred_delta, 4, 4, 1);
        W = W + W_delta;
    };
    return 0;
}








*/