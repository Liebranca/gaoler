#ifndef __2A_BOX_H__
#define __2A_BOX_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

  #include "Plane.hpp"

// ---   *   ---   *   ---
// lame fwd decl

namespace Gaol {
  class Sphere;

};

// ---   *   ---   *   ---
// info

namespace Gaol { class Box {

public:

  VERSION   "v2.00.6";
  AUTHOR    "IBN-3DILA";

  // see first note on src
  cx8 WINDING[4*6]={

    0,4,7,3, // bottom
    2,1,5,6, // top
    0,4,5,1, // front
    2,6,7,3, // back
    5,6,7,4, // right
    1,2,3,0  // left

  };

  // ^for less-confusing indexing
  enum {
    BOTTOM,TOP  ,
    FRONT ,BACK ,
    RIGHT ,LEFT

  };

  // lines across top and bottom planes
  enum {
    CBOTTOM_L,CBOTTOM_R,
    CTOP_L   ,CTOP_R

  };

// ---   *   ---   *   ---
// helpers

  struct GCHK {

    bool  hit;
    float height;

  };

// ---   *   ---   *   ---
// attrs

private:

  Planes m_planes;
  Lines  m_cross;

  Points m_points;
  vec3   m_origin={0,0,0};

  float  m_area[2];
  float  m_dim[3];

  float  m_lside;

  bool   m_prism=false;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void set(

    vec3  bot,

    float x,
    float y,
    float z

  );

  // ^alt
  void set_prism(

    vec3 nbl,
    vec3 ntl,
    vec3 ftl,
    vec3 fbl,

    vec3 nbr,
    vec3 ntr,
    vec3 ftr,
    vec3 fbr

  );

  // make planes from points
  void calc_planes(void);

  // get area for alt cstruc
  void calc_area(void);

  // ctrash
  Box(void) {};
  ~Box(void) {};

  // give self+offset
  Box project(vec3& dir);

  // ^a more exotic variation
  // projects self, extending
  // planes that face away from view
  Box project_view(
    vec3& eye,
    vec3& pos,
    vec3& up,

    float v_zfar,
    float v_hfar,
    float v_wfar

  );

  // give box-box is *possible*
  bool indom_box(Box& other);

  // give point-box is *possible*
  bool indom_point(vec3& p);

  // collision hit point inside box
  bool indom_col(Collision& col);

  // defd by Sphere
  int isect_sphere(Gaol::Sphere& s);

  // ray-box intersection
  Collision isect_ray(Line& ray);

  // bottom plane to plane intersection
  bool isect_bottom(Plane& plane,float fac);

  // ^give hit + landing height
  GCHK isect_surface(Box& other,float fac);

  // *approximate* line-box intersection
  Collision isect_line(Line& ray);

  // point-box intersection
  bool isect_point(vec3& p);

  // box-box intersection
  Collision isect_box(Box& other,vec3& dir);

  // these are naughty checks
  void nit_cage(Collisions& dst,Plane& plane);
  bool isect_cage(Plane& plane);

  // getters
  inline Planes& planes(void) {
    return m_planes;

  };

  inline vec3& origin(void) {
    return m_origin;

  };

  inline float& lside(void) {
    return m_lside;

  };

  // write shape to drawable format
  void to_mesh(CRK::Prim& me);

};};

// ---   *   ---   *   ---

#endif // __2A_BOX_H__
