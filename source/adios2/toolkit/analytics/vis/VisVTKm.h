/*
 * VisVTKm.h
 *
 *  Created on: Aug 16, 2017
 *      Author: William F Godoy
 */

#ifndef ADIOS2_TOOLKIT_ANALYTICS_VIS_VISVTKM_H_
#define ADIOS2_TOOLKIT_ANALYTICS_VIS_VISVTKM_H_

#include <set>
#include <vector>
#include "adios2/core/IO.h"

#include "adios2/ADIOSTypes.h"

namespace adios2
{

class VisVTKm
{
public:
    std::set<std::string> m_VariablesNames;

    bool m_DoRendering = true;

    VisVTKm() {}
    VisVTKm(const bool debugMode);

    virtual ~VisVTKm() {}

    void InitParameters(const Params &parameters);

    template <class T>
    void SubscribeVariable(Variable<T> &variable)
    {
        for (auto &transformInfo : variable.m_TransformsInfo)
        {

            if (transformInfo.Operator.m_Library == "Vis")
            {
                m_VariablesNames.insert(variable.m_Name);
            }
        }
    }

    /**
     *
     * @return true: success, false: fails
     */
    bool RenderAllVariables(IO &io);
};

} // end namespace adios2

#endif /* SOURCE_ADIOS2_TOOLKIT_ANALYTICS_VIS_VISVTKM_H_ */
