/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * EcoSystem.h
 *
 *  Created on: Apr 16, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ECO_ECOSYSTEM_H_
#define ADIOS2_ECO_ECOSYSTEM_H_

#include <string>

#include "adios2/core/IO.h"

namespace adios2
{

class EcoSystem
{
public:
    const std::string m_Tool;
    core::IO *m_IO = nullptr;

    EcoSystem(const std::string &tool, core::IO *io);

    ~EcoSystem() = default;
};

} // end namespace adios2

#endif /* ADIOS2_BINDINGS_CXX11_CXX11_ECO_ECOSYSTEM_H_ */
