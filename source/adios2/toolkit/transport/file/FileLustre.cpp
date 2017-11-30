/*
 * FileLustre.cpp
 *
 *  Created on: Nov 20, 2017
 *      Author: lwk
 */
#ifndef LUSTRE_NEW
#include <lustre/liblustreapi.h>
#include <lustre/lustre_user.h>
#else
#include <lustre/lustre_user.h>
#include <lustre/lustreapi.h>
#endif

#include "FileLustre.h"

namespace adios2
{
namespace transport
{
FileLustre::FileLustre(const size_t stripeSize, const size_t stripeOffset,
                       const size_t stripeCount, const size_t stripePattern,
                       MPI_Comm mpiComm, const bool debugMode)
: Transport("File", "Lustre", mpiComm, debugMode), m_StripeSize(stripeSize),
  m_StripeOffset(stripeOffset), m_StripeCount(stripeCount),
  m_StripePattern(stripePattern)
{
}

void FileLustre::Open(const std::string &name, const Mode openMode)
{
    m_Name = name;
    CheckName();
    m_OpenMode = openMode;

    switch (m_OpenMode)
    {

    case (Mode::Write):
        ProfilerStart("open");
        MkDir(m_Name);
        const int status = llapi_file_create(m_StripeSize, m_StripeOffset,
                                             m_StripeCount, m_StripePattern);
        if (status)
        {
            throw std::runtime_error(
                "ERROR: couldn't create file using Lustre API!");
        }
        m_FileDescriptor =
            open(m_Name.c_str(),
                 O_WRONLY | O_CREAT | O_TRUNC | O_LOV_DELAY_CREATE, 0777);
        ProfilerStop("open");
        break;

    case (Mode::Append):
        ProfilerStart("open");
        m_FileDescriptor = open(m_Name.c_str(), O_RDWR);
        ProfilerStop("open");
        break;

    case (Mode::Read):
        ProfilerStart("open");
        m_FileDescriptor = open(m_Name.c_str(), O_RDONLY);
        ProfilerStop("open");
        break;

    default:
        CheckFile("unknown open mode for file " + m_Name +
                  ", in call to Lustre-POSIX open");
    }

    CheckFile(
        "couldn't open file " + m_Name +
        ", check permissions or path existence, in call to Lustre-POSIX open");

    m_IsOpen = true;
}
}
}
