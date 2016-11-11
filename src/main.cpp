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

// For timings
#include <chrono>
// To remove the log file
#include <cstdio>
#include <iostream>
#include <sstream>

#include "elemInfo.h"
#include "fileSystemUtils.h"
#include "randSpg.h"
#include "randSpgOptions.h"
#include "utilityFunctions.h"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Usage: ./randSpg <inputFileName>\n";
    return -1;
  }

  // Let's time it!
  auto setup_startTime = chrono::high_resolution_clock::now();

  string logFileName = string(argv[1]);

  // Remove ".in" ending if needed
  if (hasEnding(logFileName, ".in"))
    logFileName = logFileName.substr(0, logFileName.length() - 3);

  e_logfilename = logFileName + ".log";

  // If there is an old log file here, remove it
  remove(e_logfilename.c_str());

  RandSpgOptions options = RandSpgOptions::readOptions(argv[1]);

  if (!options.optionsAreValid()) {
    cout << "Warning: the options that were received are invalid\n";
    cout << "Please go back and check them.\n";
    exit(EXIT_FAILURE);
  }

  // Write the options to the log file
  RandSpg::appendToLogFile(options.getOptionsString());

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
  randSpgInput input(1, atoms, mins, maxes);

  // Add various other input options
  input.IADScalingFactor = options.getScalingFactor();
  input.minRadius = options.getMinRadii();
  input.manualAtomicRadii = options.getRadiusVector();
  input.customMinIADs = options.getCustomMinIADs();
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

  size_t numAttempts = spacegroups.size() * numOfEach;
  size_t numSucceeds = 0;

  double successTime = 0, failTime = 0;

  auto setup_wallTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - setup_startTime).count() * 0.000000001;

  // Time the loop
  auto start_loopTime = chrono::high_resolution_clock::now();

  for (size_t i = 0; i < spacegroups.size(); i++) {
    uint spg = spacegroups[i];
    // Change the input spg to have the right spacegroup
    input.spg = spg;
    for (size_t j = 0; j < numOfEach; j++) {
      auto start = chrono::high_resolution_clock::now();
      string filename = outDir + comp + "_" + to_string(spg) +
                        "-" + to_string(j + 1);
      if (e_verbosity != 'n')
        RandSpg::appendToLogFile(string("\n**** ") + filename + " ****\n");

      Crystal c = RandSpg::randSpgCrystal(input);

      string title = comp + " -- randSpg with spg of: " + to_string(spg);

      // The volume is set to zero if the job failed.
      if (c.getVolume() != 0) {
        // Success!
        c.writePOSCAR(filename, title);
        successTime += chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count() * 0.000000001;
        numSucceeds++;
      }

      // We failed! Add this to the fail time
      else failTime += chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count() * 0.000000001;
    }
  }

  auto loop_wallTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_loopTime).count() * 0.000000001;

  if (e_verbosity != 'n') {
    stringstream ss;
    ss << "\n-------------------------------------------------------------\n"
       << "Number of structures attempted: " << numAttempts << "\n"
       << "Number of structures succeeded: " << numSucceeds << "\n"
       << "Setup wall time (in seconds): " << setup_wallTime << "\n"
       << "Structure generation wall time (in seconds): "
       << loop_wallTime << "\n"
       << "Structure generation wall time per attempt (in seconds): "
       << loop_wallTime / ((double)numAttempts) << "\n"
       << "Average success wall time (in seconds): "
       << ((numSucceeds != 0) ?
           successTime / ((double)numSucceeds) : 0)
       << "\n"
       << "Average failure wall time (in seconds): "
       << ((numAttempts != numSucceeds) ?
           failTime / ((double)(numAttempts - numSucceeds)) : 0)
       << "\n"
       << "Total wall time (in seconds): " << setup_wallTime + loop_wallTime
       << "\n------------------------------------------------------------ \n";
    RandSpg::appendToLogFile(ss.str());
  }
}
