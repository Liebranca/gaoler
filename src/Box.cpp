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
//    0: nbl,nbr,fbr,fbl | bottom
//    1: ftl,ntl,ntr,ftr | top
//    2: nbl,nbr,ntr,ntl | front
//    3: ftl,ftr,fbr,fbl | back
//    4: ntr,ftr,fbr,nbr | right
//    5: ntl,ftl,fbl,nbl | left
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

  m_points.resize(8);

  // nbl, ntl, ftl, fbl
  m_points[0]={bot.x-x,bot.y+0,bot.z+z};
  m_points[1]={bot.x-x,bot.y+y,bot.z+z};
  m_points[2]={bot.x-x,bot.y+y,bot.z-z};
  m_points[3]={bot.x-x,bot.y+0,bot.z-z};

  // nbr, ntr, ftr, fbr
  m_points[4]={bot.x+x,bot.y+0,bot.z+z};
  m_points[5]={bot.x+x,bot.y+y,bot.z+z};
  m_points[6]={bot.x+x,bot.y+y,bot.z-z};
  m_points[7]={bot.x+x,bot.y+0,bot.z-z};

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

  m_points.resize(8);

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
  for(uint8_t i=0,j=0;i<6;i++,j+=4) {

    m_planes[i].set(
      m_points[WINDING[j+0]],
      m_points[WINDING[j+1]],
      m_points[WINDING[j+2]],
      m_points[WINDING[j+3]]

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

  m_cross[CBOTTOM_L].set(
    b.edge(1).point(1),
    b.point(3)

  );

  m_cross[CBOTTOM_R].set(
    b.point(3),
    b.edge(0).point(0)

  );

  m_cross[CTOP_L].set(
    t.edge(1).point(1),
    t.point(3)

  );

  m_cross[CTOP_R].set(
    t.point(3),
    t.edge(0).point(0)

  );

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
// ^a more exotic variation
// projects self, extending
// planes that face away from view

Box Box::project_view(

  glm::vec3& eye,
  glm::vec3& pos,
  glm::vec3& up,

  float      v_zfar,
  float      v_hfar,
  float      v_wfar

) {

  Box out;

  // get horizontal axis
  glm::vec3 haxis=glm::normalize(
    glm::cross(up,eye)

  );

  glm::vec3 ntl(
    m_origin.x,
    m_planes[TOP].point(3).y,
    m_origin.z

  );

  glm::vec3 ntr(
    m_origin.x,
    m_planes[TOP].point(3).y,
    m_origin.z

  );

  glm::vec3 castdirn  = {0,0,0};
  uint8_t   num_dirns = 0;

// ---   *   ---   *   ---
// walk planes
// top and bottom skipped

  for(uint8_t i=2;i<6;i++) {

    auto&     plane = m_planes[i];
    glm::vec3 vto   = pos-plane.centroid();

    float     d     = glm::dot(vto,plane.normal());

    // plane faces away from view
    if(d > 0) {

      castdirn+=plane.normal();
      num_dirns++;

      glm::vec3 points[4]={

        plane.edge(0).point(0),
        plane.edge(0).point(1),
        plane.edge(1).point(1),

        plane.point(3)

      };

      // build reference for
      // near-plane correction
      for(auto& p : points) {

        glm::vec3 ref={
          glm::dot(p,haxis),
          glm::dot(p,up),
          glm::dot(p,eye)

        };

        glm::vec3 lolr={
          glm::dot(ntr,haxis),
          glm::dot(ntr,up),
          glm::dot(ntr,eye)

        };

        glm::vec3 loll={
          glm::dot(ntl,haxis),
          glm::dot(ntl,up),
          glm::dot(ntl,eye)

        };

        // ^correct near top from reference
        if(ref.x < lolr.x) {
          ntr.x=p.x;
          ntr.z=p.z;

        };

        if(ref.x > loll.x) {
          ntl.x=p.x;
          ntl.z=p.z;

        };

      };

    };

  };

  // adjust direction the resulting
  // prism will project towards
  if(num_dirns) {
    castdirn /= num_dirns;

  };

  // ^get final direction
  glm::vec3 dirn=normalize(
    -castdirn + ((m_origin) - pos)

  );

  // ^get offsets
  glm::vec3 fc    = dirn  * v_zfar;
  glm::vec3 upfar = up    * (v_hfar/16);
  glm::vec3 hfar  = haxis * (v_wfar/16);

// ---   *   ---   *   ---
// calc remaining points

  glm::vec3 nbl(
    ntl.x,
    m_planes[BOTTOM].point(3).y,
    ntl.z

  );

  glm::vec3 nbr(
    ntr.x,
    m_planes[BOTTOM].point(3).y,
    ntr.z

  );

  // project near plane towards direction
  glm::vec3 ftl = ntl + fc + upfar - hfar;
  glm::vec3 ftr = ntr + fc + upfar + hfar;
  glm::vec3 fbl = nbl + fc - upfar - hfar;
  glm::vec3 fbr = nbr + fc - upfar + hfar;

  // ^pass points to cstruc
  // this gives out a prism extending
  // away from view
  out.set_prism(
    nbl,ntl,ftl,fbl,
    nbr,ntr,ftr,fbr

  );

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

    b.edge(0).point(0),
    b.edge(0).point(1),
    b.edge(1).point(1),

    b.point(3)

  };

  // ^walk and shoot ray downwards
  for(auto& point : pattern) {

    glm::vec3 end=point-vel;
    r.set(point,end);

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

  int   limit = int(ray.length());
  float mid   = 0.5f;

  // cap attempts accto len of line
  for(int i=0;i<limit;i++) {

    // check mid point inside box
    glm::vec3 p=ray.point_along(mid);
    if(this->isect_point(p)) {
      return true;

    };

    // ^else find next middle point
    // relative to current position
    glm::vec3 vto = m_origin-p;
    float     d   = glm::dot(vto,ray.normal());

    // go backwards along line if
    // vto points towards normal
    // else go fowards
    mid += (d<0)
      ? -mid/2
      :  mid/2
      ;

  };

  return false;

};

// ---   *   ---   *   ---
// point-box intersection

bool Box::isect_point(glm::vec3& p) {

  for(auto& plane : m_planes) {

    float d=plane.isect_point(p);
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

    m_cross[CBOTTOM_L],
    m_cross[CBOTTOM_R],

    m_cross[CTOP_L],
    m_cross[CTOP_R],

    m_planes[FRONT].edge(1),
    m_planes[BACK].edge(1),
    m_planes[RIGHT].edge(1),
    m_planes[LEFT].edge(1),

    m_planes[TOP].edge(0),
    m_planes[TOP].edge(1),

    m_planes[BOTTOM].edge(0),
    m_planes[BOTTOM].edge(1)

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

void Box::to_mesh(CRK::Prim& me) {

  for(auto& plane : m_planes) {
    plane.to_mesh(me);

  };

};

// ---   *   ---   *   ---

}; // namespace Gaol

// ---   *   ---   *   ---
