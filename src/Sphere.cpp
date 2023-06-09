// ---   *   ---   *   ---
// SPHERE
// Infinite, equidistant points
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "Sphere.hpp"

// ---   *   ---   *   ---

namespace Gaol {

// ---   *   ---   *   ---
// sphere-line intersection

Collision Sphere::isect_ray(Line& ray) {

  Collision out;

  vec3 p=ray.point_from_dist(
    glm::distance(ray.point(0),m_origin)

  );

  if(this->isect_point(p)) {
    vec3 n=glm::normalize(p-m_origin);
    out.set(n,p);

  };

  return out;

};

// ---   *   ---   *   ---
// sphere-plane intersection

int Sphere::isect_plane(Plane& plane) {

  // manual "inline" of isect_point
  // done so we can reuse vec
  vec3 vto=plane.centroid()-m_origin;

  // centroid inside sphere
  if(glm::length(vto) < m_radius) {
    return 1;

  };

  // ^else run the rougher check
  int   out = 0;
  float d   = plane.isect_point(m_origin);

  // *possibly* outside
  if(d < 0) {

    float x=glm::dot(vto,plane.normal());
    if(x < m_radius) {
      return -1;

    };

  // inside
  } else {
    out=1;

  };

  return out;

};

// ---   *   ---   *   ---
// sphere-cuboid intersection

int Sphere::isect_box(Box& box) {

  int inside=0;

  // sphere inside box
  if(box.isect_point(m_origin)) {
    return 1;

  };

  // ^else check every plane
  auto& planes=box.planes();
  for(auto& plane : planes) {

    int test=this->isect_plane(plane);
    if(test < 0) {
      return -1;

    };

    inside+=test;

  };

  return inside==6;

};

// ---   *   ---   *   ---
// sphere-sphere intersection

Collision Sphere::isect_sphere(Sphere& other) {

  Collision out;

  vec3  vto = other.m_origin-m_origin;
  float d   = glm::length(vto);

  if(

     d < m_radius
  || d < other.m_radius

  ) {

    vec3 p=(other.m_origin+vto)*0.5f;
    vto=glm::normalize(-vto);

    out.set(vto,p);

  };

  return out;

};

// ---   *   ---   *   ---
// decld at Box

int Box::isect_sphere(Sphere& s) {
  auto& b=*this;
  return s.isect_box(b);

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
