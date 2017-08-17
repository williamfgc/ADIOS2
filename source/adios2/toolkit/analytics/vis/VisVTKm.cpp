/*
 * VisVTKm.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: wfg
 */

#include "VisVTKm.h"
#include <iostream>

namespace adios2
{

bool VisVTKm::RenderAllVariables()
{
    for (auto &visVariable : m_VisVariables)
    {
        auto &var = visVariable.VisVariable;
        std::cout << var.m_Shape.size() << " : " << var.m_Shape[0] << std::endl;

        for (auto &transform : var.m_TransformsInfo)
        {
            // transform parameters
            transform.Operator.m_Parameters;

            std::cout << __LINE__ << std::endl;
            for (auto &parameter : transform.Parameters)
            {
                const std::string key(parameter.first);
                const std::string value(parameter.second);

                std::cout << parameter.first << std::endl;
                if (key == "X1")
                {
                    auto value = parameter.second;
                    std::cout << __LINE__ << std::endl;
                    std::cout << "Meow" << std::endl;
                    /// CAll VTKm magic
                }
            }
        }
    }

    return true;
}
}
