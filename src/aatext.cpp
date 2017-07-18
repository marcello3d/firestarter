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

#include "aatext.h"


//----------------------------------------------------------------------------
// Basic font scaling variables
//----------------------------------------------------------------------------
#define HEIGHT_RATIO 7/6
#define OFFSET_RATIO 1/3

//----------------------------------------------------------------------------
// Antialiased font rendering hook
//----------------------------------------------------------------------------
int _antialiased_render (AL_CONST FONT *f, int ch, int fg, int bg, struct BITMAP *bmp, int x, int y);

//----------------------------------------------------------------------------
// Internal functions
//----------------------------------------------------------------------------
/* Used for scratchpad for blitting the characters */
#define _SCRATCHPAD_HEIGHT 128
#define _SCRATCHPAD_WIDTH  128
extern struct BITMAP *_scratchpad;

/* Used for fast aliasing for filled background modes */
extern int _aliasmap[8];

/* Used for fast changing of the transparency map in 256 color aliasing */
/* color_map46 is used as a flag to see if the color maps have been initialized */
extern COLOR_MAP* _alias_color_map46;
extern COLOR_MAP* _alias_color_map141;





/* Used for scratchpad for blitting the characters */
BITMAP *_scratchpad = NULL;

/* Used for fast aliasing for filled background modes */
int _aliasmap_foreground = -1;
int _aliasmap_background = -1;
int _aliasmap_depth = -1;
int _aliasmap[8];

/*----------------------------------------------------------------------------
   Setup the global aliasmap for fast aliasing of text with
   filled backgrounds.
*/
void _setup_aliasmap (int fg, int bg, int depth)
   {
   int cr, cg, cb, tr, tg, tb;
   int(*colmaker)(int,int,int);

   int darkness, index;

   switch(depth)
      {
      case(15):
         cr = getr15(fg);
         cg = getg15(fg);
         cb = getb15(fg);

         tr = getr15(bg);
         tg = getg15(bg);
         tb = getb15(bg);

         colmaker = makecol15;
         break;
      case(16):
         cr = getr16(fg);
         cg = getg16(fg);
         cb = getb16(fg);

         tr = getr16(bg);
         tg = getg16(bg);
         tb = getb16(bg);

         colmaker = makecol16;
         break;
      case(24):
         cr = getr24(fg);
         cg = getg24(fg);
         cb = getb24(fg);

         tr = getr24(bg);
         tg = getg24(bg);
         tb = getb24(bg);

         colmaker = makecol24;
         break;
      case(32):
         cr = getr32(fg);
         cg = getg32(fg);
         cb = getb32(fg);

         tr = getr32(bg);
         tg = getg32(bg);
         tb = getb32(bg);

         colmaker = makecol32;
         break;
      default:
         cr = getr8(fg);
         cg = getg8(fg);
         cb = getb8(fg);

         tr = getr8(bg);
         tg = getg8(bg);
         tb = getb8(bg);

         colmaker = makecol8;
         break;
      }

   /* Build the color map */
   darkness = 37;
   for(index=1;index<7;index++)
      {
      _aliasmap[index] = colmaker((cr*darkness+(255-darkness)*tr)>>8,
                                  (cg*darkness+(255-darkness)*tg)>>8,
                                  (cb*darkness+(255-darkness)*tb)>>8);
      darkness += 37;
      }
   _aliasmap[0] = bg;
   _aliasmap[7] = fg;

   /* Confirm that the aliasmap has been built */
   _aliasmap_foreground = fg;
   _aliasmap_background = bg;
   _aliasmap_depth = depth;
   }

/*----------------------------------------------------------------------------
   Make sure the aliasmap has the correct foreground and background colors
   and also prepare the scratchpad
*/
void _prepare_aliasmap (int fg, int bg, int depth)
   {
   /* Possible setup the quick color alias map */
   if((fg!=_aliasmap_foreground)||
      (bg!=_aliasmap_background)||
      (_aliasmap_depth!=depth))
      {
      _setup_aliasmap(fg,bg,depth);
      }

   /* Possible setup for internal scratchpad bitmap */
   if((_scratchpad==NULL)||
      (bitmap_color_depth(_scratchpad)!=depth))
      {
      if(_scratchpad!=NULL) destroy_bitmap(_scratchpad);
      _scratchpad = create_bitmap_ex(depth,_SCRATCHPAD_WIDTH,_SCRATCHPAD_HEIGHT);
      }
   }



int _antialiased_clip (int ct, int cb, int y, int *h)
   {
   if(y+*h>=cb)
     *h = cb - y;

   if(y<ct)
     return ct - y;

   return 0;
   }






void _antialiased_15_or_16bit_filled_render (struct BITMAP *b, struct BITMAP *bmp, int x, int y)
   {
   int i;

   /* Build each character */
   for(i = 0; i < b->h; i++)
      {
      unsigned char* input = b->line[i];
      unsigned short* output = (unsigned short*)_scratchpad->line[i];
      const unsigned char* inputend = &b->line[i][b->w];

      /* Super fast blit of a row to temp bitmap */
      while(input!=inputend)
         *output++ = _aliasmap[*input++>>5];
      }
   /* Blit the character to the destination */
   blit(_scratchpad,bmp,0,0,x,y,b->w,b->h);
   }


#ifdef ALLEGRO_COLOR16

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans16(unsigned long x, unsigned long y, unsigned long n);

/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_16bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y)
   {
   /* Setup the internal scratchpad and aliasmap to the correct color depth */
   _prepare_aliasmap(fg,bg,16);

   /* do the common 15 or 16 bit character drawing */
   _antialiased_15_or_16bit_filled_render(b,bmp,x,y);
   }

/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 16 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels
*/
void _antialiased_16bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y)
   {
   int xx, i, yy, xxend;
   unsigned char *input;

   /* Set the selector for the far poke in the inner loop */
   _farsetsel(bmp->seg);

   /* Plot the aliased pixels */
   for(i = 0; i < b->h; i++)
      {
      input = b->line[i];

      /* Find the line number and check if it's clipped */
      yy = y+i;

      /* Clipping under means we are done */
      if(yy>=bmp->cb)
        return;

      if(yy>=bmp->ct)
         {
         /* Adjust for any LHS clipping */
         int pix = 0;
         if(x<bmp->cl)
            {
            pix = bmp->cl - x;
            input += pix;
            }

         /* Calculate where the RHS is */
         xxend = x + b->w;

         /* Adjust the RHS for clipping */
         if(xxend>=bmp->cr)
            xxend = bmp->cr;

         xxend *=2;

         for(xx = (x + pix)*2; xx < xxend; xx+=2)
            {
            /* Blend the color */
            unsigned int blendlevel = *input++;

            if (blendlevel)
               {
               /* Get the bacgground and blend it */
               unsigned int back = _farnspeekw(bmp_read_line(bmp,yy)+xx);
               back = _blender_trans16(fg,back,blendlevel);

               /* Plot it to the screen */
               _farnspokew(bmp_write_line(bmp,yy)+xx, back);
               }
            }
         }
      }
   }

#endif //ALLEGRO_COLOR16


#ifdef ALLEGRO_COLOR16

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans15(unsigned long x, unsigned long y, unsigned long n);


/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_15bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y)
   {
   /* Setup the internal scratchpad and aliasmap to the correct color depth */
   _prepare_aliasmap(fg,bg,15);

   /* do the common 15 or 16 bit character drawing */
   _antialiased_15_or_16bit_filled_render(b,bmp,x,y);
   }

/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 15 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels
*/
void _antialiased_15bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y)
   {
   int xx, i, yy, xxend;
   unsigned char *input;

   /* Set the selector for the far poke in the inner loop */
   _farsetsel(bmp->seg);

   /* Plot the aliased pixels */
   for(i = 0; i < b->h; i++)
      {
      input = b->line[i];

      /* Find the line number and check if it's clipped */
      yy = y+i;

      /* Clipping under means we are done */
      if(yy>=bmp->cb)
        return;

      if(yy>=bmp->ct)
         {
         /* Adjust for any LHS clipping */
         int pix = 0;
         if(x<bmp->cl)
            {
            pix = bmp->cl - x;
            input += pix;
            }

         /* Calculate where the RHS is */
         xxend = x + b->w;

         /* Adjust the RHS for clipping */
         if(xxend>=bmp->cr)
            xxend = bmp->cr;

         xxend *= 2;

         for(xx = (x + pix)*2; xx < xxend; xx+=2)
            {
            /* Blend the color */
            const unsigned int blendlevel = *input++;

            if (blendlevel)
               {
               /* Get the bacgground and blend it */
               unsigned int back = _farnspeekw(bmp_read_line(bmp,yy)+xx);
               back = _blender_trans15(fg,back,blendlevel);

               /* Plot it to the screen */
               _farnspokew(bmp_write_line(bmp,yy)+xx, back);
               }
            }
         }
      }
   }

#endif //ALLEGRO_COLOR16










#ifdef ALLEGRO_COLOR24

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans24(unsigned long x, unsigned long y, unsigned long n);

/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_24bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y)
   {
   int i;

   /* Setup the internal scratchpad abd aliasmap to the correct color depth */
   _prepare_aliasmap(fg,bg,24);

   /* Build each character */
   for(i = 0; i < b->h; i++)
      {
      unsigned char* input = b->line[i];
      unsigned char* output = _scratchpad->line[i];
      const unsigned char* inputend = &b->line[i][b->w];

      /* Super fast blit of a row to temp bitmap */
      while(input!=inputend)
         {
         int c = _aliasmap[*input++>>5];

         *output++ = getr24(c);
         *output++ = getg24(c);
         *output++ = getb24(c);
         }
      }
   /* Blit the character to the destination */
   blit(_scratchpad,bmp,0,0,x,y,b->w,b->h);
   }

/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 24 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels.  Actually
   it works pretty well in all 15,16,24 and 32 bit modes.
*/
void _antialiased_24bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y)
   {
   int xx, i, yy, xxend;
   unsigned char *input;

   /* Set the selector for the far poke in the inner loop */
   _farsetsel(bmp->seg);

   /* Plot the aliased pixels */
   for(i = 0; i < b->h; i++)
      {
      input = b->line[i];

      /* Find the line number and check if it's clipped */
      yy = y+i;

      /* Clipping under means we are done */
      if(yy>=bmp->cb)
        return;

      if(yy>=bmp->ct)
         {
         /* Adjust for any LHS clipping */
         int pix = 0;
         if(x<bmp->cl)
            {
            pix = bmp->cl - x;
            input += pix;
            }

         /* Calculate where the RHS is */
         xxend = x + b->w;

         /* Adjust the RHS for clipping */
         if(xxend>=bmp->cr)
            xxend = bmp->cr;

         for(xx = x + pix; xx < xxend; xx++)
            {
            /* Blend the color */
            unsigned int blendlevel = *input++;

            if (blendlevel)
               {
               /* Get the bacgground and blend it */
               unsigned long raddr = bmp_read_line(bmp,yy)+xx*3;
               unsigned long waddr = bmp_write_line(bmp,yy)+xx*3;

               const unsigned int back = (_farnspeekb(raddr++)<<16) +
                                         (_farnspeekb(raddr++)<<8) +
                                         (_farnspeekb(raddr));

               /* Blend the colors and reuse the variable on raddr */
               raddr = _blender_trans24(fg,back,blendlevel);

               /* Plot it to the screen */
               _farnspokeb(waddr++, raddr>>16);
               _farnspokeb(waddr++, (raddr>>8)&0xff);
               _farnspokeb(waddr, raddr);
               }
            }
         }
      }
   }

#endif //ALLEGRO_COLOR24








#ifdef ALLEGRO_COLOR32

/* Used for fast blending for truecolor aliasing */
unsigned long _blender_trans24(unsigned long x, unsigned long y, unsigned long n);

/*----------------------------------------------------------------------------
   Very fast 6 level antialiased text blitting for true color modes
   and filled backgrounds.  I don't think it's possible to get
   it much faster than this...
*/
void _antialiased_32bit_filled_render (struct BITMAP *b, int fg, int bg, struct BITMAP *bmp, int x, int y)
   {
   int i;

   /* Setup the internal scratchpad abd aliasmap to the correct color depth */
   _prepare_aliasmap(fg,bg,32);

   /* Build each character */
   for(i = 0; i < b->h; i++)
      {
      unsigned char* input = b->line[i];
      unsigned int* output = (unsigned int*)_scratchpad->line[i];
      const unsigned char* inputend = &b->line[i][b->w];

      /* Super fast blit of a row to temp bitmap */
      while(input!=inputend)
         *output++ = _aliasmap[*input++>>5];
      }
   /* Blit the character to the destination */
   blit(_scratchpad,bmp,0,0,x,y,b->w,b->h);
   }

/*----------------------------------------------------------------------------
   Pretty fast antialiased text blitting for 32 bit modes and
   empty backgrounds. It fully supports 256 aliasing levels
*/
void _antialiased_32bit_empty_render (struct BITMAP *b, int fg, struct BITMAP *bmp, int x, int y)
   {
   int xx, i, yy, xxend;
   unsigned char *input;

   /* Set the selector for the far poke in the inner loop */
   _farsetsel(bmp->seg);

   /* Plot the aliased pixels */
   for(i = 0; i < b->h; i++)
      {
      input = b->line[i];

      /* Find the line number and check if it's clipped */
      yy = y+i;

      /* Clipping under means we are done */
      if(yy>=bmp->cb)
        return;

      if(yy>=bmp->ct)
         {
         /* Adjust for any LHS clipping */
         int pix = 0;
         if(x<bmp->cl)
            {
            pix = bmp->cl - x;
            input += pix;
            }

         /* Calculate where the RHS is */
         xxend = x + b->w;

         /* Adjust the RHS for clipping */
         if(xxend>=bmp->cr)
            xxend = bmp->cr;

         for(xx = x + pix; xx < xxend; xx++)
            {
            /* Blend the color */
            unsigned int blendlevel = *input++;

            if (blendlevel)
               {
               /* Get the bacgground and blend it */
               unsigned int back = _farnspeekl(bmp_read_line(bmp,yy)+xx*4);
               back = _blender_trans24(fg,back,blendlevel);

               /* Plot it to the screen */
               _farnspokel(bmp_write_line(bmp,yy)+xx*4, back);
               }
            }
         }
      }
   }

#endif //ALLEGRO_COLOR32






//----------------------------------------------------------------------------
// Basic text drawing functions
//----------------------------------------------------------------------------

/* find_glyph:
 *  Searches a font for a specific character.
 */
BITMAP* find_glyph(AL_CONST FONT* f, int ch)
{
   FONT_COLOR_DATA* fcd = (FONT_COLOR_DATA*)(f->data);

   while(fcd) {
       if(ch >= fcd->begin && ch < fcd->end) return fcd->bitmaps[ch - fcd->begin];
       fcd = fcd->next;
   }

   if (ch != ' ')
      return find_glyph(f, ' ');

   return NULL;
}

/*----------------------------------------------------------------------------
   Actual textout routine
*/
void aatextout (BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour)
   {
   int ch;

   /* Make sure that we haven't been called with a fixed width font by accident */
   /* Also make sure that the bitmap isn't so big as to overflow the internal scratchpad */
   if((font->vtable == font_vtable_mono)||(text_height(font)>_SCRATCHPAD_HEIGHT)) {
      textout_ex(bmp,font,string,x,y,colour,-1);
      return;
   }

   acquire_bitmap(bmp);

   /* Blit each character */
   while ((ch = *string) != 0) {

         /* Render the character */
         x += _antialiased_render(font,ch,colour,-1,bmp,x,y);

         if (x >= bmp->cr) {
            release_bitmap(bmp);
            return;
         }

      ++string;
   }

   release_bitmap(bmp);
}

/*----------------------------------------------------------------------------
   Centered textout routine
*/
void aatextout_centre(BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour)
{
   aatextout(bmp, font, string, x - (text_length(font, string) / 2), y, colour);
}

/*----------------------------------------------------------------------------
   Right aligned textout routine
*/
void aatextout_right(BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour)
{
   aatextout(bmp, font, string, x - (text_length(font, string)), y, colour);
}

/*----------------------------------------------------------------------------
   Internal character rendering function
*/
int _antialiased_render (AL_CONST FONT *f, int ch, int fg, int bg, struct BITMAP *bmp, int x, int y)
   {
   BITMAP* b = find_glyph(f, ch);

   if (!b)
      return 0;

   if (fg<0) {
      bmp->vtable->draw_256_sprite(bmp, b, x, y);
   }
   else {
      switch(bitmap_color_depth(bmp))
         {


#ifdef ALLEGRO_COLOR16
         case(15):
            if(bg>=0)
               _antialiased_15bit_filled_render(b,fg,bg,bmp,x,y);
            else
               _antialiased_15bit_empty_render(b,fg,bmp,x,y);
            break;

         case(16):
            if(bg>=0)
               _antialiased_16bit_filled_render(b,fg,bg,bmp,x,y);
            else
               _antialiased_16bit_empty_render(b,fg,bmp,x,y);
            break;
#endif //ALLEGRO_COLOR16

#ifdef ALLEGRO_COLOR24
         case(24):
            if(bg>=0)
               _antialiased_24bit_filled_render(b,fg,bg,bmp,x,y);
            else
               _antialiased_24bit_empty_render(b,fg,bmp,x,y);
            break;
#endif //ALLEGRO_COLOR24

#ifdef ALLEGRO_COLOR32
         case(32):
            if(bg>=0)
               _antialiased_32bit_filled_render(b,fg,bg,bmp,x,y);
            else
               _antialiased_32bit_empty_render(b,fg,bmp,x,y);
            break;
#endif //ALLEGRO_COLOR24
         }
      }

      return b->w;
   }

//----------------------------------------------------------------------------
// these printf-like routines come almost directly from Allegro
//----------------------------------------------------------------------------
/* aatextprintf:
 *  Formatted text output, using a printf() style format string.
 */
void aatextprintf(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...)
{
   char buf[256];

   va_list ap;
   va_start(ap, format);
   uvszprintf(buf, sizeof(buf), format, ap);
   va_end(ap);

   aatextout(bmp, f, buf, x, y, color);
}

/* aatextprintf_center:
 *  Like aatextprintf(), but uses the x coordinate as the centre rather than
 *  the left of the string.
 */
void aatextprintf_center(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...)
{
   char buf[256];

   va_list ap;
   va_start(ap, format);
   uvszprintf(buf, sizeof(buf), format, ap);
   va_end(ap);

   aatextout_centre(bmp, f, buf, x, y, color);
}

/* aatextprintf_right:
 *  Like aatextprintf(), but uses the x coordinate as the rightmost
 *  coordinate, rather than the left of the string.
 */
void aatextprintf_right(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...)
{
   char buf[256];

   va_list ap;
   va_start(ap, format);
   uvszprintf(buf, sizeof(buf), format, ap);
   va_end(ap);

   aatextout_right(bmp, f, buf, x, y, color);
}

