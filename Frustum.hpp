#ifndef __2A_FRUSTUM_H__
#define __2A_FRUSTUM_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

  #include "Bound.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Frustum {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  Box   m_box;

  float m_znear;
  float m_hnear;
  float m_wnear;

  float m_zfar;
  float m_hfar;
  float m_wfar;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void set(

    float height,
    float width,

    float fov,
    float near,
    float far

  );

  // ctrash
  Frustum(void) {};
  ~Frustum(void) {};

  // recalculate bounds
  void calc_box(
    glm::vec3& pos,
    glm::vec3& fwd,
    glm::vec3& up

  );

  // frustum-box intersection
  bool isect_box(Box& box);

  // ^prism-prism
  // same call, so just sugar
  inline bool isect_frustum(Frustum& other) {
    return this->isect_box(other.m_box);

  };

  // sphere wrapper
  int isect_sphere(Sphere& s) {
    return s.isect_box(m_box);

  };

  // checks sphere, then box
  int isect_bound(Bound& b);

  // box wrappers
  inline bool indom_col(Collision& col) {
    return m_box.indom_col(col);

  };

  inline bool isect_point(glm::vec3& p) {
    return m_box.isect_point(p);

  };

  // ^batch
  inline bool isect_points(Points& pts) {

    for(auto& p : pts) {
      if(! this->isect_point(p)) {
        return false;

      };

    };

    return true;

  };

};};

// ---   *   ---   *   ---

#endif // __2A_FRUSTUM_H__

