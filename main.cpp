#include<unistd.h>
#include<sched.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>

int err;

void readUidMap() {
	std::string uid_map_fn= "/proc/self/uid_map";
	std::ifstream uid_map_file;
	uid_map_file.open(uid_map_fn, std::ios::in);
	int nBufferSize = 1024;
	char* pBuf = new char[nBufferSize];
	while(!uid_map_file.eof()) {
		// ファイルを読み込む
		uid_map_file.read( pBuf, nBufferSize );
		std::cout << pBuf << std::endl;
	}
	uid_map_file.close();
	delete pBuf;
}
void setupUidMap(uid_t uid) {
	std::string uid_map_fn= "/proc/self/uid_map";
	std::ofstream uid_map_file;
	uid_map_file.open(uid_map_fn, std::ios::out);
	uid_map_file << 0 << ' ' << uid << ' ' << 1 << std::endl;
	uid_map_file.close();
}

void setupGidMap(gid_t gid) {
	std::string gid_map_fn= "/proc/self/gid_map";
	std::ofstream gid_map_file;
	gid_map_file.open(gid_map_fn, std::ios::out);
	gid_map_file << 0 << ' ' << gid << ' ' << 1 << std::endl;
	gid_map_file.close();
}

void run()
{
	// user
	uid_t uid = geteuid();
	gid_t gid = getgid();
	printf("%d\n", uid);
	printf("%d\n", gid);

	// unshare
	int flags = CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWUSER | CLONE_NEWUTS;
	unshare(flags);

	setupUidMap(uid);
	setupGidMap(gid);

	// exec
	// char *const argv[] = {"/proc/self/exe", "init"};
	execl("/proc/self/exe", "init");

}

void initContainer() {
	std::cout << "here" << std::endl;
	size_t len = sizeof(char);
	err = sethostname("container", len*9);
	std::cout << err << std::endl;
	char *const argv[] = {"/bin/sh", ""};
	execv("/bin/sh", argv);
}


int main(int argc, const char **argv)
{
	std::string argv1 = std::string(argv[1]);
	std::cout << argv1 << std::endl;
	if (argv1 == "run") {
		std::cout << "if" << std::endl;
		run();
	}
	if (argv1 == "init") {
		std::cout << "elif" << std::endl;
		initContainer();
	}
	return 0;
}
