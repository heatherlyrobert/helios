/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */





/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
RPTG_init           (void)
{
   RPTG_config_types_all  ();
   RPTG_config_mimes_all  ();
   RPTG_config_sizes_all  ();
   RPTG_config_ages_all   ();
   RPTG_config_ascii_all  ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     type filter control                      ----===*/
/*====================------------------------------------====================*/
static void      o___TYPE____________________o (void) {;}


char RPTG_config_types_all   (void) { strlcpy (my.types, ENTRY_ALL , LEN_LABEL); return 0; }
char RPTG_config_types_none  (void) { strlcpy (my.types, "········", LEN_LABEL); return 0; }

char
RPTG_config_types_add   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.types, ENTRY_ALL) == 0)  RPTG_config_types_none ();
   switch (a_option [2]) {
   case 'r'        :  my.types [0] = ENTRY_REG;     break;
   case ENTRY_DIR  :  my.types [1] = ENTRY_DIR;     break;
   case ENTRY_CDEV :  my.types [2] = ENTRY_CDEV;    break;
   case ENTRY_BDEV :  my.types [3] = ENTRY_BDEV;    break;
   case ENTRY_FIFO :  my.types [4] = ENTRY_FIFO;    break;
   case ENTRY_SOCK :  my.types [5] = ENTRY_SOCK;    break;
   case ENTRY_LINK :  my.types [6] = ENTRY_LINK;    break;
   case 'u'        :  my.types [7] = ENTRY_HUH;     break;
   }
   return 0;
}

char
RPTG_config_types_set   (uchar *a_mimes)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_mimes == NULL)  return -1;
   x_len = strlen (a_mimes);
   if (strlen (a_mimes) < 1)  return -2;
   RPTG_config_types_none ();
   for (i = 0; i < strlen (a_mimes); ++i) {
      sprintf (t, "--%c", a_mimes [i]);
      RPTG_config_types_add (t);
   }
   return 0;
}

char
RPTG_filter_type        (uchar a_type)
{
   switch (a_type) {
   case ENTRY_REG    :  if (my.types [0] == a_type)  return 1;    break;
   case ENTRY_DIR    :  if (my.types [1] == a_type)  return 1;    break;
   case ENTRY_CDEV   :  if (my.types [2] == a_type)  return 1;    break;
   case ENTRY_BDEV   :  if (my.types [3] == a_type)  return 1;    break;
   case ENTRY_FIFO   :  if (my.types [4] == a_type)  return 1;    break;
   case ENTRY_SOCK   :  if (my.types [5] == a_type)  return 1;    break;
   case ENTRY_LINK   :  if (my.types [6] == a_type)  return 1;    break;
   case ENTRY_HUH    :  if (my.types [7] == a_type)  return 1;    break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     mime filter control                      ----===*/
/*====================------------------------------------====================*/
static void      o___MIME____________________o (void) {;}

char RPTG_config_mimes_all   (void) { strlcpy (my.mimes, MIME_ALL       , LEN_LABEL); return 0; }
char RPTG_config_mimes_none  (void) { strlcpy (my.mimes, "·············", LEN_LABEL); return 0; }

char
RPTG_config_mimes_add   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.mimes, MIME_ALL)  == 0)  RPTG_config_mimes_none ();
   switch (a_option [2]) {
   case MIME_AUDIO  :  my.mimes [0]  = MIME_AUDIO;  break;
   case MIME_VIDEO  :  my.mimes [1]  = MIME_VIDEO;  break;
   case MIME_IMAGE  :  my.mimes [2]  = MIME_IMAGE;  break;
   case MIME_CODE   :  my.mimes [3]  = MIME_CODE;   break;
   case MIME_ASCII  :  my.mimes [4]  = MIME_ASCII;  break;
   case MIME_DBASE  :  my.mimes [5]  = MIME_DBASE;  break;
   case MIME_CRYPT  :  my.mimes [6]  = MIME_CRYPT;  break;
   case MIME_PROP   :  my.mimes [7]  = MIME_PROP;   break;
   case MIME_EXEC   :  my.mimes [8]  = MIME_EXEC;   break;
   case MIME_DIR    :  my.mimes [9]  = MIME_DIR;    break;
   case '~'         :  my.mimes [10] = MIME_TEMP;   break;
   case 'o'         :  my.mimes [11] = MIME_OTHER;  break;
   case 'h'         :  my.mimes [12] = MIME_HUH;    break;
   }
   return 0;
}

char
RPTG_config_mimes_set   (uchar *a_mimes)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_mimes == NULL)  return -1;
   x_len = strlen (a_mimes);
   if (strlen (a_mimes) < 1)  return -2;
   RPTG_config_mimes_none ();
   for (i = 0; i < strlen (a_mimes); ++i) {
      sprintf (t, "--%c", a_mimes [i]);
      RPTG_config_mimes_add (t);
   }
   return 0;
}

char
RPTG_filter_mime        (uchar a_mime)
{
   switch (a_mime) {
   case MIME_AUDIO  :  if (my.mimes [0]  == a_mime)  return 1;  break;
   case MIME_VIDEO  :  if (my.mimes [1]  == a_mime)  return 1;  break;
   case MIME_IMAGE  :  if (my.mimes [2]  == a_mime)  return 1;  break;
   case MIME_CODE   :  if (my.mimes [3]  == a_mime)  return 1;  break;
   case MIME_ASCII  :  if (my.mimes [4]  == a_mime)  return 1;  break;
   case MIME_DBASE  :  if (my.mimes [5]  == a_mime)  return 1;  break;
   case MIME_CRYPT  :  if (my.mimes [6]  == a_mime)  return 1;  break;
   case MIME_PROP   :  if (my.mimes [7]  == a_mime)  return 1;  break;
   case MIME_EXEC   :  if (my.mimes [8]  == a_mime)  return 1;  break;
   case MIME_DIR    :  if (my.mimes [9]  == a_mime)  return 1;  break;
   case MIME_TEMP   :  if (my.mimes [10] == a_mime)  return 1;  break;
   case MIME_OTHER  :  if (my.mimes [11] == a_mime)  return 1;  break;
   case MIME_HUH    :  if (my.mimes [12] == a_mime)  return 1;  break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     size filter control                      ----===*/
/*====================------------------------------------====================*/
static void      o___SIZE____________________o (void) {;}

char RPTG_config_sizes_all   (void) { strlcpy (my.sizes, SIZES_ALL             , LEN_LABEL); return 0; }
char RPTG_config_sizes_none  (void) { strlcpy (my.sizes, "···················" , LEN_LABEL); return 0; }

char
RPTG_config_sizes_add   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.sizes, SIZES_ALL) == 0)  RPTG_config_sizes_none ();
   switch (a_option [2]) {
   case 'z'        :
      my.sizes [0]  = '0';
      break;
   case 's'        :
      my.sizes [1]  = '1';
      my.sizes [2]  = '2';
      my.sizes [3]  = '3';
      break;
   case 'k'        :
      my.sizes [4]  = '4';
      my.sizes [5]  = '5';
      my.sizes [6]  = '6';
      break;
   case 'm'        :
      my.sizes [7]  = '7';
      my.sizes [8]  = '8';
      my.sizes [9]  = '9';
      break;
   case 'g'        :
      my.sizes [10] = 'a';
      my.sizes [11] = 'b';
      my.sizes [12] = 'c';
      break;
   case 't'        :
      my.sizes [13] = 'd';
      my.sizes [14] = 'e';
      my.sizes [15] = 'f';
      break;
   case 'p'        :
      my.sizes [16] = 'g';
      my.sizes [17] = 'h';
      my.sizes [18] = 'i';
      break;
   }
   return 0;
}

char
RPTG_config_sizes_set   (uchar *a_sizes)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_sizes == NULL)  return -1;
   x_len = strlen (a_sizes);
   if (strlen (a_sizes) < 1)  return -2;
   RPTG_config_sizes_none ();
   for (i = 0; i < strlen (a_sizes); ++i) {
      sprintf (t, "--%c", a_sizes [i]);
      RPTG_config_sizes_add (t);
   }
   return 0;
}

char
RPTG_filter_size        (uchar a_size)
{
   if (my.sizes [a_size] != (uchar) '·')  return 1;
   return 0;
}


/*====================------------------------------------====================*/
/*===----                        age filter                            ----===*/
/*====================------------------------------------====================*/
static void      o___AGE_____________________o (void) {;}

char RPTG_config_ages_all    (void) { strlcpy (my.ages , AGES_ALL , LEN_LABEL); return 0; }
char RPTG_config_ages_none   (void) { strlcpy (my.ages , "·······", LEN_LABEL); return 0; }

char
RPTG_config_ages_add   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.ages, AGES_ALL)  == 0)  RPTG_config_ages_none ();
   switch (a_option [2]) {
   case 'j'  :  my.ages [0]  = 'j';  break;
   case 'd'  :  my.ages [1]  = 'd';  break;
   case 'w'  :  my.ages [2]  = 'w';  break;
   case 'm'  :  my.ages [3]  = 'm';  break;
   case 'q'  :  my.ages [4]  = 'q';   break;
   case 'y'  :  my.ages [5]  = 'y';  break;
   case 'a'  :  my.ages [6]  = 'a';  break;
   }
   return 0;
}

char
RPTG_config_ages_set   (uchar *a_ages)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_ages == NULL)  return -1;
   x_len = strlen (a_ages);
   if (strlen (a_ages) < 1)  return -2;
   RPTG_config_ages_none ();
   for (i = 0; i < strlen (a_ages); ++i) {
      sprintf (t, "--%c", a_ages [i]);
      RPTG_config_ages_add (t);
   }
   return 0;
}

char
RPTG_filter_age         (long a_days)
{
   if (a_days <= 1) {
      if (my.ages [0] != (uchar) '·')  return 1;
   } else if (a_days <= 3) {
      if (my.ages [1] != (uchar) '·')  return 1;
   } else if (a_days <= 7) {
      if (my.ages [2] != (uchar) '·')  return 1;
   } else if (a_days <= 30) {
      if (my.ages [3] != (uchar) '·')  return 1;
   } else if (a_days <= 90) {
      if (my.ages [4] != (uchar) '·')  return 1;
   } else if (a_days <= 365) {
      if (my.ages [5] != (uchar) '·')  return 1;
   } else {
      if (my.ages [6] != (uchar) '·')  return 1;
   }
   return 0;
}


/*====================------------------------------------====================*/
/*===----                      name quality filter                     ----===*/
/*====================------------------------------------====================*/
static void      o___ASCII___________________o (void) {;}

char RPTG_config_ascii_all   (void) { strlcpy (my.ascii, ASCII_ALL, LEN_LABEL); return 0; }
char RPTG_config_ascii_none  (void) { strlcpy (my.ascii, "······" , LEN_LABEL); return 0; }

char
RPTG_config_ascii_add  (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.ascii, ASCII_ALL)  == 0)  RPTG_config_ascii_none ();
   switch (a_option [2]) {
   case 'u'  :  my.ascii [1]  = ASCII_UPPER;  break;
   case 'p'  :  my.ascii [2]  = ASCII_PUNCT;  break;
   case 'e'  :  my.ascii [3]  = ASCII_EXTEND; break;
   case 's'  :  my.ascii [4]  = ASCII_SPACE;  break;
   case 'c'  :  my.ascii [5]  = ASCII_CRAZY;  break;
   case 'b'  :  RPTG_config_ascii_all (); my.ascii [0] = '·';   break;
   }
   return 0;
}

char
RPTG_config_ascii_set  (uchar *a_ascii)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_LABEL] = "";
   if (a_ascii == NULL)  return -1;
   x_len = strlen (a_ascii);
   if (strlen (a_ascii) < 1)  return -2;
   RPTG_config_ascii_none ();
   for (i = 0; i < strlen (a_ascii); ++i) {
      switch (a_ascii [i]) {
      case ASCII_UPPER  :  strlcpy (t, "--upper"  , LEN_LABEL);  break;
      case ASCII_PUNCT  :  strlcpy (t, "--punct"  , LEN_LABEL);  break; 
      case ASCII_EXTEND :  strlcpy (t, "--extend" , LEN_LABEL);  break;
      case ASCII_SPACE  :  strlcpy (t, "--space"  , LEN_LABEL);  break;
      case ASCII_CRAZY  :  strlcpy (t, "--crazy"  , LEN_LABEL);  break;
      }
      RPTG_config_ascii_add (t);
   }
   return 0;
}

char
RPTG_filter_ascii       (uchar a_ascii)
{
   switch (a_ascii) {
   case ASCII_BASIC  :  if (my.ascii [0]  == a_ascii)  return 1;  break;
   case ASCII_UPPER  :  if (my.ascii [1]  == a_ascii)  return 1;  break;
   case ASCII_PUNCT  :  if (my.ascii [2]  == a_ascii)  return 1;  break;
   case ASCII_EXTEND :  if (my.ascii [3]  == a_ascii)  return 1;  break;
   case ASCII_SPACE  :  if (my.ascii [4]  == a_ascii)  return 1;  break;
   case ASCII_CRAZY  :  if (my.ascii [5]  == a_ascii)  return 1;  break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         regex filter                         ----===*/
/*====================------------------------------------====================*/
static void      o___REGEX___________________o (void) {;}

char
RPTG_regex_prep         (char *a_regex)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =    0;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.regex_len = 0;
   strncpy (my.regex, "", MAX_REGEX);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("a_regex"   , a_regex);
   --rce;  if (a_regex == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_RPTG   yLOG_info    ("a_regex"   , a_regex);
   /*---(short-cut)----------------------*/
   x_len = strlen (a_regex);
   DEBUG_RPTG   yLOG_value   ("x_len"     , x_len);
   if (x_len <= 0) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(save)---------------------------*/
   strncpy (my.regex, a_regex, MAX_REGEX);
   my.regex_len = x_len;
   /*---(compile)------------------------*/
   rc = yREGEX_comp (a_regex);
   DEBUG_RPTG   yLOG_value   ("comp"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_regex_filter       (char *a_string)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =    0;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("a_string"  , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_RPTG   yLOG_info    ("a_string"  , a_string);
   x_len = strlen (a_string);
   DEBUG_RPTG   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(filter by name)--------------*/
   rc = yREGEX_fast (a_string);
   DEBUG_INPT   yLOG_value   ("exec"      , rc);
   if (rc <= 0) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 1;
}



/*====================------------------------------------====================*/
/*===----                       permission filter                      ----===*/
/*====================------------------------------------====================*/
static void      o___PERMS___________________o (void) {;}

char
RPTG_perms_filter       (int a_uid, char a_own, int a_gid, char a_grp, char a_oth)
{
   /*---(local variables)--+-----------+-*/
   uchar       x_allowed   =  '·';
   uchar       x_mask      = 0x04;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   DEBUG_RPTG   yLOG_complex ("other"     , "%2x, %2x, %2x", a_oth, x_mask, a_oth & x_mask);
   DEBUG_RPTG   yLOG_complex ("group"     , "%2x, %2x, %2x", a_grp, x_mask, a_grp & x_mask);
   DEBUG_RPTG   yLOG_complex ("owner"     , "%2x, %2x, %2x", a_own, x_mask, a_own & x_mask);
   if (my.uid == 0)                                        x_allowed = 'r';
   else if ((a_oth & x_mask) == x_mask)                    x_allowed = 'o';
   else if (a_gid == my.gid && (a_grp & x_mask) == x_mask) x_allowed = 'g';
   else if (a_uid == my.uid && (a_own & x_mask) == x_mask) x_allowed = 'u';
   DEBUG_RPTG   yLOG_char    ("x_allowed" , x_allowed);
   if (x_allowed != (uchar) '·') {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_perms_dir          (int a_uid, char a_own, int a_gid, char a_grp, char a_oth)
{
   /*---(local variables)--+-----------+-*/
   uchar       x_allowed   =  '·';
   uchar       x_mask      = 0x05;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   DEBUG_RPTG   yLOG_complex ("other"     , "%2x, %2x, %2x", a_oth, x_mask, a_oth & x_mask);
   DEBUG_RPTG   yLOG_complex ("group"     , "%2x, %2x, %2x", a_grp, x_mask, a_grp & x_mask);
   DEBUG_RPTG   yLOG_complex ("owner"     , "%2x, %2x, %2x", a_own, x_mask, a_own & x_mask);
   if (my.uid == 0)                                        x_allowed = 'r';
   else if ((a_oth & x_mask) == x_mask)                    x_allowed = 'o';
   else if (a_gid == my.gid && (a_grp & x_mask) == x_mask) x_allowed = 'g';
   else if (a_uid == my.uid && (a_own & x_mask) == x_mask) x_allowed = 'u';
   DEBUG_RPTG   yLOG_char    ("x_allowed" , x_allowed);
   if (x_allowed != (uchar) '·') {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
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
   long        x_days      =    0;
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
   rc = RPTG_perms_filter (a_data->uid, a_data->own, a_data->gid, a_data->grp, a_data->oth);
   DEBUG_DATA  yLOG_complex  ("perms"     , "%d, %4d %4d, %d %4d %4d, %d, %d", a_data->own, a_data->uid, my.uid, a_data->grp, a_data->gid, my.gid, a_data->oth, rc);
   if (rc <= 0)  return 0;
   /*---(regex)--------------------------*/
   rc = RPTG_regex_filter (a_data->name);
   DEBUG_DATA  yLOG_complex  ("regex"     , "%s, %d", a_data->name, rc);
   if (rc <= 0)  return 0;
   /*---(other filters)------------------*/
   rc = RPTG_filter_type  (a_data->type);
   DEBUG_DATA  yLOG_complex  ("type"      , "%s, %c, %d", my.types, a_data->type , rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_mime  (a_data->cat);
   DEBUG_DATA  yLOG_complex  ("mime"      , "%s, %c, %d", my.mimes, a_data->cat  , rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_size  (a_data->size);
   DEBUG_DATA  yLOG_complex  ("mime"      , "%s, %d, %d", my.sizes, a_data->size , rc);
   if (rc <= 0)  return 0;
   x_days = (my.runtime - a_data->changed) / (24 * 60 * 60);
   rc = RPTG_filter_age   (x_days);
   DEBUG_DATA  yLOG_complex  ("age"       , "%s, %d, %d", my.ages , x_days       , rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_ascii (a_data->ascii);
   DEBUG_DATA  yLOG_complex  ("ascii"     , "%s, %c, %d", my.ascii, a_data->ascii, rc);
   if (rc <= 0)  return 0;
   /*---(serious)------------------------*/
   DEBUG_DATA  yLOG_char     ("a_serious" , a_serious);
   if (a_serious == 'y') {
      g_found = a_data;
      strlcpy (g_path, a_full, LEN_RECD);
   }
   /*---(complete)-----------------------*/
   printf ("%s\n", a_full);
   /*---(complete)-----------------------*/
   return 1;
}

char
RPTG_walker       (char a_trigger)
{
   return ENTRY_walk (a_trigger, RPTG__callback);
}



/*====================------------------------------------====================*/
/*===----                       search driver                          ----===*/
/*====================------------------------------------====================*/
static void      o___SEARCH__________________o (void) {;}

char         /*===[[ gather entries in dir ]]=============[ ------ [ ------ ]=*/
RPTG_regex         (
      /*---(formal parameters)+-------------+---------------------------------*/
      int         a_level     ,             /* depth of search                */
      tPTRS      *a_ptrs      ,             /* ptrs of directory to scan      */
      char       *a_path      )             /* full path of directory to scan */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tENTRY      *x_dir       = NULL;          /* directory data                 */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   tENTRY      *x_data      = NULL;          /* current entry data             */
   char        rc          = 0;
   char        x_path      [500];
   int         i           = 0;
   char        x_allowed   = '-';
   char        x_comma     [20] = "";
   long        x_days      =   0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_value   ("a_level"   , a_level);
   DEBUG_GRAF   yLOG_point   ("a_ptrs"    , a_ptrs);
   DEBUG_GRAF   yLOG_info    ("a_path"    , a_path);
   /*---(defense)------------------------*/
   --rce;  if (a_level >  my.maxlevel) {
      DEBUG_GRAF   yLOG_note    ("max recursion reached, return");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (my.regex_len  <= 0) {
      DEBUG_GRAF   yLOG_note    ("regex not loaded, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_ptrs == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory pointer NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_point   ("dir_data"  , a_ptrs->data);
   --rce;  if (a_ptrs->data == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory data payload NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_dir = a_ptrs->data;
   DEBUG_GRAF   yLOG_point   ("dir_name"  , x_dir->name);
   --rce;  if (x_dir->name == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory name NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_info    ("dir_name"  , x_dir->name);
   /*---(name directory)--------------*/
   if      (a_level == 1)  sprintf (x_path, "%s"   , x_dir->name);
   else if (a_level == 2)  sprintf (x_path, "/%s"  , x_dir->name);
   else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);
   DEBUG_GRAF   yLOG_info    ("x_path"    , x_path);
   OPT_VERBOSE  printf ("%s\n", x_path);
   /*---(check start path)---------------*/
   DEBUG_GRAF   yLOG_value   ("my.level"  , my.level);
   if (a_level <= my.level) {
      DEBUG_GRAF   yLOG_note    ("using --start path");
      x_ptrs = root_stack [a_level];
      x_data = x_ptrs->data;
      DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
      RPTG_regex (a_level + 1, x_ptrs, x_path);
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(spin through entries)-----------*/
   x_ptrs = a_ptrs->c_head;
   while (x_ptrs != NULL) {
      x_data = x_ptrs->data;
      if (x_data != NULL) {
         DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
         x_days = (my.runtime - x_data->changed) / (365 * 24 * 60);
         if (x_data->type != ENTRY_DIR) {
            /*---(non-regex filtering)---*/
            if (my.find == 'y') {
               /*---(days)---------------*/
               if      (my.find_days  == 'j' && x_days >=   2) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_days  == 'w' && x_days >=   7) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_days  == 'm' && x_days >=  30) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_days  == 'y' && x_days >= 365) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_days  == 'o' && x_days <= 365) { x_ptrs = x_ptrs->s_next; continue; }
               /*---(size)---------------*/
               if      (my.find_size  == 's' && x_data->size >=   3) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_size  == 'k' && (x_data->size < 4 || x_data->size > 6)) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_size  == 'm' && (x_data->size < 7 || x_data->size > 9)) { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_size  == 'g' && x_data->size <=   9) { x_ptrs = x_ptrs->s_next; continue; }
               /*---(name quality)-------*/
               if      (my.find_name  == 'p' && x_data->ascii != '+') { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_name  == 'e' && x_data->ascii != '#') { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_name  == 's' && x_data->ascii != '>') { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_name  == 'j' && x_data->ascii != 'X') { x_ptrs = x_ptrs->s_next; continue; }
               else if (my.find_name  == 'b' && strchr ("+#>X", x_data->ascii) == NULL) { x_ptrs = x_ptrs->s_next; continue; }
               /*---(mime-cat)-----------*/
               if (my.find_cat != '-') {
                  if (my.find_cat != x_data->cat) {
                     x_ptrs = x_ptrs->s_next;
                     continue;
                  }
               }
               /*---(mime-type)----------*/
               if (my.find_mime[0] != '\0') {
                  if (my.find_mime [0] != x_data->ext [0] || strcmp (my.find_mime, x_data->ext) != 0) {
                     x_ptrs = x_ptrs->s_next;
                     continue;
                  }
               }
               /*---(done)---------------*/
            }
            /*---(regex filtering)-------*/
            rc = regexec (&(my.regex_comp), x_data->name, 0, NULL, 0);
            if (rc == 0 && my.total < my.limit) {
               ++my.total;
               if (my.count != 'y' && (my.number == 0  || my.number == my.total)) {
                  if (my.show_cat   == 'y')  printf ("%c  "    , x_data->cat);
                  if (my.show_mime  == 'y')  printf ("%-5s  "  , x_data->ext);
                  if (my.show_days  == 'y')  printf ("%4d  "   , x_days);
                  if (my.show_size  == 'y')  printf ("%1d  "   , x_data->size);
                  if (my.show_bytes == 'y') { FILE_commas (x_data->bytes, x_comma); printf ("%11.11s  ", x_comma); }
                  if (my.show_level == 'y')  printf ("%2d  "   , x_data->lvl);
                  if (my.show_ascii == 'y')  printf ("%c  "    , x_data->ascii);
                  printf ("%s/%s\n", x_path, x_data->name);
               }
               for (i = 0; i < n_mime; ++i) {
                  if (g_mime [i].cat       != x_data->cat)        continue;
                  if (strcmp (g_mime [i].ext, x_data->ext) != 0)  continue;
                  DEBUG_GRAF   yLOG_info    ("type"      , g_mime [i].ext);
                  ++(g_mime [i].found);
                  g_mime [i].fbytes += x_data->bytes;
                  ++(g_mime [0].found);
                  g_mime [0].fbytes += x_data->bytes;
                  break;
               }
            }
         } else {
            x_allowed = '-';
            if (my.uid == 0)                                                x_allowed = 'r';
            else if (((x_data->oth % 4) % 2) == 1)                          x_allowed = 'o';
            else if (x_data->gid == my.gid && ((x_data->grp % 4) % 2) == 1) x_allowed = 'g';
            else if (x_data->uid == my.uid && ((x_data->own % 4) % 2) == 1) x_allowed = 'u';
            DEBUG_GRAF   printf ("%c   ", x_allowed);
            DEBUG_GRAF   printf ("%c %d   ", x_data->oth, (x_data->oth % 4) % 2);
            DEBUG_GRAF   printf ("%4d %4d %c %d   ", my.gid, x_data->gid, x_data->grp, (x_data->grp % 4) % 2);
            DEBUG_GRAF   printf ("%4d %4d %c %d   ", my.uid, x_data->uid, x_data->own, (x_data->own % 4) % 2);
            DEBUG_GRAF   printf ("%s/%s\n", x_path, x_data->name);
            if (x_allowed != '-')  RPTG_regex (a_level + 1, x_ptrs, x_path);
         }
      }
      x_ptrs = x_ptrs->s_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ output the dir tree ]]===============[ ------ [ ------ ]=*/
RPTG_dirtree       (
      /*---(formal parameters)+-------------+---------------------------------*/
      int         a_level     ,             /* depth of search                */
      tPTRS      *a_ptrs      ,             /* ptrs of directory to scan      */
      char       *a_path      )             /* full path of directory to scan */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tENTRY      *x_dir       = NULL;          /* directory data                 */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   tENTRY      *x_data      = NULL;          /* current entry data             */
   char        rc          = 0;
   char        x_path      [500];
   int         i           = 0;
   char        x_prefix    [100] = "";
   char        x_temp      [200] = "";
   char        x_cum       [20]  = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_value   ("a_level"   , a_level);
   DEBUG_GRAF   yLOG_point   ("a_ptrs"    , a_ptrs);
   DEBUG_GRAF   yLOG_info    ("a_path"    , a_path);
   /*---(defense)------------------------*/
   --rce;  if (a_level >  my.maxlevel) {
      DEBUG_GRAF   yLOG_note    ("max recursion reached, return");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (a_ptrs == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory pointer NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_point   ("dir_data"  , a_ptrs->data);
   --rce;  if (a_ptrs->data == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory data payload NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_dir = a_ptrs->data;
   DEBUG_GRAF   yLOG_point   ("dir_name"  , x_dir->name);
   --rce;  if (x_dir->name == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory name NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_info    ("dir_name"  , x_dir->name);
   /*---(header)-------------------------*/
   if (a_level == 1) {
      printf ("#!/usr/local/bin/hyleoroi\n");
      printf ("#   hyleoroi -- tree visualization input file\n");
      printf ("\n\n\n");
      printf ("SIMPLE\n");
      printf ("\n\n\n");
      printf ("#--context  ---values------------------------------- \n");
      printf ("source      heilos-phaeton                           \n");
      printf ("label       full directory tree for 2017-05-21       \n");
      printf ("format      dirtree                                  \n");
      printf ("\n\n\n");
   }
   /*---(prefix)-------------------------*/
   for (i = 1; i < a_level; ++i)  strcat (x_prefix, "   ");
   /*---(name directory)--------------*/
   if      (a_level == 1)  sprintf (x_path, "%s"   , x_dir->name);
   else if (a_level == 2)  sprintf (x_path, "/%s"  , x_dir->name);
   else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);
   DEBUG_GRAF   yLOG_info    ("x_path"    , x_path);
   OPT_VERBOSE  printf ("%s\n", x_path);
   /*> printf ("%s%s\n", x_prefix, x_path);                                           <*/
   sprintf (x_temp, "%s%s", x_prefix, x_dir->name);
   /*> FILE_commas (x_dir->bcum, x_cum);                                               <*/
   /*> printf  ("%-100.100s %14s\n", x_temp, x_cum);                                  <*/
   printf  ("%-100.100s | %14ld\n", x_temp, x_dir->bcum);
   /*---(check start path)---------------*/
   DEBUG_GRAF   yLOG_value   ("my.level"  , my.level);
   if (a_level <= my.level) {
      DEBUG_GRAF   yLOG_note    ("using --start path");
      x_ptrs = root_stack [a_level];
      x_data = x_ptrs->data;
      DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
      RPTG_dirtree (a_level + 1, x_ptrs, x_path);
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(spin through entries)-----------*/
   x_ptrs = a_ptrs->c_head;
   while (x_ptrs != NULL) {
      x_data = x_ptrs->data;
      if (x_data != NULL) {
         DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
         if (x_data->type == ENTRY_DIR) {
            RPTG_dirtree (a_level + 1, x_ptrs, x_path);
         }
      }
      x_ptrs = x_ptrs->s_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   for (i = 1; i < n_mime; ++i) {
      if (g_mime [i].cat == MIME_DIR && strcmp (g_mime [i].ext, "dir") == 0) {
         x_dseen  += g_mime [i].seen;
         x_dkept  += g_mime [i].kept;
      } else {
         x_fseen  += g_mime [i].seen;
         x_fkept  += g_mime [i].kept;
      }
   }
   /*---(output)-------------------------*/
   printf ("database %s:\n", my.database);
   printf ("\n");
   printf ("ref-   ---host--- --serial-- ---device------ ---mount-point------ --type-- ---bytes------------   ---written----------\n");
   x_drive = h_drive;
   while (x_drive != NULL) {
      x_written = x_drive->written;
      curr = localtime (&x_written);
      if (curr == NULL) printf ("local time failed\n");
      else              strftime (x_time, 100, "%y.%m.%d %H:%M:%S %U", curr);
      FILE_commas (x_drive->size , x_comma);
      printf ("[%02d]   %-10.10s %-10.10s %-15.15s %-20.20s %-8.8s %20.20s   %s\n",
            x_drive->ref   , x_drive->host  ,
            x_drive->serial, x_drive->device, x_drive->mpoint,
            x_drive->type  , x_comma        , x_time);
      x_drive = x_drive->m_next;
   }
   printf ("\n");
   printf ("---type----- ---seen----- ---kept----- ---each----- ---bytes----\n");
   printf ("directories ");
   FILE_commas (x_dseen, x_comma);
   printf (" %12.12s");
   FILE_commas (x_dkept, x_comma);
   printf (" %12.12s");
   FILE_commas (sizeof (tENTRY), x_comma);
   printf (" %12.12s");
   FILE_commas (x_dkept * sizeof (tENTRY), x_comma);
   printf (" %12.12s\n");
   printf ("files       ");
   FILE_commas (x_fseen, x_comma);
   printf (" %12.12s");
   FILE_commas (x_fkept, x_comma);
   printf (" %12.12s");
   FILE_commas (sizeof (tENTRY), x_comma);
   printf (" %12.12s");
   FILE_commas (x_fkept * sizeof (tENTRY), x_comma);
   printf (" %12.12s\n");
   printf ("             ------------ ------------              ------------\n");
   printf ("   total    ");
   FILE_commas (x_dseen + x_fseen, x_comma);
   printf (" %12.12s");
   FILE_commas (x_dkept + x_fkept, x_comma);
   printf (" %12.12s");
   printf ("             ");
   FILE_commas ((x_dkept + x_fkept) * sizeof (tENTRY), x_comma);
   printf (" %12.12s\n");
   printf ("                                                    ------------\n");
   printf ("                 with device entries (approximate) ");
   FILE_commas (sizeof (short) + (n_drive * sizeof (tDRIVE)) + ((x_dkept + x_fkept) * sizeof (tENTRY)), x_comma);
   printf (" %12.12s\n");
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
      strlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.types); ++i) {
         sprintf (s, " %c", my.types [i]);
         strlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG types       :%s", t);
   }
   else if (strcmp (a_question, "mimes"         ) == 0) {
      strlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.mimes); ++i) {
         sprintf (s, " %c", my.mimes [i]);
         strlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG mimes       :%s", t);
   }
   else if (strcmp (a_question, "sizes"         ) == 0) {
      strlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.sizes); ++i) {
         sprintf (s, " %c", my.sizes [i]);
         strlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG sizes       :%s", t);
   }
   else if (strcmp (a_question, "ages"          ) == 0) {
      strlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.ages); ++i) {
         sprintf (s, " %c", my.ages [i]);
         strlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG ages        :%s", t);
   }
   else if (strcmp (a_question, "ascii"         ) == 0) {
      strlcpy (t, "", LEN_DESC);
      for (i = 0; i < strlen (my.ascii); ++i) {
         sprintf (s, " %c", my.ascii [i]);
         strlcat (t, s, LEN_DESC);
      }
      snprintf (unit_answer, LEN_FULL, "RPTG ascii       :%s", t);
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
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}





/*===============================[[ end-code ]]===============================*/
