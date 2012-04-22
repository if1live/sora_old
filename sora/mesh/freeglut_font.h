/*
 * freeglut_internal.h
 *
 * The freeglut library private include file.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 2 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef  FREEGLUT_INTERNAL_H
#define  FREEGLUT_INTERNAL_H

#include "renderer/globals.h"
#include "core/vertex.h"

namespace sora {;
/*
 * GLUT API macro definitions -- fonts definitions
 *
 * Steve Baker suggested to make it binary compatible with GLUT:
 */
//#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__WATCOMC__)
#   define  GLUT_STROKE_ROMAN               ((void *)0x0000)
#   define  GLUT_STROKE_MONO_ROMAN          ((void *)0x0001)
#   define  GLUT_BITMAP_9_BY_15             ((void *)0x0002)
#   define  GLUT_BITMAP_8_BY_13             ((void *)0x0003)
#   define  GLUT_BITMAP_TIMES_ROMAN_10      ((void *)0x0004)
#   define  GLUT_BITMAP_TIMES_ROMAN_24      ((void *)0x0005)
#   define  GLUT_BITMAP_HELVETICA_10        ((void *)0x0006)
#   define  GLUT_BITMAP_HELVETICA_12        ((void *)0x0007)
#   define  GLUT_BITMAP_HELVETICA_18        ((void *)0x0008)
//#else

/* The bitmap font structure */
typedef struct tagSFG_Font SFG_Font;
struct tagSFG_Font
{
  char*           Name;         /* The source font name             */
  int             Quantity;     /* Number of chars in font          */
  int             Height;       /* Height of the characters         */
  const GLubyte** Characters;   /* The characters mapping           */

  float           xorig, yorig; /* Relative origin of the character */
};


/* The stroke font structures */

typedef struct tagSFG_StrokeVertex SFG_StrokeVertex;
struct tagSFG_StrokeVertex
{
  GLfloat         X, Y;
};

typedef struct tagSFG_StrokeStrip SFG_StrokeStrip;
struct tagSFG_StrokeStrip
{
  int             Number;
  const SFG_StrokeVertex* Vertices;
};

typedef struct tagSFG_StrokeChar SFG_StrokeChar;
struct tagSFG_StrokeChar
{
  GLfloat         Right;
  int             Number;
  const SFG_StrokeStrip* Strips;
};

typedef struct tagSFG_StrokeFont SFG_StrokeFont;
struct tagSFG_StrokeFont
{
  char*           Name;                       /* The source font name      */
  int             Quantity;                   /* Number of chars in font   */
  GLfloat         Height;                     /* Height of the characters  */
  const SFG_StrokeChar** Characters;          /* The characters mapping    */
};


/*
 * Font stuff, see freeglut_font.c
 */
//void    glutBitmapCharacter( void* font, int character );
//int     glutBitmapWidth( void* font, int character );
//void    glutBitmapString( void* font, const unsigned char *string );
//int     glutBitmapLength( void* font, const unsigned char* string );
//int     glutBitmapHeight( void* font );

int     glutStrokeWidth( void* font, int character );
int     glutStrokeLength( void* font, const unsigned char* string );
GLfloat glutStrokeHeight( void* font );
std::vector< DrawCmdData<Vertex> > glutStrokeCharacter( void* font, int character );
std::vector< DrawCmdData<Vertex> > glutStrokeString( void* font, const char *string );

/*
 * Following definitions are somewhat similiar to GLib's,
 * but do not generate any log messages:
 */
#define  freeglut_return_if_fail( expr ) \
    if( !(expr) )                        \
        return;
#define  freeglut_return_val_if_fail( expr, val ) \
    if( !(expr) )                                 \
        return val ;

SFG_StrokeFont *get_fgStrokeRoman();
SFG_StrokeFont *get_fgStrokeMonoRoman();
}

#endif  // FREEGLUT_INTERNAL_H