#ifndef __2A_SPHERE_H__
#define __2A_SPHERE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "Box.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Sphere {

public:

  VERSION   "v2.00.4";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  vec3  m_origin;
  float m_radius;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  inline void set(vec3& o,float r) {
    m_origin=o;
    m_radius=r;

  };

  // ctrash
  Sphere(void) {};
  ~Sphere(void) {};

  // point inside sphere
  inline bool isect_point(vec3& p) {
    return glm::distance(m_origin,p) < m_radius;

  };

  // sphere-line intersection
  Collision isect_ray(Line& ray);

  // sphere-plane intersection
  int isect_plane(Plane& plane);

  // sphere-cuboid intersection
  int isect_box(Box& box);

  // sphere-sphere intersection
  Collision isect_sphere(Sphere& other);

  // getters
  inline vec3& origin(void) {
    return m_origin;

  };

};};

// ---   *   ---   *   ---

#endif // __2A_SPHERE_H__
