#-------------------------------------------------------------------------------
# Name:         ozz_sql3_constants.py (based upon basics_2.c, ozz_sql3.h)
# Purpose:      SQLite3 constant definitions.
#               Convenience wrapper functions for SQLite version 3.
#
# Platform:     Win64, Ubuntu64
# Depends:      SQLite v3.34.1 plus (dll/so), ctypes, sys, os
# SQLite3.h     SQLITE_VERSION      "3.34.1"
#
# Author:       Axle
#
# Created:      12/05/2023
# Updated:      15/05/2023
# Copyright:    (c) Axle 2023
# Licence:      MIT-0 No Attribution
#-------------------------------------------------------------------------------
# Notes:
# Only a subset of constants have been defined. Error, result codes, data types
#
# import ozz_sql3_constants as rc
# or
# from ozz_sql3_constants import *
#-------------------------------------------------------------------------------

#include <inttypes.h> (<stdint.h>)
#Then use uint64_t or int64_t.

##
## CAPI3REF: 64-Bit Integer Types
## KEYWORDS: sqlite_int64 sqlite_uint64
##
## Because there is no cross-platform way to specify 64-bit integer types
## SQLite includes typedefs for 64-bit signed and unsigned integers.
##
## The sqlite3_int64 and sqlite3_uint64 are the preferred type definitions.
## The sqlite_int64 and sqlite_uint64 types are supported for backwards
## compatibility only.
##
## ^The sqlite3_int64 and sqlite_int64 types can store integer values
## between -9223372036854775808 and +9223372036854775807 inclusive.  ^The
## sqlite3_uint64 and sqlite_uint64 types can store integer values
## between 0 and +18446744073709551615 inclusive.
##
##  #ifdef SQLITE_INT64_TYPE
##    typedef SQLITE_INT64_TYPE sqlite_int64;
##  # ifdef SQLITE_UINT64_TYPE
##      typedef SQLITE_UINT64_TYPE sqlite_uint64;
##  # else
##      typedef unsigned SQLITE_INT64_TYPE sqlite_uint64;
##  # endif
##  #elif defined(_MSC_VER) || defined(__BORLANDC__)
##    typedef __int64 sqlite_int64;
##    typedef unsigned __int64 sqlite_uint64;
##  #else
##    typedef long long int sqlite_int64;
##    typedef unsigned long long int sqlite_uint64;
##  #endif
##  typedef sqlite_int64 sqlite3_int64;
##  typedef sqlite_uint64 sqlite3_uint64;

##
## CAPI3REF: Result Codes
## KEYWORDS: {result code definitions}
##
## Many SQLite functions return an integer result code from the set shown
## here in order to indicate success or failure.
##
## New error codes may be added in future versions of SQLite.
##
## See also: [extended result code definitions]
##
SQLITE_OK           =    0      # Successful result
## beginning-of-error-codes
SQLITE_ERROR        =    1      # Generic error
SQLITE_INTERNAL     =    2      # Internal logic error in SQLite
SQLITE_PERM         =    3      # Access permission denied
SQLITE_ABORT        =    4      # Callback routine requested an abort
SQLITE_BUSY         =    5      # The database file is locked
SQLITE_LOCKED       =    6      # A table in the database is locked
SQLITE_NOMEM        =    7      # A malloc() failed
SQLITE_READONLY     =    8      # Attempt to write a readonly database
SQLITE_INTERRUPT    =    9      # Operation terminated by sqlite3_interrupt()
SQLITE_IOERR        =   10      # Some kind of disk I/O error occurred
SQLITE_CORRUPT      =   11      # The database disk image is malformed
SQLITE_NOTFOUND     =   12      # Unknown opcode in sqlite3_file_control()
SQLITE_FULL         =   13      # Insertion failed because database is full
SQLITE_CANTOPEN     =   14      # Unable to open the database file
SQLITE_PROTOCOL     =   15      # Database lock protocol error
SQLITE_EMPTY        =   16      # Internal use only
SQLITE_SCHEMA       =   17      # The database schema changed
SQLITE_TOOBIG       =   18      # String or BLOB exceeds size limit
SQLITE_CONSTRAINT   =   19      # Abort due to constraint violation
SQLITE_MISMATCH     =   20      # Data type mismatch
SQLITE_MISUSE       =   21      # Library used incorrectly
SQLITE_NOLFS        =   22      # Uses OS features not supported on host
SQLITE_AUTH         =   23      # Authorization denied
SQLITE_FORMAT       =   24      # Not used
SQLITE_RANGE        =   25      # 2nd parameter to sqlite3_bind out of range
SQLITE_NOTADB       =   26      # File opened that is not a database file
SQLITE_NOTICE       =   27      # Notifications from sqlite3_log()
SQLITE_WARNING      =   28      # Warnings from sqlite3_log()
SQLITE_ROW          =   100     # sqlite3_step() has another row ready
SQLITE_DONE         =   101     # sqlite3_step() has finished executing
## end-of-error-codes


##
## CAPI3REF: Enable Or Disable Extended Result Codes
## METHOD: sqlite3
##
## ^The sqlite3_extended_result_codes() routine enables or disables the
## [extended result codes] feature of SQLite. ^The extended result
## codes are disabled by default for historical compatibility.
##
# SQLITE_API int sqlite3_extended_result_codes(sqlite3*, int onoff);

##
## CAPI3REF: Extended Result Codes
## KEYWORDS: {extended result code definitions}
##
## In its default configuration, SQLite API routines return one of 30 integer
## [result codes].  However, experience has shown that many of
## these result codes are too coarse-grained.  They do not provide as
## much information about problems as programmers might like.  In an effort to
## address this, newer versions of SQLite (version 3.3.8 [dateof:3.3.8]
## and later) include
## support for additional result codes that provide more detailed information
## about errors. These [extended result codes] are enabled or disabled
## on a per database connection basis using the
## [sqlite3_extended_result_codes()] API.  Or, the extended code for
## the most recent error can be obtained using
## [sqlite3_extended_errcode()].
##
"""
SQLITE_ERROR_MISSING_COLLSEQ    =    (SQLITE_ERROR | (1<<8))
SQLITE_ERROR_RETRY              =    (SQLITE_ERROR | (2<<8))
SQLITE_ERROR_SNAPSHOT           =    (SQLITE_ERROR | (3<<8))
SQLITE_IOERR_READ               =    (SQLITE_IOERR | (1<<8))
SQLITE_IOERR_SHORT_READ         =    (SQLITE_IOERR | (2<<8))
SQLITE_IOERR_WRITE              =    (SQLITE_IOERR | (3<<8))
SQLITE_IOERR_FSYNC              =    (SQLITE_IOERR | (4<<8))
SQLITE_IOERR_DIR_FSYNC          =    (SQLITE_IOERR | (5<<8))
SQLITE_IOERR_TRUNCATE           =    (SQLITE_IOERR | (6<<8))
SQLITE_IOERR_FSTAT              =    (SQLITE_IOERR | (7<<8))
SQLITE_IOERR_UNLOCK             =    (SQLITE_IOERR | (8<<8))
SQLITE_IOERR_RDLOCK             =    (SQLITE_IOERR | (9<<8))
SQLITE_IOERR_DELETE             =    (SQLITE_IOERR | (10<<8))
SQLITE_IOERR_BLOCKED            =    (SQLITE_IOERR | (11<<8))
SQLITE_IOERR_NOMEM              =    (SQLITE_IOERR | (12<<8))
SQLITE_IOERR_ACCESS             =    (SQLITE_IOERR | (13<<8))
SQLITE_IOERR_CHECKRESERVEDLOCK  =    (SQLITE_IOERR | (14<<8))
SQLITE_IOERR_LOCK               =    (SQLITE_IOERR | (15<<8))
SQLITE_IOERR_CLOSE              =    (SQLITE_IOERR | (16<<8))
SQLITE_IOERR_DIR_CLOSE          =    (SQLITE_IOERR | (17<<8))
SQLITE_IOERR_SHMOPEN            =    (SQLITE_IOERR | (18<<8))
SQLITE_IOERR_SHMSIZE            =    (SQLITE_IOERR | (19<<8))
SQLITE_IOERR_SHMLOCK            =    (SQLITE_IOERR | (20<<8))
SQLITE_IOERR_SHMMAP             =    (SQLITE_IOERR | (21<<8))
SQLITE_IOERR_SEEK               =    (SQLITE_IOERR | (22<<8))
SQLITE_IOERR_DELETE_NOENT       =    (SQLITE_IOERR | (23<<8))
SQLITE_IOERR_MMAP               =    (SQLITE_IOERR | (24<<8))
SQLITE_IOERR_GETTEMPPATH        =    (SQLITE_IOERR | (25<<8))
SQLITE_IOERR_CONVPATH           =    (SQLITE_IOERR | (26<<8))
SQLITE_IOERR_VNODE              =    (SQLITE_IOERR | (27<<8))
SQLITE_IOERR_AUTH               =    (SQLITE_IOERR | (28<<8))
SQLITE_IOERR_BEGIN_ATOMIC       =    (SQLITE_IOERR | (29<<8))
SQLITE_IOERR_COMMIT_ATOMIC      =    (SQLITE_IOERR | (30<<8))
SQLITE_IOERR_ROLLBACK_ATOMIC    =    (SQLITE_IOERR | (31<<8))
SQLITE_IOERR_DATA               =    (SQLITE_IOERR | (32<<8))
SQLITE_IOERR_CORRUPTFS          =    (SQLITE_IOERR | (33<<8))
SQLITE_LOCKED_SHAREDCACHE       =    (SQLITE_LOCKED |  (1<<8))
SQLITE_LOCKED_VTAB              =    (SQLITE_LOCKED |  (2<<8))
SQLITE_BUSY_RECOVERY            =    (SQLITE_BUSY   |  (1<<8))
SQLITE_BUSY_SNAPSHOT            =    (SQLITE_BUSY   |  (2<<8))
SQLITE_BUSY_TIMEOUT             =    (SQLITE_BUSY   |  (3<<8))
SQLITE_CANTOPEN_NOTEMPDIR       =    (SQLITE_CANTOPEN | (1<<8))
SQLITE_CANTOPEN_ISDIR           =    (SQLITE_CANTOPEN | (2<<8))
SQLITE_CANTOPEN_FULLPATH        =    (SQLITE_CANTOPEN | (3<<8))
SQLITE_CANTOPEN_CONVPATH        =    (SQLITE_CANTOPEN | (4<<8))
SQLITE_CANTOPEN_DIRTYWAL        =    (SQLITE_CANTOPEN | (5<<8)) # Not Used
SQLITE_CANTOPEN_SYMLINK         =    (SQLITE_CANTOPEN | (6<<8))
SQLITE_CORRUPT_VTAB             =    (SQLITE_CORRUPT | (1<<8))
SQLITE_CORRUPT_SEQUENCE         =    (SQLITE_CORRUPT | (2<<8))
SQLITE_CORRUPT_INDEX            =    (SQLITE_CORRUPT | (3<<8))
SQLITE_READONLY_RECOVERY        =    (SQLITE_READONLY | (1<<8))
SQLITE_READONLY_CANTLOCK        =    (SQLITE_READONLY | (2<<8))
SQLITE_READONLY_ROLLBACK        =    (SQLITE_READONLY | (3<<8))
SQLITE_READONLY_DBMOVED         =    (SQLITE_READONLY | (4<<8))
SQLITE_READONLY_CANTINIT        =    (SQLITE_READONLY | (5<<8))
SQLITE_READONLY_DIRECTORY       =    (SQLITE_READONLY | (6<<8))
SQLITE_ABORT_ROLLBACK           =    (SQLITE_ABORT | (2<<8))
SQLITE_CONSTRAINT_CHECK         =    (SQLITE_CONSTRAINT | (1<<8))
SQLITE_CONSTRAINT_COMMITHOOK    =    (SQLITE_CONSTRAINT | (2<<8))
SQLITE_CONSTRAINT_FOREIGNKEY    =    (SQLITE_CONSTRAINT | (3<<8))
SQLITE_CONSTRAINT_FUNCTION      =    (SQLITE_CONSTRAINT | (4<<8))
SQLITE_CONSTRAINT_NOTNULL       =    (SQLITE_CONSTRAINT | (5<<8))
SQLITE_CONSTRAINT_PRIMARYKEY    =    (SQLITE_CONSTRAINT | (6<<8))
SQLITE_CONSTRAINT_TRIGGER       =    (SQLITE_CONSTRAINT | (7<<8))
SQLITE_CONSTRAINT_UNIQUE        =    (SQLITE_CONSTRAINT | (8<<8))
SQLITE_CONSTRAINT_VTAB          =    (SQLITE_CONSTRAINT | (9<<8))
SQLITE_CONSTRAINT_ROWID         =    (SQLITE_CONSTRAINT |(10<<8))
SQLITE_CONSTRAINT_PINNED        =    (SQLITE_CONSTRAINT |(11<<8))
SQLITE_NOTICE_RECOVER_WAL       =    (SQLITE_NOTICE | (1<<8))
SQLITE_NOTICE_RECOVER_ROLLBACK  =    (SQLITE_NOTICE | (2<<8))
SQLITE_WARNING_AUTOINDEX        =    (SQLITE_WARNING | (1<<8))
SQLITE_AUTH_USER                =    (SQLITE_AUTH | (1<<8))
SQLITE_OK_LOAD_PERMANENTLY      =    (SQLITE_OK | (1<<8))
SQLITE_OK_SYMLINK               =    (SQLITE_OK | (2<<8))
"""

##
## CAPI3REF: Flags For File Open Operations
##
## These bit values are intended for use in the
## 3rd parameter to the [sqlite3_open_v2()] interface and
## in the 4th parameter to the [sqlite3_vfs.xOpen] method.
##
SQLITE_OPEN_READONLY        =   0x00000001     # Ok for sqlite3_open_v2()
SQLITE_OPEN_READWRITE       =   0x00000002     # Ok for sqlite3_open_v2()
SQLITE_OPEN_CREATE          =   0x00000004     # Ok for sqlite3_open_v2()
SQLITE_OPEN_DELETEONCLOSE   =   0x00000008     # VFS only
SQLITE_OPEN_EXCLUSIVE       =   0x00000010     # VFS only
SQLITE_OPEN_AUTOPROXY       =   0x00000020     # VFS only
SQLITE_OPEN_URI             =   0x00000040     # Ok for sqlite3_open_v2()
SQLITE_OPEN_MEMORY          =   0x00000080     # Ok for sqlite3_open_v2()
SQLITE_OPEN_MAIN_DB         =   0x00000100     # VFS only
SQLITE_OPEN_TEMP_DB         =   0x00000200     # VFS only
SQLITE_OPEN_TRANSIENT_DB    =   0x00000400     # VFS only
SQLITE_OPEN_MAIN_JOURNAL    =   0x00000800     # VFS only
SQLITE_OPEN_TEMP_JOURNAL    =   0x00001000     # VFS only
SQLITE_OPEN_SUBJOURNAL      =   0x00002000     # VFS only
SQLITE_OPEN_SUPER_JOURNAL   =   0x00004000     # VFS only
SQLITE_OPEN_NOMUTEX         =   0x00008000     # Ok for sqlite3_open_v2()
SQLITE_OPEN_FULLMUTEX       =   0x00010000     # Ok for sqlite3_open_v2()
SQLITE_OPEN_SHAREDCACHE     =   0x00020000     # Ok for sqlite3_open_v2()
SQLITE_OPEN_PRIVATECACHE    =   0x00040000     # Ok for sqlite3_open_v2()
SQLITE_OPEN_WAL             =   0x00080000     # VFS only
SQLITE_OPEN_NOFOLLOW        =   0x01000000     # Ok for sqlite3_open_v2()

## Reserved:                         0x00F00000 */
## Legacy compatibility: */
SQLITE_OPEN_MASTER_JOURNAL  =   0x00004000  # VFS only


##
## CAPI3REF: Fundamental Datatypes
## KEYWORDS: SQLITE_TEXT
##
## ^(Every value in SQLite has one of five fundamental datatypes:
##
## <ul>
## <li> 64-bit signed integer
## <li> 64-bit IEEE floating point number
## <li> string
## <li> BLOB
## <li> NULL
## </ul>)^
##
## These constants are codes for each of those types.
##
## Note that the SQLITE_TEXT constant was also used in SQLite version 2
## for a completely different meaning.  Software that links against both
## SQLite version 2 and SQLite version 3 should use SQLITE3_TEXT, not
## SQLITE_TEXT.
##
SQLITE_INTEGER  =   1
SQLITE_FLOAT    =   2
SQLITE_BLOB     =   4
SQLITE_NULL     =   5

# for lagacy sqlite compatability
if 'SQLITE_TEXT' in locals():
    del SQLITE_TEXT
    SQLITE_TEXT = 3
elif 'SQLITE_TEXT' in globals():
    del SQLITE_TEXT
    SQLITE_TEXT = 3
else:
    SQLITE_TEXT = 3

## NOTE: NUMERIC is an affinity not a data type.
## NUMERIC can hold either INTEGER or REAL
## This is used to reduce data base size for floating point values without
## values to the right of the floating point. f 111.000 = i 111 (16 bytes vs 4 to 8 bytes)


##
## CAPI3REF: Prepare Flags
##
## These constants define various flags that can be passed into
## "prepFlags" parameter of the [sqlite3_prepare_v3()] and
## [sqlite3_prepare16_v3()] interfaces.
##
## New flags may be added in future releases of SQLite.
##
## <dl>
## [[SQLITE_PREPARE_PERSISTENT]] ^(<dt>SQLITE_PREPARE_PERSISTENT</dt>
## <dd>The SQLITE_PREPARE_PERSISTENT flag is a hint to the query planner
## that the prepared statement will be retained for a long time and
## probably reused many times.)^ ^Without this flag, [sqlite3_prepare_v3()]
## and [sqlite3_prepare16_v3()] assume that the prepared statement will
## be used just once or at most a few times and then destroyed using
## [sqlite3_finalize()] relatively soon. The current implementation acts
## on this hint by avoiding the use of [lookaside memory] so as not to
## deplete the limited store of lookaside memory. Future versions of
## SQLite may act on this hint differently.
##
## [[SQLITE_PREPARE_NORMALIZE]] <dt>SQLITE_PREPARE_NORMALIZE</dt>
## <dd>The SQLITE_PREPARE_NORMALIZE flag is a no-op. This flag used
## to be required for any prepared statement that wanted to use the
## [sqlite3_normalized_sql()] interface.  However, the
## [sqlite3_normalized_sql()] interface is now available to all
## prepared statements, regardless of whether or not they use this
## flag.
##
## [[SQLITE_PREPARE_NO_VTAB]] <dt>SQLITE_PREPARE_NO_VTAB</dt>
## <dd>The SQLITE_PREPARE_NO_VTAB flag causes the SQL compiler
## to return an error (error code SQLITE_ERROR) if the statement uses
## any virtual tables.
## </dl>
##
SQLITE_PREPARE_PERSISTENT   =   0x01
SQLITE_PREPARE_NORMALIZE    =   0x02
SQLITE_PREPARE_NO_VTAB      =   0x04

"""
Global Const $SQLITE_ENCODING_UTF8		= 0 ; /* Database will be created if not exists with UTF8 encoding (default) */
Global Const $SQLITE_ENCODING_UTF16		= 1 ; /* Database will be created if not exists with UTF16le encoding */
Global Const $SQLITE_ENCODING_UTF16be	= 2 ; /* Database will be created if not exists with UTF16be encoding (special usage) */
"""

"""
## CAPI3REF: Create Or Redefine SQL Functions

##
## CAPI3REF: Text Encodings
##
## These constant define integer codes that represent the various
## text encodings supported by SQLite.
##
SQLITE_UTF8             =   1   # IMP: R-37514-35566
SQLITE_UTF16LE          =   2   # IMP: R-03371-37637
SQLITE_UTF16BE          =   3   # IMP: R-51971-34154
SQLITE_UTF16            =   4   # Use native byte order
SQLITE_ANY              =   5   # Deprecated
SQLITE_UTF16_ALIGNED    =   8   # sqlite3_create_collation only

## CAPI3REF: Function Flags
"""



## Only want the result and error codes atm
"""
##
## CAPI3REF: Device Characteristics
##
## The xDeviceCharacteristics method of the [sqlite3_io_methods]
## object returns an integer which is a vector of these
## bit values expressing I/O characteristics of the mass storage
## device that holds the file that the [sqlite3_io_methods]
## refers to.
##
## The SQLITE_IOCAP_ATOMIC property means that all writes of
## any size are atomic.  The SQLITE_IOCAP_ATOMICnnn values
## mean that writes of blocks that are nnn bytes in size and
## are aligned to an address which is an integer multiple of
## nnn are atomic.  The SQLITE_IOCAP_SAFE_APPEND value means
## that when data is appended to a file, the data is appended
## first then the size of the file is extended, never the other
## way around.  The SQLITE_IOCAP_SEQUENTIAL property means that
## information is written to disk in the same order as calls
## to xWrite().  The SQLITE_IOCAP_POWERSAFE_OVERWRITE property means that
## after reboot following a crash or power loss, the only bytes in a
## file that were written at the application level might have changed
## and that adjacent bytes, even bytes within the same sector are
## guaranteed to be unchanged.  The SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN
## flag indicates that a file cannot be deleted when open.  The
## SQLITE_IOCAP_IMMUTABLE flag indicates that the file is on
## read-only media and cannot be changed even by processes with
## elevated privileges.
##
## The SQLITE_IOCAP_BATCH_ATOMIC property means that the underlying
## filesystem supports doing multiple write operations atomically when those
## write operations are bracketed by [SQLITE_FCNTL_BEGIN_ATOMIC_WRITE] and
## [SQLITE_FCNTL_COMMIT_ATOMIC_WRITE].
##
SQLITE_IOCAP_ATOMIC                 =   0x00000001
SQLITE_IOCAP_ATOMIC512              =   0x00000002
SQLITE_IOCAP_ATOMIC1K               =   0x00000004
SQLITE_IOCAP_ATOMIC2K               =   0x00000008
SQLITE_IOCAP_ATOMIC4K               =   0x00000010
SQLITE_IOCAP_ATOMIC8K               =   0x00000020
SQLITE_IOCAP_ATOMIC16K              =   0x00000040
SQLITE_IOCAP_ATOMIC32K              =   0x00000080
SQLITE_IOCAP_ATOMIC64K              =   0x00000100
SQLITE_IOCAP_SAFE_APPEND            =   0x00000200
SQLITE_IOCAP_SEQUENTIAL             =   0x00000400
SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN  =   0x00000800
SQLITE_IOCAP_POWERSAFE_OVERWRITE    =   0x00001000
SQLITE_IOCAP_IMMUTABLE              =   0x00002000
SQLITE_IOCAP_BATCH_ATOMIC           =   0x00004000

##
## CAPI3REF: File Locking Levels
##
## SQLite uses one of these integer values as the second
## argument to calls it makes to the xLock() and xUnlock() methods
## of an [sqlite3_io_methods] object.
##
SQLITE_LOCK_NONE        =   0
SQLITE_LOCK_SHARED      =   1
SQLITE_LOCK_RESERVED    =   2
SQLITE_LOCK_PENDING     =   3
SQLITE_LOCK_EXCLUSIVE   =   4

##
## CAPI3REF: Synchronization Type Flags
##
## When SQLite invokes the xSync() method of an
## [sqlite3_io_methods] object it uses a combination of
## these integer values as the second argument.
##
## When the SQLITE_SYNC_DATAONLY flag is used, it means that the
## sync operation only needs to flush data to mass storage.  Inode
## information need not be flushed. If the lower four bits of the flag
## equal SQLITE_SYNC_NORMAL, that means to use normal fsync() semantics.
## If the lower four bits equal SQLITE_SYNC_FULL, that means
## to use Mac OS X style fullsync instead of fsync().
##
## Do not confuse the SQLITE_SYNC_NORMAL and SQLITE_SYNC_FULL flags
## with the [PRAGMA synchronous]=NORMAL and [PRAGMA synchronous]=FULL
## settings.  The [synchronous pragma] determines when calls to the
## xSync VFS method occur and applies uniformly across all platforms.
## The SQLITE_SYNC_NORMAL and SQLITE_SYNC_FULL flags determine how
## energetic or rigorous or forceful the sync operations are and
## only make a difference on Mac OSX for the default SQLite code.
## (Third-party VFS implementations might also make the distinction
## between SQLITE_SYNC_NORMAL and SQLITE_SYNC_FULL, but among the
## operating systems natively supported by SQLite, only Mac OSX
## cares about the difference.)
##
SQLITE_SYNC_NORMAL      =   0x00002
SQLITE_SYNC_FULL        =   0x00003
SQLITE_SYNC_DATAONLY    =   0x00010

"""

##
## CAPI3REF: Constants Defining Special Destructor Behavior
##
## These are special values for the destructor that is passed in as the
## final argument to routines like [sqlite3_result_blob()].  ^If the destructor
## argument is SQLITE_STATIC, it means that the content pointer is constant
## and will never change.  It does not need to be destroyed.  ^The
## SQLITE_TRANSIENT value means that the content will likely change in
## the near future and that SQLite should make its own private copy of
## the content before returning.
##
## The typedef is necessary to work around problems in certain
## C++ compilers.
##
#typedef void (*sqlite3_destructor_type)(void*);
#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
SQLITE_STATIC       =   0
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)
SQLITE_TRANSIENT    =   -1