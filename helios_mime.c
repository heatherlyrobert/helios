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
   uchar       name        [LEN_TERSE];
   uchar       cat;
   uchar       desc        [LEN_DESC];
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
MIME__action            (uchar *a_ext, int *a_cindex, int *a_mindex, uchar *a_cat, char a_action, llong a_bytes, char *a_full)
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
   --rce;  if (a_full != NULL) {
      DEBUG_STATS  yLOG_snote   (a_full);
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
      DEBUG_STATS  yLOG_sint    (x_inc);
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
      DEBUG_STATS  yLOG_sint    (x_inc);
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
      DEBUG_STATS  yLOG_sint    (x_inc);
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

char MIME_get_index   (uchar *a_ext, int *a_cindex, int *a_mindex) { return MIME__action (a_ext, a_cindex, a_mindex, NULL, 'i', 0, NULL); }
char MIME_add_seen    (uchar *a_ext, uchar *a_cat, llong a_bytes, char *o_path) { return MIME__action (a_ext, NULL, NULL, a_cat, 's', a_bytes, o_path); }
char MIME_add_kept    (uchar *a_ext, llong a_bytes, char *o_path) { return MIME__action (a_ext, NULL , NULL, NULL, 'k', a_bytes, o_path); }
char MIME_add_found   (uchar *a_ext, llong a_bytes) { return MIME__action (a_ext, NULL , NULL, NULL, 'f', a_bytes, NULL); }
char MIME_del_seen    (uchar *a_ext, llong a_bytes, char *o_path) { return MIME__action (a_ext, NULL , NULL, NULL, 's', -a_bytes, o_path); }
char MIME_del_kept    (uchar *a_ext, llong a_bytes) { return MIME__action (a_ext, NULL , NULL, NULL, 'k', -a_bytes, NULL); }
char MIME_del_found   (uchar *a_ext, llong a_bytes) { return MIME__action (a_ext, NULL , NULL, NULL, 'f', -a_bytes, NULL); }

char
MIME_add_man            (uchar *a_ext, uchar *a_cat, long a_bytes)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =   -1;
   char        x_ext       [LEN_TERSE] = "";
   int         i           =   0;
   int         x_len       =   0;
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_cat   != NULL)  *a_cat   = MIME_HUH;
   /*---(defense)------------------------*/
   DEBUG_STATS  yLOG_point   ("a_ext"     , a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len = strlen  (a_ext);
   DEBUG_STATS  yLOG_complex ("ext"       , "%d, %s", x_len, a_ext);
   --rce;  if (x_len == 0 || x_len > 2) {
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(make copy)----------------------*/
   strlcpy (x_ext, a_ext, LEN_TERSE);
   --rce;  if (strchr ("123456789", x_ext [0]) == NULL) {
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len > 1 && strchr ("x"        , x_ext [1]) == NULL) {
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = MIME_add_seen ("manual", a_cat, a_bytes, NULL);
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
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
MIME__find_cat          (uchar a_cat, uchar *a_name, int *n)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_senter  (__FUNCTION__);
   if (n != NULL)   *n = -1;
   for (i = 0; i <= s_ncat; ++i) {
      if (strcmp (a_name, s_cats [i].name) == 0) {
         DEBUG_STATS  yLOG_snote   ("FOUND ext");
         DEBUG_STATS  yLOG_sint    (i);
         if (n != NULL)   *n = i;
         DEBUG_STATS  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      if (a_cat == s_cats [i].cat) {
         DEBUG_STATS  yLOG_snote   ("FOUND cat");
         DEBUG_STATS  yLOG_sint    (i);
         if (n != NULL)   *n = i;
         DEBUG_STATS  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
   }
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
}

char
MIME__read_one          (int a_line, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   uchar       x_name      [LEN_LABEL] = "";
   uchar       x_cat       =  '-';
   uchar       x_desc      [LEN_DESC]  = "";
   uchar       x_like      =  '-';
   int         x_scount    =    0;
   llong       x_ssize     =    0;
   int         x_kcount    =    0;
   llong       x_ksize     =    0;
   int         i           =    0;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   DEBUG_STATS  yLOG_complex ("args"      , "%3d, %s", a_line, a_verb);
   /*---(read)---------------------------*/
   rc = yPARSE_scanf (a_verb, "TcDc,s;s,s;s", x_name, &x_cat, x_desc, &x_like, &x_scount, NULL, &x_ssize, NULL, &x_kcount, NULL, &x_ksize, NULL);
   DEBUG_STATS  yLOG_complex ("returns"   , "%-10.10s %c %-10.10s %c %10.0d %16.0ld %10.0d %16.0ld", x_name, x_cat, x_desc, x_like, x_scount, x_ssize, x_kcount, x_ksize);
   /*---(defense)------------------------*/
   --rce; if (x_name == NULL || strcmp (x_name, "") == 0 || strlen (x_name) <= 0) {
      DEBUG_STATS  yLOG_note    ("label not found");
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce; if (strchr ("Tabcdefghijklmnopqrstuvwxyz?!-", x_cat) == NULL) {
      DEBUG_STATS  yLOG_note    ("cat not allowed");
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(duplicate)----------------------*/
   if        (strcmp (a_verb, "CAT") == 0) {
      rc = MIME__find_cat (x_cat, x_name, &n);
   } else if (strcmp (a_verb, "ext") == 0) {
      rc = MIME_get_index (x_name, NULL, &n);
   } else {
      DEBUG_STATS  yLOG_note    ("verb not recognized");
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_STATS  yLOG_value   ("rc"        , rc);
   DEBUG_STATS  yLOG_value   ("n"         , n);
   --rce;  if (n >= 0) {
      DEBUG_STATS  yLOG_info    ("duplicate" , x_name);
      DEBUG_STATS  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   switch (a_verb [0]) {
   case 'C' :
      strlcpy (s_cats [s_ncat].name, x_name, LEN_TERSE);
      s_cats [s_ncat].cat     = x_cat;
      strlcpy (s_cats [s_ncat].desc, x_desc, LEN_DESC);
      s_cats [s_ncat].n_seen  = x_scount;
      s_cats [s_ncat].b_seen  = x_ssize;
      s_cats [s_ncat].n_kept  = x_kcount;
      s_cats [s_ncat].b_kept  = x_ksize;
      DEBUG_STATS  yLOG_complex ("cat"       , "%-10.10s %c %-10.10s   %10.0d %16.0ld %10.0d %16.0ld", s_cats [s_ncat].name, s_cats [s_ncat].cat, s_cats [s_ncat].desc, s_cats [s_ncat].n_seen, s_cats [s_ncat].b_seen, s_cats [s_ncat].n_kept, s_cats [s_ncat].b_kept);
      ++s_ncat;
      if (x_cat == MIME_EMPTY)  my.empty = x_ssize;
      break;
   case 'e' :
      strlcpy (g_mime [n_mime].ext , x_name, LEN_TERSE);
      g_mime [n_mime].cat     = x_cat;
      strlcpy (g_mime [n_mime].desc, x_desc, LEN_DESC);
      g_mime [n_mime].like    = x_like;
      g_mime [n_mime].n_seen  = x_scount;
      g_mime [n_mime].b_seen  = x_ssize;
      g_mime [n_mime].n_kept  = x_kcount;
      g_mime [n_mime].b_kept  = x_ksize;
      DEBUG_STATS  yLOG_complex ("mime"      , "%-10.10s %c %-10.10s   %10.0d %16.0ld %10.0d %16.0ld", g_mime [n_mime].ext, g_mime [n_mime].cat, g_mime [n_mime].desc, g_mime [n_mime].n_seen, g_mime [n_mime].b_seen, g_mime [n_mime].n_kept, g_mime [n_mime].b_kept);
      ++n_mime;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_read          (void)
{
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   /*---(purge the tables)---------------*/
   MIME__purge ();
   yPARSE_autoread (my.file_mime, NULL, MIME__read_one);
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
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

int
MIME__write_callback    (int a_line, uchar *a_verb)
{
   char        rc          =    0;
   int         n           =    0;
   uchar       x_cat       =  ' ';
   int         i           =    0;
   int         c           =    0;
   int         xn_spct, xb_spct;
   int         xn_kpct, xb_kpct;
   int         xn_fpct, xb_fpct;
   int         xn_pct , xb_pct;
   int         xn_pctf, xb_pctf;
   /*---(start category)-----------------*/
   rc    = MIME__find_cat ('¬', a_verb, &n);
   x_cat = MIME_cat_abbr (n);
   yPARSE_sect_begin (a_verb);
   /*---(write category)-----------------*/
   xn_spct  = ((double) s_cats [n].n_seen / s_cats [0].n_seen) * 100;
   xb_spct  = ((double) s_cats [n].b_seen / s_cats [0].b_seen) * 100;
   xn_kpct  = ((double) s_cats [n].n_kept / s_cats [0].n_kept) * 100;
   xb_kpct  = ((double) s_cats [n].b_kept / s_cats [0].b_kept) * 100;
   xn_pct   = ((double) s_cats [n].n_kept / s_cats [n].n_seen) * 100;
   xb_pct   = ((double) s_cats [n].b_kept / s_cats [n].b_seen) * 100;
   xn_fpct  = ((double) s_cats [n].n_found / s_cats [0].n_found) * 100;
   xb_fpct  = ((double) s_cats [n].b_found / s_cats [0].b_found) * 100;
   xn_pctf  = ((double) s_cats [n].n_found / s_cats [n].n_seen) * 100;
   xb_pctf  = ((double) s_cats [n].b_found / s_cats [n].b_seen) * 100;
   yPARSE_printf ("CAT","TcDc,s;s,s;sss,s;sss",
         s_cats [n].name, s_cats [n].cat, s_cats [n].desc, '-',
         s_cats [n].n_seen, xn_spct, s_cats [n].b_seen, xb_spct,
         s_cats [n].n_kept, xn_kpct, s_cats [n].b_kept, xb_kpct,
         xn_pct, xb_pct,
         s_cats [n].n_found, xn_fpct, s_cats [n].b_found, xb_fpct,
         xn_pctf, xb_pctf);
   /*---(process mime ext)---------------*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].cat != x_cat)  continue;
      xn_spct  = ((double) g_mime [i].n_seen / s_cats [0].n_seen) * 100;
      xb_spct  = ((double) g_mime [i].b_seen / s_cats [0].b_seen) * 100;
      xn_kpct  = ((double) g_mime [i].n_kept / s_cats [0].n_kept) * 100;
      xb_kpct  = ((double) g_mime [i].b_kept / s_cats [0].b_kept) * 100;
      xn_pct   = ((double) g_mime [i].n_kept / g_mime [i].n_seen) * 100;
      xb_pct   = ((double) g_mime [i].b_kept / g_mime [i].b_seen) * 100;
      xn_fpct  = ((double) g_mime [i].n_found / s_cats [0].n_found) * 100;
      xb_fpct  = ((double) g_mime [i].b_found / s_cats [0].b_found) * 100;
      yPARSE_sect_break (c);
      yPARSE_printf ("ext","TcDc,s;s,s;sss,s;sss",
            g_mime [i].ext, g_mime [i].cat, g_mime [i].desc, g_mime [i].like,
            g_mime [i].n_seen, xn_spct, g_mime [i].b_seen, xb_spct,
            g_mime [i].n_kept, xn_kpct, g_mime [i].b_kept, xb_kpct,
            xn_pct, xb_pct,
            g_mime [i].n_found, xn_fpct, g_mime [i].b_found, xb_fpct,
            xn_pctf, xb_pctf);
      ++c;
      /*---(done)------------------------*/
   }
   /*---(end category)-------------------*/
   yPARSE_sect_end (c);
   /*---(complete)-----------------------*/
   return c;
}

char
MIME__write_handlers    (void)
{
   int         i           =    0;
   for (i = 0; i < s_ncat; ++i) {
      yPARSE_handler_plus (s_cats [i].name, "TcDc,s;s,s;sss,s;sss", i, NULL, MIME__write_callback, "ext,t,desc,l,seen,pct,seen-bytes,pct,kept,pct,kept-bytes,pct,cnt,siz,found,pct,found-bytes,pct,cnt,siz");
   }
   return 0;
}

char
MIME__write_header      (void)
{
   yPARSE_spacer (1);
   yPARSE_printf ("f_public"    , "c"     , my.pub               );
   yPARSE_printf ("f_path"      , "O"     , my.path              );
   yPARSE_printf ("f_regex"     , "O"     , my.regex             );
   yPARSE_printf ("f_types"     , "O"     , my.types             );
   yPARSE_printf ("f_mimes"     , "O"     , my.mimes             );
   yPARSE_printf ("f_sizes"     , "O"     , my.sizes             );
   yPARSE_printf ("f_ages"      , "O"     , my.ages              );
   yPARSE_printf ("f_ascii"     , "O"     , my.ascii             );
   yPARSE_printf ("f_super"     , "c"     , my.super             );
   return 0;
}

char
MIME_write         (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   DEBUG_STATS  yLOG_char    ("a_type"    , a_type);
   /*---(open file)----------------------*/
   switch (a_type) {
   case DEST_FILE   :
      yPARSE_planned  (NULL, my.file_mime, NULL);
      break;
   case DEST_STDOUT :
      yPARSE_planned  (NULL, "stdout"    , NULL);
      break;
   }
   /*---(prepare)------------------------*/
   yPARSE_header  (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, "mime configuration file", MIME__write_header);
   MIME__write_handlers ();
   yPARSE_write_all ();
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       writing tree files                     ----===*/
/*====================------------------------------------====================*/
static void  o___TREE____________o () { return; }

char
MIME__tree_header       (void)
{
   yPARSE_spacer (1);
   yPARSE_printf ("o_format"    , "O"     , "mimecat"            );
   yPARSE_printf ("o_label0"    , "O"     , "total bytes"        );
   yPARSE_printf ("o_label1"    , "O"     , "total count"        );
   yPARSE_printf ("o_label2"    , "O"     , "average"            );
   yPARSE_printf ("o_label3"    , "O"     , "primary"            );
   yPARSE_printf ("o_label4"    , "O"     , "secondary"          );
   yPARSE_printf ("o_label5"    , "O"     , "terciary"           );
   yPARSE_spacer (1);
   return 0;
}

char
MIME__tree_line         (char a_lvl, uchar *a_ext, llong a_size, llong a_count, char *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   static int  c           =    0;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   strlcpy (s_recd, "", LEN_RECD);
   /*---(main)---------------------------*/
   DEBUG_STATS  yLOG_complex ("node"      , "%2d %-10.10s %10ld %10d %s", a_lvl, a_ext, a_size, a_count, a_desc);
   if (a_size > 0)  yPARSE_vprintf (c, "node", a_lvl, a_ext, a_desc, a_size, a_count, a_size / a_count, 0, 0, 0);
   ++c;
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_tree          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         n           =    0;
   uchar       x_save      = MIME_TOTAL;
   /*---(header)-------------------------*/
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
   /*---(header)-------------------------*/
   yPARSE_planned (NULL, "stdout", NULL);
   yPARSE_header  (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, "file size by mime category", MIME__tree_header);
   yPARSE_handler_plus ("node", "sTDhlllll", 0.0, NULL, NULL, "lvl,type,description,size,count,avg,1st,2nd,3rd");
   /*---(root)---------------------------*/
   if      (my.mime_all != '-')     MIME__tree_line      (0, s_cats [n].name, s_cats [n].b_seen , s_cats [n].n_seen , s_cats [n].desc);
   else if (s_cats [0].b_found > 0) MIME__tree_line      (0, s_cats [n].name, s_cats [n].b_found, s_cats [n].n_found, s_cats [n].desc);
   else                             MIME__tree_line      (0, s_cats [n].name, s_cats [n].b_kept , s_cats [n].n_kept , s_cats [n].desc);
   /*---(process all)--------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].cat != x_save) {
         x_save = g_mime [i].cat;
         MIME__find_cat (x_save, "¬¬¬¬¬", &n);
            if      (my.mime_all != '-')     MIME__tree_line  (1, s_cats [n].name, s_cats [n].b_seen , s_cats [n].n_seen , s_cats [n].desc);
            else if (s_cats [0].b_found > 0) MIME__tree_line  (1, s_cats [n].name, s_cats [n].b_found, s_cats [n].n_found, s_cats [n].desc);
            else                             MIME__tree_line  (1, s_cats [n].name, s_cats [n].b_kept , s_cats [n].n_kept , s_cats [n].desc);
      }
      if      (my.mime_all != '-')     MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_seen , g_mime [i].n_seen , g_mime [i].desc);
      else if (s_cats [0].b_found > 0) MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_found, g_mime [i].n_found, g_mime [i].desc);
      else                             MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_kept , g_mime [i].n_kept , g_mime [i].desc);
   }
   /*---(footer)-------------------------*/
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return 0;
}



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
   DEBUG_STATS  yLOG_enter   (__FUNCTION__);
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
   DEBUG_STATS  yLOG_exit    (__FUNCTION__);
   return unit_answer;
}


