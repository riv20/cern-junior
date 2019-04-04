#pragma once

#include <vector>
#include <cmath>
#include <memory> // for unique_ptr

#include "../general/drawable.h"
#include "../misc/exceptions.h"

#include "../physics/particle.h"

class Element : public Drawable{
	protected:
		Vector3D entry_point; // entry position
		Vector3D exit_point; // exit position

		double radius; // radius of the vaccum chamber
		double curvature; // radial curvature (potentially zero)

		Element* successor = nullptr; // pointer to the following element
		Element* predecessor = nullptr; // pointer to the previous element

		std::vector<std::unique_ptr<Particle>> particle_list; // list of particles contained inside

		std::shared_ptr<double> clock;

		int number_of_particles = 0;

	public:
		Element(Canvas* display, const Vector3D& entry, const Vector3D& exit, double my_radius, double my_curvature, std::shared_ptr<double> my_clock) :
			Drawable(display),
			entry_point(entry),
			exit_point(exit),
			radius(my_radius),
			curvature(my_curvature),
			clock(my_clock)
		{}

		void draw_particles(void) const{ for(const auto &p : particle_list) p->draw(); }

		void setCanvas(Canvas* c){ canvas = c; }

		Vector3D getEntry_point(void) const{ return entry_point; }
		Vector3D getExit_point(void) const{ return exit_point; }
		double getRadius(void) const{ return radius; }
		double getCurvature(void) const{ return curvature; }

		double length(void) const;

		void setSuccessor(Element* my_successor){ successor = my_successor; }
		void setPredecessor(Element* my_predecessor){ predecessor = my_predecessor; }

		void addParticle(std::unique_ptr<Particle> &p);

		virtual ~Element(void){}

		virtual std::ostream& print(std::ostream& output) const;
		// Base method prints only basic information (i.e. about its shape)
		// Subclass overrides will add additional information e.g. type of the element, electric/magnetic fields, other parameters...

		bool print_elements(std::ostream& output) const;
		
		void link(Element &next_element);

		bool is_straight(void) const;
		Vector3D center(void) const; // returns the center of circular element assuming curvature is non-zero

		Vector3D direction(void) const; // returns the vector exit_point - entry_point
		Vector3D unit_direction(void) const; // returns director().unitary()

		Vector3D relative_coords(const Vector3D &x) const;
		Vector3D local_coords(const Vector3D &x) const;
		Vector3D inverse_curvilinear_coord(double s) const;

		double curvilinear_coord(const Vector3D &x) const;

		bool has_collided(const Vector3D &r) const; // returns true iff r has collided with the element's edge
		bool is_after(const Vector3D &r) const; // returns true iff r has passed to the next element
		bool is_before(const Vector3D &r) const; // returns true iff r has passed to the next element
		bool contains(const Vector3D &r) const; // returns true iff r has passed to the next element

		bool has_collided(const Particle &p) const{ return has_collided(p.getPosition()); }
		bool is_after(const Particle &p) const{ return is_after(p.getPosition()); }
		bool is_before(const Particle &p) const{ return is_before(p.getPosition()); }
		bool contains(const Particle &p) const{ return contains(p.getPosition()); }
		
		virtual void add_lorentz_force(Particle &, double) const{}
		virtual void evolve(double dt) override;
}; 

std::ostream& operator<<(std::ostream& output, const Element &E);

class StraightSection : public Element{
	public:
		StraightSection(Canvas* display, const Vector3D& entry, const Vector3D& exit, double my_radius, std::shared_ptr<double> my_clock) :
			Element(display, entry, exit, my_radius,0.0,my_clock)
		{}
		virtual ~StraightSection(void) override{}

		virtual std::ostream& print(std::ostream& output) const override;
		virtual void draw(void) override{ canvas->draw(*this); }

		virtual void add_lorentz_force(Particle& p, double dt) const override{ return; } // no electromagnetic interaction
};

class Electric_element : public Element{
	public:
		using Element::Element;
		virtual ~Electric_element(void) override{}

		virtual void add_lorentz_force(Particle& p, double dt) const override;
		virtual Vector3D E(const Vector3D &x, double t = 0.0) const = 0;
};

class Magnetic_element : public Element{
	public:
		using Element::Element;
		virtual ~Magnetic_element(void) override{}

		virtual void add_lorentz_force(Particle& p, double dt) const override;
		virtual Vector3D B(const Vector3D &x, double t = 0.0) const = 0;
};

class Dipole : public Magnetic_element{
	private:
		const double B_0; // constant amplitude of the magnetic field generated by the dipole
	public:
		Dipole(Canvas* display, const Vector3D& entry, const Vector3D& exit, double my_radius, double my_curvature, std::shared_ptr<double> my_clock, double my_B_0) :
			Magnetic_element(display, entry, exit, my_radius, my_curvature, my_clock), B_0(my_B_0) {}

		virtual std::ostream& print(std::ostream& output) const override;

		virtual void draw(void) override{ canvas->draw(*this); }

		virtual Vector3D B(const Vector3D &x, double dt) const override;
};

class Quadrupole : public Magnetic_element{
	private:
		const double b; // parameter of the quadrupole
	public:
		Quadrupole(Canvas* display, const Vector3D& entry, const Vector3D& exit, double my_radius, std::shared_ptr<double> my_clock, double my_b) :
			Magnetic_element(display, entry, exit, my_radius, 0.0, my_clock), b(my_b){}

		virtual Vector3D B(const Vector3D &x, double dt) const override;
		virtual std::ostream& print(std::ostream& output) const override;

		virtual void draw(void) override{ canvas->draw(*this); }
};

class RadiofrequencyCavity : public Electric_element{
	private:
		const double E_0;
		const double omega;
		const double kappa;
		const double phi;
	public:
		RadiofrequencyCavity(Canvas* display, const Vector3D& entry, const Vector3D& exit, double my_radius, double my_curvature, std::shared_ptr<double> my_clock, double my_E_0, double my_omega, double my_kappa, double my_phi) :
			Electric_element(display, entry, exit, my_radius, my_curvature, my_clock),
			E_0(my_E_0),
			omega(my_omega),
			kappa(my_kappa),
			phi(my_phi)
			{}

		virtual Vector3D E(const Vector3D &x, double dt) const override;
		virtual std::ostream& print(std::ostream& output) const override;

		virtual void evolve(double dt) override{ Element::evolve(dt); }

		virtual void draw(void) override{ canvas->draw(*this); }
};
