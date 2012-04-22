#include "sora_stdafx.h"
#include "parametric_surface.h"

using namespace std;
using namespace glm;

namespace sora {;

enum VertexFlags {
    kVertexFlagsNormals = 1 << 0,
    kVertexFlagsTexCoords = 1 << 1,
    kVertexFlagsTangents = 1 << 2,
};

void ParametricSurface::SetInterval(const ParametricInterval& interval) {
  m_divisions = interval.divisions;
  m_upperBound = interval.upper_bound;
  m_textureCount = interval.texture_count;
  m_slices = m_divisions - ivec2(1, 1);
}

int ParametricSurface::GetVertexCount() const {
  return m_divisions.x * m_divisions.y;
}

int ParametricSurface::GetLineIndexCount() const {
  return 4 * m_slices.x * m_slices.x;
}

int ParametricSurface::GetTriangleIndexCount() const {
  return 6 * m_slices.x * m_slices.y;
}

vec2 ParametricSurface::ComputeDomain(float x, float y) const {
  return vec2(x * m_upperBound.x / m_slices.x, y * m_upperBound.y / m_slices.y);
}

void ParametricSurface::GenerateVertices(vector<float>& vertices, unsigned char flags) const {
  int floatsPerVertex = 3;
  if (flags & kVertexFlagsNormals) {
    floatsPerVertex += 3;
  }
  if (flags & kVertexFlagsTexCoords) {
    floatsPerVertex += 2;
  }
  if(flags & kVertexFlagsTangents) {
    floatsPerVertex += 3;
  }


  vertices.resize(GetVertexCount() * floatsPerVertex);
  float* attribute = &vertices[0];

  for (int j = 0; j < m_divisions.y; j++) {
    for (int i = 0; i < m_divisions.x; i++) {

      // Compute Position
      vec2 domain = ComputeDomain((float)i, (float)j);
      vec3 range = Evaluate(domain);
      memcpy(attribute, &range, sizeof(range));
      attribute = (float*)(((unsigned char*)attribute) + sizeof(range));
      //attribute = range.Write(attribute);

      // Compute Normal
      if (flags & kVertexFlagsNormals) {
        float s = (float)i, t = (float)j;

        // Nudge the point if the normal is indeterminate.
        if (i == 0) s += 0.01f;
        if (i == m_divisions.x - 1) s -= 0.01f;
        if (j == 0) t += 0.01f;
        if (j == m_divisions.y - 1) t -= 0.01f;

        // Compute the tangents and their cross product.
        vec3 p = Evaluate(ComputeDomain(s, t));
        vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
        vec3 v = Evaluate(ComputeDomain(s, t + 0.01f)) - p;
        vec3 normal = glm::normalize(glm::cross(u, v));
        if (InvertNormal(domain)) {
          normal = -normal;
        }
        //attribute = normal.Write(attribute);
        memcpy(attribute, &normal, sizeof(normal));
        attribute = (float*)(((unsigned char*)attribute) + sizeof(normal));
      }

      // Compute Texture Coordinates
      if (flags & kVertexFlagsTexCoords) {
        float s = m_textureCount.x * i / m_slices.x;
        float t = m_textureCount.y * j / m_slices.y;
        vec2 texcoord(s, t);
        //attribute = vec2(s, t).Write(attribute);
        memcpy(attribute, &texcoord, sizeof(texcoord));
        attribute = (float*)(((unsigned char*)attribute) + sizeof(texcoord));
      }

      if(flags & kVertexFlagsTangents) {
        float s = (float)i, t = (float)j;
        vec3 p = Evaluate(ComputeDomain(s, t));
        vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
        if(InvertNormal(domain)) {
          u = -u;
        }
        memcpy(attribute, &u, sizeof(u));
        attribute = (float*)(((unsigned char*)attribute) + sizeof(u));
      }
    }
  }
}

void ParametricSurface::GenerateLineIndices(vector<unsigned short>& indices) const {
  indices.resize(GetLineIndexCount());
  vector<unsigned short>::iterator index = indices.begin();
  for (int j = 0, vertex = 0; j < m_slices.y; j++) {
    for (int i = 0; i < m_slices.x; i++) {
      int next = (i + 1) % m_divisions.x;
      *index++ = vertex + i;
      *index++ = vertex + next;
      *index++ = vertex + i;
      *index++ = vertex + i + m_divisions.x;
    }
    vertex += m_divisions.x;
  }
}

void ParametricSurface::GenerateTriangleIndices(vector<unsigned short>& indices) const {
  indices.resize(GetTriangleIndexCount());
  vector<unsigned short>::iterator index = indices.begin();
  for (int j = 0, vertex = 0; j < m_slices.y; j++) {
    for (int i = 0; i < m_slices.x; i++) {
      int next = (i + 1) % m_divisions.x;
      *index++ = vertex + i;
      *index++ = vertex + next;
      *index++ = vertex + i + m_divisions.x;
      *index++ = vertex + next;
      *index++ = vertex + next + m_divisions.x;
      *index++ = vertex + i + m_divisions.x;
    }
    vertex += m_divisions.x;
  }
}

void ParametricSurface::GenerateVertices(std::vector<Vertex> &vertices) const {
  vector<float> raw_vert_list;
  GenerateVertices(raw_vert_list, kVertexFlagsNormals | kVertexFlagsTexCoords);

  vertices.clear();
  vertices.resize(raw_vert_list.size() / 8);

  for(size_t i = 0 ; i < raw_vert_list.size() / 8 ; i++) {
    Vertex &v = vertices[i];
    v.pos.x = raw_vert_list[i*8 + 0];
    v.pos.y = raw_vert_list[i*8 + 1];
    v.pos.z = raw_vert_list[i*8 + 2];

    v.normal.x = raw_vert_list[i*8 + 3];
    v.normal.y = raw_vert_list[i*8 + 4];
    v.normal.z = raw_vert_list[i*8 + 5];

    v.texcoord.x = raw_vert_list[i*8 + 6];
    v.texcoord.y = raw_vert_list[i*8 + 7];
  }
}

void ParametricSurface::GenerateVertices(std::vector<TangentVertex> &vertices) const {
  vector<float> raw_vert_list;
  unsigned int flag = 0;
  flag |= kVertexFlagsNormals;
  flag |= kVertexFlagsTexCoords;
  flag |= kVertexFlagsTangents;
  GenerateVertices(raw_vert_list, flag);

  int scanline = 11;
  vertices.clear();
  vertices.resize(raw_vert_list.size() / scanline);

  for(size_t i = 0 ; i < raw_vert_list.size() / scanline ; i++) {
    TangentVertex &v = vertices[i];
    v.pos.x = raw_vert_list[i*scanline + 0];
    v.pos.y = raw_vert_list[i*scanline + 1];
    v.pos.z = raw_vert_list[i*scanline + 2];

    v.normal.x = raw_vert_list[i*scanline + 3];
    v.normal.y = raw_vert_list[i*scanline + 4];
    v.normal.z = raw_vert_list[i*scanline + 5];

    v.texcoord.x = raw_vert_list[i*scanline + 6];
    v.texcoord.y = raw_vert_list[i*scanline + 7];

    v.tangent.x = raw_vert_list[i*scanline + 8];
    v.tangent.y = raw_vert_list[i*scanline + 9];
    v.tangent.z = raw_vert_list[i*scanline + 10];
  }
}
}