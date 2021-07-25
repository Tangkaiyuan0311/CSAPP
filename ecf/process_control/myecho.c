#include <stdio.h>

int main(int argc, char** argv, char** envp){

	int i;

	//print command-line arguments
	printf("Command line arguments:\n");
	for (i = 0; i < argc; i++){
		printf("\targv[%2d]: %s\n", i, argv[i]);
	}

	//print environment variables
	printf("Environment variables\n");
	i = 0;
	while (envp[i] != NULL){
		printf("\tenvp[%2d]: %s\n", i, envp[i++]);
	}
	return 0;
}


