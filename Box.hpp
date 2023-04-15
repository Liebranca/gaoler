#ifndef __2A_BOX_H__
#define __2A_BOX_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

  #include "Plane.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Box {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

  // see first note on src
  cx8 WINDING[3*6]={

    0,4,7, // bottom
    2,1,5, // top
    0,4,5, // front
    2,6,7, // back
    5,6,7, // right
    1,2,3, // left

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

  Planes    m_planes;
  Lines     m_cross;

  glm::vec3 m_points[8];
  glm::vec3 m_origin={0,0,0};

  float     m_area[2];
  float     m_dim[3];

  float     m_lside;

  bool      m_prism=false;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void set(

    glm::vec3 bot,

    float     x,
    float     y,
    float     z

  );

  // ^alt
  void set_prism(

    glm::vec3 nbl,
    glm::vec3 ntl,
    glm::vec3 ftl,
    glm::vec3 fbl,

    glm::vec3 nbr,
    glm::vec3 ntr,
    glm::vec3 ftr,
    glm::vec3 fbr

  );

  // make planes from points
  void calc_planes(void);

  // get area for alt cstruc
  void calc_area(void);

  // ctrash
  Box(void) {};
  ~Box(void) {};

  // give self+offset
  Box project(glm::vec3& dir);

  // give box-box is *possible*
  bool indom_box(Box& other);

  // give point-box is *possible*
  bool indom_point(glm::vec3& p);

  // collision hit point inside box
  bool indom_col(Collision& col);

  // ray-box intersection
  Collision isect_ray(Line& ray);

  // bottom plane to plane intersection
  bool isect_bottom(Plane& plane,float fac);

  // ^give hit + landing height
  GCHK isect_surface(Box& other,float fac);

  // *approximate* line-box intersection
  bool isect_line(Line& ray);

  // point-box intersection
  bool isect_point(glm::vec3& p);

  // box-box intersection
  Collision isect_box(Box& other,glm::vec3& dir);

  // these are naughty checks
  void nit_cage(Collisions& dst,Plane& plane);
  bool isect_cage(Plane& plane);

  // getters
  inline Planes& planes(void) {
    return m_planes;

  };

  inline glm::vec3& origin(void) {
    return m_origin;

  };

  inline float& lside(void) {
    return m_lside;

  };

};};

// ---   *   ---   *   ---

#endif // __2A_BOX_H__
