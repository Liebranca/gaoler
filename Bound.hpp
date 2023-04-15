#ifndef __2A_BOUND_H__
#define __2A_BOUND_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

  #include "Sphere.hpp"
  #include "Box.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Bound {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  Box    m_box;
  Sphere m_sphere;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void set(

    glm::mat4& model,
    glm::vec3& pos,
    glm::vec3& dim

  );

  // ctrash
  Bound(void) {};
  ~Bound(void) {};

  // getters
  inline Box& box(void) {
    return m_box;

  };

  inline Sphere& sphere(void) {
    return m_sphere;

  };

};};

// ---   *   ---   *   ---
// sugar

namespace Gaol {
  typedef std::vector<Bound> Bounds;

};

// ---   *   ---   *   ---

#endif // __2A_BOUND_H__

