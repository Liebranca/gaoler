#ifndef __2A_COLLISION_H__
#define __2A_COLLISION_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Collision {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  bool      m_hit    = false;

  vec3 m_normal = {0,0,0};
  vec3 m_point  = {0,0,0};

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  inline void set(vec3& n,vec3& p) {
    m_hit    = true;
    m_normal = n;
    m_point  = p;

  };

  // ctrash
  Collision(void) {};
  ~Collision(void) {};

  // undoes false positive
  inline void fake(void) {
    m_hit=false;

  };

  // getters
  inline bool hit(void) {
    return m_hit;

  };

  inline vec3& normal(void) {
    return m_normal;

  };

  inline vec3& point(void) {
    return m_point;

  };

};};

// ---   *   ---   *   ---
// sugar

namespace Gaol {
  typedef std::vector<Collision> Collisions;

};

// ---   *   ---   *   ---

#endif // __2A_COLLISION_H__

