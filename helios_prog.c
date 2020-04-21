/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */

tGLOBAL     my;


/*====================------------------------------------====================*/
/*===----                            utilities                         ----===*/
/*====================------------------------------------====================*/
void  o___UTILITIES_______o () { return; }

char        s_string    [MAX_STR];

char*        /*--> clean whitespace from a string --------[--------[--------]-*/
ySTR_trim          (char *a_source, char a_mode)
{
   /*---(design notes)-------------------*/
   /*
    *   n = none   (string untouched)
    *   h = head   (all leading spaces gone)
    *   t = tail   (all ending spaces gone)
    *   b = both   (head and tail)
    *   s = single (both plus internal not-in-strings compressed to one space)
    *   e = every  (both plus internal not-in-strings taken out)
    *   m = max    (both plus all internal taken fully out)
    *
    */
   /*---(locals)-----------+-----------+-*/
   int         i, j;                             /* iterators -- characters   */
   int         x_len       = 0;                  /* source string length      */
   int         x_len2      = 0;                  /* source string length      */
   int         x_count     = 0;                  /* whitespace counter        */
   int         x_limit     = 0;
   char        in_str      = '-';
   /*---(defense: bad source)------------*/
   strcpy (s_string, "(null)");
   if (a_source == NULL)   return NULL;
   x_len = strlen(a_source);
   strcpy (s_string, "(empty)");
   if (x_len    <= 0   )   return NULL;
   /*---(prepare)------------------------*/
   if (a_mode == 's')   x_limit = 1;
   /*---(leading whitespace)-------------*/
   x_len2 = x_len;
   if (strchr("hbsem", a_mode) != 0) {
      for (i = 0; i <= x_len2; ++i) {
         /*> printf ("%3d %c <<%s>>\n", x_len, a_source [0], a_source);               <*/
         if (a_source[0] != ' ') break;
         for (j = 0; j <= x_len; ++j)
            a_source[j] = a_source[j + 1];
         --x_len;
      }
   }
   /*---(trailing whitespace)------------*/
   if (strchr("tbsem", a_mode) != 0) {
      for (i = x_len - 1; i >= 0; --i) {
         if (a_source[i] != ' ') break;
         a_source[i] = '\0';
         --x_len;
      }
   }
   /*---(internal whitespace)------------*/
   if (strchr("esm" , a_mode) != 0) {
      for (i = 0; i <= x_len; ++i) {
         /*---(check for strings)--------*/
         if (a_mode != 'm') {
            if (in_str == 'y') {
               if (a_source[i] == '"') {
                  /*> if (i > 0 && a_source[i-1] == '\\')  continue;                  <*/
                  in_str = '-';
                  continue;
               }
               continue;
            } else {
               if (a_source[i] == '"') {
                  /*> if (i > 0 && a_source[i-1] == '\\')  continue;                  <*/
                  in_str = 'y';
                  continue;
               }
            }
         }
         /*---(check limit)--------------*/
         if (a_source[i] != ' '    )  { x_count = 0; continue; }
         if (x_count   <  x_limit)  { ++x_count;   continue; }
         /*---(compress)-----------------*/
         for (j = i; j <= x_len; ++j)
            a_source[j] = a_source[j + 1];
         --x_len;
         --i;
         /*---(done)---------------------*/
      }
   }
   /*---(prepare for return)-------------*/
   strncpy (s_string, a_source, MAX_STR);
   /*---(complete)-----------------------*/
   return a_source;
}

char*        /*===[[ pull a substring ]]====================[--------[--------]=*/
ySTR_sub           (char *a_source, int a_beg, int a_len, char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   int         i, j;                             /* iterators -- characters   */
   int         x_len       = 0;                  /* source string length      */
   int         x_max       = 0;                  /* max index                 */
   int         x_rem       = 0;                  /* space between beg and end */
   int         x_end       = 0;                  /* end point                 */
   char        x_str       [MAX_STR] = "";
   /*---(defense: null)------------------*/
   strcpy (s_string, "((null))");
   if (a_source == NULL)   return NULL;
   /*---(defense: empty)-----------------*/
   x_len = strlen(a_source);
   strcpy (s_string, "((empty))");
   if (x_len    <= 0   )   return NULL;
   x_max = x_len - 1;
   /*---(limits)-------------------------*/
   /*> printf ("x_len = %3d, a_beg = %3d, a_len = %3d, x_max = %3d, x_rem = %3d, x_end = %3d\n", x_len, a_beg, a_len, x_max, x_rem, x_end);   <*/
   if (a_beg <  0    )   a_beg = 0;
   if (a_beg >  x_max)   a_beg = x_max;
   if (a_len <  0    )   a_len = 0;
   x_rem = x_len - a_beg;
   if (a_len >  x_rem)   a_len = x_rem;
   x_end = a_beg + a_len - 1;
   /*> printf ("x_len = %3d, a_beg = %3d, a_len = %3d, x_max = %3d, x_rem = %3d, x_end = %3d\n", x_len, a_beg, a_len, x_max, x_rem, x_end);   <*/
   /*---(pull substring)-----------------*/
   i = 0;
   /*> printf ("<<");                                                                 <*/
   for (j = a_beg; j <= x_end; ++j) {
      /*> printf ("%c", a_source [j]);                                                <*/
      x_str [i] = a_source [j];
      ++i;
   }
   /*> printf (">>\n");                                                               <*/
   x_str [i] = '\0';
   /*---(trim)---------------------------*/
   ySTR_trim (x_str, a_mode);
   /*---(complete)-----------------------*/
   return s_string;
}



/*====================------------------------------------====================*/
/*===----                   standard program functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char      verstring    [500];

char*        /*--: return versioning information ---------[ ------ [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc  ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang    ]", 15);
#else
   strncpy (t, "[unknown  ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

   /*> DEBUG_ARGS  yLOG_char    ("mime"      , debug.mime);                           <* 
    *> DEBUG_ARGS  yLOG_char    ("perms"     , debug.perms);                          <*/

char             /* [------] minimal pre-argument program initialization -----*/
PROG_init          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(log header)---------------------*/
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info     ("helios"  , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version   ());
   DEBUG_PROG   yLOG_info     ("ySORT"   , ySORT_version   ());
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(default run-time options)-------*/
   strcpy (my.host     , "-"      );
   my.runtime      = time (NULL);
   my.maxlevel     =   99;   /* maxiumum */
   my.conf         =  'y';
   strcpy (my.conf_file, FILE_CONF);
   my.updatedb     =  '-';
   n_nodir         =    0;
   my.dump         =  '-';
   my.dirtree      =  '-';
   my.mimetree     =  '-';
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
   my.number       =    0;
   my.find         =  '-';
   my.find_cat     =  '-';
   my.find_mime[0] = '\0';
   my.find_days    =  '-';
   my.find_size    =  '-';
   /*---(regex handling)-----------------*/
   strncpy (my.regex, ".", MAX_REGEX);
   my.regex_len    =    1;
   my.regex_case   =  'y';
   my.count        =  '-';
   my.total        =    0;
   /*---(others)-------------------------*/
   my.mpoint [0]   = '\0';
   DATA_init   ();
   my.n_audio      =    0;
   my.n_video      =    0;
   my.n_image      =    0;
   my.n_text       =    0;
   my.n_progs      =    0;
   my.n_data       =    0;
   my.n_apps       =    0;
   my.n_comp       =    0;
   my.n_model      =    0;
   my.n_other      =    0;
   my.n_unknown    =    0;
   my.n_huh        =    0;
   my.path  [0]    = '\0';
   my.start        = NULL;
   /*---(drives)-------------------------*/
   h_drive = NULL;
   t_drive = NULL;
   n_drive = 0;
   u_drive = 0;
   /*---(database)-----------------------*/
   strncpy (my.database, FILE_DB, MAX_NAME);
   for (i = 0; i < MAX_DEPTH; ++i)  root_stack [i] == NULL;
   for (i = 0; i < MAX_DEPTH; ++i)  dir_stack  [i] == NULL;
   my.level        =   0;
   DEBUG_CONF  yLOG_value ("n_mime"    , n_mime);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] process the command line arguments --------------*/
PROG_args          (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;             /* loop iterator -- arguments     */
   char       *a           = NULL;          /* current argument               */
   int         len         = 0;             /* argument length                */
   int         x_total     = 0;
   int         x_args      = 0;
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      /*---(read)------------------------*/
      a   = argv [i];
      len = strlen(a);
      ++x_total;
      /*---(filter)----------------------*/
      if (a[0] == '@')  continue;
      ++x_args;
      DEBUG_ARGS  yLOG_info  ("argument"  , a);
      /*---(version)---------------------*/
      if      (strcmp (a, "--version"      ) == 0 || strcmp (a, "-V") == 0) {
         PROG_version ();
         printf ("%s\n", verstring);
         return -1;
      }
      /*---(configuration)---------------*/
      else if (strcmp (a, "--conf"         ) == 0)                           my.conf           = 'y';
      else if (strcmp (a, "--noconf"       ) == 0)                           my.conf           = 'n';
      else if (strcmp (a, "--database"     ) == 0 || strcmp (a, "-d") == 0) {
         if (i + 1 < argc && argv [i + 1][0] == '/') 
            strncpy (my.database, argv [++i], MAX_NAME);
      }
      /*---(output control)--------------*/
      else if (strcmp (a, "--quiet"        ) == 0 || strcmp (a, "-q") == 0)  ;
      else if (strcmp (a, "--stdio"        ) == 0 || strcmp (a, "-s") == 0)  ;
      else if (strcmp (a, "--null"         ) == 0 || strcmp (a, "-0") == 0)  ;
      else if (strcmp (a, "--verbose"      ) == 0 || strcmp (a, "-v") == 0)  my.verbose        = 'y';
      else if (strcmp (a, "--dump"         ) == 0)                           my.dump           = 'y';
      else if (strcmp (a, "--dirtree"      ) == 0)                           my.dirtree        = 'y';
      else if (strcmp (a, "--mimetree"     ) == 0)                           my.mimetree       = 'y';
      else if (strcmp (a, "--count"        ) == 0 || strcmp (a, "-c") == 0)  my.count          = 'y';
      else if (strcmp (a, "--mime-table"   ) == 0)                           my.mime_table     = 'y';
      else if (strcmp (a, "--statistics"   ) == 0 || strcmp (a, "-S") == 0)  my.statistics     = 'y';
      /*---(output control)--------------*/
      else if (strcmp (a, "--show-cat"     ) == 0)                           my.show_cat       = 'y';
      else if (strcmp (a, "--show-mime"    ) == 0)                           my.show_mime      = 'y';
      else if (strcmp (a, "--show-days"    ) == 0)                           my.show_days      = 'y';
      else if (strcmp (a, "--show-size"    ) == 0)                           my.show_size      = 'y';
      else if (strcmp (a, "--show-bytes"   ) == 0)                           my.show_bytes     = 'y';
      else if (strcmp (a, "--show-level"   ) == 0)                           my.show_level     = 'y';
      else if (strcmp (a, "--show-ascii"   ) == 0)                           my.show_ascii     = 'y';
      /*---(processing control)----------*/
      else if (strcmp (a, "--nmap"         ) == 0 || strcmp (a, "-m") == 0)  ;
      else if (strcmp (a, "--existing"     ) == 0 || strcmp (a, "-e") == 0)  ;
      else if (strcmp (a, "--follow"       ) == 0 || strcmp (a, "-L") == 0)  ;
      else if (strcmp (a, "--nofollow"     ) == 0 || strcmp (a, "-P") == 0)  ;
      else if (strcmp (a, "--updatedb"     ) == 0)                           my.updatedb       = 'y';
      else if (strcmp (a, "--mpoint"       ) == 0) {
         if (i + 1 < argc)
            strncpy (my.mpoint, argv [++i], MAX_STR);
      }
      /*---(search filtering)------------*/
      else if (strcmp (a, "--start"        ) == 0) {
         if (i + 1 < argc && argv [i + 1][0] == '/')
            strncpy (my.path    , argv [++i], MAX_NAME);
      }
      else if (strcmp (a, "--depth"        ) == 0) {
         if (i + 1 < argc)      my.maxlevel = atoi (argv[++i]);
         if (my.maxlevel == 0)  my.maxlevel = 99;

      }
      else if (strcmp (a, "--first"        ) == 0)                           my.limit          =  1 ;
      else if (strcmp (a, "--limit"        ) == 0 || strcmp (a, "-n") == 0) {
         if (i + 1 < argc)   my.limit  = atoi (argv[++i]);
         if (my.limit == 0)  my.limit  = 999999;
      }
      else if (strcmp (a, "--number"       ) == 0) {
         if (i + 1 < argc)   my.number = atoi (argv[++i]);
      }
      else if (strcmp (a, "--cat"          ) == 0) {
         if (i + 1 < argc) {
            my.find = 'y';
            my.find_cat = argv [++i][0];
         }
      }
      else if (strcmp (a, "--mime"         ) == 0) {
         if (i + 1 < argc) {
            my.find = 'y';
            strncpy (my.find_mime, argv [++i], 10);
         }
      }
      else if (strcmp (a, "--just"         ) == 0)  { my.find = 'y';  my.find_days     = 'j'; }
      else if (strcmp (a, "--week"         ) == 0)  { my.find = 'y';  my.find_days     = 'w'; }
      else if (strcmp (a, "--month"        ) == 0)  { my.find = 'y';  my.find_days     = 'm'; }
      else if (strcmp (a, "--year"         ) == 0)  { my.find = 'y';  my.find_days     = 'y'; }
      else if (strcmp (a, "--old"          ) == 0)  { my.find = 'y';  my.find_days     = 'o'; }
      else if (strcmp (a, "--sb"           ) == 0)  { my.find = 'y';  my.find_size     = 's'; }
      else if (strcmp (a, "--kb"           ) == 0)  { my.find = 'y';  my.find_size     = 'k'; }
      else if (strcmp (a, "--mb"           ) == 0)  { my.find = 'y';  my.find_size     = 'm'; }
      else if (strcmp (a, "--gb"           ) == 0)  { my.find = 'y';  my.find_size     = 'g'; }
      else if (strcmp (a, "--punct"        ) == 0)  { my.find = 'y';  my.find_name     = 'p'; }
      else if (strcmp (a, "--extend"       ) == 0)  { my.find = 'y';  my.find_name     = 'e'; }
      else if (strcmp (a, "--space"        ) == 0)  { my.find = 'y';  my.find_name     = 's'; }
      else if (strcmp (a, "--junk"         ) == 0)  { my.find = 'y';  my.find_name     = 'j'; }
      else if (strcmp (a, "--badname"      ) == 0)  { my.find = 'y';  my.find_name     = 'b'; }
      /*---(regex search)----------------*/
      else if (strcmp (a, "--all"          ) == 0 || strcmp (a, "-A") == 0)  strncpy (my.regex, ".", MAX_REGEX);
      else if (strcmp (a, "--regex"        ) == 0)                           ;
      else if (strcmp (a, "--regexp"       ) == 0 || strcmp (a, "-r") == 0)  ;
      else if (strcmp (a, "--basename"     ) == 0 || strcmp (a, "-b") == 0)  ;
      else if (strcmp (a, "--wholename"    ) == 0 || strcmp (a, "-w") == 0)  ;
      else if (strcmp (a, "--ignore-case"  ) == 0 || strcmp (a, "-i") == 0)  my.regex_case     = '-';
      else if (a[0] != '-') 
         strncpy (my.regex, argv [i], MAX_REGEX);
      /*---(not understood)--------------*/
      else {
         printf  ("option <<%s>> not understood or implemented\n", a);
         DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
         return -1;
      }
      /*---(done)------------------------*/
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
   DEBUG_ARGS  yLOG_char    ("regex_case", my.regex_case);
   DEBUG_ARGS  yLOG_value   ("limit"     , my.limit);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ read configuration ]]================[ ------ [ ------ ]=*/
PROG_conf          (void)
{  /*---(locals)-----------+-----------+-*/
   FILE       *f_conf      = NULL;
   char        rc          = 0;
   char        rce         = -10;
   char        x_recd      [MAX_RECD];           /* input record              */
   int         x_len       = 0;                  /* string length             */
   char       *p;
   char       *q           = "\x1F";
   char       *r           = NULL;
   char        x_temp      [20];
   char        x_verbs     [1000] = " dir_ignore dir_stop dir_over dir_never dir_last dir_append mountpoint ";
   char        x_verb      [20];
   int         i           = 0;
   char        x_field     [ 20][100];
   /*---(header)-------------------------*/
   DEBUG_TOPS  yLOG_enter   (__FUNCTION__);
   /*---(check for option)---------------*/
   if (my.conf != 'y') {
      DEBUG_CONF   yLOG_note    ("elected not to process configuration file");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open configuration)-------------*/
   DEBUG_CONF   yLOG_info    ("conf_file" , my.conf_file);
   f_conf = fopen (my.conf_file, "r");
   DEBUG_CONF   yLOG_point   ("file point", f_conf);
   --rce;  if (f_conf == NULL) {
      printf ("fatal, helios configuration file %s could not be openned\n", my.conf_file);
      DEBUG_CONF   yLOG_note    ("failed to open file");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CONF   yLOG_note    ("successfully openned");
   /*---(read)---------------------------*/
   while (1) {
      fgets (x_recd, MAX_RECD, f_conf);
      if (feof(f_conf))                     break;
      /*---(filter)----------------------*/
      x_len = strlen (x_recd);
      if (x_len <= 0)                       continue;
      x_recd [--x_len] = '\0';
      if (x_len <= 0)                       continue;
      if (x_recd [0] == '#')                continue;
      DEBUG_CONF   yLOG_info    ("read"      , x_recd);
      /*---(get recd type)---------------*/
      p = strtok_r (x_recd, q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      x_len = strlen (p);
      if (x_len <= 2)                       continue;
      sprintf (x_temp, " %s ", p);
      DEBUG_CONF   yLOG_info    ("x_temp"    , x_temp);
      if (strstr (x_verbs, p) == NULL)      continue;
      strncpy (x_verb, p, 20);
      DEBUG_CONF   yLOG_info    ("x_verb"    , x_verb);
      /*---(get fields)------------------*/
      for (i = 0; i < 20; ++i) {
         p = strtok_r (NULL  , q, &r);
         if (p == NULL)                     break;
         ySTR_trim (p, ySTR_BOTH);
         strncpy (x_field [i], p, 100);
         DEBUG_CONF   yLOG_info    ("field"     , x_field [i]);
      }
      /*---(act)-------------------------*/
      rc = 0;
      if (x_verb [0] == 'd') {
         switch (x_verb [4]) {
         case 'i' : strncpy (nodir [n_nodir].name, x_field [0], MAX_NAME);
                    nodir [n_nodir].len = strlen (x_field [0]);
                    nodir [n_nodir].type = '#';
                    ++n_nodir;
                    DEBUG_CONF   yLOG_note    ("adding an ignore type");
                    break;
         case 's' : strncpy (nodir [n_nodir].name, x_field [0], MAX_NAME);
                    nodir [n_nodir].len = strlen (x_field [0]);
                    nodir [n_nodir].type = ')';
                    ++n_nodir;
                    DEBUG_CONF   yLOG_note    ("adding a stop type");
                    break;
         case 'o' : strncpy (nodir [n_nodir].name, x_field [0], MAX_NAME);
                    nodir [n_nodir].len = strlen (x_field [0]);
                    nodir [n_nodir].type = '(';
                    ++n_nodir;
                    DEBUG_CONF   yLOG_note    ("adding a over type");
                    break;
         case 'n' : strncpy (nodir [n_nodir].name, x_field [0], MAX_NAME);
                    nodir [n_nodir].len = strlen (x_field [0]);
                    nodir [n_nodir].type = 'x';
                    ++n_nodir;
                    DEBUG_CONF   yLOG_note    ("adding a never type");
                    break;
         case 'l' : strncpy (nodir [n_nodir].name, x_field [0], MAX_NAME);
                    nodir [n_nodir].len = strlen (x_field [0]);
                    nodir [n_nodir].type = '@';
                    ++n_nodir;
                    DEBUG_CONF   yLOG_note    ("adding a last type");
                    break;
         case 'a' : strncpy (nodir [n_nodir].name, x_field [0], MAX_NAME);
                    nodir [n_nodir].len = strlen (x_field [0]);
                    nodir [n_nodir].type = '/';
                    ++n_nodir;
                    DEBUG_CONF   yLOG_note    ("adding a append type");
                    break;
         default  : break;
         }
         DEBUG_CONF   yLOG_value   ("n_nodir"   , n_nodir);
      } else if (strcmp (x_verb, "mountpoint") == 0) {
         DEBUG_CONF   yLOG_note    ("mountpoint default found in conf");
         if (my.mpoint [0] != '\0') {
            DEBUG_CONF   yLOG_note    ("but, mountpoint already set by arg");
         } else {
            DEBUG_CONF   yLOG_note    ("mpoint currently null");
            strcpy (my.mpoint, x_field [0]);
         }
         DEBUG_CONF   yLOG_info    ("mpoint"    , my.mpoint);
      }
      /*---(done)------------------------*/
   }
   /*---(close configuration)------------*/
   DEBUG_CONF   yLOG_note    ("closing configuration file");
   rc = fclose (f_conf);
   DEBUG_CONF   yLOG_value   ("close rc"  , rc);
   --rce;  if (rc != 0) {
      DEBUG_CONF   yLOG_note    ("failed to close file");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CONF   yLOG_note    ("successfully closed configuration file");
   /*---(complete)-----------------------*/
   DEBUG_TOPS  yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] post-argument program initialization ------------*/
PROG_begin         (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   api_ysort_init ();
   my.regex_len = strlen (my.regex);
   if (my.regex_case == 'y')  my.regex_rc  = regcomp (&(my.regex_comp), my.regex, REG_EXTENDED);
   else                       my.regex_rc  = regcomp (&(my.regex_comp), my.regex, REG_EXTENDED | REG_ICASE);
   DEBUG_PROG   yLOG_value   ("regex_rc"  , my.regex_rc);
   if (my.regex_rc != 0) {
      printf ("regex could not be compiled\n");
      DEBUG_TOPS   yLOG_note    ("regex could not be compiled");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   my.uid    = geteuid();
   my.gid    = getegid();
   DRIVE_init ();
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOG_break   ();
   /*---(complete)-----------------------*/
   return 0;
}

char                /* PURPOSE : shutdown program and free memory ------------*/
PROG_end           (void)
{
   DRIVE_wrap ();
   DEBUG_TOPS   yLOG_break   ();
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   if (my.regex_len > 0)  regfree (&(my.regex_comp));
   DEBUG_RPTG   yLOG_value   ("n_audio"   , my.n_audio);
   DEBUG_RPTG   yLOG_value   ("n_video"   , my.n_video);
   DEBUG_RPTG   yLOG_value   ("n_image"   , my.n_image);
   DEBUG_RPTG   yLOG_value   ("n_text"    , my.n_text);
   DEBUG_RPTG   yLOG_value   ("n_progs"   , my.n_progs);
   DEBUG_RPTG   yLOG_value   ("n_data"    , my.n_data);
   DEBUG_RPTG   yLOG_value   ("n_apps"    , my.n_apps);
   DEBUG_RPTG   yLOG_value   ("n_comp"    , my.n_comp);
   DEBUG_RPTG   yLOG_value   ("n_model"   , my.n_model);
   DEBUG_RPTG   yLOG_value   ("n_other"   , my.n_other);
   DEBUG_RPTG   yLOG_value   ("n_unknown" , my.n_unknown);
   DEBUG_RPTG   yLOG_value   ("n_huh"     , my.n_huh);
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOGS_end    ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROG__unit_quiet     (void)
{
   char       *x_args [1]  = { "helios" };
   yURG_logger (1, x_args);
   PROG_init   ();
   yURG_urgs   (1, x_args);
   PROG_args   (1, x_args);
   PROG_begin  ();
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud      (void)
{
   int         x_argc      = 3;
   char       *x_args [20] = { "helios_unit", "@@kitchen", "@@args" };
   yURG_logger (x_argc, x_args);
   PROG_init   ();
   yURG_urgs   (x_argc, x_args);
   PROG_args   (x_argc, x_args);
   PROG_begin  ();
   return 0;
}

char         /*-> set up program urgents/debugging ---[ light  [uz.210.001.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_end       (void)
{
   PROG_end       ();
   return 0;
}




/*===============================[[ end-code ]]===============================*/
