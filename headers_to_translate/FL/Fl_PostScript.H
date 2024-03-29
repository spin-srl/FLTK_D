//
// Support for graphics output to PostScript file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2020 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

/** \file Fl_PostScript.H
 \brief declaration of classes Fl_PostScript_Graphics_Driver, Fl_PostScript_File_Device.
 */

#ifndef Fl_PostScript_H
#define Fl_PostScript_H

#include <FL/Fl_Paged_Device.H>
#include <FL/fl_draw.H>
#include <stdarg.h>

/* Signature of Fl_PostScript::close_command() functions passed as parameters. */
extern "C" {
  typedef int (Fl_PostScript_Close_Command)(FILE*);
}

/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */

/**
 PostScript graphical backend.
 */
class Fl_PostScript_Graphics_Driver : public Fl_Graphics_Driver {
private:
  void transformed_draw_extra(const char* str, int n, double x, double y, int w, bool rtl);
  void* prepare_rle85();
  void write_rle85(uchar b, void* data);
  void close_rle85(void* data);
  void* prepare85();
  void write85(void* data, const uchar* p, int len);
  void close85(void* data);
  int scale_for_image_(Fl_Image* img, int XP, int YP, int WP, int HP, int cx, int cy);
protected:
  uchar** mask_bitmap() {return &mask;}
public:
  Fl_PostScript_Graphics_Driver();
#ifndef FL_DOXYGEN
  enum SHAPE {NONE = 0, LINE, LOOP, POLYGON, POINTS};

  class Clip {
  public:
    int x, y, w, h;
    Clip* prev;
  };
  Clip* clip_;

  int lang_level_;
  int gap_;
  int pages_;

  double width_;
  double height_;

  int shape_;
  int linewidth_;// need for clipping, lang level 1-2
  int linestyle_;//
  int interpolate_; //interpolation of images
  unsigned char cr_, cg_, cb_;
  char  linedash_[256];//should be enough
  void concat();  // transform ror scalable dradings...
  void reconcat(); //invert
  void recover(); //recovers the state after grestore (such as line styles...)
  void reset();

  uchar* mask;
  int mx; // width of mask;
  int my; // mask lines
  //Fl_Color bg_;
  Fl_PostScript_Close_Command* close_cmd_;
  int page_policy_;
  int nPages;
  int orientation_;

  float scale_x;
  float scale_y;
  float angle;
  int left_margin;
  int top_margin;

  FILE* output;
  double pw_, ph_;

  uchar bg_r, bg_g, bg_b;
  int start_postscript(int pagecount, enum Fl_Paged_Device::Page_Format format, enum Fl_Paged_Device::Page_Layout layout);
  /*  int alpha_mask(const uchar * data, int w, int h, int D, int LD=0);
   */
  void transformed_draw(const char* s, int n, double x, double y); //precise text placing
  void transformed_draw(const char* s, double x, double y);
  int alpha_mask(const uchar* data, int w, int h, int D, int LD = 0);

  enum Fl_Paged_Device::Page_Format page_format_;
  char* ps_filename_;

  void page_policy(int p);
  int page_policy() {return page_policy_;};
  void close_command(Fl_PostScript_Close_Command* cmd) {close_cmd_ = cmd;};
  FILE* file() {return output;};
  //void orientation (int o);
  //Fl_PostScript_Graphics_Driver(FILE *o, int lang_level, int pages = 0); // ps (also multi-page) constructor
  //Fl_PostScript_Graphics_Driver(FILE *o, int lang_level, int x, int y, int w, int h); //eps constructor
  void interpolate(int i) {interpolate_ = i;};
  int interpolate() {return interpolate_;}

  void page(double pw, double ph, int media = 0);
  void page(int format);
#endif // FL_DOXYGEN

  // implementation of drawing methods
  void color(Fl_Color c);
  void color(uchar r, uchar g, uchar b);

  void push_clip(int x, int y, int w, int h);
  int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
  int not_clipped(int x, int y, int w, int h);
  void push_no_clip();
  void pop_clip();

  void line_style(int style, int width = 0, char* dashes = 0);

  void rect(int x, int y, int w, int h);
  void rectf(int x, int y, int w, int h);

  void xyline(int x, int y, int x1);
  void xyline(int x, int y, int x1, int y2);
  void xyline(int x, int y, int x1, int y2, int x3);

  void yxline(int x, int y, int y1);
  void yxline(int x, int y, int y1, int x2);
  void yxline(int x, int y, int y1, int x2, int y3);

  void line(int x1, int y1, int x2, int y2);
  void line(int x1, int y1, int x2, int y2, int x3, int y3);

  void loop(int x0, int y0, int x1, int y1, int x2, int y2);
  void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
  void polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  void point(int x, int y);

  void begin_points();
  void begin_line();
  void begin_loop();
  void begin_polygon();
  void vertex(double x, double y);
  void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);
  void circle(double x, double y, double r);
  void arc(double x, double y, double r, double start, double a);
  void arc(int x, int y, int w, int h, double a1, double a2);
  void pie(int x, int y, int w, int h, double a1, double a2);
  void end_points();
  void end_line();
  void end_loop();
  void end_polygon();
  void begin_complex_polygon() {begin_polygon();};
  void gap() {gap_ = 1;};
  void end_complex_polygon() {end_polygon();};
  void transformed_vertex(double x, double y);

  void draw_image(const uchar* d, int x, int y, int w, int h, int delta = 3, int ldelta = 0);
  void draw_image_mono(const uchar* d, int x, int y, int w, int h, int delta = 1, int ld = 0);
  void draw_image(Fl_Draw_Image_Cb call, void* data, int x, int y, int w, int h, int delta = 3);
  void draw_image_mono(Fl_Draw_Image_Cb call, void* data, int x, int y, int w, int h, int delta = 1);

  void draw(const char* s, int nBytes, int x, int y) {transformed_draw(s, nBytes, x, y); };
  void draw(const char* s, int nBytes, float x, float y) {transformed_draw(s, nBytes, x, y); };
  void draw(int angle, const char* str, int n, int x, int y);
  void rtl_draw(const char* s, int n, int x, int y);
  void font(int face, int size);
  double width(const char*, int);
  double width(unsigned int u);
  void text_extents(const char* c, int n, int &dx, int &dy, int &w, int &h);
  int height();
  int descent();
  void draw_pixmap(Fl_Pixmap* pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_bitmap(Fl_Bitmap* bitmap, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_rgb(Fl_RGB_Image* rgb, int XP, int YP, int WP, int HP, int cx, int cy);
  /** Shields output PostScript data from modifications of the current locale.
   It typically avoids PostScript errors caused if the current locale uses comma instead of dot
   as "decimal point".
   \param format  directives controlling output PostScript data
   \return value returned by vfprintf() call
   */
  int clocale_printf(const char* format, ...);
  ~Fl_PostScript_Graphics_Driver();
  // ---
  Fl_Bitmask create_bitmask(int w, int h, const uchar* array) { return 0L; }
  virtual int has_feature(driver_feature feature_mask) { return feature_mask & PRINTER; }

  int start_eps(int width, int height);
  void ps_origin(int x, int y);
  void ps_translate(int, int);
  void ps_untranslate();
};

/**
 \}
 \endcond
 */

/**
 To send graphical output to a PostScript file.
 This class is used exactly as the Fl_Printer class except for the begin_job() call,
 two variants of which are usable and allow to specify what page format and layout are desired.

 PostScript text uses vectorial fonts when using the FLTK standard fonts
 and the latin alphabet or a few other characters listed in the following table.
 The latin alphabet means all unicode characters between U+0020 and U+017F, or, in other words,
 the ASCII, Latin-1 Supplement and Latin Extended-A charts.
 <table>
 <tr> <th>Char</th><th>Codepoint</th><th>Name</th>  <th>Char</th><th>Codepoint</th><th>Name</th> <th>Char</th><th>Codepoint</th><th>Name</th></tr>
 <tr><td>ƒ</td><td>U+0192</td><td>florin</td><td>‚</td><td>U+201A</td><td>quotesinglbase</td><td>™</td><td>U+2122</td><td>trademark</td></tr>
 <tr><td>ˆ</td><td>U+02C6</td><td>circumflex</td><td>“</td><td>U+201C</td><td>quotedblleft</td><td>∂</td><td>U+2202</td><td>partialdiff</td></tr>
 <tr><td>ˇ</td><td>U+02C7</td><td>caron</td><td>”</td><td>U+201D</td><td>quotedblright</td><td>Δ</td><td>U+2206</td><td>Delta</td></tr>
 <tr><td>˘</td><td>U+02D8</td><td>breve</td><td>„</td><td>U+201E</td><td>quotedblbase</td><td>∑</td><td>U+2211</td><td>summation</td></tr>
 <tr><td>˙</td><td>U+02D9</td><td>dotaccent</td><td>†</td><td>U+2020</td><td>dagger</td><td>√</td><td>U+221A</td><td>radical</td></tr>
 <tr><td>˚</td><td>U+02DA</td><td>ring</td><td>‡</td><td>U+2021</td><td>daggerdbl</td><td>∞</td><td>U+221E</td><td>infinity</td></tr>
 <tr><td>˛</td><td>U+02DB</td><td>ogonek</td><td>•</td><td>U+2022</td><td>bullet</td><td>≠</td><td>U+2260</td><td>notequal</td></tr>
 <tr><td>˜</td><td>U+02DC</td><td>tilde</td><td>…</td><td>U+2026</td><td>ellipsis</td><td>≤</td><td>U+2264</td><td>lessequal</td></tr>
 <tr><td>˝</td><td>U+02DD</td><td>hungarumlaut</td><td>‰</td><td>U+2030</td><td>perthousand</td><td>≥</td><td>U+2265</td><td>greaterequal</td></tr>
 <tr><td>–</td><td>U+2013</td><td>endash</td><td>‹</td><td>U+2039</td><td>guilsinglleft</td><td>◊</td><td>U+25CA</td><td>lozenge</td></tr>
 <tr><td>—</td><td>U+2014</td><td>emdash</td><td>›</td><td>U+203A</td><td>guilsinglright</td><td>ﬁ</td><td>U+FB01</td><td>fi</td></tr>
 <tr><td>‘</td><td>U+2018</td><td>quoteleft</td><td>/</td><td>U+2044</td><td>fraction</td><td>ﬂ</td><td>U+FB02</td><td>fl</td></tr>
 <tr><td>’</td><td>U+2019</td><td>quoteright</td><td>€</td><td>U+20AC</td><td>Euro</td><td></td><td>U+F8FF</td><td>apple (Mac OS only)</td></tr>
 </table>
 <br> All other unicode characters or all other fonts (FL_FREE_FONT and above) are output as a bitmap.
 <br> FLTK standard fonts are output using the corresponding PostScript standard fonts.

 */
class Fl_PostScript_File_Device : public Fl_Paged_Device {
protected:
  /**
   \brief Returns the PostScript driver of this drawing surface.
   */
  Fl_PostScript_Graphics_Driver* driver();
public:
  /**
   @brief The constructor.
   */
  Fl_PostScript_File_Device();
  /**
   @brief The destructor.
   */
  ~Fl_PostScript_File_Device();
  /** Don't use with this class. */
  int begin_job(int pagecount, int* from, int* to, char** perr_message);
  /**
   @brief Begins the session where all graphics requests will go to a local PostScript file.
   *
   Opens a file dialog entitled with Fl_PostScript_File_Device::file_chooser_title to select an output PostScript file.
   @param pagecount The total number of pages to be created. Use 0 if this number is unknown when this function is called.
   @param format Desired page format.
   @param layout Desired page layout.
   @return 0 if OK, 1 if user cancelled the file dialog, 2 if fopen failed on user-selected output file.
   */
  int begin_job(int pagecount = 0, enum Fl_Paged_Device::Page_Format format = Fl_Paged_Device::A4,
                enum Fl_Paged_Device::Page_Layout layout = Fl_Paged_Device::PORTRAIT);
  /** Synonym of begin_job().
   For API compatibility with FLTK 1.3.x */
  int start_job(int pagecount = 0, enum Fl_Paged_Device::Page_Format format = Fl_Paged_Device::A4,
                enum Fl_Paged_Device::Page_Layout layout = Fl_Paged_Device::PORTRAIT) {
    return begin_job(pagecount, format, layout);
  }
  /**
   @brief Begins the session where all graphics requests will go to FILE pointer.
   *
   @param ps_output A writable FILE pointer that will receive PostScript output and that should not be closed
   until after end_job() has been called.
   @param pagecount The total number of pages to be created. Use 0 if this number is unknown when this function is called.
   @param format Desired page format.
   @param layout Desired page layout.
   @return always 0.
   */
  int begin_job(FILE* ps_output, int pagecount = 0, enum Fl_Paged_Device::Page_Format format = Fl_Paged_Device::A4,
                enum Fl_Paged_Device::Page_Layout layout = Fl_Paged_Device::PORTRAIT);
  /** Synonym of begin_job().
   For API compatibility with FLTK 1.3.x */
  int start_job(FILE* ps_output, int pagecount = 0, enum Fl_Paged_Device::Page_Format format = Fl_Paged_Device::A4,
                enum Fl_Paged_Device::Page_Layout layout = Fl_Paged_Device::PORTRAIT) {
    return begin_job(ps_output, pagecount, format, layout);
  }

  int begin_page(void);
  int printable_rect(int* w, int* h);
  void margins(int* left, int* top, int* right, int* bottom);
  void origin(int* x, int* y);
  void origin(int x, int y);
  void scale(float scale_x, float scale_y = 0.);
  void rotate(float angle);
  void translate(int x, int y);
  void untranslate(void);
  int end_page(void);
  void end_job(void);
  /** \brief Label of the PostScript file chooser window */
  static const char* file_chooser_title;
  /** Returns the underlying FILE* receiving all PostScript data */
  FILE* file() { return driver()->file(); }
};

/** Encapsulated PostScript drawing surface.
 This drawing surface allows to store any FLTK graphics in vectorial form in an "Encapsulated PostScript" file.
 \n Usage example:
 \code
   Fl_Window *win = ...// Window to draw to an .eps file
   int ww = win->decorated_w();
   int wh = win->decorated_h();
   FILE *eps = fl_fopen("/path/to/mywindow.eps", "w");
   if (eps) {
     Fl_EPS_File_Surface *surface = new Fl_EPS_File_Surface(ww, wh, eps, win->color());
     Fl_Surface_Device::push_current(surface);
     surface->draw_decorated_window(win);
     Fl_Surface_Device::pop_current();
     delete surface; // the .eps file is not complete until the destructor was run
     fclose(eps);
   }
 \endcode
 */
class Fl_EPS_File_Surface : public Fl_Widget_Surface {
private:
  void complete_();
protected:
  /** Returns the PostScript driver of this drawing surface. */
  inline Fl_PostScript_Graphics_Driver* driver() { return (Fl_PostScript_Graphics_Driver*)Fl_Surface_Device::driver(); }
public:
  /**
  Constructor.
  \param width,height Width and height of the EPS drawing area
  \param eps A writable FILE pointer where the Encapsulated PostScript data will be sent
  \param background Color expected to cover the background of the EPS drawing area.
  This parameter affects only the drawing of transparent Fl_RGB_Image objects:
  transparent areas of RGB images are blended with the \p background color.
  */
  Fl_EPS_File_Surface(int width, int height, FILE* eps, Fl_Color background = FL_WHITE);
  /**
   Destructor.
   The underlying FILE pointer remains open after destruction of the Fl_EPS_File_Surface object
   unless close() was called.
   */
  ~Fl_EPS_File_Surface();
  virtual int printable_rect(int* w, int* h);
  /** Returns the underlying FILE pointer */
  FILE* file() { return driver() ? driver()->output : NULL; }
  virtual void origin(int x, int y);
  virtual void origin(int* px, int* py);
  virtual void translate(int x, int y);
  virtual void untranslate();
  /** Closes using fclose() the underlying FILE pointer.
   The only operation possible with the Fl_EPS_File_Surface object after calling close() is its destruction. */
  int close();
};

#endif // Fl_PostScript_H
