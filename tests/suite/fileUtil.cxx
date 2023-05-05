#include "fileUtil.h"
#include "benchmark/Benchmark.h"
#include <glob.h>
#include <cstdio>
#include <fstream>

std::vector<std::string> readDirectory(std::string dir)
{
  if (*dir.rbegin() != '/') 
    dir += "/";
  dir += "*";

  glob_t glob_result;
  glob(dir.c_str(), GLOB_TILDE, NULL, &glob_result);

  std::vector<std::string> filenames;
  for(unsigned int i = 0; i <  glob_result.gl_pathc; i++) {
    std::string filename = glob_result.gl_pathv[i];
    filenames.push_back(filename);
  }

  std::vector<FILE*> files;
  for (unsigned int i = 0; i < filenames.size(); i++)
      files.push_back(loadFile(filenames[i]));

  globfree(&glob_result);
  return filenames;

}

std::vector<std::pair<double, double> > loadOffsets(std::string filename)
{
  // FIXME: error handling
  std::vector<std::pair<double, double> > offsets;
  std::ifstream is(filename);
  int x, y;
  while (is >> x >> y) {
    offsets.push_back({x, y});
  }
  return offsets;
}

FILE* loadFile(std::string filename)
{
  FILE* file;
  file = std::fopen(filename.c_str(), "r");
  if (!file)
  {
    std::perror("Error opening file");
    return NULL;
  }
return file;
}
