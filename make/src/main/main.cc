#include "common.h"
#include "CentralData.h"
#include "String.h"
#include "Configuration.h"

#ifndef __linux__
#error
#endif

namespace
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @function: initCentral
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
initCentral (const char * bin2source, const char * source2bin)
{ 
  CentralData::appendPathName (PATH_BIN_2_SOURCE, bin2source);
  CentralData::appendPathName (PATH_SOURCE_2_BIN, source2bin);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @function: generateMakefiles
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
generateMakefiles (const String & root_file_name)
{ 
  Configuration configuration (root_file_name);
}

};  // anonymous namespace

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @function: main
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
main (int argc, char ** argv)
{
  if (argc != 4)
  {
    printError ("Use is 'make_build <bin_2_src_path> <src_2_bin_path> <configuration_params_file>.\n");
    exit (1);
  }

  const char * bin2source = argv[1];
  const char * source2bin = argv[2];
  const char * root_file_name = argv[3];

  try
  {
    initCentral (bin2source, source2bin);
    generateMakefiles (root_file_name);
  }
  catch (...)
  {
    printError ("bin 2 source path is: '%s'.\n", argv[1], ERROR_COLOR_WHITE);
    printError ("source 2 bin path is: '%s'.\n", argv[2], ERROR_COLOR_WHITE);
    printError ("configuration params file is: '%s%s'.\n", argv[1], argv[3], ERROR_COLOR_WHITE);
    printError ("Aborting.\n");
    exit (1);
  }

  return 0;
}

