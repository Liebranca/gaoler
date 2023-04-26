// ---   *   ---   *   ---
// BOUND
// A sphere from a box
// with xforms applied
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "Consts.hpp"
  #include "Bound.hpp"

// ---   *   ---   *   ---

namespace Gaol {

// ---   *   ---   *   ---
// cstruc

void Bound::set(

  glm::mat4& model,

  glm::vec3& pos,
  glm::vec3& dim

) {

  // transform dimentions
  glm::vec4 ndim(dim.x,dim.y,dim.z,1);
  glm::vec3 wdim=glm::vec3(model * ndim);

  // ^regenerate shape from deformed
  m_box.set(pos,wdim.x,wdim.y,wdim.z);
  m_sphere.set(
    m_box.origin(),
    m_box.lside()+Limit::MARGIN

  );

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
