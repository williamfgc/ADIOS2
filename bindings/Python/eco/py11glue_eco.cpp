/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * py11glue_eco.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <stdexcept>

#include <adios2.h>

#include "py11File.h"

#ifdef ADIOS2_HAVE_MPI

namespace pybind11
{

PYBIND11_MODULE(adios2_eco, m) {}





}
