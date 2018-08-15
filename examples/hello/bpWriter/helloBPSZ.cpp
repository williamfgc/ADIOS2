/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloBPSZ.cpp : example passing runtime compression arguments
 *
 *  Created on: Aug 3, 2018
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <ios>      //std::ios_base::failure
#include <iostream> //std::cout
#include <mpi.h>
#include <numeric>   //std::iota
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

void Usage()
{
    std::cout << "\n";
    std::cout << "USAGE:\n";
    std::cout << "./helloBPSZ Nx sz_accuracy\n";
    std::cout << "\t Nx: size of float and double arrays to be compressed\n";
    std::cout << "\t sz_accuracy: absolute accuracy e.g. 0.1, 0.001, to skip "
                 "compression: -1\n\n";
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    try
    {

        if (argc != 3)
        {
            throw std::invalid_argument(
                "ERROR: need sz accuracy e.g. 0.01, 0.1 as "
                "2nd parameter in argv\n");
        }

        const std::size_t Nx = static_cast<std::size_t>(std::stoull(argv[1]));
        const double accuracy = std::stod(argv[2]);

        /** Application variable */
        std::vector<float> myFloats(Nx);
        std::vector<double> myDoubles(Nx);
        std::iota(myFloats.begin(), myFloats.end(), 0.);
        std::iota(myDoubles.begin(), myDoubles.end(), 0.);

        /** ADIOS class factory of IO class objects, DebugON is recommended */
        adios2::ADIOS adios(MPI_COMM_WORLD, adios2::DebugON);

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO bpIO = adios.DeclareIO("BPFile_SZ");

        adios2::Variable<float> varFloats = bpIO.DefineVariable<float>(
            "bpFloats", {size * Nx}, {rank * Nx}, {Nx}, adios2::ConstantDims);

        adios2::Variable<double> varDoubles = bpIO.DefineVariable<double>(
            "bpDoubles", {size * Nx}, {rank * Nx}, {Nx}, adios2::ConstantDims);

        if (accuracy > 1E-16)
        {
            adios2::Operator op = adios.DefineOperator("SZCompressor", "sz");
            varFloats.AddOperation(op,
                                   {{"accuracy", std::to_string(accuracy)}});
            varDoubles.AddOperation(op,
                                    {{"accuracy", std::to_string(accuracy)}});
        }

        adios2::Attribute<double> attribute =
            bpIO.DefineAttribute<double>("SZ_accuracy", accuracy);

        /** Engine derived class, spawned to start IO operations */
        adios2::Engine bpFileWriter =
            bpIO.Open("SZexample.bp", adios2::Mode::Write);

        for (unsigned int t = 0; t < 3; ++t)
        {
            bpFileWriter.BeginStep();

            // here you can modify myFloats, myDoubles per step

            bpFileWriter.Put(varFloats, myFloats.data());
            bpFileWriter.Put(varDoubles, myDoubles.data());
            bpFileWriter.EndStep();
        }

        /** Create bp file, engine becomes unreachable after this*/
        bpFileWriter.Close();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
        Usage();
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "IO System base failure exception, STOPPING PROGRAM "
                     "from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
        Usage();
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM from rank " << rank << "\n";
        std::cout << e.what() << "\n";
        Usage();
    }

    MPI_Finalize();

    return 0;
}