/*===============================[[ beg-code ]]===============================*/

/*===[[ HEADER ]]=============================================================*/
/*
 *   focus         : ((tbd))
 *   application   : helios-phaeton    (greek titan of the sun)
 *   purpose       : provide file location, change, and search services
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (wicked, limitless, universal, and everlasting)
 *   dependencies  : none
 *   size          : small       (approximately 1,000 slocL)
 * 
 *   author        : rsheatherly
 *   created       : 2014-10
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
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
 *      -- ability to gather sizes below, but not entries below (dir_over)
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

/*---(filesystems)--------------*/
#include    <dirent.h>       /* POSIX  (11) opendir, readdir, alphasort       */
#include    <fcntl.h>        /* UNIX   (06) creat, fcntl, open                */
#include    <sys/stat.h>     /* UNIX   : fstat, umask                         */
#include    <sys/ioctl.h>    /* C_ANSI :                                      */
#include    <linux/fs.h>     /* LINUX  :                                      */
#include    <linux/hdreg.h>  /* LINUX  :                                      */

/*---(other standard)-----------*/
#include    <time.h>         /* C_ANSI (28) time, localtime, strftime, timer  */
#include    <regex.h>        /* POSIX  (04) regcomp, regexec, regfree         */

/*===[[ CUSTOM LIBRARIES ]]===================================================*/
#include    <yLOG.h>         /* CUSTOM (35) heatherly program logging/tracing */




/*---(struct typedefs)-------------------*/
typedef struct dirent    tDIRENT;
typedef struct FILE      tFILE;
typedef struct stat      tSTAT;

typedef unsigned char    uchar;
typedef unsigned short   ushort;
typedef long long        llong;



/* rapidly evolving version number to aid with visual change confirmation     */
#define     VER_NUM     "01.0e"
#define     VER_TXT     "hardlink helios.conf to source code directory"


#define     FILE_CONF   "/etc/helios.conf"
#define     FILE_DB     "/var/lib/helios/helios.db"
#define     FILE_MIME   "/var/lib/helios/helios.mime"


#define     MAX_STR     50
#define     MAX_NAME   100
#define     MAX_REGEX  200
#define     MAX_RECD  2000
#define     MAX_DEPTH   20



/*===[[ DEBUGGING SETUP ]]====================================================*/
/* this is my latest standard format, vars, and urgents                       */
/* v3.0b : added signal handling                                (2014-feb-01) */
struct cDEBUG
{
   /*---(handle)-------------------------*/
   int         logger;                 /* log file so that we don't close it  */
   /*---(overall)------------------------*/  /* abcdefghi_kl__opq_stu__x__    */
   /* f = full urgents turns on all standard urgents                          */
   /* k = kitchen sink and turns everything, i mean everything on             */
   /* q = quiet turns all urgents off including the log itself                */
   char        tops;                   /* t) broad structure and context      */
   char        summ;                   /* s) statistics and analytical output */
   /*---(startup/shutdown)---------------*/
   char        args;                   /* a) command line args and urgents    */
   char        conf;                   /* c) configuration handling           */
   char        prog;                   /* p) program setup and teardown       */
   /*---(file processing)----------------*/
   char        inpt;                   /* i) text/data file input             */
   char        inptM;                  /* i) text/data file input   (mas/more)*/
   char        outp;                   /* o) text/data file output            */
   char        outpM;                  /* o) text/data file output  (mas/more)*/
   /*---(event handling)-----------------*/
   char        loop;                   /* l) main program event loop          */
   char        user;                   /* u) user input and handling          */
   char        apis;                   /* z) interprocess communication       */
   char        sign;                   /* x) os signal handling               */
   char        scrp;                   /* b) scripts and batch operations     */
   char        hist;                   /* h) history, undo, redo              */
   /*---(program)------------------------*/
   char        graf;                   /* g) grahpics, drawing, and display   */
   char        data;                   /* d) complex data structure handling  */
   char        envi;                   /* e) environment processing           */
   char        enviM;                  /* E) environment processing (mas/more)*/
   /*---(specific)-----------------------*/
   char        sort;                   /* debugging directory sort            */
   char        mime;                   /* debugging assignment of mime info   */
   char        perms;                  /* debugging access permissions        */
   char        drive;                  /* debugging drive                     */
   /*---(done)---------------------------*/
};
typedef     struct      cDEBUG       tDEBUG;
extern      tDEBUG      debug;

#define     DEBUG_TOPS          if (debug.tops      == 'y')
#define     DEBUG_SUMM          if (debug.summ      == 'y')
#define     DEBUG_ARGS          if (debug.args      == 'y')
#define     DEBUG_CONF          if (debug.conf      == 'y')
#define     DEBUG_PROG          if (debug.prog      == 'y')
#define     DEBUG_INPT          if (debug.inpt      == 'y')
#define     DEBUG_INPTM         if (debug.inptM     == 'y')
#define     DEBUG_OUTP          if (debug.outp      == 'y')
#define     DEBUG_OUTPM         if (debug.outpM     == 'y')
#define     DEBUG_LOOP          if (debug.loop      == 'y')
#define     DEBUG_USER          if (debug.user      == 'y')
#define     DEBUG_APIS          if (debug.apis      == 'y')
#define     DEBUG_SIGN          if (debug.sign      == 'y')
#define     DEBUG_SCRP          if (debug.scrp      == 'y')
#define     DEBUG_HIST          if (debug.hist      == 'y')
#define     DEBUG_GRAF          if (debug.graf      == 'y')
#define     DEBUG_DATA          if (debug.data      == 'y')
#define     DEBUG_ENVI          if (debug.envi      == 'y')
#define     DEBUG_ENVIM         if (debug.enviM     == 'y')
#define     DEBUG_SORT          if (debug.sort      == 'y')
#define     DEBUG_MIME          if (debug.mime      == 'y')
#define     DEBUG_PERMS         if (debug.mime      == 'y')
#define     DEBUG_DRIVE         if (debug.drive     == 'y')


typedef     struct     cDRIVE    tDRIVE;
struct cDRIVE {
   uchar       ref;
   char        host        [20];
   char        serial      [20];
   char        device      [20];
   char        mpoint      [50];
   char        type        [10];
   llong       size;
   int         written;                /* dir entry was written on            */
   tDRIVE     *prev;
   tDRIVE     *next;
};
/*> extern      tDRIVE     drives       [10];                                         <*/
extern      tDRIVE    *h_drive;
extern      tDRIVE    *t_drive;
extern      short      n_drive;
extern      short      u_drive;


typedef     struct     cDATA     tDATA;
struct cDATA {
   /*---(types)--------------------------*/
   char        type;                   /* dir entry type                      */
   char        stype;                  /* dir entry link/blocked              */
   /*---(perms)--------------------------*/
   ushort      uid;                    /* dir entry user id                   */
   char        own;                    /* dir entry permissions for user      */
   ushort      gid;                    /* dir entry group id                  */
   char        grp;                    /* dir entry permissions for group     */
   char        oth;                    /* dir entry permissions for other     */
   /*---(unique)-------------------------*/
   uchar       drive;                  /* unique drive/partition/mpoint       */
   int         inode;                  /* dir entry filesystem inode          */
   int         dnode;                  /* dir entry filesystem inode          */
   /*---(times)--------------------------*/
   int         changed;                /* dir entry last update timestamp     */
   /*---(size)---------------------------*/
   char        size;                   /* exp of bytes                        */
   long        bytes;                  /* bytes in entry                      */
   long        cum;                    /* bytes in entry and below            */
   /*---(categorization)-----------------*/
   char        cat;                    /* mime-like category                  */
   char        ext         [   7];     /* mime-like extension                 */
   /*---(name)---------------------------*/
   char        level;                  /* recursion level of entry            */
   uchar       ascii;                  /* quality of name                     */
   uchar       len;                    /* length of name                      */
   char        name        [MAX_NAME]; /* name                                */
};

typedef     struct     cPTRS     tPTRS;
struct cPTRS {
   /*---(payload)------------------------*/
   tDATA      *data;
   /*---(parent/child connection)--------*/
   tPTRS      *parent;                 /* actual containing dir entry         */
   tPTRS      *sib_head;               /* first entry under a directory       */
   tPTRS      *sib_tail;               /* last entry under a directory        */
   tPTRS      *sib_next;               /* directory's doublly linked list     */
   tPTRS      *sib_prev;               /* directory's doublly linked list     */
   int         nchildren;              /* number of entries in directory      */
   /*---(oveall connection)--------------*/
   tPTRS      *next;                   /* overall doublly linked list         */
   tPTRS      *prev;                   /* overall doublly linked list         */
   /*---(done)---------------------------*/
};

extern      tPTRS      *h_ptrs;
extern      tPTRS      *t_ptrs;
extern      int         n_ptrs;
extern      int         n_ptrs_ever;

extern      tPTRS      *h_ptrd;
extern      tPTRS      *t_ptrd;
extern      int         n_ptrd;

extern      tPTRS      *dir_stack  [MAX_DEPTH];
extern      tPTRS      *root_stack [MAX_DEPTH];


#define     MAX_NODIR   100
typedef     struct      cNODIR    tNODIR;
struct cNODIR {
   char        type;
   char        name        [MAX_NAME];
   int         len;
};
extern      tNODIR      nodir [MAX_NODIR];
extern      int         n_nodir;


typedef  struct cBUCKET tSLOT;
struct cBUCKET {
   int         count;
   tPTRS      *head;
   tPTRS      *tail;
};
#define     SEVENBIT      128


typedef     struct      cMIME       tMIME;
struct cMIME {
   char        ext         [10];
   char        cat;
   char        desc        [50];
   char        like;
   int         seen;
   llong       sbytes;
   int         kept;
   llong       kbytes;
   int         found;
   llong       fbytes;
};
extern      tMIME       mime [500];
extern      int         n_mime;


struct cGLOBAL {
   /*------------------------------------*/
   char        host        [MAX_STR];  /* host name of current computer       */
   long        runtime;                /* run time for helios                 */
   int         maxlevel;               /* maximum recursion on entry gather   */          
   char        conf;                   /* whether to process conf file        */
   char        conf_file   [MAX_NAME]; /* alternate configuration file        */
   char        updatedb;               /* update mode flag (y/-)              */
   char        verbose;                /* show dirs as traversing             */
   int         uid;                    /* users user id                       */
   int         gid;                    /* users group id                      */
   /*---(regex search)-------------------*/
   char        regex       [MAX_REGEX];/* regex text pattern                  */
   char        regex_case;             /* ignore regex case                   */
   int         regex_len;              /* regex text pattern length           */
   regex_t     regex_comp;             /* regex pattern compilied             */
   char        regex_rc;               /* regex return code                   */
   char        count;                  /* count rather than show results      */
   int         total;                  /* total matches                       */
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
   char        path        [MAX_NAME]; /* begin search using this path        */
   tPTRS      *start;                  /* begin search from this location     */
   int         level;                  /* begin search at this level          */
   char        dirtree;                /* display directory tree              */
   char        mpoint      [MAX_STR];  /* mountpoint to inventory             */
   short       drive;                  /* currently processed drive           */
   char        database    [MAX_NAME]; /* alternative database                */
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
   /*---(statistics)---------------------*/
   int         n_audio;
   int         n_video;
   int         n_image;
   int         n_text;
   int         n_progs;
   int         n_data;
   int         n_apps;
   int         n_comp;
   int         n_model;
   int         n_other;
   int         n_unknown;
   int         n_huh;
   /*---(done)---------------------------*/
};
typedef     struct     cGLOBAL      tGLOBAL;
extern      tGLOBAL    my;


#define     OPT_UPDATE          if (my.update       == 'y')
#define     OPT_VERBOSE         if (my.verbose      == 'y')
#define     OPT_MIME            if (my.mime_table   == 'y')
#define     OPT_STATS           if (my.statistics   == 'y')



/*===[[ HELIOS_MAIN.C ]]======================================================*/
int         main               (int argc, char *argv[]);


/*===[[ HELIOS_PROG.C ]]======================================================*/
/*---(program)--------------*/
char        PROG_init          (void);
char        PROG_urgsmass      (char a_set, char a_extra);
char        PROG_urgs          (int argc, char *argv[]);
char        PROG_args          (int argc, char *argv[]);
/*> char        PROG_getdrive      (void);                                            <*/
char        PROG_conf          (void);
char        PROG_begin         (void);
char        PROG_end           (void);
/*---(testing)--------------*/
char        PROG_testquiet     (void);
char        PROG_testloud      (void);


/*===[[ HELIOS_FILE.C ]]======================================================*/
char        FWRITE_dir         (FILE *a_file, tPTRS *a_ptrs, int *a_count);
char        FWRITE_all         (tPTRS  *a_ptrs);
char        FREAD_all          (void);
char        MIME_read          (void);
char        MIME_write         (char a_dest, char a_space);
char        FILE_commas        (long long a_number, char *a_string);


/*===[[ HELIOS_ENTRY.C ]]=====================================================*/
char        DATA_gather        (int a_level, tDRIVE *a_drive, tPTRS  *a_ptrs, char *a_path, char a_keep);
tPTRS*      DATA_root          (tDRIVE *a_drive);
tPTRS*      DATA_empty         (tDRIVE *a_drive, tPTRS *a_root);
tPTRS*      DATA_start         (char *a_path);


/*===[[ HELIOS_SORT.C ]]======================================================*/
/*---(gnome)----------------*/
char        GNOME_comp         (char a_type1, char *a_name1, char a_type2, char *a_name2);
char        GNOME_move         (tPTRS **a_head, tPTRS **a_tail, tPTRS *a_ins, tPTRS *a_bef);
char        GNOME_sort         (int a_level, tPTRS **a_head, tPTRS **a_tail);
/*---(postal)---------------*/
char        GNOME_scatter      (int a_level, tPTRS **a_head, tPTRS **a_tail, tSLOT a_slots [SEVENBIT]);
char        GNOME_gather       (int a_level, tPTRS **a_head, tPTRS **a_tail, tSLOT a_slots [SEVENBIT]);
char        GNOME_bucket       (int a_level, tPTRS **a_head, tPTRS **a_tail);


/*===[[ HELIOS_RPTG.C ]]======================================================*/
char        RPTG_regex         (int a_level, tPTRS *a_ptrs, char *a_path);
char        RPTG_dirtree       (int a_level, tPTRS *a_ptrs, char *a_path);
char        RPTG_summ          (void);

char        DRIVE_host         (void);
tDRIVE     *DRIVE_append       (void);
char        DRIVE_wipe         (tDRIVE *a_drive);
char        DRIVE_populate     (tDRIVE *a_drive);

char       *ySTR_trim          (char *a_source, char a_mode);
#define       ySTR_NONE      'n'
#define       ySTR_HEAD      'h'
#define       ySTR_TAIL      't'
#define       ySTR_BOTH      'b'
#define       ySTR_SINGLE    's'
#define       ySTR_EVERY     'e'
#define       ySTR_MAX       'm'

char       *ySTR_sub           (char *a_source, int a_beg, int a_len, char a_mode);



/*===============================[[ end-code ]]===============================*/
