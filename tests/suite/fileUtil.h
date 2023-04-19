#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <string>
#include <vector>

// Returns a vector containing all filenames in thedirectory
std::vector<std::string> readDirectory(std::string dir);
FILE* loadFile(std::string filename);

#endif
