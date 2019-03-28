#pragma once

#include <iostream>

#include "element.h"

class Accelerator : public Drawable{
	friend class OpenGLView;
	private:
		void initialize(void);

		std::vector<Element*> element_list; // (ordered) list of elements that make up the accelerator
		std::vector<Particle*> particle_list; // list of particles
	public:
		Accelerator(Canvas* canvas = nullptr, std::vector<Element*> my_element_list = {}, std::vector<Particle*> my_particle_list = {}) :
			Drawable(canvas),	
			element_list(my_element_list),
			particle_list(my_particle_list){}

		virtual void draw(void) override{ canvas->draw(*this); }

		std::ostream& print(std::ostream& output) const;

		virtual void evolve(double dt) override;

		void setCanvas(Canvas* c);
};

std::ostream& operator<<(std::ostream& output, const Accelerator &A);
