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

  float width,
  float height,

  float fov,
  float near,
  float far

) {

  m_znear = near;
  m_zfar  = far;
  fov     = 2 * tan(fov*0.5f);

  float aspect=width/height;


  m_hnear  = fov * m_znear;
  m_hfar   = fov * m_zfar;
  m_wnear  = m_hnear * aspect;
  m_wfar   = m_hfar  * aspect;

};

// ---   *   ---   *   ---
// recalculate bounds

void Frustum::calc_box(
  vec3& pos,
  vec3& fwd,
  vec3& up

) {

  // horizontal axis
  vec3 haxis=glm::normalize(
    glm::cross(fwd,up)

  );

  // far offsets
  vec3 fc     = pos + fwd * m_zfar;
  vec3 wfar   = haxis * (m_wfar/2);
  vec3 upfar  = up    * (m_hfar/2);

  // far points
  vec3 ftl    = fc + upfar - wfar;
  vec3 ftr    = fc + upfar + wfar;
  vec3 fbl    = fc - upfar - wfar;
  vec3 fbr    = fc - upfar + wfar;

  // near offsets
  vec3 nc     = pos + fwd * m_znear;
  vec3 wnear  = haxis * (m_wnear/2);
  vec3 upnear = up    * (m_hnear/2);

  // near points
  vec3 ntl    = nc + upnear - wnear;
  vec3 ntr    = nc + upnear + wnear;
  vec3 nbl    = nc - upnear - wnear;
  vec3 nbr    = nc - upnear + wnear;

  // ^make prism from points
  m_box.set_prism(
    nbl,ntl,ftl,fbl,
    nbr,ntr,ftr,fbr

  );

};

// ---   *   ---   *   ---
// frustum-box intersection

bool Frustum::isect_box(Box& b) {

  auto& planes=m_box.planes();

  for(auto& plane : planes) {
    if(b.isect_cage(plane)) {
      return true;

    };

  };

  return false;

};

// ---   *   ---   *   ---
// checks sphere, then box

int Frustum::isect_bound(Bound& b) {

  int out=b.sphere().isect_box(m_box);

  if(out < 0) {

    out=this->isect_box(b.box());

  };

  return out;

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
