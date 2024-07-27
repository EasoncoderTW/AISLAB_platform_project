#include <iostream>
#include "systemc.h"
#include "adder.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <csignal>
#include <sys/types.h>
#include <unistd.h>

#define SHMSZ     40
#define REG_A     0
#define REG_B     1
#define REG_O     2
#define IPC_PID_CPU  3
#define IPC_PID_DEV  4

using namespace std;

int *mmio;
sc_signal<int> a,b,sum;

void signal_handler(int signum){
  cout << "[System C] a = " << a << ", b = " << b << ", sum = " << sum << endl;
  kill(SIGUSR1,mmio[IPC_PID_CPU]);
}

int sc_main(int argc, char* argv[]) {
  /* shared memory IPC */
  int shmid;
  key_t key;
  void *shm;
  
  key = 5678; // Can be modify
  // Create the segment.
  if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
      perror("shmget");
      exit(1); 
  }
  // attach the segment to our data space.
  if ((shm = shmat(shmid, NULL, 0)) == (char*) - 1) {
      perror("shmat");
      exit(1);
  }
  mmio = (int*)shm;
  mmio[REG_A] = 0;
  mmio[REG_B] = 0;
  mmio[REG_O] = 0;
  mmio[IPC_PID_DEV] = getpid();
  cout << "[SystemC] pid = " << mmio[IPC_PID_DEV] << endl;
  cout << "[SystemC] pid = " << mmio[IPC_PID_CPU] << endl;
  signal(SIGUSR1,signal_handler);

  /* systenC module */
  // Create instance of Adder
  adder adder_0("adder");  
  // Declare singal in testbench
 
  // Initialize adder port by constructor
  adder_0.a(a); 
  adder_0.b(b);
  adder_0.sum(sum);

  while(1)
  {
    a = mmio[REG_A];
    b = mmio[REG_B];
    sc_start(1,SC_NS); // Start simulation
    mmio[REG_O] = sum;
    
    /*cout << "[System C] waiting \r";
    if(a!=mmio[REG_A] || b!=mmio[REG_B])
    {
      a = mmio[REG_A];
      b = mmio[REG_B];
      sc_start(1,SC_NS); // Start simulation
      mmio[REG_O] = sum;
      cout << "[System C] a = " << a << ", b = " << b << ", sum = " << sum << endl;
      kill(128,);
    }*/
  }
  
  shmctl(shmid, IPC_RMID, 0);
  return 0;
}