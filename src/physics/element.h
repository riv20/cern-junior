#pragma once
#include "../vector3d/vector3d.h"
#include "particle.h"
#include <memory>

class Element {
	private :
	Vector3D pos_en; // entry position
	Vector3D pos_ex; // exit position
	double radius; // radius of the vaccum chamber
	std::unique_ptr<Element> next_element; // points to the following element
	
	public :
	Vector3D getPos_en(void) const;
	Vector3D getPos_ex(void) const;
	double getRadius(void) const;

	virtual double shortest_distance(const Particle& p) const = 0; // returns the distance between p and the optimal path

	virtual bool collision_edge(const Particle& p) const = 0; // returns true if dist_opti(p) > radius
	bool isNext(const Particle& p) const; // returns true if p is after pos_ex
}; 

class Curve : public Element {
	private :
	double const k; // curvature
	
	public :
	double shortest_distance(const Particle& p) const override;
	Vector3D curv_center(void) const; // returns the center of curvature as a vector

	bool collision_edge(const Particle& p) const override;
	Vector3D Vector_field(const Vector3D& pos); // returns the vector assigned at position pos in the vector field
}; 

class Straight : public Element {
	public :
	double shortest_distance(const Particle& p) const override;
	bool collision_edge(const Particle& p) const override;
}; 

// note : je mettrai à jour le fichier réponse d'ici ce weekend, je voulais avoir ton avis :
// je pensais qu'il était mieux de considerer un champ comme une méthode
// l'écriture des méthodes collision edge, celle de Straight ne fais pas de copie mais est illisible, celle de Curve fais deux copies mais est lisible, que préfères tu ?
// pour finir la méthode shortest_distance est un plus qui sera peut être utile à l'avenir mais sinon on pourra juste la supprimer
// il me reste une méthode à définir à par ça
