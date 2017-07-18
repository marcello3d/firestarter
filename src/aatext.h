/********
**  Space Slicer Unlimited
***   Copyright © 2002 Marcello Bastea-Forte
 **          contact: marcello@cellosoft.com
 ********
** 
** aatext.cpp    -   March 5, 2002
**
 **   AATEXT - Antialiased text fonts for Allegro
 **   Douglas Eleveld (D.J.Eleveld@anest.azg.nl)
 **
 **     (due to some implementation problems, this has library
 **      has been included directly) 
 **
 ********/

#ifndef INCLUDE_AATEXT_H
#define INCLUDE_AATEXT_H
#include <allegro.h>
#include <allegro/internal/aintern.h>


//----------------------------------------------------------------------------
// Basic font scaling variables
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Antialiased font rendering hook
//----------------------------------------------------------------------------
int _antialiased_render (AL_CONST FONT *f, int ch, int fg, int bg, struct BITMAP *bmp, int x, int y);

//----------------------------------------------------------------------------
// Internal functions
//----------------------------------------------------------------------------
/* Used for scratchpad for blitting the characters */
extern struct BITMAP *_scratchpad;

/* Used for fast aliasing for filled background modes */
extern int _aliasmap[8];

/* Used for fast changing of the transparency map in 256 color aliasing */
/* color_map46 is used as a flag to see if the color maps have been initialized */
extern COLOR_MAP* _alias_color_map46;
extern COLOR_MAP* _alias_color_map141;





/* Used for scratchpad for blitting the characters */
extern BITMAP *_scratchpad;

/* Used for fast aliasing for filled background modes */
extern int _aliasmap_foreground;
extern int _aliasmap_background;
extern int _aliasmap_depth;
extern int _aliasmap[8];

/*----------------------------------------------------------------------------
   Setup the global aliasmap for fast aliasing of text with
   filled backgrounds.
*/
void _setup_aliasmap (int fg, int bg, int depth);

/*----------------------------------------------------------------------------
   Make sure the aliasmap has the correct foreground and background colors
   and also prepare the scratchpad
*/
void _prepare_aliasmap (int fg, int bg, int depth);


int _antialiased_clip (int ct, int cb, int y, int *h);

void _antialiased_15_or_16bit_filled_render (struct BITMAP *b, struct BITMAP *bmp, int x, int y);


#ifdef ALLEGRO_COLOR16

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans16(unsigned long x, unsigned long y, unsigned long n);

/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_16bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y);
/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 16 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels
*/
void _antialiased_16bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y);

#endif //ALLEGRO_COLOR16


#ifdef ALLEGRO_COLOR16

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans15(unsigned long x, unsigned long y, unsigned long n);


/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_15bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y);
/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 15 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels
*/
void _antialiased_15bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y);

#endif //ALLEGRO_COLOR16










#ifdef ALLEGRO_COLOR24

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans24(unsigned long x, unsigned long y, unsigned long n);

/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_24bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y);

/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 24 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels.  Actually
   it works pretty well in all 15,16,24 and 32 bit modes.
*/
void _antialiased_24bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y);
#endif //ALLEGRO_COLOR24








#ifdef ALLEGRO_COLOR32

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans24(unsigned long x, unsigned long y, unsigned long n);

/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_32bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y);

/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 32 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels
*/
void _antialiased_32bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y);
#endif //ALLEGRO_COLOR32





//----------------------------------------------------------------------------
// Basic text drawing functions
//----------------------------------------------------------------------------

/* find_glyph:
 *  Searches a font for a specific character.
 */
BITMAP* find_glyph(AL_CONST FONT* f, int ch);

/*----------------------------------------------------------------------------
   Actual textout routine
*/
void aatextout (BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour);

/*----------------------------------------------------------------------------
   Centered textout routine
*/
void aatextout_centre(BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour);

/*----------------------------------------------------------------------------
   Right aligned textout routine
*/
void aatextout_right(BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour);

/*----------------------------------------------------------------------------
   Internal character rendering function
*/
int _antialiased_render (AL_CONST FONT *f, int ch, int fg, int bg, struct BITMAP *bmp, int x, int y);

//----------------------------------------------------------------------------
// these printf-like routines come almost directly from Allegro
//----------------------------------------------------------------------------
/* aatextprintf:
 *  Formatted text output, using a printf() style format string.
 */
void aatextprintf(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...);

/* aatextprintf_center:
 *  Like aatextprintf(), but uses the x coordinate as the centre rather than
 *  the left of the string.
 */
void aatextprintf_center(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...);

/* aatextprintf_right:
 *  Like aatextprintf(), but uses the x coordinate as the rightmost
 *  coordinate, rather than the left of the string.
 */
void aatextprintf_right(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...);
#endif//!INCLUDE_AATEXT_H

