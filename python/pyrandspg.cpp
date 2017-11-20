#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <iostream>

#include "crystal.h"

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
      .def_readwrite("atomicNum", &atomStruct::atomicNum)
      .def_readwrite("x", &atomStruct::x)
      .def_readwrite("y", &atomStruct::y)
      .def_readwrite("z", &atomStruct::z);

  // The basic lattice struct. Only 6 variables here: a, b, c, alpha, beta,
  //  gamma.
  py::class_<latticeStruct>(m, "LatticeStruct",
                            "Basic lattice struct with a, b, c, alpha, beta, "
                            "gamma")
      .def(py::init())
      .def(py::init<double, double, double, double, double, double>(),
           py::arg("a") = 0.0, py::arg("b") = 0.0, py::arg("c") = 0.0,
           py::arg("alpha") = 0.0, py::arg("beta") = 0.0,
           py::arg("gamma") = 0.0)
      .def_readwrite("a", &latticeStruct::a)
      .def_readwrite("b", &latticeStruct::b)
      .def_readwrite("c", &latticeStruct::c)
      .def_readwrite("alpha", &latticeStruct::alpha)
      .def_readwrite("beta", &latticeStruct::beta)
      .def_readwrite("gamma", &latticeStruct::gamma);

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
      .def("printLatticeVecs", &Crystal::printLatticeVecs,
           "Prints to the console the lattice vectors")
      .def("printIADs", &Crystal::printIADs,
           "Prints to the console the interatomic distances");
}

