#include "../misc/exceptions.h"
#include "beam.h"


Beam::Beam(Accelerator& machine, const Particle &p, uint number_of_particles, double my_lambda) :
	Drawable(machine.getCanvas()),
	model_particle(p.copy()),
	N(number_of_particles / my_lambda),
	lambda(my_lambda),
	habitat(&machine)
{
	if(my_lambda <= simcst::ZERO_LAMBDA) throw excptn::ZERO_LAMBDA;
}

void Beam::update(void){
	// TODO
}

double Beam::mean_energy(void) const{
	double mean(0.0);
	for(const auto &p : *this){
		mean += (*p)->getEnergy();
	}
	return N ? (lambda/N) * mean : 0.0;
}

Vector3D Beam::mean_position(void) const{
	double radial(0.0);
	double vertical(0.0);
	int n(0);
	for(const auto &p : *this){
		if(*p){
			radial += (**p)[0]; // TODO project on local trajectory
			vertical += (**p)[1];
			++n;
		}
	}
	Vector3D mean(radial, vertical);
	return n ? n*mean : mean;
}

Vector3D Beam::mean_velocity(void) const{
	double radial(0.0);
	double vertical(0.0);
	int n(0);
	for(const auto &p : *this){
		if(*p){
			radial += (**p).getVelocity()[0]; // TODO project on local trajectory
			vertical += (**p).getVelocity()[1];
			++n;
		}
	}
	Vector3D mean(radial, vertical);
	return n ? n*mean : mean;
}

double Beam::vertical_emittance(void) const{
	// TODO
}

double Beam::radial_emittance(void) const{
	// TODO
}

std::array<double,3> Beam::radial_ellipse_coefficients(void) const{
	// TODO
}

std::array<double,3> Beam::vertical_ellipse_coefficients(void) const{
	// TODO
}

void Beam::evolve(double dt){
	// TODO
}

std::ostream& Beam::print(std::ostream& output) const{
	output << "Model particle:\n" << *model_particle;
	// TODO
	return output;
}

void CircularBeam::activate(){
	if(N == 0) return;

	double l(habitat->getLength());
	double h(l / N);

	const double v(model_particle->getVelocity().norm());
	for(size_t i(1); i <= N; ++i){
		std::array<Vector3D,2> position_and_trajectory(habitat->position_and_trajectory(i*h));

		std::unique_ptr<Particle> copy(model_particle->copy());

		copy->setPosition(position_and_trajectory[0]);
		copy->setVelocity((model_particle->getCharge() >= 0 ? v : -v)*position_and_trajectory[1]);
		copy->scale(lambda);

		habitat->addParticle(*copy);
	}
	shrink_to_fit();
}
