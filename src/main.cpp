
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

}
