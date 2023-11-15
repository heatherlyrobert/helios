/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */

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

const char *EXT_DIR     = "d_dir";
const char *EXT_DHIDDEN = "d_hidden";
const char *EXT_DGIT    = "d_gitroot";

const char *EXT_CONF    = "t_conf";

const char *EXT_EXEC    = "x_exec";
const char *EXT_DEBUG   = "x_debug";
const char *EXT_UNIT    = "x_unit";
const char *EXT_MAKEF   = "makefile";
const char *EXT_MMAKE   = "mmake";
const char *EXT_ADEBUG  = "a_debug";
const char *EXT_SODEBUG = "so_debug";
const char *EXT_JEXEC   = "j_exec";
const char *EXT_JLIB    = "j_lib";
const char *EXT_JUNIT   = "j_unit";
const char *EXT_JC      = "j_c";

const char *EXT_BDEV    = "b_dev";
const char *EXT_CDEV    = "c_dev";
const char *EXT_FIFO    = "f_fifo";
const char *EXT_SOCK    = "s_socket";

const char *EXT_DLINK   = "d_link";
const char *EXT_RLINK   = "r_link";
const char *EXT_ELINK   = "x_link";
const char *EXT_CLINK   = "c_link";
const char *EXT_BLINK   = "b_link";
const char *EXT_FLINK   = "f_link";
const char *EXT_SLINK   = "s_link";
const char *EXT_ULINK   = "u_link";

const char *EXT_BACKUP  = "b_tilde";
const char *EXT_UNKNOWN = "o_ext";
const char *EXT_MYSTERY = "o_none";
const char *EXT_MANUAL  = "manual";
const char *EXT_OHIDDEN = "o_hidden";
const char *EXT_XHIDDEN = "x_hidden";

const char *EXT_GIT     = "o_git";
const char *EXT_PORTAGE = "o_portage";
const char *EXT_KERNEL  = "o_kernel";
const char *EXT_CACHE   = "o_cache";

const char *EXT_SCAN    = "i_scan";
const char *EXT_PHOTO   = "i_photo";
const char *EXT_WALL    = "i_wall";

const char *EXT_PRIVATE = "!_priv";
const char *EXT_EMPTY   = "e_empty";


static char    s_ext   [LEN_TERSE] = "";




/*====================------------------------------------====================*/
/*===----                        first pass                            ----===*/
/*====================------------------------------------====================*/
static void      o___FIRST______________o (void) {;}

char
EXT__gentoo             (cchar a_full [LEN_PATH], char **r_ext)
{  /*
    *  everything within these directories is the same, i do not differentiate
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(quick-out)----------------------*/
   --rce;  if (r_ext == NULL)  return rce;
   /*---(portage)------------------------*/
   if        (strncmp (a_full, "/var/lib/portage/"   , 17) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling portage related I");
      *r_ext = EXT_PORTAGE;
      return 1;
   } else if (strncmp (a_full, "/var/tmp/portage/"   , 17) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling portage related II");
      *r_ext = EXT_PORTAGE;
      return 1;
   } else if (strncmp (a_full, "/var/log/portage/"   , 17) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling portage related III");
      *r_ext = EXT_PORTAGE;
      return 1;
   } else if (strncmp (a_full, "/var/db/pkg/"        , 12) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling portage related IV");
      *r_ext = EXT_PORTAGE;
      return 1;
   } else if (strncmp (a_full, "/var/db/repos/"      , 14) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling portage related V");
      *r_ext = EXT_PORTAGE;
      return 1;
   } else if (strncmp (a_full, "/usr/share/portage/" , 19) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling portage related VI");
      *r_ext = EXT_PORTAGE;
      return 1;
   }
   /*---(transient)----------------------*/
   else if (strncmp (a_full, "/usr/src/"        ,  9) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling kernel source related");
      *r_ext = EXT_KERNEL;
      return 1;
   }
   /*---(transient, BUT UNIT TESTING)----*/
   else if   (strncmp (a_full, "/tmp/",  5) == 0 && my.run_as == IAM_HELIOS) {
      DEBUG_RPTG   yLOG_note    ("handling global cache related I");
      *r_ext = EXT_CACHE;
      return 1;
   }
   /*---(transient)----------------------*/
   else if (strncmp (a_full, "/run/"            ,  5) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling global cache related II");
      *r_ext = EXT_CACHE;
      return 1;
   } else if (strncmp (a_full, "/var/run/"        ,  9) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling global cache related III");
      *r_ext = EXT_CACHE;
      return 1;
   } else if (strncmp (a_full, "/var/cache/"      , 11) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling global cache related IV");
      *r_ext = EXT_CACHE;
      return 1;
   } else if (strncmp (a_full, "/var/tmp/"        ,  9) == 0) {
      DEBUG_RPTG   yLOG_note    ("handling global cache related V");
      *r_ext = EXT_CACHE;
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__gentoo related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__git                (cchar a_full [LEN_PATH], char **r_ext)
{  /*
    *  everything under a /.git is git related and i do not differentiate
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(quick-out)----------------------*/
   --rce;  if (r_ext == NULL)  return rce;
   /*---(check)--------------------------*/
   if (strstr (a_full, "/.git/") != NULL) {
      *r_ext = EXT_GIT;
      /*---(done)------------------------*/
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__git related");
   /*---(complete)-----------------------*/
   return 0;
}


char
EXT__symlink            (char a_stype, char a_type, char **r_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(quick-out)----------------------*/
   --rce;  if (r_ext == NULL)  return rce;
   /*---(check)--------------------------*/
   if (a_stype == STYPE_LINK) {
      /*---(handle)----------------------*/
      DEBUG_ENVI   yLOG_note    ("handle as symlink entry");
      switch (a_type) {
      case   ENTRY_DIR    :  *r_ext = EXT_DLINK;      break;
      case   ENTRY_REG    :  *r_ext = EXT_RLINK;      break;
      case   ENTRY_CDEV   :  *r_ext = EXT_CLINK;      break;
      case   ENTRY_BDEV   :  *r_ext = EXT_BLINK;      break;
      case   ENTRY_FIFO   :  *r_ext = EXT_FLINK;      break;
      case   ENTRY_SOCK   :  *r_ext = EXT_SLINK;      break;
      default             :  *r_ext = EXT_ULINK;      break;
      }
      /*---(done)------------------------*/
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__symlink related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__device             (char a_type, char **r_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(quick-out)----------------------*/
   --rce;  if (r_ext == NULL)  return rce;
   /*---(check)--------------------------*/
   if (a_type != 0 && strchr (ENTRY_DEVICES, a_type) != NULL) {
      /*---(handle)----------------------*/
      DEBUG_ENVI   yLOG_note    ("handle device/special entry");
      switch (a_type) {
      case   ENTRY_DIR    :  *r_ext = EXT_DIR;        break;
      case   ENTRY_CDEV   :  *r_ext = EXT_CDEV;       break;
      case   ENTRY_BDEV   :  *r_ext = EXT_BDEV;       break;
      case   ENTRY_FIFO   :  *r_ext = EXT_FIFO;       break;
      case   ENTRY_SOCK   :  *r_ext = EXT_SOCK;       break;
      default             :  return 0;                break;
      }
      /*---(done)------------------------*/
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__device related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__tilde              (cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   DEBUG_RPTG   yLOG_note    (__FUNCTION__);
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   /*---(handle)-------------------------*/
   DEBUG_ENVI   yLOG_info    ("a_name"    , a_name);
   l = strlen (a_name);
   DEBUG_ENVI   yLOG_value   ("l"         , l);
   if (a_name [l - 1] == '~') {
      DEBUG_ENVI   yLOG_note    ("handle backup/cache entry");
      *b_ext  = EXT_BACKUP;
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__tilde related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__suffix             (cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        c           =    0;
   char       *p           = NULL;
   char        x_name      [LEN_HUND]  = "";
   char        l           =    0;
   int         i           =    0;
   int         x_mime      =    0;
   DEBUG_RPTG   yLOG_note    (__FUNCTION__);
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   if (*b_ext != NULL)         return 0;
   /*---(prepare)------------------------*/
   ystrlcpy (x_name, a_name, LEN_HUND);
   l = strlen (x_name);
   if (l <= 2)  return 0;
   c = ystrldcnt (x_name, '.', LEN_HUND);
   if (c <= 0)  return 0;
   DEBUG_RPTG   yLOG_note    ("test for extension");
   DEBUG_RPTG   yLOG_value   ("dots"      , c);
   /*---(rightmost)----------------------*/
   for (i = 0;  i < 3;  ++i) {
      /*---(find extension)--------------*/
      p = strrchr (x_name, '.');
      if (p == NULL)  return 0;
      /*---(filter)----------------------*/
      l = p - x_name;
      if (l <= 1)  return 0;   /* no room for base name  */
      /*---(update)----------------------*/
      p [0] = '\0';
      l = strlen (++p);
      if (l >  9)  continue;   /* too long for an extension */
      DEBUG_RPTG   yLOG_complex ("testing"   , "%d[%s]", l, p);
      /*---(handle)----------------------*/
      if (l > 0) {
         rc = MIME_find (p, NULL, NULL, NULL, NULL, NULL, &x_mime);
         DEBUG_RPTG   yLOG_complex ("mime"      , "%4d rc, %d x_mime", rc, x_mime);
         if (rc >= 0 && x_mime >= 0) {
            ystrlcpy (s_ext, p, LEN_TERSE);
            *b_ext = s_ext;
            return i + 1;
         }
      }
      /*---(done)------------------------*/
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__suffix related");
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       second pass                            ----===*/
/*====================------------------------------------====================*/
static void      o___SECOND_____________o (void) {;}

char
EXT__hidden             (cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   /*---(files)--------------------------*/
   /* only unidenfified files are assigned to hidden                          */
   if (*b_ext == NULL && a_name [0] == '.') {
      DEBUG_ENVI   yLOG_note    ("handle hidden file");
      *b_ext = EXT_OHIDDEN;
      return 1;
   }
   /*---(directories)--------------------*/
   if (*b_ext == EXT_DIR  && strcmp (a_name, ".git") == 0) {
      DEBUG_ENVI   yLOG_note    ("git root directory");
      *b_ext = EXT_DGIT;
      return 1;
   }
   if (*b_ext == EXT_DIR  && a_name [0] == '.') {
      DEBUG_ENVI   yLOG_note    ("handle hidden directory");
      *b_ext = EXT_DHIDDEN;
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__hidden related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__exec               (cchar a_name [LEN_HUND], tSTAT *a_stat, char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   /*---(check)--------------------------*/
   if (*b_ext == NULL ||  *b_ext == EXT_OHIDDEN) {
      if (  (a_stat->st_mode & S_IXUSR)  ||
            (a_stat->st_mode & S_IXGRP)  ||
            (a_stat->st_mode & S_IXOTH))  {
         DEBUG_ENVI   yLOG_note    ("switching to an executable entry");
         if      (*b_ext == EXT_OHIDDEN)  *b_ext  = EXT_XHIDDEN;
         else {
            l = strlen (a_name);
            if (l >= 7 && strcmp ("_debug", a_name + l - 6) == 0) {
               *b_ext = EXT_DEBUG;
            } else if (l >= 6 && strcmp ("_unit" , a_name + l - 5) == 0) {
               *b_ext = EXT_UNIT;
            } else if (strcmp ("Makefile" , a_name) == 0) {
               *b_ext = EXT_MAKEF;
            } else if (strcmp ("Mmake"    , a_name) == 0) {
               *b_ext = EXT_MMAKE;
            } else {
               *b_ext = EXT_EXEC;
            }
         }
         return 1;
      }
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__exec related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__manual             (cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         l           =    0;
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   if (*b_ext != NULL)         return 0;
   /*---(check for manuals)---------------*/
   l = strlen (a_name);
   /*> DEBUG_RPTG   yLOG_value   ("l"         , l);                                   <*/
   /*> if (l >= 7) {                                                                  <* 
    *>    DEBUG_RPTG   yLOG_char    ("l-6"       , a_name [l - 6]);                   <* 
    *>    DEBUG_RPTG   yLOG_info    ("l-6"       , a_name + l - 5);                   <* 
    *> }                                                                              <*/
   if (l >= 3 && a_name [l - 2] == '.') {
      DEBUG_ENVI   yLOG_note    ("handling normal manual extension");
      if (strchr ("123456789", a_name [l - 1]) != NULL) {
         *b_ext = EXT_MANUAL;
         return 1;
      }
   }
   else if (l >= 7 && a_name [l - 6] == '.') {
      DEBUG_ENVI   yLOG_note    ("handling special manual extension I");
      if (strcmp ("2type"   , a_name + l - 5) == 0) {
         *b_ext = EXT_MANUAL;
         return 1;
      }
      if (strcmp ("3type"   , a_name + l - 5) == 0) {
         *b_ext = EXT_MANUAL;
         return 1;
      }
      if (strcmp ("3head"   , a_name + l - 5) == 0) {
         *b_ext = EXT_MANUAL;
         return 1;
      }
   }
   else if (l >= 8 && a_name [l - 7] == '.') {
      DEBUG_ENVI   yLOG_note    ("handling special manual extension II");
      if (strcmp ("3const"  , a_name + l - 6) == 0) {
         *b_ext = EXT_MANUAL;
         return 1;
      }
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__manual related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__unitc              (cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   if (*b_ext == NULL)         return 0;
   /*---(handle)--------------------------*/
   if (strcmp (*b_ext, "c") == 0) {
      l = strlen (a_name);
      if (l >= 8 && strcmp ("_unit.c" , a_name + l - 7) == 0) {
         DEBUG_ENVI   yLOG_note    ("handling transcient c unit file");
         *b_ext = EXT_JC;
         return 1;
      }
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__unitc related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__libs_two           (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], tSTAT *a_stat, char **r_ext)
{ /* versions at end of libraries can hide suffixes                          */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   char        x_inst      =  '-';
   char        x_exec      =  '-';
   char        x_ext       [LEN_LABEL] = "";
   /*---(quick-out)----------------------*/
   --rce;  if (r_ext == NULL)  return rce;
   /*---(prepare)-------------------------*/
   sprintf (x_ext, " %s ", *r_ext);
   /*---(check location)------------------*/
   if      (strncmp ("/lib"           , a_full,  4) == 0)  x_inst = 'y';
   else if (strncmp ("/usr/lib"       , a_full,  8) == 0)  x_inst = 'y';
   else if (strncmp ("/usr/local/lib" , a_full, 14) == 0)  x_inst = 'y';
   else if (strncmp ("/home/system"   , a_full, 12) == 0)  x_inst = 'y';
   if (x_inst != 'y')  return 0;
   /*---(check executable)----------------*/
   if (  (a_stat->st_mode & S_IXUSR)  ||
         (a_stat->st_mode & S_IXGRP)  ||
         (a_stat->st_mode & S_IXOTH))   x_exec = 'y';
   if (x_exec != 'y')  return 0;
   /*---(check buried suffixes)-----------*/
   if (strstr (a_name, ".so.") != NULL) {
      *r_ext = "so";
      return 1;
   }
   else if (strstr (a_name, ".a." ) != NULL)  {
      *r_ext = "a";
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__libs_two related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__exec_two           (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   char        x_inst      =  '-';
   char        x_ext       [LEN_LABEL] = "";
   tSTAT       x_st;
   int         rci         = 0;             /* return code as integer         */
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   if (*b_ext == NULL)         return 0;
   /*---(check executable link)----------*/
   if (*b_ext == EXT_RLINK) {
      /*---(get the file information)-------*/
      rci = stat (a_full, &x_st);
      DEBUG_ENVI   yLOG_value   ("lstat"     , rci);
      --rce; if (rci < 0) {
         DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      if (  (x_st.st_mode & S_IXUSR)  ||
            (x_st.st_mode & S_IXGRP)  ||
            (x_st.st_mode & S_IXOTH))  {
         DEBUG_ENVI   yLOG_note    ("switching to an executable entry");
         *b_ext  = EXT_ELINK;
         return 1;
      }
   }
   /*---(prepare)-------------------------*/
   sprintf (x_ext, " %s ", *b_ext);
   /*---(libraries)-----------------------*/
   if (strstr (" so ar a dll ", x_ext) != NULL) {
      if      (strncmp ("/lib"           , a_full,  4) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/lib"       , a_full,  8) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/local/lib" , a_full, 14) == 0)  x_inst = 'y';
      if (x_inst == 'y') {
         if (strstr (a_name, "_debug.") != NULL) {
            if      (strcmp (*b_ext, "so") == 0) {
               *b_ext = EXT_SODEBUG;
               return 1;
            }
            else if (strcmp (*b_ext, "a" ) == 0) {
               *b_ext = EXT_ADEBUG;
               return 1;
            }
         }
      }
      else if (x_inst != 'y') {
         *b_ext = EXT_JLIB;
         return 1;
      }
   }
   /*---(executables)---------------------*/
   else if (strstr (" x_exec x_debug ", x_ext) != NULL) {
      if      (strncmp ("/bin"           , a_full,  4) == 0)  x_inst = 'y';
      else if (strncmp ("/sbin"          , a_full,  5) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/bin"       , a_full,  8) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/sbin"      , a_full,  9) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/local/bin" , a_full, 14) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/local/sbin", a_full, 15) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/opt/bin"   , a_full, 12) == 0)  x_inst = 'y';
      else if (strncmp ("/usr/opt/sbin"  , a_full, 13) == 0)  x_inst = 'y';
      if (x_inst != 'y') {
         *b_ext = EXT_JEXEC;
         return 1;
      }
   }
   /*---(unittests)-----------------------*/
   else if (strstr (" x_unit ", x_ext) != NULL) {
      if      (strncmp ("/home/system/"  , a_full, 13) == 0)  x_inst = 'y';
      if (x_inst != 'y') {
         *b_ext = EXT_JUNIT;
         return 1;
      }
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__exec_two related");
   /*---(complete)-----------------------*/
   return 0;
}

char
EXT__cleanup            (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], char **b_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   /*---(quick-out)----------------------*/
   --rce;  if (b_ext == NULL)  return rce;
   if (*b_ext != NULL)         return 0;
   /*---(handle)-------------------------*/
   l = strlen (a_name);
   if (strncmp (a_full, "/etc/", 5) == 0) {
      DEBUG_ENVI   yLOG_note    ("configuration entry");
      *b_ext  = EXT_CONF;
      return 1;
   /*> } else if (l > 0 && l <= 7) {                                                  <* 
    *>    DEBUG_ENVI   yLOG_note    ("other entry");                                  <* 
    *>    *b_ext  = EXT_UNKNOWN;                                                      <* 
    *>    return 1;                                                                   <*/
   } else {
      DEBUG_ENVI   yLOG_note    ("unknown entry");
      *b_ext  = EXT_MYSTERY;
      return 1;
   }
   /*---(failure)------------------------*/
   DEBUG_RPTG   yLOG_note    ("NOT EXT__cleanup related");
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         driver                               ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER_____________o (void) {;}

char
EXT_categorize          (cchar a_full [LEN_PATH], cchar a_name [LEN_HUND], tSTAT *a_stat, char a_stype, char a_type, long a_bytes, char *r_cat, char *r_ext)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tSTAT       x_stat;
   char       *x_ext       = NULL;
   char        x_cat       = MIME_HUH;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_name"    , a_name);
   DEBUG_ENVI   yLOG_point   ("a_stat"    , a_stat);
   --rce;  if (a_stat == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_stat = *a_stat;
   /*---(first pass)----------------------*/
   rc = 0;       rc = EXT__gentoo  (a_full   , &x_ext);
   if (rc == 0)  rc = EXT__git     (a_full   , &x_ext);
   if (rc == 0)  rc = EXT__symlink (a_stype  , a_type , &x_ext);
   if (rc == 0)  rc = EXT__device  (a_type   , &x_ext);
   if (rc == 0)  rc = EXT__tilde   (a_name   , &x_ext);
   if (rc == 0)  rc = EXT__suffix  (a_name   , &x_ext);
   /*---(second pass)---------------------*/
   rc = EXT__hidden    (a_name, &x_ext);
   rc = EXT__exec      (a_name, a_stat, &x_ext);
   rc = EXT__manual    (a_name, &x_ext);
   rc = EXT__unitc     (a_name, &x_ext);
   rc = EXT__libs_two  (a_full, a_name, a_stat, &x_ext);
   rc = EXT__exec_two  (a_full, a_name, &x_ext);
   rc = EXT__cleanup   (a_full, a_name, &x_ext);
   /*---(save)---------------------------*/
   MIME_add_seen (x_ext, a_bytes, &x_cat);
   if (r_cat != NULL)  *r_cat = x_cat;
   if (r_ext != NULL)  ystrlcpy (r_ext, x_ext, LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



