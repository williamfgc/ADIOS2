/*
 * VisVTKm.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: wfg
 */

#include "VisVTKm.h"

#include <vtk-m.h>

namespace adios2
{

bool VisVTKm::RenderAllVariables(IO &io)
{
    for (auto &variableName : m_VariablesNames)
    {
        auto *var1 = io.GetVariableBase("variableName");

        for( auto& transform : var1->m_TransformsInfo )
        {
        	for(auto& parameter : transform.Parameters)
        	{
        		if( parameter.first == "X0" )
        		{
        			auto value = parameter.second;
        			///CAll VTKm magic
        		}

        	}
        }
    }
}
}
