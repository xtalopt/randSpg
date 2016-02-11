/**********************************************************************
  main.cpp -- Main function for spacegroup initialization.

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
#include "spgInit.h"
#include "spgInitOptions.h"

using namespace std;

// These are externs declared in spgInit.h
string e_logfilename = "spgInit.log";
char e_verbosity = 'r';

int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Usage: ./spgInit <inputFileName>\n";
    return -1;
  }

  // If there is an old log file here, remove it
  remove(e_logfilename.c_str());

  SpgInitOptions options = SpgInitOptions::readOptions(argv[1]);
  options.printOptions();

  vector<uint> atoms;

  string comp = options.getComposition();

  if (!ElemInfo::readComposition(comp, atoms)) {
    cout << "Error: Composition, " << comp << ", is an invalid composition.\n";
    return -1;
  }

  e_verbosity = options.getVerbosity();

  // Set the min radius
  ElemInfo::setMinRadius(options.getMinRadii());

  // Set some explicit min radii
  vector<pair<uint, double>> minRadiusVector = options.getMinRadiusVector();
  for (size_t i = 0; i < minRadiusVector.size(); i++) {
    uint atomicNum = minRadiusVector.at(i).first;
    double rad = minRadiusVector.at(i).second;
    ElemInfo::setRadius(atomicNum, rad);
  }

  // Set up lattice mins and maxes
  latticeStruct mins  = options.getLatticeMins();
  latticeStruct maxes = options.getLatticeMaxes();

  // Set up various other options
  vector<uint> spacegroups = options.getSpacegroups();
  int numOfEach = options.getNumOfEachSpgToGenerate();
  double minIADScalingFactor = options.getScalingFactor();
  int maxAttempts = options.getMaxAttempts();
  string outDir = options.getOutputDir();

#ifdef _WIN32
  outDir += "\\";
#else
  outDir += "/";
#endif

  for (size_t i = 0; i < spacegroups.size(); i++) {
    uint spg = spacegroups.at(i);
    for (size_t j = 0; j < numOfEach; j++) {
      Crystal c = SpgInit::spgInitCrystal(spg, atoms, mins, maxes,
                                          minIADScalingFactor, maxAttempts);

      string filename = outDir + comp + "_" + to_string(spg) +
                        "-" + to_string(j + 1);
      string title = comp + " -- spgInit with spg of: " + to_string(spg);
      // The volume is set to zero if the job failed.
      if (c.getVolume() != 0) c.writePOSCAR(filename, title);
    }
  }
}
