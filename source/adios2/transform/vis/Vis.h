/*
 * Vis.h
 *
 *  Created on: Aug 16, 2017
 *      Author: wfg
 */

#ifndef SOURCE_ADIOS2_TRANSFORM_VIS_VIS_H_
#define SOURCE_ADIOS2_TRANSFORM_VIS_VIS_H_

#include "adios2/core/Transform.h"

namespace adios2
{
namespace transform
{

class VisVTKm : public Transform
{
public:
//    VisVTKm() {}
    VisVTKm(bool debugMode) : Transform("vis", debugMode) {}
};

}
}

#endif /* SOURCE_ADIOS2_TRANSFORM_VIS_VIS_H_ */
