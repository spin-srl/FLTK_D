//
// Preferences implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 2002-2010 by Matthias Melcher.
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

/* \file
   Fl_Preferences class . */

#ifndef Fl_Preferences_H
#  define Fl_Preferences_H

#  include <stdio.h>
#  include " .H"

/**
   \brief Fl_Preferences provides methods to store user
   settings between application starts.

   It is similar to the
   Registry on Windows and Preferences on MacOS, and provides a
   simple configuration mechanism for UNIX.

   Fl_Preferences uses a hierarchy to store data. It
   bundles similar data into groups and manages entries in these
   groups as name/value pairs.

   Preferences are stored in text files that can be edited
   manually. The file format is easy to read and relatively
   forgiving. Preferences files are the same on all platforms. User
   comments in preference files are preserved. Filenames are unique
   for each application by using a vendor/application naming
   scheme. The user must provide default values for all entries to
   ensure proper operation should preferences be corrupted or not
   yet exist.

   Entries can be of any length. However, the size of each
   preferences file should be kept small for performance
   reasons. One application can have multiple preferences files.
   Extensive binary data however should be stored in separate
   files: see \a Fl_Preferences::getUserdataPath() .

   \note Starting with FLTK 1.3, preference databases are expected to
     be in UTF-8 encoding. Previous databases were stored in the
     current character set or code page which renders them incompatible
     for text entries using international characters.

   \note Starting with FLTK 1.4, searching a valid path to store
     the preferences files has changed slightly. Please see
     Fl_Preferences::Fl_Preferences(Root, const char*, const char*)
     for details.
 */
class Fl_Preferences {

public:
  /**
     Define the scope of the preferences.
   */
  enum Root {
    SYSTEM = 0,        ///< Preferences are used system-wide
    USER,              ///< Preferences apply only to the current user
    ROOT_MASK = 0xFF,  ///< masks for the values above
    CORE = 0x100,      ///< OR'd by FLTK to read and write core library preferences and options
    CORE_SYSTEM = CORE | SYSTEM,
    CORE_USER = CORE | USER
  };

  /**
   Every Fl_Preferences-Group has a uniqe ID.

   ID's can be retrieved from an Fl_Preferences-Group and can then be used
   to create more Fl_Preference references to the same data set, as long as the
   database remains open.
   */
  typedef void* ID;

  static const char* newUUID();

  /** Set this, if no call to Fl_Preferences shall access the file sytem
   @see Fl_Preferences::file_access(unsigned int)
   @see Fl_Preferences::file_access()
   */
  static const unsigned int NONE = 0x0000;
  /** set this if it is ok for applications to read user preference files */
  static const unsigned int USER_READ_OK = 0x0001;
  /** set this if it is ok for applications to create and write user preference files */
  static const unsigned int USER_WRITE_OK = 0x0002;
  /** set this if it is ok for applications to read, create, and write user preference files */
  static const unsigned int USER_OK = USER_READ_OK | USER_WRITE_OK;
  /** set this if it is ok for applications to read system wide preference files */
  static const unsigned int SYSTEM_READ_OK = 0x0004;
  /** set this if it is ok for applications to create and write system wide preference files */
  static const unsigned int SYSTEM_WRITE_OK = 0x0008;
  /** set this if it is ok for applications to read, create, and write system wide preference files */
  static const unsigned int SYSTEM_OK = SYSTEM_READ_OK | SYSTEM_WRITE_OK;
  /** set this if it is ok for applications to read, create, and write any kind of preference files */
  static const unsigned int APP_OK = SYSTEM_OK | USER_OK;
  /** Set this if it is ok for FLTK to read preference files. USER_READ_OK and/or SYSTEM_READ_OK must also be set. */
  static const unsigned int CORE_READ_OK = 0x0010;
  /** Set this if it is ok for FLTK to create or write preference files. USER_WRITE_OK and/or SYSTEM_WRITE_OK must also be set. */
  static const unsigned int CORE_WRITE_OK = 0x0020;
  /** set this if it is ok for FLTK to read, create, or write preference files */
  static const unsigned int CORE_OK = CORE_READ_OK | CORE_WRITE_OK;
  /** set this to allow FLTK and applications to read preference files */
  static const unsigned int ALL_READ_OK = USER_READ_OK | SYSTEM_READ_OK | CORE_READ_OK;
  /** set this to allow FLTK and applications to create and write preference files */
  static const unsigned int ALL_WRITE_OK = USER_WRITE_OK | SYSTEM_WRITE_OK | CORE_WRITE_OK;
  /** set this to give FLTK and applications permission to read, write, and create preference files */
  static const unsigned int ALL = ALL_READ_OK | ALL_WRITE_OK;

  static void file_access(unsigned int flags);
  static unsigned int file_access();

  Fl_Preferences(Root root, const char* vendor, const char* application);
  Fl_Preferences(const char* path, const char* vendor, const char* application);
  Fl_Preferences(Fl_Preferences &parent, const char* group);
  Fl_Preferences(Fl_Preferences* parent, const char* group);
  Fl_Preferences(Fl_Preferences &parent, int groupIndex);
  Fl_Preferences(Fl_Preferences* parent, int groupIndex);
  Fl_Preferences(const Fl_Preferences &);
  Fl_Preferences(ID id);
  virtual ~Fl_Preferences();

  /** Return an ID that can later be reused to open more references to this dataset.
   */
  ID id() { return (ID)node; }

  /** Remove the group with this ID from a database.
   */
  static char remove(ID id_) { return ((Node*)id_)->remove(); }

  /** Return the name of this entry.
   */
  const char* name() { return node->name(); }

  /** Return the full path to this entry.
   */
  const char* path() { return node->path(); }

  int groups();
  const char* group(int num_group);
  char groupExists(const char* key);
  char deleteGroup(const char* group);
  char deleteAllGroups();

  int entries();
  const char* entry(int index);
  char entryExists(const char* key);
  char deleteEntry(const char* entry);
  char deleteAllEntries();

  char clear();

  char set(const char* entry, int value);
  char set(const char* entry, float value);
  char set(const char* entry, float value, int precision);
  char set(const char* entry, double value);
  char set(const char* entry, double value, int precision);
  char set(const char* entry, const char* value);
  char set(const char* entry, const void* value, int size);

  char get(const char* entry, int &value, int defaultValue);
  char get(const char* entry, float &value,  float defaultValue);
  char get(const char* entry, double &value, double defaultValue);
  char get(const char* entry, char* &value,  const char* defaultValue);
  char get(const char* entry, char* value,   const char* defaultValue, int maxSize);
  char get(const char* entry, void* &value,  const void* defaultValue, int defaultSize);
  char get(const char* entry, void* value,   const void* defaultValue, int defaultSize, int maxSize);

  int size(const char* entry);

  char getUserdataPath(char* path, int pathlen);

  void flush();

  // char export( const char *filename, Type fileFormat );
  // char import( const char *filename );

  /**
     'Name' provides a simple method to create numerical or more complex
     procedural names for entries and groups on the fly.

     Example: prefs.set(Fl_Preferences::Name("File%d",i),file[i]);.

     See test/preferences.cxx as a sample for writing arrays into preferences.

     'Name' is actually implemented as a class inside Fl_Preferences. It casts
     into const char* and gets automatically destroyed after the enclosing call
     ends.
   */
  class Name {

    char* data_;

  public:
    Name(unsigned int n);
    Name(const char* format, ...);

    /**
       Return the Name as a "C" string.
       \internal
     */
    operator const char* () { return data_; }
    ~Name();
  };

  /** \internal An entry associates a preference name to its corresponding value */
  struct Entry {
    char* name, *value;
  };

private:
  Fl_Preferences() : node(0), rootNode(0) { }
  Fl_Preferences &operator=(const Fl_Preferences &);

  static char nameBuffer[128];
  static char uuidBuffer[40];
  static Fl_Preferences* runtimePrefs;
  static unsigned int fileAccess_;

public:  // older Sun compilers need this (public definition of the following classes)
  class RootNode;

  class Node {        // a node contains a list to all its entries
    // and all means to manage the tree structure
    Node* child_, *next_;
    union {                     // these two are mutually exclusive
      Node* parent_;            // top_ bit clear
      RootNode* root_;          // top_ bit set
    };
    char* path_;
    Entry* entry_;
    int nEntry_, NEntry_;
    unsigned char dirty_: 1;
    unsigned char top_: 1;
    unsigned char indexed_: 1;
    // indexing routines
    Node** index_;
    int nIndex_, NIndex_;
    void createIndex();
    void updateIndex();
    void deleteIndex();
  public:
    static int lastEntrySet;
  public:
    Node(const char* path);
    ~Node();
    // node methods
    int write(FILE* f);
    const char* name();
    const char* path() { return path_; }
    Node* find(const char* path);
    Node* search(const char* path, int offset = 0);
    Node* childNode(int ix);
    Node* addChild(const char* path);
    void setParent(Node* parent);
    Node* parent() { return top_ ? 0L : parent_; }
    void setRoot(RootNode* r) { root_ = r; top_ = 1; }
    RootNode* findRoot();
    char remove();
    char dirty();
    void clearDirtyFlags();
    void deleteAllChildren();
    // entry methods
    int nChildren();
    const char* child(int ix);
    void set(const char* name, const char* value);
    void set(const char* line);
    void add(const char* line);
    const char* get(const char* name);
    int getEntry(const char* name);
    char deleteEntry(const char* name);
    void deleteAllEntries();
    int nEntry() { return nEntry_; }
    Entry &entry(int i) { return entry_[i]; }
  };
  friend class Node;

  class RootNode {            // the root node manages file paths and basic reading and writing
    Fl_Preferences* prefs_;
    char* filename_;
    char* vendor_, *application_;
    Root root_;
  public:
    RootNode(Fl_Preferences*, Root root, const char* vendor, const char* application);
    RootNode(Fl_Preferences*, const char* path, const char* vendor, const char* application);
    RootNode(Fl_Preferences*);
    ~RootNode();
    int read();
    int write();
    char getPath(char* path, int pathlen);
  };
  friend class RootNode;

protected:
  Node* node;
  RootNode* rootNode;
};

#endif // !Fl_Preferences_H
