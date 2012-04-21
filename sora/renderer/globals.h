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

namespace sora {;

typedef enum {
  kHandleNone = 0,
  kHandleAttrib,
  kHandleUniform,
} HandleType;

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
} DrawType;

typedef enum {
  kTexFormatLumiance,
  kTexFormatAlpha,
  kTexFormatLumianceAlpha,
  kTexFormatRGBA,
  kTexFormatRGB,
} TexFormatType;

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
#define kMVPHandleName "u_modelViewProjection"
#define kWorldHandleName "u_world"
#define kProjectionHandleName "u_projection"
#define kViewHandleName "u_view"

#define kAmbientColorHandleName "u_ambientColor"
#define kDiffuseColorHandleName "u_diffuseColor"
#define kSpecularColorHandleName "u_specularColor"
#define kSpecularShininessHandleName "u_specularShininess"

#define kDiffuseMapHandleName "s_diffuseMap"
#define kSpecularMapHandleName "s_specularMap"
#define kAmbientMapHandleName "s_ambientMap"
#define kAlbedoHandleName "s_texture"
#define kNormalMapHandleName "s_normalMap"

}

#endif  // SORA_GLOBALS_H_