#ifndef PARTICLE_H
#define PARTICLE_H

#include "../vector3d/vector3d.h"
#include <vector>
#include <array>
#include <math.h>

typedef std::array<double,3> RGB;


class Particle{
	private:
		Vector3D r;// position at time t
		Vector3D r_p;// position at time t-dt

		Vector3D v;// velocity at time t 
		Vector3D v_p;// velocity at time t-dt

		Vector3D F;// resultant force being applied 

		// physical attributes
		double mass;
		double charge; 
		double radius;
		RGB color;
	public:
		static constexpr double DEFAULT_MASS = 1.0;
		static constexpr double DEFAULT_RADIUS = 1.0;
		static constexpr double G = 1.0;
		static constexpr double C = 3e8;
		static constexpr double GeV = 6.242e9;

		double getMass(void) const;
		double getCharge(void) const;
		double getRadius(void) const;

		double gamma(void) const;
		double energy(void) const;

		RGB getColor(void) const;

		const Vector3D* getPosition(void) const;
		const Vector3D* getForce(void) const;

		void setForce(const Vector3D &my_F);
		void incrementForce(const Vector3D &my_F);
		void addMagneticForce(const Vector3D &B, double dt);

		void evolve(double dt);

		Particle(double x, double y, double z, double v_x, double v_y, double v_z, double my_mass = DEFAULT_MASS, double my_charge = 0, double my_radius = DEFAULT_RADIUS, RGB my_color = {1.0,1.0,1.0}) :
			r(Vector3D(x,y,z)),
			r_p(Vector3D(x,y,z)),
			v(Vector3D(v_x,v_y,v_z)),
			v_p(Vector3D(v_x,v_y,v_z)),
			F(Vector3D()),
			mass(my_mass),
			charge(my_charge),
			radius(my_radius),
			color(my_color)

		{}

		Particle(Vector3D x_0, Vector3D v_0, double my_mass = DEFAULT_MASS, double my_charge = 0, double my_radius = DEFAULT_RADIUS, RGB my_color = {1.0,1.0,1.0}) :
			r(Vector3D(x_0)),
			r_p(Vector3D(x_0)),
			v(Vector3D(v_0)),
			v_p(Vector3D(v_0)),
			F(Vector3D()),
			mass(my_mass),
			charge(my_charge),
			radius(my_radius),
			color(my_color)
		{}
};

std::ostream& operator<<(std::ostream& output, Particle const& particle);

class Universe{
	private:
		std::vector<Particle> particle_list;
	public:
		std::vector<Particle>* getParticle_list(void);
		void new_particle(double x, double y, double z, double v_x, double v_y, double v_z, double my_mass = Particle::DEFAULT_MASS, double my_charge = 0, double my_radius = Particle::DEFAULT_RADIUS, RGB my_color = {1.0,1.0,1.0});
		void new_particle(Vector3D x_0, Vector3D v_0, double my_mass = Particle::DEFAULT_MASS, double my_charge = 0, double my_radius = Particle::DEFAULT_RADIUS, RGB my_color = {1.0,1.0,1.0});
		
		void clear_forces(void);
		void calculate_gravitational_forces(void);
		void evolve(double dt);
};

#endif
