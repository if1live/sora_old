﻿#pragma once
#include "parametric_surface.h"
#include "core/math_helper.h"

namespace sora {;

class Cone : public ParametricSurface {
public:
  Cone(float height, float radius) : m_height(height), m_radius(radius) {
    ParametricInterval interval = { Vec2i(20, 20), Vec2f(kTwoPi, 1), Vec2f(30, 20) };
    SetInterval(interval);
  }
  Vec3f Evaluate(const Vec2f& domain) const {
    float u = domain.x, v = domain.y;
    float x = m_radius * (1 - v) * cos(u);
    float y = m_height * (v - 0.5f);
    float z = m_radius * (1 - v) * -sin(u);
    return Vec3f(x, y, z);
  }
private:
  float m_height;
  float m_radius;
};

class Sphere : public ParametricSurface {
public:
  Sphere(float radius) : m_radius(radius) {
    ParametricInterval interval = { Vec2i(20, 20), Vec2f(kPi, kTwoPi), Vec2f(20, 35) };
    SetInterval(interval);
  }
  Vec3f Evaluate(const Vec2f& domain) const {
    float u = domain.x, v = domain.y;
    float x = m_radius * sin(u) * cos(v);
    float y = m_radius * cos(u);
    float z = m_radius * -sin(u) * sin(v);
    return Vec3f(x, y, z);
  }
private:
  float m_radius;
};

class Torus : public ParametricSurface {
public:
  Torus(float majorRadius, float minorRadius) :
      m_majorRadius(majorRadius),
        m_minorRadius(minorRadius) {
        ParametricInterval interval = { Vec2i(20, 20), Vec2f(kTwoPi, kTwoPi), Vec2f(40, 10) };
        SetInterval(interval);
      }
      Vec3f Evaluate(const Vec2f& domain) const {
        const float major = m_majorRadius;
        const float minor = m_minorRadius;
        float u = domain.x, v = domain.y;
        float x = (major + minor * cos(v)) * cos(u);
        float y = (major + minor * cos(v)) * sin(u);
        float z = minor * sin(v);
        return Vec3f(x, y, z);
      }
private:
  float m_majorRadius;
  float m_minorRadius;
};

class TrefoilKnot : public ParametricSurface {
public:
  TrefoilKnot(float scale) : m_scale(scale) {
    ParametricInterval interval = { Vec2i(60, 15), Vec2f(kTwoPi, kTwoPi), Vec2f(100, 8) };
    SetInterval(interval);
  }
  Vec3f Evaluate(const Vec2f& domain) const {
    const float a = 0.5f;
    const float b = 0.3f;
    const float c = 0.5f;
    const float d = 0.1f;
    float u = (kTwoPi - domain.x) * 2;
    float v = domain.y;

    float r = a + b * cos(1.5f * u);
    float x = r * cos(u);
    float y = r * sin(u);
    float z = c * sin(1.5f * u);

    Vec3f dv;
    dv.x = -1.5f * b * sin(1.5f * u) * cos(u) -
      (a + b * cos(1.5f * u)) * sin(u);
    dv.y = -1.5f * b * sin(1.5f * u) * sin(u) +
      (a + b * cos(1.5f * u)) * cos(u);
    dv.z = 1.5f * c * cos(1.5f * u);

    Vec3f q = dv.Normalize();
    Vec3f qvn = Vec3f(q.y, -q.x, 0).Normalize();
    Vec3f ww = q.Cross(qvn);

    Vec3f range;
    range.x = x + d * (qvn.x * cos(v) + ww.x * sin(v));
    range.y = y + d * (qvn.y * cos(v) + ww.y * sin(v));
    range.z = z + d * ww.z * sin(v);
    return range * m_scale;
  }
private:
  float m_scale;
};

class MobiusStrip : public ParametricSurface {
public:
  MobiusStrip(float scale) : m_scale(scale) {
    ParametricInterval interval = { Vec2i(40, 20), Vec2f(kTwoPi, kTwoPi), Vec2f(40, 15) };
    SetInterval(interval);
  }
  Vec3f Evaluate(const Vec2f& domain) const {
    float u = domain.x;
    float t = domain.y;
    float major = 1.25;
    float a = 0.125f;
    float b = 0.5f;
    float phi = u / 2;

    // General equation for an ellipse where phi is the angle
    // between the major axis and the X axis.
    float x = a * cos(t) * cos(phi) - b * sin(t) * sin(phi);
    float y = a * cos(t) * sin(phi) + b * sin(t) * cos(phi);

    // Sweep the ellipse along a circle, like a torus.
    Vec3f range;
    range.x = (major + x) * cos(u);
    range.y = (major + x) * sin(u);
    range.z = y;
    return range * m_scale;
  }
private:
  float m_scale;
};

class KleinBottle : public ParametricSurface {
public:
  KleinBottle(float scale) : m_scale(scale) {
    ParametricInterval interval = { Vec2i(20, 20), Vec2f(kTwoPi, kTwoPi), Vec2f(15, 50) };
    SetInterval(interval);
  }
  Vec3f Evaluate(const Vec2f& domain) const {
    float v = 1 - domain.x;
    float u = domain.y;

    float x0 = 3 * cos(u) * (1 + sin(u)) +
      (2 * (1 - cos(u) / 2)) * cos(u) * cos(v);

    float y0  = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);

    float x1 = 3 * cos(u) * (1 + sin(u)) +
      (2 * (1 - cos(u) / 2)) * cos(v + kPi);

    float y1 = 8 * sin(u);

    Vec3f range;
    range.x = u < kPi ? x0 : x1;
    range.y = u < kPi ? -y0 : -y1;
    range.z = (-2 * (1 - cos(u) / 2)) * sin(v);
    return range * m_scale;
  }
  bool InvertNormal(const Vec2f& domain) const {
    return domain.y > 3 * kPi / 2;
  }
private:
  float m_scale;
};

}