// ---   *   ---   *   ---
// FRUSTUM
// Can't C me
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "Frustum.hpp"

// ---   *   ---   *   ---

namespace Gaol {

// ---   *   ---   *   ---
// cstruc

void Frustum::set(

  float height,
  float width,

  float fov,
  float near,
  float far

) {

  m_znear = near;
  m_zfar  = far;

  float aspect=width/height;

  m_hnear  = 2 * tan(fov / 2) * m_znear;
  m_hfar   = 2 * tan(fov / 2) * m_zfar;
  m_wnear  = m_hnear * aspect;
  m_wfar   = m_hfar  * aspect;

};

// ---   *   ---   *   ---
// recalculate bounds

void Frustum::calc_box(
  glm::vec3& pos,
  glm::vec3& fwd,
  glm::vec3& up

) {

  // horizontal axis
  glm::vec3 haxis=glm::normalize(
    glm::cross(fwd,up)

  );

  // far offsets
  glm::vec3 fc     = pos + fwd * m_zfar;
  glm::vec3 hfar   = haxis * (m_wfar/2);
  glm::vec3 upfar  = up    * (m_hfar/2);

  // far points
  glm::vec3 ftl    = fc + upfar + hfar;
  glm::vec3 ftr    = fc + upfar - hfar;
  glm::vec3 fbl    = fc - upfar + hfar;
  glm::vec3 fbr    = fc - upfar - hfar;

  // near offsets
  glm::vec3 nc     = pos + fwd * m_znear;
  glm::vec3 hnear  = haxis * (m_wnear/2);
  glm::vec3 upnear = up    * (m_hnear/2);

  // near points
  glm::vec3 ntl    = nc + upnear + hnear;
  glm::vec3 ntr    = nc + upnear - hnear;
  glm::vec3 nbl    = nc - upnear + hnear;
  glm::vec3 nbr    = nc - upnear - hnear;

  // ^make prism from points
  m_box.set_prism(
    nbl,ntl,ftl,fbl,
    nbr,ntr,ftr,fbr

  );

};

// ---   *   ---   *   ---
// frustum-box intersection

bool Frustum::isect_box(Box& box) {

  auto& planes=m_box.planes();
  for(auto& plane : planes) {
    if(m_box.isect_cage(plane)) {
      return true;

    };

  };

  return false;

};

// ---   *   ---   *   ---
// checks sphere, then box

int Frustum::isect_bound(Bound& b) {

  int out=b.sphere().isect_box(m_box);
  if(out<0) {
    out=this->isect_box(b.box());

  };

  return out;

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
