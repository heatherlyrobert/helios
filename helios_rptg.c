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
 * METIS § ····· § need to get yREGEX find stats working                                  § NA05on §  · §
 *
 *
 */



char  g_print    [LEN_RECD]  = "";
int   s_days    =   0;
char  s_age     = '-';
int   s_pos     =  -1;
int   s_len     =  -1;
static long  s_bytes   =   0;

#define   IF_MIME   if (my.columns [0] != '·') 

#define   IF_AGE    if (my.columns [1] != '·') 
#define   IF_NAGE   if (my.columns [1] == 'a') 
#define   IF_DAGE   if (my.columns [1] == 'A') 

#define   IF_SIZE   if (my.columns [2] != '·') 
#define   IF_NSIZE  if (my.columns [2] == 's') 
#define   IF_DSIZE  if (my.columns [2] == 'S') 
#define   IF_MSIZE  if (my.columns [2] == '$') 

#define   IF_LVL    if (my.columns [3] != '·') 

#define   IF_ASCII  if (my.columns [4] != '·') 

#define   IF_FIND   if (my.columns [5] != '·') 

#define   IF_TYPE   if (my.columns [6] != '·') 
#define   IF_NTYPE  if (my.columns [6] == 't') 
#define   IF_DTYPE  if (my.columns [6] == 'T') 

#define   IF_PERM   if (my.columns [7] != '·') 
#define   IF_NPERM  if (my.columns [7] == 'p') 
#define   IF_DPERM  if (my.columns [7] == 'P') 

#define   IF_DRIVE  if (my.columns [8] != '·') 
#define   IF_NDRIVE if (my.columns [8] == 'd') 
#define   IF_DDRIVE if (my.columns [8] == 'D') 

#define   IF_BASE   if (my.columns [9] != '·') 



#define     MAX_PERM         50
typedef     struct cPERM    tPERM;
struct cPERM {
   char    name  [LEN_LABEL];
   char    desc  [LEN_HUND];
   int     value;
   char    text  [LEN_TERSE];
};


const tPERM g_perms [MAX_PERM] = {
   /* 123456789012345    12345678901234567890123456789012345678901234567890123456789012345678901234567890   12345          */
   /* ---name--------    ---description------------------------------------------------------------------   value    text  */
   { "g_only"         , "only the group can see, access, or modify"                                       , 00070 , "00070" },
   { "g_share"        , "only the group can modify, but all others can see and access"                    , 00075 , "00075" },
   { "f_nodel"        , "everyone can see, access, and modify, but only owner can delete"                 , 01777 , "01777" },
   /*---(directories)-------------*/
   { "d_open"         , "everyone can access directory, then list, read, create, and delete files"        , 00777 , "00777" },
   { "d_group"        , "owner and group can do anything, others can read"                                , 00775 , "00775" },
   { "d_control"      , "owner and group can do anything, everyone else can do nothing"                   , 00770 , "00770" },
   { "d_normal"       , "everyone can access directory, group and other can list and read files only"     , 00755 , "00755" },
   { "d_normals"      , "everyone can access directory, group and other can list and read files only"     , 01755 , "01755" },
   { "d_sgroup"       , "same as d_normal, but new files get directories group id"                        , 02755 , "02755" },
   { "d_secure"       , "owner can do anything,  group can list and read files only, all others nothing"  , 00750 , "00750" },
   { "d_tight"        , "owner can do anything, all others nothing"                                       , 00700 , "00700" },
   { "d_tights"       , "everyone can see, access, and modify, but only owner can delete"                 , 01700 , "01700" },
   /*---(root)--------------------*/
   { "r_share"        , "only root specifically can modify; but all others can see and access"            , 00555 , "00555" },
   { "r_only"         , "only root specifically can sse, access, or modify; even root group can not"      , 00000 , "00000" },
   /*---(regular files)-----------*/
   { "f_open"         , "everyone can read and write"                                                     , 00666 , "00666" },
   { "f_control"      , "owner and group can read and write, everyone else gets nothing"                  , 00660 , "00660" },
   { "f_hidev"        , "owner and group can read and write, everyone else read-only"                     , 00664 , "00664" },
   { "f_usb"          , "owner and group can read and write, everyone else read-only"                     , 00662 , "00662" },
   { "f_normal"       , "owner can read and write, everyone else can read only"                           , 00644 , "00644" },
   { "f_secure"       , "owner can read and write, group can read only, everyone else gets nothing"       , 00640 , "00640" },
   { "f_tty"          , "owner can read and write, group can write only, everyone else gets nothing"      , 00620 , "00620" },
   { "f_tight"        , "owner can read and write, everyone else gets nothing"                            , 00600 , "00600" },
   { "f_exec"         , "owner can do anything, all can only execute within"                              , 00711 , "00711" },
   /*---(end)---------------------*/
   { "end-of-entries" , "---marker---"                                                                    , 00000 , "!"     },
};

char*
RPTG_find_perm          (char a_perms [LEN_TERSE])
{
   int         i           =    0;
   for (i = 0; i < MAX_PERM; ++i) {
      if (g_perms [i].text [0] == '!')  break;
      if (strcmp (a_perms, g_perms [i].text) != 0)   continue;
      return g_perms [i].name;
   }
   return g_perms [i].text;
}



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
RPTG_init           (void)
{
   FILTER_type_all  ();
   FILTER_mime_all  ();
   FILTER_size_all  ();
   FILTER_age_all   ();
   FILTER_ascii_all ();
   RPTG_config_super_off  ();
   RPTG_col_none    ();
   s_days    =   0;
   s_age     = '-';
   s_pos     =  -1;
   s_len     =  -1;
   s_bytes   =   0;
   my.r_uid  =  -1;
   my.r_gid  =  -1;
   strcpy (my.ext, "");
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         suid and guid                        ----===*/
/*====================------------------------------------====================*/
static void      o___SUPER___________________o (void) {;}

char RPTG_config_super_off     (void) { my.super = '-'; return 0; }
char RPTG_config_super_on      (void) { my.super = 'y'; return 0; }

char
RPTG_filter_super         (uchar a_super)
{
   if (my.super != 'y')  return 1;
   if (a_super == '*')   return 1;
   if (a_super == '!')   return 1;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   output column control                      ----===*/
/*====================------------------------------------====================*/
static void      o___COLUMNS_________________o (void) {;}

char RPTG_col_none           (void)
{
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   my.report = REPORT_MATCHES;
   my.layout = LAYOUT_DEFAULT;
   strcpy (my.columns, "··········");
   strcpy (my.delimit, "");
   my.headers = '-';
   my.lineno  = '-';
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_col_singles         (uchar *a_option)
{
   int         x_len       =     0;
   char        x_opt       =   '-';
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen  (a_option) <= 7)  return -2;
   if (strncmp (a_option, "--show", 6)    != 0)  return -3;
   if      (strcmp  (a_option, "--show-ascii") == 0)  x_opt = 'n';
   else if (strcmp  (a_option, "--show-SIZES") == 0)  x_opt = '$';
   else                                               x_opt = a_option [7];
   switch (x_opt) {
   case 'm' :  my.columns [0] = x_opt;         break;
   case 'a' :  my.columns [1] = x_opt;         break;
   case 'A' :  my.columns [1] = x_opt;         break;
   case 's' :  my.columns [2] = x_opt;         break;
   case 'S' :  my.columns [2] = x_opt;         break;
   case '$' :  my.columns [2] = x_opt;         break;
   case 'l' :  my.columns [3] = x_opt;         break;
   case 'n' :  my.columns [4] = x_opt;         break;
   case 'f' :  my.columns [5] = x_opt;         break;
   case 't' :  my.columns [6] = x_opt;         break;
   case 'T' :  my.columns [6] = x_opt;         break;
   case 'p' :  my.columns [7] = x_opt;         break;
   case 'P' :  my.columns [7] = x_opt;         break;
   case 'd' :  my.columns [8] = x_opt;         break;
   case 'D' :  my.columns [8] = x_opt;         break;
   case 'b' :  my.columns [9] = x_opt;         break;
   default  :  return -4;                      break;
   }
   my.layout = LAYOUT_PREFIX;
   return 0;
}

char
RPTG_col_layouts         (uchar *a_option)
{
   int         x_len       =     0;
   char        x_opt       =   '-';
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (x_len < 1)  return -2;
   if (strcmp  (a_option, "--default"   ) == 0)  x_opt = 'n';
   else                                          x_opt = a_option [2];
   RPTG_col_none ();
   my.report = REPORT_MATCHES;
   switch (x_opt) {
   case 'n'  :  my.layout = LAYOUT_DEFAULT;   strcpy (my.columns, "··········");  my.lineno = '-';  my.headers = '-';  strcpy (my.delimit, "");     break;
   case 'm'  :  my.layout = LAYOUT_MIME;      strcpy (my.columns, "m·········");  my.lineno = '-';  my.headers = '-';  strcpy (my.delimit, "");     break;
   case 'd'  :  my.layout = LAYOUT_DETAIL;    strcpy (my.columns, "maSl··tpd·");  my.lineno = 'y';  my.headers = 'y';  strcpy (my.delimit, "");     break;
   case 'a'  :  my.layout = LAYOUT_ANALYSIS;  strcpy (my.columns, "mA$lNfTPDb");  my.lineno = 'y';  my.headers = 'Y';  strcpy (my.delimit, "");     break;
   case 'g'  :  my.layout = LAYOUT_GYGES;     strcpy (my.columns, "mA$lNfTPDb");  my.lineno = '-';  my.headers = '-';  strcpy (my.delimit, "");   break;
   case 'c'  :  my.layout = LAYOUT_COUNT;     strcpy (my.columns, "··········");  my.lineno = '-';  my.headers = '-';  strcpy (my.delimit, "");     break;
   case 's'  :  my.layout = LAYOUT_SILENT;    strcpy (my.columns, "··········");  my.lineno = '-';  my.headers = '-';  strcpy (my.delimit, "");     break;
   default   :  return -3; break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       output types                           ----===*/
/*====================------------------------------------====================*/
static void      o___OUTPUT__________________o (void) {;}

char
RPTG__title             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_PATH]  = "";
   /*---(prepare)------------------------*/
   ystrlcpy (g_print, "", LEN_RECD);
   /*---(quick out)----------------------*/
   if (my.headers != 'Y')            return 0;
   if (my.layout  == LAYOUT_SILENT)  return 0;
   if (my.layout  == LAYOUT_COUNT)   return 0;
   if (my.total   != 0)              return 0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(shared)-------------------------*/
   if (my.run_as != IAM_UHELIOS) {
      printf ("## HELIOS-PHAETON (locate) -- filesystem searching and indexing services\n");
   }
   if (my.path [0] != '\0')         ystrlcpy (t, my.path  , LEN_PATH);
   else                             ystrlcpy (t, "<none>" , LEN_PATH);
   if (my.maxlevel >= MAX_LEVEL)    ystrlcpy (s, "<unset>", LEN_LABEL);
   else                             sprintf (t, "%d"     , my.maxlevel);
   /*---(individual)---------------------*/
   DEBUG_RPTG   yLOG_char    ("my.layout" , my.layout);
   switch (my.layout) {
   case LAYOUT_DEFAULT   :
      ystrlcat (g_print, "##    report option (name only) --default", LEN_RECD);
      break;
   case LAYOUT_PREFIX   :
      ystrlcat (g_print, "##    report option (prefixed)", LEN_RECD);
      IF_MIME    ystrlcat (g_print, " --show-mime"  , LEN_RECD);
      IF_NAGE    ystrlcat (g_print, " --show-age"   , LEN_RECD);
      IF_DAGE    ystrlcat (g_print, " --show-AGE"   , LEN_RECD);
      IF_NSIZE   ystrlcat (g_print, " --show-size"  , LEN_RECD);
      IF_DSIZE   ystrlcat (g_print, " --show-SIZE"  , LEN_RECD);
      IF_MSIZE   ystrlcat (g_print, " --show-SIZES" , LEN_RECD);
      IF_LVL     ystrlcat (g_print, " --show-level" , LEN_RECD);
      IF_ASCII   ystrlcat (g_print, " --show-ascii" , LEN_RECD);
      IF_FIND    ystrlcat (g_print, " --show-find"  , LEN_RECD);
      IF_NTYPE   ystrlcat (g_print, " --show-type"  , LEN_RECD);
      IF_DTYPE   ystrlcat (g_print, " --show-TYPE"  , LEN_RECD);
      IF_NPERM   ystrlcat (g_print, " --show-perms" , LEN_RECD);
      IF_DPERM   ystrlcat (g_print, " --show-PERMS" , LEN_RECD);
      IF_NDRIVE  ystrlcat (g_print, " --show-drive" , LEN_RECD);
      IF_DDRIVE  ystrlcat (g_print, " --show-DRIVE" , LEN_RECD);
      IF_BASE    ystrlcat (g_print, " --show-base"  , LEN_RECD);
      break;
   case LAYOUT_DETAIL   :
      ystrlcat (g_print, "##    report option (favorite) --detail", LEN_RECD);
      break;
   case LAYOUT_ANALYSIS :
      ystrlcat (g_print, "##    report option (kitchen sink) --analysis", LEN_RECD);
      break;
   case LAYOUT_GYGES    :
      ystrlcat (g_print, "##    report option (kitchen sink for gyges) --gyges", LEN_RECD);
      break;
   }
   /*---(output)-------------------------*/
   if (my.run_as != IAM_UHELIOS && strcmp (g_print, "") != 0) {
      printf ("%s\n", g_print);
   }
   if (my.run_as != IAM_UHELIOS) {
      printf ("##    start   %-25.25s  depth   %-20.20s\n", t, s);
      printf ("##    regex   %-25.25s  ascii   %-20.20s\n", my.regex, my.ascii);
      printf ("##    types   %-25.25s  mimes   %-20.20s\n", my.types, my.mimes);
      printf ("##    sizes   %-25.25s  ages    %-20.20s\n", my.sizes, my.ages);
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG__break             (char a_force)
{
   /*---(quick out)----------------------*/
   if (strchr ("Yy", my.headers) == NULL)  return 0;
   if (my.layout  == LAYOUT_SILENT)  return 0;
   if (my.layout  == LAYOUT_COUNT)   return 0;
   if (my.total %  5 != 0 && a_force != 'y')  return 0;
   /*---(shared)-------------------------*/
   if (my.run_as != IAM_UHELIOS)  printf ("\n");
   /*---(complete)-----------------------*/
   return 0;
}

char
RPTG__add_normal        (tENTRY *a_data, char *a_delimit)
{
   return 0;
}

char
RPTG__add_mime          (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        t           [LEN_HUND]  = "";
   if (a_data == NULL) {
      sprintf  (t, "M %s ---ext--- %s ", a_delimit, a_delimit);
   } else {
      sprintf  (t, "%c %s %-9.9s %s "  , a_data->cat, a_delimit, a_data->ext, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_age           (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_TERSE] = "";
   char        t           [LEN_HUND]  = "";
   char        u           [LEN_TERSE] = "";
   if (a_data == NULL) {
      IF_NAGE      sprintf (t, "AGE %s ", a_delimit);
      IF_DAGE      sprintf (t, "AGE %s dys %s c %s --epoch--- %s ", a_delimit, a_delimit, a_delimit, a_delimit);
   } else {
      ystrlager (my.runtime, a_data->changed, 'u', s);
      if      (s_days ==   0)  strcpy  (u, "  ·");
      else if (s_days >= 999)  strcpy  (u, "+++");
      else                     sprintf (u, "%3d", s_days);
      IF_NAGE    sprintf (t, "%-3.3s %s "  , s, a_delimit);
      else       sprintf (t, "%-3.3s %s %3.3s %s %c %s %-10d %s "  , s, a_delimit, u, a_delimit, s_age, a_delimit, a_data->changed, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_size          (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_LABEL] = "";
   char        w           [LEN_SHORT] = "";
   char        x_bytes     [LEN_LABEL] = "";
   char        x_cbytes    [LEN_LABEL] = "";
   if (a_data == NULL) {
      IF_NSIZE     sprintf (t, "SIZE %s ", a_delimit);
      IF_DSIZE     sprintf (t, "SIZE %s mg %s cnts %s csiz %s ", a_delimit, a_delimit, a_delimit, a_delimit);
      IF_MSIZE     sprintf (t, "SIZE %s mg %s ---bytes------- %s cnts %s csiz %s ---bytes------- %s ", a_delimit, a_delimit, a_delimit, a_delimit, a_delimit, a_delimit);
   } else {
      /*---(bytes)-----------------------*/
      ystrlsize   (a_data->bytes, 'u', s);
      ystrl4comma (a_data->bytes, v, 0, ',', '-', 15);
      ystrlpad    (v, x_bytes, '?', '>', 15);
      /*---(cum bytes)-------------------*/
      ystrlcount  (a_data->bcum , 'u', u);
      ystrl4comma (a_data->bcum , v, 0, ',', '-', 15);
      ystrlpad    (v, x_cbytes, '?', '>', 15);
      /*---(cum bytes)-------------------*/
      if      (a_data->ccum ==    1)  {
         strcpy  (w, "   ·");
         strcpy  (u, "   ·");
         strcpy  (x_cbytes, "              ·");
      } else if (a_data->ccum >  9999) {
         strcpy  (w, "++++");
      } else {
         sprintf (w, "%4d", a_data->ccum);
      }
      if (a_data->size == 0) {
         ystrlcpy (v, " -"  , LEN_LABEL);
      } else  {
         sprintf (v, "%2d", a_data->size);
      }
      IF_NSIZE   sprintf (t, "%-4.4s %s "  , s, a_delimit);
      IF_DSIZE   sprintf (t, "%-4.4s %s %-2.2s %s %-4.4s %s %-4.4s %s ", s, a_delimit, v, a_delimit, w, a_delimit, u, a_delimit);
      IF_MSIZE   sprintf (t, "%-4.4s %s %-2.2s %s %-15.15s %s %-4.4s %s %-4.4s %s %-15.15s %s ", s, a_delimit, v, a_delimit, x_bytes, a_delimit, w, a_delimit, u, a_delimit, x_cbytes, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_level         (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_SHORT] = "";
   char        w           [LEN_SHORT] = "";
   if (a_data == NULL) {
      sprintf (t, "LV %s ", a_delimit);
   } else {
      sprintf (t, "%2d %s "      , a_data->lvl, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_ascii         (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_SHORT] = "";
   char        w           [LEN_SHORT] = "";
   if (a_data == NULL) {
      sprintf (t, "A %s len %s ", a_delimit, a_delimit);
   } else {
      sprintf (t, "%c %s %-3d %s "  , a_data->ascii, a_delimit, a_data->len, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_find          (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        t           [LEN_RECD]  = "";
   char        a           [LEN_TERSE] = "";
   char        b           [LEN_TERSE] = "";
   if (a_data == NULL) {
      sprintf (t, "POS %s len %s ", a_delimit, a_delimit);
   } else {
      if (s_pos == -1)   strcpy  (a, "·  ");
      else               sprintf (a, "%3d", s_pos);
      if (s_len == -1)   strcpy  (b, "·  ");
      else               sprintf (b, "%3d", s_len);
      sprintf (t, "%-3.3s %s %-3.3s %s ", a, a_delimit, b, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_type          (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        t           [LEN_RECD]  = "";
   char        a           =  '-';
   char        b           =  '-';
   if (a_data == NULL) {
      IF_NTYPE     sprintf (t, "S %s ", a_delimit);
      IF_DTYPE     sprintf (t, "S %s t %s ", a_delimit, a_delimit);
   } else {
      if (a_data->stype == '-')  a = '·';
      else                       a = a_data->stype;
      if (a_data->type  == '-')  b = '·';
      else                       b = a_data->type;
      IF_NTYPE {
         sprintf (t, "%c %s "   , a, a_delimit);
      } else  {
         sprintf (t, "%c %s %c %s "   , a, a_delimit, b, a_delimit);
      }
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_perm          (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_SHORT] = "";
   char        w           [LEN_SHORT] = "";
   char        x_own, x_grp;
   char        x_perm      [LEN_TERSE] = "";
   if (a_data == NULL) {
      IF_NPERM     sprintf (t, "--PERM--- %s o %s g %s ", a_delimit, a_delimit, a_delimit);
      IF_DPERM     sprintf (t, "--PERM--- %s octal %s o %s own# %s g %s grp# %s s %s ", a_delimit, a_delimit, a_delimit, a_delimit, a_delimit, a_delimit, a_delimit);
   } else {
      sprintf (u, "00%d%d%d", a_data->own, a_data->grp, a_data->oth);
      ystrlcpy (s, RPTG_find_perm (u), LEN_TERSE);
      switch (a_data->uid) {
      case 0    : x_own = 'r';  break;
      case 1000 : x_own = 'm';  break;
      default   : x_own = '-';  break;
      }
      switch (a_data->gid) {
      case 0    : x_grp = 'r';  break;
      case 100  : x_grp = 'u';  break;
      case 1000 : x_grp = 'm';  break;
      default   : x_grp = '-';  break;
      }
      if (a_data->stype == '>') {
         x_own = x_grp = '·';
         strcpy (s, "·");

      }
      IF_NPERM   sprintf (t, "%-9.9s %s %c %s %c %s ", s, a_delimit, x_own, a_delimit, x_grp, a_delimit);
      else       sprintf (t, "%-9.9s %s %-5.5s %s %c %s %-4d %s %c %s %-4d %s %c %s "   , s, a_delimit, u, a_delimit, x_own, a_delimit, a_data->uid, a_delimit, x_grp, a_delimit, a_data->gid, a_delimit, a_data->super, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_drive         (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_SHORT] = "";
   char        w           [LEN_SHORT] = "";
   char        a           [LEN_TERSE] = "";
   if (a_data == NULL) {
      IF_NDRIVE   sprintf (t, "DR %s ", a_delimit);
      IF_DDRIVE   sprintf (t, "DR %s -inode-- %s -dnode-- %s ", a_delimit, a_delimit, a_delimit);
   } else {
      if (a_data->drive == 0)  strcpy  (a, "· ");
      else                     sprintf (a, "%2d", a_data->drive);
      IF_NDRIVE   sprintf (t, "%-2.2s %s ", a, a_delimit);
      IF_DDRIVE   sprintf (t, "%-2.2s %s %8d %s %8d %s ", a, a_delimit, a_data->inode, a_delimit, a_data->dnode, a_delimit);
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__add_base          (char a_sub, tENTRY *a_data, char *a_delimit)
{
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_SHORT] = "";
   char        w           [LEN_SHORT] = "";
   if (a_data == NULL) {
      sprintf (t, "---base-name---------------------------- %s ", a_delimit);
   } else {
      sprintf (t, "%-40.40s %s "   , a_data->name, a_delimit);
      if (a_data->len > 39)  t [39] = '>';
   }
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}

char
RPTG__style_detail      (tENTRY *a_data, char *a_full)
{
   char        t           [LEN_RECD]  = "";
   sprintf (t, "%c  %c  %d  %d  %c  %c  %c  ", a_data->cat, s_age, a_data->size, a_data->lvl, a_data->ascii, a_data->type, a_data->stype);
   ystrlcat (g_print, t, LEN_RECD);
   sprintf (t, "%s", a_full);
   ystrlcat (g_print, t, LEN_RECD);
   return 0;
}


char
RPTG__columns           (char a_force)
{
   char        t           [LEN_RECD]  = "";
   /*---(prepare)------------------------*/
   ystrlcpy (g_print, "", LEN_RECD);
   /*---(quick out)----------------------*/
   if (strchr ("Yy", my.headers) == NULL)  return 0;
   if (my.layout  == LAYOUT_SILENT)  return 0;
   if (my.layout  == LAYOUT_COUNT)   return 0;
   if (my.total % 25 != 0 && a_force != 'y')  return 0;
   /*---(shared)-------------------------*/
   if (my.run_as != IAM_UHELIOS)   printf ("\n");
   ystrlcat (g_print, "## ", LEN_RECD);
   if (my.lineno == 'y') {
      sprintf (t, "line--- %s ", my.delimit);
      ystrlcat (g_print, t, LEN_RECD);
   }
   /*---(individual)---------------------*/
   IF_MIME      RPTG__add_mime  (my.columns [0], NULL, my.delimit);
   IF_AGE       RPTG__add_age   (my.columns [1], NULL, my.delimit);
   IF_SIZE      RPTG__add_size  (my.columns [2], NULL, my.delimit);
   IF_LVL       RPTG__add_level (my.columns [3], NULL, my.delimit);
   IF_FIND      RPTG__add_find  (my.columns [5], NULL, my.delimit);
   IF_TYPE      RPTG__add_type  (my.columns [6], NULL, my.delimit);
   IF_PERM      RPTG__add_perm  (my.columns [7], NULL, my.delimit);
   IF_DRIVE     RPTG__add_drive (my.columns [8], NULL, my.delimit);
   IF_ASCII     RPTG__add_ascii (my.columns [4], NULL, my.delimit);
   IF_BASE      RPTG__add_base  (my.columns [9], NULL, my.delimit);
   ystrlcat (g_print, "---fully qualified file---------------------------", LEN_RECD);
   /*---(output)-------------------------*/
   if (strcmp (g_print, "") != 0) {
      if (my.run_as != IAM_UHELIOS)   printf ("%s\n", g_print);
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG__line        (tENTRY *a_data, char *a_full)
{
   /*---(local variables)--+-----+-----+-*/
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_SHORT] = "";
   char        w           [LEN_SHORT] = "";
   uchar       x_mag       =  '·';
   long        x_size      =    0;
   int         i           =    0;
   char        x_own, x_grp;
   char        x_perm      [LEN_TERSE] = "";
   /*---(prepare)------------------------*/
   ystrlcpy (g_print, "", LEN_RECD);
   /*---(line number)--------------------*/
   if (strchr ("Yy", my.headers) != NULL) {
      ystrlcat (g_print, "   ", LEN_RECD);
   }
   /*---(line number)--------------------*/
   if (my.lineno == 'y')  {
      ystrl4comma         (my.total, s, 0, ',', '-', 8);
      sprintf (t, "%-7.7s %s ", s, my.delimit);
      ystrlcat (g_print, t, LEN_RECD);
   }
   /*---(create lines)-------------------*/
   IF_MIME    RPTG__add_mime   (my.columns [0], a_data, my.delimit);
   IF_AGE     RPTG__add_age    (my.columns [1], a_data, my.delimit);
   IF_SIZE    RPTG__add_size   (my.columns [2], a_data, my.delimit);
   IF_LVL     RPTG__add_level  (my.columns [3], a_data, my.delimit);
   IF_FIND    RPTG__add_find   (my.columns [5], a_data, my.delimit);
   IF_TYPE    RPTG__add_type   (my.columns [6], a_data, my.delimit);
   IF_PERM    RPTG__add_perm   (my.columns [7], a_data, my.delimit);
   IF_DRIVE   RPTG__add_drive  (my.columns [8], a_data, my.delimit);
   IF_ASCII   RPTG__add_ascii  (my.columns [4], a_data, my.delimit);
   IF_BASE    RPTG__add_base   (my.columns [9], a_data, my.delimit);
   /*---(add full)-----------------------*/
   if (strchr (LAYOUT_QUIETS, my.layout) == NULL) {
      sprintf (t, "%s %s", a_full, my.delimit);
      ystrlcat (g_print, t, LEN_RECD);
   }
   /*---(oupput)-------------------------*/
   if (strcmp (g_print, "") != 0) {
      if (my.run_as != IAM_UHELIOS)  printf ("%s\n", g_print);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
RPTG_driver             (tENTRY *a_data, char *a_full)
{
   /*---(title)--------------------------*/
   if (my.total == 0) {
      RPTG__title   ();
      RPTG__columns ('-');
      RPTG__break   ('-');
   }
   /*---(individual headers)-------------*/
   else if (my.total % 25 == 0) {
      RPTG__columns ('-');
      RPTG__break   ('-');
   }
   /*---(small group breaks)-------------*/
   else if (my.total %  5 == 0) {
      RPTG__break   ('-');
   }
   /*---(output line)--------------------*/
   RPTG__line (a_data, a_full);
   /*---(complete)-----------------------*/
   return 0;
}

char
RPTG_footer             (void)
{
   char        s           [LEN_LABEL];
   char        t           [LEN_LABEL];
   /*---(quick out)----------------------*/
   if (strchr ("Yy", my.headers) == NULL)  return 0;
   if (my.layout  == LAYOUT_SILENT)  return 0;
   if (my.layout  == LAYOUT_COUNT)   return 0;
   /*---(columns)------------------------*/
   if (my.total % 25 >= 5)  RPTG__columns ('y');
   /*---(shared)-------------------------*/
   if (my.headers == 'Y') {
      DRIVE_list ();
   }
   if (my.run_as != IAM_UHELIOS)   printf ("\n");
   DB_commas (my.total, s);
   /*> DB_commas (s_bytes, t);                                                      <*/
   ystrlsize (s_bytes, 'u', t);
   if (my.run_as != IAM_UHELIOS)   printf ("##  %s total entries matched, taking up %s\n", s, t);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        walker function                       ----===*/
/*====================------------------------------------====================*/
static void      o___WALKER__________________o (void) {;}

char
RPTG__callback    (char a_serious, tENTRY *a_data, char *a_full)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_complex ("callback"  , "%p, %p", a_data, a_full);
   /*---(defense)------------------------*/
   --rce;  if (a_data == NULL) {
      return rce;
   }
   --rce;  if (a_full == NULL) {
      return rce;
   }
   /*---(permissions)--------------------*/
   rc = FILTER_by_perms   (a_data->uid, a_data->gid, a_data->own, a_data->grp, a_data->oth, NULL);
   DEBUG_DATA  yLOG_complex  ("perms"     , "%d, %4d %4d, %d %4d %4d, %d, %d", a_data->own, a_data->uid, my.uid, a_data->grp, a_data->gid, my.gid, a_data->oth, rc);
   if (rc <= 0)  return 0;
   /*---(regex)--------------------------*/
   if (my.regex_len != 0) {
      rc = FILTER_by_regex (a_data->name);
      DEBUG_DATA  yLOG_complex  ("regex"     , "%s, %d", a_data->name, rc);
      if (rc <= 0)  return 0;
   } else {
      DEBUG_DATA  yLOG_note     ("no regex, continue filtering");
   }
   /*---(other filters)------------------*/
   rc = FILTER_by_type    (a_data->type, a_data->stype);
   DEBUG_DATA  yLOG_complex  ("type"      , "%s, %c, %d", my.types, a_data->type , rc);
   if (rc <= 0)  return 0;
   rc = FILTER_by_mime    (a_data->cat, a_data->ext);
   DEBUG_DATA  yLOG_complex  ("mime"      , "%s, %s, %c, %s, %d", my.mimes, my.ext, a_data->cat, a_data->ext, rc);
   if (rc <= 0)  return 0;
   rc = FILTER_by_size    (a_data->size);
   DEBUG_DATA  yLOG_complex  ("mime"      , "%s, %d, %d", my.sizes, a_data->size , rc);
   if (rc <= 0)  return 0;
   s_days = (my.runtime - a_data->changed) / (24 * 60 * 60);
   rc = FILTER_by_age     (s_days);
   DEBUG_DATA  yLOG_complex  ("age"       , "%s, %d, %d", my.ages , s_days       , rc);
   if (rc <= 0)  return 0;
   rc = FILTER_by_ascii   (a_data->ascii);
   DEBUG_DATA  yLOG_complex  ("ascii"     , "%s, %c, %d", my.ascii, a_data->ascii, rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_super (a_data->super);
   DEBUG_DATA  yLOG_complex  ("super"     , "%c, %c, %d", my.super, a_data->super, rc);
   if (rc <= 0)  return 0;
   /*---(serious)------------------------*/
   DEBUG_DATA  yLOG_char     ("a_serious" , a_serious);
   if (a_serious == 'y') {
      g_found = a_data;
      ystrlcpy (g_path, a_full, LEN_RECD);
   }
   /*---(report out)---------------------*/
   if (my.number >= 0) {
      if (my.total == my.number)    RPTG_driver (a_data, a_full);
   } else if (my.total < my.limit) {
      RPTG_driver (a_data, a_full);
   }
   /*---(update total)-------------------*/
   ++my.total;
   s_bytes  += a_data->bytes;
   if (a_data->count > 0)  MIME_add_found (a_data->ext, a_data->bytes);
   /*---(complete)-----------------------*/
   return 1;
}

char
RPTG_walker       (char a_trigger)
{
   char        rc          =    0;
   MIME_reset_found ();
   rc = ENTRY_walk (a_trigger, RPTG__callback);
   if (my.layout == LAYOUT_COUNT)  printf ("%d\n", my.total);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       search driver                          ----===*/
/*====================------------------------------------====================*/
static void      o___SEARCH__________________o (void) {;}

char
RPTG__dir_header        (void)
{
   yPARSE_spacer (1);
   yPARSE_printf ("o_format"    , "O"     , "dirtree"            );
   yPARSE_printf ("o_label0"    , "O"     , "total bytes"        );
   yPARSE_spacer (1);
   return 0;
}

char
RPTG__dir_callback    (char a_serious, tENTRY *a_data, char *a_full)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   static int  c           =    0;
   char        t           [LEN_HUND]  = "";
   llong       x_adj       =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_complex ("callback"  , "%p, %p", a_data, a_full);
   /*---(defense)------------------------*/
   --rce;  if (a_data == NULL) {
      return rce;
   }
   --rce;  if (a_full == NULL) {
      return rce;
   }
   /*---(permissions)--------------------*/
   if (a_data->type != ENTRY_DIR) {
      DEBUG_DATA  yLOG_note     ("not a directory, skip");
      return 0;
   }
   /*---(report out)---------------------*/
   /*> if (strcmp (a_data->name, "/"      ) == 0 && my.dir_all != 'y') x_adj = my.empty;   <*/
   /*> if (strcmp (a_data->name, "(empty)") == 0 && my.dir_all != 'y') return 1;      <*/
   ystrlcpy (t, a_data->name, LEN_HUND);
   ystrldchg (t, '.', 0, LEN_HUND);
   yPARSE_vprintf (c, "node", a_data->lvl, t, a_full, a_data->bcum, a_data->ccum, 0, 0, 0, 0);
   ++c;
   /*---(complete)-----------------------*/
   return 1;
}

char
RPTG_dirtree      (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   int         i           =    0;
   int         n           =    0;
   char        x_save      = MIME_TOTAL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(header)-------------------------*/
   yPARSE_planned (NULL, "stdout", NULL);
   yPARSE_header  (P_FULLPATH, P_VERNUM, P_VERTXT, P_ONELINE, "filesystem directory heirarchy", RPTG__dir_header);
   yPARSE_handler_plus ("node", "sLDhlllll", 0.0, NULL, NULL, "lvl,name,full,size,cnt,-,-,-,-,-");
   /*---(process all)--------------------*/
   rc = ENTRY_walk (WALK_ALL, RPTG__dir_callback);
   /*---(footer)-------------------------*/
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;


   return rc;
}

/*> char         /+===[[ output the dir tree ]]===============[ ------ [ ------ ]=+/             <* 
 *> RPTG_dirtree       (                                                                         <* 
 *>       /+---(formal parameters)+-------------+---------------------------------+/             <* 
 *>       int         a_level     ,             /+ depth of search                +/             <* 
 *>       tPTRS      *a_ptrs      ,             /+ ptrs of directory to scan      +/             <* 
 *>       char       *a_path      )             /+ full path of directory to scan +/             <* 
 *> {  /+---(local variables)--+-----------+-+/                                                  <* 
 *>    char        rce         = -10;           /+ return code for errors         +/             <* 
 *>    tENTRY      *x_dir       = NULL;          /+ directory data                 +/            <* 
 *>    tPTRS      *x_ptrs      = NULL;          /+ current entry                  +/             <* 
 *>    tENTRY      *x_data      = NULL;          /+ current entry data             +/            <* 
 *>    char        rc          = 0;                                                              <* 
 *>    char        x_path      [500];                                                            <* 
 *>    int         i           = 0;                                                              <* 
 *>    char        x_prefix    [100] = "";                                                       <* 
 *>    char        x_temp      [200] = "";                                                       <* 
 *>    char        x_cum       [20]  = "";                                                       <* 
 *>    /+---(header)-------------------------+/                                                  <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                                 <* 
 *>    DEBUG_GRAF   yLOG_value   ("a_level"   , a_level);                                        <* 
 *>    DEBUG_GRAF   yLOG_point   ("a_ptrs"    , a_ptrs);                                         <* 
 *>    DEBUG_GRAF   yLOG_info    ("a_path"    , a_path);                                         <* 
 *>    /+---(defense)------------------------+/                                                  <* 
 *>    --rce;  if (a_level >  my.maxlevel) {                                                     <* 
 *>       DEBUG_GRAF   yLOG_note    ("max recursion reached, return");                           <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                              <* 
 *>       return 0;                                                                              <* 
 *>    }                                                                                         <* 
 *>    --rce;  if (a_ptrs == NULL) {                                                             <* 
 *>       DEBUG_GRAF   yLOG_note    ("directory pointer NULL, failed");                          <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                              <* 
 *>       return rce;                                                                            <* 
 *>    }                                                                                         <* 
 *>    DEBUG_GRAF   yLOG_point   ("dir_data"  , a_ptrs->data);                                   <* 
 *>    --rce;  if (a_ptrs->data == NULL) {                                                       <* 
 *>       DEBUG_GRAF   yLOG_note    ("directory data payload NULL, failed");                     <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                              <* 
 *>       return rce;                                                                            <* 
 *>    }                                                                                         <* 
 *>    x_dir = a_ptrs->data;                                                                     <* 
 *>    DEBUG_GRAF   yLOG_point   ("dir_name"  , x_dir->name);                                    <* 
 *>    --rce;  if (x_dir->name == NULL) {                                                        <* 
 *>       DEBUG_GRAF   yLOG_note    ("directory name NULL, failed");                             <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                              <* 
 *>       return rce;                                                                            <* 
 *>    }                                                                                         <* 
 *>    DEBUG_GRAF   yLOG_info    ("dir_name"  , x_dir->name);                                    <* 
 *>    /+---(header)-------------------------+/                                                  <* 
 *>    if (a_level == 1) {                                                                       <* 
 *>       printf ("#!/usr/local/bin/hyleoroi\n");                                                <* 
 *>       printf ("#   hyleoroi -- tree visualization input file\n");                            <* 
 *>       printf ("\n\n\n");                                                                     <* 
 *>       printf ("SIMPLE\n");                                                                   <* 
 *>       printf ("\n\n\n");                                                                     <* 
 *>       printf ("#--context  ---values------------------------------- \n");                  <* 
 *>       printf ("source      heilos-phaeton                           \n");                  <* 
 *>       printf ("label       full directory tree for 2017-05-21       \n");                  <* 
 *>       printf ("format      dirtree                                  \n");                  <* 
 *>       printf ("\n\n\n");                                                                     <* 
 *>    }                                                                                         <* 
 *>    /+---(prefix)-------------------------+/                                                  <* 
 *>    for (i = 1; i < a_level; ++i)  strcat (x_prefix, "   ");                                  <* 
 *>    /+---(name directory)--------------+/                                                     <* 
 *>    if      (a_level == 1)  sprintf (x_path, "%s"   , x_dir->name);                           <* 
 *>    else if (a_level == 2)  sprintf (x_path, "/%s"  , x_dir->name);                           <* 
 *>    else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);                   <* 
 *>    DEBUG_GRAF   yLOG_info    ("x_path"    , x_path);                                         <* 
 *>    /+> OPT_VERBOSE  printf ("%s\n", x_path);                                          <+/    <* 
 *>    /+> printf ("%s%s\n", x_prefix, x_path);                                           <+/    <* 
 *>    sprintf (x_temp, "%s%s", x_prefix, x_dir->name);                                          <* 
 *>    /+> DB_commas (x_dir->bcum, x_cum);                                               <+/   <* 
*>    /+> printf  ("%-100.100s %14s\n", x_temp, x_cum);                                  <+/    <* 
*>    printf  ("%-100.100s | %14ld\n", x_temp, x_dir->bcum);                                    <* 
*>    /+---(check start path)---------------+/                                                  <* 
*>    DEBUG_GRAF   yLOG_value   ("my.level"  , my.level);                                       <* 
*>    if (a_level <= my.level) {                                                                <* 
   *>       DEBUG_GRAF   yLOG_note    ("using --start path");                                      <* 
      *>       x_ptrs = root_stack [a_level];                                                         <* 
      *>       x_data = x_ptrs->data;                                                                 <* 
      *>       DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);                                <* 
      *>       RPTG_dirtree (a_level + 1, x_ptrs, x_path);                                            <* 
      *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                              <* 
      *>       return 0;                                                                              <* 
      *>    }                                                                                         <* 
      *>    /+---(spin through entries)-----------+/                                                  <* 
      *>    x_ptrs = a_ptrs->c_head;                                                                  <* 
      *>    while (x_ptrs != NULL) {                                                                  <* 
         *>       x_data = x_ptrs->data;                                                                 <* 
            *>       if (x_data != NULL) {                                                                  <* 
               *>          DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);                             <* 
                  *>          if (x_data->type == ENTRY_DIR) {                                                    <* 
                     *>             RPTG_dirtree (a_level + 1, x_ptrs, x_path);                                      <* 
                        *>          }                                                                                   <* 
                        *>       }                                                                                      <* 
                        *>       x_ptrs = x_ptrs->s_next;                                                               <* 
                        *>    }                                                                                         <* 
                        *>    /+---(complete)-----------------------+/                                                  <* 
                        *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                                 <* 
                        *>    return 0;                                                                                 <* 
                        *> }                                                                                            <*/

char
RPTG_summ          (void)
{
   /*---(local variables)--+-----------+-*/
   long        x_dseen     = 0;
   long        x_dkept     = 0;
   long        x_fseen     = 0;
   long        x_fkept     = 0;
   char        x_comma     [20] = "";
   int         i           = 0;
   struct      tm         *curr  = NULL;;
   time_t      x_written   = 0;
   char        x_time      [100] = "";
   tDRIVE     *x_drive     = NULL;
   /*---(process all)--------------------*/
   for (i = 1; i < g_nmime; ++i) {
      if (g_mime [i].cat == MIME_DIR && strcmp (g_mime [i].ext, "dir") == 0) {
         x_dseen  += g_mime [i].n_seen;
         x_dkept  += g_mime [i].n_kept;
      } else {
         x_fseen  += g_mime [i].n_seen;
         x_fkept  += g_mime [i].n_kept;
      }
   }
   /*---(output)-------------------------*/
   if (my.run_as != IAM_UHELIOS) {
      printf ("database %s:\n", my.file_data);
      printf ("\n");
      printf ("ref-   ---host--- --serial-- ---device------ ---mount-point------ --type-- ---bytes------------   ---written----------\n");
      x_drive = h_drive;
      while (x_drive != NULL) {
         x_written = x_drive->written;
         curr = localtime (&x_written);
         if (curr == NULL) printf ("local time failed\n");
         else              strftime (x_time, 100, "%y.%m.%d %H:%M:%S %U", curr);
         DB_commas (x_drive->size , x_comma);
         printf ("[%02d]   %-10.10s %-10.10s %-15.15s %-20.20s %-8.8s %20.20s   %s\n",
               x_drive->ref   , x_drive->host  ,
               x_drive->serial, x_drive->device, x_drive->mpoint,
               x_drive->type  , x_comma        , x_time);
         x_drive = x_drive->m_next;
      }
      printf ("\n");
      printf ("---type----- ---seen----- ---kept----- ---each----- ---bytes----\n");
      printf ("directories ");
      DB_commas (x_dseen, x_comma);
      printf (" %12.12s");
      DB_commas (x_dkept, x_comma);
      printf (" %12.12s");
      DB_commas (sizeof (tENTRY), x_comma);
      printf (" %12.12s");
      DB_commas (x_dkept * sizeof (tENTRY), x_comma);
      printf (" %12.12s\n");
      printf ("files       ");
      DB_commas (x_fseen, x_comma);
      printf (" %12.12s");
      DB_commas (x_fkept, x_comma);
      printf (" %12.12s");
      DB_commas (sizeof (tENTRY), x_comma);
      printf (" %12.12s");
      DB_commas (x_fkept * sizeof (tENTRY), x_comma);
      printf (" %12.12s\n");
      printf ("             ------------ ------------              ------------\n");
      printf ("   total    ");
      DB_commas (x_dseen + x_fseen, x_comma);
      printf (" %12.12s");
      DB_commas (x_dkept + x_fkept, x_comma);
      printf (" %12.12s");
      printf ("             ");
      DB_commas ((x_dkept + x_fkept) * sizeof (tENTRY), x_comma);
      printf (" %12.12s\n");
      printf ("                                                    ------------\n");
      printf ("                 with device entries (approximate) ");
      DB_commas (sizeof (short) + (g_ndrive * sizeof (tDRIVE)) + ((x_dkept + x_fkept) * sizeof (tENTRY)), x_comma);
      printf (" %12.12s\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char
RPTG__create_file       (char a_how, char *a_src, char *a_dst, int a_days, int a_perms, char *a_own, char *a_grp)
{
   long        x_off       =    0;
   int         x_day       =    0;
   time_t      x_new       =    0;
   char        x_cmd       [LEN_RECD]  = "";
   tTIME      *x_broke     = NULL;
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(create file)--------------------*/
   switch (a_how) {
   case 't'  :
      sprintf (x_cmd, "touch \"%s\" > /dev/null", a_dst);
      break;
   case 'c'  :
      sprintf (x_cmd, "cp -f \"%s\" \"%s\" > /dev/null", a_src, a_dst);
      break;
   case 'm'  :
      sprintf (x_cmd, "mknod \"%s\" c 4 2 > /dev/null", a_dst);
      break;
   case 'd'  :
      sprintf (x_cmd, "mkdir \"%s\" > /dev/null", a_dst);
      break;
   }
   DEBUG_RPTG   yLOG_info    ("create"    , x_cmd);
   system (x_cmd);
   /*---(update ownership)---------------*/
   sprintf (x_cmd, "chown %s:%s \"%s\" > /dev/null", a_own, a_grp, a_dst);
   DEBUG_RPTG   yLOG_info    ("ownership" , x_cmd);
   system (x_cmd);
   /*---(update permissions)-------------*/
   sprintf (x_cmd, "chmod %o \"%s\" > /dev/null", a_perms, a_dst);
   DEBUG_RPTG   yLOG_info    ("perms"     , x_cmd);
   system (x_cmd);
   /*---(modification time)--------------*/
   x_off = a_days * 60 * 60 * 24;
   DEBUG_RPTG   yLOG_value   ("x_off"     , x_off);
   x_new = my.runtime - x_off;
   DEBUG_RPTG   yLOG_value   ("x_new"     , x_new);
   x_broke = localtime (&x_new);
   x_day = x_broke->tm_mday;
   if (x_day > 28)  x_day = 28;
   sprintf (x_cmd, "touch -c -t %04d%02d%02d%02d%02d \"%s\"",
         x_broke->tm_year + 1900, x_broke->tm_mon + 1,
         x_day           , x_broke->tm_hour,
         x_broke->tm_min , a_dst);
   DEBUG_RPTG   yLOG_info    ("x_cmd"     , x_cmd);
   system (x_cmd);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG__change_modtime    (char *a_file, int a_days)
{
   /*> x_days = (my.runtime - a_data->changed) / (365 * 24 * 60);                     <*/
   long        x_off       =    0;
   int         x_day       =    0;
   time_t      x_new       =    0;
   char        x_cmd       [LEN_RECD]  = "";
   tTIME      *x_broke     = NULL;
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   DEBUG_RPTG   yLOG_info    ("a_file"    , a_file);
   DEBUG_RPTG   yLOG_value   ("a_days"    , a_days);
   DEBUG_RPTG   yLOG_value   ("runtime"   , my.runtime);
   x_off = a_days * 60 * 60 * 24;
   DEBUG_RPTG   yLOG_value   ("x_off"     , x_off);
   x_new = my.runtime - x_off;
   DEBUG_RPTG   yLOG_value   ("x_new"     , x_new);
   x_broke = localtime (&x_new);
   x_day = x_broke->tm_mday;
   if (x_day > 28)  x_day = 28;
   sprintf (x_cmd, "touch -c -t %04d%02d%02d%02d%02d %s",
         x_broke->tm_year + 1900, x_broke->tm_mon + 1,
         x_day           , x_broke->tm_hour,
         x_broke->tm_min , a_file);
   DEBUG_RPTG   yLOG_info    ("x_cmd"     , x_cmd);
   system (x_cmd);
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
RPTG__unit              (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   short       c           =    0;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_DESC]  = "";
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "RPTG             : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "types"         ) == 0) {
      ystrlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.types); ++i) {
         sprintf (s, " %c", my.types [i]);
         ystrlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG types       :%s", t);
   }
   else if (strcmp (a_question, "mimes"         ) == 0) {
      ystrlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.mimes); ++i) {
         sprintf (s, " %c", my.mimes [i]);
         ystrlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG mimes       :%s", t);
   }
   else if (strcmp (a_question, "sizes"         ) == 0) {
      ystrlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.sizes); ++i) {
         sprintf (s, " %c", my.sizes [i]);
         ystrlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG sizes       :%s", t);
   }
   else if (strcmp (a_question, "ages"          ) == 0) {
      ystrlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.ages); ++i) {
         sprintf (s, " %c", my.ages [i]);
         ystrlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG ages        :%s", t);
   }
   else if (strcmp (a_question, "ascii"         ) == 0) {
      ystrlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.ascii); ++i) {
         sprintf (s, " %c", my.ascii [i]);
         ystrlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG ascii       :%s", t);
   }
   else if (strcmp (a_question, "output"        ) == 0) {
      sprintf (t, " %c   ", my.layout);
      for (i = 0; i < strlen (my.columns); ++i) {
         sprintf (s, " %c", my.columns [i]);
         ystrlcat (t, s, LEN_DESC);
      }
      sprintf (s, "   %c", my.lineno);
      ystrlcat (t, s, LEN_DESC);
      sprintf (s, "   %c", my.headers);
      ystrlcat (t, s, LEN_DESC);
      if      (strcmp (my.delimit, ""  ) == 0)  sprintf (s, "   ·");
      else if (strcmp (my.delimit, "") == 0)  sprintf (s, "   §");
      else                                      sprintf (s, "   ?");
      ystrlcat (t, s, LEN_DESC);
      snprintf (unit_answer, LEN_FULL, "RPTG output      :%s", t);
   }
   else if (strcmp (a_question, "walk"          ) == 0) {
      g_target = n;
      rc = ENTRY_walk ('#', RPTG__callback);
      if (rc < 1)   snprintf (unit_answer, LEN_FULL, "RPTG walk   (%2d) : no such entry", n);
      else          snprintf (unit_answer, LEN_FULL, "RPTG walk   (%2d) : %-2d  %-2d %2d[%-20.20s]   %c %c   %c %-6.6s   [%.40s]", n,
            g_found->drive, g_found->lvl  ,
            g_found->len  , g_found->name , 
            g_found->type , g_found->stype,
            g_found->cat  , g_found->ext  ,
            g_path);
   }
   else if (strcmp (a_question, "g_print"        ) == 0) {
      ystrlencode (g_print, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_RECD, "RPTG g_print     : [%s]", g_print);
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}





/*===============================[[ end-code ]]===============================*/
