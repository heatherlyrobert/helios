/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



/*====================------------------------------------====================*/
/*===----                     read from file                           ----===*/
/*====================------------------------------------====================*/
static void  o___READING_________o () { return; }

static uchar  s_verbs     [LEN_RECD] = " dir_silent dir_pass dir_last dir_never mountpoint ";
static uchar  s_field     [ 20][LEN_HUND];
static uchar  s_nfield    = 0;

char         /*===[[ read configuration ]]================[ ------ [ ------ ]=*/
CONF__parse             (cchar *a_recd)
{  /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p;
   char       *q           = "\x1F";
   char       *r           = NULL;
   char       *u           = NULL;
   uchar       x_recd      [LEN_RECD]  = "";
   uchar       x_verb      [LEN_RECD]  = "";
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_nfield = 0;
   for (i = 0; i < 20; ++i) {
      strncpy (s_field [i], "", LEN_HUND);
   }
   /*---(defense)------------------------*/
   DEBUG_CONF    yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_recd, a_recd, LEN_RECD);
   x_len = strlen (x_recd);
   DEBUG_CONF    yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 25) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
   --rce;  if (x_recd [0] <= ' ' || x_recd [0] >= '~') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_recd [0] == '#' || x_recd [0] == ' ') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get fields)------------------*/
   p = strtok_r (x_recd, q, &r);
   for (i = 0; i < 20; ++i) {
      if (p == NULL)  break;
      ySTR_trim (p, ySTR_BOTH);
      strncpy (s_field [i], p, LEN_HUND);
      DEBUG_CONF   yLOG_complex ("field"     , "%2d, %2d[%s]", i, strlen (s_field [i]), s_field [i]);
      ++s_nfield;
      p = strtok_r (NULL  , q, &r);
   }
   DEBUG_CONF   yLOG_value   ("s_nfield"  , s_nfield);
   /*---(filter recd types)-----------*/
   sprintf (x_verb, " %s ", s_field [0]);
   DEBUG_CONF   yLOG_info    ("x_verb"    , x_verb);
   u = strstr (s_verbs, x_verb);
   DEBUG_CONF    yLOG_point   ("u"         , u);
   --rce;  if (u == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CONF__handle            (void)
{  /*---(locals)-----------+-----------+-*/
   FILE       *f_conf      = NULL;
   char        rc          = 0;
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_CONF    yLOG_enter   (__FUNCTION__);
   DEBUG_CONF    yLOG_info    ("s_field 0" , s_field [0]);
   /*---(directory types)----------------*/
   if (s_field [0][0] == 'd') {
      switch (s_field [0][4]) {
      case 's' : g_nodir [n_nodir].type = STYPE_SILENT;
                 DEBUG_CONF   yLOG_note    ("adding a silent type");
                 break;
      case 'p' : g_nodir [n_nodir].type = STYPE_PASS;
                 DEBUG_CONF   yLOG_note    ("adding a pass type");
                 break;
      case 'l' : g_nodir [n_nodir].type = STYPE_LAST;
                 DEBUG_CONF   yLOG_note    ("adding a last type");
                 break;
      case 'n' : g_nodir [n_nodir].type = STYPE_NEVER;
                 DEBUG_CONF   yLOG_note    ("adding a never type");
                 break;
      default  : /* unknown verb */
                 DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
                 return rce;
      }
   }
   /*---(mountpoint)---------------------*/
   else if (strcmp (s_field [0], "mountpoint") == 0) {
      DEBUG_CONF   yLOG_note    ("mountpoint default found in conf");
      if (my.mpoint [0] != '\0') {
         DEBUG_CONF   yLOG_note    ("but, mountpoint already set by arg");
      } else {
         DEBUG_CONF   yLOG_note    ("mpoint currently null");
         strcpy (my.mpoint, s_field [1]);
      }
      DEBUG_CONF   yLOG_info    ("mpoint"    , my.mpoint);
   }
   /*---(common)-------------------------*/
   strncpy (g_nodir [n_nodir].name, s_field [1], MAX_NAME);
   g_nodir [n_nodir].len = strlen (s_field [1]);
   ++n_nodir;
   DEBUG_CONF   yLOG_value   ("n_nodir"   , n_nodir);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ read configuration ]]================[ ------ [ ------ ]=*/
CONF_read          (void)
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   FILE       *f_conf      = NULL;
   char        x_recd      [MAX_RECD];           /* input record              */
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(check for option)---------------*/
   if (my.conf != 'y') {
      DEBUG_CONF   yLOG_note    ("elected not to process configuration file");
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open configuration)-------------*/
   DEBUG_CONF   yLOG_info    ("conf_file" , my.conf_file);
   f_conf = fopen (my.conf_file, "r");
   DEBUG_CONF   yLOG_point   ("file point", f_conf);
   --rce;  if (f_conf == NULL) {
      printf ("fatal, helios configuration file %s could not be openned\n", my.conf_file);
      DEBUG_CONF   yLOG_note    ("failed to open file");
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CONF   yLOG_note    ("successfully openned");
   /*---(read)---------------------------*/
   while (1) {
      /*---(get line)--------------------*/
      fgets (x_recd, MAX_RECD, f_conf);
      if (feof(f_conf))                     break;
      /*---(parse)-----------------------*/
      rc = CONF__parse (x_recd);
      DEBUG_CONF   yLOG_value   ("parse"     , rc);
      if (rc < 0)                           continue;
      /*---(act)-------------------------*/
      rc = CONF__handle ();
      DEBUG_CONF   yLOG_value   ("handle"    , rc);
      /*---(done)------------------------*/
   }
   /*---(close configuration)------------*/
   DEBUG_CONF   yLOG_note    ("closing configuration file");
   rc = fclose (f_conf);
   DEBUG_CONF   yLOG_value   ("close rc"  , rc);
   --rce;  if (rc != 0) {
      DEBUG_CONF   yLOG_note    ("failed to close file");
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CONF   yLOG_note    ("successfully closed configuration file");
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
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
   DEBUG_CONF   yLOG_value   ("n_nodir"   , n_nodir);
   --rce;  for (i = 0; i < n_nodir; ++i) {
      /*---(filter)----------------------*/
      DEBUG_CONF   yLOG_info    ("dir"       , g_nodir [i].name);
      /*---(name-only options)-----------*/
      if (g_nodir [i].len == x_len2 && strcmp (g_nodir [i].name, a_name) == 0) {
         /*---(never option)-------------*/
         if (g_nodir [i].type == STYPE_NEVER) {
            DEBUG_CONF    yLOG_note    ("found a dir_never rule");
            rc = 1;
            break;
         }
      }
      /*---(full-path options)-----------*/
      if (g_nodir [i].len == x_len  && strcmp (g_nodir [i].name, a_full) == 0) {
         /*---(silent)-------------------*/
         if (g_nodir [i].type == STYPE_SILENT) {
            DEBUG_CONF    yLOG_note    ("found a dir_silent rule");
            *a_silent = 'y';
            rc = 1;
            break;
         }
         /*---(ignore)-------------------*/
         if (g_nodir [i].type == STYPE_PASS) {
            DEBUG_CONF    yLOG_note    ("found a dir_past rule");
            rc = 1;
            break;
         }
         /*---(last)---------------------*/
         if (g_nodir [i].type == STYPE_LAST) {
            DEBUG_CONF    yLOG_note    ("found a dir_last rule");
            rc = 1;
            break;
         }
      }
   }
   /*---(update)-------------------------*/
   if (rc != 0) {
      *a_stype = g_nodir [i].type;
      DEBUG_CONF    yLOG_char    ("*a_stype"  , *a_stype);
      DEBUG_CONF    yLOG_char    ("*a_silent" , *a_silent);
   } else {
      DEBUG_CONF    yLOG_note    ("not matching rules found");
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return rc;
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
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "field"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "CONF field  (%2d) : %2d[%s]", n,
            strlen (s_field [n]), s_field [n]);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      if (n >= n_nodir) {
         snprintf (unit_answer, LEN_FULL, "CONF entry  (%2d) : no entry", n);
      } else {
         snprintf (unit_answer, LEN_FULL, "CONF entry  (%2d) : %c %2d[%.50s]", n,
               g_nodir [n].type, g_nodir [n].len,  g_nodir [n].name);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}
