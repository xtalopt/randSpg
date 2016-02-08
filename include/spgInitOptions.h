/**********************************************************************
  spgInitOptions.h - Options class for spacegroup initialization.

  Copyright (C) 2015 - 2016 by Patrick S. Avery

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

 ***********************************************************************/

#ifndef SPG_INIT_OPTIONS_H
#define SPG_INIT_OPTIONS_H

#include <string>
#include <vector>

// This is for 'latticeStruct'
#include "crystal.h"

class SpgInitOptions {
 public:
  explicit SpgInitOptions();

  /* Reads options from a specified file and returns a SpgInitOptions object
   * that has the options set.
   *
   * @param filename The name of the file to be read.
   *
   * @return The SpgInitOptions object that has the options set.
   */
  static SpgInitOptions readOptions(std::string filename);

  /* Reads a line and sets an option based upon the contents
   *
   * @param line The line to be interpreted
   */
  void interpretLineAndSetOption(std::string line);

  /* Reads all the options and returns a string that gives all the information.
   * This string is meant to be printed to the terminal or a log file.
   *
   * @return The options info.
   */
  std::string getOptionsString() const;

  /* Call Print options info to the terminal. Calls getOptionsString() and
   * prints the string to the terminal output.
   */
  void printOptions() const;

  // Getters
  std::string getFileName() const {return m_filename;};
  std::string getComposition() const {return m_composition;};
  std::vector<uint> getSpacegroups() const {return m_spacegroups;};
  bool latticeMinsSet() const {return m_latticeMinsSet;};
  bool latticeMaxesSet() const {return m_latticeMaxesSet;};
  latticeStruct getLatticeMins()  const {return m_latticeMins;};
  latticeStruct getLatticeMaxes() const {return m_latticeMaxes;};
  uint getNumOfEachSpgToGenerate() const {return m_numOfEachSpgToGenerate;};
  std::vector<std::pair<uint, double>> getMinRadiusVector() const {return m_minRadiusVector;};
  bool setAllMinRadii() const {return m_setAllMinRadii;};
  double getMinRadii() const {return m_minRadii;};
  double getScalingFactor() const {return m_scalingFactor;};
  int getMaxAttempts() const {return m_maxAttempts;};
  std::string getOutputDir() const {return m_outputDir;};
  char getVerbosity() const {return m_verbosity;};

 private:
  std::string m_filename, m_composition;

  std::vector<uint> m_spacegroups;

  bool m_latticeMinsSet, m_latticeMaxesSet;
  latticeStruct m_latticeMins, m_latticeMaxes;

  uint m_numOfEachSpgToGenerate;

  std::vector<std::pair<uint, double>> m_minRadiusVector;

  bool m_setAllMinRadii;

  double m_minRadii;

  double m_scalingFactor;

  int m_maxAttempts;

  std::string m_outputDir;

  char m_verbosity;

};

#endif
