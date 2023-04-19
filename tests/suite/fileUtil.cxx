#include "fileUtil.h"
#include <glob.h>
#include <cstdio>

std::vector<std::string> readDirectory(std::string dir)
{
  if (*dir.rbegin() != '/') 
    dir += "/";
  dir += "*";

  glob_t glob_result;
  glob(dir.c_str(), GLOB_TILDE, NULL, &glob_result);

  std::vector<std::string> filenames;
  for(unsigned int i = 0; i <  glob_result.gl_pathc; i++)
    filenames.push_back(glob_result.gl_pathv[i]);

  std::vector<FILE*> files;
  for (unsigned int i = 0; i < filenames.size(); i++)
      files.push_back(loadFile(filenames[i]));

  globfree(&glob_result);
  return filenames;

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
