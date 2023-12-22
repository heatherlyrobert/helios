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
 * METIS § dr4·· § -v verbose option to provide feedback just like yJOBS style            § N9J1bC §  · §
 * METIS § dr2·· § take yURG_msg and yURG_err control functions from yJOBS                § N9J1l7 §  · §
 * METIS § wr4·· § unit test searches with less privilege (hide secure things)            § N9L6P9 §  · §
 * METIS § dn2·· § empty regex means fail (do not return all)                             § N9U16l §  · §
 * METIS § mg8·· § option for helios to gather and report, never use database             § N9U1Wf §  · §
 * METIS § ····· § --ext should accept comma lists also                                   § NA059M §  · §
 * METIS § ····· § merge mountpoint and start variables                                   § NA26Js §  · §
 */


tGLOBAL     my;
tAUDIT      g_audit;



/*====================------------------------------------====================*/
/*===----                       small support                          ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;}

char      verstring    [500];

char*        /*--: return versioning information ---------[ ------ [ ------ ]-*/
PROG_version            (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang      ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char
PROG_vershow            (void)
{
   printf ("%s\n", PROG_version ());
   exit (0);
}


/*====================------------------------------------====================*/
/*===----                       pre-initialization                     ----===*/
/*====================------------------------------------====================*/
static void      o___PREINIT_________________o (void) {;}

char       /*----: very first setup ------------------s-----------------------*/
PROG__header            (void)
{
   /*---(header)----------------------*/
   DEBUG_PROG   yLOG_enter (__FUNCTION__);
   /*---(versioning)------------------*/
   DEBUG_PROG   yLOG_info     ("khronos" , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_note     ("custom core");
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version      ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version     ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version      ());
   DEBUG_PROG   yLOG_note     ("custom other");
   DEBUG_PROG   yLOG_info     ("yPARSE"  , yPARSE_version    ());
   DEBUG_PROG   yLOG_info     ("ySORT"   , ySORT_version     ());
   DEBUG_PROG   yLOG_info     ("yREGEX"  , yREGEX_version    ());
   DEBUG_PROG   yLOG_note     ("job control");
   DEBUG_PROG   yLOG_info     ("yJOBS"   , yJOBS_version     ());
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
PROG_urgents            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(set mute)-----------------------*/
   /*> printf ("helios starting\n");                                                  <*/
   yURG_all_off  ();
   /*---(start logger)-------------------*/
   rc = yURG_logger  (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = PROG__header ();
   DEBUG_PROG   yLOG_value    ("header"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program startup                       ----===*/
/*====================------------------------------------====================*/
void  o___STARTUP_________o () { return; }

char
PROG_reset              (void)
{
   my.path  [0] = '\0';
   my.maxlevel  = MAX_LEVEL;
   return 0;
}

char             /* [------] minimal pre-argument program initialization -----*/
PROG__init              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*> printf ("helios init (%s)\n", a_argv [0]);                                     <*/
   /*---(security)-----------------------*/
   my.uid    = geteuid();
   my.gid    = getegid();
   DEBUG_PROG   yLOG_complex ("security"  , "%4do, %4dg", my.uid, my.gid);
   /*---(make heartbeat)-----------------*/
   yEXEC_heartbeat (getpid (), time (NULL), NULL, NULL, &(my.heartbeat));
   /*---(update database)----------------*/
   ystrlcpy (my.progname, a_argv [0], LEN_TITLE);
   DEBUG_PROG   yLOG_info    ("progname"  , my.progname);
   my.updatedb     =  '-';
   if (strcmp (my.progname, "helios_update") == 0)     my.updatedb = 'y';
   if (strcmp (my.progname, "updatedb"     ) == 0)     my.updatedb = 'y';
   DEBUG_PROG   yLOG_char    ("updatedb"  , my.updatedb);
   /*---(defaults)-----------------------*/
   my.run_as   = IAM_HELIOS;
   my.run_mode = ACT_NONE;
   ystrlcpy (my.run_file, "", LEN_PATH);
   if (my.updatedb == 'y')  ystrlcpy (my.progname, "helios", LEN_TITLE);
   /*> printf ("helios init (%s)\n", my.progname);                                    <*/
   /*---(begin)--------------------------*/
   rc = yJOBS_runas (my.progname, &(my.run_as), P_FOCUS, P_NICHE, P_SUBJECT, P_PURPOSE, P_NAMESAKE, P_PRONOUNCE, P_HERITAGE, P_BRIEFLY, P_IMAGERY, P_REASON, P_ONELINE, P_HOMEDIR, P_BASENAME, P_FULLPATH, P_SUFFIX, P_CONTENT, P_SYSTEM, P_LANGUAGE, P_COMPILER, P_CODESIZE, P_DEPSTDC, P_AUTHOR, P_CREATED, P_VERMAJOR, P_VERMINOR, P_VERNUM, P_VERTXT, NULL);
   DEBUG_PROG  yLOG_value   ("runas"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_char    ("run_as"    , my.run_as);
   rc = yPARSE_config  (YPARSE_MANUAL, NULL, YPARSE_ONETIME, YPARSE_FIELD, YPARSE_FILL);
   /*---(default modes)------------------*/
   my.mode         = MODE_SEARCH;
   my.report       = REPORT_MATCHES;
   my.layout       = LAYOUT_DEFAULT;
   my.pub          = '-';
   my.headers      = '-';
   my.lineno       = '-';
   my.empty        =   0;
   /*---(default run-time options)-------*/
   strcpy (my.host     , "-"      );
   my.runtime      = time (NULL);
   my.maxlevel     = MAX_LEVEL;
   my.conf         =  'y';
   g_nconf         =    0;
   my.dump         =  '-';
   my.dirtree      =  '-';
   my.mimetree     =  '-';
   my.mime_all     =  '-';
   my.mime_table   =  '-';
   my.statistics   =  '-';
   /*---(output prepending)--------------*/
   my.show_cat     =  '-';
   my.show_mime    =  '-';
   my.show_days    =  '-';
   my.show_size    =  '-';
   my.show_bytes   =  '-';
   my.show_level   =  '-';
   /*---(non-regex filtering)------------*/
   my.limit        = 999999;
   my.number       =   -1;
   my.find         =  '-';
   my.find_cat     =  '-';
   my.find_mime[0] = '\0';
   my.find_days    =  '-';
   my.find_size    =  '-';
   /*---(regex handling)-----------------*/
   ystrlcpy (my.regex, "", MAX_REGEX);
   my.regex_len    =    0;
   my.count        =  '-';
   my.total        =    0;
   /*---(others)-------------------------*/
   ystrlcpy (my.mpoint, "/", LEN_FULL);
   DB_init     ();
   ENTRY_init  ();
   CONF_init   ();
   my.path  [0]    = '\0';
   my.start        = NULL;
   /*---(drives)-------------------------*/
   h_drive = NULL;
   t_drive = NULL;
   g_ndrive = 0;
   g_udrive = 0;
   /*---(reporting)----------------------*/
   RPTG_init  ();
   yREGEX_clear ();
   /*---(database)-----------------------*/
   strncpy (my.file_conf, FILE_CONF, LEN_PATH);
   strncpy (my.file_mime, FILE_MIME, LEN_PATH);
   strncpy (my.file_data, FILE_DB  , LEN_PATH);
   for (i = 0; i < MAX_DEPTH; ++i)  root_stack [i] == NULL;
   for (i = 0; i < MAX_DEPTH; ++i)  dir_stack  [i] == NULL;
   my.level        =   0;
   DEBUG_CONF  yLOG_value ("g_nmime"   , g_nmime);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] process the command line arguments --------------*/
PROG__args              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *a           = NULL;          /* current argument               */
   char       *b           = NULL;          /* next argument                  */
   int         x_total     =    0;          /* total arg/urg count            */
   int         x_args      =    0;          /* argument count                 */
   char        two_arg     =    0;
   int         x_max       =    0;
   /*---(begin)--------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*> printf ("helios args\n");                                                      <*/
   /*> printf ("helios run_mode (%c)\n", my.run_mode);                                <*/
   yURG_msg ('>', "command line arguments handling...");
   yURG_msg ('-', "total of %d arguments, including name", a_argc);
   /*---(check for no args)--------------*/
   DEBUG_PROG   yLOG_value   ("a_argc"    , a_argc);
   /*> if (a_argc == 1) {                                                             <* 
    *>    DEBUG_PROG   yLOG_exit    (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <*/
   /*---(process)------------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(prepare)---------------------*/
      a = a_argv [i];
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_PROG   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      /*---(debugging--------------------*/
      ++x_total;
      if (a [0] == '@')       continue;
      ++x_args;
      DEBUG_ARGS  yLOG_info  ("argument"  , a);
      /*---(yJOBS argument check)--------*/
      ++x_args;
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      rc = yJOBS_argument (&i, a, b, &(my.run_as), &(my.run_mode), my.run_file);
      DEBUG_ARGS  yLOG_value    ("rc"        , rc);
      if (rc > 0)  continue;
      /*---(version)---------------------*/
      if      (strcmp (a, "--version"      ) == 0 || strcmp (a, "-V") == 0) {
         PROG_version ();
         printf ("%s\n", verstring);
         return -1;
      }
      /*---(verbose)---------------------*/
      else if (strcmp (a, "-v"             ) == 0 && i + 1 < a_argc)   rc = yJOBS_argument (&i, "--vnormal", NULL, &(my.run_as), &(my.run_mode), my.run_file);
      /*---(configuration)---------------*/
      else if (strcmp (a, "--config"       ) == 0 && i + 1 < a_argc)   strncpy (my.file_conf, a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--noconf"       ) == 0)                     my.conf = 'n';
      else if (strcmp (a, "--database"     ) == 0 && i + 1 < a_argc)   strncpy (my.file_data, a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--mimefile"     ) == 0 && i + 1 < a_argc)   strncpy (my.file_mime, a_argv [++i], LEN_PATH);
      /*---(filters)---------------------*/
      else if (strcmp (a, "--public"       ) == 0)                     my.pub  = 'y';
      else if (strcmp (a, "--types"        ) == 0 && i + 1 < a_argc)   FILTER_type_direct  (a_argv [++i]);
      else if (strstr (ENTRY_OPTIONS    , a) != NULL)                  FILTER_type_pos     (a);
      else if (strstr (ENTRY_NEGS       , a) != NULL)                  FILTER_type_neg     (a);
      else if (strcmp (a, "--mimes"        ) == 0 && i + 1 < a_argc)   FILTER_mime_direct (a_argv [++i]);
      else if (strstr (MIME_OPTS        , a) != NULL)                  FILTER_mime_pos     (a);
      else if (strstr (MIME_NEGS        , a) != NULL)                  FILTER_mime_neg     (a);
      else if (strcmp (a, "--ext"          ) == 0 && i + 1 < a_argc) { snprintf (my.ext, LEN_HUND, " %s ", a_argv [++i]);  ystrldchg (my.ext, ',', ' ', LEN_HUND);  }
      else if (strcmp (a, "--sizes"        ) == 0 && i + 1 < a_argc)   FILTER_size_direct  (a_argv [++i]);
      else if (strstr (SIZES_OPTIONS    , a) != NULL)                  FILTER_size_pos     (a);
      else if (strstr (SIZES_NEGS       , a) != NULL)                  FILTER_size_neg     (a);
      else if (strcmp (a, "--ages"         ) == 0 && i + 1 < a_argc)   FILTER_age_direct   (a_argv [++i]);
      else if (strstr (AGES_OPTIONS     , a) != NULL)                  FILTER_age_pos      (a);
      else if (strstr (AGES_NEGS        , a) != NULL)                  FILTER_age_neg      (a);
      else if (strcmp (a, "--naming"       ) == 0 && i + 1 < a_argc)   FILTER_ascii_direct (a_argv [++i]);
      else if (strstr (ASCII_OPTIONS    , a) != NULL)                  FILTER_ascii_pos    (a);
      else if (strstr (ASCII_NEGS       , a) != NULL)                  FILTER_ascii_neg    (a);
      else if (strcmp (a, "--super"        ) == 0)                     RPTG_config_super_on  ();
      /*---(root can over-ride)----------*/
      else if (strcmp (a, "--uid"          ) == 0 && i + 1 < a_argc)   my.r_uid = atoi (a_argv [++i]);
      else if (strcmp (a, "--gid"          ) == 0 && i + 1 < a_argc)   my.r_gid = atoi (a_argv [++i]);
      /*---(output control)--------------*/
      else if (strcmp (a, "--headers"      ) == 0)                     my.headers  = 'y';
      else if (strcmp (a, "--HEADERS"      ) == 0)                     my.headers  = 'Y';
      else if (strcmp (a, "--noheaders"    ) == 0)                     my.headers  = '-';
      else if (strcmp (a, "--lineno"       ) == 0)                     my.lineno   = 'y';
      else if (strcmp (a, "--nolineno"     ) == 0)                     my.lineno   = '-';
      else if (strstr (LAYOUT_OPTIONS   , a) != NULL)                  RPTG_col_layouts      (a);
      else if (strcmp (a, "--mimetree"     ) == 0)                     my.mimetree = 'y';
      else if (strcmp (a, "--mimefind"     ) == 0)                   { my.mimetree = 'f'; RPTG_col_layouts      ("--silent"); }
      else if (strcmp (a, "--mimeall"      ) == 0)                   { my.mime_all = 'y'; RPTG_col_layouts      ("--silent"); }
      else if (strcmp (a, "--dirtree"      ) == 0)                   { my.dirtree  = 'y'; RPTG_col_layouts      ("--silent"); }
      else if (strcmp (a, "--dirall"       ) == 0)                   { my.dir_all  = 'y'; RPTG_col_layouts      ("--silent"); }
      else if (strcmp (a, "--dump"         ) == 0)                     my.dump     = 'y';
      else if (strcmp (a, "--count"        ) == 0 || strcmp (a, "-c") == 0)  my.count          = 'y';
      else if (strcmp (a, "--statistics"   ) == 0 || strcmp (a, "-S") == 0)  my.statistics     = 'y';
      /*---(output control)--------------*/
      else if (strstr (COL_OPTIONS      , a) != NULL)                  RPTG_col_singles    (a);
      /*---(processing control)----------*/
      else if (strcmp (a, "-u"             ) == 0)                     my.updatedb       = 'y';
      else if (strcmp (a, "--updatedb"     ) == 0)                     my.updatedb       = 'y';
      else if (strcmp (a, "--mpoint"       ) == 0 && i + 1 < a_argc)   strncpy (my.mpoint  , a_argv [++i], LEN_FULL);
      /*---(search filtering)------------*/
      else if (strcmp (a, "--first"        ) == 0)                     my.limit    =  1 ;
      else if (strcmp (a, "--limit"        ) == 0 && i + 1 < a_argc)   my.limit    = atoi (a_argv[++i]);
      else if (strcmp (a, "--number"       ) == 0 && i + 1 < a_argc)   my.number   = atoi (a_argv[++i]);
      else if (strcmp (a, "--start"        ) == 0 && i + 1 < a_argc)   strncpy (my.path    , a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--depth"        ) == 0 && i + 1 < a_argc)   my.maxlevel = atoi (a_argv[++i]);
      /*---(regex search)----------------*/
      else if (strcmp (a, "--all"          ) == 0 || strcmp (a, "-A") == 0)  strncpy (my.regex, ".", MAX_REGEX);
      else if (a[0] != '-')    FILTER__regex (a);
      /*---(not understood)--------------*/
      else {
         printf  ("option <<%s>> not understood or implemented\n", a);
         DEBUG_PROG   yLOG_exit    (__FUNCTION__);
         return -1;
      }
      /*---(done)------------------------*/
   }
   /*---(max name)-----------------------*/
   IF_RUNNING {
      rc = yEXEC_maxname (a_argc, a_argv, &x_max);
      /*> printf ("x_max = %d\n", x_max);                                             <*/
      ystrlcpy (a_argv [0], P_ONELINE, x_max);
   }
   /*---(fix limits)---------------------*/
   if (my.maxlevel <=  0)       my.maxlevel = MAX_LEVEL;
   if (my.maxlevel >  99)       my.maxlevel = MAX_LEVEL;
   if (my.limit    <= 0)        my.limit    = 999999;
   if (my.limit    >  999999)   my.limit    = 999999;
   /*---(catch security over-rides)------*/
   DEBUG_PROG   yLOG_complex ("security"  , "%4dro, %4drg", my.r_uid, my.r_gid);
   --rce;  if (my.uid != 0) {
      DEBUG_ARGS  yLOG_note    ("can only over-ride security as root");
      my.r_uid = -1;
      my.r_gid = -1;
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(display urgents)----------------*/
   DEBUG_ARGS  yLOG_note    ("summarization of argument processing");
   DEBUG_ARGS  yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS  yLOG_value   ("arguments" , x_args);
   DEBUG_ARGS  yLOG_char    ("conf"      , my.conf);
   DEBUG_ARGS  yLOG_char    ("update"    , my.updatedb);
   DEBUG_ARGS  yLOG_value   ("depth"     , my.maxlevel);
   DEBUG_ARGS  yLOG_char    ("verbose"   , my.verbose);
   DEBUG_ARGS  yLOG_char    ("dump"      , my.dump);
   DEBUG_ARGS  yLOG_char    ("mime_table", my.mime_table);
   DEBUG_ARGS  yLOG_note    ("output filtering");
   DEBUG_ARGS  yLOG_char    ("find"      , my.find);
   DEBUG_ARGS  yLOG_char    ("find-cat"  , my.find_cat);
   DEBUG_ARGS  yLOG_info    ("find-mime" , my.find_mime);
   DEBUG_ARGS  yLOG_char    ("find-days" , my.find_days);
   DEBUG_ARGS  yLOG_char    ("find-size" , my.find_size);
   DEBUG_ARGS  yLOG_note    ("output prepending");
   DEBUG_ARGS  yLOG_char    ("show-days" , my.show_days);
   DEBUG_ARGS  yLOG_char    ("show-mime" , my.show_mime);
   DEBUG_ARGS  yLOG_char    ("show-size" , my.show_size);
   DEBUG_ARGS  yLOG_char    ("show-bytes", my.show_bytes);
   DEBUG_ARGS  yLOG_char    ("show-level", my.show_level);
   DEBUG_ARGS  yLOG_char    ("statistics", my.statistics);
   DEBUG_ARGS  yLOG_info    ("path"      , my.path);
   DEBUG_ARGS  yLOG_char    ("dirtree"   , my.dirtree);
   DEBUG_ARGS  yLOG_char    ("mimetree"  , my.mimetree);
   DEBUG_ARGS  yLOG_info    ("mpoint"    , my.mpoint);
   DEBUG_ARGS  yLOG_info    ("regex"     , my.regex);
   DEBUG_ARGS  yLOG_value   ("limit"     , my.limit);
   DEBUG_ARGS  yLOG_info    ("ext"       , my.ext);
   /*---(if no mode, its reporting)------*/
   /*> printf ("helios run_mode (%c)\n", my.run_mode);                                <*/
   --rce;  if (my.run_mode == ACT_NONE) {
      /*> printf ("helios run mode not set\n");                                       <*/
      if (my.updatedb == 'y') {
         DEBUG_PROG  yLOG_note  ("no run mode selected, but updatedb requested");
         rc = yJOBS_argument (&i, "--gather", NULL, &(my.run_as), &(my.run_mode), my.run_file);
         /*> printf ("helios changing to gather (%d)\n", rc);                         <*/
      } else {
         DEBUG_PROG  yLOG_note  ("no run mode selected, force reporting");
         rc = yJOBS_argument (&i, "--normal", NULL, &(my.run_as), &(my.run_mode), my.run_file);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] post-argument program initialization ------------*/
PROG__begin             (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*> printf ("helios begin\n");                                                     <*/
   MIME_init   ();
   api_ysort_init ();
   DRIVE_init   ();
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   /*> yLOG_stage   ('m');                                                            <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)----------------------*/
   yURG_stage_check (YURG_BEG);
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = PROG__init   (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(arguments)----------------------*/
   rc = PROG__args   (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("args"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(begin)--------------------------*/
   rc = PROG__begin  ();
   DEBUG_PROG   yLOG_value    ("begin"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   yURG_stage_check (YURG_MID);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        main program                          ----===*/
/*====================------------------------------------====================*/
void  o___DRIVER__________o () { return; }

char
PROG_driver             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(update)-------------------------*/
   DEBUG_PROG   yLOG_char    ("updatedb"  , my.updatedb);
   if (my.updatedb == 'y') {
      rc = DRIVE_inventory ();
   }
   /*---(reporting)----------------------*/
   else if (my.mimetree == 'y') {
      DB_read      (my.file_data, &c);
      MIME_tree    ();
   }
   else if (my.dirtree == 'y') {
      DB_read    (my.file_data, &c);
      if (rc >= 0)  rc = ENTRY_start ();
      /*> if (my.path != NULL)  DATA_start (my.path);                                 <*/
      RPTG_dirtree ();
   }
   else if (my.statistics == 'y') {
      DB_read    (my.file_data, &c);
      RPTG_summ  ();
   }
   else if (my.mime_table == 'y' && strcpy (my.regex, "") == 0) {
      DB_read    (my.file_data, &c);
      MIME_report   ("stdout");
   }
   else if (my.dump == 'y') {
      DB_read    (my.file_data, &c);
   }
   /*---(search)-------------------------*/
   else {
      /*> rc = CONF_read  ();                                                         <*/
      DEBUG_PROG   yLOG_char    ("conf read" , rc);
      if (rc < 0) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = DB_read    (my.file_data, &c);
      if (rc >= 0)  rc = ENTRY_start ();
      if (rc >= 0)  rc = RPTG_walker (WALK_ALL);
      if (my.total > 0 && my.layout != LAYOUT_COUNT)  RPTG_footer ();
      MIME_report (my.file_mime);
      if      (my.mimetree == 'f')  MIME_tree    ();
      else if (my.mime_all == 'y')  MIME_tree    ();
   }
   /*---(check for trouble)--------------*/
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        program shutdown                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

char                /* PURPOSE : shutdown program and free memory ------------*/
PROG__end               (void)
{
   /*> yLOG_stage   ('w');                                                            <*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   DRIVE_wrap ();
   yREGEX_clear ();
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_shutdown           (void)
{
   /*---(stage-check)--------------------*/
   yURG_stage_check (YURG_END);
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   PROG__end ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   DEBUG_PROG   yLOGS_end    ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        program shutdown                      ----===*/
/*====================------------------------------------====================*/
void  o___SHUTDOWN________o () { return; }




/*===============================[[ end-code ]]===============================*/
