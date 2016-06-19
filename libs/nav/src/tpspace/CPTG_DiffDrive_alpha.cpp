/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2016, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#include "nav-precomp.h" // Precomp header
#include <mrpt/nav/tpspace/CPTG_DiffDrive_alpha.h>
#include <mrpt/system/os.h>

using namespace mrpt;
using namespace mrpt::nav;
using namespace mrpt::system;
using namespace mrpt::utils;

IMPLEMENTS_SERIALIZABLE(CPTG_DiffDrive_alpha,CParameterizedTrajectoryGenerator,mrpt::nav)

void CPTG_DiffDrive_alpha::setParams(const mrpt::utils::CConfigFileBase &cfg,const std::string &sSection,  const std::string &sKeyPrefix)
{
	this->cte_a0v        = mrpt::utils::DEG2RAD( cfg.read_double  (sSection, sKeyPrefix+std::string("cte_a0v_deg"), .0, true ) );
	this->cte_a0w        = mrpt::utils::DEG2RAD( cfg.read_double  (sSection, sKeyPrefix+std::string("cte_a0w_deg"), .0, true ) );

	CPTG_DiffDrive_CollisionGridBased::setParamsCommon(cfg,sSection,sKeyPrefix);
}

std::string CPTG_DiffDrive_alpha::getDescription() const
{
	char str[100];
	os::sprintf(str,100,"CPTG_DiffDrive_alpha,av=%udeg,aw=%udeg",(int)RAD2DEG(cte_a0v),(int)RAD2DEG(cte_a0w) );
	return std::string(str);
}


void CPTG_DiffDrive_alpha::readFromStream(mrpt::utils::CStream &in, int version)
{
	CPTG_DiffDrive_CollisionGridBased::internal_readFromStream(in);

	switch (version)
	{
	case 0:
		in  >> cte_a0v >> cte_a0w;
		break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)
	};
}

void CPTG_DiffDrive_alpha::writeToStream(mrpt::utils::CStream &out, int *version) const
{
	if (version) 
	{
		*version = 0;
		return;
	}

	CPTG_DiffDrive_CollisionGridBased::internal_writeToStream(out);
	out << cte_a0v << cte_a0w;
}
/*---------------------------------------------------------------
						ptgDiffDriveSteeringFunction
  ---------------------------------------------------------------*/
void CPTG_DiffDrive_alpha::ptgDiffDriveSteeringFunction( float alpha, float t,float x, float y, float phi, float &v, float &w ) const
{
	MRPT_UNUSED_PARAM(t); MRPT_UNUSED_PARAM(x); MRPT_UNUSED_PARAM(y);
    float At_a = alpha - phi;

    while (At_a>M_PI) At_a -= (float) M_2PI;
    while (At_a<-M_PI) At_a += (float) M_2PI;

    v = V_MAX * exp(-square( At_a / cte_a0v ));
    w=  W_MAX * (-0.5f + (1/(1+exp(-At_a/cte_a0w))));
}
