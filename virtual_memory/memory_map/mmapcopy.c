#include "wrappers.h"





int main(int argc, const char** argv){

	FILE* fp;
	int inputFd, outputFd;
	size_t sz; //text file size
	const char* filename = argv[1];
       	fp = fopen(filename, "r");
	inputFd = fileno(fp);
	
	// get file size in bytes
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET); //seek back

	// memory mapping
	void* areap = Mmap(NULL, sz, PROT_READ, MAP_PRIVATE, inputFd, 0);
		
	// write to output
	outputFd = 1; //standard output
	ssize_t bytescnt = Write(outputFd, areap, sz);
	if (bytescnt != sz)
		printf("write error\n");

	return 0;
}

