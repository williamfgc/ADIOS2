/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * FileSystem.h
 *
 *  Created on: Apr 16, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ECO_DATA_MODEL_FILESYSTEM_H_
#define ADIOS2_ECO_DATA_MODEL_FILESYSTEM_H_

#include "adios2/eco/EcoSystem.h"

#include <map>
#include <ostream>
#include <string>

#include "adios2/core/IO.h"
#include "adios2/core/Stream.h"

namespace adios2
{
namespace eco
{

class FileSystem : public EcoSystem
{
    enum class Entry
    {
        Variables,
        Attributes,
        All
    };

    FileSystem(core::IO &io, const std::string &location = "",
               const std::string separator = "/");

    ~FileSystem() = default;

    std::string PWD() const noexcept;

    void CD(const std::string &location = "");

    std::map<std::string, adios2::Params> LS(const std::string &names = "",
                                             const std::string &options = "",
                                             const Entry = Entry::All) const;

private:
    core::IO &m_IO;
    std::string m_Location;
};

}
} // end namespace adios2

#endif /*  ADIOS2_ECO_DATA_MODEL_FILESYSTEM_H_ */
