/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * EcoSystem.cpp
 *
 *  Created on: Apr 16, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "../eco/EcoSystem.h"

namespace adios2
{

EcoSystem::EcoSystem(const std::string &tool, core::IO *io)
: m_Tool(tool), m_IO(io)
{
}

} // end namespace adios2
