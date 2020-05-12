/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */

/*
 *
 *
 * metis  § ----- § strerr messages about bad configuration
 *
 *
 *
 *
 */

#define     MAX_CAT         50
typedef     struct      cCAT        tCAT;
struct cCAT {
   char        name        [LEN_TERSE];
   char        cat;
   char        desc        [LEN_DESC];
   int         n_seen;
   llong       b_seen;
   int         n_kept;
   llong       b_kept;
   int         n_found;
   llong       b_found;
};
static tCAT        s_cats [MAX_CAT];
static int         s_ncat = 0;



static char  s_head    [LEN_RECD]  = "";
static char  s_cols    [LEN_RECD]  = "";
static char  s_recd    [LEN_RECD]  = "";



/*====================------------------------------------====================*/
/*===----                        mass changes                          ----===*/
/*====================------------------------------------====================*/
static void  o___MASS____________o () { return; }

char
MIME_reset_all          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =  0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_senter  (__FUNCTION__);
   /*---(reset cats)---------------------*/
   DEBUG_STATS  yLOG_snote   ("cats");
   DEBUG_STATS  yLOG_sint    (s_ncat);
   for (i = 0; i < s_ncat; ++i) {
      s_cats [i].n_seen   = 0;
      s_cats [i].b_seen   = 0;
      s_cats [i].n_kept   = 0;
      s_cats [i].b_kept   = 0;
      s_cats [i].n_found  = 0;
      s_cats [i].b_found  = 0;
   }
   /*---(reset mime)---------------------*/
   DEBUG_STATS  yLOG_snote   ("mime");
   DEBUG_STATS  yLOG_sint    (n_mime);
   for (i = 0; i < n_mime; ++i) {
      g_mime [i].n_seen   = 0;
      g_mime [i].b_seen   = 0;
      g_mime [i].n_kept   = 0;
      g_mime [i].b_kept   = 0;
      g_mime [i].n_found  = 0;
      g_mime [i].b_found  = 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MIME_reset_found        (void)
{
   int         i           =  0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_senter  (__FUNCTION__);
   /*---(reset cats)---------------------*/
   DEBUG_STATS  yLOG_snote   ("cats");
   DEBUG_STATS  yLOG_sint    (s_ncat);
   for (i = 0; i < s_ncat; ++i) {
      s_cats [i].n_found  = 0;
      s_cats [i].b_found  = 0;
   }
   /*---(reset mime)---------------------*/
   DEBUG_STATS  yLOG_snote   ("mime");
   DEBUG_STATS  yLOG_sint    (n_mime);
   for (i = 0; i < n_mime; ++i) {
      g_mime [i].n_found  = 0;
      g_mime [i].b_found  = 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MIME__purge             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   /*---(purge cats)---------------------*/
   DEBUG_STATS  yLOG_note    ("initialize cats table");
   for (i = 0; i < MAX_CAT; ++i) {
      strlcpy (s_cats [i].name, "", LEN_TERSE);
      s_cats [i].cat       = '-';
      strlcpy (s_cats [i].desc, "", LEN_DESC);
   }
   /*---(purge mime)---------------------*/
   DEBUG_STATS  yLOG_note    ("initialize mime table");
   for (i = 0; i < MAX_MIME; ++i) {
      strlcpy (g_mime [i].ext , "", LEN_TERSE);
      g_mime [i].cat       = '-';
      strlcpy (g_mime [i].desc, "", LEN_DESC);
      g_mime [i].like      = '-';
   }
   /*---(reset totals)-------------------*/
   s_ncat = MAX_CAT;
   n_mime = MAX_MIME;
   MIME_reset_all ();
   /*---(reset globals)------------------*/
   DEBUG_STATS  yLOG_note    ("reset global counters");
   s_ncat    = 0;
   n_mime    = 0;
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      program level                           ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
MIME_init          (void)
{
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   /*---(clear table)--------------------*/
   MIME__purge ();
   /*---(read config)--------------------*/
   rc = MIME_read ();
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return rc;
}


/*====================------------------------------------====================*/
/*===----                     single entity action                     ----===*/
/*====================------------------------------------====================*/
static void  o___SINGLE__________o () { return; }

char
MIME__action            (uchar *a_ext, int *a_cindex, int *a_mindex, uchar *a_cat, char a_action, llong a_bytes)
{
   /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =   -1;
   uchar       x_ext       [LEN_TERSE] = "";
   uchar       x_cat       =  '-';
   int         x_len       =    0;
   int         i           =    0;
   int         m           =   -1;
   int         c           =   -1;
   char        x_inc       =    1;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_senter  (__FUNCTION__);
   DEBUG_STATS  yLOG_schar   (a_action);
   DEBUG_STATS  yLOG_sint    (a_bytes);
   /*---(default)------------------------*/
   if (a_cindex != NULL)  *a_cindex = -1;
   if (a_mindex != NULL)  *a_mindex = -1;
   if (a_cat    != NULL)  *a_cat    = MIME_HUH;
   /*---(defense)------------------------*/
   DEBUG_STATS  yLOG_spoint  (a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_STATS  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_STATS  yLOG_snote   (a_ext);
   x_len = strlen  (a_ext);
   DEBUG_STATS  yLOG_sint    (x_len);
   --rce;  if (x_len <= 0 || x_len >= LEN_TERSE) {
      DEBUG_STATS  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_ext, a_ext, LEN_TERSE);
   for (i = 0; i < x_len; ++i)   x_ext [i] = tolower (x_ext [i]);
   if (a_bytes < 0)  x_inc = -1;
   /*---(find mime)----------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].ext  [0] != x_ext [0])      continue;
      if (g_mime [i].ext  [1] != x_ext [1])      continue;
      if (strcmp (g_mime [i].ext , x_ext) != 0)  continue;
      m     = i;
      x_cat = g_mime [m].cat;
      break;
   }
   DEBUG_STATS  yLOG_sint    (m);
   --rce;  if (m < 0) {
      DEBUG_STATS  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_STATS  yLOG_snote   (g_mime [m].ext);
   DEBUG_STATS  yLOG_schar   (x_cat);
   /*---(find cat)-----------------------*/
   for (i = 0; i < s_ncat; ++i) {
      if (s_cats [i].cat != x_cat)  continue;
      c = i;
      break;
   }
   DEBUG_STATS  yLOG_sint    (c);
   --rce;  if (c < 0) {
      DEBUG_STATS  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_STATS  yLOG_snote   (s_cats [c].name);
   /*---(act)----------------------------*/
   --rce;  switch (a_action) {
   case '-'  : 
      break;
   case DEST_STDOUT  : 
      DEBUG_STATS  yLOG_snote   ("seen");
      s_cats [0].n_seen  += x_inc;
      s_cats [c].n_seen  += x_inc;
      g_mime [m].n_seen  += x_inc;
      s_cats [0].b_seen  += a_bytes;
      s_cats [c].b_seen  += a_bytes;
      g_mime [m].b_seen  += a_bytes;
      DEBUG_STATS  yLOG_sint    (g_mime [m].n_seen);
      DEBUG_STATS  yLOG_sint    (g_mime [m].b_seen);
      break;
   case 'k'  : 
      DEBUG_STATS  yLOG_snote   ("kept");
      s_cats [0].n_kept  += x_inc;
      s_cats [c].n_kept  += x_inc;
      g_mime [m].n_kept  += x_inc;
      s_cats [0].b_kept  += a_bytes;
      s_cats [c].b_kept  += a_bytes;
      g_mime [m].b_kept  += a_bytes;
      DEBUG_STATS  yLOG_sint    (g_mime [m].n_kept);
      DEBUG_STATS  yLOG_sint    (g_mime [m].b_kept);
      break;
   case 'f'  : 
      DEBUG_STATS  yLOG_snote   ("found");
      s_cats [0].n_found += x_inc;
      s_cats [c].n_found += x_inc;
      g_mime [m].n_found += x_inc;
      s_cats [0].b_found += a_bytes;
      s_cats [c].b_found += a_bytes;
      g_mime [m].b_found += a_bytes;
      DEBUG_STATS  yLOG_sint    (g_mime [m].n_found);
      DEBUG_STATS  yLOG_sint    (g_mime [m].b_found);
      break;
   case 'c'  :
      break;
   case 'i'  :
      break;
   default   :
      DEBUG_STATS  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   if (a_cindex != NULL)  *a_cindex = c;
   if (a_mindex != NULL)  *a_mindex = m;
   if (a_cat    != NULL)  *a_cat    = x_cat;
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char MIME_get_index   (uchar *a_ext, int *a_cindex, int *a_mindex) { return MIME__action (a_ext, a_cindex, a_mindex, NULL, 'i', 0); }
char MIME_add_seen    (uchar *a_ext, uchar *a_cat, llong a_bytes) { return MIME__action (a_ext, NULL, NULL, a_cat, 's', a_bytes); }
char MIME_add_kept    (uchar *a_ext, llong a_bytes) { return MIME__action (a_ext, NULL , NULL, NULL, 'k', a_bytes); }
char MIME_add_found   (uchar *a_ext, llong a_bytes) { return MIME__action (a_ext, NULL , NULL, NULL, 'f', a_bytes); }

char
MIME_add_man            (uchar *a_ext, uchar *a_cat, long a_bytes)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =   -1;
   char        x_ext       [LEN_TERSE] = "";
   int         i           =   0;
   int         x_len       =   0;
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_cat   != NULL)  *a_cat   = MIME_HUH;
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_ext"     , a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len = strlen  (a_ext);
   DEBUG_DATA   yLOG_complex ("ext"       , "%d, %s", x_len, a_ext);
   --rce;  if (x_len == 0 || x_len > 2) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(make copy)----------------------*/
   strlcpy (x_ext, a_ext, LEN_TERSE);
   --rce;  if (strchr ("123456789", x_ext [0]) == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len > 1 && strchr ("x"        , x_ext [1]) == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = MIME_add_seen ("manual", a_cat, a_bytes);
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return rc;
}


/*====================------------------------------------====================*/
/*===----                       simple accessors                       ----===*/
/*====================------------------------------------====================*/
static void  o___ACCESSOR________o () { return; }

uchar  MIME_cat_abbr    (int c) { return s_cats [c].cat;  }
uchar *MIME_cat_name    (int c) { return s_cats [c].name; }

uchar  MIME_mime_cat    (int m) { return g_mime [m].cat;  }
uchar *MIME_mime_ext    (int m) { return g_mime [m].ext;  }



/*====================------------------------------------====================*/
/*===----                       reading mime files                     ----===*/
/*====================------------------------------------====================*/
static void  o___INPUT___________o () { return; }

char
MIME__parse_essential   (char *a_recd, char *a_flag, char **s)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char       *x_name      =    0;
   char       *x_cat       =    0;
   char       *q           = "§";
   char        x_flag      =  '-';
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(default)---------------------*/
   if (a_flag != NULL)  *a_flag = '-';
   /*---(extension)-------------------*/
   x_name = strtok_r (a_recd, q, s);
   DEBUG_INPT  yLOG_point   ("name"      , x_name);
   --rce;  if (x_name == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strltrim (x_name, ySTR_BOTH, LEN_TERSE);
   DEBUG_INPT  yLOG_info    ("name"      , x_name);
   /*---(set flag))-------------------*/
   if (x_name [0] == toupper (x_name [0]))  x_flag = 'y';
   else                                     x_flag = '-';
   DEBUG_INPT  yLOG_char    ("x_flag"    , x_flag);
   /*---(category)--------------------*/
   x_cat  = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("cat"       , x_cat );
   --rce;  if (x_cat  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strltrim (x_cat , ySTR_BOTH, LEN_TERSE);
   DEBUG_INPT  yLOG_char    ("cat"       , x_cat  [0]);
   /*---(save category)---------------*/
   --rce;  if (x_flag == 'y') {
      DEBUG_INPT   yLOG_note    ("check for category duplicates");
      for (i = 0; i <= s_ncat; ++i) {
         if (strcmp (x_name, s_cats [i].name) == 0) {
            DEBUG_INPT   yLOG_info    ("duplicate" , x_name);
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         if (x_cat [0] == s_cats [i].cat) {
            DEBUG_INPT   yLOG_char    ("duplicate" , x_cat);
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
      DEBUG_INPT  yLOG_note    ("save to table");
      strlcpy (s_cats [s_ncat].name, x_name, LEN_TERSE);
      s_cats [s_ncat].cat     = x_cat [0];
      ++s_ncat;
      DEBUG_INPT  yLOG_value   ("s_ncat"    , s_ncat);
   }
   /*---(save mime)-------------------*/
   --rce;  if (x_flag != 'y') {
      DEBUG_INPT   yLOG_note    ("check for matching category");
      for (i = 0; i < s_ncat; ++i) {
         if (x_cat [0] == s_cats [i].cat) {
            DEBUG_INPT   yLOG_value   ("found it"  , i);
            break;
         }
      }
      DEBUG_INPT   yLOG_complex ("checked"   , "%d, %d", s_ncat, i);
      if (i >= s_ncat) {
         DEBUG_INPT   yLOG_note    ("no matching category");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_note    ("check for mime entry duplicates");
      for (i = 0; i < n_mime; ++i) {
         if (strcmp (x_name, g_mime [i].ext) == 0) {
            DEBUG_INPT   yLOG_info    ("duplicate" , x_name);
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
      DEBUG_INPT  yLOG_note    ("save to table");
      strlcpy (g_mime [n_mime].ext , x_name, LEN_TERSE);
      g_mime [n_mime].cat     = x_cat [0];
      ++n_mime;
      DEBUG_INPT  yLOG_value   ("n_mime"    , n_mime);
   }
   /*---(save back)-------------------*/
   if (a_flag != NULL)  *a_flag = x_flag;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME__parse_optional    (char a_flag, char **s)
{
   /*---(locals)-----------+-----------+-*/
   char       *p           =    0;
   char       *q           = "§";
   llong       v           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(desc)------------------------*/
   p = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("desc"      , p);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   strltrim (p, ySTR_BOTH, LEN_DESC);
   DEBUG_INPT  yLOG_info    ("desc"      , p);
   if (a_flag == 'y')  strlcpy (s_cats [s_ncat - 1].desc, p, LEN_DESC);
   else                strlcpy (g_mime [n_mime - 1].desc, p, LEN_DESC);
   /*---(like)------------------------*/
   p = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("like"      , p);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   DEBUG_INPT  yLOG_char    ("like"      , p [0]);
   if (a_flag == 'y')  ;
   else                g_mime [n_mime - 1].like    = p [0];
   /*---(seen)------------------------*/
   p = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("n_seen"    , p);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   DEBUG_INPT  yLOG_info    ("n_seen"    , p);
   FILE_uncommas (p, &v);
   DEBUG_INPT  yLOG_value   ("n_seen"    , v);
   if (a_flag == 'y')  s_cats [s_ncat - 1].n_seen  = v;
   else                g_mime [n_mime - 1].n_seen  = v;
   /*---(seen percent)----------------*/
   p = strtok_r (NULL  , q, s);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(seen bytes)------------------*/
   p = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("b_seen"    , p);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   DEBUG_INPT  yLOG_info    ("b_seen"    , p);
   FILE_uncommas (p, &v);
   DEBUG_INPT  yLOG_llong   ("b_seen"    , v);
   if (a_flag == 'y')  s_cats [s_ncat - 1].b_seen  = v;
   else                g_mime [n_mime - 1].b_seen  = v;
   /*---(seen bytes percent)----------*/
   p = strtok_r (NULL  , q, s);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(kept)------------------------*/
   p = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("n_kept"    , p);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   DEBUG_INPT  yLOG_info    ("n_kept"    , p);
   FILE_uncommas (p, &v);
   DEBUG_INPT  yLOG_value   ("n_kept"    , v);
   if (a_flag == 'y')  s_cats [s_ncat - 1].n_kept  = v;
   else                g_mime [n_mime - 1].n_kept  = v;
   /*---(kept percent)----------------*/
   p = strtok_r (NULL  , q, s);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(kept bytes)------------------*/
   p = strtok_r (NULL  , q, s);
   DEBUG_INPT  yLOG_point   ("b_kept"    , p);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   DEBUG_INPT  yLOG_info    ("b_kept"    , p);
   FILE_uncommas (p, &v);
   DEBUG_INPT  yLOG_llong   ("b_kept"    , v);
   if (a_flag == 'y')  s_cats [s_ncat - 1].b_kept  = v;
   else                g_mime [n_mime - 1].b_kept  = v;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}


char
MIME_read          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;          /* file pointer                   */
   int         f_lines     =    0;          /* line counter                   */
   char        x_recd      [MAX_RECD];      /* input record                   */
   int         x_len       =    0;          /* input length                   */
   char       *s           = NULL;          /* strtok context                 */
   char        x_flag      =  '-';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(purge the tables)---------------*/
   MIME__purge ();
   /*---(open)---------------------------*/
   rc = FILE_open  (&f, my.file_mime, 'r');
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   DEBUG_INPT   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0 || f == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read lines)---------------------*/
   DEBUG_INPT  yLOG_note    ("read lines");
   --rce;  while (1) {
      /*---(read and clean)--------------*/
      ++f_lines;
      DEBUG_INPT  yLOG_value   ("line"      , f_lines);
      fgets (x_recd, MAX_RECD, f);
      if (feof(f))                break;
      if (x_recd [0] == '#')      continue;
      if (x_recd [0] == '\0')     continue;
      if (x_recd [0] == ' ')      continue;
      x_len = strlen (x_recd);
      if (x_len <= 1)             continue;
      x_recd [--x_len] = '\0';
      DEBUG_INPT  yLOG_value   ("n_mime"    , n_mime);
      strldecode (x_recd, LEN_RECD);
      DEBUG_INPT  yLOG_info    ("fixed"     , x_recd);
      /*---(essential)-------------------*/
      rc = MIME__parse_essential (x_recd, &x_flag, &s);
      if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(optional)--------------------*/
      rc = MIME__parse_optional  (x_flag, &s);
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   rc = FILE__close (&f);
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   DEBUG_INPT   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0 || f != NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       writing mime files                     ----===*/
/*====================------------------------------------====================*/
static void  o___OUTPUT__________o () { return; }

char
MIME__write_clear       (void)
{
   strlcpy (s_head, "", LEN_RECD);
   strlcpy (s_cols, "", LEN_RECD);
   strlcpy (s_recd, "", LEN_RECD);
   return 0;
}

char
MIME__write_title       (FILE *f, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(default)------------------------*/
   strlcpy (s_head, "", LEN_RECD);
   /*---(spacing)------------------------*/
   --rce;  switch (a_type) {
   case DEST_FILE   :
      fprintf (f, "#!/usr/local/bin/helios\n");
      fprintf (f, "#   HELIOS-PHAETON (locate) -- filesystem searching and indexing services\n");
      fprintf (f, "#   mime configuration file\n");
      break;
   case DEST_STDOUT :
      fprintf (f, "HELIOS-PHAETON (locate) -- filesystem searching and indexing services\n");
      fprintf (f, "report : mime type summary\n");
      break;
   default  :
      return rce;
      break;
   }
   return 0;
}

char
MIME__write_line        (FILE *f, char a_type, uchar *a_name, uchar a_cat, uchar *a_desc, uchar a_like, int a_nseen, llong a_bseen, int a_nkept, llong a_bkept)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        s           =  ' ';
   char        t           [LEN_RECD]  = "";
   char        c           [LEN_LABEL] = "";
   char        p           [LEN_LABEL] = "";
   /*---(default)------------------------*/
   strlcpy (s_recd, "", LEN_RECD);
   /*---(defense)------------------------*/
   --rce;  if (a_name == NULL)    return rce;
   /*---(spacing)------------------------*/
   --rce;  switch (a_type) {
   case DEST_FILE   : s = '';           break;
   case DEST_STDOUT : s = '§';            break;
   default  : return rce;         break;
   }
   /*---(prefix)-------------------------*/
   sprintf (t, "%-10.10s %c %c %c %-40.40s %c %c %c ",
         a_name, s, a_cat , s, a_desc, s, a_like, s);
   strlcat (s_recd, t, LEN_RECD);
   /*---(internal spacer)----------------*/
   sprintf (t, "   ");
   strlcat (s_recd, t, LEN_RECD);
   /*---(seen count)---------------------*/
   FILE_commas   (a_nseen, c);
   FILE_percents (a_nseen, s_cats [0].n_seen, p);
   sprintf (t, "%10.10s %c %3.3s %c ", c, s, p, s);
   strlcat (s_recd, t, LEN_RECD);
   /*---(seen bytes)---------------------*/
   FILE_commas   (a_bseen, c);
   FILE_percents (a_bseen, s_cats [0].b_seen, p);
   sprintf (t, "%18.18s %c %3.3s %c ", c, s, p, s);
   strlcat (s_recd, t, LEN_RECD);
   /*---(internal spacer)----------------*/
   sprintf (t, "   ");
   strlcat (s_recd, t, LEN_RECD);
   /*---(kept count)---------------------*/
   FILE_commas   (a_nkept, c);
   FILE_percents (a_nkept, s_cats [0].n_kept, p);
   sprintf (t, "%10.10s %c %3.3s %c ", c, s, p, s);
   strlcat (s_recd, t, LEN_RECD);
   /*---(kept bytes)---------------------*/
   FILE_commas   (a_bkept, c);
   FILE_percents (a_bkept, s_cats [0].b_kept, p);
   sprintf (t, "%18.18s %c %3.3s %c ", c, s, p, s);
   strlcat (s_recd, t, LEN_RECD);
   /*---(internal spacer)----------------*/
   sprintf (t, "   ");
   strlcat (s_recd, t, LEN_RECD);
   /*---(seen vs kept)-------------------*/
   FILE_percents (a_nkept, a_nseen, c);
   FILE_percents (a_bkept, a_bseen, p);
   sprintf (t, "  %3.3s %c %3.3s %c ", c, s, p, s);
   strlcat (s_recd, t, LEN_RECD);
   /*---(write)--------------------------*/
   if (f != NULL)   fprintf (f, "%s\n", s_recd);
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME__write_columns     (FILE *f, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        s           =  ' ';
   char        t           [LEN_RECD]  = "";
   /*---(default)------------------------*/
   strlcpy (s_cols, "", LEN_RECD);
   /*---(spacing)------------------------*/
   --rce;  switch (a_type) {
   case DEST_FILE   : s = '';           break;
   case DEST_STDOUT : s = '§';            break;
   default  : return rce;         break;
   }
   /*---(prefix)-------------------------*/
   sprintf (t, "#-ext----- %c t %c ---desc--------------------------------- %c l %c ",
         s, s, s, s);
   strlcat (s_cols, t, LEN_RECD);
   /*---(internal spacer)----------------*/
   sprintf (t, "   ");
   strlcat (s_cols, t, LEN_RECD);
   /*---(seen)---------------------------*/
   sprintf (t, "---seen--- %c pct %c ---seen-bytes----- %c pct %c ", s, s, s, s);
   strlcat (s_cols, t, LEN_RECD);
   /*---(internal spacer)----------------*/
   sprintf (t, "   ");
   strlcat (s_cols, t, LEN_RECD);
   /*---(kept)---------------------------*/
   sprintf (t, "---kept--- %c pct %c ---kept-bytes----- %c pct %c ", s, s, s, s);
   strlcat (s_cols, t, LEN_RECD);
   /*---(internal spacer)----------------*/
   sprintf (t, "   ");
   strlcat (s_cols, t, LEN_RECD);
   /*---(seen vs kept--------------------*/
   sprintf (t, "  cnt %c siz %c ", s, s);
   strlcat (s_cols, t, LEN_RECD);
   /*---(write)--------------------------*/
   if (f != NULL)   fprintf (f, "\n%s\n", s_cols);
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME__write_category    (FILE *f, char a_type, char a_cat)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =   0;
   int         n           =   0;
   char        x           [LEN_TERSE] = "";
   char        t           [LEN_RECD]  = "";
   /*---(default)------------------------*/
   strlcpy (s_head, "", LEN_RECD);
   /*---(find)---------------------------*/
   for (i = 0; i < s_ncat; ++i) {
      if (s_cats [i].cat == 0    )  break;
      if (s_cats [i].cat != a_cat)  continue;
      n = i;
      break;
   }
   --rce;  if (n < 0)    return rce;
   /*---(type spacing)-------------------*/
   if (a_type == DEST_FILE)  strlcpy (x, "====", LEN_TERSE);
   /*---(heading bar)--------------------*/
   sprintf (t, "[[ %s ]]==================================", s_cats [n].name);
   sprintf (s_head, "#===%-16.16s==========================================%s]    [===========================================%s]    [===========================================%s]", t, x, x, x);
   if (a_type == DEST_FILE)  strlcpy (x, "==", LEN_TERSE);
   sprintf (t, "    [===========%s]", x);
   strlcat (s_head, t, LEN_RECD);
   /*---(write)--------------------------*/
   if (f != NULL) {
      fprintf (f, "\n\n\n%s\n", s_head);
   }
   /*---(detailed line)------------------*/
   MIME__write_line (f, a_type, s_cats [n].name, a_cat, s_cats [n].desc, '-', s_cats [n].n_seen, s_cats [n].b_seen, s_cats [n].n_kept, s_cats [n].b_kept);
   /*---(columns)------------------------*/
   if (a_cat != MIME_TOTAL)    MIME__write_columns (f, a_type);
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME_write         (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   int         i           =    0;
   /*> char        x_comma     [20];                                                  <*/
   /*> char        x_percent   [20];                                                  <*/
   char        x_save      =  '-';
   int         x_count     =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   DEBUG_STATS  yLOG_char    ("a_type"    , a_type);
   /*---(open)---------------------------*/
   rc = FILE_open  (&f, my.file_mime, 'w');
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   DEBUG_INPT   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0 || f == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(titles)-------------------------*/
   MIME__write_title (f, a_type);
   /*---(total category)-----------------*/
   MIME__write_category (f, a_type, MIME_TOTAL);
   /*---(process all)--------------------*/
   for (i = 0; i < n_mime; ++i) {
      /*---(formatting)------------------*/
      if (g_mime [i].cat != x_save) {
         x_count = 0;
         x_save = g_mime [i].cat;
         MIME__write_category (f, a_type, x_save);
      } else if ((x_count % 25) == 0 ) {
         MIME__write_columns  (f, a_type);
      } else if ((x_count %  5) == 0 ) {
         fprintf (f, "\n");
      }
      ++x_count;
      /*---(general part)----------------*/
      MIME__write_line (f, a_type, g_mime [i].ext, g_mime [i].cat, g_mime [i].desc, g_mime [i].like, g_mime [i].n_seen, g_mime [i].b_seen, g_mime [i].n_kept, g_mime [i].b_kept);
      /*---(done)------------------------*/
   }
   /*---(wrapup)-------------------------*/
   fprintf (f, "\n");
   /*---(close)--------------------------*/
   rc = FILE__close (&f);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   DEBUG_OUTP   yLOG_point   ("f"         , f);
   --rce;  if (rc < 0 || f != NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       writing tree files                     ----===*/
/*====================------------------------------------====================*/
static void  o___TREE____________o () { return; }

char
MIME_treehead      (void)
{
   printf ("\n#---name------------------------------------------  --------size---  -------count---  ---desc------------------------------------------- \n");
   return 0;
}

char
MIME_treecat       (char a_cat)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   llong       x_total     =    0;
   llong       x_count     =    0;
   char        x_title     [500]       = "";
   char        x_cat       [500]       = "n/a";
   int         c           =    0;
   /*---(summary run)--------------------*/
   /*> printf ("\n\nrunning cat %c\n", a_cat);                                        <*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].cat != a_cat)  continue;
      /*> printf ("      %-44.44s | %15lld¦", g_mime[i].desc, g_mime [i].b_seen);        <*/
      x_count += g_mime[i].n_seen;
      x_total += g_mime[i].b_seen;
   }
   for (i = 0; i < s_ncat; ++i) {
      if (s_cats [i].cat == 0    ) break;
      if (s_cats [i].cat != a_cat) continue;
      strcpy (x_cat, s_cats [i].desc);
   }
   MIME_treehead ();
   printf ("   %-44.44s     %15lld  %15lld  %-50.50s \n", x_cat, x_total, x_count, x_cat);
   /*---(detail run)---------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].cat != a_cat)  continue;
      if ((c % 5) == 0) printf ("\n");
      sprintf (x_title, "%s, %s", g_mime [i].ext, g_mime [i].desc);
      printf  ("      %-44.44s  %15lld  %15lld  %-50.50s ¦", g_mime [i].ext, g_mime [i].b_seen, g_mime [i].n_seen, x_title); 
      ++c;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME_tree          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        x_save      = g_mime [i].cat;
   /*---(header)-------------------------*/
   printf ("#!/usr/local/bin/hyleoroi\n");
   printf ("#   hyleoroi -- tree visualization input file\n");
   printf ("#   mime-type summary written by helios\n");
   printf ("\n\n\n");
   printf ("SIMPLE-FORMAT\n");
   printf ("\n\n\n");
   printf ("#--context  ---values------------------------------- \n");
   printf ("source      helios-phaeton                           \n");
   printf ("label       total file size by mime category         \n");
   printf ("format      mimecat                                  \n");
   printf ("\n\n\n");
   /*---(root)---------------------------*/
   MIME_treehead ();
   printf ("%-44.44s        %15lld  %15lld  %-50.50s \n"   , h_drive->device, h_drive->size, g_mime [0].n_seen, h_drive->device);
   /*---(process all)--------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].cat == x_save)  continue;
      x_save = g_mime [i].cat;
      MIME_treecat (x_save);
   }
   /*---(footer)-------------------------*/
   printf ("   %-44.44s     %15lld  %15lld  %-50.50s ¦", "(empty)", h_drive->size - g_mime [0].b_seen, 0, "(empty)");
   MIME_treehead ();
   printf ("# END-OF-FILE\n");
   /*---(complete)-----------------------*/
   return 0;
}

/*> char                                                                              <* 
 *> MIME__find_match        (cchar *a_ext, int *a_index, char *a_cat)                 <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    int         rc          =  -1;                                                 <* 
 *>    char        x_ext       [LEN_TERSE] = "";                                      <* 
 *>    int         i           =   0;                                                 <* 
 *>    int         x_len       =   0;                                                 <* 
 *>    strcpy (x_ext, a_ext);                                                         <* 
 *>    x_len = strlen (x_ext);                                                        <* 
 *>    for (i = 0; i < x_len; ++i)   x_ext [i] = tolower (x_ext [i]);                 <* 
 *>    /+---(search for match)---------------+/                                       <* 
 *>    for (i = 0; i < n_mime; ++i) {                                                 <* 
 *>       /+---(filter)----------------------+/                                       <* 
 *>       if (g_mime [i].ext  [0] != x_ext [0])      continue;                        <* 
 *>       if (g_mime [i].ext  [1] != x_ext [1])      continue;                        <* 
 *>       if (strcmp (g_mime [i].ext , x_ext) != 0)  continue;                        <* 
 *>       /+---(update)----------------------+/                                       <* 
 *>       if (a_index != NULL)  *a_index = i;                                         <* 
 *>       if (a_cat   != NULL)  *a_cat   = g_mime [i].cat;                            <* 
 *>       /+---(get out)---------------------+/                                       <* 
 *>       rc = 0;                                                                     <* 
 *>       break;                                                                      <* 
 *>    }                                                                              <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

/*> char         /+===[[ create entry statistics ]]===========[ ------ [ ------ ]=+/                 <* 
 *> MIME_find_by_ext        (cchar *a_ext, cchar *a_name, int *a_index, char *a_cat, long a_bytes)   <* 
 *> {                                                                                                <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                      <* 
 *>    char        rce         =  -10;                                                               <* 
 *>    int         rc          = -1;            /+ return code                    +/                 <* 
 *>    int         x_len       =  0;            /+ length of extension            +/                 <* 
 *>    char        x_ext       [LEN_TERSE] = "";                                                     <* 
 *>    int         i           =  0;            /+ generic iterator               +/                 <* 
 *>    /+---(header)-------------------------+/                                                      <* 
 *>    DEBUG_DATA   yLOG_enter   (__FUNCTION__);                                                     <* 
 *>    /+---(prepare)------------------------+/                                                      <* 
 *>    if (a_index != NULL)  *a_index = 0;                                                           <* 
 *>    if (a_cat   != NULL)  *a_cat   = MIME_HUH;                                                    <* 
 *>    /+---(defense)------------------------+/                                                      <* 
 *>    DEBUG_DATA   yLOG_point   ("a_ext"     , a_ext);                                              <* 
 *>    --rce;  if (a_ext == NULL) {                                                                  <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                                             <* 
 *>       return rce;                                                                                <* 
 *>    }                                                                                             <* 
 *>    x_len = strlen  (a_ext);                                                                      <* 
 *>    DEBUG_DATA   yLOG_complex ("ext"       , "%d, %s", x_len, a_ext);                             <* 
 *>    --rce;  if (x_len <= 0) {                                                                     <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                                             <* 
 *>       return rce;                                                                                <* 
 *>    }                                                                                             <* 
 *>    --rce;  if (x_len >=  LEN_TERSE) {                                                            <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                                             <* 
 *>       return rce;                                                                                <* 
 *>    }                                                                                             <* 
 *>    DEBUG_DATA   yLOG_point   ("a_name"    , a_name);                                             <* 
 *>    --rce;  if (a_name == NULL) {                                                                 <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                                             <* 
 *>       return rce;                                                                                <* 
 *>    }                                                                                             <* 
 *>    DEBUG_DATA   yLOG_info    ("a_name"    , a_name);                                             <* 
 *>    /+---(prepare)------------------------+/                                                      <* 
 *>    strcpy (x_ext, a_ext);                                                                        <* 
 *>    for (i = 0; i < x_len; ++i)   x_ext [i] = tolower (x_ext [i]);                                <* 
 *>    /+---(search for match)---------------+/                                                      <* 
 *>    for (i = 0; i < n_mime; ++i) {                                                                <* 
 *>       /+---(filter)----------------------+/                                                      <* 
 *>       if (g_mime [i].ext  [0] != x_ext [0])      continue;                                       <* 
 *>       if (g_mime [i].ext  [1] != x_ext [1])      continue;                                       <* 
 *>       if (strcmp (g_mime [i].ext , x_ext) != 0)  continue;                                       <* 
 *>       /+---(update)----------------------+/                                                      <* 
 *>       if (a_index != NULL)  *a_index = i;                                                        <* 
 *>       if (a_cat   != NULL)  *a_cat   = g_mime [i].cat;                                           <* 
 *>       /+---(update mime)-----------------+/                                                      <* 
 *>       if (a_bytes > 0) {                                                                         <* 
 *>          /+---(entry)----------+/                                                                <* 
 *>          ++(g_mime [i].n_seen);                                                                  <* 
 *>          g_mime [i].b_seen += a_bytes;                                                           <* 
 *>          /+---(counters)-------+/                                                                <* 
 *>          /+> MIME_add_seen (g_mime [i].cat, a_bytes);                                 <+/        <* 
 *>          /+---(done)-----------+/                                                                <* 
 *>       }                                                                                          <* 
 *>       /+---(get out)---------------------+/                                                      <* 
 *>       rc = 0;                                                                                    <* 
 *>       break;                                                                                     <* 
 *>    }                                                                                             <* 
 *>    DEBUG_DATA   yLOG_value   ("rc"        , rc);                                                 <* 
 *>    /+---(complete)-----------------------+/                                                      <* 
 *>    DEBUG_DATA   yLOG_exit    (__FUNCTION__);                                                     <* 
 *>    return rc;                                                                                    <* 
 *> }                                                                                                <*/

/*> char                                                                               <* 
 *> MIME_find_whole         (cchar *a_name, uchar *a_cat, long a_bytes)                <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                        <* 
 *>    char        rce         =  -10;                                                 <* 
 *>    int         rc          = -1;            /+ return code                    +/   <* 
 *>    char        x_name      [LEN_TERSE] = "";                                       <* 
 *>    int         i           =  0;            /+ generic iterator               +/   <* 
 *>    int         x_len       =   0;                                                  <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_DATA   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(prepare)------------------------+/                                        <* 
 *>    if (a_cat   != NULL)  *a_cat   = MIME_HUH;                                      <* 
 *>    /+---(defense)------------------------+/                                        <* 
 *>    DEBUG_DATA   yLOG_point   ("a_name"    , a_name);                               <* 
 *>    --rce;  if (a_name == NULL) {                                                   <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    x_len = strlen  (a_name);                                                       <* 
 *>    DEBUG_DATA   yLOG_complex ("name"      , "%d, %s", x_len, a_name);              <* 
 *>    --rce;  if (x_len == 0 || x_len > 9) {                                          <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(make copy)----------------------+/                                        <* 
 *>    strlcpy (x_name, a_name, LEN_TERSE);                                            <* 
 *>    for (i = 0; i < x_len; ++i)   x_name [i] = tolower (x_name [i]);                <* 
 *>    DEBUG_DATA   yLOG_info    ("x_name"    , x_name);                               <* 
 *>    rc = MIME_find_by_ext (x_name, "", NULL, a_cat, a_bytes);                       <* 
 *>    DEBUG_DATA   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return rc;                                                                      <* 
 *> }                                                                                  <*/



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
MIME__unit              (char *a_question, char *a_ext, int n)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
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
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME count       : mime=%4d, cats=%4d", n_mime, s_ncat);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME entry (%3d) : %c %-7.7s %-17.17s  %c  [%3d,%7lds] [%3d,%7ldk] [%3d,%7ldf]", n,
            g_mime [n].cat   , g_mime [n].ext , g_mime [n].desc, g_mime [n].like ,
            g_mime [n].n_seen  , g_mime [n].b_seen ,
            g_mime [n].n_kept  , g_mime [n].b_kept ,
            g_mime [n].n_found , g_mime [n].b_found);
   }
   else if (strcmp (a_question, "ext"           ) == 0) {
      if (a_ext == NULL) {
         snprintf (unit_answer, LEN_FULL, "MIME ext   (---) : no ext given");
      } else {
         rc = MIME_get_index (a_ext, NULL, &n);
         if (rc < 0)  snprintf (unit_answer, LEN_FULL, "MIME entry (---) : - %-7.7s no entry found", a_ext);
         else {
            x_curr = &(g_mime [n]);
            snprintf (unit_answer, LEN_FULL, "MIME ext   (%3d) : %c %-7.7s %-17.17s  %c  [%3d,%7lds] [%3d,%7ldk] [%3d,%7ldf]", n,
                  x_curr->cat   , x_curr->ext , x_curr->desc, x_curr->like ,
                  x_curr->n_seen  , x_curr->b_seen ,
                  x_curr->n_kept  , x_curr->b_kept ,
                  x_curr->n_found , x_curr->b_found);
         }
      }
   }
   else if (strcmp (a_question, "cat"           ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME cat    (%2d) : %c %-7.7s %-20.20s  [%3d,%7lds] [%3d,%7ldk] [%3d,%7ldf]",
            n, s_cats [n].cat, s_cats [n].name, s_cats [n].desc,
            s_cats [n].n_seen , s_cats [n].b_seen ,
            s_cats [n].n_kept , s_cats [n].b_kept ,
            s_cats [n].n_found, s_cats [n].b_found);
   }
   else if (strcmp (a_question, "s_head"        ) == 0) {
      strlencode (s_head, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_head      : [%.79s]", s_head);
   }
   else if (strcmp (a_question, "s_cols"        ) == 0) {
      strlencode (s_cols, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_cols      : [%.79s]", s_cols);
   }
   else if (strcmp (a_question, "s_recd"        ) == 0) {
      strlencode (s_recd, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_recd      : [%.79s]", s_recd);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}


