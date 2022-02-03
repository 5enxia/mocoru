#include<unistd.h>
#include<sched.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>

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

int main(int argc, char const **argv)
{
	// user
	uid_t uid = geteuid();
	gid_t gid = getgid();
	printf("%d\n", uid);
	printf("%d\n", gid);

	// unshare
	int flags = CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWUSER;
	unshare(flags);

	setupUidMap(uid);
	setupGidMap(gid);

	// exec
	const char *path = "/bin/sh";
	char *const *argv2;
	execv(path, argv2);

	return 0;
}
