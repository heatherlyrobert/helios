/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */

tGLOBAL     my;



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

char             /* [------] minimal pre-argument program initialization -----*/
PROG__init              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(update database)----------------*/
   strlcpy (my.progname, a_argv [0], LEN_TITLE);
   DEBUG_PROG   yLOG_info    ("progname"  , my.progname);
   my.updatedb     =  '-';
   if (strcmp (my.progname, "helios_update") == 0)     my.updatedb = 'y';
   if (strcmp (my.progname, "updatedb"     ) == 0)     my.updatedb = 'y';
   DEBUG_PROG   yLOG_char    ("updatedb"  , my.updatedb);
   /*---(defaults)-----------------------*/
   my.run_as   = IAM_HELIOS;
   my.run_mode = ACT_NONE;
   strlcpy (my.run_file, "", LEN_PATH);
   /*---(begin)--------------------------*/
   rc = yJOBS_runas (a_argv [0], &(my.run_as), P_FOCUS, P_NICHE, P_SUBJECT, P_PURPOSE, P_NAMESAKE, P_PRONOUNCE, P_HERITAGE, P_BRIEFLY, P_IMAGERY, P_REASON, P_ONELINE, P_HOMEDIR, P_BASENAME, P_FULLPATH, P_SUFFIX, P_CONTENT, P_SYSTEM, P_LANGUAGE, P_COMPILER, P_CODESIZE, P_DEPSTDC, P_AUTHOR, P_CREATED, P_VERMAJOR, P_VERMINOR, P_VERNUM, P_VERTXT, NULL);
   DEBUG_PROG  yLOG_value   ("runas"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_PROG  yLOG_char    ("run_as"    , my.run_as);
   rc = yPARSE_init  ('-', NULL, '-');
   rc = yPARSE_delimiters  ("§");
   /*---(default modes)------------------*/
   my.mode         = MODE_SEARCH;
   my.report       = RPT_MATCHES;
   my.output       = OUTPUT_NORMAL;
   my.pub          = '-';
   my.headers      = '-';
   my.lineno       = '-';
   my.empty        =   0;
   /*---(default run-time options)-------*/
   strcpy (my.host     , "-"      );
   my.runtime      = time (NULL);
   my.maxlevel     =   99;   /* maxiumum */
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
   strlcpy (my.regex, ".", MAX_REGEX);
   my.regex_len    =    1;
   my.count        =  '-';
   my.total        =    0;
   /*---(others)-------------------------*/
   strlcpy (my.mpoint, "/", LEN_FULL);
   ENTRY_init  ();
   CONF_init   ();
   my.path  [0]    = '\0';
   my.start        = NULL;
   /*---(drives)-------------------------*/
   h_drive = NULL;
   t_drive = NULL;
   n_drive = 0;
   u_drive = 0;
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
   DEBUG_CONF  yLOG_value ("n_mime"    , n_mime);
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
   yURG_msg ('>', "command line arguments handling...");
   yURG_msg ('-', "total of %d arguments, including name", a_argc);
   /*---(check for no args)--------------*/
   DEBUG_PROG   yLOG_value   ("a_argc"    , a_argc);
   if (a_argc == 1) {
      DEBUG_PROG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
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
      /*---(configuration)---------------*/
      else if (strcmp (a, "-c"             ) == 0 && i + 1 < a_argc)   strncpy (my.file_conf, a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--config"       ) == 0 && i + 1 < a_argc)   strncpy (my.file_conf, a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--noconf"       ) == 0)                           my.conf = 'n';
      else if (strcmp (a, "-d"             ) == 0 && i + 1 < a_argc)   strncpy (my.file_data, a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--database"     ) == 0 && i + 1 < a_argc)   strncpy (my.file_data, a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--mimefile"     ) == 0 && i + 1 < a_argc)   strncpy (my.file_mime, a_argv [++i], LEN_PATH);
      /*---(filters)---------------------*/
      else if (strcmp (a, "--public"       ) == 0)                           my.pub  = 'y';
      else if (strcmp (a, "--types"        ) == 0 && i + 1 < a_argc)   RPTG_config_types_set (a_argv [++i]);
      else if (strstr (ENTRY_OPTIONS    , a) != NULL)                RPTG_config_types_add (a);
      else if (strstr (ENTRY_NEGS       , a) != NULL)                RPTG_config_types_sub (a);
      else if (strcmp (a, "--mimes"        ) == 0 && i + 1 < a_argc)   RPTG_config_mimes_set (a_argv [++i]);
      else if (strstr (MIME_OPTIONS     , a) != NULL)                RPTG_config_mimes_add (a);
      else if (strstr (MIME_NEGS        , a) != NULL)                RPTG_config_mimes_sub (a);
      else if (strcmp (a, "--ext"          ) == 0 && i + 1 < a_argc)   strlcpy (my.ext, a_argv [++i], LEN_TERSE);
      else if (strcmp (a, "--sizes"        ) == 0 && i + 1 < a_argc)   RPTG_config_sizes_set (a_argv [++i]);
      else if (strstr (SIZES_OPTIONS    , a) != NULL)                RPTG_config_sizes_add (a);
      else if (strstr (SIZES_NEGS       , a) != NULL)                RPTG_config_sizes_sub (a);
      else if (strcmp (a, "--ages"         ) == 0 && i + 1 < a_argc)   RPTG_config_ages_set  (a_argv [++i]);
      else if (strstr (AGES_OPTIONS     , a) != NULL)                RPTG_config_ages_add  (a);
      else if (strstr (AGES_NEGS        , a) != NULL)                RPTG_config_ages_sub  (a);
      else if (strcmp (a, "--ages"         ) == 0 && i + 1 < a_argc)   RPTG_config_ages_set  (a_argv [++i]);
      else if (strcmp (a, "--naming"       ) == 0 && i + 1 < a_argc)   RPTG_config_ascii_set (a_argv [++i]);
      else if (strstr (ASCII_OPTIONS    , a) != NULL)                RPTG_config_ascii_add (a);
      else if (strstr (ASCII_NEGS       , a) != NULL)                RPTG_config_ascii_sub (a);
      else if (strcmp (a, "--super"        ) == 0)                   RPTG_config_super_on  ();
      else if (strcmp (a, "--headers"      ) == 0)                   my.headers  = 'y';
      else if (strcmp (a, "--lineno"       ) == 0)                   my.lineno   = 'y';
      else if (strcmp (a, "--uid"          ) == 0 && i + 1 < a_argc)   my.r_uid = atoi (a_argv [++i]);
      else if (strcmp (a, "--gid"          ) == 0 && i + 1 < a_argc)   my.r_gid = atoi (a_argv [++i]);
      /*---(output control)--------------*/
      else if (strstr (OUTPUT_OPTIONS   , a) != NULL)                RPTG_config_output    (a);
      else if (strcmp (a, "--mimetree"     ) == 0)                   my.mimetree = 'y';
      else if (strcmp (a, "--mimefind"     ) == 0)                 { my.mimetree = 'f'; RPTG_config_output    ("--silent"); }
      else if (strcmp (a, "--mimeall"      ) == 0)                 { my.mime_all = 'y'; RPTG_config_output    ("--silent"); }
      else if (strcmp (a, "--dirtree"      ) == 0)                 { my.dirtree  = 'y'; RPTG_config_output    ("--silent"); }
      else if (strcmp (a, "--dirall"       ) == 0)                 { my.dir_all  = 'y'; RPTG_config_output    ("--silent"); }
      else if (strcmp (a, "--dump"         ) == 0)                   my.dump     = 'y';
      else if (strcmp (a, "--count"        ) == 0 || strcmp (a, "-c") == 0)  my.count          = 'y';
      else if (strcmp (a, "--statistics"   ) == 0 || strcmp (a, "-S") == 0)  my.statistics     = 'y';
      /*---(output control)--------------*/
      else if (strstr (COL_OPTIONS      , a) != NULL)                RPTG_config_columns   (a);
      /*---(processing control)----------*/
      else if (strcmp (a, "-u"             ) == 0)                           my.updatedb       = 'y';
      else if (strcmp (a, "--updatedb"     ) == 0)                           my.updatedb       = 'y';
      else if (strcmp (a, "--mpoint"       ) == 0 && i + 1 < a_argc)   strncpy (my.mpoint  , a_argv [++i], LEN_FULL);
      /*---(search filtering)------------*/
      else if (strcmp (a, "--first"        ) == 0)                   my.limit    =  1 ;
      else if (strcmp (a, "--limit"        ) == 0 && i + 1 < a_argc)   my.limit    = atoi (a_argv[++i]);
      else if (strcmp (a, "--number"       ) == 0 && i + 1 < a_argc)   my.number   = atoi (a_argv[++i]);
      else if (strcmp (a, "--start"        ) == 0 && i + 1 < a_argc)   strncpy (my.path    , a_argv [++i], LEN_PATH);
      else if (strcmp (a, "--depth"        ) == 0 && i + 1 < a_argc)   my.maxlevel = atoi (a_argv[++i]);
      /*---(regex search)----------------*/
      else if (strcmp (a, "--all"          ) == 0 || strcmp (a, "-A") == 0)  strncpy (my.regex, ".", MAX_REGEX);
      else if (a[0] != '-')    RPTG_regex_prep (a);
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
      printf ("x_max = %d\n", x_max);
      strlcpy (a_argv [0], P_ONELINE, x_max);
   }
   /*---(fix limits)---------------------*/
   if (my.maxlevel <=  0)       my.maxlevel = 99;
   if (my.maxlevel >  99)       my.maxlevel = 99;
   if (my.limit    <= 0)        my.limit  = 999999;
   if (my.limit    >  999999)   my.limit  = 999999;
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
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] post-argument program initialization ------------*/
PROG__begin             (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   MIME_init   ();
   api_ysort_init ();
   my.uid    = geteuid();
   my.gid    = getegid();
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
   if (my.updatedb == 'y') {
      rc = DRIVE_inventory ();
   }
   /*---(reporting)----------------------*/
   else if (my.mimetree == 'y') {
      READ_all     (my.file_data, &c);
      MIME_tree    ();
   }
   else if (my.dirtree == 'y') {
      READ_all   (my.file_data, &c);
      if (rc >= 0)  rc = ENTRY_start ();
      /*> if (my.path != NULL)  DATA_start (my.path);                                 <*/
      RPTG_dirtree ();
   }
   else if (my.statistics == 'y') {
      READ_all   (my.file_data, &c);
      RPTG_summ  ();
   }
   else if (my.mime_table == 'y' && strcmp (my.regex, "") == 0) {
      READ_all   (my.file_data, &c);
      MIME_write   ('s');
   }
   else if (my.dump == 'y') {
      READ_all   (my.file_data, &c);
   }
   /*---(search)-------------------------*/
   else {
      CONF_read  ();
      READ_all   (my.file_data, &c);
      if (rc >= 0)  rc = ENTRY_start ();
      if (rc >= 0)  rc = RPTG_walker (WALK_ALL);
      if (my.total > 0 && my.output != OUTPUT_COUNT)  RPTG_footer ();
      /*> if (my.mime_table == 'y') MIME_write   ('s', ' ');                          <*/
      MIME_write ('f');
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
