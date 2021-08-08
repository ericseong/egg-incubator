#include <stdio.h>
#include <signal.h>
#include <unistd.h>
 
void signalHandler(int signalNum) {
  if (signalNum == SIGUSR1) {
    printf("Caught SIGUSR1 signal.\n");
  }
}
 
int main(void) {
  signal(SIGUSR1, signalHandler);
  raise(SIGUSR1);

  for( int i = 0 ; i < 10 ; i++ ) { 
    printf( "iteration: %d\n", i );
    sleep(1);
  }
  return 0;
}
