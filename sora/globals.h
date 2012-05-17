﻿/*  Copyright (C) 2011-2012 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#ifndef SORA_GLOBALS_H_
#define SORA_GLOBALS_H_

#include "shared_ptr_inc.h"
#include "arch.h"

namespace sora {;

typedef enum {
  kHandleNone = 0,
  kHandleAttrib,
  kHandleUniform,
} VariableHandleType;


typedef enum {
  kVertexNone,
  kVertex2D,
  kVertex,
  kVertexTangent,
  kVertexPos2D,
  kVertexPos3D,
  kVertexColor3D,   //color + 3d pos

  kVertexCodeCount,
} VertexCode;


//텍스쳐 관련
typedef enum {
  kTexFileUnknown = -1,  //soil로 일단 시도해보자
  kTexFilePNG,
  kTexFileJPEG,
} TexFileType;

enum {
  kTexPolicyForcePOT = 0x01, //2의 승수로 강제 보정할것인가
};

typedef enum {
  kDrawPoints,
  kDrawLines,
  kDrawLineStrip,
  kDrawLineLoop,
  kDrawTriangles,
  kDrawTriangleStrip,
  kDrawTriangleFan,
  kDrawTypeCount,
} DrawType;

typedef enum {
  kTexFormatLumiance,
  kTexFormatAlpha,
  kTexFormatLumianceAlpha,
  kTexFormatRGBA,
  kTexFormatRGB,
  kTexFormatTypeCount,
} TexFormatType;

typedef enum {
  kTexMinLinear,
  kTexMinNearest,
  kTexMinNearestMipmapNearest,
  kTexMinNearestMipmapLinear,
  kTexMinLinearMipmapNearest,
  kTexMinLinearMipmapLinear,
  kTexMinFilterCount,
} TexMinFilter;

typedef enum {
  kTexMagLinear,
  kTexMagNearest,
  kTexMagFilterCount,
} TexMagFilter;

typedef enum {
  kTexWrapRepeat,
  kTexWrapClampToEdge,
  kTexWrapMirroredRepeat,
  kTexWrapModeCount,
} TexWrapMode;

typedef enum {
  kTypeFloat,
  kTypeInt,
  kTypeUint,
  kTypeShort,
  kTypeUshort,
  kTypeByte,
  kTypeUbyte,

  kTypeFloatMat4,
  kTypeFloatMat3,
  kTypeFloatMat2,

  kTypeFloatVec4,
  kTypeFloatVec3,
  kTypeFloatVec2,
  kTypeIntVec4,
  kTypeIntVec3,
  kTypeIntVec2,

  kTypeSample2D, 
  kTypeSampleCube,
  kVarTypeCount,
} VarType;

typedef enum {
  kLightPoint,
  kLightDirection,
  kLightSpotLight,
} LightType;

typedef enum {
  kRenderState2D,
  kRenderState3D,
} RenderStateType;

struct TextureParam {
  TextureParam() 
    : mag_filter(kTexMagLinear),
    min_filter(kTexMinLinear),
    wrap_s(kTexWrapRepeat),
    wrap_t(kTexWrapRepeat) {}
  TexMagFilter mag_filter;
  TexMinFilter min_filter;
  TexWrapMode wrap_s;
  TexWrapMode wrap_t;
};


struct ImageDesc {
  ImageDesc() { memset(this, sizeof(ImageDesc), 0); }
  int width;
  int height;
  int bit_depth;
  int bpp;
  int color_channels;
  bool is_grayscale;
  bool is_alpha;
};

typedef enum {
  kBufferUsageStatic,
  kBufferUsageDyanmic,
  kBufferUsageStream,
} BufferUsageType;

//기본도형은 normal까지만 지원하낟. 그 이상의 정보가 필요하면
//버텍스 목록에 추가 가공을 다른방식으로 집어넣자
template<typename VertexType>
struct DrawCmdData {
  DrawCmdData() : disable_cull_face(false) { }
  bool disable_cull_face;
  DrawType draw_mode;
  std::vector<VertexType> vertex_list;
  std::vector<unsigned short> index_list;
};

//템플릿을 사용한 클래스의 전방선언을 일일이 치는것은 좀 미친짓같다. 그래서 여기에 하나 모앗다
//for shader - begin
#if SR_USE_GL
namespace gl {
  class GLProgram;
  class GLShaderVariable;
}
typedef unsigned int ShaderHandle;
typedef int ShaderVariableLocation;
typedef sora::gl::GLProgram ShaderPolicy;
typedef sora::gl::GLShaderVariable ShaderVariablePolicy;
const int kInvalidShaderVarLocation = -1;
#endif
class Shader;
//for shader - end

//for buffer - begin
#if SR_USE_GL
namespace gl {
  template<GLenum Target> class GLBufferObject;
  typedef GLBufferObject<GL_ARRAY_BUFFER> GLVertexBufferObject;
  typedef GLBufferObject<GL_ELEMENT_ARRAY_BUFFER> GLIndexBufferObject;
}
typedef sora::gl::GLVertexBufferObject VBOPolicy;
typedef sora::gl::GLIndexBufferObject IBOPolicy;
#endif

template<typename VertexT> class VertexBufferObjectT;
class IndexBufferObject;

typedef unsigned int VertexBufferHandle;
typedef unsigned int IndexBufferHandle;
//for bufer - end

//for texture - begin
#if SR_USE_GL
namespace gl {
  class GLTexture;
}
typedef sora::gl::GLTexture TexturePolicy;
typedef unsigned int TextureHandle;
#endif
class Image;
class Texture;
typedef std::tr1::shared_ptr<Texture> TexturePtr;
//for texture - end

//for frame buffer - begin
#if SR_USE_GL
namespace gl {
  class GLFrameBuffer;
}
typedef unsigned int FrameBufferHandle;
#endif

template<typename PolicyType> class FrameBufferT;
typedef sora::gl::GLFrameBuffer FrameBufferPolicy;
typedef FrameBufferT<FrameBufferPolicy> FrameBuffer;
//for frame buffer - end

//for render device
#if SR_USE_GL
namespace gl {
  class GLRenderState;
}
typedef sora::gl::GLRenderState RenderStatePolicy;
#endif
class RenderState;
//for render device

//for mesh buffer - begin
struct VertexBufferInterface;
struct IndexBufferInterface;
template<typename T, typename InterfaceT> class VectorWrapper;
typedef VectorWrapper<unsigned short, IndexBufferInterface> IndexArray;
template<typename T>  class VertexArrayT;
//for mesh buffer - end

//material/uber shader에서 사용되는 플래그용
enum {
  kMaterialAmbient = 1 << 0,
  kMaterialDiffuse = 1 << 1,
  kMaterialSpecular = 1 << 2,
  kMaterialDiffuseMap = 1 << 3,
  kMaterialSpecularMap = 1 << 4,
  kMaterialNormalMap = 1 << 5,
};

typedef enum {
  kMaterialTypeNone,
  kMaterialTypeUber,
} MaterialType;

//predefined semantic
#define kPositionHandleName "a_position"
#define kTexcoordHandleName "a_texcoord"
#define kNormalHandleName "a_normal"
#define kColorHandleName "a_color"
#define kTangentHandleName "a_tangent"

#define kConstColorHandleName "u_constColor"

#define kViewPositionHandleName "u_viewPosition"
#define kViewSideHandleName "u_viewSide"
#define kViewUpHandleName "u_viewUp"
#define kViewDirHandleName "u_viewDir"

#define kMVPHandleName "u_mvp"
#define kModelHandleName "u_model"
#define kProjectionHandleName "u_projection"
#define kViewHandleName "u_view"
#define kMVInvTransposeHandleName "u_mvInvTranspose"

#define kAmbientColorHandleName "u_ambientColor"
#define kDiffuseColorHandleName "u_diffuseColor"
#define kSpecularColorHandleName "u_specularColor"
#define kSpecularShininessHandleName "u_specularShininess"

#define kDiffuseMapHandleName "s_diffuseMap"
#define kSpecularMapHandleName "s_specularMap"
#define kAlbedoHandleName "s_texture"
#define kNormalMapHandleName "s_normalMap"

#define kLightPositionHandleName "u_lightPos"
}

#endif  // SORA_GLOBALS_H_