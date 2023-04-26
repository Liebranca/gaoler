#ifndef __2A_SPHERE_H__
#define __2A_SPHERE_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

  #include "Box.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Sphere {

public:

  VERSION   "v2.00.3";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  glm::vec3 m_origin;
  float     m_radius;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  inline void set(glm::vec3& o,float r) {
    m_origin=o;
    m_radius=r;

  };

  // ctrash
  Sphere(void) {};
  ~Sphere(void) {};

  // point inside sphere
  inline bool isect_point(glm::vec3& p) {
    glm::vec3 dist=m_origin-p;
    return glm::length(dist) < m_radius;

  };

  // sphere-plane intersection
  int isect_plane(Plane& plane);

  // sphere-cuboid intersection
  int isect_box(Box& box);

  // sphere-sphere intersection
  Collision isect_sphere(Sphere& other);

  // getters
  inline glm::vec3& origin(void) {
    return m_origin;

  };

};};

// ---   *   ---   *   ---

#endif // __2A_SPHERE_H__
