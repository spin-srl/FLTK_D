//
// FLTK native OS file chooser widget
//
// Copyright 1998-2016 by Bill Spitzak and others.
// Copyright 2004 Greg Ercolano.
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

/** \file
   Fl_Native_File_Chooser widget. */

/* Implementation note:

 class Fl_Native_File_Chooser  <== public API used by applications

 class Fl_Native_File_Chooser_Driver   <== virtual API that a platform may implement
                                           this API has a do-nothing default implementation

        class Fl_Native_File_Chooser_FLTK_Driver   <== this API implementation is the default FLTK file chooser
                class Fl_GTK_Native_File_Chooser_Driver  <== this API implementation runs a GTK file chooser
                                        it is determined at run-time if the GTK dynamic libraries are available

        class Fl_Quartz_Native_File_Chooser_Driver  <== this API implementation runs a Mac OS X file chooser

        class Fl_WinAPI_Native_File_Chooser_Driver  <== this API implementation runs a Windows file chooser


 Each platform must implement the constructor of the Fl_Native_File_Chooser class.
 This particular implementation:

 Fl_Native_File_Chooser::Fl_Native_File_Chooser(int val) {
  platform_fnfc = new Fl_Native_File_Chooser_FLTK_Driver(val);
 }

 can be used by any platform.
 No more code is required. The cross-platform Fl_Native_File_Chooser_FLTK.cxx file must be compiled in libfltk,
 and the default FLTK file chooser will be used.

 This other implementation:
 Fl_Native_File_Chooser::Fl_Native_File_Chooser(int val) {
   platform_fnfc = 0;
 }
 can be used by a platform that needs no file chooser.
 */

#ifndef FL_NATIVE_FILE_CHOOSER_H
#define FL_NATIVE_FILE_CHOOSER_H

#include <FL/ .H>
#include <FL/Fl_File_Chooser.H>

class Fl_Native_File_Chooser_Driver;

/**
 This class lets an FLTK application easily and consistently access
 the operating system's native file chooser. Some operating systems
 have very complex and specific file choosers that many users want
 access to specifically, instead of FLTK's default file chooser(s).

 In cases where there is no native file browser, FLTK's own file browser
 is used instead.

 To use this widget, use the following include in your code:
 \code
 #include <FL/Fl_Native_File_Chooser.H>
 \endcode

 The following example shows how to pick a single file:
 \code
 // Create and post the local native file chooser
 #include <FL/Fl_Native_File_Chooser.H>
 [..]
 Fl_Native_File_Chooser fnfc;
 fnfc.title("Pick a file");
 fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
 fnfc.filter("Text\t*.txt\n"
             "C Files\t*.{cxx,h,c}");
 fnfc.directory("/var/tmp");           // default directory to use
 // Show native chooser
 switch ( fnfc.show() ) {
   case -1: printf("ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
   case  1: printf("CANCEL\n");                      break;  // CANCEL
   default: printf("PICKED: %s\n", fnfc.filename()); break;  // FILE CHOSEN
 }
 \endcode

 The Fl_Native_File_Chooser widget transmits UTF-8 encoded filenames to its user. It is
 recommended to open files that may have non-ASCII names with the fl_fopen() or
 fl_open() utility functions that handle these names in a cross-platform way
 (whereas the standard fopen()/open() functions fail on the Windows platform
 to open files with a non-ASCII name).

 <B>Platform Specific Caveats</B>

 - Under X windows, and if Fl::OPTION_FNFC_USES_GTK has not been switched off,
 the widget attempts to use standard GTK file chooser dialogs if they are
 available at run-time on the platform, and falls back to use FLTK's Fl_File_Chooser if they are not.
 In the first case, calling fl_register_images() adds  a "Preview" button to the GTK chooser dialog.
 In the latter case, it's best if you call Fl_File_Icon::load_system_icons()
 at the start of main(), to enable the nicer looking file browser widgets.
 Use the static public attributes of class Fl_File_Chooser to localize
 the browser.
 - Some operating systems support certain OS specific options; see
 Fl_Native_File_Chooser::options() for a list.

 \image html Fl_Native_File_Chooser.png "The Fl_Native_File_Chooser on different platforms."
 \image latex Fl_Native_File_Chooser.png "The Fl_Native_File_Chooser on different platforms" width=14cm

 */
class Fl_Native_File_Chooser {
// private:
  // Fl_Native_File_Chooser_Driver* platform_fnfc;
public:
  enum Type {
    BROWSE_FILE = 0,                    ///< browse files (lets user choose one file)
    BROWSE_DIRECTORY,                   ///< browse directories (lets user choose one directory)
    BROWSE_MULTI_FILE,                  ///< browse files (lets user choose multiple files)
    BROWSE_MULTI_DIRECTORY,             ///< browse directories (lets user choose multiple directories)
    BROWSE_SAVE_FILE,                   ///< browse to save a file
    BROWSE_SAVE_DIRECTORY               ///< browse to save a directory
  };
  enum Option {
    NO_OPTIONS     = 0x0000,            ///< no options enabled
    SAVEAS_CONFIRM = 0x0001,            ///< Show native 'Save As' overwrite confirm dialog
    NEW_FOLDER     = 0x0002,            ///< Show 'New Folder' icon (if supported)
    PREVIEW        = 0x0004,            ///< enable preview mode (if supported)
    USE_FILTER_EXT = 0x0008             ///< Chooser filter pilots the output file extension (if supported)
  };
  static const char* file_exists_message;

  /**
   The constructor. Internally allocates the native widgets.
   Optional \p val presets the type of browser this will be,
   which can also be changed with type().
   */
  Fl_Native_File_Chooser(int val = BROWSE_FILE); // each platorm implements it
  ~Fl_Native_File_Chooser();
  void type(int t);
  int type() const ;
  void options(int o);
  int options() const;
  int count() const;
  const char* filename() const ;
  const char* filename(int i) const ;
  void directory(const char* val) ;
  const char* directory() const;
  void title(const char* t);
  const char* title() const;
  const char* filter() const ;
  void filter(const char* f);
  int filters() const ;
  void filter_value(int i) ;
  int filter_value() const ;
  void preset_file(const char* f) ;
  const char* preset_file() const;
  const char* errmsg() const ;
  int show() ;
};

/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */

/** Represents the interface between FLTK and a native file chooser.
 This class is only for internal use by the FLTK library.
 A platform that wants to provide a native file chooser implements all virtual methods
 of this class. Each platform supported by FLTK must also implement the constructor of the
 Fl_Native_File_Chooser class.
 */


// /** A cross-platform implementation of Fl_Native_File_Chooser_Driver.
//  This implementation uses a Fl_File_Chooser object as file chooser.

//  Any platform can support the Fl_Native_File_Chooser class by implementing
//  its constructor as follows:
//  \code
//  Fl_Native_File_Chooser::Fl_Native_File_Chooser(int type) {
//    platform_fnfc = new Fl_Native_File_Chooser_FLTK_Driver(type);
//  }
// \endcode
//  */
// class Fl_Native_File_Chooser_FLTK_Driver : public Fl_Native_File_Chooser_Driver {
// private:
//   void errmsg(const char* msg);
//   int type_fl_file(int val);
//   int exist_dialog();
//   void parse_filter();
// protected:
//   int   _btype;                 // kind-of browser to show()
//   int   _options;               // general options
//   int   _nfilters;
//   char* _filter;                // user supplied filter
//   char* _parsedfilt;            // parsed filter
//   int   _filtvalue;             // selected filter
//   char* _preset_file;
//   char* _prevvalue;             // Returned filename
//   char* _directory;
//   char* _errmsg;                // error message
//   Fl_File_Chooser* _file_chooser;
// public:
//   Fl_Native_File_Chooser_FLTK_Driver(int val);
//   virtual ~Fl_Native_File_Chooser_FLTK_Driver();
//   virtual void type(int t);
//   virtual int type() const ;
//   virtual void options(int o);
//   virtual int options() const;
//   virtual int count() const;
//   virtual const char* filename() const ;
//   virtual const char* filename(int i) const ;
//   virtual void directory(const char* val) ;
//   virtual const char* directory() const;
//   virtual void title(const char* t);
//   virtual const char* title() const;
//   virtual const char* filter() const ;
//   virtual void filter(const char* f);
//   virtual int filters() const ;
//   virtual void filter_value(int i) ;
//   virtual int filter_value() const ;
//   virtual void preset_file(const char* f) ;
//   virtual const char* preset_file() const;
//   virtual const char* errmsg() const ;
//   virtual int show() ;
// };

// /**
//  \}
//  \endcond
//  */


#endif /*FL_NATIVE_FILE_CHOOSER_H*/
