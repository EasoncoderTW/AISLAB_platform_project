#include "systemc.h"

SC_MODULE(adder) { // Declare Adder Module
  sc_in<int> a,b;  // Declare in port
  sc_out<int> sum; // Declare out port

  void doAdd();

  SC_CTOR(adder) {
      SC_METHOD(doAdd);    // Register doAdd process
      sensitive << a << b; // specify sensitivity
  }
};