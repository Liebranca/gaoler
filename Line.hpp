#ifndef __2A_LINE_H__
#define __2A_LINE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"

// ---   *   ---   *   ---
// sugar

namespace Gaol {
  typedef std::vector<vec3> Points;

};

// ---   *   ---   *   ---
// info

namespace Gaol { class Line {

public:

  VERSION   "v2.00.3";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  Points m_points;

  vec3   m_normal;
  vec3   m_centroid;

  float  m_length;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  inline void set(vec3& a,vec3& b) {

    m_normal    = b-a;
    m_centroid  = (a+b)*0.5f;
    m_length    = glm::length(m_normal);
    m_normal    = glm::normalize(m_normal);

    m_points.resize(2);

    m_points[0] = a;
    m_points[1] = b;

  };

  // ctrash
  Line(void) {};
  ~Line(void) {};

  // get aligned point
  inline vec3 point_along(float s) {
    return m_points[0] + ((m_normal*m_length) * s);

  };

  // ^cast point accto distance to another
  // give end of line if distance is OOB
  inline vec3 point_from_dist(float s) {

    return (s >= m_length)
      ? m_points[1]
      : this->point_along(s/m_length)
      ;

  };

  // get intersection point
  inline vec3 isect(float t) {
    return m_points[0] + (t*m_normal);

  };

  // get sum of lengths
  inline float plen(Line& other) {
    return m_length+other.m_length;

  };

  // getters
  inline vec3& point(uint8_t idex) {
    return m_points[idex];

  };

  inline vec3& normal(void) {
    return m_normal;

  };

  inline vec3& centroid(void) {
    return m_centroid;

  };

  inline float length(void) {
    return m_length;

  };

};};

// ---   *   ---   *   ---
// sugar

namespace Gaol {
  typedef std::vector<Line> Lines;

};

// ---   *   ---   *   ---

#endif // __2A_LINE_H__
