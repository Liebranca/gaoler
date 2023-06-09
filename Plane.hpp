#ifndef __2A_PLANE_H__
#define __2A_PLANE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "bitter/ff/CRK.hpp"

  #include "Collision.hpp"
  #include "Line.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Plane {

public:

  VERSION   "v2.00.4";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  float     m_dom[3][2];

  vec3 m_centroid;
  vec3 m_normal;

  Points    m_points;
  Lines     m_edges;

  float     m_dir;

// ---   *   ---   *   ---
// guts

  // pre-calc low-high cords on
  // each axis for later checks
  void calc_indom(void);

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void set(
    vec3& a,
    vec3& b,
    vec3& c,
    vec3& d

  );

  // ctrash
  Plane(void) {};
  ~Plane(void) {};

  // invert facing
  inline void flip(void) {
    m_normal = -m_normal;
    m_dir    = -m_dir;

  };

  // other *possibly* intersects
  inline bool indom(Plane& other,int ax) {

    return

    (  other.m_dom[ax][0] <= m_dom[ax][0]
    && other.m_dom[ax][1] <= m_dom[ax][1] )

    ||

    (  other.m_dom[ax][0] <= m_dom[ax][1]
    && other.m_dom[ax][0] <= m_dom[ax][1] )

    ;

  };

  // point *possibly* intersects
  bool indom_point(vec3& p,int ax);
  bool indom_point(vec2& p,int ax);
  bool indom_point_xy(vec2& p);

  // checks point in plane
  inline float isect_point(vec3 p) {
    return glm::dot(m_normal,p)+m_dir;

  };

  // detect intersection
  Collision isect_plane(Plane& other);
  Collision isect_ray(Line& ray);

  // ^special case [*1]
  bool isect_edge(Plane& other);

  // getters
  inline vec3& normal(void) {
    return m_normal;

  };

  inline vec3& centroid(void) {
    return m_centroid;

  };

  inline Line& edge(int idex) {
    return m_edges[idex];

  };

  inline vec3& point(uint8_t idex) {
    return m_points[idex];

  };

  // write plane to drawable
  inline void to_mesh(CRK::Prim& me) {
    CRK::Tri t(m_points,m_normal);
    t.pack(me);

  };

};};

// ---   *   ---   *   ---
// sugar

namespace Gaol {
  typedef std::vector<Plane> Planes;

};

// ---   *   ---   *   ---

#endif // __2A_PLANE_H__
