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

/*
 * METIS § wr2·· § make sure yJOBS confirms config file safe before reading               § N9J6B1 §  · §
 * METIS § dr2·· § make sure yJOBS checks config file before audit read                   § N9J6Fi §  · §
 * METIS § wv2·· § allow conf file name override for testing                              § N9JLai §  · §
 * METIS § mv2·· § dir_note (like dir_never) records, but ignores children                § N9U1Uh §  · §
 */

/*
 *
 *
 * status of dir_normal means helios will recurce into all sub-directories
 * and record files, attributes, and sizes
 *
 * allowed verbs...
 *
 *    dir_silent  : instructs helios to recurse into, but not record names below
 *    -             this level, not useful, just capture cumulative sizes
 *    -
 *    dir_avoid   : instructs helios to not record and not recurse into this
 *    -             path, due to value, risk, technical, or security reasons
 *    -
 *    dir_pass    : instructs helios to record, but not recurse into this path,
 *    -             due to value, risk, technical, or security reasons
 *    -
 *    dir_last    : instructs helios to recurse into, but not below this path,
 *    -             due to value, risk, technical, or security reasons
 *    -
 *    dir_only    : combines dir_last and dir_silent to recurse in and record
 *    -             one more level, then go silent
 *    -
 *    dir_never   : instructs helios to completely ignore ALL instances of
 *    -             directories using this name (example .git)
 *    -
 *    dir_private : instructs helios recurse into, but mark contents as private
 *    -             and only then seen by owner
 *    -
 *
 *    mountpoint  : provides helios with a default starting point for database
 *                  craetion and update
 *
 *
 */


tCONFS      g_confs [MAX_CONFS];
int         g_nconf;


/*---(simple file reading stats)---------*/
static    int s_seen  = 0;
static    int s_pass  = 0;
static    int s_fail  = 0;



/*====================------------------------------------====================*/
/*===----                      basic support                           ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
CONF__purge             (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_CONFS; ++i) {
      g_confs [i].type     = '-';
      ystrlcpy (g_confs [i].path  , "", LEN_HUND);
      g_confs [i].len      =   0;
      g_confs [i].uses     =   0;
      ystrlcpy (g_confs [i].reason, "", LEN_DESC);
   }
   g_nconf = 0;
   return 0;
}

char
CONF_init               (void)
{
   CONF__purge ();
   return 0;
}

char
CONF__duplicate         (uchar *a_path)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   for (i = 0; i < g_nconf; ++i) {
      if (strcmp  (a_path, g_confs [i].path) == 0) {
         DEBUG_CONF   yLOG_note    ("exact match");
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*> if (strncmp (a_path, g_confs [i].path, g_confs [i].len) == 0) {             <* 
       *>    DEBUG_CONF   yLOG_note    ("obsured by earlier rule");                   <* 
       *>    DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                           <* 
       *>    return rce;                                                              <* 
       *> }                                                                           <*/
   }
   DEBUG_CONF   yLOG_note    ("no duplicate found");
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     read from file                           ----===*/
/*====================------------------------------------====================*/
static void  o___READING_________o () { return; }

char
CONF__handler           (int a_line, cchar a_verb [LEN_LABEL])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   uchar       x_path      [LEN_HUND]  = "";
   uchar       x_reason    [LEN_DESC]  = "";
   int         i           =    0;
   int         n           =    0;
   char        t           [LEN_HUND]  = "";
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   DEBUG_CONF   yLOG_value   ("a_line"    , a_line);
   ++s_seen;
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || a_verb [0] == '\0') {
      yURG_err ('f', "%3d verb was null or empty", a_line);
      ++s_fail;
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;

   }
   DEBUG_CONF   yLOG_info    ("a_verb"    , a_verb);
   /*---(default)------------------------*/
   g_confs [g_nconf].type = STYPE_EMPTY;
   /*---(read)---------------------------*/
   rc = yPARSE_scanf (a_verb, "DO", x_path, x_reason);
   DEBUG_CONF   yLOG_complex ("returns"   , "%3d %-15.15s %-50.50s %-50.50s", a_line, a_verb, x_path, x_reason);
   --rce;  if (x_path == NULL || strcmp (x_path, "") == 0 || strlen (x_path) <= 0) {
      DEBUG_CONF   yLOG_note    ("path not found");
      sprintf (t, "%3d could not allow      å%sæ", a_line, a_verb);
      yURG_err ('f', "%-42.42s  as path was null/empty", t);
      ++s_fail;
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(mountpoint)---------------------*/
   if (strcmp (a_verb, "start") == 0) {
      DEBUG_CONF   yLOG_note    ("start default found in conf");
      sprintf (t, "%3d confirmed  å%sæ", a_line, a_verb);
      yURG_msg ('-', "%-32.32s  on path  å%sæ", t, x_path);
      if (my.mpoint [0] != '\0') {
         DEBUG_CONF   yLOG_note    ("but, start already set by arg");
      } else {
         DEBUG_CONF   yLOG_note    ("mpoint currently null");
         strcpy (my.mpoint, x_path);
      }
      DEBUG_CONF   yLOG_info    ("mpoint"    , my.mpoint);
   }
   /*---(handle avoids)------------------*/
   else if (strncmp (a_verb, "avoid", 5) == 0) {
      DEBUG_CONF   yLOG_note    ("found an avoid prefix");
      if (strcmp (a_verb, "avoid") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a avoid (but name) type");
         g_confs [g_nconf].type = STYPE_AVOID;
      }
      else if (strcmp (a_verb, "avoid_full") == 0) {
         DEBUG_CONF   yLOG_note    ("adding an avoid type");
         g_confs [g_nconf].type = STYPE_AVOID_FULL;
      }
      else if (strcmp (a_verb, "avoid_under") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a avoid after one more level type");
         g_confs [g_nconf].type = STYPE_AVOID_UNDER;
      }
      else if (strcmp (a_verb, "avoid_every") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a avoid substring match type");
         g_confs [g_nconf].type = STYPE_AVOID_EVERY;
      }
      else {
         DEBUG_CONF   yLOG_note    ("unknown avoid type");
      }
   }
   /*---(handle silent)------------------*/
   else if (strncmp (a_verb, "silent", 6) == 0) {
      DEBUG_CONF   yLOG_note    ("found a silent prefix");
      if (strcmp (a_verb, "silent") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a silent type");
         g_confs [g_nconf].type = STYPE_SILENT;
      }
      else if (strcmp (a_verb, "silent_under") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a silent after one more level type");
         g_confs [g_nconf].type = STYPE_SILENT_UNDER;
      }
      else if (strcmp (a_verb, "silent_every") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a silent match name type");
         g_confs [g_nconf].type = STYPE_SILENT_EVERY;
      }
      else if (strcmp (a_verb, "silent_when") == 0) {
         DEBUG_CONF   yLOG_note    ("adding a silent match for substring of name type");
         g_confs [g_nconf].type = STYPE_SILENT_WHEN;
      }
      else {
         DEBUG_CONF   yLOG_note    ("unknown silent type");
      }
   }
   /*---(private)------------------------*/
   else if (strcmp (a_verb, "private" ) == 0) {
      DEBUG_CONF   yLOG_note    ("adding a private type");
      g_confs [g_nconf].type = STYPE_PRIVATE;
   }
   /*---(legacy)-------------------------*/
   /*> if (strncmp (a_verb, "dir_", 4) == 0) {                                        <* 
    *>    g_confs [g_nconf].type = STYPE_EMPTY;                                       <* 
    *>    switch (a_verb[4]) {                                                        <* 
    *>    case 's' :                                                                  <* 
    *>       if (strcmp (a_verb, "dir_silent"  ) == 0) {                              <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a silent type");                   <* 
    *>          g_confs [g_nconf].type = STYPE_SILENT;                                <* 
    *>       }                                                                        <* 
    *>       break;                                                                   <* 
    *>    case 'a' :                                                                  <* 
    *>       if (strcmp (a_verb, "dir_avoid"   ) == 0) {                              <* 
    *>          DEBUG_CONF   yLOG_note    ("adding an avoid type");                   <* 
    *>          g_confs [g_nconf].type = STYPE_AVOID_FULL;                                 <* 
    *>       }                                                                        <* 
    *>       break;                                                                   <* 
    *>    case 'p' :                                                                  <* 
    *>       if (strcmp (a_verb, "dir_pass"    ) == 0) {                              <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a pass type");                     <* 
    *>          g_confs [g_nconf].type = STYPE_AVOID;                                  <* 
    *>       }                                                                        <* 
    *>       else if (strcmp (a_verb, "dir_private" ) == 0) {                         <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a private type");                  <* 
    *>          g_confs [g_nconf].type = STYPE_PRIVATE;                               <* 
    *>       }                                                                        <* 
    *>       break;                                                                   <* 
    *>    case 'l' :                                                                  <* 
    *>       if (strcmp (a_verb, "dir_last"    ) == 0) {                              <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a last type");                     <* 
    *>          g_confs [g_nconf].type = STYPE_AVOID_UNDER;                                  <* 
    *>       }                                                                        <* 
    *>       else if (strcmp (a_verb, "dir_lastplus") == 0) {                         <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a dir lastplus type");             <* 
    *>          g_confs [g_nconf].type = STYPE_SILENT_UNDER;                              <* 
    *>       }                                                                        <* 
    *>       break;                                                                   <* 
    *>    case 'n' :                                                                  <* 
    *>       if (strcmp (a_verb, "dir_never"   ) == 0) {                              <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a never type");                    <* 
    *>          g_confs [g_nconf].type = STYPE_SILENT_EVERY;                                 <* 
    *>       } else if (strcmp (a_verb, "dir_neverx"  ) == 0) {                       <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a neverx type");                   <* 
    *>          g_confs [g_nconf].type = STYPE_AVOID_EVERY;                                <* 
    *>       } else if (strcmp (a_verb, "dir_neverp"  ) == 0) {                       <* 
    *>          DEBUG_CONF   yLOG_note    ("adding a neverp type");                   <* 
    *>          g_confs [g_nconf].type = STYPE_SILENT_WHEN;                                <* 
    *>       }                                                                        <* 
    *>       break;                                                                   <* 
    *>    }                                                                           <*/
   /*---(save rest)----------------------*/
   if (strcmp (a_verb, "start") != 0 && g_confs [g_nconf].type == STYPE_EMPTY) {
      sprintf (t, "%3d could not interpret  å%sæ", a_line, a_verb);
      yURG_err ('f', "%-42.42s  on path  å%sæ", t, x_path);
      DEBUG_CONF   yLOG_note    ("not a valid dir_´´´´´ type verb");
      ++s_fail;
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   rc = CONF__duplicate (x_path);
   if (rc < 0) {
      yURG_err ('f', "%3d duplicated å%sæ on path å%sæ", a_line, a_verb, x_path);
      DEBUG_CONF   yLOG_note    ("rule path is duplicate/overlapping");
      ++s_fail;
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   sprintf (t, "%3d confirmed  å%sæ", a_line, a_verb);
   yURG_msg ('-', "%-32.32s  on path  å%sæ", t, x_path);
   strncpy (g_confs [g_nconf].path, x_path, LEN_HUND);
   g_confs [g_nconf].len = strlen (x_path);
   strncpy (g_confs [g_nconf].reason, x_reason, LEN_DESC);
   ++g_nconf;
   DEBUG_CONF   yLOG_value   ("g_nconf"   , g_nconf);
   /*---(bad verb)-----------------------*/
   /*> else {                                                                         <* 
    *>    sprintf (t, "%3d could not interpret  å%sæ", a_line, a_verb);               <* 
    *>    yURG_err ('f', "%-42.42s  on path  å%sæ", t, x_path);                       <* 
    *>    DEBUG_CONF   yLOG_note    ("not a valid dir_´´´´´ type verb");              <* 
    *>    ++s_fail;                                                                   <* 
    *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <*/
   /*---(final count)--------------------*/
   ++s_pass;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CONF_pull          (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading configuration file...");
   /*---(purge the tables)---------------*/
   rc = yPARSE_reset_in ();
   DEBUG_CONF   yLOG_value   ("purge_in"  , rc);
   rc = CONF__purge ();
   DEBUG_CONF   yLOG_value   ("purge"     , rc);
   s_seen = s_pass = s_fail = 0;
   /*---(read in configuration)----------*/
   rc = yPARSE_autoread (a_name, NULL, CONF__handler);
   DEBUG_CONF   yLOG_value   ("autoread"  , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed while openning configuration file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yPARSE_close ();
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed closing configuration file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle results)-----------------*/
   yURG_msg ('-', "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   --rce; if (s_seen <= 0) {
      yURG_err ('f', "zero configuration records found");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce; if (s_fail > 0) {
      yURG_err ('f', "ended with failed configuration records");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, configuration file read");
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       accessing rules                        ----===*/
/*====================------------------------------------====================*/
static void  o___ACCESS__________o () { return; }

char
CONF_find               (char *a_full, char *a_name, char *a_stype, char *a_silent)
{
   /*---(defense)------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   int         x_len2      =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   DEBUG_CONF   yLOG_complex ("args"      , "%p, %p, %p", a_full, a_stype, a_silent);
   /*---(defense)------------------------*/
   DEBUG_CONF    yLOG_point   ("a_full"    , a_full);
   --rce;  if (a_full == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len  = strlen (a_full);
   DEBUG_CONF    yLOG_complex ("full"      , "%2d[%s]", x_len , a_full);
   DEBUG_CONF    yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len2 = strlen (a_name);
   DEBUG_CONF    yLOG_complex ("name"      , "%2d[%s]", x_len2, a_name);
   DEBUG_CONF    yLOG_point   ("a_stype"   , a_stype);
   --rce;  if (a_stype == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF    yLOG_char    ("*a_stype"  , *a_stype);
   DEBUG_CONF    yLOG_point   ("a_silent"  , a_silent);
   --rce;  if (a_silent == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF    yLOG_char    ("*a_silent" , *a_silent);
   /*---(walk entries)-------------------*/
   DEBUG_CONF   yLOG_value   ("g_nconf"   , g_nconf);
   --rce;  for (i = 0; i < g_nconf; ++i) {
      /*---(filter)----------------------*/
      DEBUG_CONF   yLOG_info    ("dir"       , g_confs [i].path);
      /*---(name-only options)-----------*/
      if (g_confs [i].len == x_len2 && strcmp (g_confs [i].path, a_name) == 0) {
         /*---(every options)------------*/
         if      (g_confs [i].type == STYPE_AVOID_EVERY) {
            DEBUG_CONF    yLOG_note    ("found a dir_neverx rule");
            rc = 1;
            break;
         }
         else if (g_confs [i].type == STYPE_SILENT_EVERY) {
            DEBUG_CONF    yLOG_note    ("found a dir_never rule");
            *a_silent = 'y';
            rc = 2;
            break;
         }
      }
      if (g_confs [i].len <= x_len2 && strncmp (g_confs [i].path, a_name, g_confs [i].len) == 0) {
         /*---(when option)--------------*/
         if      (g_confs [i].type == STYPE_SILENT_WHEN) {
            DEBUG_CONF    yLOG_note    ("found a dir_neverP rule");
            *a_silent = 'y';
            rc = 2;
            break;
         }
      }
      /*---(full-path options)-----------*/
      if (g_confs [i].len == x_len  && strcmp (g_confs [i].path, a_full) == 0) {
         /*---(silent)-------------------*/
         if      (g_confs [i].type == STYPE_SILENT) {
            DEBUG_CONF    yLOG_note    ("found a silent rule");
            *a_silent = 'y';
            rc = 1;
            break;
         }
         else if (g_confs [i].type == STYPE_SILENT_UNDER) {
            DEBUG_CONF    yLOG_note    ("found a silent_plus rule");
            rc = 1;
            break;
         }
         /*---(avoid)--------------------*/
         else if (g_confs [i].type == STYPE_AVOID_FULL) {
            DEBUG_CONF    yLOG_note    ("found a avoid_full rule");
            rc = 1;
            break;
         }
         /*---(ignore)-------------------*/
         else if (g_confs [i].type == STYPE_AVOID) {
            DEBUG_CONF    yLOG_note    ("found a avoid rule");
            rc = 1;
            break;
         }
         /*---(last)---------------------*/
         else if (g_confs [i].type == STYPE_AVOID_UNDER) {
            DEBUG_CONF    yLOG_note    ("found a avoid_plus rule");
            rc = 1;
            break;
         }
         /*---(last)---------------------*/
         else if (g_confs [i].type == STYPE_PRIVATE) {
            DEBUG_CONF    yLOG_note    ("found a dir_private rule");
            if (my.pub == 'y') {
               rc = 1;
               *a_silent = 'p';
            }
            break;
         }
         /*---(done)---------------------*/
      }
   }
   /*---(update)-------------------------*/
   if (rc == 2) {
      *a_stype = STYPE_SILENT;
      DEBUG_CONF    yLOG_char    ("*a_stype"  , *a_stype);
      DEBUG_CONF    yLOG_char    ("*a_silent" , *a_silent);
      ++g_confs [i].uses;
   } else if (rc == 1) {
      *a_stype = g_confs [i].type;
      DEBUG_CONF    yLOG_char    ("*a_stype"  , *a_stype);
      DEBUG_CONF    yLOG_char    ("*a_silent" , *a_silent);
      ++g_confs [i].uses;
   } else {
      DEBUG_CONF    yLOG_note    ("not matching rules found");
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
CONF_private            (uchar *a_path)
{
   /*---(defense)------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF    yLOG_point   ("a_path"    , a_path);
   --rce;  if (a_path == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len  = strlen (a_path);
   DEBUG_CONF    yLOG_complex ("full"      , "%2d[%s]", x_len , a_path);
   /*---(walk entries)-------------------*/
   DEBUG_CONF   yLOG_value   ("g_nconf"   , g_nconf);
   --rce;  for (i = 0; i < g_nconf; ++i) {
      /*---(filter)----------------------*/
      DEBUG_CONF   yLOG_info    ("dir"       , g_confs [i].path);
      /*---(full-path options)-----------*/
      if (g_confs [i].len == x_len  && strcmp (g_confs [i].path, a_path) == 0) {
         /*---(private)------------------*/
         if (g_confs [i].type == STYPE_PRIVATE) {
            DEBUG_CONF    yLOG_note    ("found a dir_private rule");
            if (my.pub == 'y') {
               DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
               return rce;
            }
         }
         /*---(done)---------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      central database                        ----===*/
/*====================------------------------------------====================*/
static void  o___DATABASE________o () { return; }

char
CONF_db_write           (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "writing configuration to database...");
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      yURG_err ('f', "database for reading is not open");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP  yLOG_value   ("g_nconf"   , g_nconf);
   --rce;  if (g_nconf <= 0) {
      yURG_err ('f', "no existing/loaded data to be written");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   yURG_msg ('-', "expecting %d records", g_nconf);
   for (i = 0; i < g_nconf; ++i) {
      rc = fwrite (&(g_confs [i]), sizeof (tCONFS), 1, a_file);
      if (rc != 1)   break;
   }
   yURG_msg ('-', "wrote out %d records", i);
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("i"         , i);
   --rce;  if (g_nconf != i) {
      yURG_err ('f', "wrote fewer than the expected records");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, wrote expected count of records");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CONF_db_read            (FILE *a_file, short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   tCONFS      x_junk;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading configuration from database...");
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      yURG_err ('f', "database for reading is not open");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database is open and ready");
   /*---(purge existing)-----------------*/
   rc = CONF__purge ();
   DEBUG_CONF   yLOG_value   ("purge"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "existing data could not be purged");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "purged existing configuration");
   /*---(write all)----------------------*/
   yURG_msg ('-', "expecting %d records", a_count);
   for (i = 0; i < a_count; ++i) {
      rc = fread  (&x_junk, sizeof (tCONFS), 1, a_file);
      if (rc != 1)   break;
      ++c;
   }
   yURG_msg ('-', "read in %d records", i);
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("a_count"   , a_count);
   --rce;  if (c != a_count) {
      yURG_err ('f', "read fewer than the expected records");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, read expected count of records");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
CONF__unit              (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   tMIME      *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "file"          ) == 0) {
      snprintf (unit_answer, LEN_FULL, "CONF file name   : %2d[%s]", strlen (my.file_conf), my.file_conf);
   }
   else if (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "CONF count       : %d", g_nconf);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      if (n >= g_nconf) {
         snprintf (unit_answer, LEN_FULL, "CONF entry  (%2d) : no entry", n);
      } else {
         sprintf  (t, "[%.30s]", g_confs [n].path);
         snprintf (unit_answer, LEN_FULL, "CONF entry  (%2d) : %c %2d%-32s %2d [%.30s]", n,
               g_confs [n].type, g_confs [n].len, t, g_confs [n].uses, g_confs [n].reason);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}


