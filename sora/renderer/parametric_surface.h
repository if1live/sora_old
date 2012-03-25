#pragma once

#include <vector>
#include "core/vector.h"

namespace sora {;

enum VertexFlags {
    kVertexFlagsNormals = 1 << 0,
    kVertexFlagsTexCoords = 1 << 1,
};

struct ISurface {
  virtual int GetVertexCount() const = 0;
  virtual int GetLineIndexCount() const = 0;
  virtual int GetTriangleIndexCount() const = 0;
  virtual void GenerateVertices(std::vector<float>& vertices,
    unsigned char flags = 0) const = 0;
  virtual void GenerateLineIndices(std::vector<unsigned short>& indices) const = 0;
  virtual void GenerateTriangleIndices(std::vector<unsigned short>& indices) const = 0;
  virtual ~ISurface() {}
};


struct ParametricInterval {
  Vec2i Divisions;
  Vec2f UpperBound;
  Vec2f TextureCount;
};

class ParametricSurface : public ISurface {
public:
  int GetVertexCount() const;
  int GetLineIndexCount() const;
  int GetTriangleIndexCount() const;
  void GenerateVertices(std::vector<float>& vertices, unsigned char flags) const;
  void GenerateLineIndices(std::vector<unsigned short>& indices) const;
  void GenerateTriangleIndices(std::vector<unsigned short>& indices) const;
protected:
  void SetInterval(const ParametricInterval& interval);
  virtual Vec3f Evaluate(const Vec2f& domain) const = 0;
  virtual bool InvertNormal(const Vec2f& domain) const { return false; }
private:
  Vec2f ComputeDomain(float i, float j) const;
  Vec2i m_slices;
  Vec2i m_divisions;
  Vec2f m_upperBound;
  Vec2f m_textureCount;
};

}