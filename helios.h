/*===============================[[ beg-code ]]===============================*/
#ifndef helios
#define helios yes


/*===[[ ONE_LINERS ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "SU (system utilities)"
#define     P_NICHE     "ff (filesystem finder)"
#define     P_SUBJECT   "file-system searching"
#define     P_PURPOSE   "file system indexing, searching, and analytics"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "helios-phaeton (radiant one)"
#define     P_PRONOUNCE "hee·lee·ohs fay·tuhn"
#define     P_HERITAGE  "the all-seeing titan god of the sun and clear sight"
#define     P_BRIEFLY   "all-seeing sun god"
#define     P_IMAGERY   "drives the sun across the sky in a chariot pulled by winged horses"
#define     P_REASON    ""
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/helios.filesystem_locator"
#define     P_BASENAME  "helios"
#define     P_FULLPATH  "/usr/local/bin/helios"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 5.3.0"
#define     P_CODESIZE  "small       (appoximately 2,000 slocl)"
/*········· ··········· ´·····························´········································*/
#define     P_DEPSTDC   "stdio,stdlib,string,errno,time"
#define     P_DEPPOSIX  "unistd,dirent,fcntl,sys/stat,sys/ioctl,linux/fs,linux/hdreg"
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS "···"
#define     P_DEPOTHER  "yPARSE,ySORT,yREGEX,yJOBS,yEXEC"
#define     P_DEPSOLO   "···"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2014-10"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "1.--, first major version in production"
#define     P_VERMINOR  "1.2-, stablizing after too many changes"
#define     P_VERNUM    "1.2e"
#define     P_VERTXT    "yPARSE_config updated for latest library change"
/*········· ··········· ´·····························´········································*/
#define     P_WARNING   "this does exactly what i want with ZERO thought to working for you"
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*===[[ HEADER END ]]=========================================================*/

/*
 * ========================== EXPLICITLY GPL LICENSED ==========================
 *
 * the only place you could have gotten this code is my github or website.
 * given that, you already know it is GPL licensed, so act accordingly.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of the Software, its documentation and marketing & publicity
 * materials, and acknowledgment shall be given in the documentation, materials
 * and software packages that this Software was used.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * this code is custom-tailored to the author's requirements.  given that,
 * AND the fact that i update, upgrade, and refactor constantly, this is a
 * volitile and quirky environment.  therefore, i do NOT recommend this
 * program for anyone else's use ;) i share it just to potentially provide
 * insight into alternate architectures and approaches.
 */


/*
 * METIS § ····· § create a link table to store symlink destinations                      § NA15Pd §  · §
 *
 *
 *
 */

/*
 *
 *  design notes...
 *  -- data and pointers where separated to make binary read and write simplier
 *
 *
 *
 *
 *
 *
 *
 */



/*===[[ GREEK HERITAGE ]]=====================================================*/
/*
 *   helios-phaeton (radiant) is the greek titan of the sun, giving light to
 *   both gods and men alike.  helios is the god of clear sight, and as such,
 *   he sees and hears everything that transpires on the earth and seas.  he
 *   drives a tethrippon (chariot with four horses abreast) from his palace in
 *   the east to the gates of helios in the west then a golden boat takes him
 *   back to his palace in the east at night.  his chariot is pulled by four
 *   winged, fiery horses.  white horses and the rooster are sacred to helios.
 *   the collosus of rhodes was his likeness.  he is the son of theia.
 */

/*===[[ PURPOSE ]]============================================================*/
/*
 *   locate is a well-worn, familiar, time-tested, and posix-defined application
 *   that provides solid, basic, usable file indexing.  while there are many
 *   other theories on file organization and meta-data usage, locate is still
 *   the most solid, responsive, and commonly used by power users.  many
 *   attempts have been made to replace it with real-time and glitzy interfaces,
 *   but locate is still here.
 *
 *   the early developers of locate nailed it in the original by focusing on
 *   real and definate needs by folks that use the system frequently as well as
 *   the realities of low power systems.  they built the architecture around
 *   a batch update to the master index in order to keep system pressure down,
 *   request speed up, and wear-n-tear on the hard drives low.
 *
 *   but, there are not many file search implementations available that add
 *   new features, additional capabilities, and integration with desktop tools.
 *   some of the new features are worthwhile, some are creaping featurism, and
 *   others are just to satisfy relatively ignorant desktop users.  i don't care
 *   if its easy for an idiot to use -- they don't use it anyway ;)
 *
 *   the purpose of helios is to bring some updates to the architecture, give me
 *   a chance to learn some great skills by following in locates footprints,
 *   and add some cool stuff to the search options.  additionally, i want it to
 *   integrate with visualization tools which can give me graphical analysis
 *   and navigation of the file system without needing glue-scripts.
 */

/*===[[ REASONING ]]==========================================================*/
/*
 *   problem statement (p) ---------------------------------
 *
 *   there are many various file system tools for locating files and finding
 *   sizes and duplication, but they are not integrated and often require many
 *   helper languages, toolkits, and libraries.  i would like to take the best
 *   thinking from the original unix tools and integrate capabilities that help
 *   navigate and maintain files within file systems.
 */
/*
 *   requirement and reward (rr)---------------------------
 *
 *   files and directories within filesystems often go unmanaged and atrophy
 *   badly over time.  the result is duplication, unfindable files, and excess
 *   storage that could be better applied to other usees.
 *
 *      -- better filing by enabling powerful global search (better org)
 *      -- quickly find files by numerous methods (more productivity)
 *      -- take pressure off drives by using file database (reduce wear)
 *      -- drive organization improvements by see big picture (better org)
 *      -- quickly find unused and duplicate files (save space)
 */
/*
 *   objectives (o) ---------------------------------------
 *
 *   objectives and ideas have been collected by reviewing tools that normally
 *   fit into the same functional area, including, locate, mlocate, slocate,
 *   baobab, filesight, kdirstat, and xdiskusage.
 *
 *   helios will accept the following traditional locate requirements...
 *      -- search a database rather than the file system itself (speed/wear)
 *      -- update the database in batch as most almost never change (efficiency)
 *      -- provide regular expression matching (always and skip other types)
 *      -- allow for multiple potential databases for flexibility (--database)
 *      -- provide ability to simply get count of matches (--count)
 *      -- allow case-insensitive matching (--ignore-case)
 *      -- provide ability to stop after a certain number of matches (--limit)
 *      -- automatically focus on just the file and dir names (--basename)
 *      -- able to produce simple database statistics (--statistics)
 *
 *   helios will not accept the following traditional locate requirements...
 *      -- will not provide literal or gobbing searches (regex can do more)
 *      -- will not allow for multiple simultaneous regex searches
 *      -- will not allow check that file still exists at runtime (--existing)
 *      -- will not search concatenated dir and file names (--wholename)
 *      -- will not follow symlinks as its duplicate anyway (--follow)
 *      -- will not provide an option for various list formatting (--null)
 *      -- will not update database with only new enteries (runs fast anyway)
 *
 *   helios will also add several other requirements to enhance locate...
 *      -- allow search depth to be limited to avoid run away detail (--depth)
 *      -- can focus on a specific branch of the dir structure (--start)
 *      -- add the ability to dump the entire database (--dump)
 *      -- add the ability to reproduce the dir structure (--dirtree)
 *      -- add the ability to show size of files by mime type (--mimetree)
 *      -- add ability to report on badly named files and directories
 *      -- add ability to spot potentially duplicate files
 *      -- ability to limit search to certain mime-like types and extensions
 *      -- search for files below, near, or above a certain size
 *      -- search for files updated before, during, or after certain times
 *      -- automatically report on file and dir changes between updates
 *      -- add ability to generate mime-type summary statistics
 *
 *   helios will also add several enhancements to target database building...
 *      -- ability to ignore specific directories altogether (dir_ignore)
 *      -- ability to gather sizes, but not the dir itself (dir_stop)
 *      -- ability to gather sizes below, but not entries below (dir_silent)
 *      -- ability to skip certain dir name where ever found (dir_never)
 *      -- ability to merge databases across machines and networks
 *
 *   helios will also keep some options only in one version of locate...
 *      -- only allow a user to see dirs and files to which they have access
 *
 *   finally, helios will not...
 *      -- get involved in meta-data or tag searching (way too spotty)
 *      -- provide any scripting to process files specially
 *      -- call any external programs directly (use pipes and xargs)
 *      -- provide any graphical viewing directly (use external tools)
 * 
 *   as always, applications running on my systems must...
 *      -- stay focused, small, tight, reliable, and secure
 *      -- forgo sloppy, kitchen-sink languages, like python, java, or perl
 *      -- stay away from dependency on external code libraries, like boost
 *      -- only use human maintainable, 7-bit safe, ascii streams (not database)
 *      -- have dedicated, maintained, automated unit testing
 *      -- provide either detailed reporting or logging for debugging
 *      -- use secure communications between systems where required
 *      -- clean, clean code so i can maintain after long absences
 *      -- no international, multi-language, multi-byte support (freaking ever)
 */
/*
 *   i don't want to just use the system, i want to *own* it; so that means
 *   i have to fundmentally understand the critical services which only tearing
 *   down and rebuilding can really teach -- even if its more basic in the end.
 *
 *   so, as always, there are many stable, accepted, existing programs and
 *   utilities written by better programmers which are likely superior in
 *   speed, size, capability, and reliability; BUT, i would not have learned
 *   nearly as much just using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   at the end of the day, this is hughly useful and deserves the attention.
 *   it is also full of learning opportunities ;)
 *
 */



/*===[[ PUBLIC HEADERS ]]=====================================================*/

/*---(big standards)------------*/
#include    <stdio.h>        /* C_ANSI (68) printf, fgets, fopen, fread       */
#include    <string.h>       /* C_ANSI (34) strcpy, strchr, strcmp, strtok    */
#include    <stdlib.h>       /* C_ANSI (81) exit, system, atoi, rand, malloc  */
#include    <unistd.h>       /* LINUX  (82) getuid, chown, sleep, fork, chdir */
#include    <errno.h>        /* LINUX  (--) errno                             */

/*---(filesystems)--------------*/
#include    <dirent.h>       /* POSIX  (11) opendir, readdir, alphasort       */
#include    <fcntl.h>        /* UNIX   (06) creat, fcntl, open                */
#include    <sys/stat.h>     /* UNIX   : fstat, umask                         */
#include    <sys/ioctl.h>    /* C_ANSI :                                      */
#include    <linux/fs.h>     /* LINUX  :                                      */
#include    <linux/hdreg.h>  /* LINUX  :                                      */

/*---(other standard)-----------*/
#include    <time.h>         /* C_ANSI (28) time, localtime, strftime, timer  */

/*===[[ CUSTOM LIBRARIES ]]===================================================*/
#include    <yURG.h>         /* CUSTOM : heatherly urgent processing          */
#include    <yLOG.h>         /* CUSTOM : heatherly program logging            */
#include    <ySTR.h>         /* CUSTOM : heatherly string handling            */
#include    <yPARSE.h>       /* CUSTOM : heatherly file handling              */
#include    <ySORT.h>        /* CUSTOM : heatherly sorting and searching      */
#include    <yREGEX.h>       /* CUSTOM : heatherly regular expressions        */
#include    <yJOBS.h>             /* heatherly job execution and control      */
#include    <yEXEC.h>
#include    <yCOLOR_solo.h>




/*---(struct typedefs)-------------------*/
/*---(library simplifications)---------*/
typedef struct dirent    tDIRENT;
typedef struct FILE      tFILE;
typedef struct tm        tTIME;
typedef struct stat      tSTAT;

typedef unsigned char    uchar;
typedef unsigned short   ushort;
typedef long long        llong;


typedef     struct     cDRIVE    tDRIVE;
typedef     struct     cPTRS     tPTRS;
typedef     struct     cENTRY    tENTRY;



#define     FILE_CONF   "/etc/helios.conf"
#define     FILE_DB     "/var/lib/helios/helios.db"
#define     FILE_MIME   "/var/lib/helios/helios_mime.rptg"


#define     MAX_STR     50
#define     MAX_NAME   100
#define     MAX_REGEX  200
#define     MAX_DEPTH   20



struct cDRIVE {
   /*---(base)-----------------*/
   uchar       ref;
   char        host        [LEN_LABEL];
   char        serial      [LEN_LABEL];
   char        device      [LEN_FULL];
   char        mpoint      [LEN_FULL];
   char        type        [LEN_LABEL];
   llong       size;
   int         written;                     /* dir entry was written on       */
   /*---(stats)----------------*/
   int         n_seen;                      /* number seen on drive           */
   llong       b_seen;                      /* bytes seen on drive            */
   int         n_kept;                      /* number made into entries       */
   llong       b_kept;                      /* bytes of entries               */
   int         n_found;                     /* number in last find            */
   llong       b_found;                     /* bytes in last find             */
   /*---(links)----------------*/
   tDRIVE     *m_prev;                      /* master list                    */
   tDRIVE     *m_next;                      /* master list                    */
   tENTRY     *root;                        /* root of mount poinet           */
   /*---(done)-----------------*/
};
extern      tDRIVE    *h_drive;
extern      tDRIVE    *t_drive;
extern      short      g_ndrive;
extern      short      g_udrive;


#define       ENTRY_REG      '-'
#define       ENTRY_DIR      'd'
#define       ENTRY_CDEV     'c'
#define       ENTRY_BDEV     'b'
#define       ENTRY_FIFO     'f'
#define       ENTRY_SOCK     's'
#define       ENTRY_LINK     'l'
#define       ENTRY_HUH      '?'
#define       ENTRY_DEVICES  "dcbfs"
#define       ENTRY_ALL      "-dcbfsl?"
#define       ENTRY_OPTIONS  " --reg --dir --cdev --bdev --fifo --sock --link --unknown "
#define       ENTRY_NEGS     " --noreg --nodir --nocdev --nobdev --nofifo --nosock --nolink --nounknown "

#define       WALK_ALL       '*'
#define       WALK_FIRST     '1'
#define       WALK_INDEXED   '#'
#define       WALK_UPTO      '-'

/*---(normal stypes)------------------*/
#define       STYPE_EMPTY        ' '
#define       STYPE_NORMAL       '-'
#define       STYPE_LINK         '>'
/*---(overall stypes)-----------------*/
#define       STYPE_START        '/'
#define       STYPE_LEVEL        '#'
/*---(silenced stypes)----------------*/
#define       STYPE_SILENT       '~'
#define       STYPE_SILENT_UNDER '+'
#define       STYPE_SILENT_EVERY 'x'
#define       STYPE_SILENT_WHEN  '?'
/*---(avoided stypes)-----------------*/
#define       STYPE_AVOID        '('
#define       STYPE_AVOID_FULL   '['
#define       STYPE_AVOID_UNDER  ')'
#define       STYPE_AVOID_EVERY  'X'
/*---(system stypes)------------------*/
#define       STYPE_SYSTEM       '$'
#define       STYPE_SYSTEM_UNDER '%'
/*---(private stype)------------------*/
#define       STYPE_PRIVATE      '!'
/*---(combinations)-------------------*/
#define       STYPE_BADS         "[!"
/*---(end)----------------------------*/

#define       AGES_JUST      'j'
#define       AGES_DAYS      'd'
#define       AGES_WEEK      'w'
#define       AGES_MONTH     'm'
#define       AGES_QUARTER   'q'
#define       AGES_YEAR      'y'
#define       AGES_ANCIENT   'a'
#define       AGES_ALL       "jdwmqya"
#define       AGES_OPTIONS   " --just --days --week --month --quarter --year --ancient "
#define       AGES_NEGS      " --nojust --nodays --noweek --nomonth --noquarter --noyear --noancient "

#define       ASCII_BASIC    '-'
#define       ASCII_UPPER    'A'
#define       ASCII_PUNCT    '+'
#define       ASCII_EXTEND   '#'
#define       ASCII_SPACE    '>'
#define       ASCII_CRAZY    'X'
#define       ASCII_ALL      "-A+#>X"
#define       ASCII_OPTIONS  " --basic --upper --punct --extend --space --crazy --badname "
#define       ASCII_NEGS     " --nobasic --noupper --nopunct --noextend --nospace --nocrazy "

extern const char *EXT_DIR;
extern const char *EXT_DHIDDEN;

extern const char *EXT_CONF;

extern const char *EXT_EXEC;
extern const char *EXT_DEBUG;
extern const char *EXT_UNIT;
extern const char *EXT_MAKEF;
extern const char *EXT_MMAKE;
extern const char *EXT_JEXEC;
extern const char *EXT_JLIB;
extern const char *EXT_JUNIT;
extern const char *EXT_JC;

extern const char *EXT_BDEV;
extern const char *EXT_CDEV;
extern const char *EXT_FIFO;
extern const char *EXT_SOCK;

extern const char *EXT_DLINK;
extern const char *EXT_RLINK;
extern const char *EXT_ELINK;
extern const char *EXT_CLINK;
extern const char *EXT_BLINK;
extern const char *EXT_FLINK;
extern const char *EXT_SLINK;
extern const char *EXT_ULINK;

extern const char *EXT_BACKUP;
extern const char *EXT_UNKNOWN;
extern const char *EXT_MYSTERY;
extern const char *EXT_MANUAL;
extern const char *EXT_OHIDDEN;
extern const char *EXT_XHIDDEN;

extern const char *EXT_GIT;
extern const char *EXT_PORTAGE;
extern const char *EXT_KERNEL;
extern const char *EXT_CACHE;

extern const char *EXT_SCAN;
extern const char *EXT_PHOTO;
extern const char *EXT_WALL;

extern const char *EXT_PRIVATE;
extern const char *EXT_EMPTY;

#define       SIZES_ALL      "0123456789abcdefghi"
#define       SIZES_OPTIONS  " --zero --byte --kilo --mega --giga --tera --peta "
#define       SIZES_NEGS     " --nozero --nobyte --nokilo --nomega --nogiga --notera --nopeta "


struct cENTRY {
   /*---(types)--------------------------*/
   uchar       lvl;                    /* level/depth in tree                 */
   char        type;                   /* entry type                          */
   char        stype;                  /* entry link/blocked                  */
   /*---(perms)--------------------------*/
   ushort      uid;                    /* dir entry user id                   */
   char        own;                    /* dir entry permissions for user      */
   ushort      gid;                    /* dir entry group id                  */
   char        grp;                    /* dir entry permissions for group     */
   char        oth;                    /* dir entry permissions for other     */
   char        super;                  /* whether suid or guid is set         */
   /*---(unique)-------------------------*/
   uchar       drive;                  /* unique drive/partition/mpoint       */
   int         inode;                  /* dir entry filesystem inode          */
   int         dnode;                  /* dir entry filesystem inode          */
   /*---(times)--------------------------*/
   int         changed;                /* dir entry last update timestamp     */
   /*---(size)---------------------------*/
   char        size;                   /* exp of bytes                        */
   long        bytes;                  /* bytes in entry                      */
   long        bcum;                   /* bytes in entry and below            */
   short       count;                  /* number of entries in dir            */
   int         ccum;                   /* number of below full branch         */
   /*---(categorization)-----------------*/
   char        cat;                    /* mime-like category                  */
   char        ext         [LEN_HUND]; /* mime-like extension                 */
   /*---(name)---------------------------*/
   uchar       ascii;                  /* quality of name                     */
   uchar       len;                    /* length of name                      */
   char        name        [LEN_HUND]; /* name                                */
};

struct cPTRS {
   /*---(payload)------------------------*/
   tENTRY      *data;
   /*---(parent connection)--------------*/
   tPTRS      *parent;                 /* actual containing dir entry         */
   tPTRS      *s_next;                 /* siblings under same parent          */
   tPTRS      *s_prev;                 /* siblings under same parent          */
   /*---(child connections)--------------*/
   tPTRS      *c_head;                 /* first entry under a directory       */
   tPTRS      *c_tail;                 /* last entry under a directory        */
   int         nchild;                 /* number of entries in directory      */
   /*---(oveall connection)--------------*/
   tPTRS      *m_next;                 /* master list of entries             */
   tPTRS      *m_prev;                 /* master list of entries              */
   /*---(done)---------------------------*/
};
extern      tPTRS      *h_ptrs;
extern      tPTRS      *t_ptrs;
extern      int         g_nptrs;
extern      int         g_uptrs;



extern      tPTRS      *dir_stack  [MAX_DEPTH];
extern      tPTRS      *root_stack [MAX_DEPTH];


#define     MAX_CONFS   100
typedef     struct      cCONFS    tCONFS;
struct cCONFS {
   uchar       type;
   uchar       path        [LEN_HUND];
   int         len;
   int         uses;
   uchar       reason      [LEN_DESC];
};
extern      tCONFS      g_confs [MAX_CONFS];
extern      int         g_nconf;

typedef  struct cBUCKET tSLOT;
struct cBUCKET {
   int         count;
   tPTRS      *head;
   tPTRS      *tail;
};
#define     SEVENBIT      128


/*---(content)--------------*/
#define     MIME_TOTAL       'T'
#define     MIME_AUDIO       'a'
#define     MIME_VIDEO       'v'
#define     MIME_IMAGE       'i'
#define     MIME_CODE        's'
#define     MIME_ASCII       't'
#define     MIME_DBASE       'b'
#define     MIME_CRYPT       'c'
#define     MIME_PROP        'p'
#define     MIME_EXEC        'x'
#define     MIME_DIR         'd'
#define     MIME_JUNK        'j'
#define     MIME_OTHER       'o'
#define     MIME_HUH         '?'
#define     MIME_PRIV        '!'
#define     MIME_EMPTY       'e'
#define     MIME_ALL         "avistbcpxdjo?!e"
#define     MIME_MEDIA       'M'
#define     MIME_WORK        'W'
#define     MIME_TEMP        'T'
#define     MIME_OPTS        " --audio --video --image --source --text --base --crypt --prop --exec --dir --junk --other --huh --media --work --temp "
#define     MIME_NEGS        " --noaudio --novideo --noimage --nosource --notext --nobase --nocrypt --noprop --noexec --nodir --nojunk --noother --nohuh --nomedia --nowork --notemp "



/*> typedef     struct      cCAT        tCAT;                                         <* 
 *> struct cCAT {                                                                     <* 
 *>    uchar       name        [LEN_TERSE];                                           <* 
 *>    uchar       cat;                                                               <* 
 *>    uchar       desc        [LEN_DESC];                                            <* 
 *>    int         n_seen;                                                            <* 
 *>    llong       b_seen;                                                            <* 
 *>    int         n_kept;                                                            <* 
 *>    llong       b_kept;                                                            <* 
 *>    int         n_found;                                                           <* 
 *>    llong       b_found;                                                           <* 
 *> };                                                                                <*/


#define     MAX_CAT        50
#define     MAX_MIME      500
typedef     struct      cMIME       tMIME;
struct cMIME {
   /*---(master)------------*/
   char        level;
   char        ext         [LEN_TERSE];
   char        cat;
   short       tie;
   char        desc        [LEN_DESC];
   char        like;
   /*---(seen)--------------*/
   short       u_seen;
   int         n_seen;
   llong       b_seen;
   /*---(kept)--------------*/
   short       u_kept;
   int         n_kept;
   llong       b_kept;
   /*---(found)-------------*/
   short       u_found;
   int         n_found;
   llong       b_found;
   /*---(done)--------------*/
};
extern    tMIME       g_cats [MAX_CAT];
extern    int         g_ncat;
extern    tMIME       g_mime [MAX_MIME];
extern    int         g_nmime;

#define    MAX_LEVEL    99

struct cGLOBAL {
   /*---(yJOBS)--------------------------*/
   char        run_as;                      /* khronos, eos, heracles, ...    */
   char        run_mode;                    /* verify, install, audit, ...    */
   char        run_file    [LEN_PATH];      /* file to act on                 */
   char        heartbeat   [LEN_HUND];      /* heartbeat                      */
   /*---(run control)--------------------*/
   char        mode;                        /* run mode                       */
   char        report;                      /* report type                    */
   char        layout;                      /* match report types             */
   char        delimit     [LEN_SHORT];     /* match report types             */
   char        columns     [LEN_LABEL];     /* specific columns               */
   char        pub;                         /* allow only public findings     */
   char        headers;                     /* format output as report        */
   char        lineno;                      /* format with line numbers       */
   llong       empty;
   /*------------------------------------*/
   char        progname    [LEN_TITLE];     /* run-name of program            */
   char        host        [LEN_DESC]; /* host name of current computer       */
   long        runtime;                /* run time for helios                 */
   int         maxlevel;               /* maximum recursion on entry gather   */          
   char        conf;                   /* whether to process conf file        */
   char        updatedb;               /* update mode flag (y/-)              */
   char        verbose;                /* show dirs as traversing             */
   int         uid;                    /* users user id                       */
   int         gid;                    /* users group id                      */
   /*---(regex search)-------------------*/
   uchar       regex       [MAX_REGEX];/* regex text pattern                  */
   int         regex_len;              /* regex text pattern length           */
   char        count;                  /* count rather than show results      */
   int         total;                  /* total matches                       */
   /*---(filtering)----------------------*/
   uchar       types       [LEN_LABEL];     /* filter on entry type           */
   uchar       mimes       [LEN_LABEL];     /* filter on mime categories      */
   uchar       sizes       [LEN_LABEL];     /* filter on entry byte size      */
   uchar       ages        [LEN_LABEL];     /* filter on entry age/days       */
   uchar       ascii       [LEN_LABEL];     /* filter on name quality         */
   uchar       super;                       /* filter for both suid/guid      */
   uchar       ext         [LEN_TERSE];     /* filter specific extension      */
   int         r_uid;                       /* filter uid                     */
   int         r_gid;                       /* filter gid                     */
   /*---(search filters)-----------------*/
   int         limit;                  /* max number of finds                 */
   int         number;                 /* which entry to present              */
   char        find;                   /* apply find filters                  */
   char        find_cat;               /* mime cat for search                 */
   char        find_mime   [10];       /* mime type for search                */
   char        find_days;              /* search files by days since update   */
   char        find_size;              /* search files by general size        */
   char        find_name;              /* search files by name quality        */
   /*---(summary stats)------------------*/
   char        mime_table;             /* show summary mime statistics        */
   char        statistics;             /* show summary database statistics    */
   char        dump;                   /* dump all recorts as read            */
   char        path        [LEN_PATH]; /* begin search using this path        */
   tPTRS      *start;                  /* begin search from this location     */
   int         level;                  /* begin search at this level          */
   char        dirtree;                /* display directory tree (kept)       */
   char        dir_all;                /* display mime-based tree (seen)      */
   char        mimetree;               /* display mime-based tree (kept/found)*/
   char        mime_all;               /* display mime-based tree (seen)      */
   char        mpoint      [LEN_FULL]; /* mountpoint to inventory             */
   short       drive;                  /* currently processed drive           */
   char        file_conf   [LEN_PATH]; /* alternative mime file               */
   char        file_mime   [LEN_PATH]; /* alternative mime file               */
   char        file_data   [LEN_PATH]; /* alternative database                */
   char        begin       [LEN_PATH]; /* starting point for database         */
   /*---(mime type processing)-----------*/
   int         mime_exe;               /* mime index for executable           */
   int         mime_other;             /* mime index for unlabeled            */
   int         mime_guess;             /* mime index for not recognized       */
   int         mime_link;              /* mime index for a file link          */
   int         mime_conf;              /* mime index for a conf link          */
   /*---(output control)-----------------*/
   char        show_cat;               /* show file mime cat in regex output  */
   char        show_mime;              /* show file mime type in regex output */
   char        show_days;              /* show days ago in regex output       */
   char        show_size;              /* show file size in regex output      */
   char        show_bytes;             /* show file bytes in regex output     */
   char        show_level;             /* show file level in regex output     */
   char        show_ascii;             /* show file name quality        */
   /*---(done)---------------------------*/
};
typedef     struct     cGLOBAL      tGLOBAL;
extern      tGLOBAL    my;

extern char  g_print    [LEN_RECD];

/*---(running modes)------------------*/
#define     MODE_UPDATE         'u'
#define     MODE_SEARCH         's'
#define     MODE_REPORT         'r'
/*---(reporting options)--------------*/
#define     REPORT_MATCHES      '-'
#define     REPORT_DIRTREE      'D'
#define     REPORT_MIMESUM      'm'
#define     REPORT_MIMETREE     'M'
#define     REPORT_DATABASE     's'
/*---(output columns)-----------------*/
#define     LAYOUT_SILENT       '-'
#define     LAYOUT_DEFAULT      'n'
#define     LAYOUT_MIME         'm'
#define     LAYOUT_PREFIX       'p'
#define     LAYOUT_DETAIL       'd'
#define     LAYOUT_ANALYSIS     'a'
#define     LAYOUT_GYGES        'g'
#define     LAYOUT_COUNT        'c'
#define     LAYOUT_OPTIONS      " --silent --default --mime --detail --analysis --gyges --count --indent "
#define     LAYOUT_QUIETS       "-c"
/*---(column options)-----------------*/
#define     COL_MIME            'm'
#define     COL_SIZE            's'
#define     COL_LEVEL           'l'
#define     COL_NAMING          'n'
#define     COL_FIND            'f'
#define     COL_TYPE            't'
#define     COL_PERMS           'p'
#define     COL_DRIVE           'd'
#define     COL_BASE            'b'
#define     COL_OPTIONS         " --show-mime --show-age --show-AGE --show-size --show-SIZE --show-SIZES --show-level --show-ascii --show-find --show-type --show-TYPE --show-perms --show-PERMS --show-drive --show-DRIVE --show-base --show-rsh "
/*---(destinations)-------------------*/
#define     DEST_FILE           'f'
#define     DEST_STDOUT         's'
#define     DEST_TREE           't'



extern int     g_target;
extern int     g_looked;
extern int     g_matched;
extern tENTRY *g_found;
extern char    g_path    [LEN_RECD];


extern int   s_days;
extern char  s_age;
extern int   s_pos;
extern int   s_len;

extern      char          unit_answer [LEN_RECD];


/*===[[ HELIOS_MAIN.C ]]======================================================*/
int         main               (int argc, char *argv[]);


/*===[[ HELIOS_PROG.C ]]======================================================*/
/*---(support)--------------*/
char*       PROG_version            (void);
char        PROG_vershow            (void);
char        PROG_reset              (void);
/*---(preinit)--------------*/
char        PROG__header            (void);
char        PROG_urgents            (int a_argc, char *a_argv []);
/*---(startup)--------------*/
char        PROG__init              (int argc, char *argv[]);
char        PROG__args              (int argc, char *argv[]);
char        PROG__begin             (void);
char        PROG_startup            (int a_argc, char *a_argv []);
/*---(driver)---------------*/
char        PROG_driver             (void);
/*---(shutdown)-------------*/
char        PROG__end               (void);
char        PROG_shutdown           (void);
/*---(testing)--------------*/
char        PROG__unit_loud         (void);
char        PROG__unit_mute         (void);
char        PROG__unit_quiet        (void);
char        PROG__unit_end          (void);
/*---(done)-----------------*/


/*===[[ HELIOS_FILE.C ]]======================================================*/
/*---(program)--------------*/
char        DB_init                 (void);
/*---(header)---------------*/
char        DB__head_write_one      (FILE *a_file, char a_label [LEN_TERSE], int a_var);
char        DB__head_write          (FILE *a_file, char a_name [LEN_LABEL], char a_ver [LEN_SHORT], int a_nconf, int a_nmime, int a_ndrive, int a_nentry, char a_heart [LEN_DESC]);
char        DB__head_read_one       (FILE *a_file, char a_label [LEN_TERSE], int *r_var);
char        DB__head_read           (FILE *a_file, char r_name [LEN_LABEL], char r_ver [LEN_SHORT], int *r_nconf, int *r_nmime, int *r_ndrive, int *r_nentry, char r_heart [LEN_DESC]);
/*---(file)-----------------*/
char        DB__open                (cchar a_name [LEN_PATH], char a_mode, FILE **r_file);
char        DB__close               (FILE **b_file);
/*---(drives)---------------*/
char        DB__drive_write         (FILE *a_file);
char        DB__drive_read          (FILE *a_file, int n);
/*---(entries)--------------*/
char        DB__entry_write         (FILE *a_file, tENTRY *a_entry, int *a_count);
char        DB__entry_read          (FILE *a_file, int *a_count);
/*---(directories)----------*/
char        DB__dir_write           (FILE *a_file, tPTRS *a_dir, int *a_count);
/*---(drivers)--------------*/
char        DB_write                (char *a_name, int *a_count);
char        DB_read                 (char *a_name, int *a_count);
/*---(reading)--------------*/
char        DB_commas             (llong a_number, char *a_string);



/*===[[ HELIOS_MIME.C ]]======================================================*/
/*---(mass)-----------------*/
char        MIME_reset_all          (void);
char        MIME_reset_found        (void);
char        MIME__purge             (void);
/*---(program)--------------*/
char        MIME_init               (void);
/*---(action)---------------*/
char        MIME_find               (char a_ext [LEN_TERSE], int *r_itotal, char r_total [LEN_TERSE], int *r_icat, char r_cat [LEN_TERSE], char *r_ccat, int *r_iext);

char        MIME__action            (char a_ext [LEN_TERSE], char a_action, llong a_bytes, char *r_ccat);
char        MIME_add_seen           (char a_ext [LEN_TERSE], llong a_bytes, char *r_ccat);
char        MIME_add_kept           (char a_ext [LEN_TERSE], llong a_bytes);
char        MIME_add_found          (char a_ext [LEN_TERSE], llong a_bytes);
char        MIME_del_seen           (char a_ext [LEN_TERSE], llong a_bytes);
/*---(input)----------------*/
char        MIME__parse_essential   (char *a_recd, char *a_flag, char **s);
char        MIME__single            (short n, char a_level, char a_ext [LEN_TERSE], char a_cat, char a_desc [LEN_DESC], char a_like);
char        MIME_standard           (void);
char        MIME__handler           (int a_line, uchar *a_verb);
char        MIME_pull               (cchar a_name [LEN_PATH]);
/*---(output)---------------*/
char        MIME__write_clear       (void);
char        MIME__write_title       (FILE *f, char a_type);
char        MIME__write_line        (FILE *f, char a_type, uchar *a_name, uchar a_cat, uchar *a_desc, uchar a_like, int a_nseen, llong a_bseen, int a_nkept, llong a_bkept);
char        MIME__write_columns     (FILE *f, char a_type);
char        MIME__write_category    (FILE *f, char a_type, char a_cat);
char        MIME_report             (char a_name [LEN_PATH]);
/*---(tree)-----------------*/
char        MIME__tree_line         (char a_type, uchar *a_ext, llong a_size, llong a_count, char *a_desc);
char        MIME_tree               (void);
char        MIME_all                (void);
char        MIME_found              (void);
/*---(database)-------------*/
char        MIME_db_write           (FILE *a_file);
char        MIME_db_read            (FILE *a_file, short a_count);
/*---(unittest)-------------*/
char*       MIME__unit              (char *a_question, char *a_ext, int n);
/*---(done)-----------------*/



/*---(config)---------------*/
char        CONF__purge             (void);
char        CONF_init               (void);
char        CONF__duplicate         (uchar *a_path);
char        CONF__handler           (int a_line, cchar a_verb [LEN_LABEL]);
char        CONF_pull               (cchar a_name [LEN_PATH]);

char        CONF_find               (char *a_full, char *a_name, char *a_stype, char *a_silent);
char        CONF_private            (uchar *a_path);
/*---(database)-------------*/
char        CONF_db_write           (FILE *a_file);
char        CONF_db_read            (FILE *a_file, short a_count);
/*---(unittest)-------------*/
char*       CONF__unit              (char *a_question, int n);
/*---(done)-----------------*/


/*===[[ HELIOS_ENTRY.C ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        ENTRY__wipe             (tPTRS *a_curr);
/*---(malloc)---------------*/
char        ENTRY__new              (tPTRS **a_new, tPTRS *a_parent, tDRIVE *a_drive);
char        ENTRY__free             (tPTRS **a_ptrs);
/*---(simplify)-------------*/
char        ENTRY_root              (tPTRS **a_new, tDRIVE *a_drive);
char        ENTRY_fullroot          (tPTRS **a_new, tDRIVE *a_drive);
char        ENTRY_normal            (tPTRS **a_new, tPTRS *a_parent);
char        ENTRY_submount          (tPTRS **a_new, tPTRS *a_parent, tDRIVE *a_drive);
/*---(program)--------------*/
char        ENTRY_init              (void);
char        ENTRY__purge            (void);
char        ENTRY__dirclear         (tPTRS *a_from);
char        ENTRY_wrap              (void);
/*---(checks)---------------*/
char        ENTRY__name_check       (cchar *a_name, char *a_warn, uchar *a_len);
char        ENTRY__type_check       (cchar *a_name, tSTAT *a_stat, uchar *a_stype, uchar *a_type);
char        ENTRY__perms_check      (tSTAT *a_stat, ushort *a_uid, char *a_own, ushort *a_gid, char *a_grp, char *a_oth, char *a_super);
/*---(populate)-------------*/
int         ENTRY__populate         (tPTRS *a_ptrs, char *a_full);
char        ENTRY_manual            (tENTRY *a_entry, char *a_name, char a_type, char a_stype, char a_cat, char *a_ext);
/*---(levels)---------------*/
char        ENTRY__level_prep       (tPTRS *a_parent, char *a_path, char *a_newpath, char *a_begin);
char        ENTRY__level_read       (tPTRS  *a_ptrs, char *a_path, char a_silent);
/*---(?????)----------------*/
char        ENTRY_tail              (tDRIVE *a_drive, tPTRS *a_root);
char        ENTRY_start             (void);
/*> tPTRS*      DATA_start              (char *a_path);                               <*/
/*---(walker)---------------*/
char        ENTRY_walk              (char a_trigger, void *a_callback);
/*---(unit_test)------------*/
char*       ENTRY__unit             (char *a_question, int n);
char*       TREE__unit              (tPTRS *a_focus, char *a_question, int n);


/*===[[ HELIOS_SORT.C ]]======================================================*/
/*---(gnome)----------------*/
char        GNOME_comp         (char a_type1, char *a_name1, char a_type2, char *a_name2);
char        GNOME_move         (tPTRS **a_head, tPTRS **a_tail, tPTRS *a_ins, tPTRS *a_bef);
char        GNOME_sort         (int a_level, tPTRS **a_head, tPTRS **a_tail);
/*---(postal)---------------*/
char        GNOME_scatter      (int a_level, tPTRS **a_head, tPTRS **a_tail, tSLOT a_slots [SEVENBIT]);
char        GNOME_gather       (int a_level, tPTRS **a_head, tPTRS **a_tail, tSLOT a_slots [SEVENBIT]);
/*> char        GNOME_bucket       (int a_level, tPTRS **a_head, tPTRS **a_tail);     <*/
char        api_ysort_init          (void);




/*===[[ HELIOS_FILTER.C ]]====================================================*/
/*---(types)----------------*/
char        FILTER_type_all         (void);
char        FILTER_type_none        (void);
char        FILTER_type_pos         (char *a_option);
char        FILTER_type_neg         (char *a_option);
char        FILTER_type_direct      (char *a_types);
char        FILTER_by_type          (char a_type, char a_stype);
/*---(mimes)----------------*/
char        FILTER_mime_all         (void);
char        FILTER_mime_none        (void);
char        FILTER_mime_pos         (char *a_option);
char        FILTER_mime_neg         (char *a_option);
char        FILTER_mime_direct      (char *a_mimes);
char        FILTER_by_mime          (char a_cat, char *a_ext);
/*---(sizes)----------------*/
char        FILTER_size_all         (void);
char        FILTER_size_none        (void);
char        FILTER_size_add         (char *a_option);
char        FILTER_size_sub         (char *a_option);
char        FILTER_size_direct      (char *a_sizes);
char        FILTER_by_size          (char a_size);
/*---(ages)-----------------*/
char        FILTER_age_all          (void);
char        FILTER_age_none         (void);
char        FILTER_age_pos          (char *a_option);
char        FILTER_age_neg          (char *a_option);
char        FILTER_age_direct       (char *a_ages);
char        FILTER_by_age           (long a_days);
/*---(ascii)----------------*/
char        FILTER_ascii_all        (void);
char        FILTER_ascii_none       (void);
char        FILTER_ascii_pos        (char *a_option);
char        FILTER_ascii_neg        (char *a_option);
char        FILTER_ascii_direct     (char *a_ascii);
char        FILTER_by_ascii         (char a_ascii);
/*---(perms)----------------*/
char        FILTER__perms           (char *a_func, char a_type, int a_uid, int a_gid, char a_own, char a_grp, char a_oth, char *a_reason);
char        FILTER_by_perms         (int a_uid, int a_gid, char a_own, char a_grp, char a_oth, char *r_reason);
char        FILTER_by_dir           (int a_uid, int a_gid, char a_own, char a_grp, char a_oth, char *r_reason);
/*---(regex)----------------*/
char        FILTER__regex           (uchar *a_regex);
char        FILTER_by_regex         (uchar *a_string);
/*---(done)-----------------*/



/*===[[ HELIOS_RPTG.C ]]======================================================*/
char        RPTG_init               (void);
/*---(types)----------------*/
/*> char        RPTG_config_types_all   (void);                                       <*/
/*> char        RPTG_config_types_none  (void);                                       <*/
/*> char        RPTG_config_types_set   (uchar *a_types);                             <*/
/*> char        RPTG_config_types_add   (uchar *a_option);                            <*/
/*> char        RPTG_filter_type        (uchar a_type, uchar a_stype);                <*/
/*---(mimes)----------------*/
/*> char        RPTG_config_mimes_all   (void);                                       <*/
/*> char        RPTG_config_mimes_none  (void);                                       <*/
/*> char        RPTG_config_mimes_add   (uchar *a_option);                            <*/
/*> char        RPTG_config_mimes_sub   (uchar *a_option);                            <*/
/*> char        RPTG_config_mimes_set   (uchar *a_mimes);                             <*/
/*> char        RPTG_filter_mime        (uchar a_cat, uchar *a_ext);                  <*/
/*---(sizes)----------------*/
/*> char        RPTG_config_sizes_all   (void);                                       <*/
/*> char        RPTG_config_sizes_none  (void);                                       <*/
/*> char        RPTG_config_sizes_add   (uchar *a_option);                            <*/
/*> char        RPTG_config_sizes_sub   (uchar *a_option);                            <*/
/*> char        RPTG_filter_size        (uchar a_size);                               <*/
/*---(ages)-----------------*/
/*> char        RPTG_config_ages_all    (void);                                       <*/
/*> char        RPTG_config_ages_none   (void);                                       <*/
/*> char        RPTG_config_ages_add    (uchar *a_option);                            <*/
/*> char        RPTG_config_ages_sub    (uchar *a_option);                            <*/
/*> char        RPTG_config_ages_set    (uchar *a_ages);                              <*/
/*> char        RPTG_filter_age         (long a_days);                                <*/
/*---(ascii)----------------*/
/*> char        RPTG_config_ascii_all   (void);                                       <*/
/*> char        RPTG_config_ascii_none  (void);                                       <*/
/*> char        RPTG_config_ascii_add   (uchar *a_option);                            <*/
/*> char        RPTG_config_ascii_sub   (uchar *a_option);                            <*/
/*> char        RPTG_config_ascii_set   (uchar *a_ascii);                             <*/
/*> char        RPTG_filter_ascii       (uchar a_ascii);                              <*/
/*---(perms)----------------*/
/*> char        RPTG_perms_filter       (int a_uid, char a_own, int a_gid, char a_grp, char a_oth);   <*/
/*> char        RPTG_perms_dir          (int a_uid, char a_own, int a_gid, char a_grp, char a_oth);   <*/
/*---(super)----------------*/
char        RPTG_config_super_off   (void);
char        RPTG_config_super_on    (void);
char        RPTG_filter_super       (uchar a_super);
/*---(regex)----------------*/
/*> char        RPTG_regex_prep         (uchar *a_regex);                             <*/
/*> char        RPTG_regex_filter       (uchar *a_string);                            <*/
/*---(output)---------------*/
char        RPTG_col_none           (void);
char        RPTG_col_singles        (uchar *a_option);
char        RPTG_col_layouts        (uchar *a_option);
/*---(reporting-------------*/
char        RPTG__title             (void);
char        RPTG__break             (char a_force);
char        RPTG__columns           (char a_force);
char        RPTG__line              (tENTRY *a_data, char *a_full);
char        RPTG_driver             (tENTRY *a_data, char *a_full);
char        RPTG_footer             (void);
/*---(walker)---------------*/
char        RPTG__callback          (char a_serious, tENTRY *a_data, char *a_full);
char        RPTG_walker             (char a_trigger);


char        RPTG_regex              (int a_level, tPTRS *a_ptrs, char *a_path);
char        RPTG_dirtree            (void);
char        RPTG_summ               (void);
char        RPTG__change_modtime    (char *a_file, int a_days);
char        RPTG__create_file       (char a_how, char *a_src, char *a_dst, int a_days, int a_perms, char *a_own, char *a_grp);
char*       RPTG__unit              (char *a_question, int n);

char        DRIVE__mtab_read        (void);
char        DRIVE_mtab_find         (cchar *a_path, tDRIVE **a_drive, char *a_mtab, char *a_follow, char *a_device, char *a_type);
char        DRIVE_init              (void);
char        DRIVE__action           (char n, char a_action, llong a_bytes, tDRIVE **a_drive);
char        DRIVE_add_seen          (char a_ref, llong a_bytes);
char        DRIVE_add_kept          (char a_ref, llong a_bytes);
char        DRIVE_add_found         (char a_ref, llong a_bytes);
char        DRIVE_wrap              (void);
char        DRIVE_new               (tDRIVE **a_drive);
char        DRIVE__remove           (tDRIVE **a_drive);
char        DRIVE__purge            (void);
char        DRIVE_manual            (tDRIVE **a_drive, uchar a_ref, char *a_host, char *a_serial, char *a_device, char *a_mpoint, char *a_type, llong a_size, int a_written);
/*> char        DRIVE__mtab             (cchar *a_mount, char *a_part, char *a_type);   <*/
char        DRIVE__stats            (cchar *a_part, llong *a_size, char *a_serial);
char        DRIVE_populate          (tDRIVE **a_drive, char *a_mount, long a_time, char *a_index);
char        DRIVE_inventory         (void);
char        DRIVE_list              (void);
char*       DRIVE__unit             (char *a_question, int n);


char       *ySTR_sub           (char *a_source, int a_beg, int a_len, char a_mode);


char        helios_yjobs            (cchar a_req, cchar *a_data);

/*===[[ HELIOS_EXT.C ]]=======================================================*/
/*--------- *---------------------- *-----------------------------------------*/
/*---(first)----------------*/
char        EXT__gentoo             (cchar a_full [LEN_PATH], char **r_ext);
char        EXT__git                (cchar a_full [LEN_PATH], char **r_ext);
char        EXT__symlink            (char a_stype, char a_type, char **r_ext);
char        EXT__device             (char a_type, char **r_ext);
char        EXT__tilde              (cchar a_name [LEN_HUND], char **b_ext);
char        EXT__suffix             (cchar a_name [LEN_HUND], char **r_ext);
/*---(second)---------------*/
char        EXT__hidden             (cchar a_name [LEN_HUND], char **b_ext);
char        EXT__exec               (cchar a_name [LEN_HUND], tSTAT *a_stat, char **b_ext);
char        EXT__manual             (cchar a_name [LEN_HUND], char **b_ext);
char        EXT__unitc              (cchar a_name [LEN_HUND], char **b_ext);
char        EXT__libs_two           (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], tSTAT *a_stat, char **r_ext);
char        EXT__exec_two           (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], char **b_ext);
char        EXT__cleanup            (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], char **b_ext);
/*---(driver)---------------*/
char        EXT_categorize          (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], tSTAT *a_stat, char a_stype, char a_type, long a_bytes, char *r_cat, char *r_ext);
/*---(done)-----------------*/



#endif
/*===============================[[ end-code ]]===============================*/
