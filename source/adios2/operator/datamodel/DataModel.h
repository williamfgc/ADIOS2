/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * DataModel.h
 *
 *  Created on: Jan 9, 2018
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_OPERATOR_DATAMODEL_DATAMODEL_H_
#define ADIOS2_OPERATOR_DATAMODEL_DATAMODEL_H_

namespace adios2
{

class DataModel : public Operator
{
public:
    /**
     * Unique constructor
     * @param params
     * @param debugMode
     */
    DataModel(const Params &params = Params(), const bool debugMode = true);

    ~DataModel() = default;
};

} // end namespace adios2

#endif /* ADIOS2_OPERATOR_DATAMODEL_DATAMODEL_H_ */
