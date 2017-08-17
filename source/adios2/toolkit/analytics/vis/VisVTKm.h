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

#include "adios2/ADIOSTypes.h"
#include "adios2/core/IO.h"

namespace adios2
{

struct VariableInfo
{
    VariableBase &VisVariable;
    const void *Data;
    const size_t Size;
};

class VisVTKm
{
public:
    std::vector<VariableInfo> m_VisVariables;

    bool m_DoRendering = true;

    VisVTKm() {}
    VisVTKm(const bool debugMode);

    virtual ~VisVTKm() {}

    void InitParameters(const Params &parameters);

    template <class T>
    void SubscribeVariable(Variable<T> &variable, const void *data,
                           const size_t size)
    {
        for (auto &transformInfo : variable.m_TransformsInfo)
        {

            if (transformInfo.Operator.m_Library == "Vis")
            {
                m_VisVariables.push_back(VariableInfo{variable, data, size});
            }
        }
    }

    /**
     *
     * @return true: success, false: fails
     */
    bool RenderAllVariables();
};

} // end namespace adios2

#endif /* SOURCE_ADIOS2_TOOLKIT_ANALYTICS_VIS_VISVTKM_H_ */
