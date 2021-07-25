#include "memlib.h"
#include "wrappers.h"
#include "mm.h"


//not a thorough test
int main(){
	mem_init(); //initialize the memory system
	if (mm_init() == -1){
		printf("heap initialization fails...\n");
		exit(0);
	}
	printf("heap initialization succeed...\n");
	
	char* test = "hello world!";

	char* array1 = (char*)mm_malloc(14);
	char* array2 = (char*)mm_malloc(14);
	char* array3 = (char*)mm_malloc(14);
	char* array4 = (char*)mm_malloc(14);
	int i;
	for (i = 0; i < 14; i++){
		array1[i] = test[i];
		array2[i] = test[i];
		array3[i] = test[i];
		array4[i] = test[i];
	}
	printf("test = %s\n", array1);
	printf("test = %s\n", array2);
	printf("test = %s\n", array3);
	printf("test = %s\n", array4);

	mm_free((void*)array1);
	mm_free((void*)array2);
	mm_free((void*)array3);
	mm_free((void*)array4);
	
	mem_close(); //close the memory system

	return 0;
}


