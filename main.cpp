#include<unistd.h>
#include<sched.h>

int main(int argc, char const **argv)
{
	// exec
	const char *path = "/bin/sh";
	char *const *argv2;
	execv(path, argv2);

	// unshare
	int flags = CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWUSER;
	unshare(flags);
	return 0;
}
