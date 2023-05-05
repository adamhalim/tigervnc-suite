#ifndef __SUITE_FILE_UTIL_H__
#define __SUITE_FILE_UTIL_H__

#include <string>
#include <vector>

// Returns a vector containing all filenames in thedirectory
std::vector<std::string> readDirectory(std::string dir);
std::vector<std::pair<double, double> > loadOffsets(std::string filename);
FILE* loadFile(std::string filename);

#endif // __SUITE_FILE_UTIL_H__
