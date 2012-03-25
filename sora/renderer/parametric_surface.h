#pragma once

#include <vector>
#include "core/vector.h"

#include "vertex.h"

namespace sora {;

struct ISurface {
  virtual int GetVertexCount() const = 0;
  virtual int GetLineIndexCount() const = 0;
  virtual int GetTriangleIndexCount() const = 0;
  virtual void GenerateVertices(std::vector<Vertex> &vertices) const = 0;
  virtual void GenerateLineIndices(std::vector<unsigned short>& indices) const = 0;
  virtual void GenerateTriangleIndices(std::vector<unsigned short>& indices) const = 0;
  virtual ~ISurface() {}
};


struct ParametricInterval {
  Vec2i divisions;
  Vec2f upper_bound;
  Vec2f texture_count;
};

class ParametricSurface : public ISurface {
public:
  int GetVertexCount() const;
  int GetLineIndexCount() const;
  int GetTriangleIndexCount() const;
  //position, normal, texcoord 전부 함유
  void GenerateVertices(std::vector<Vertex> &vertices) const;
  void GenerateLineIndices(std::vector<unsigned short>& indices) const;
  void GenerateTriangleIndices(std::vector<unsigned short>& indices) const;

protected:
  void SetInterval(const ParametricInterval& interval);
  virtual Vec3f Evaluate(const Vec2f& domain) const = 0;
  virtual bool InvertNormal(const Vec2f& domain) const { return false; }

private:
  //필요에 따라서 normal만 있는거, texcoord만 있는거 같은식으로 구분하면
  //복잡도가 커져서 그냥 이거 안쓰고 Vertex를 쓰는거로 수정햇다
  void GenerateVertices(std::vector<float>& vertices, unsigned char flags) const;
  Vec2f ComputeDomain(float i, float j) const;
  Vec2i m_slices;
  Vec2i m_divisions;
  Vec2f m_upperBound;
  Vec2f m_textureCount;
};

}