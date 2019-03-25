#include <cmath>

#include "../misc/exceptions.h"

#include "../vector3d/vector3d.h"
#include "../physics/particle.h"
#include "element.h"

using namespace std;
using namespace basicvector;
using namespace excptn;

bool Element::is_straight(void) const{
	return abs(curvature) <= phcst::ZERO_DISTANCE;
}

std::ostream& Element::print(std::ostream& output) const{
	cout << "   Entry point: " << entry_point
	     << "\n   Exit point: " << exit_point
	     << "\n   Chamber radius: " << radius
	     << "\n   Curvature: " << curvature;
	if(not is_straight()){
		cout << "\n   Center of curvature: " << center();
	}
	return output;
}

std::ostream& operator<<(std::ostream& output, const Element &E){
	return E.print(output);
}

void Element::link(Element &next_element){
	if(exit_point != next_element.entry_point) throw NON_MATCHING_LINK_POINTS;
	successor = &next_element;
}

Vector3D Element::center(void) const{
	if(is_straight()) throw ZERO_CURVATURE_CENTER;
	Vector3D direction(exit_point - entry_point);
	return 0.5*(entry_point + exit_point) + (1.0/curvature)*sqrt(1.0-curvature*curvature*direction.norm2()/4.0)*(direction.unitary()^Z_VECTOR);
}

std::vector<Vector3D> Element::sample_points(void) const{
	// we want the sample points of an element to be evenly distributed on the circle of its entry surface
	// i.e. a circle orthogonal to exit_point - entry_point and with radius that of the element (i.e. Element::radius)
	std::vector<Vector3D> list;

	double SAMPLE_POINT_DENSITY(1e-3);// number of points per unit of distance
				// note: move somewhere more appropriate

	Vector3D u((exit_point - entry_point).unitary());

	// constructs an orthonormal pair of vector {u,v} in the plane of the circle
	Vector3D v(u.orthogonal());
	u = u^v;

	int number_of_points(2*M_PI*radius/SAMPLE_POINT_DENSITY);

	for(int i(1); i <= number_of_points; ++i){
		double theta(SAMPLE_POINT_DENSITY * i);
		list.push_back(entry_point + sin(theta)*u + cos(theta)*v);
	}
	return list;
}

bool Element::has_collided(const Particle& p) const{
	if(is_straight()){
		Vector3D r(p.getPosition() - center()); // position relative to center
		return (r - 1.0/abs(curvature)*(r - r[2]*Z_VECTOR)).norm2() >= radius*radius;
	}else{
		Vector3D d((exit_point - entry_point).unitary());
		Vector3D r(p.getPosition() - entry_point); // position relative to entry point
		return (r - (r|d)*d).norm2() >= radius*radius;
	}
}

bool Element::has_left(const Particle& p) const{
	return Vector3D::mixed_prod(Z_VECTOR, p.getPosition(), entry_point) >= 0;
}

std::ostream& StraightSection::print(std::ostream& output) const{
	output << "Straight section:\n";
	Element::print(output);
	return output;
}

void Magnetic_element::add_lorentz_force(Particle& p, double dt) const{
	p.add_magnetic_force(field(p.getPosition()), dt);
}

void Electric_element::add_lorentz_force(Particle& p, double dt) const{
	p.add_electric_force(field(p.getPosition()));
}

std::ostream& Dipole::print(std::ostream& output) const{
	output << "Dipole:\n";
	Element::print(output);
	cout << "\n   Magnetic amplitude: B_0 = " << B_0;
	return output;
}

std::ostream& Quadrupole::print(std::ostream& output) const{
	output << "Quadrupole:\n";
	Element::print(output);
	cout << "\n   Quadrupole parameter: b = " << b;
	return output;
}
