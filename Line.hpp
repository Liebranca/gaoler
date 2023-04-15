#ifndef __2A_LINE_H__
#define __2A_LINE_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

namespace Gaol { class Line {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  float     m_length;

  glm::vec3 m_p1;
  glm::vec3 m_p2;
  glm::vec3 m_normal;
  glm::vec3 m_centroid;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  inline void set(glm::vec3 a,glm::vec3 b) {
    m_normal   = b-a;
    m_centroid = (a+b)*0.5f;
    m_length   = glm::length(m_normal);
    m_normal   = glm::normalize(m_normal);

  };

  // ctrash
  Line(void) {};
  ~Line(void) {};

  // get aligned point
  inline glm::vec3 point_along(float s) {
    return m_p1 + ((m_normal*m_length) * s);

  };

  // get intersection point
  inline glm::vec3 isect(float t) {
    return m_p1+(t*m_normal);

  };

  // get sum of lengths
  inline float plen(Line& other) {
    return m_length+other.m_length;

  };

  // getters
  inline glm::vec3& p1(void) {
    return m_p1;

  };

  inline glm::vec3& p2(void) {
    return m_p2;

  };

  inline glm::vec3& normal(void) {
    return m_normal;

  };

  inline glm::vec3& centroid(void) {
    return m_centroid;

  };

  inline float length(void) {
    return m_length;

  };

};};

// ---   *   ---   *   ---
// sugar

namespace Gaol {
  typedef std::vector<glm::vec3> Points;
  typedef std::vector<Line> Lines;

};

// ---   *   ---   *   ---

#endif // __2A_LINE_H__
