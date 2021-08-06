#include "wrappers.h"

int main(int argc, char** argv) {
	DIR* streamp;
	struct dirent* dep;

	streamp = Opendir(argv[1]);

	errno = 0; // reset errno
	while ((dep = readdir(streamp)) != NULL) {
		printf("Found file: %s\n", dep->d_name);
	}
	if (errno != 0) // errno has been set by readdir due to some error
		unix_error("readdir error");
	
	Closedir(streamp);
	exit(0);
}
