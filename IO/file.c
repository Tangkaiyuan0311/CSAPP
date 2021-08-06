#include "wrappers.h"

int main(int argc, char** argv) {
	struct stat stat;
	char* type;
	char* read;
	off_t size;

	Stat(argv[1], &stat);

	// determine the file type
	if (S_ISREG(stat.st_mode))
		type = "regular";
	else if (S_ISDIR(stat.st_mode))
		type = "directory";
	else
		type = "other";

	// check read access
	if ((stat.st_mode & S_IRUSR)) // check for individual bit
		read = "yes";
	else
		read = "no";

	// check file size
	size = stat.st_size;
	printf("type: %s, read: %s, file size: %ld bytes\n", type, read, size);
	exit(0);
}
