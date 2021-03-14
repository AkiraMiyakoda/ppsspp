#pragma once

#include <string>
#include <vector>

#include <cstdio>

#include <inttypes.h>

// Beginnings of a directory utility system. TODO: Improve.

namespace File {

struct FileInfo {
	std::string name;
	std::string fullName;
	bool exists = false;
	bool isDirectory = false;
	bool isWritable = false;
	uint64_t size = 0;

	uint64_t atime;
	uint64_t mtime;
	uint64_t ctime;
	uint32_t access;  // st_mode & 0x1ff

	bool operator <(const FileInfo &other) const;
};

bool GetFileInfo(const char *path, FileInfo *fileInfo);

enum {
	GETFILES_GETHIDDEN = 1,
	GETFILES_URIENCODE_ANDROID = 2,  // Android shenanigans
};
size_t GetFilesInDir(const char *directory, std::vector<FileInfo> *files, const char *filter = nullptr, int flags = 0);

// rootDir should NOT include the directory separator.
#ifndef _WIN32
size_t GetFilesInDirByFd(int fd, const std::string &rootDir, std::vector<FileInfo> *files, const char *filter = nullptr, int flags = 0);
#endif
int64_t GetDirectoryRecursiveSize(const std::string &path, const char *filter = nullptr, int flags = 0);

#ifdef _WIN32
std::vector<std::string> GetWindowsDrives();
#endif

}
