#include <sys/utsname.h>
#include <stdio.h>

int main(void) {
	struct utsname results;
	int err = uname(&results);
	if (err) {
		printf("Failure!\n");
		return -1;
	}

	printf("System:  %s\n", results.sysname);
	printf("Node:    %s\n",  results.nodename);
	printf("Release: %s\n", results.release);
	printf("Version: %s\n", results.version);
	printf("Machine: %s\n", results.machine);
}
