#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <iostream>

#include "crystal.h"
#include "randSpg.h"

namespace py = pybind11;

PYBIND11_MODULE(pyrandspg, m) {

  m.doc() = "RandSpg Python Bindings";

  // The basic atom struct. Only 4 variables here: atomicNum, x, y, z.
  // The units are in fractional coordinates
  py::class_<atomStruct>(m, "AtomStruct",
                         "Basic atom class with fractional positional units")
      .def(py::init())
      .def(py::init<unsigned int, double, double, double>(),
           py::arg("atomicNum") = 0, py::arg("x") = 0.0,
           py::arg("y") = 0.0, py::arg("z") = 0.0)
      .def_readwrite("atomicNum", &atomStruct::atomicNum,
                     "The atomic number of the atom.")
      .def_readwrite("x", &atomStruct::x, "x position in fractional units.")
      .def_readwrite("y", &atomStruct::y, "y position in fractional units.")
      .def_readwrite("z", &atomStruct::z, "z position in fractional units.");

  // The basic lattice struct. Only 6 variables here: a, b, c, alpha, beta,
  //  gamma.
  py::class_<latticeStruct>(m, "LatticeStruct",
                            "Basic lattice struct with a, b, c (in Angstroms)"
                            "and alpha, beta, gamma (degrees)")
      .def(py::init())
      .def(py::init<double, double, double, double, double, double>(),
           py::arg("a") = 0.0, py::arg("b") = 0.0, py::arg("c") = 0.0,
           py::arg("alpha") = 0.0, py::arg("beta") = 0.0,
           py::arg("gamma") = 0.0)
      .def_readwrite("a", &latticeStruct::a,
                     "The magnitude of the a vector in Angstroms")
      .def_readwrite("b", &latticeStruct::b,
                     "The magnitude of the b vector in Angstroms")
      .def_readwrite("c", &latticeStruct::c,
                     "The magnitude of the a vector in Angstroms")
      .def_readwrite("alpha", &latticeStruct::alpha,
                     "The alpha angle in degrees")
      .def_readwrite("beta", &latticeStruct::beta,
                     "The beta angle in degrees")
      .def_readwrite("gamma", &latticeStruct::gamma,
                     "The gamma angle in degrees");

  py::class_<Crystal>(m, "Crystal", "A crystal class that contains a list of "
                      "AtomStructs and a LatticeStruct.")
      .def(py::init<latticeStruct, std::vector<atomStruct>, bool>(),
           py::arg("l") = latticeStruct(),
           py::arg("a") = std::vector<atomStruct>(),
           py::arg("usingVdwRad") = false)
      .def("setAtoms", &Crystal::setAtoms, "Set the list of atoms. "
           "Takes a list of AtomStructs as a single argument.")
      .def("getAtoms", &Crystal::getAtoms, "Get the list of atoms. "
           "Returns a list of AtomStructs")
      .def("numAtoms", &Crystal::numAtoms, "Get the number of atoms")
      .def("setLattice", &Crystal::setLattice, "Set the lattice. "
           "Takes a LatticeStruct as a single argument")
      .def("getLattice", &Crystal::getLattice, "Get the lattice. "
           "Returns a LatticeStruct")
      .def("setUsingVdwRadii", &Crystal::setUsingVdwRadii, "Set whether we "
           "are using Vdw radii or covalent radii")
      .def("usingVdwRadii", &Crystal::usingVdwRadii, "Whether or not we are "
           "using Vdw radii (as opposed to covalent radii")
      .def("addAtom", &Crystal::addAtom, "Add an atom to the crystal")
      .def("removeAtomAt", &Crystal::removeAtomAt, "Remove an atom from the "
           "crystal at a specified index")
      .def("getVolume", &Crystal::getVolume, "Get the volume in Angstroms "
           "cubed")
      .def("getPOSCARString", &Crystal::getPOSCARString,
           py::arg("title") = " ", "Get the crystal as a POSCAR string")
      .def("writePOSCAR", &Crystal::writePOSCAR,
           py::arg("filename") = "POSCAR",
           py::arg("title") = " ", "Write a POSCAR to a specified file.")
      .def("printAtomInfo",
           (void (Crystal::*)(void) const) &Crystal::printAtomInfo,
           "Prints to the console the atom info in the crystal")
      .def("printLatticeInfo", &Crystal::printLatticeInfo,
           "Prints to the console the lattice info in the crystal")
      .def("printCrystalInfo", &Crystal::printCrystalInfo,
           "Prints to the console both the atom info and the lattice info for "
           "the crystal")
      .def("printLatticeVecs", &Crystal::printLatticeVecs,
           "Prints to the console the lattice vectors")
      .def("printIADs", &Crystal::printIADs,
           "Prints to the console the interatomic distances");

  // The input for RandSpg
  py::class_<randSpgInput>(m, "RandSpgInput",
                           "Input struct for RandSpg. Required parameters are "
                           "(in order): spg, atomicNumbers, latticeMins, and "
                           "latticeMaxes.")
      .def(py::init<uint, const std::vector<uint>&,
                    const latticeStruct&, const latticeStruct&>())
      .def_readwrite("spg", &randSpgInput::spg,
                     "The space group to be generated. Unsigned integer.")
      .def_readwrite("atoms", &randSpgInput::atoms,
                     "The vector of atomic numbers.")
      .def_readwrite("latticeMins", &randSpgInput::latticeMins,
                     "The min values for the lattice.")
      .def_readwrite("latticeMaxes", &randSpgInput::latticeMaxes,
                     "The max values for the lattice.")
      .def_readwrite("IADScalingFactor", &randSpgInput::IADScalingFactor,
                     "Scaling factor for interatomic distances. For example, "
                     "'0.5' would imply that all atomic radii are 0.5 of "
                     "their original value. Default is 1.0.")
      .def_readwrite("minRadius", &randSpgInput::minRadius,
                     "Minimum radius for atomic radii in Angstroms. All "
                     "atomic radii below this radius will be set to this "
                     "value. Default is 0.0.")
      .def_readwrite("manualAtomicRadii", &randSpgInput::manualAtomicRadii,
                     "A list of pairs. Each pair is an atomic number "
                     "followed by a radius in Angstroms. You may append to "
                     "this list to set your own radii. The default is that "
                     "it is empty.")
      .def_readwrite("customMinIADs", &randSpgInput::customMinIADs,
                     "custom minIADs that the user can set. It is a list of "
                     "a pair of a pair of unsigned ints and a float. In the "
                     "innermost pair, the first uint is the first atomic "
                     "number, and the second uint is the second atomic "
                     "number. These two are paired with a float (the minIAD).")
      .def_readwrite("minVolume", &randSpgInput::minVolume,
                     "Minimum volume for the final crystal in Angstroms "
                     "cubed. Default is -1 (No min volume).")
      .def_readwrite("maxVolume", &randSpgInput::maxVolume,
                     "Maximum volume for the final crystal in Angstroms "
                     "cubed. Default is -1 (No max volume).")
      .def_readwrite("forcedWyckAssignments",
                     &randSpgInput::forcedWyckAssignments,
                     "A list of pairs. Each pair is an atomic number "
                     "followed by a character representing a Wyckoff letter. "
                     "This essentially 'forces' the program to use the "
                     "specified atomic number for the Wyckoff position "
                     "defined by the Wyckoff letter. If you wish to have an "
                     "atom type in a Wyckoff position multiple times, just "
                     "add to this list multiple items of it.")
      .def_readwrite("verbosity", &randSpgInput::verbosity,
                     "Verbosity for log file printing. 'v' for verbose "
                     "(prints all possibilities found), 'r' for regular "
                     "(prints Wyckoff assignments), or 'n' for none (prints "
                     "nothing other than what the options were set to). "
                     "Default is 'n'")
      .def_readwrite("maxAttempts", &randSpgInput::maxAttempts,
                     "Max number of attempts to satisfy the minIAD "
                     "requirements when placing atoms in the Wyckoff "
                     "positions. If it fails this many times, it will abort "
                     "the operation. Default is 100")
      .def_readwrite("forceMostGeneralWyckPos",
                     &randSpgInput::forceMostGeneralWyckPos,
                     "This forces the program to use the most general Wyckoff "
                     "position at least once. This ensures that the crystal "
                     "will be of the correct space group. If this is not set "
                     "to true, then more compositions are possible for some "
                     "space groups, but the final space group will not be "
                     "guaranteed to be the correct space group. "
                     "Default is true.");

  py::class_<RandSpg>(m, "RandSpg", "Static method class for performing "
                      "primary RandSpg procedures.")
      .def("randSpgCrystal", &RandSpg::randSpgCrystal, "Generate a random "
           "crystal with a specific space group and all other constraints "
           "given in the input struct");
}
