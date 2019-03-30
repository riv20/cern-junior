#pragma once

#include <iostream>

namespace excptn{
	const std::invalid_argument ZERO_VECTOR_UNITARY("Could not normalize zero-vector");
	const std::out_of_range BAD_VECTOR3D_ACCESS("Could not access Vector3D's i-th coordinates for i≠0,1,2");
	const std::out_of_range BAD_RGB_ACCESS("Could not access RGB's i-th value for i≠0,1,2");

	const std::invalid_argument NON_MATCHING_LINK_POINTS("Could not link elements with non-matching exit/entry points");
	const std::invalid_argument ZERO_CURVATURE_CENTER("Center of circle with zero curvature is undefined");
	const std::invalid_argument ZERO_CURVATURE_DIPOLE("Dipole must have nonzero curvature");

}