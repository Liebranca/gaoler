#ifndef __2A_PLANE_H__
#define __2A_PLANE_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"
  #include "bitter/ff/CRK.hpp"

  #include "Collision.hpp"
  #include "Line.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Plane {

public:

  VERSION   "v2.00.2";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  float     m_dom[3][2];

  glm::vec3 m_centroid;
  glm::vec3 m_normal;

  Points    m_points;
  Lines     m_edges;

  float     m_dir;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void set(
    glm::vec3& a,
    glm::vec3& b,
    glm::vec3& c

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
  bool indom_point(glm::vec3& p,int ax);

  // checks point in plane
  inline float point_isect(glm::vec3 p) {
    return glm::dot(m_normal,p)+m_dir;

  };

  // detect intersection
  Collision isect_plane(Plane& other);
  Collision isect_ray(Line& ray);

  // ^special case [*1]
  bool isect_edge(Plane& other);

  // getters
  inline glm::vec3& normal(void) {
    return m_normal;

  };

  inline glm::vec3& centroid(void) {
    return m_centroid;

  };

  inline Line& edge(int idex) {
    return m_edges[idex];

  };

  inline glm::vec3& point(uint8_t idex) {
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
