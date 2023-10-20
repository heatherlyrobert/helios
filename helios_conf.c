/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */


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
 *
 *    mountpoint  : provides helios with a default starting point for database
 *                  craetion and update
 *
 *
 */


tCONFS      g_confs [MAX_CONFS];
int         g_nconf;


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
      if (strncmp (a_path, g_confs [i].path, g_confs [i].len) == 0) {
         DEBUG_CONF   yLOG_note    ("obsured by earlier rule");
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                     read from file                           ----===*/
/*====================------------------------------------====================*/
static void  o___READING_________o () { return; }

char
CONF__read_one          (int a_line, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   uchar       x_path      [LEN_HUND]  = "";
   uchar       x_reason    [LEN_DESC]  = "";
   int         i           =    0;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(read)---------------------------*/
   rc = yPARSE_scanf (a_verb, "DO", x_path, x_reason);
   DEBUG_CONF   yLOG_complex ("returns"   , "%3d %-15.15s %-50.50s %-50.50s", a_line, a_verb, x_path, x_reason);
   /*---(defense)------------------------*/
   --rce; if (x_path == NULL || strcmp (x_path, "") == 0 || strlen (x_path) <= 0) {
      DEBUG_CONF   yLOG_note    ("path not found");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle dir related)-------------*/
   if (strncmp (a_verb, "dir_", 4) == 0) {
      g_confs [g_nconf].type = STYPE_EMPTY;
      switch (a_verb[4]) {
      case 's' :
         if (strcmp (a_verb, "dir_silent"  ) == 0) {
            DEBUG_CONF   yLOG_note    ("adding a silent type");
            g_confs [g_nconf].type = STYPE_SILENT;
         }
         break;
      case 'a' :
         if (strcmp (a_verb, "dir_avoid"   ) == 0) {
            DEBUG_CONF   yLOG_note    ("adding an avoid type");
            g_confs [g_nconf].type = STYPE_AVOID;
         }
         break;
      case 'p' :
         if (strcmp (a_verb, "dir_pass"    ) == 0) {
            DEBUG_CONF   yLOG_note    ("adding a pass type");
            g_confs [g_nconf].type = STYPE_PASS;
         }
         else if (strcmp (a_verb, "dir_private" ) == 0) {
            DEBUG_CONF   yLOG_note    ("adding a private type");
            g_confs [g_nconf].type = STYPE_PRIVATE;
         }
         break;
      case 'l' :
         if (strcmp (a_verb, "dir_last"    ) == 0) {
            DEBUG_CONF   yLOG_note    ("adding a last type");
            g_confs [g_nconf].type = STYPE_LAST;
         }
         else if (strcmp (a_verb, "dir_lastplus") == 0) {
            DEBUG_CONF   yLOG_note    ("adding a dir lastplus type");
            g_confs [g_nconf].type = STYPE_LASTPLUS;
         }
         break;
      case 'n' :
         if (strcmp (a_verb, "dir_never"   ) == 0) {
            DEBUG_CONF   yLOG_note    ("adding a never type");
            g_confs [g_nconf].type = STYPE_NEVER;
         }
         break;
      default  :
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(save rest)----------------------*/
      if (g_confs [g_nconf].type == STYPE_EMPTY) {
         DEBUG_CONF   yLOG_note    ("not a valid dir_´´´´´ type verb");
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = CONF__duplicate (x_path);
      if (rc < 0) {
         DEBUG_CONF   yLOG_note    ("rule path is duplicate/overlapping");
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strncpy (g_confs [g_nconf].path, x_path, LEN_HUND);
      g_confs [g_nconf].len = strlen (x_path);
      strncpy (g_confs [g_nconf].reason, x_reason, LEN_DESC);
      ++g_nconf;
      DEBUG_CONF   yLOG_value   ("g_nconf"   , g_nconf);
   }
   /*---(mountpoint)---------------------*/
   else if (strcmp (a_verb, "mountpoint") == 0) {
      DEBUG_CONF   yLOG_note    ("mountpoint default found in conf");
      if (my.mpoint [0] != '\0') {
         DEBUG_CONF   yLOG_note    ("but, mountpoint already set by arg");
      } else {
         DEBUG_CONF   yLOG_note    ("mpoint currently null");
         strcpy (my.mpoint, x_path);
      }
      DEBUG_CONF   yLOG_info    ("mpoint"    , my.mpoint);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CONF_read          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading configuration file...");
   /*---(purge the tables)---------------*/
   rc = CONF__purge ();
   /*---(read in configuration)----------*/
   rc = yPARSE_autoread (my.file_conf, NULL, CONF__read_one);
   --rce; if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yPARSE_close ();
   --rce; if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
         /*---(never option)-------------*/
         if (g_confs [i].type == STYPE_NEVER) {
            DEBUG_CONF    yLOG_note    ("found a dir_never rule");
            rc = 1;
            break;
         }
      }
      /*---(full-path options)-----------*/
      if (g_confs [i].len == x_len  && strcmp (g_confs [i].path, a_full) == 0) {
         /*---(silent)-------------------*/
         if (g_confs [i].type == STYPE_SILENT) {
            DEBUG_CONF    yLOG_note    ("found a dir_silent rule");
            *a_silent = 'y';
            rc = 1;
            break;
         }
         else if (g_confs [i].type == STYPE_LASTPLUS) {
            DEBUG_CONF    yLOG_note    ("found a dir_lastplus rule");
            rc = 1;
            break;
         }
         /*---(avoid)--------------------*/
         else if (g_confs [i].type == STYPE_AVOID) {
            DEBUG_CONF    yLOG_note    ("found a dir_avoid rule");
            rc = 1;
            break;
         }
         /*---(ignore)-------------------*/
         else if (g_confs [i].type == STYPE_PASS) {
            DEBUG_CONF    yLOG_note    ("found a dir_pass rule");
            rc = 1;
            break;
         }
         /*---(last)---------------------*/
         else if (g_confs [i].type == STYPE_LAST) {
            DEBUG_CONF    yLOG_note    ("found a dir_last rule");
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
   if (rc != 0) {
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
