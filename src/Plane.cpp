// ---   *   ---   *   ---
// PLANE
// Four points from three
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <cmath>

  #include "Consts.hpp"
  #include "Plane.hpp"

// ---   *   ---   *   ---

namespace Gaol {

// ---   *   ---   *   ---
// cstruc

void Plane::set(
  vec3& a,
  vec3& b,
  vec3& c,
  vec3& d

) {

  m_centroid  = (a+c)*0.5f;
  m_normal    = glm::normalize(
    glm::cross(b-a,c-a)

  );

  m_dir       = glm::dot(-m_centroid,m_normal);

  m_points.resize(4);

  m_points[0] = a;
  m_points[1] = b;
  m_points[2] = c;
  m_points[3] = d;

  m_edges.resize(2);

  m_edges[0].set(a,b);
  m_edges[1].set(b,c);

  // pre-calc for indom
  auto& d0=m_edges[0].point(0);
  auto& d1=m_edges[1].point(1);

  // min && max of X
  m_dom[0][0]=(d0.x < d1.x) ? d0.x : d1.x;
  m_dom[0][1]=(d0.x > d1.x) ? d0.x : d1.x;

  // min && max of Y
  m_dom[1][0]=(d0.y < d1.y) ? d0.y : d1.y;
  m_dom[1][1]=(d0.y > d1.y) ? d0.y : d1.y;

  // min && max of Z
  m_dom[2][0]=(d0.z < d1.z) ? d0.z : d1.z;
  m_dom[2][1]=(d0.z > d1.z) ? d0.z : d1.z;

};

// ---   *   ---   *   ---
// point *possibly* intersects

bool Plane::indom_point(vec3& p,int ax) {

  float xm=m_dom[ax][0] - Limit::MARGIN;
  float xp=m_dom[ax][1] + Limit::MARGIN;

  return (xm <= p[ax] && p[ax] <= xp);

};

// ---   *   ---   *   ---
// plane-plane intersection

Collision Plane::isect_plane(Plane& other) {

  Collision out;

  vec3 inormal=glm::cross(
    m_normal,other.m_normal

  );

  float det=pow(glm::length(inormal),2);

  if(det > 0.001f) {
    vec3 a=glm::cross(inormal,other.m_normal);
    vec3 b=glm::cross(m_normal,inormal);

    vec3 ipoint=(
      (a*m_dir)
    + (b*other.m_dir)

    ) / det;

    out.set(other.m_normal,ipoint);

  } else {
    out.set(other.m_normal,other.m_centroid);

  };

  return out;

};

// ---   *   ---   *   ---
// ray intersects plane
//
// i am proud of this f
// i am also not sure why it works ;>

Collision Plane::isect_ray(Line& ray) {

  Collision out;

  float denom=glm::dot(m_normal,ray.normal());

  // no hit
  if(abs(denom) <= 1e-4f) {
    return out;

  };

  float dot = glm::dot(m_normal,ray.point(0));
  float t   = -(dot + m_dir) / denom;

  // no hit
  if (t <= 1e-4f) {
    return out;

  };

  // hit
  vec3 p=ray.isect(t);
  out.set(ray.normal(),p);

  return out;

};

// ---   *   ---   *   ---
// plane-plane *edge* intersection
//
// [*2]: rather than a collision point,
//       this one tells whether the distance
//       between the centroids is less than
//       the sum of the edges
//
//       ie, it *approximates* the isect
//       calculation: thus, no point

bool Plane::isect_edge(Plane& other) {

  float dx = fabs(m_centroid.x-other.m_centroid.x);
  float dy = fabs(m_centroid.y-other.m_centroid.y);
  float dz = fabs(m_centroid.z-other.m_centroid.z);

  float l1 = m_edges[0].plen(other.m_edges[0]);
  float l2 = m_edges[1].plen(other.m_edges[1]);

  bool  b1 = dx+dz < l1;
  bool  b2 = dy    < l2;

  return b1 && b2;

};

// ---   *   ---   *   ---

}; // namespace Gaol
