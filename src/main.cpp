/**********************************************************************
  main.cpp -- Main function for spacegroup generation.

  Copyright (C) 2015 - 2016 by Patrick S. Avery

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

 ***********************************************************************/

// To remove the log file
#include <cstdio>
#include <iostream>

#include "elemInfo.h"
#include "fileSystemUtils.h"
#include "spgGen.h"
#include "spgGenOptions.h"

using namespace std;

// These are externs declared in spgGen.h
string e_logfilename = "spgGen.log";
char e_verbosity = 'r';

int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Usage: ./spgGen <inputFileName>\n";
    return -1;
  }

  // If there is an old log file here, remove it
  remove(e_logfilename.c_str());

  SpgGenOptions options = SpgGenOptions::readOptions(argv[1]);
  options.printOptions();

  vector<uint> atoms;

  string comp = options.getComposition();

  if (!ElemInfo::readComposition(comp, atoms)) {
    cout << "Error: Composition, " << comp << ", is an invalid composition.\n";
    return -1;
  }

  e_verbosity = options.getVerbosity();

  // Set up lattice mins and maxes
  latticeStruct mins  = options.getLatticeMins();
  latticeStruct maxes = options.getLatticeMaxes();

  // Create the input
  spgGenInput input(1, atoms, mins, maxes);

  // Add various other input options
  input.IADScalingFactor = options.getScalingFactor();
  input.minRadius = options.getMinRadii();
  input.manualAtomicRadii = options.getRadiusVector();
  input.minVolume = options.getMinVolume();
  input.maxVolume = options.getMaxVolume();
  input.forcedWyckAssignments = options.getForcedWyckAssignments();
  input.verbosity = options.getVerbosity();
  input.maxAttempts = options.getMaxAttempts();
  input.forceMostGeneralWyckPos = options.forceMostGeneralWyckPos();

  // Set up various other options
  vector<uint> spacegroups = options.getSpacegroups();
  int numOfEach = options.getNumOfEachSpgToGenerate();
  string outDir = options.getOutputDir();

#ifdef _WIN32
  outDir += "\\";
#else
  outDir += "/";
#endif

  // Defined in fileSystemUtils.h
  mkDir(outDir);

  for (size_t i = 0; i < spacegroups.size(); i++) {
    uint spg = spacegroups.at(i);
    // Change the input spg to have the right spacegroup
    input.spg = spg;
    for (size_t j = 0; j < numOfEach; j++) {
      string filename = outDir + comp + "_" + to_string(spg) +
                        "-" + to_string(j + 1);
      if (e_verbosity != 'n')
        SpgGen::appendToLogFile(string("\n**** ") + filename + " ****\n");

      Crystal c = SpgGen::spgGenCrystal(input);

      string title = comp + " -- spgGen with spg of: " + to_string(spg);
      // The volume is set to zero if the job failed.
      if (c.getVolume() != 0) c.writePOSCAR(filename, title);
    }
  }
}
