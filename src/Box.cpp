// ---   *   ---   *   ---
// BOX
// Cuboids galore
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// NOTE:
//
//  N for NEAR   | F for FAR
//  B for BOTTOM | T for TOP
//  L for left   | R for RIGHT
//
//
//  points are ordered so:
//
//    0: nbl | 4: nbr
//    1: ntl | 5: ntr
//    2: ftl | 6: ftr
//    3: fbl | 7: fbr
//
//
//  planes are arranged as such:
//
//    0: nbl,nbr,fbr | bottom
//    1: ftl,ntl,ntr | top
//    2: nbl,nbr,ntr | front
//    3: ftl,ftr,fbr | back
//    4: ntr,ftr,fbr | right
//    5: ntl,ftl,fbl | right
//
//
// remember the winding order
// or youll be pretty much screwed

// ---   *   ---   *   ---
// deps

  #include "Consts.hpp"
  #include "Box.hpp"

// ---   *   ---   *   ---

namespace Gaol {

// ---   *   ---   *   ---
// cstruc

void Box::set(

  glm::vec3 bot,

  float     x,
  float     y,
  float     z

) {

  // nbl, ntl, ftl, fbl
  m_points[0]={bot.x-x,bot.y+0,bot.z-z};
  m_points[1]={bot.x-x,bot.y+y,bot.z-z};
  m_points[2]={bot.x-x,bot.y+y,bot.z+z};
  m_points[3]={bot.x-x,bot.y+0,bot.z+z};

  // nbr, ntr, ftr, fbr
  m_points[4]={bot.x+x,bot.y+0,bot.z-z};
  m_points[5]={bot.x+x,bot.y+y,bot.z-z};
  m_points[6]={bot.x+x,bot.y+y,bot.z+z};
  m_points[7]={bot.x+x,bot.y+0,bot.z+z};

  this->calc_planes();

  // save area
  m_area[0]=(x<z) ? z*2 : x*2;
  m_area[1]=y;

  // length of longest edge
  m_lside=(m_area[0] > m_area[1])
    ? m_area[0]
    : m_area[1]
    ;

  // save cstruc args
  m_dim[0] = x;
  m_dim[1] = y;
  m_dim[2] = z;

  m_prism  = false;

};

// ---   *   ---   *   ---
// ^alternate version

void Box::set_prism(

  glm::vec3 nbl,
  glm::vec3 ntl,
  glm::vec3 ftl,
  glm::vec3 fbl,

  glm::vec3 nbr,
  glm::vec3 ntr,
  glm::vec3 ftr,
  glm::vec3 fbr

) {

  // ^save points
  m_points[0]=nbl;m_points[1]=ntl;
  m_points[2]=ftl;m_points[3]=fbl;
  m_points[4]=nbr;m_points[5]=ntr;
  m_points[6]=ftr;m_points[7]=fbr;

  this->calc_planes();
  this->calc_area();

  m_prism=true;

};

// ---   *   ---   *   ---
// make planes from points

void Box::calc_planes(void) {

  m_planes.resize(6);

  // follow winding pattern decld in hed
  for(uint8_t i=0,j=0;i<6;i++,j+=3) {

    m_planes[i].set(
      m_points[WINDING[j+0]],
      m_points[WINDING[j+1]],
      m_points[WINDING[j+2]]

    );

  };

  // ^correct the automatic normal calcs
  m_planes[BOTTOM].flip();
  m_planes[RIGHT].flip();

  // calc origin
  for(uint8_t i=0;i<8;i++) {
    m_origin+=m_points[i];

  };

  m_origin*=0.125f;

  // get lines across top and bottom
  m_cross.resize(4);

  auto& b=m_planes[BOTTOM];
  auto& t=m_planes[TOP];

  m_cross[CBOTTOM_L].set(b.edge(1).p2(),b.p4());
  m_cross[CBOTTOM_R].set(b.p4(),b.edge(0).p1());

  m_cross[CTOP_L].set(t.edge(1).p2(),t.p4());
  m_cross[CTOP_R].set(t.p4(),t.edge(0).p1());

};

// ---   *   ---   *   ---
// get area for alt cstruc

void Box::calc_area(void) {

  auto x=m_planes[BOTTOM].edge(0).length();
  auto z=m_planes[BOTTOM].edge(1).length();

  auto y=m_planes[FRONT].edge(1).length();

  // save area
  m_area[0]=(x<z) ? z : x;
  m_area[1]=y;

  // length of longest edge
  m_lside=(m_area[0] > m_area[1])
    ? m_area[0]
    : m_area[1]
    ;

  // save cstruc args
  m_dim[0] = x/2;
  m_dim[1] = y/2;
  m_dim[2] = z/2;

};

// ---   *   ---   *   ---
// give self+offset

Box Box::project(glm::vec3& dir) {

  Box out;

  // cuboid projection
  if(! m_prism) {
    glm::vec3 bot=m_planes[BOTTOM].centroid()+dir;
    out.set(bot,m_dim[0],m_dim[1],m_dim[2]);

  // trapezoid prism (aka frustum)
  } else {
    out.set_prism(
      m_points[0]+dir,m_points[1]+dir,
      m_points[2]+dir,m_points[3]+dir,
      m_points[4]+dir,m_points[5]+dir,
      m_points[6]+dir,m_points[7]+dir

    );

  };

  return out;

};

// ---   *   ---   *   ---
// give box-box is *possible*

bool Box::indom_box(Box& other) {

  auto& a=m_planes[BOTTOM];
  auto& b=other.m_planes[BOTTOM];

  return a.indom(b,0) && a.indom(b,2);

};

// ---   *   ---   *   ---
// give point-box is *possible*

bool Box::indom_point(glm::vec3& p) {

  auto&  a=m_planes[BOTTOM];

  return
       a.indom_point(p,0)
    && a.indom_point(p,1)
    ;

};

// ---   *   ---   *   ---
// collision hit point inside box

bool Box::indom_col(Collision& col) {

  bool out=(col.hit())
    ? this->isect_point(col.point())
    : false
    ;

  return out;

};

// ---   *   ---   *   ---
// ray-box intersection

Collision Box::isect_ray(Line& ray) {

  Collision test,out;

  // skip bottom and top planes
  for(uint8_t i=2;i<6;i++) {

    test=m_planes[i].isect_ray(ray);

    // ray *apparently* hits
    if(test.hit()) {
      out=test;
      break;

    };

  };

  // ^double check
  if(out.hit()) {

    float dist=glm::distance(
      out.point(),
      ray.centroid()

    );

    float wall=ray.length() * 0.5f;

    // false positive!
    if(dist > wall) {
      out.fake();

    };

  };

  return out;

};

// ---   *   ---   *   ---
// bottom plane to plane intersection

bool Box::isect_bottom(
  Plane& plane,
  float  fac

) {

  Line  r;

  auto& b=m_planes[BOTTOM];
  glm::vec3 vel(0,fac,0);

  // get all four corners
  std::vector<glm::vec3> pattern {

    b.edge(0).p1(),
    b.edge(0).p2(),
    b.edge(1).p2(),

    b.p4()

  };

  // ^walk and shoot ray downwards
  for(auto& point : pattern) {

    r.set(point,point-vel);

    if(plane.isect_ray(r).hit()) {
      return true;

    };

  };

  return false;

};

// ---   *   ---   *   ---
// ^give hit + landing height

Box::GCHK Box::isect_surface(
  Box&  other,
  float fac

) {

  GCHK out={false,0.0f};

  // collision not possible; exit
  if(! this->indom_box(other)) {
    return out;

  };

  // own bottom plane
  auto& b=m_planes[BOTTOM];

  // ^for each of other's
  for(auto& a : other.m_planes) {

    // b *approaches* a
    float d=glm::dot(b.normal(),-a.normal());
    if(d < Limit::NORMAL) {

      // collision confirmed
      if(this->isect_bottom(a,fac)) {
        out.hit    = true;
        out.height = a.centroid().y;

        break;

      };

    };

  };

  return out;

};

// ---   *   ---   *   ---
// *approximate* line-box intersection

bool Box::isect_line(Line& ray) {

  int limit=int(ray.length());

  for(int i=0;i<limit;i++) {
    glm::vec3 p=ray.point_along(i*0.1f);

    if(this->isect_point(p)) {
      return true;

    };

  };

  return false;

};

// ---   *   ---   *   ---
// point-box intersection

bool Box::isect_point(glm::vec3& p) {

  for(auto& plane : m_planes) {

    float d=plane.point_isect(p);

    if(d > Limit::NORMAL) {
      return false;

    };

  };

  return true;

};

// ---   *   ---   *   ---
// box-box intersection

Collision Box::isect_box(
  Box&       other,
  glm::vec3& dir

) {

  Collision out;

  // collision not possible; exit
  if(! this->indom_box(other)) {
    return out;

  };

  // isect check each
  for(auto& plane : other.m_planes) {

    float d=glm::dot(dir,-plane.normal());
    if(
       d > Limit::NORMAL
    && this->isect_cage(plane)

    ) {

      // TODO: get colpoint
      glm::vec3 p={0,0,0};
      out.set(plane.normal(),p);

      break;

    };

  };

  return out;

};

// ---   *   ---   *   ---
// shoot rays around self

bool Box::isect_cage(Plane& plane) {

  Lines cage {

    m_planes[FRONT].edge(1),
    m_planes[BACK].edge(1),
    m_planes[RIGHT].edge(1),
    m_planes[LEFT].edge(1),

    m_planes[BOTTOM].edge(0),
    m_planes[BOTTOM].edge(1),
    m_cross[CBOTTOM_L],
    m_cross[CBOTTOM_R],

    m_planes[TOP].edge(0),
    m_planes[TOP].edge(1),
    m_cross[CTOP_L],
    m_cross[CTOP_R]

  };

  // ^shoot one ray per edge
  for(auto& ray : cage) {

    // stop on collision
    auto col=plane.isect_ray(ray);
    if(this->indom_col(col)) {
      return true;

    };

  };

  return false;

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
