#ifndef __2A_FRUSTUM_H__
#define __2A_FRUSTUM_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "Bound.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Frustum {

public:

  VERSION   "v2.00.4";
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

    float width,
    float height,

    float fov,
    float near,
    float far

  );

  // ctrash
  Frustum(void) {};
  ~Frustum(void) {};

  // recalculate bounds
  void calc_box(
    vec3& pos,
    vec3& fwd,
    vec3& up

  );

  // frustum-box intersection
  bool isect_box(Box& b);

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

  inline bool isect_point(vec3& p) {
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

  // getters
  inline Box& box(void) {
    return m_box;

  };

};};

// ---   *   ---   *   ---

#endif // __2A_FRUSTUM_H__

