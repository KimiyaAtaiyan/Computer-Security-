#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_lucet_sandbox.hpp"
#include "rlbox.hpp"



//Callback on completion of library function
/*void on_completion(char* result) {
   char result_str[100];
   strcpy(result_str, result);
   printf("Done: %s\n", result_str);
}*/


//cb_on_completion
void on_completion(rlbox::rlbox_sandbox<rlbox::rlbox_lucet_sandbox>& _, rlbox::tainted<char*, rlbox::rlbox_lucet_sandbox> tainted_str){

   auto checked_string = tainted_str.UNSAFE_unverified();
   char result_str[100];
   strcpy(result_str, checked_string);
   printf("Done: %s\n", result_str);

    /*auto checked_string = str.copy_and_verify_string([](std::unique_ptr<char[]> val){
		    return std::strlen(val.get()) < 1024 ? std::move(val) : nullptr;
		    });

    printf("cb_on_completion %s\n", checked_string.get());*/

}


int main(int argc, char const *argv[])
{
    //check for input from command line
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " COPY_STRING RESULT_STRING" << std::endl;
        return 1;
    }

    char* copy_str = (char*)argv[1];
    char* result_str = (char*)argv[2];

    //creaye a new sandbox 
    rlbox::rlbox_sandbox<rlbox::rlbox_lucet_sandbox> sandbox;

    //create sandbox
    sandbox.create_sandbox("./my_lib.so"); 
    //sandbox.invoke_sandbox_function(print_version);

    //strings
    size_t copySize = strlen(copy_str) +1;
    size_t resultSize = strlen(result_str)+1;
    auto taintedCopy = sandbox.malloc_in_sandbox<char>(copySize);
    auto taintedResult = sandbox.malloc_in_sandbox<char>(resultSize); 

    strncpy(taintedCopy.unverified_safe_pointer_because(copySize, "writing to region"), copy_str, copySize);
    strncpy(taintedResult.unverified_safe_pointer_because(resultSize, "writing to region"), result_str, resultSize);

   
    //callback -> on_completion
    auto cb = sandbox.register_callback(on_completion);



    sandbox.invoke_sandbox_function(print_version);    
    auto hash = sandbox.invoke_sandbox_function(get_hash, taintedCopy , cb , taintedResult);
    
    printf("Hash = %llx\n", hash);

    //destroy sandbox
    sandbox.destroy_sandbox();
    sandbox.free_in_sandbox(taintedCopy);
    sandbox.free_in_sandbox(taintedResult);
    return 0;
}
