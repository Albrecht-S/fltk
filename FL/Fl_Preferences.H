//
// "$Id$"
//
// Preferences implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 2002-2010 by Matthias Melcher.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
   Fl_Preferences class . */

#ifndef Fl_Preferences_H
#  define Fl_Preferences_H

#  include <stdio.h>
#  include "Fl_Export.H" 

/**
   \brief Fl_Preferences provides methods to store user
   settings between application starts.

   Fl_Preferences are similar to the Registry on Windows and Preferences on
   MacOS, and provides a simple database mechanism for UNIX.

   Fl_Preferences uses a hierarchy to store data. This hierarchy consists of
   \em groups (sometimes called \em nodes, similar to \em elements in XML) that
   can again contain groups. Groups can also contain \em entries (also
   called \em "name/value pairs", or \em attributes in XML). Groups and entries
   must have a unique name within their scope.

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
   files: see getUserdataPath().
 
 \note Platform-specific location of the Fl_Preferences text files.
 This construct
  \code new Fl_Preferences(Fl_Preferences::USER, "vendor", "application"); \endcode
 where "vendor" and "application" are any user-chosen strings, will create these text files:
 \li Windows XP platform: <tt>C:\\Documents and Settings\\<i>username</i>\\Application Data\\<i>vendor</i>\\<i>application</i>.prefs</tt>
 \li Windows 7+ platform: <tt>C:\\Users\\<i>username</i>\\AppData\\Roaming\\<i>vendor</i>\\<i>application</i>.prefs </tt>
 \li X11 platform: <tt>$HOME/.fltk/<i>vendor</i>/<i>application</i>.prefs </tt>
 \li MacOS platform: <tt>$HOME/Library/Preferences/<i>vendor</i>/<i>application</i>.prefs </tt>

   \note Starting with FLTK 1.3, preference databases are expected to
   be in UTF-8 encoding. Previous databases were stored in the
   current character set or code page which renders them incompatible
   for text entries using international characters.

   \note When creating the initial Fl_Preferences instance, the contents of the
   entire \c .prefs file is read and stored in a hierarchy of <tt>Node</tt>s
   in memory.
   Only when this initial instance is destroyed again, the contents of the
   hierarchy is written back to the \c .prefs file. There is no locking
   mechanism that will keep to separate tasks from reading and writing the same
   file. It is therefore recommended that the initial Fl_Preferences instance
   should only exist for a short time.

   \note Instances of Fl_Preferences are just bookmarks, pointing to nodes
   in memory. Multiple Fl_Preferences can point to the same node. The user
   needs to manage creation and deletion of Fl_Preferences. Delting an
   Fl_Prefrences instance will only delete that bookmark, but not the node.
   Nodes are deleted using Fl_Preferences::deleteGroup(). If a node is deleted,
   all bookmarks to the deleted node will continue to exist, but will no longer
   serve any purpose.

   \note It is entirely up to the user to create and delete Fl_Preferences
   instances.
 */
class FL_EXPORT Fl_Preferences {

public: 
  /**
     Define the scope of the preferences.
   */
  enum Root { 
    SYSTEM=0,   ///< Preferences are used system-wide
    USER        ///< Preferences apply only to the current user
  };
  
  /**
   Every Fl_Preferences-Group has a uniqe ID.
   
   ID's can be retrieved from an Fl_Preferences-Group and can then be used
   to create more Fl_Preference references to the same data set, as long as the 
   database remains open.
   */
  typedef void *ID;
  
  static const char *newUUID();

  Fl_Preferences( Root root, const char *vendor, const char *application );
  Fl_Preferences( const char *path, const char *vendor, const char *application );
  Fl_Preferences( Fl_Preferences &parent, const char *group );
  Fl_Preferences( Fl_Preferences *parent, const char *group );
  Fl_Preferences( Fl_Preferences &parent, int groupIndex );
  Fl_Preferences( Fl_Preferences *parent, int groupIndex );
  Fl_Preferences(const Fl_Preferences&);
  Fl_Preferences( ID id );
  virtual ~Fl_Preferences();
  
  /** Return an ID that can later be reused to open more references to this dataset.
   */
  ID id() { return (ID)node; }
  
  /** Remove the group with this ID from a database.
   */
  static char remove(ID id_) { ((Node*)id_)->remove(); return 0; }

  /** Return the name of this entry.
   */
  const char *name() { return node->name(); }
  
  /** Return the full path to this entry.
   */
  const char *path() { return node->path(); }
  
  int groups();
  const char *group( int num_group );
  char groupExists( const char *key );
  char deleteGroup( const char *group );
  char deleteAllGroups();

  int entries();
  const char *entry( int index );
  char entryExists( const char *key );
  char deleteEntry( const char *entry );
  char deleteAllEntries();
  
  char clear();

  char set( const char *entry, int value );
  char set( const char *entry, float value );
  char set( const char *entry, float value, int precision );
  char set( const char *entry, double value );
  char set( const char *entry, double value, int precision );
  char set( const char *entry, const char *value );
  char set( const char *entry, const void *value, int size ); 
  
  char get( const char *entry, int &value, int defaultValue );
  char get( const char *entry, float &value,  float defaultValue );
  char get( const char *entry, double &value, double defaultValue );
  char get( const char *entry, char *&value,  const char *defaultValue );
  char get( const char *entry, char *value,   const char *defaultValue, int maxSize );
  char get( const char *entry, void *&value,  const void *defaultValue, int defaultSize );
  char get( const char *entry, void *value,   const void *defaultValue, int defaultSize, int maxSize );

  int size( const char *entry );

  char getUserdataPath( char *path, int pathlen );

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
  class FL_EXPORT Name {

    char *data_;

  public: 
    Name( unsigned int n );
    Name( const char *format, ... );

    /**
       Return the Name as a "C" string.
       \internal
     */
    operator const char *() { return data_; }
    ~Name();
  };

  /** \internal An entry associates a preference name to its corresponding value */
  struct Entry {
    char *name, *value;
  };

private: 
  Fl_Preferences() : node(0), rootNode(0) { }
  Fl_Preferences &operator=(const Fl_Preferences&);

  static char nameBuffer[128];
  static char uuidBuffer[40];
  static Fl_Preferences *runtimePrefs;

public:  // older Sun compilers need this (public definition of the following classes)
  class RootNode;
  
    /** \internal
     \brief A node contains a list to all its entries and all means to manage
        the tree structure.
     */
  class FL_EXPORT Node {
    Node *child_, *next_;
    Node *parent_;
    RootNode *root_;
    char *name_;
    char *path_;
    Entry *entry_;
    int nEntry_, NEntry_;
    unsigned char dirty_:1;
    unsigned char indexed_:1;
    unsigned char zombie_:1;
    unsigned char isRoot_:1;
    // indexing routines
    Node **index_;
    int nIndex_, NIndex_;
    int refCount_;
    void createIndex();
    void invalidateIndex();
    void deleteIndex();
    ~Node();
  public:
    // low level stuff
    Node( const char *path );
    void ref();
    void unref();
    void unlink();
    void addChild(Node *nd);
    void removeChild(Node *nd);

    // access to member variables
    Node *firstChild() { return child_; }
    Node *next() { return next_; }
    Node *parent() { return parent_; }
    RootNode *root() { return root_; }
    void setRoot(RootNode *r) { root_ = r; }
    const char *name() { return name_; }
    const char *path() { return path_; }
    int isRoot() { return isRoot_; }

    // more complex methods
    Node *find( const char *path );
    Node *search( const char *path, int offset=0 );
    Node *childNode( int ix );
    Node *addChild( const char *path );
    void remove();
    char dirty();
    void clearDirtyFlags();
    void deleteAllChildren();
    int write( FILE *f );

    // entry methods
    int nChildren();
    const char *child( int ix );
    int set( const char *name, const char *value );
    void set( const char *line );
    void add( const char *line );
    const char *get( const char *name );
    int getEntry( const char *name );
    char deleteEntry( const char *name );
    void deleteAllEntries();
    int nEntry() { return nEntry_; }
    Entry &entry(int i) { return entry_[i]; }

    static const char *ROOT;
  };
  friend class Node;

  class FL_EXPORT RootNode {		// the root node manages file paths and basic reading and writing
    Fl_Preferences *prefs_;
    char *filename_;
    char *vendor_, *application_;
    int lastEntrySet_;
  public:
    RootNode( Fl_Preferences *, Root root, const char *vendor, const char *application );
    RootNode( Fl_Preferences *, const char *path, const char *vendor, const char *application );
    RootNode( Fl_Preferences * );
    ~RootNode();
    int read();
    int write();
    char getPath( char *path, int pathlen );
    void lastEntrySet(int e) { lastEntrySet_ = e; }
    int lastEntrySet() { return lastEntrySet_; }
  };
  friend class RootNode;

protected:
  Node *node;
  RootNode *rootNode;
};

#endif // !Fl_Preferences_H

//
// End of "$Id$".
//
