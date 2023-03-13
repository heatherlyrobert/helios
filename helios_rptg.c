/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



static char  s_recd    [LEN_RECD]  = "";
static int   s_days    =   0;
static char  s_age     = '-';
static int   s_pos     =  -1;
static int   s_len     =  -1;
static long  s_bytes   =   0;



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
   RPTG_config_super_off  ();
   RPTG_config_col_none   ();
   s_days    =   0;
   s_age     = '-';
   s_pos     =  -1;
   s_len     =  -1;
   s_bytes   =   0;
   my.r_uid  =  -1;
   my.r_gid  =  -1;
   strlcpy (my.ext, "", LEN_TERSE);
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
   if (strlen (a_option) <= 2)  return -2;
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
RPTG_config_types_sub   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   switch (a_option [4]) {
   case 'r'        :  my.types [0] = '·';           break;
   case ENTRY_DIR  :  my.types [1] = '·';           break;
   case ENTRY_CDEV :  my.types [2] = '·';           break;
   case ENTRY_BDEV :  my.types [3] = '·';           break;
   case ENTRY_FIFO :  my.types [4] = '·';           break;
   case ENTRY_SOCK :  my.types [5] = '·';           break;
   case ENTRY_LINK :  my.types [6] = '·';           break;
   case 'u'        :  my.types [7] = '·';           break;
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
RPTG_filter_type        (uchar a_type, uchar a_stype)
{
   if (a_stype == STYPE_LINK && my.types [6] == (uchar) '·')  return 0;
   switch (a_type) {
   case ENTRY_REG    :  if (my.types [0] == a_type)  return 1;    break;
   case ENTRY_DIR    :  if (my.types [1] == a_type)  return 1;    break;
   case ENTRY_CDEV   :  if (my.types [2] == a_type)  return 1;    break;
   case ENTRY_BDEV   :  if (my.types [3] == a_type)  return 1;    break;
   case ENTRY_FIFO   :  if (my.types [4] == a_type)  return 1;    break;
   case ENTRY_SOCK   :  if (my.types [5] == a_type)  return 1;    break;
   case ENTRY_HUH    :  if (my.types [7] == a_type)  return 1;    break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     mime filter control                      ----===*/
/*====================------------------------------------====================*/
static void      o___MIME____________________o (void) {;}

char RPTG_config_mimes_all   (void) { strlcpy (my.mimes, MIME_ALL        , LEN_LABEL); return 0; }
char RPTG_config_mimes_none  (void) { strlcpy (my.mimes, "··············", LEN_LABEL); return 0; }

char
RPTG_config_mimes_add   (uchar *a_option)
{
   int         x_len       =     0;
   int         x_ch        =   '-';
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.mimes, MIME_ALL)  == 0)  RPTG_config_mimes_none ();
   x_ch = a_option [2];
   if (strncmp (a_option, "--all", 5) == 0) {
      switch (a_option [5]) {
      case 'm' :  x_ch = MIME_MEDIA;  break;
      case 'w' :  x_ch = MIME_WORK;   break;
      case 't' :  x_ch = MIME_TEMP;   break;
      }
   }
   switch (x_ch) {
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
   case MIME_JUNK   :  my.mimes [10] = MIME_JUNK;   break;
   case MIME_OTHER  :  my.mimes [11] = MIME_OTHER;  break;
   case 'h'         :  my.mimes [12] = MIME_HUH;    break;
   }
   switch (x_ch) {
   case MIME_MEDIA  :  my.mimes [0]  = MIME_AUDIO;
                       my.mimes [1]  = MIME_VIDEO;
                       my.mimes [2]  = MIME_IMAGE;
                       break;
   case MIME_WORK   :  my.mimes [3]  = MIME_CODE;
                       my.mimes [4]  = MIME_ASCII;
                       my.mimes [5]  = MIME_DBASE;
                       my.mimes [6]  = MIME_CRYPT;
                       my.mimes [7]  = MIME_PROP;
                       my.mimes [8]  = MIME_EXEC;
                       my.mimes [9]  = MIME_DIR;
                       break;
   case MIME_TEMP   :  my.mimes [10] = MIME_JUNK;
                       my.mimes [11] = MIME_OTHER;
                       my.mimes [12] = MIME_HUH;
                       break;
   }
   return 0;
}

char
RPTG_config_mimes_sub   (uchar *a_option)
{
   int         x_len       =     0;
   int         x_ch        =   '-';
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   x_ch = a_option [4];
   if (strncmp (a_option, "--zero", 6) == 0) {
      switch (a_option [6]) {
      case 'm' :  x_ch = MIME_MEDIA;  break;
      case 'w' :  x_ch = MIME_WORK;   break;
      case 't' :  x_ch = MIME_TEMP;   break;
      }
   }
   switch (x_ch) {
   case MIME_AUDIO  :  my.mimes [0]  = (uchar) '·'; break;
   case MIME_VIDEO  :  my.mimes [1]  = (uchar) '·'; break;
   case MIME_IMAGE  :  my.mimes [2]  = (uchar) '·'; break;
   case MIME_CODE   :  my.mimes [3]  = (uchar) '·'; break;
   case MIME_ASCII  :  my.mimes [4]  = (uchar) '·'; break;
   case MIME_DBASE  :  my.mimes [5]  = (uchar) '·'; break;
   case MIME_CRYPT  :  my.mimes [6]  = (uchar) '·'; break;
   case MIME_PROP   :  my.mimes [7]  = (uchar) '·'; break;
   case MIME_EXEC   :  my.mimes [8]  = (uchar) '·'; break;
   case MIME_DIR    :  my.mimes [9]  = (uchar) '·'; break;
   case MIME_JUNK   :  my.mimes [10] = (uchar) '·'; break;
   case MIME_OTHER  :  my.mimes [11] = (uchar) '·'; break;
   case 'h'         :  my.mimes [12] = (uchar) '·'; break;
   }
   switch (x_ch) {
   case MIME_MEDIA  :  my.mimes [0]  = (uchar) '·';
                       my.mimes [1]  = (uchar) '·';
                       my.mimes [2]  = (uchar) '·';
                       break;
   case MIME_WORK   :  my.mimes [3]  = (uchar) '·';
                       my.mimes [4]  = (uchar) '·';
                       my.mimes [5]  = (uchar) '·';
                       my.mimes [6]  = (uchar) '·';
                       my.mimes [7]  = (uchar) '·';
                       my.mimes [8]  = (uchar) '·';
                       my.mimes [9]  = (uchar) '·';
                       break;
   case MIME_TEMP   :  my.mimes [10] = (uchar) '·';
                       my.mimes [11] = (uchar) '·';
                       my.mimes [12] = (uchar) '·';
                       break;
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
RPTG_filter_mime        (uchar a_cat, uchar *a_ext)
{
   char          rc        =    0;
   rc = strcmp (my.ext, "");
   DEBUG_DATA  yLOG_complex  ("mime check", "%s, %s, %c, %s, %d", my.mimes, my.ext, a_cat, a_ext, rc);
   if (rc != 0) {
      DEBUG_DATA  yLOG_note     ("must test extension");
      rc = strcmp (a_ext, my.ext);
      DEBUG_DATA  yLOG_complex  ("check"     , "%s, %s, %d", my.ext, a_ext, rc);
      if (rc == 0)  {
         DEBUG_DATA  yLOG_note     ("MATCH");
         return 1;
      }
      DEBUG_DATA  yLOG_note     ("no match");
      return 0;
   }
   switch (a_cat) {
   case MIME_AUDIO  :  if (my.mimes [0]  == a_cat)  return 1;  break;
   case MIME_VIDEO  :  if (my.mimes [1]  == a_cat)  return 1;  break;
   case MIME_IMAGE  :  if (my.mimes [2]  == a_cat)  return 1;  break;
   case MIME_CODE   :  if (my.mimes [3]  == a_cat)  return 1;  break;
   case MIME_ASCII  :  if (my.mimes [4]  == a_cat)  return 1;  break;
   case MIME_DBASE  :  if (my.mimes [5]  == a_cat)  return 1;  break;
   case MIME_CRYPT  :  if (my.mimes [6]  == a_cat)  return 1;  break;
   case MIME_PROP   :  if (my.mimes [7]  == a_cat)  return 1;  break;
   case MIME_EXEC   :  if (my.mimes [8]  == a_cat)  return 1;  break;
   case MIME_DIR    :  if (my.mimes [9]  == a_cat)  return 1;  break;
   case MIME_JUNK   :  if (my.mimes [10] == a_cat)  return 1;  break;
   case MIME_OTHER  :  if (my.mimes [11] == a_cat)  return 1;  break;
   case MIME_HUH    :  if (my.mimes [12] == a_cat)  return 1;  break;
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
RPTG_config_sizes_sub   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   switch (a_option [4]) {
   case 'z'        :
      my.sizes [0]  = (uchar) '·';
      break;
   case 's'        :
      my.sizes [1]  = (uchar) '·';
      my.sizes [2]  = (uchar) '·';
      my.sizes [3]  = (uchar) '·';
      break;
   case 'k'        :
      my.sizes [4]  = (uchar) '·';
      my.sizes [5]  = (uchar) '·';
      my.sizes [6]  = (uchar) '·';
      break;
   case 'm'        :
      my.sizes [7]  = (uchar) '·';
      my.sizes [8]  = (uchar) '·';
      my.sizes [9]  = (uchar) '·';
      break;
   case 'g'        :
      my.sizes [10] = (uchar) '·';
      my.sizes [11] = (uchar) '·';
      my.sizes [12] = (uchar) '·';
      break;
   case 't'        :
      my.sizes [13] = (uchar) '·';
      my.sizes [14] = (uchar) '·';
      my.sizes [15] = (uchar) '·';
      break;
   case 'p'        :
      my.sizes [16] = (uchar) '·';
      my.sizes [17] = (uchar) '·';
      my.sizes [18] = (uchar) '·';
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
   case AGES_JUST    :  my.ages [0]  = AGES_JUST;     break;
   case AGES_DAYS    :  my.ages [1]  = AGES_DAYS;     break;
   case AGES_WEEK    :  my.ages [2]  = AGES_WEEK;     break;
   case AGES_MONTH   :  my.ages [3]  = AGES_MONTH;    break;
   case AGES_QUARTER :  my.ages [4]  = AGES_QUARTER;  break;
   case AGES_YEAR    :  my.ages [5]  = AGES_YEAR;     break;
   case AGES_ANCIENT :  my.ages [6]  = AGES_ANCIENT;  break;
   }
   return 0;
}

char
RPTG_config_ages_sub    (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   switch (a_option [4]) {
   case AGES_JUST    :  my.ages [0]  = (uchar) '·';   break;
   case AGES_DAYS    :  my.ages [1]  = (uchar) '·';   break;
   case AGES_WEEK    :  my.ages [2]  = (uchar) '·';   break;
   case AGES_MONTH   :  my.ages [3]  = (uchar) '·';   break;
   case AGES_QUARTER :  my.ages [4]  = (uchar) '·';   break;
   case AGES_YEAR    :  my.ages [5]  = (uchar) '·';   break;
   case AGES_ANCIENT :  my.ages [6]  = (uchar) '·';   break;
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
      s_age = AGES_JUST;
      if (my.ages [0] != (uchar) '·')  return 1;
   } else if (a_days <= 3) {
      s_age = AGES_DAYS;
      if (my.ages [1] != (uchar) '·')  return 1;
   } else if (a_days <= 7) {
      s_age = AGES_WEEK;
      if (my.ages [2] != (uchar) '·')  return 1;
   } else if (a_days <= 30) {
      s_age = AGES_MONTH;
      if (my.ages [3] != (uchar) '·')  return 1;
   } else if (a_days <= 90) {
      s_age = AGES_QUARTER;
      if (my.ages [4] != (uchar) '·')  return 1;
   } else if (a_days <= 365) {
      s_age = AGES_YEAR;
      if (my.ages [5] != (uchar) '·')  return 1;
   } else {
      s_age = AGES_ANCIENT;
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
RPTG_config_ascii_sub   (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   switch (a_option [4]) {
   case 'u'  :  my.ascii [1]  = (uchar) '·';  break;
   case 'p'  :  my.ascii [2]  = (uchar) '·';  break;
   case 'e'  :  my.ascii [3]  = (uchar) '·';  break;
   case 's'  :  my.ascii [4]  = (uchar) '·';  break;
   case 'c'  :  my.ascii [5]  = (uchar) '·';  break;
   case 'b'  :  RPTG_config_ascii_none (); my.ascii [0] = '-';   break;
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
/*===----                         regex filter                         ----===*/
/*====================------------------------------------====================*/
static void      o___REGEX___________________o (void) {;}

char
RPTG_regex_prep         (uchar *a_regex)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =    0;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.regex_len = 1;
   strncpy (my.regex, ".", MAX_REGEX);
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
RPTG_regex_filter       (uchar *a_string)
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
   DEBUG_RPTG   yLOG_info    ("my.regex"  , my.regex);
   if (strcmp (my.regex, ".") == 0) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(filter by name)--------------*/
   rc = yREGEX_filter (a_string);
   DEBUG_INPT   yLOG_value   ("exec"      , rc);
   if (rc <= 0) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*> rc = yREGEX_best (YREGEX_BEST, -1, &s_pos, &s_len, NULL, NULL);                <*/
   rc = yREGEX_best (YREGEX_BEST, &s_pos, &s_len, NULL, NULL);
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
   /*---(can you see it)-----------------*/
   if (my.uid == 0)                                        x_allowed = 'r';
   else if ((a_oth & x_mask) == x_mask)                    x_allowed = 'o';
   else if (a_gid == my.gid && (a_grp & x_mask) == x_mask) x_allowed = 'g';
   else if (a_uid == my.uid && (a_own & x_mask) == x_mask) x_allowed = 'u';
   DEBUG_RPTG   yLOG_char    ("x_allowed" , x_allowed);
   /*---(uid/gid)------------------------*/
   if (my.r_uid >= 0 && my.r_uid != a_uid)   x_allowed = '·';
   if (my.r_gid >= 0 && my.r_gid != a_gid)   x_allowed = '·';
   /*---(decide)-------------------------*/
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
/*===----                       output formats                         ----===*/
/*====================------------------------------------====================*/
static void      o___FORMAT__________________o (void) {;}

char RPTG_config_col_none    (void) { strlcpy (my.columns, "··········", LEN_LABEL); if (my.output == OUTPUT_PREFIX)  my.output = OUTPUT_NORMAL;  return 0; }

char
RPTG_config_columns      (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 7)  return -2;
   switch (a_option [7]) {
   case COL_MIME   :  my.columns [0] = COL_MIME;      break;
   case COL_AGES   :  my.columns [1] = COL_AGES;      break;
   case COL_SIZE   :  my.columns [2] = COL_SIZE;      break;
   case COL_LEVEL  :  my.columns [3] = COL_LEVEL;     break;
   case COL_NAMING :  my.columns [4] = COL_NAMING;    break;
   case COL_FIND   :  my.columns [5] = COL_FIND;      break;
   case COL_TYPE   :  my.columns [6] = COL_TYPE;      break;
   case COL_PERMS  :  my.columns [7] = COL_PERMS;     break;
   case COL_DRIVE  :  my.columns [8] = COL_DRIVE;     break;
   case COL_BASE   :  my.columns [9] = COL_BASE;      break;
   }
   my.output = OUTPUT_PREFIX;
   return 0;
}

char
RPTG_config_output       (uchar *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   RPTG_config_col_none ();
   switch (a_option [2]) {
   case 'n'  :  my.output = OUTPUT_NORMAL;      break;
   case 'd'  :  my.output = OUTPUT_DETAIL;      break;
   case 'a'  :  my.output = OUTPUT_ANALYSIS;    break;
   case 'g'  :  my.output = OUTPUT_GYGES;       break;
   case 'c'  :  my.output = OUTPUT_COUNT;       break;
   case 's'  :  my.output = OUTPUT_SILENT;      break;
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
   strlcpy (s_recd, "", LEN_RECD);
   /*---(quick out)----------------------*/
   if (my.headers != 'y')            return 0;
   if (my.output  == OUTPUT_SILENT)  return 0;
   if (my.output  == OUTPUT_COUNT)   return 0;
   if (my.total   != 0)              return 0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(shared)-------------------------*/
   printf ("## HELIOS-PHAETON (locate) -- filesystem searching and indexing services\n");
   if (my.path [0] != '\0')  strlcpy (t, my.path  , LEN_PATH);
   else                      strlcpy (t, "<none>" , LEN_PATH);
   if (my.maxlevel >= 99)    strlcpy (s, "<unset>", LEN_LABEL);
   else                      sprintf (t, "%d"     , my.maxlevel);
   /*---(individual)---------------------*/
   DEBUG_RPTG   yLOG_char    ("my.output" , my.output);
   switch (my.output) {
   case OUTPUT_NORMAL   :
      strlcat (s_recd, "##    report option (name only) --normal", LEN_RECD);
      break;
   case OUTPUT_PREFIX   :
      strlcat (s_recd, "##    report option (prefixed)", LEN_RECD);
      if (my.columns [0] != (uchar) '·')  strlcat (s_recd, " --show-mime"  , LEN_RECD);
      if (my.columns [1] != (uchar) '·')  strlcat (s_recd, " --show-age"   , LEN_RECD);
      if (my.columns [2] != (uchar) '·')  strlcat (s_recd, " --show-size"  , LEN_RECD);
      if (my.columns [3] != (uchar) '·')  strlcat (s_recd, " --show-level" , LEN_RECD);
      if (my.columns [4] != (uchar) '·')  strlcat (s_recd, " --show-naming", LEN_RECD);
      if (my.columns [5] != (uchar) '·')  strlcat (s_recd, " --show-find"  , LEN_RECD);
      if (my.columns [6] != (uchar) '·')  strlcat (s_recd, " --show-type"  , LEN_RECD);
      if (my.columns [7] != (uchar) '·')  strlcat (s_recd, " --show-perms" , LEN_RECD);
      if (my.columns [8] != (uchar) '·')  strlcat (s_recd, " --show-drive" , LEN_RECD);
      if (my.columns [9] != (uchar) '·')  strlcat (s_recd, " --show-base"  , LEN_RECD);
      break;
   case OUTPUT_DETAIL   :
      strlcat (s_recd, "##    report option (key statistics) --detail", LEN_RECD);
      break;
   case OUTPUT_ANALYSIS :
      strlcat (s_recd, "##    report option (kitchen sink) --analysis", LEN_RECD);
      break;
   case OUTPUT_GYGES    :
      strlcat (s_recd, "##    report option (kitchen sink for gyges) --gyges", LEN_RECD);
      break;
   }
   /*---(output)-------------------------*/
   if (strcmp (s_recd, "") != 0) {
      printf ("%s\n", s_recd);
   }
   printf ("##    start   %-25.25s  depth   %-20.20s\n", t, s);
   printf ("##    regex   %-25.25s  ascii   %-20.20s\n", my.regex, my.ascii);
   printf ("##    types   %-25.25s  mimes   %-20.20s\n", my.types, my.mimes);
   printf ("##    sizes   %-25.25s  ages    %-20.20s\n", my.sizes, my.ages);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG__break             (char a_force)
{
   /*---(quick out)----------------------*/
   if (my.headers != 'y')            return 0;
   if (my.output  == OUTPUT_SILENT)  return 0;
   if (my.output  == OUTPUT_COUNT)   return 0;
   if (my.total %  5 != 0 && a_force != 'y')  return 0;
   /*---(shared)-------------------------*/
   printf ("\n");
   /*---(complete)-----------------------*/
   return 0;
}

char
RPTG__columns           (char a_force)
{
   /*---(prepare)------------------------*/
   strlcpy (s_recd, "", LEN_RECD);
   /*---(quick out)----------------------*/
   if (my.headers != 'y')            return 0;
   if (my.output  == OUTPUT_SILENT)  return 0;
   if (my.output  == OUTPUT_COUNT)   return 0;
   if (my.total % 25 != 0 && a_force != 'y')  return 0;
   /*---(shared)-------------------------*/
   printf ("\n");
   strlcat (s_recd, "## ", LEN_RECD);
   if (my.lineno == 'y') strlcat (s_recd, "line  ", LEN_RECD);
   /*---(individual)---------------------*/
   switch (my.output) {
   case OUTPUT_NORMAL   :
      strlcat (s_recd, "---fully qualified file---------------------------", LEN_RECD);
      break;
   case OUTPUT_PREFIX   :
      if (my.columns [0] != (uchar) '·')  strlcat (s_recd, "-  ---ext---  ", LEN_RECD);
      if (my.columns [1] != (uchar) '·')  strlcat (s_recd, "-day-  -  ", LEN_RECD);
      if (my.columns [2] != (uchar) '·')  strlcat (s_recd, "sz  appr  ---bytes-------  ", LEN_RECD);
      if (my.columns [3] != (uchar) '·')  strlcat (s_recd, "lvl  ", LEN_RECD);
      if (my.columns [4] != (uchar) '·')  strlcat (s_recd, "-  len  ", LEN_RECD);
      if (my.columns [5] != (uchar) '·')  strlcat (s_recd, "pos len  ", LEN_RECD);
      if (my.columns [6] != (uchar) '·')  strlcat (s_recd, "t  s  ", LEN_RECD);
      if (my.columns [7] != (uchar) '·')  strlcat (s_recd, "--own-  --grp-  o  s  ", LEN_RECD);
      if (my.columns [8] != (uchar) '·')  strlcat (s_recd, "dr  ", LEN_RECD);
      if (my.columns [9] != (uchar) '·')  strlcat (s_recd, "---name---------------------------------  ", LEN_RECD);
      strlcat (s_recd, "---fully qualified file---------------------------", LEN_RECD);
      break;
   case OUTPUT_DETAIL   :
      strlcat (s_recd, "mi ag sz lv nm ty st ---fully qualified file---------------------------", LEN_RECD);
      break;
   case OUTPUT_ANALYSIS :
      strlcat (s_recd, "len  ---name---------------------------------  pos nm  ", LEN_RECD);
      strlcat (s_recd, "lv t s   uid# u gid# g o s   ", LEN_RECD);
      strlcat (s_recd, "dr --inode- --dhode-   ", LEN_RECD);
      strlcat (s_recd, "---epoch-- days a   ", LEN_RECD);
      strlcat (s_recd, "sz ---bytes------- ---bcum--------   ", LEN_RECD);
      strlcat (s_recd, "-cnt-- -ccum-   ", LEN_RECD);
      strlcat (s_recd, "c ---ext---   pos len   ", LEN_RECD);
      strlcat (s_recd, "---fully qualified file-------------------------------", LEN_RECD);
      break;
   case OUTPUT_GYGES    :
      strlcat (s_recd, "len  ---name----------------------------------  pos  nm  ", LEN_RECD);
      strlcat (s_recd, "lv  t  s  uid#  u  gid#  g  o  s  ", LEN_RECD);
      strlcat (s_recd, "dr  --inode-  --dhode-  ", LEN_RECD);
      strlcat (s_recd, "---epoch--  days  a  ", LEN_RECD);
      strlcat (s_recd, "sz  ---bytes-------  ---bcum--------  ", LEN_RECD);
      strlcat (s_recd, "-cnt--  -ccum-  ", LEN_RECD);
      strlcat (s_recd, "c  ---ext---  pos  len  ", LEN_RECD);
      strlcat (s_recd, "---fully qualified file------------------------------- ", LEN_RECD);
      break;
   }
   /*---(output)-------------------------*/
   if (strcmp (s_recd, "") != 0) {
      printf ("%s\n", s_recd);
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
   uchar       x_mag       =  '·';
   long        x_size      =    0;
   /*---(prepare)------------------------*/
   strlcpy (s_recd, "", LEN_RECD);
   /*---(line number)--------------------*/
   if (my.headers == 'y')  {
      strlcat (s_recd, "   ", LEN_RECD);
   }
   /*---(line number)--------------------*/
   if (my.lineno == 'y')  {
      sprintf (t, "%-4d  ", my.total);
      strlcat (s_recd, t, LEN_RECD);
   }
   /*---(create lines)-------------------*/
   switch (my.output) {
   case OUTPUT_NORMAL   :
      sprintf (t, "%s", a_full);
      strlcat (s_recd, t, LEN_RECD);
      break;
   case OUTPUT_PREFIX   :
      if (my.columns [0] != (uchar) '·')  {
         sprintf (t, "%c  %-9.9s  ", a_data->cat, a_data->ext);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [1] != (uchar) '·') {
         sprintf (t, "%-5d  %c  "  , s_days, s_age);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [2] != (uchar) '·') {
         FILE_commas (a_data->bytes, s);
         if      (a_data->size >  15) { x_mag = 'P'; x_size = round (a_data->bytes / 1000.0 / 1000.0 / 1000.0 / 1000.0 / 1000.0); }
         else if (a_data->size >  12) { x_mag = 'T'; x_size = round (a_data->bytes / 1000.0 / 1000.0 / 1000.0 / 1000.0);          }
         else if (a_data->size >   9) { x_mag = 'G'; x_size = round (a_data->bytes / 1000.0 / 1000.0 / 1000.0);                   }
         else if (a_data->size >   6) { x_mag = 'M'; x_size = round (a_data->bytes / 1000.0 / 1000.0);                            }
         else if (a_data->size >   3) { x_mag = 'K'; x_size = round (a_data->bytes / 1000.0);                                     }
         else if (a_data->size ==  0) { x_mag = '-'; x_size = a_data->bytes; }
         else                         { x_mag = '·'; x_size = a_data->bytes; }
         if (a_data->size == 0) {
            strlcpy (v, " -"  , LEN_LABEL);
            strlcpy (u, "   " , LEN_LABEL);
         } else  {
            sprintf (v, "%2d", a_data->size);
            sprintf (u, "%3ld", x_size);
         }
         sprintf (t, "%2s  %3s%c  %15.15s  "      , v, u, x_mag, s);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [3] != (uchar) '·') {
         sprintf (t, "%-3d  "      , a_data->lvl);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [4] != (uchar) '·') {
         sprintf (t, "%c  %-3d  "  , a_data->ascii, a_data->len);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [5] != (uchar) '·') {
         sprintf (t, "%3d %3d  "   , s_pos, s_len);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [6] != (uchar) '·') {
         sprintf (t, "%c  %c  "   , a_data->type, a_data->stype);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [7] != (uchar) '·') {
         sprintf (t, "%-4d %d  %-4d %d  %d  %c  "   , a_data->uid, a_data->own, a_data->gid, a_data->grp, a_data->oth, a_data->super);
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [8] != (uchar) '·') {
         if (a_data->drive > 0)  sprintf (t, "%-2d  "   , a_data->drive);
         else                    sprintf (t, "-   ");
         strlcat (s_recd, t, LEN_RECD);
      }
      if (my.columns [9] != (uchar) '·') {
         sprintf (t, "%-40.40s  "   , a_data->name);
         strlcat (s_recd, t, LEN_RECD);
      }
      sprintf (t, "%s", a_full);
      strlcat (s_recd, t, LEN_RECD);
      break;
   case OUTPUT_DETAIL   :
      sprintf (t, "%c  %c  %d  %d  %c  %c  %c  ", a_data->cat, s_age, a_data->size, a_data->lvl, a_data->ascii, a_data->type, a_data->stype);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%s", a_full);
      strlcat (s_recd, t, LEN_RECD);
      break;
   case OUTPUT_ANALYSIS :
      sprintf (s, "%-40.40s", a_data->name);
      if (a_data->len > 39)  s [39] = '>';
      sprintf (t, "%3d  %s  %3d %c"        , a_data->len, s, s_pos, a_data->ascii);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %2d %c %c"           , a_data->lvl, a_data->type, a_data->stype);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %4d %d %4d %d %d %c" , a_data->uid, a_data->own, a_data->gid, a_data->grp, a_data->oth, a_data->super);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %2d %-8d %-8d"       , a_data->drive, a_data->inode, a_data->dnode);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %-10d %-4d %c"       , a_data->changed, s_days, s_age);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %d  %-15ld %-15ld"   , a_data->size, a_data->bytes, a_data->bcum);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %-6ld %-6ld"         , a_data->count, a_data->ccum);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %c %-9.9s"           , a_data->cat  , a_data->ext);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %3d %3d"             , s_pos, s_len);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "   %s", a_full);
      strlcat (s_recd, t, LEN_RECD);
      break;
   case OUTPUT_GYGES    :
      sprintf (s, "%-40.40s", a_data->name);
      if (a_data->len > 39)  s [39] = '>';
      sprintf (t, "%3d  %s  %3d  %c  "        , a_data->len, s, s_pos, a_data->ascii);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%2d  %c  %c  "           , a_data->lvl, a_data->type, a_data->stype);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%4d  %d  %4d  %d  %d  %c  " , a_data->uid, a_data->own, a_data->gid, a_data->grp, a_data->oth, a_data->super);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%2d  %-8d  %-8d  "       , a_data->drive, a_data->inode, a_data->dnode);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%-10d  %-4d  %c  "       , a_data->changed, s_days, s_age);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%d  %-15ld  %-15ld  "    , a_data->size, a_data->bytes, a_data->bcum);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%-6d  %-6d  "         , a_data->count, a_data->ccum);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%c  %-9.9s  "           , a_data->cat  , a_data->ext);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%3d  %3d  "             , s_pos, s_len);
      strlcat (s_recd, t, LEN_RECD);
      sprintf (t, "%s  ", a_full);
      strlcat (s_recd, t, LEN_RECD);
      break;
   case OUTPUT_SILENT   : case OUTPUT_COUNT    : default :
      return 0;
      break;
   } 
   if (strcmp (s_recd, "") != 0) {
      printf ("%s\n", s_recd);
   }
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
   if (my.headers != 'y')            return 0;
   if (my.output  == OUTPUT_SILENT)  return 0;
   if (my.output  == OUTPUT_COUNT)   return 0;
   /*---(columns)------------------------*/
   if (my.total % 25 >= 5)  RPTG__columns ('y');
   /*---(shared)-------------------------*/
   DRIVE_list ();
   printf ("\n");
   FILE_commas (my.total, s);
   FILE_commas (s_bytes, t);
   printf ("## total entries matched %s with %s bytes\n", s, t);
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
   rc = RPTG_perms_filter (a_data->uid, a_data->own, a_data->gid, a_data->grp, a_data->oth);
   DEBUG_DATA  yLOG_complex  ("perms"     , "%d, %4d %4d, %d %4d %4d, %d, %d", a_data->own, a_data->uid, my.uid, a_data->grp, a_data->gid, my.gid, a_data->oth, rc);
   if (rc <= 0)  return 0;
   /*---(regex)--------------------------*/
   if (my.regex_len != 0) {
      rc = RPTG_regex_filter (a_data->name);
      DEBUG_DATA  yLOG_complex  ("regex"     , "%s, %d", a_data->name, rc);
      if (rc <= 0)  return 0;
   } else {
      DEBUG_DATA  yLOG_note     ("no regex, continue filtering");
   }
   /*---(other filters)------------------*/
   rc = RPTG_filter_type  (a_data->type, a_data->stype);
   DEBUG_DATA  yLOG_complex  ("type"      , "%s, %c, %d", my.types, a_data->type , rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_mime  (a_data->cat, a_data->ext);
   DEBUG_DATA  yLOG_complex  ("mime"      , "%s, %s, %c, %s, %d", my.mimes, my.ext, a_data->cat, a_data->ext, rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_size  (a_data->size);
   DEBUG_DATA  yLOG_complex  ("mime"      , "%s, %d, %d", my.sizes, a_data->size , rc);
   if (rc <= 0)  return 0;
   s_days = (my.runtime - a_data->changed) / (24 * 60 * 60);
   rc = RPTG_filter_age   (s_days);
   DEBUG_DATA  yLOG_complex  ("age"       , "%s, %d, %d", my.ages , s_days       , rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_ascii (a_data->ascii);
   DEBUG_DATA  yLOG_complex  ("ascii"     , "%s, %c, %d", my.ascii, a_data->ascii, rc);
   if (rc <= 0)  return 0;
   rc = RPTG_filter_super (a_data->super);
   DEBUG_DATA  yLOG_complex  ("super"     , "%c, %c, %d", my.super, a_data->super, rc);
   if (rc <= 0)  return 0;
   /*---(serious)------------------------*/
   DEBUG_DATA  yLOG_char     ("a_serious" , a_serious);
   if (a_serious == 'y') {
      g_found = a_data;
      strlcpy (g_path, a_full, LEN_RECD);
   }
   /*---(report out)---------------------*/
   if (my.number >= 0) {
      if (my.total == my.number)  RPTG_driver (a_data, a_full);
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
   if (my.output == OUTPUT_COUNT)  printf ("%d\n", my.total);
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
   strlcpy (t, a_data->name, LEN_HUND);
   strldchg (t, '.', 0, LEN_HUND);
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
 *>    /+> FILE_commas (x_dir->bcum, x_cum);                                               <+/   <* 
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
   for (i = 1; i < n_mime; ++i) {
      if (g_mime [i].cat == MIME_DIR && strcmp (g_mime [i].ext, "dir") == 0) {
         x_dseen  += g_mime [i].n_seen;
         x_dkept  += g_mime [i].n_kept;
      } else {
         x_fseen  += g_mime [i].n_seen;
         x_fkept  += g_mime [i].n_kept;
      }
   }
   /*---(output)-------------------------*/
   printf ("database %s:\n", my.file_data);
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
   else if (strcmp (a_question, "output"        ) == 0) {
      sprintf (t, " %c   ", my.output);
      for (i = 0; i < strlen (my.columns); ++i) {
         sprintf (s, " %c", my.columns [i]);
         strlcat (t, s, LEN_DESC);
      }
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
   else if (strcmp (a_question, "s_recd"        ) == 0) {
      strlencode (s_recd, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "RPTG s_recd      : [%.79s]", s_recd);
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}





/*===============================[[ end-code ]]===============================*/
