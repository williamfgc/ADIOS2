#!/bin/bash

# builds a special adios2 version for Jupyter Notebooks cling using openmpi

set -e
set -x

mkdir build-dir
cd build-dir

cmake \
    -DCMAKE_INSTALL_PREFIX=$PREFIX \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DADIOS2_BUILD_EXAMPLES=OFF \
    -DADIOS2_BUILD_TESTING=OFF \
    -DADIOS2_USE_Python=ON \
    -DADIOS2_USE_Fortran=ON \
    -DADIOS2_USE_MPI=ON \
    -DADIOS2_USE_SST=ON \
    -DADIOS2_USE_DataMan=ON \
    -DADIOS2_USE_ZeroMQ=ON \
    -DADIOS2_USE_HDF5=OFF \
    -DADIOS2_USE_BZip2=ON \
    -DADIOS2_USE_PNG=ON \
    -DADIOS2_USE_Blosc=ON \
    -DADIOS2_USE_ZFP=OFF \
    -DADIOS2_USE_SZ=OFF \
    -DADIOS2_USE_MGARD=OFF \
    ..

make -j 8
make install

echo -e '#pragma cling add_library_path("'$PREFIX'/lib")' > $PREFIX/include/adios2_cling.h
echo -e '#pragma cling add_include_path("'$PREFIX'/include")' >> $PREFIX/include/adios2_cling.h
echo -e '#pragma cling load("adios2")' >> $PREFIX/include/adios2_cling.h
echo -e '#include <adios2.h>' >> $PREFIX/include/adios2_cling.h

