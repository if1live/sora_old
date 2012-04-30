/*
* freeglut_font.c
*
* Bitmap and stroke fonts displaying.
*
* Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
* Written by Pawel W. Olszta, <olszta@sourceforge.net>
* Creation date: Thu Dec 16 1999
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

//#include "freeglut_internal.h"

/*
* TODO BEFORE THE STABLE RELEASE:
*
*  Test things out ...
*/

/* -- IMPORT DECLARATIONS -------------------------------------------------- */
#include "sora_stdafx.h"
#include "freeglut_font.h"

using namespace glm;


namespace sora {;

/* -- PRIVATE FUNCTIONS ---------------------------------------------------- */


/*
* Matches a font ID with a SFG_StrokeFont structure pointer.
* This was changed to match the GLUT header style.
*/
static SFG_StrokeFont* fghStrokeByID( void* font )
{
  if( font == GLUT_STROKE_ROMAN      )
    return get_fgStrokeRoman();
  if( font == GLUT_STROKE_MONO_ROMAN )
    return get_fgStrokeMonoRoman();

  LOGE( "stroke font 0x%08x not found", font );
  return 0;
}


/* -- INTERFACE FUNCTIONS -------------------------------------------------- */
/*
* Draw a stroke character
*/
std::vector< DrawCmdData<glm::vec2> > glutStrokeCharacter( void* fontID, int character ) {
  std::vector< DrawCmdData<glm::vec2> > result;

  const SFG_StrokeChar *schar;
  const SFG_StrokeStrip *strip;
  int i, j;
  SFG_StrokeFont* font;
  font = fghStrokeByID( fontID );
  if(!( character >= 0 )) {
    return result;
  }
  if(!(character < font->Quantity )) {
    return result;
  }
  if( !(font )) {
    return result;
  }

  schar = font->Characters[ character ];
  if( !(schar )) {
    return result;
  }
  strip = schar->Strips;

  for( i = 0; i < schar->Number; i++, strip++ ) {
    DrawCmdData<glm::vec2> line_cmd;
    line_cmd.draw_mode = kDrawLineStrip;
    for( j = 0; j < strip->Number; j++ ) {
      //glVertex2f( strip->Vertices[ j ].X, strip->Vertices[ j ].Y );
      vec2 vert( strip->Vertices[ j ].X, strip->Vertices[ j ].Y);
      line_cmd.vertex_list.push_back(vert);
    }
    result.push_back(line_cmd);

    DrawCmdData<glm::vec2> point_cmd;
    point_cmd.draw_mode = kDrawPoints;
    for( j = 0; j < strip->Number; j++ ) {
      vec2 vert( strip->Vertices[ j ].X, strip->Vertices[ j ].Y);
      line_cmd.vertex_list.push_back(vert);
    }
    result.push_back(point_cmd);
  }
  //glTranslatef( schar->Right, 0.0, 0.0 );
  return result;
}

std::vector< DrawCmdData<glm::vec2> > glutStrokeString( void* fontID, const char *string ) {
  std::vector< DrawCmdData<glm::vec2> > result;
  unsigned char c;
  int i, j;
  float length = 0.0;
  SFG_StrokeFont* font;
  font = fghStrokeByID( fontID );
  if(font == NULL) {
    return result;
  }
  if ( !string || ! *string )
    return result;

  /*
  * Step through the string, drawing each character.
  * A newline will simply translate the next character's insertion
  * point back to the start of the line and down one line.
  */

  //translate로 좌표이동하는걸 수동으로 추적해야 vertex list를 구성할수 잇다
  float translate_x = 0;
  float translate_y = 0;

  while( ( c = *string++) ) {
    if( c < font->Quantity )
    {
      if( c == '\n' )
      {
        //glTranslatef ( -length, -( float )( font->Height ), 0.0 );
        translate_x += -length;
        translate_y += -( float )( font->Height );
        length = 0.0;
      }
      else  /* Not an EOL, draw the bitmap character */
      {
        const SFG_StrokeChar *schar = font->Characters[ c ];
        if( schar )
        {
          const SFG_StrokeStrip *strip = schar->Strips;

          for( i = 0; i < schar->Number; i++, strip++ )
          {
            DrawCmdData<glm::vec2> cmd;
            cmd.draw_mode = kDrawLineStrip;
            for( j = 0; j < strip->Number; j++ ) {
              //glVertex2f( strip->Vertices[ j ].X, strip->Vertices[ j ].Y);
              float x = translate_x + strip->Vertices[ j ].X;
              float y = translate_y + strip->Vertices[ j ].Y;
              vec2 vert(x, y);
              cmd.vertex_list.push_back(vert);
            }
            result.push_back(cmd);
          }

          length += schar->Right;
          //glTranslatef( schar->Right, 0.0, 0.0 );
          translate_x += schar->Right;
        }
      }
    }
  }
  return result;
}

/*
* Return the width in pixels of a stroke character
*/
int  glutStrokeWidth( void* fontID, int character )
{
  const SFG_StrokeChar *schar;
  SFG_StrokeFont* font;
  font = fghStrokeByID( fontID );
  freeglut_return_val_if_fail( ( character >= 0 ) &&
    ( character < font->Quantity ),
    0
    );
  freeglut_return_val_if_fail( font, 0 );
  schar = font->Characters[ character ];
  freeglut_return_val_if_fail( schar, 0 );

  return ( int )( schar->Right + 0.5 );
}

/*
* Return the width of a string drawn using a stroke font
*/
int  glutStrokeLength( void* fontID, const unsigned char* string )
{
  unsigned char c;
  float length = 0.0;
  float this_line_length = 0.0;
  SFG_StrokeFont* font;
  font = fghStrokeByID( fontID );
  freeglut_return_val_if_fail( font, 0 );
  if ( !string || ! *string )
    return 0;

  while( ( c = *string++) )
    if( c < font->Quantity )
    {
      if( c == '\n' ) /* EOL; reset the length of this line */
      {
        if( length < this_line_length )
          length = this_line_length;
        this_line_length = 0.0;
      }
      else  /* Not an EOL, increment the length of this line */
      {
        const SFG_StrokeChar *schar = font->Characters[ c ];
        if( schar )
          this_line_length += schar->Right;
      }
    }
    if( length < this_line_length )
      length = this_line_length;
    return( int )( length + 0.5 );
}

/*
* Returns the height of a stroke font
*/
GLfloat  glutStrokeHeight( void* fontID )
{
  SFG_StrokeFont* font;
  font = fghStrokeByID( fontID );
  freeglut_return_val_if_fail( font, 0.0 );
  return font->Height;
}

}

/*** END OF FILE ***/