#pragma once

#include <vector>


#include "renderer/vertex.h"
#include "renderer/mesh.h"

namespace sora {;

struct ISurface {
  virtual int GetVertexCount() const = 0;
  virtual int GetLineIndexCount() const = 0;
  virtual int GetTriangleIndexCount() const = 0;
  virtual void GenerateVertices(std::vector<Vertex> &vertices) const = 0;
  virtual void GenerateVertices(std::vector<TangentVertex> &vertices) const = 0;
  virtual void GenerateLineIndices(std::vector<unsigned short>& indices) const = 0;
  virtual void GenerateTriangleIndices(std::vector<unsigned short>& indices) const = 0;
  virtual ~ISurface() {}

  template<typename VertexType>
  MeshBufferObject CreateSolidMeshObject() {
    std::vector<VertexType> vert_list;
    GenerateVertices(vert_list);

    std::vector<unsigned short> index_list;
    GenerateTriangleIndices(index_list);

    DrawCommand<VertexType> draw_cmd;
    draw_cmd.draw_mode = GL_TRIANGLES;
    draw_cmd.index_count = index_list.size();
    draw_cmd.vert_count = vert_list.size();
    draw_cmd.index_type = GL_UNSIGNED_SHORT;
    draw_cmd.vert_ptr = &vert_list[0];
    draw_cmd.index_ptr = &index_list[0];

    MeshBufferObject mbo;
    mbo.Add(draw_cmd);
    return mbo;
  }

  template<typename VertexType>
  MeshBufferObject CreateWireMeshObject() {
    std::vector<VertexType> vert_list;
    GenerateVertices(vert_list);

    std::vector<unsigned short> index_list;
    GenerateLineIndices(index_list);

    DrawCommand<VertexType> draw_cmd;
    draw_cmd.draw_mode = GL_LINES;
    draw_cmd.index_count = index_list.size();
    draw_cmd.vert_count = vert_list.size();
    draw_cmd.index_type = GL_UNSIGNED_SHORT;
    draw_cmd.vert_ptr = &vert_list[0];
    draw_cmd.index_ptr = &index_list[0];

    MeshBufferObject mbo;
    mbo.Add(draw_cmd);
    return mbo;
  }
};


struct ParametricInterval {
  glm::ivec2 divisions;
  glm::vec2 upper_bound;
  glm::vec2 texture_count;
};

class ParametricSurface : public ISurface {
public:
  int GetVertexCount() const;
  int GetLineIndexCount() const;
  int GetTriangleIndexCount() const;
  //position, normal, texcoord 전부 함유
  void GenerateVertices(std::vector<Vertex> &vertices) const;
  void GenerateVertices(std::vector<TangentVertex> &vertices) const;
  void GenerateLineIndices(std::vector<unsigned short>& indices) const;
  void GenerateTriangleIndices(std::vector<unsigned short>& indices) const;

protected:
  void SetInterval(const ParametricInterval& interval);
  virtual glm::vec3 Evaluate(const glm::vec2& domain) const = 0;
  virtual bool InvertNormal(const glm::vec2& domain) const { return false; }

private:
  //필요에 따라서 normal만 있는거, texcoord만 있는거 같은식으로 구분하면
  //복잡도가 커져서 그냥 이거 안쓰고 Vertex를 쓰는거로 수정햇다
  void GenerateVertices(std::vector<float>& vertices, unsigned char flags) const;
  glm::vec2 ComputeDomain(float i, float j) const;
  glm::ivec2 m_slices;
  glm::ivec2 m_divisions;
  glm::vec2 m_upperBound;
  glm::vec2 m_textureCount;
};

}