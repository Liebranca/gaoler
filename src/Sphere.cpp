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
// sphere-plane intersection

int Sphere::isect_plane(Plane& plane) {

  // manual inline of point_isect
  // done so we can reuse vec
  glm::vec3 vto=plane.centroid()-m_origin;

  // centroid inside sphere
  if(glm::length(vto) < m_radius) {
    return 1;

  };

  // ^else run the rougher check
  int   out = 0;
  float d   = plane.point_isect(m_origin);

  // *possibly* outside
  if(d<0) {

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
    if(test<0) {
      return -1;

    };

    inside+=test;

  };

  return inside==6;

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
