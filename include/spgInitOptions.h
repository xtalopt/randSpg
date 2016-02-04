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

  std::string m_filename, m_composition;

  std::vector<uint> m_spacegroups;

  bool m_latticeMinsSet, m_latticeMaxesSet;
  latticeStruct m_latticeMins, m_latticeMaxes;

  uint m_numOfEachSpgToGenerate;

  std::vector<std::pair<uint, double>> m_minRadiusVector;

  bool m_setAllMinRadii;

  double m_minRadii;

  char m_verbosity;

};
