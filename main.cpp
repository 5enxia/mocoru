#include<unistd.h>

int main(int argc, char const **argv)
{
	const char *path = "/bin/sh";
	char *const *argv2;
	execv(path, argv2);
	return 0;
}
