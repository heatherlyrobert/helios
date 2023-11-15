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



/*====================------------------------------------====================*/
/*===----                     type filter control                      ----===*/
/*====================------------------------------------====================*/
static void      o___TYPE____________________o (void) {;}

char FILTER_type_all        (void) { ystrlcpy (my.types, ENTRY_ALL , LEN_LABEL); return 0; }
char FILTER_type_none       (void) { ystrlcpy (my.types, "········", LEN_LABEL); return 0; }

char
FILTER_type_pos        (char *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 2)  return -2;
   if (strcmp (my.types, ENTRY_ALL) == 0)  FILTER_type_none ();
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
FILTER_type_neg        (char *a_option)
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
FILTER_type_direct     (char *a_type)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_type == NULL)  return -1;
   x_len = strlen (a_type);
   if (strlen (a_type) < 1)  return -2;
   FILTER_type_none ();
   for (i = 0; i < strlen (a_type); ++i) {
      sprintf (t, "--%c", a_type [i]);
      FILTER_type_pos (t);
   }
   return 0;
}

char
FILTER_by_type          (char a_type, char a_stype)
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

char FILTER_mime_all   (void) { ystrlcpy (my.mimes, MIME_ALL         , LEN_LABEL); return 0; }
char FILTER_mime_none  (void) { ystrlcpy (my.mimes, "···············", LEN_LABEL); return 0; }

char
FILTER_mime_pos   (char *a_option)
{
   int         x_len       =     0;
   int         x_ch        =   '-';
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.mimes, MIME_ALL)  == 0)  FILTER_mime_none ();
   x_ch = a_option [2];
   if      (strcmp (a_option + 2, "media") == 0)  x_ch = MIME_MEDIA;
   else if (strcmp (a_option + 2, "work" ) == 0)  x_ch = MIME_WORK;
   else if (strcmp (a_option + 2, "temp" ) == 0)  x_ch = MIME_TEMP;
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
FILTER_mime_neg   (char *a_option)
{
   int         x_len       =     0;
   int         x_ch        =   '-';
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   x_ch = a_option [4];
   if      (strcmp (a_option + 4, "media") == 0)  x_ch = MIME_MEDIA;
   else if (strcmp (a_option + 4, "work" ) == 0)  x_ch = MIME_WORK;
   else if (strcmp (a_option + 4, "temp" ) == 0)  x_ch = MIME_TEMP;
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
FILTER_mime_direct     (char *a_mimes)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_mimes == NULL)  return -1;
   x_len = strlen (a_mimes);
   if (strlen (a_mimes) < 1)  return -2;
   FILTER_mime_none ();
   for (i = 0; i < strlen (a_mimes); ++i) {
      sprintf (t, "--%c", a_mimes [i]);
      FILTER_mime_pos (t);
   }
   return 0;
}

char
FILTER_by_mime          (char a_cat, char *a_ext)
{
   char          rc        =    0;
   char          x_ext     [LEN_LABEL] = "";
   char         *p         = NULL;
   rc = strcmp (my.ext, "");
   DEBUG_DATA  yLOG_complex  ("mime check", "%s, %s, %c, %s, %d", my.mimes, my.ext, a_cat, a_ext, rc);
   if (rc != 0) {
      DEBUG_DATA  yLOG_note     ("must test extension");
      sprintf (x_ext, " %s ", a_ext);
      p = strstr (my.ext, x_ext);
      DEBUG_DATA  yLOG_complex  ("check"     , "%s, %s, %p", my.ext, a_ext, p);
      if (p != NULL) {
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

char FILTER_size_all   (void) { ystrlcpy (my.sizes, SIZES_ALL             , LEN_LABEL); return 0; }
char FILTER_size_none  (void) { ystrlcpy (my.sizes, "···················" , LEN_LABEL); return 0; }

char
FILTER_size_pos         (char *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.sizes, SIZES_ALL) == 0)  FILTER_size_none ();
   switch (a_option [2]) {
   case 'z'        :
      my.sizes [0]  = '0';
      break;
   case 'b'        :
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
FILTER_size_neg         (char *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   switch (a_option [4]) {
   case 'z'        :
      my.sizes [0]  = (uchar) '·';
      break;
   case 'b'        :
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
FILTER_size_direct      (char *a_sizes)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_sizes == NULL)  return -1;
   x_len = strlen (a_sizes);
   if (strlen (a_sizes) < 1)  return -2;
   FILTER_size_none ();
   for (i = 0; i < strlen (a_sizes); ++i) {
      sprintf (t, "--%c", a_sizes [i]);
      FILTER_size_pos (t);
   }
   return 0;
}

char
FILTER_by_size          (char a_size)
{
   if (my.sizes [a_size] != (uchar) '·')  return 1;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        age filter                            ----===*/
/*====================------------------------------------====================*/
static void      o___AGE_____________________o (void) {;}

char FILTER_age_all    (void) { ystrlcpy (my.ages , AGES_ALL , LEN_LABEL); return 0; }
char FILTER_age_none   (void) { ystrlcpy (my.ages , "·······", LEN_LABEL); return 0; }

char
FILTER_age_pos         (char *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.ages, AGES_ALL)  == 0)  FILTER_age_none ();
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
FILTER_age_neg          (char *a_option)
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
FILTER_age_direct      (char *a_ages)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_TERSE] = "";
   if (a_ages == NULL)  return -1;
   x_len = strlen (a_ages);
   if (strlen (a_ages) < 1)  return -2;
   FILTER_age_none ();
   for (i = 0; i < strlen (a_ages); ++i) {
      sprintf (t, "--%c", a_ages [i]);
      FILTER_age_pos (t);
   }
   return 0;
}

char
FILTER_by_age           (long a_days)
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

char FILTER_ascii_all   (void) { ystrlcpy (my.ascii, ASCII_ALL, LEN_LABEL); return 0; }
char FILTER_ascii_none  (void) { ystrlcpy (my.ascii, "······" , LEN_LABEL); return 0; }

char
FILTER_ascii_pos       (char *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) < 1)  return -2;
   if (strcmp (my.ascii, ASCII_ALL)  == 0)  FILTER_ascii_none ();
   if (strcmp (a_option, "--badname") == 0) {
      strcpy (my.ascii, "·A+#>X");
      return 0;
   }
   switch (a_option [2]) {
   case 'b'  :  my.ascii [0]  = ASCII_BASIC;  break;
   case 'u'  :  my.ascii [1]  = ASCII_UPPER;  break;
   case 'p'  :  my.ascii [2]  = ASCII_PUNCT;  break;
   case 'e'  :  my.ascii [3]  = ASCII_EXTEND; break;
   case 's'  :  my.ascii [4]  = ASCII_SPACE;  break;
   case 'c'  :  my.ascii [5]  = ASCII_CRAZY;  break;
   }
   return 0;
}

char
FILTER_ascii_neg        (char *a_option)
{
   int         x_len       =     0;
   if (a_option == NULL)  return -1;
   x_len = strlen (a_option);
   if (strlen (a_option) <= 4)  return -2;
   switch (a_option [4]) {
   case 'b'  :  my.ascii [0]  = (uchar) '·';  break;
   case 'u'  :  my.ascii [1]  = (uchar) '·';  break;
   case 'p'  :  my.ascii [2]  = (uchar) '·';  break;
   case 'e'  :  my.ascii [3]  = (uchar) '·';  break;
   case 's'  :  my.ascii [4]  = (uchar) '·';  break;
   case 'c'  :  my.ascii [5]  = (uchar) '·';  break;
   }
   return 0;
}

char
FILTER_ascii_direct    (char *a_ascii)
{
   char        i           =     0;
   int         x_len       =     0;
   char        t           [LEN_LABEL] = "";
   if (a_ascii == NULL)  return -1;
   x_len = strlen (a_ascii);
   if (strlen (a_ascii) < 1)  return -2;
   FILTER_ascii_none ();
   for (i = 0; i < strlen (a_ascii); ++i) {
      switch (a_ascii [i]) {
      case ASCII_BASIC  :  ystrlcpy (t, "--basic"  , LEN_LABEL);  break;
      case ASCII_UPPER  :  ystrlcpy (t, "--upper"  , LEN_LABEL);  break;
      case ASCII_PUNCT  :  ystrlcpy (t, "--punct"  , LEN_LABEL);  break; 
      case ASCII_EXTEND :  ystrlcpy (t, "--extend" , LEN_LABEL);  break;
      case ASCII_SPACE  :  ystrlcpy (t, "--space"  , LEN_LABEL);  break;
      case ASCII_CRAZY  :  ystrlcpy (t, "--crazy"  , LEN_LABEL);  break;
      }
      FILTER_ascii_pos (t);
   }
   return 0;
}

char
FILTER_by_ascii         (char a_ascii)
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
/*===----                       permission filter                      ----===*/
/*====================------------------------------------====================*/
static void      o___PERMS___________________o (void) {;}

char
FILTER__perms           (char *a_func, char a_type, int a_uid, int a_gid, char a_own, char a_grp, char a_oth, char *a_reason)
{
   /*---(local variables)--+-----------+-*/
   uchar       x_allowed   =  '·';
   uchar       x_mask      = 0x04;
   int         x_uid       = my.uid;
   int         x_gid       = my.gid;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (a_func);
   /*---(prepare)------------------------*/
   switch (a_type) {
   case 'f'  : x_mask      = 0x04;   break;
   case 'd'  : x_mask      = 0x05;   break;
   }
   DEBUG_RPTG   yLOG_value   ("x_mask"    , x_mask);
   /*---(security)-----------------------*/
   if (my.uid == 0) {
      if (my.r_uid >= 0) x_uid = my.r_uid;
      if (my.r_gid >= 0) x_gid = my.r_gid;
   }
   DEBUG_RPTG   yLOG_complex ("owner"     , "%4do, %4dro, %4dxo", my.uid, my.r_uid, x_uid);
   DEBUG_RPTG   yLOG_complex ("running"   , "%4dg, %4drg, %4dxg", my.gid, my.r_gid, x_gid);
   /*---(debug)--------------------------*/
   DEBUG_RPTG   yLOG_complex ("owner"     , "%2x, %2x, %2x", a_own, x_mask, a_own & x_mask);
   DEBUG_RPTG   yLOG_complex ("group"     , "%2x, %2x, %2x", a_grp, x_mask, a_grp & x_mask);
   DEBUG_RPTG   yLOG_complex ("other"     , "%2x, %2x, %2x", a_oth, x_mask, a_oth & x_mask);
   /*---(can you see it)-----------------*/
   if (x_uid == 0)                                         x_allowed = 'r';
   else if ((a_oth & x_mask) == x_mask)                    x_allowed = 'o';
   else if (a_gid == x_gid && (a_grp & x_mask) == x_mask)  x_allowed = 'g';
   else if (a_uid == x_uid && (a_own & x_mask) == x_mask)  x_allowed = 'u';
   DEBUG_RPTG   yLOG_char    ("x_allowed" , x_allowed);
   /*---(save-back)----------------------*/
   if (a_reason != NULL)  *a_reason = x_allowed;
   /*---(decide)-------------------------*/
   if (x_allowed != (uchar) '·') {
      DEBUG_RPTG   yLOG_exit    (a_func);
      return 1;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (a_func);
   return 0;
}

char
FILTER_by_perms         (int a_uid, int a_gid, char a_own, char a_grp, char a_oth, char *r_reason)
{
   return FILTER__perms (__FUNCTION__, 'f', a_uid, a_gid, a_own, a_grp, a_oth, r_reason);
}

char
FILTER_by_dir           (int a_uid, int a_gid, char a_own, char a_grp, char a_oth, char *r_reason)
{
   return FILTER__perms (__FUNCTION__, 'd', a_uid, a_gid, a_own, a_grp, a_oth, r_reason);
}



/*====================------------------------------------====================*/
/*===----                         regex filter                         ----===*/
/*====================------------------------------------====================*/
static void      o___REGEX___________________o (void) {;}

char
FILTER__regex           (uchar *a_regex)
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
   --rce;  if (x_len <= 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
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
FILTER_by_regex         (uchar *a_string)
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
   if (strcmp (my.regex, "") == 0) {
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
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


