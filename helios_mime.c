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
 * METIS § wv4·· § create logic to verify mime file owner, group, and perms               § N9JLe3 §  · §
 * METIS § wv2·· § add logic to treat mime load like a pull request                       § N9JLeV §  · §
 * METIS § wg··· § allow mime file name override for testing purposes                     § N9JLgC §  · §
 * METIS § wv··· § merge cCAT and cMIME data structures (almost identical)                § N9M1Ij §  · §
 *
 */


tMIME       g_cats  [MAX_CAT];
int         g_ncat  = 0;
tMIME       g_mime  [MAX_MIME];
int         g_nmime = 0;


static char  s_head    [LEN_RECD]  = "";
static char  s_cols    [LEN_RECD]  = "";
static char  s_recd    [LEN_RECD]  = "";


/*---(simple file reading stats)---------*/
static    int s_seen  = 0;
static    int s_pass  = 0;
static    int s_fail  = 0;



/*====================------------------------------------====================*/
/*===----                        mass changes                          ----===*/
/*====================------------------------------------====================*/
static void  o___MASS____________o () { return; }

char
MIME__reset_all         (tMIME *a_curr)
{
   a_curr->n_seen   = 0;
   a_curr->b_seen   = 0;
   a_curr->n_kept   = 0;
   a_curr->b_kept   = 0;
   a_curr->n_found  = 0;
   a_curr->b_found  = 0;
   return 0;
}

char
MIME__reset_found       (tMIME *a_curr)
{
   a_curr->n_found  = 0;
   a_curr->b_found  = 0;
   return 0;
}

char
MIME__clear             (tMIME *a_curr)
{
   ystrlcpy (a_curr->ext , "", LEN_TERSE);
   a_curr->cat       = '-';
   ystrlcpy (a_curr->desc, "", LEN_DESC);
   a_curr->like      = '-';
   MIME__reset_all (a_curr);
   return 0;
}

char
MIME_reset_all          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =  0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   /*---(reset cats)---------------------*/
   DEBUG_CONF  yLOG_snote   ("cats");
   DEBUG_CONF  yLOG_sint    (g_ncat);
   for (i = 0; i < g_ncat;  ++i)   MIME__reset_all (&(g_cats [i]));
   /*---(reset mime)---------------------*/
   DEBUG_CONF  yLOG_snote   ("mime");
   DEBUG_CONF  yLOG_sint    (g_nmime);
   for (i = 0; i < g_nmime; ++i)    MIME__reset_all (&(g_mime [i]));
      /*---(complete)-----------------------*/
      DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MIME_reset_found        (void)
{
   int         i           =  0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   /*---(reset cats)---------------------*/
   DEBUG_CONF  yLOG_snote   ("cats");
   DEBUG_CONF  yLOG_sint    (g_ncat);
   for (i = 0; i < g_ncat;  ++i)   MIME__reset_found (&(g_cats [i]));
   /*---(reset mime)---------------------*/
   DEBUG_CONF  yLOG_snote   ("mime");
   DEBUG_CONF  yLOG_sint    (g_nmime);
   for (i = 0; i < g_nmime; ++i)    MIME__reset_found (&(g_mime [i]));
      /*---(complete)-----------------------*/
      DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MIME__purge             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(purge cats)---------------------*/
   DEBUG_CONF  yLOG_note    ("initialize cats table");
   for (i = 0; i < MAX_CAT;  ++i)   MIME__clear  (&(g_cats [i]));
   /*---(purge mime)---------------------*/
   DEBUG_CONF  yLOG_note    ("initialize mime table");
   for (i = 0; i < MAX_MIME; ++i)   MIME__clear  (&(g_mime [i]));
   /*---(reset globals)------------------*/
   DEBUG_CONF  yLOG_note    ("reset global counters");
   g_ncat     = 0;
   g_nmime    = 0;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(clear table)--------------------*/
   MIME__purge ();
   /*---(read config)--------------------*/
   rc = MIME_pull (my.file_mime);
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
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
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   DEBUG_CONF  yLOG_schar   (a_action);
   DEBUG_CONF  yLOG_sint    (a_bytes);
   /*---(default)------------------------*/
   if (a_cindex != NULL)  *a_cindex = -1;
   if (a_mindex != NULL)  *a_mindex = -1;
   if (a_cat    != NULL)  *a_cat    = MIME_HUH;
   /*---(defense)------------------------*/
   DEBUG_CONF  yLOG_spoint  (a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_snote   (a_ext);
   x_len = strlen  (a_ext);
   DEBUG_CONF  yLOG_sint    (x_len);
   --rce;  if (x_len <= 0 || x_len >= LEN_TERSE) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_full != NULL) {
      DEBUG_CONF  yLOG_snote   (a_full);
   }
   /*---(prepare)------------------------*/
   ystrlcpy (x_ext, a_ext, LEN_TERSE);
   for (i = 0; i < x_len; ++i)   x_ext [i] = tolower (x_ext [i]);
   if (a_bytes < 0)  x_inc = -1;
   /*---(find mime)----------------------*/
   for (i = 0; i < g_nmime; ++i) {
      if (g_mime [i].ext  [0] != x_ext [0])      continue;
      if (g_mime [i].ext  [1] != x_ext [1])      continue;
      if (strcmp (g_mime [i].ext , x_ext) != 0)  continue;
      m     = i;
      x_cat = g_mime [m].cat;
      break;
   }
   DEBUG_CONF  yLOG_sint    (m);
   --rce;  if (m < 0) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_snote   (g_mime [m].ext);
   DEBUG_CONF  yLOG_schar   (x_cat);
   /*---(find cat)-----------------------*/
   for (i = 0; i < g_ncat; ++i) {
      if (g_cats [i].cat != x_cat)  continue;
      c = i;
      break;
   }
   DEBUG_CONF  yLOG_sint    (c);
   --rce;  if (c < 0) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_snote   (g_cats [c].ext);
   /*---(act)----------------------------*/
   --rce;  switch (a_action) {
   case '-'  : 
      break;
   case DEST_STDOUT  : 
      DEBUG_CONF  yLOG_snote   ("seen");
      DEBUG_CONF  yLOG_sint    (x_inc);
      g_cats [0].n_seen  += x_inc;
      g_cats [c].n_seen  += x_inc;
      g_mime [m].n_seen  += x_inc;
      g_cats [0].b_seen  += a_bytes;
      g_cats [c].b_seen  += a_bytes;
      g_mime [m].b_seen  += a_bytes;
      DEBUG_CONF  yLOG_sint    (g_mime [m].n_seen);
      DEBUG_CONF  yLOG_sint    (g_mime [m].b_seen);
      break;
   case 'k'  : 
      DEBUG_CONF  yLOG_snote   ("kept");
      DEBUG_CONF  yLOG_sint    (x_inc);
      g_cats [0].n_kept  += x_inc;
      g_cats [c].n_kept  += x_inc;
      g_mime [m].n_kept  += x_inc;
      g_cats [0].b_kept  += a_bytes;
      g_cats [c].b_kept  += a_bytes;
      g_mime [m].b_kept  += a_bytes;
      DEBUG_CONF  yLOG_sint    (g_mime [m].n_kept);
      DEBUG_CONF  yLOG_sint    (g_mime [m].b_kept);
      break;
   case 'f'  : 
      DEBUG_CONF  yLOG_snote   ("found");
      DEBUG_CONF  yLOG_sint    (x_inc);
      g_cats [0].n_found += x_inc;
      g_cats [c].n_found += x_inc;
      g_mime [m].n_found += x_inc;
      g_cats [0].b_found += a_bytes;
      g_cats [c].b_found += a_bytes;
      g_mime [m].b_found += a_bytes;
      DEBUG_CONF  yLOG_sint    (g_mime [m].n_found);
      DEBUG_CONF  yLOG_sint    (g_mime [m].b_found);
      break;
   case 'c'  :
      break;
   case 'i'  :
      break;
   default   :
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   if (a_cindex != NULL)  *a_cindex = c;
   if (a_mindex != NULL)  *a_mindex = m;
   if (a_cat    != NULL)  *a_cat    = x_cat;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_cat   != NULL)  *a_cat   = MIME_HUH;
   /*---(defense)------------------------*/
   DEBUG_CONF  yLOG_point   ("a_ext"     , a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len = strlen  (a_ext);
   DEBUG_CONF  yLOG_complex ("ext"       , "%d, %s", x_len, a_ext);
   --rce;  if (x_len == 0 || x_len > 2) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(make copy)----------------------*/
   ystrlcpy (x_ext, a_ext, LEN_TERSE);
   --rce;  if (strchr ("123456789", x_ext [0]) == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len > 1 && strchr ("x"        , x_ext [1]) == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = MIME_add_seen ("manual", a_cat, a_bytes, NULL);
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return rc;
}


/*====================------------------------------------====================*/
/*===----                       simple accessors                       ----===*/
/*====================------------------------------------====================*/
static void  o___ACCESSOR________o () { return; }

uchar  MIME_cat_abbr    (int c) { return g_cats [c].cat;  }
uchar *MIME_cat_name    (int c) { return g_cats [c].ext; }

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
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   if (n != NULL)   *n = -1;
   for (i = 0; i <= g_ncat; ++i) {
      if (strcmp (a_name, g_cats [i].ext) == 0) {
         DEBUG_CONF  yLOG_snote   ("FOUND ext");
         DEBUG_CONF  yLOG_sint    (i);
         if (n != NULL)   *n = i;
         DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      if (a_cat == g_cats [i].cat) {
         DEBUG_CONF  yLOG_snote   ("FOUND cat");
         DEBUG_CONF  yLOG_sint    (i);
         if (n != NULL)   *n = i;
         DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
   }
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
}

char
MIME__handler           (int a_line, uchar *a_verb)
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   DEBUG_CONF  yLOG_complex ("args"      , "%3d, %s", a_line, a_verb);
   /*---(read)---------------------------*/
   rc = yPARSE_scanf (a_verb, "TcDc,s;s,s;s", x_name, &x_cat, x_desc, &x_like, &x_scount, NULL, &x_ssize, NULL, &x_kcount, NULL, &x_ksize, NULL);
   DEBUG_CONF  yLOG_complex ("returns"   , "%-10.10s %c %-10.10s %c %10.0d %16.0ld %10.0d %16.0ld", x_name, x_cat, x_desc, x_like, x_scount, x_ssize, x_kcount, x_ksize);
   /*---(filter)-------------------------*/
   if (strncmp (a_verb, "s_", 2) == 0 || strncmp (a_verb, "f_", 2) == 0) {
      DEBUG_CONF  yLOG_note    ("file header data");
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   ++s_seen;
   /*---(defense)------------------------*/
   --rce; if (x_name == NULL || strcmp (x_name, "") == 0 || strlen (x_name) <= 0) {
      DEBUG_CONF  yLOG_note    ("FATAL, suffix/label was empty/null");
      yURG_err ('f', "%3d suffix/label was empty/null", a_line);
      ++s_fail;
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce; if (strchr ("Tabcdefghijklmnopqrstuvwxyz?!-", x_cat) == NULL) {
      DEBUG_CONF  yLOG_note    ("FATAL, cat not allowed");
      yURG_err ('f', "%3d category å%cæ not recognized", a_line, x_cat);
      ++s_fail;
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(duplicate)----------------------*/
   if        (strcmp (a_verb, "CAT") == 0) {
      rc = MIME__find_cat (x_cat, x_name, &n);
   } else if (strcmp (a_verb, "ext") == 0) {
      rc = MIME_get_index (x_name, NULL, &n);
   } else {
      DEBUG_CONF  yLOG_note    ("FATAL, verb not recognized");
      yURG_err ('f', "%3d verb å%sæ is not either ¶CAT¶ or ¶ext¶", a_line, a_verb);
      ++s_fail;
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_CONF  yLOG_value   ("rc"        , rc);
   DEBUG_CONF  yLOG_value   ("n"         , n);
   --rce;  if (n >= 0) {
      DEBUG_CONF  yLOG_info    ("FATAL, duplicate" , x_name);
      yURG_err ('f', "%3d verb å%sæ label å%sæ is a duplicate", a_line, a_verb, x_name);
      ++s_fail;
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(save)---------------------------*/
   switch (a_verb [0]) {
   case 'C' :
      ystrlcpy (g_cats [g_ncat].ext, x_name, LEN_TERSE);
      g_cats [g_ncat].cat     = x_cat;
      ystrlcpy (g_cats [g_ncat].desc, x_desc, LEN_DESC);
      g_cats [g_ncat].n_seen  = x_scount;
      g_cats [g_ncat].b_seen  = x_ssize;
      g_cats [g_ncat].n_kept  = x_kcount;
      g_cats [g_ncat].b_kept  = x_ksize;
      DEBUG_CONF  yLOG_complex ("cat added" , "%-10.10s %c %-10.10s   %10.0d %16.0ld %10.0d %16.0ld", g_cats [g_ncat].ext, g_cats [g_ncat].cat, g_cats [g_ncat].desc, g_cats [g_ncat].n_seen, g_cats [g_ncat].b_seen, g_cats [g_ncat].n_kept, g_cats [g_ncat].b_kept);
      ++g_ncat;
      if (x_cat == MIME_EMPTY)  my.empty = x_ssize;
      break;
   case 'e' :
      ystrlcpy (g_mime [g_nmime].ext , x_name, LEN_TERSE);
      g_mime [g_nmime].cat     = x_cat;
      ystrlcpy (g_mime [g_nmime].desc, x_desc, LEN_DESC);
      g_mime [g_nmime].like    = x_like;
      g_mime [g_nmime].n_seen  = x_scount;
      g_mime [g_nmime].b_seen  = x_ssize;
      g_mime [g_nmime].n_kept  = x_kcount;
      g_mime [g_nmime].b_kept  = x_ksize;
      DEBUG_CONF  yLOG_complex ("mime added", "%-10.10s %c %-10.10s   %10.0d %16.0ld %10.0d %16.0ld", g_mime [g_nmime].ext, g_mime [g_nmime].cat, g_mime [g_nmime].desc, g_mime [g_nmime].n_seen, g_mime [g_nmime].b_seen, g_mime [g_nmime].n_kept, g_mime [g_nmime].b_kept);
      ++g_nmime;
      break;
   }
   /*---(final count)--------------------*/
   yURG_msg ('-', "%3d verb å%sæ label å%sæ has been added", a_line, a_verb, x_name);
   ++s_pass;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_pull          (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading mime categorization file...");
   /*---(purge the tables)---------------*/
   rc = yPARSE_reset_in ();
   DEBUG_CONF   yLOG_value   ("purge_in"  , rc);
   rc = MIME__purge ();
   DEBUG_CONF   yLOG_value   ("purge"     , rc);
   s_seen = s_pass = s_fail = 0;
   /*---(read in configuration)----------*/
   rc = yPARSE_autoread (a_name, NULL, MIME__handler);
   DEBUG_CONF   yLOG_value   ("autoread"  , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed while openning mime file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yPARSE_close ();
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed closing mime file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle results)-----------------*/
   yURG_msg ('-', "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   DEBUG_CONF   yLOG_complex ("values"    , "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   --rce; if (s_seen <= 0) {
      yURG_err ('f', "zero mime records found");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce; if (s_fail > 0) {
      yURG_err ('f', "ended with failed mime records");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, mime file read");
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       writing mime files                     ----===*/
/*====================------------------------------------====================*/
static void  o___OUTPUT__________o () { return; }

char
MIME__write_clear       (void)
{
   ystrlcpy (s_head, "", LEN_RECD);
   ystrlcpy (s_cols, "", LEN_RECD);
   ystrlcpy (s_recd, "", LEN_RECD);
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
   xn_spct  = ((double) g_cats [n].n_seen / g_cats [0].n_seen) * 100;
   xb_spct  = ((double) g_cats [n].b_seen / g_cats [0].b_seen) * 100;
   xn_kpct  = ((double) g_cats [n].n_kept / g_cats [0].n_kept) * 100;
   xb_kpct  = ((double) g_cats [n].b_kept / g_cats [0].b_kept) * 100;
   xn_pct   = ((double) g_cats [n].n_kept / g_cats [n].n_seen) * 100;
   xb_pct   = ((double) g_cats [n].b_kept / g_cats [n].b_seen) * 100;
   xn_fpct  = ((double) g_cats [n].n_found / g_cats [0].n_found) * 100;
   xb_fpct  = ((double) g_cats [n].b_found / g_cats [0].b_found) * 100;
   xn_pctf  = ((double) g_cats [n].n_found / g_cats [n].n_seen) * 100;
   xb_pctf  = ((double) g_cats [n].b_found / g_cats [n].b_seen) * 100;
   yPARSE_printf ("CAT","TcDc,s;s,s;sss,s;sss",
         g_cats [n].ext, g_cats [n].cat, g_cats [n].desc, '-',
         g_cats [n].n_seen, xn_spct, g_cats [n].b_seen, xb_spct,
         g_cats [n].n_kept, xn_kpct, g_cats [n].b_kept, xb_kpct,
         xn_pct, xb_pct,
         g_cats [n].n_found, xn_fpct, g_cats [n].b_found, xb_fpct,
         xn_pctf, xb_pctf);
   /*---(process mime ext)---------------*/
   for (i = 0; i < g_nmime; ++i) {
      if (g_mime [i].cat != x_cat)  continue;
      xn_spct  = ((double) g_mime [i].n_seen / g_cats [0].n_seen) * 100;
      xb_spct  = ((double) g_mime [i].b_seen / g_cats [0].b_seen) * 100;
      xn_kpct  = ((double) g_mime [i].n_kept / g_cats [0].n_kept) * 100;
      xb_kpct  = ((double) g_mime [i].b_kept / g_cats [0].b_kept) * 100;
      xn_pct   = ((double) g_mime [i].n_kept / g_mime [i].n_seen) * 100;
      xb_pct   = ((double) g_mime [i].b_kept / g_mime [i].b_seen) * 100;
      xn_fpct  = ((double) g_mime [i].n_found / g_cats [0].n_found) * 100;
      xb_fpct  = ((double) g_mime [i].b_found / g_cats [0].b_found) * 100;
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
   for (i = 0; i < g_ncat; ++i) {
      yPARSE_handler_plus (g_cats [i].ext, "TcDc,s;s,s;sss,s;sss", i, NULL, MIME__write_callback, "ext,t,desc,l,seen,pct,seen-bytes,pct,kept,pct,kept-bytes,pct,cnt,siz,found,pct,found-bytes,pct,cnt,siz");
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   DEBUG_CONF  yLOG_char    ("a_type"    , a_type);
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
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ystrlcpy (s_recd, "", LEN_RECD);
   /*---(main)---------------------------*/
   DEBUG_CONF  yLOG_complex ("node"      , "%2d %-10.10s %10ld %10d %s", a_lvl, a_ext, a_size, a_count, a_desc);
   if (a_size > 0)  yPARSE_vprintf (c, "node", a_lvl, a_ext, a_desc, a_size, a_count, a_size / a_count, 0, 0, 0);
   ++c;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(header)-------------------------*/
   yPARSE_planned (NULL, "stdout", NULL);
   yPARSE_header  (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, "file size by mime category", MIME__tree_header);
   yPARSE_handler_plus ("node", "sTDhlllll", 0.0, NULL, NULL, "lvl,type,description,size,count,avg,1st,2nd,3rd");
   /*---(root)---------------------------*/
   if      (my.mime_all != '-')     MIME__tree_line      (0, g_cats [n].ext, g_cats [n].b_seen , g_cats [n].n_seen , g_cats [n].desc);
   else if (g_cats [0].b_found > 0) MIME__tree_line      (0, g_cats [n].ext, g_cats [n].b_found, g_cats [n].n_found, g_cats [n].desc);
   else                             MIME__tree_line      (0, g_cats [n].ext, g_cats [n].b_kept , g_cats [n].n_kept , g_cats [n].desc);
   /*---(process all)--------------------*/
   for (i = 0; i < g_nmime; ++i) {
      if (g_mime [i].cat != x_save) {
         x_save = g_mime [i].cat;
         MIME__find_cat (x_save, "¬¬¬¬¬", &n);
         if      (my.mime_all != '-')     MIME__tree_line  (1, g_cats [n].ext, g_cats [n].b_seen , g_cats [n].n_seen , g_cats [n].desc);
         else if (g_cats [0].b_found > 0) MIME__tree_line  (1, g_cats [n].ext, g_cats [n].b_found, g_cats [n].n_found, g_cats [n].desc);
         else                             MIME__tree_line  (1, g_cats [n].ext, g_cats [n].b_kept , g_cats [n].n_kept , g_cats [n].desc);
      }
      if      (my.mime_all != '-')     MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_seen , g_mime [i].n_seen , g_mime [i].desc);
      else if (g_cats [0].b_found > 0) MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_found, g_mime [i].n_found, g_mime [i].desc);
      else                             MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_kept , g_mime [i].n_kept , g_mime [i].desc);
   }
   /*---(footer)-------------------------*/
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
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
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME count       : mime=%4d, cats=%4d", g_nmime, g_ncat);
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
            n, g_cats [n].cat, g_cats [n].ext, g_cats [n].desc,
            g_cats [n].n_seen , g_cats [n].b_seen ,
            g_cats [n].n_kept , g_cats [n].b_kept ,
            g_cats [n].n_found, g_cats [n].b_found);
   }
   else if (strcmp (a_question, "s_head"        ) == 0) {
      ystrlencode (s_head, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_head      : [%.79s]", s_head);
   }
   else if (strcmp (a_question, "s_cols"        ) == 0) {
      ystrlencode (s_cols, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_cols      : [%.79s]", s_cols);
   }
   else if (strcmp (a_question, "s_recd"        ) == 0) {
      ystrlencode (s_recd, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_recd      : [%.79s]", s_recd);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return unit_answer;
}


