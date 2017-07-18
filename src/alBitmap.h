/***********************************
 *
 * alBitmap.h - Simple allegro BITMAP* wrapper
 *   composed by marcello (marcello@cellosoft.com)
 *
 *  v.02b 6/7/2002
 *  v.02 6/6/2002
 *  v.01 6/4/2002
 *
 ***********************************/

#ifndef _ALBITMAP_H
#define _ALBITMAP_H



#include <allegro.h>

#define ALBITMAP_

class alBitmap
{
	protected:
		BITMAP *bmp;
		// certain surfaces shouldn't be destroyed through alBitmap
		bool no_destroy;

#ifndef ALBITMAP_NO_DIRTY_RECTS
		// Dirty rectangle features.
		bool use_dirty_rects;
		int gridRectX;
		int gridRectY;

		int gridCellWidth;
		int gridCellHeight;

		unsigned short *gridArray;

		bool updateAll;
#endif

	public:
	// constructors
		// null constructor
		inline alBitmap() {
			bmp = screen;
			no_destroy=true;
			use_dirty_rects=false;
		}
		// create_bitmap constructor
		inline alBitmap(int width, int height) {
			bmp = create_bitmap(width,height);
			no_destroy=false;
			use_dirty_rects=false;
		}
		// copy BITMAP* constructor
		//   - cloneme will copy the image data or the pointer
		//   - stealme will steal the pointer (allows alBitmap to destroy bitmap)
		inline alBitmap(BITMAP *nbmp, bool cloneme = false, bool bno_destroy = false) {
			if (cloneme) clone(nbmp);
			else {
				bmp = nbmp;
				no_destroy=bno_destroy;
			}
			use_dirty_rects=false;
		}
		// copy alBitmap constructor
		//   - cloneme will copy the image data or the pointer
		//   - stealme will steal the pointer (allows alBitmap to destroy bitmap)
		inline alBitmap(alBitmap& nbmp, bool cloneme = false, bool bno_destroy = false) {
			if (cloneme) clone(nbmp);
			else {
				bmp = nbmp.bmp;
				no_destroy=bno_destroy;
			}
			use_dirty_rects=false;
		}
		// create_bitmap_ex constructor
		inline alBitmap(int width, int height, int depth) {
			bmp = create_bitmap_ex(width,height,depth);
			no_destroy=false;
			use_dirty_rects=false;
		}
		// create video/system/normal bitmap group constructor
		// Yes I know it's kinda iffy, suggestions?
		/* TODO: Replace with something better.

		inline alBitmap(char c, int width, int height) {
			if (c=='v')
				bmp = create_video_bitmap(width,height);
			else if (c=='s')
				bmp = create_system_bitmap(width,height);
			else
				bmp = create_bitmap(width,height);
			no_destroy=false;
			use_dirty_rects=false;
		}*/
		// alBitmap Sub-bitmap constructor
		inline alBitmap(alBitmap &parent, int x, int y, int width, int height) {
			bmp = create_sub_bitmap(parent.bmp,x,y,width,height);
			no_destroy=false;
			use_dirty_rects=false;
		}
		// BITMAP* Sub-bitmap constructor
		inline alBitmap(BITMAP *parent, int x, int y, int width, int height) {
			bmp = create_sub_bitmap(parent,x,y,width,height);
			no_destroy=false;
			use_dirty_rects=false;
		}

	// deconstructor
		inline ~alBitmap() {
			destroy();
		}
	// cloning
		inline bool clone(BITMAP *nbmp) {
			destroy();
			if (is_video_bitmap(nbmp))
				bmp = create_video_bitmap(nbmp->w,nbmp->h);
			else if (is_system_bitmap(nbmp))
				bmp = create_system_bitmap(nbmp->w,nbmp->h);
			else
				bmp = create_bitmap_ex(bitmap_color_depth(nbmp),nbmp->w,nbmp->h);
			no_destroy=false;
			use_dirty_rects=false;
			if (bmp) {
				blit(nbmp,bmp,0,0,0,0,bmp->w,bmp->h);
				return true;
			}
			return false;
		}
		inline bool clone(const alBitmap& nbmp) {
			return clone(nbmp.bmp);
		}

	// creation methods
		// Creates a new blank bitmap
		inline bool create(int width, int height) {
			destroy();
			bmp = create_bitmap(width,height);
			return bmp != NULL;
		}

		// create_bitmap_ex creator
		inline bool create_ex(int width, int height, int depth) {
			destroy();
			bmp = create_bitmap_ex(width,height,depth);
			return bmp != NULL;
		}

		// create_video_bitmap creator
		inline bool create_video(char c, int width, int height) {
			destroy();
			bmp = create_video_bitmap(width,height);
			return bmp != NULL;
		}

		// create_system_bitmap creator
		inline bool create_system(char c, int width, int height) {
			destroy();
			bmp = create_system_bitmap(width,height);
			return bmp != NULL;
		}

		// alBitmap Sub-bitmap creator
		inline bool create_sub(alBitmap &parent, int x, int y, int width, int height) {
			destroy();
			bmp = create_sub_bitmap(parent.bmp,x,y,width,height);
			return bmp != NULL;
		}

		// BITMAP* Sub-bitmap creator
		inline bool create_sub(BITMAP *parent, int x, int y, int width, int height) {
			destroy();
			bmp = create_sub_bitmap(parent,x,y,width,height);
			return bmp != NULL;
		}

		// Load bitmap from a file
		inline bool load(const char *filename, RGB *pal = NULL) {
			destroy();
			bmp = load_bitmap(filename, pal);
			return bmp!=NULL;
		}

		// Copy bitmap from a BITMAP pointer
		inline alBitmap& operator= (BITMAP* newbmp) {
			clone(newbmp);
			return *this;
		}

		// Copy bitmap from another alBitmap
		inline alBitmap& operator= (const alBitmap& newbmp) {
			clone(newbmp);
			return *this;
		}

		// Clear color
		inline alBitmap& operator= (int color) {
			clear(color);
			return *this;
		}

		// Compares if two alBitmaps with is_same_bitmap()
		inline friend bool operator== (const alBitmap& bmp1, const alBitmap& bmp2) {
			return is_same_bitmap(bmp1.bmp,bmp2.bmp);
		}
		// Compares if two alBitmaps with is_same_bitmap()
		inline friend bool operator!= (const alBitmap& bmp1, const alBitmap& bmp2) {
			return !is_same_bitmap(bmp1.bmp,bmp2.bmp);
		}



		// Saves bitmap to file
		inline int save(const char *filename, const RGB *pal = NULL) {
			return save_bmp(filename, bmp, pal);
		}

		// Destroys current bitmap
		inline void destroy() {
			if (bmp&&!no_destroy) destroy_bitmap(bmp); // Oh no, a double negative!
			destroyDirtyRects();
		}

	// Clearing
		// Clears bitmap
		inline void clear() {
			clear_bitmap(bmp);
		}
		// Clears bitmap with color
		inline void clear(int color) {
			clear_to_color(bmp, color);
		}


	// Retrieving info
		inline bool isLinear() {
			return is_linear_bitmap(bmp);
		}
		inline bool isMemory() {
			return is_memory_bitmap(bmp);
		}
		inline bool isPlanar() {
			return is_planar_bitmap(bmp);
		}
		inline bool isScreen() {
			return is_screen_bitmap(bmp);
		}
		inline bool isSub() {
			return is_sub_bitmap(bmp);
		}
		inline bool isSystem() {
			return is_system_bitmap(bmp);
		}
		inline bool isVideo() {
			return is_video_bitmap(bmp);
		}
		inline int getMaskColor() {
			return bitmap_mask_color(bmp);
		}
		inline BITMAP* getBITMAP() {
			return bmp;
		}
		inline int getWidth() {
			return bmp->w;
		}
		inline int getHeight() {
			return bmp->h;
		}
		inline int getDepth() {
			return bitmap_color_depth(bmp);
		}
		inline bool isNoDestroy() {
			return no_destroy;
		}

	// Controlling Bitmap
		inline void acquire() {
			acquire_bitmap(bmp);
		}
		inline void release() {
			release_bitmap(bmp);
		}
		inline void lock() {
			lock_bitmap(bmp);
		}
		inline void setClip(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0) {
			set_clip_rect(bmp,x1,y1,x2,y2);
			set_clip_state(bmp,1);
		}
		inline void unsetClip() {
			set_clip_rect(bmp,0,0,bmp->w-1,bmp->h-1);
			set_clip_state(bmp,0);
		}

	// Primitives
		// Puts a pixel
		inline void setPixel(int x, int y, int c) {
			putpixel(bmp, x, y, c);

			markDirty(x,y);
		}
		// Gets a pixel
		inline int getPixel(int x, int y) {
			return getpixel(bmp, x, y);
		}



		inline void vLine(int x, int y1, int y2, int color) {
			vline(bmp, x, y1, y2, color);

			markDirty(x,y1,1,y2-y1);
		}

		inline void hLine(int x1, int y, int x2, int color) {
			hline(bmp, x1, y, x2, color);

			markDirty(x1,y,x2-x1,1);
		}

		inline void line(int x1, int y1, int x2, int y2, int color) {
			::line(bmp, x1, y1, x2, y2, color);

			markDirty(x1,y1,x2-x1,y2-y1);
		}

		inline void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) {
			::triangle(bmp, x1, y1, x2, y2, x3, y3, color);

			// TODO: Dirty rects
		}

		inline void polygon(int vertices, int *points, int color) {
			::polygon(bmp, vertices, points, color);

			// TODO: Dirty rects
		}

		inline void rect(int x1, int y1, int x2, int y2, int color) {
			::rect(bmp, x1, y1, x2, y2, color);

			markDirty(x1,y1,x2-x1,y2-y1);
		}

		inline void rectFill(int x1, int y1, int x2, int y2, int color) {
			rectfill(bmp, x1, y1, x2, y2, color);

			markDirty(x1,y1,x2-x1,y2-y1);
		}

		inline void circle(int x, int y, int radius, int color) {
			::circle(bmp, x, y, radius, color);

			markDirty(x-radius,y-radius,radius*2,radius*2);
		}

		inline void circleFill(int x, int y, int radius, int color) {
			circlefill(bmp, x, y, radius, color);

			markDirty(x-radius,y-radius,radius*2,radius*2);
		}

		inline void ellipse(int x, int y, int rx, int ry, int color) {
			::ellipse(bmp, x, y, rx, ry, color);

			markDirty(x-rx,y-ry,rx*2,ry*2);
		}

		inline void ellipseFill(int x, int y, int rx, int ry, int color) {
			ellipsefill(bmp, x, y, rx, ry, color);

			markDirty(x-rx,y-ry,rx*2,ry*2);
		}

		inline void arc(int x, int y, fixed ang1, fixed ang2, int r, int color) {
			::arc(bmp, x, y, ang1, ang2, r, color);

			markDirty(x-r,y-r,r*2,r*2);
		}

		inline void spline(int points[8], int color) {
			::spline(bmp, points, color);

			// TODO: Spline dirty rect.
		}

		inline void floodFill(int x, int y, int color) {
			floodfill(bmp, x, y, color);

			// Dirty rect for flood fill?  No clue.
		}

	// Text drawing

		inline void textout(const FONT *f, const char *s,  int x, int y, int color, int bg=-1) {
			::textout_ex(bmp, f, s,  x, y, color, bg);

			markDirty(x, y, text_length(f, s), text_height(f));
		}
		inline void textoutCenter(const FONT *f, const char *s, int x, int y, int color, int bg=-1) {
			::textout_centre_ex(bmp, f, s, x, y, color, bg);

			int w = text_length(f, s);
			markDirty(x-(w<<2), y, w, text_height(f));
		}
		inline void textoutRight(const FONT *f, const char *s, int x, int y, int color, int bg=-1) {
			::textout_right_ex(bmp, f, s, x, y, color, bg);

			int w = text_length(f, s);
			markDirty(x-w, y, w, text_height(f));
		}
		inline void textoutJustify(const FONT *f, const char *s, int x1, int x2, int y, int diff, int color, int bg=-1) {
			::textout_justify_ex(bmp, f, s, x1, x2, y, diff, color, bg);

			int w = text_length(f, s);
			if (w<x2-x1) w=x2-x1;
			markDirty(x1, y, w, text_height(f));
		}
// Wrapper for STL string objects
#ifdef __STRING__
		inline void textout(const FONT *f, string &s,  int x, int y, int color, int bg=-1) {
			::textout_ex(bmp, f, s.c_str(),  x, y, color, bg);

			markDirty(x, y, text_length(f, s.c_str()), text_height(f));
		}
		inline void textoutCenter(const FONT *f, string &s, int x, int y, int color, int bg=-1) {
			::textout_centre_ex(bmp, f, s.c_str(), x, y, color, bg);

			int w = text_length(f, buf);
			markDirty(x-(w<<2), y, w, text_height(f));
		}
		inline void textoutRight(const FONT *f, string &s, int x, int y, int color, int bg=-1) {
			::textout_right_ex(bmp, f, s.c_str(), x, y, color, bg);

			int w = text_length(f, s.c_str());
			markDirty(x-w, y, w, text_height(f));
		}
		inline void textoutJustify(const FONT *f, string &s, int x1, int x2, int y, int diff, int color, int bg=-1) {
			::textout_justify_ex(bmp, f, s.c_str(), x1, x2, y, diff, color, bg);

			int w = text_length(f, s.c_str());
			if (w<x2-x1) w=x2-x1;
			markDirty(x1, y, w, text_height(f));
		}
#endif
		inline void textprintf(AL_CONST FONT *f, int x, int y, int color, int bg, AL_CONST char *format, ...) {
			char buf[512];

			va_list ap;
			va_start(ap, format);
			uvszprintf(buf, sizeof(buf), format, ap);
			va_end(ap);

			::textout_ex(bmp, f, buf, x, y, color, bg);

			markDirty(x, y, text_length(f, buf), text_height(f));
		}
		inline void textprintfCenter(AL_CONST FONT *f, int x, int y, int color, int bg, AL_CONST char *format, ...) {
			char buf[512];

			va_list ap;
			va_start(ap, format);
			uvszprintf(buf, sizeof(buf), format, ap);
			va_end(ap);

			textout_centre_ex(bmp, f, buf, x, y, color, bg);

			int w = text_length(f, buf);
			markDirty(x-(w<<2), y, w, text_height(f));
		}

		inline void textprintfRight(AL_CONST FONT *f, int x, int y, int color, int bg, AL_CONST char *format, ...) {
			char buf[512];

			va_list ap;
			va_start(ap, format);
			uvszprintf(buf, sizeof(buf), format, ap);
			va_end(ap);

			textout_right_ex(bmp, f, buf, x, y, color, bg);

			int w = text_length(f, buf);
			markDirty(x-w, y, w, text_height(f));
		}

		inline void textprintfJustify(AL_CONST FONT *f, int x1, int x2, int y, int diff, int color, int bg, AL_CONST char *format, ...) {
			char buf[512];

			va_list ap;
			va_start(ap, format);
			uvszprintf(buf, sizeof(buf), format, ap);
			va_end(ap);

			textout_justify_ex(bmp, f, buf, x1, x2, y, diff, color, bg);

			int w = text_length(f, buf);
			if (w<x2-x1) w=x2-x1;
			markDirty(x1, y, w, text_height(f));
		}

	// Polygon drawing

		inline void polygon3d(int type, BITMAP *texture, int vc, V3D *vtx[]) {
			::polygon3d(bmp, type, texture, vc, vtx);

			//TODO: Add dirty rectangle code
		}
		inline void polygon3d(int type, BITMAP *texture, int vc, V3D_f *vtx[]) {
			::polygon3d_f(bmp, type, texture, vc, vtx);

			//TODO: Add dirty rectangle code
		}

		inline void triangle3d(int type, BITMAP *tex, V3D *v1, V3D *v2, V3D *v3) {
			::triangle3d(bmp, type, tex, v1, v2, v3);

			//TODO: Add dirty rectangle code
		}
		inline void triangle3d(int type, BITMAP *tex, V3D_f *v1, V3D_f *v2, V3D_f *v3) {
			::triangle3d_f(bmp, type, tex, v1, v2, v3);

			//TODO: Add dirty rectangle code
		}

		inline void quad3d(int type, BITMAP *tex, V3D *v1, V3D *v2, V3D *v3, V3D *v4) {
			::quad3d(bmp, type, tex, v1, v2, v3, v4);

			//TODO: Add dirty rectangle code
		}
		inline void quad3d(int type, BITMAP *tex, V3D_f *v1, V3D_f *v2, V3D_f *v3, V3D_f *v4) {
			::quad3d_f(bmp, type, tex, v1, v2, v3, v4);

			//TODO: Add dirty rectangle code
		}


	// Blitting/Drawing To functions
	//alBitmap

		// Blitting functions
		inline void blitTo(alBitmap& dest, int x = 0, int y = 0, int source_x = 0, int source_y = 0) {
			blit(bmp, dest.bmp, source_x, source_y, x, y, bmp->w, bmp->h);

			dest.markDirty(x,y, getWidth(), getHeight());
		}
		inline void blitTo(alBitmap& dest, int x, int y, int source_x, int source_y, int width, int height) {
			blit(bmp, dest.bmp, source_x, source_y, x, y, width, height);

			dest.markDirty(x,y,width, height);
		}

		// Masked blitting functions
		inline void drawTo(alBitmap& dest, int x = 0, int y = 0, int source_x = 0, int source_y = 0) {
			masked_blit(bmp, dest.bmp, source_x, source_y, x, y, bmp->w, bmp->h);

			dest.markDirty(x,y,getWidth(), getHeight());
		}
		inline void drawTo(alBitmap& dest, int x, int y, int source_x, int source_y, int width, int height) {
			masked_blit(bmp, dest.bmp, source_x, source_y, x, y, width, height);

			dest.markDirty(x,y,width, height);
		}

		// Stretched blitting functions
		inline void stretchBlitTo(alBitmap& dest, int x, int y, int dest_width, int dest_height) {
			stretch_blit(bmp, dest.bmp, 0, 0, bmp->w, bmp->h, x, y, dest_width, dest_height);

			dest.markDirty(x,y,dest_width, dest_height);
		}
		inline void stretchBlitTo(alBitmap& dest, int x, int y, int source_x, int source_y, int source_width, int source_height, int dest_width, int dest_height) {
			stretch_blit(bmp, dest.bmp, source_x, source_y, source_width, source_height, x, y, dest_width, dest_height);

			dest.markDirty(x,y,dest_width, dest_height);
		}
		inline void stretchDrawTo(alBitmap& dest, int x, int y, int dest_width, int dest_height) {
			masked_stretch_blit(bmp, dest.bmp, 0, 0, bmp->w, bmp->h, x, y, dest_width, dest_height);

			dest.markDirty(x,y,dest_width, dest_height);
		}
		inline void stretchDrawTo(alBitmap& dest, int x, int y, int source_x, int source_y, int source_width, int source_height, int dest_width, int dest_height) {
			masked_stretch_blit(bmp, dest.bmp, source_x, source_y, source_width, source_height, x, y, dest_width, dest_height);

			dest.markDirty(x,y,dest_width, dest_height);
		}
	//BITMAP*
		// Blitting functions
		inline void blitTo(BITMAP* dest, int x = 0, int y = 0, int source_x = 0, int source_y = 0) {
			blit(bmp, dest, source_x, source_y, x, y, bmp->w, bmp->h);
		}
		inline void blitTo(BITMAP* dest, int x, int y, int source_x, int source_y, int width, int height) {
			blit(bmp, dest, source_x, source_y, x, y, width, height);
		}

		// Masked blitting functions
		inline void drawTo(BITMAP* dest, int x = 0, int y = 0, int source_x = 0, int source_y = 0) {
			masked_blit(bmp, dest, source_x, source_y, x, y, bmp->w, bmp->h);
		}
		inline void drawTo(BITMAP* dest, int x, int y, int source_x, int source_y, int width, int height) {
			masked_blit(bmp, dest, source_x, source_y, x, y, width, height);
		}

		// Masked blitting functions
		inline void draw(BITMAP* src, int x, int y, int source_x, int source_y, int width, int height) {
			masked_blit(src, bmp, source_x, source_y, x, y, width, height);
		}

		// Stretched blitting functions
		inline void stretchBlitTo(BITMAP* dest, int x, int y, int dest_width, int dest_height) {
			stretch_blit(bmp, dest, 0, 0, bmp->w, bmp->h, x, y, dest_width, dest_height);
		}
		inline void stretchBlitTo(BITMAP* dest, int x, int y, int source_x, int source_y, int source_width, int source_height, int dest_width, int dest_height) {
			stretch_blit(bmp, dest, source_x, source_y, source_width, source_height, x, y, dest_width, dest_height);
		}
		inline void stretchDrawTo(BITMAP* dest, int x, int y, int dest_width, int dest_height) {
			masked_stretch_blit(bmp, dest, 0, 0, bmp->w, bmp->h, x, y, dest_width, dest_height);
		}
		inline void stretchDrawTo(BITMAP* dest, int x, int y, int source_x, int source_y, int source_width, int source_height, int dest_width, int dest_height) {
			masked_stretch_blit(bmp, dest, source_x, source_y, source_width, source_height, x, y, dest_width, dest_height);
		}

	// Blended sprite drawing
		// alBitmap
		inline void drawTransTo(alBitmap& dest, int x = 0, int y = 0) {
			draw_trans_sprite(dest.bmp, bmp, x, y);

			dest.markDirty(x,y,getWidth(), getHeight());
		}
		inline void drawLitTo(alBitmap& dest, int x, int y, int color) {
			draw_lit_sprite(dest.bmp, bmp, x, y, color);

			dest.markDirty(x,y,getWidth(), getHeight());
		}
		inline void drawGouraudTo(alBitmap& dest, int x, int y, int c1, int c2, int c3, int c4) {
			draw_gouraud_sprite(dest.bmp, bmp, x, y, c1, c2, c3, c4);

			dest.markDirty(x,y,getWidth(), getHeight());
		}
		// BITMAP*
		inline void drawTransTo(BITMAP* dest, int x, int y) {
			draw_trans_sprite(dest, bmp, x, y);
		}
		inline void drawLitTo(BITMAP* dest, int x, int y, int color) {
			draw_lit_sprite(dest, bmp, x, y, color);
		}
		inline void drawGouraudTo(BITMAP* dest, int x, int y, int c1, int c2, int c3, int c4) {
			draw_gouraud_sprite(dest, bmp, x, y, c1, c2, c3, c4);
		}

	// Blended sprite drawing
		inline void draw(alBitmap& sprite, int x = 0, int y = 0) {
			draw_sprite(bmp, sprite.bmp, x, y);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawTrans(alBitmap& sprite, int x = 0, int y = 0) {
			draw_trans_sprite(bmp, sprite.bmp, x, y);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawLit(alBitmap& sprite, int x, int y, int color) {
			draw_lit_sprite(bmp, sprite.bmp, x, y, color);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawGouraud(alBitmap& sprite, int x, int y, int c1, int c2, int c3, int c4) {
			draw_gouraud_sprite(bmp, sprite.bmp, x, y, c1, c2, c3, c4);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void draw(BITMAP* sprite, int x = 0, int y = 0) {
			draw_sprite(bmp, sprite, x, y);

			markDirty(x,y,sprite->w, sprite->h);
		}
		inline void drawTrans(BITMAP* sprite, int x = 0, int y = 0) {
			draw_trans_sprite(bmp, sprite, x, y);

			markDirty(x,y,sprite->w, sprite->h);
		}
		inline void drawLit(BITMAP* sprite, int x, int y, int color) {
			draw_lit_sprite(bmp, sprite, x, y, color);

			markDirty(x,y,sprite->w, sprite->h);
		}
		inline void drawGouraud(BITMAP* sprite, int x, int y, int c1, int c2, int c3, int c4) {
			draw_gouraud_sprite(bmp, sprite, x, y, c1, c2, c3, c4);

			markDirty(x,y,sprite->w, sprite->h);
		}
#ifdef FBLEND_VERSION_STRING

		inline void drawTrans(alBitmap& sprite, int x, int y, int fact) {
			fblend_trans(sprite.bmp, bmp, x, y, fact);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawAdd(alBitmap& sprite, int x, int y, int fact) {
			fblend_add(sprite.bmp, bmp, x, y, fact);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawTrans(BITMAP* sprite, int x, int y, int fact) {
			fblend_trans(sprite, bmp, x, y, fact);

			markDirty(x,y,sprite->w, sprite->h);
		}
		inline void drawAdd(BITMAP* sprite, int x, int y, int fact) {
			fblend_add(sprite, bmp, x, y, fact);

			markDirty(x,y,sprite->w, sprite->h);
		}
#else // FBLEND trans/add wrappers
		inline void drawTrans(alBitmap& sprite, int x, int y, int fact) {
			set_trans_blender(0,0,0,fact);
			draw_trans_sprite(bmp, sprite.bmp, x, y);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawAdd(alBitmap& sprite, int x, int y, int fact) {
			set_add_blender(0,0,0,fact);
			draw_trans_sprite(bmp, sprite.bmp, x, y);

			markDirty(x,y,sprite.getWidth(), sprite.getHeight());
		}
		inline void drawTrans(BITMAP* sprite, int x, int y, int fact) {
			set_trans_blender(0,0,0,fact);
			draw_trans_sprite(bmp, sprite, x, y);

			markDirty(x,y,sprite->w, sprite->h);
		}
		inline void drawAdd(BITMAP* sprite, int x, int y, int fact) {
			set_add_blender(0,0,0,fact);
			draw_trans_sprite(bmp, sprite, x, y);

			markDirty(x,y,sprite->w, sprite->h);
		}
#endif

#ifndef ALBITMAP_NO_DIRTY_RECTS
	// Dirty Rectangles

		inline void destroyDirtyRects() {
			if (!use_dirty_rects) return;
			if (gridArray)
				delete [] gridArray;
		}
		inline void updateScreen() {
			acquire_screen();
			if (updateAll) {
				blitTo(screen, 0, 0);
				updateAll = false;
			} else {
				int sx,sy=0,c=0,j=0;
				for (int y=0; y<gridRectY; y++, sy+=gridCellHeight) {
					sx=0;
					for (int x=0; x<gridRectX; x++, sx+=gridCellWidth) {
						if (gridArray[x + y * gridRectX] >= 1) {
							blitTo(screen, sx,sy, sx,sy, gridCellWidth,gridCellHeight);
							c++;
						}
						j++;
					}
				}
			}
			release_screen();
		}
		inline void markDirty() {
			updateAll = true;
		}
		inline void markDirty(int x, int y) {
			if (!use_dirty_rects) return;

			if (updateAll) return;

			int x1=MIN(MAX((x/gridCellWidth),0),gridRectX-1);
			int y1=MIN(MAX((y/gridCellHeight),0),gridRectY-1);

			gridArray[x1 + y1 * gridRectX] = 2;
		}
		inline void markDirty(int x, int y, int w, int h) {
			if (!use_dirty_rects) return;
			if (updateAll) return;

			if (w<0) {
				x += w;
				w = -w;
			}
			if (h<0) {
				y += h;
				h = -h;
			}

			int x1=MAX((x/gridCellWidth),0);
			int y1=MAX((y/gridCellHeight),0);
			int x2=MIN(x1+(w/gridCellWidth)+1,gridRectX-1);
			int y2=MIN(y1+(h/gridCellHeight)+1,gridRectY-1);
			for (int y=y1; y<=y2; y++)
				for (int x=x1; x<=x2; x++)
					gridArray[x + y * gridRectX] = 2;
		}

		inline void refreshRectangles(bool clearOld) {
			if (!use_dirty_rects) return;
			use_dirty_rects=false;
			int sx,sy=0;
			for (int y=0; y<gridRectY; y++,sy += gridCellHeight) {
				sx=0;
				for (int x=0; x<gridRectX; x++,sx += gridCellWidth)
					if (gridArray[x + y * gridRectX] > 0) {
						gridArray[x + y * gridRectX]--;
						if (clearOld)
							rectFill(sx,sy,sx+gridCellWidth,sy+gridCellHeight,0);
					}
			}
			use_dirty_rects=true;
		}

		// Enabling
		inline void useDirtyRects(bool b) {
			if (use_dirty_rects == b) return;
			if (use_dirty_rects = b) {

				// Initialize
				gridCellWidth = 10;
				gridCellHeight = 10;

				gridRectX = getWidth() / gridCellWidth;
				gridRectY = getHeight() / gridCellHeight;

				gridArray = new unsigned short[gridRectX * gridRectY];
				for (int i=0; i<gridRectX*gridRectY; i++)
					gridArray[i] = 0;

			} else {
				destroyDirtyRects();
			}

		}
#else
		inline void markDirty() { }
		inline void markDirty(int x, int y) { }
		inline void markDirty(int x, int y, int w, int h) { }
#endif


};

#endif
