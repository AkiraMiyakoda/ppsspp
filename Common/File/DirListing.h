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

	// Currently only supported for Android storage files.
	// Other places use different methods to get this.
	uint64_t lastModified = 0;

	bool operator <(const FileInfo &other) const;
};

bool GetFileInfo(const char *path, FileInfo *fileInfo);

enum {
	GETFILES_GETHIDDEN = 1,
	GETFILES_URIENCODE_ANDROID = 2,  // Android shenanigans
};

size_t GetFilesInDir(const char *directory, std::vector<FileInfo> *files, const char *filter = nullptr, int flags = 0);
int64_t GetDirectoryRecursiveSize(const std::string &path, const char *filter = nullptr, int flags = 0);

#ifdef _WIN32
std::vector<std::string> GetWindowsDrives();
#endif

}
