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

#include <iostream>

#include "elemInfo.h"
#include "spgInit.h"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 3) {
    cout << "Usage: ./spgInit <composition> <spacegroup>\n";
    return -1;
  }

  vector<uint> atoms;

  if (!ElemInfo::readComposition(argv[1], atoms)) {
    cout << "Error: Composition, " << argv[1] << ", is an invalid composition.\n";
    return -1;
  }

  latticeStruct mins, maxes;
  mins.a = 3.0; maxes.a = 20.0;
  mins.b = 3.0; maxes.b = 20.0;
  mins.c = 3.0; maxes.c = 20.0;
  mins.alpha = 60.0; maxes.alpha = 120.0;
  mins.beta  = 60.0; maxes.beta  = 120.0;
  mins.gamma = 60.0; maxes.gamma = 120.0;

  uint spg = atoi(argv[2]);

  double minIADScalingFactor = 0.5;

  Crystal c = SpgInit::spgInitCrystal(spg, atoms, mins, maxes,
                                      minIADScalingFactor);

  string filename = string("/home/patrick/src/spgInit/build/") + string(argv[1]) + "_" + to_string(spg);
  string title = string(argv[1]) + " -- spgInit with spg of: " +
                 to_string(spg);
  if (c.getVolume() != 0) c.writePOSCAR(filename, title);
}
