/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"

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
 *
 * ========================== EXPLICITLY GPL LICENSED ==========================
 */

/*
 * METIS § wc4#· § change FILE_ functions to DB_ to keep consistent across programs       § N9J1jd §  · §
 * METIS § wr2·· § make sure yJOBS confirms database itself during audit                  § N9J6Dg §  · §
 * METIS § wv2·· § allow db name override for testing purposes                            § N9JLcO §  · §
 * METIS § wg4·· § option to sync existing data to new mime table                         § N9T6e0 §  · §
 *
 */



static char    s_name    [LEN_RECD]  = "";
static int     s_level   = 0;
static tPTRS  *s_stack   [LEN_HUND];


char
DB_init                 (void)
{
   int         i           =    0;
   for (i = 0; i < LEN_HUND; ++i)   s_stack [i] = NULL;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     header handling                          ----===*/
/*====================------------------------------------====================*/
static void  o___HEADER__________o () { return; }

char
DB__head_write_one      (FILE *a_file, char a_label [LEN_TERSE], int a_var)
{
   char        rc          =    0;
   rc = fwrite (&a_var, sizeof (int), 1, a_file);
   DEBUG_OUTP   yLOG_complex (a_label     , "%4drc, val=%d", rc, a_var);
   return rc;
}

char
DB__head_write          (FILE *a_file, char a_name [LEN_LABEL], char a_ver [LEN_SHORT], int a_nconf, int a_nmime, int a_ndrive, int a_nentry, char a_heart [LEN_DESC])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        t           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name)------------------------*/
   for (i = 0; i < LEN_LABEL; i++)  t [i] = ' ';
   strlcpy (t, a_name, LEN_LABEL);
   rc = fwrite (t, LEN_LABEL, 1, a_file);
   DEBUG_OUTP   yLOG_complex ("name"      , "%4d %s", rc, t);
   /*---(version)---------------------*/
   for (i = 0; i < LEN_SHORT; i++)  t [i] = ' ';
   strlcpy (t, a_ver  , LEN_SHORT);
   rc = fwrite (t, LEN_SHORT, 1, a_file);
   DEBUG_OUTP   yLOG_complex ("vernum"    , "%4d %s", rc, t);
   /*---(stats)-----------------------*/
   rc = DB__head_write_one (a_file, "confs"   , a_nconf);
   rc = DB__head_write_one (a_file, "mimes"   , a_nmime);
   rc = DB__head_write_one (a_file, "drives"  , a_ndrive);
   rc = DB__head_write_one (a_file, "entries" , a_nentry);
   /*---(heartbeat)-------------------*/
   for (i = 0; i < LEN_DESC;  i++)  t [i] = '·';
   strlcpy (t, a_heart    , LEN_DESC);
   rc = fwrite (t, LEN_DESC , 1, a_file);
   DEBUG_OUTP   yLOG_complex ("heart"     , "%4d %s", rc, t);
   /*---(force flush)-----------------*/
   fflush (a_file);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__head_read_one       (FILE *a_file, char a_label [LEN_TERSE], int *r_var)
{
   char        rc          =    0;
   int         a           =    0;
   rc = fread  (&a, sizeof (int), 1, a_file);
   DEBUG_INPT   yLOG_complex (a_label     , "%4d %d", rc, a);
   if (r_var != NULL)  *r_var = a;
   return 0;
}

char
DB__head_read           (FILE *a_file, char r_name [LEN_LABEL], char r_ver [LEN_SHORT], int *r_nconf, int *r_nmime, int *r_ndrive, int *r_nentry, char r_heart [LEN_DESC])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_HUND]  = "";
   int         a           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name)------------------------*/
   rc = fread  (&t, LEN_LABEL, 1, a_file);
   DEBUG_INPT   yLOG_complex ("name"      , "%4d %s", rc, t);
   if (r_name    != NULL)  strlcpy (r_name   , t, LEN_LABEL);
   /*---(version)---------------------*/
   rc = fread  (&t, LEN_SHORT, 1, a_file);
   DEBUG_INPT   yLOG_complex ("version"   , "%4d %s", rc, t);
   if (r_ver  != NULL)  strlcpy (r_ver , t, LEN_SHORT);
   /*---(stats)-----------------------*/
   rc = DB__head_read_one  (a_file, "confs"   , &a);
   if (r_nconf  != NULL)  *r_nconf  = a;
   rc = DB__head_read_one  (a_file, "mimes"   , &a);
   if (r_nmime  != NULL)  *r_nmime  = a;
   rc = DB__head_read_one  (a_file, "drives"  , &a);
   if (r_ndrive != NULL)  *r_ndrive = a;
   rc = DB__head_read_one  (a_file, "entries" , &a);
   if (r_nentry != NULL)  *r_nentry = a;
   /*---(heartbeat)-------------------*/
   rc = fread  (t, LEN_DESC , 1, a_file);
   DEBUG_INPT   yLOG_complex ("heart"     , "%4d %s", rc, t);
   if (r_heart   != NULL)  strlcpy (r_heart  , t, LEN_DESC);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                       file open/close                        ----===*/
/*====================------------------------------------====================*/
static void      o___FILE_______________o (void) {;}

/*> char                                                                              <* 
 *> FILE__check             (char *a_name, char a_mode)                               <* 
 *> {  /+---(locals)-----------+-----------+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    int         rci         =    0;                                                <* 
 *>    tSTAT       st;                                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_FILE   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(prepare)------------------------+/                                       <* 
 *>    ystrlcpy (s_name, "", LEN_RECD);                                               <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_FILE   yLOG_point   ("a_name"    , a_name);                              <* 
 *>    --rce;  if (a_name == NULL) {                                                  <* 
 *>       DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_FILE   yLOG_info    ("a_name"    , a_name);                              <* 
 *>    /+---(get the file information)-------+/                                       <* 
 *>    rci = lstat (a_name, &st);                                                     <* 
 *>    DEBUG_FILE   yLOG_value   ("lstat"     , rci);                                 <* 
 *>    --rce; if (rci < 0) {                                                          <* 
 *>       if (a_mode == 'r') {                                                        <* 
 *>          DEBUG_FILE   yLOG_note    ("file does not exist, can not read");         <* 
 *>          DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                <* 
 *>          return rce;                                                              <* 
 *>       }                                                                           <* 
 *>       DEBUG_FILE   yLOG_note    ("writing data to a brand new file");             <* 
 *>       DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                   <* 
 *>       return 0;                                                                   <* 
 *>    } else {                                                                       <* 
 *>       if (a_mode == 'w') {                                                        <* 
 *>          DEBUG_FILE   yLOG_note    ("file exists, writing over existing file");   <* 
 *>       }                                                                           <* 
 *>    }                                                                              <* 
 *>    /+---(check regular file)-------------+/                                       <* 
 *>    if (S_ISREG (st.st_mode)) {                                                    <* 
 *>       DEBUG_FILE   yLOG_note    ("refers to a regular file, perfect");            <* 
 *>    }                                                                              <* 
 *>    /+---(check symlink)------------------+/                                       <* 
 *>    else if (S_ISLNK (st.st_mode)) {                                               <* 
 *>       DEBUG_FILE   yLOG_note    ("file can not be a symlink to another file");    <* 
 *>       DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                   <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    else  {                                                                        <* 
 *>       if      (S_ISDIR (st.st_mode)) {                                            <* 
 *>          DEBUG_FILE   yLOG_note    ("file is a directory, not allowwed");         <* 
 *>       } else if (S_ISCHR (st.st_mode)) {                                          <* 
 *>          DEBUG_FILE   yLOG_note    ("file is a char dev, not allowwed");          <* 
 *>       } else if (S_ISBLK (st.st_mode)) {                                          <* 
 *>          DEBUG_FILE   yLOG_note    ("file is a block dev, not allowwed");         <* 
 *>       } else if (S_ISFIFO(st.st_mode)) {                                          <* 
 *>          DEBUG_FILE   yLOG_note    ("file is a fifo/pipe, not allowwed");         <* 
 *>       } else if (S_ISSOCK(st.st_mode)) {                                          <* 
 *>          DEBUG_FILE   yLOG_note    ("file is a socket, not allowwed");            <* 
 *>       } else  {                                                                   <* 
 *>          DEBUG_FILE   yLOG_note    ("file is not recognized, not allowwed");      <* 
 *>       }                                                                           <* 
 *>       DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                   <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(save name)----------------------+/                                       <* 
 *>    ystrlcpy (s_name, a_name, LEN_RECD);                                           <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_FILE   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

char
DB__open                (cchar a_name [LEN_PATH], char a_mode, FILE **r_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_mode      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("r_file"    , r_file);
   --rce;  if (r_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("*r_file"   , *r_file);
   --rce;  if (*r_file != NULL) {
      DEBUG_FILE   yLOG_note    ("file pointer in use, must leave alone");
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   /*---(set mode)-----------------------*/
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  switch (a_mode) {
   case 'r' :
      DEBUG_FILE   yLOG_note    ("open as read-binary");
      ystrlcpy (x_mode, "rb", LEN_LABEL);
      break;
   case 'w' :
      DEBUG_FILE   yLOG_note    ("open as write-binary");
      ystrlcpy (x_mode, "wb", LEN_LABEL);
      break;
   default  :
      DEBUG_FILE   yLOG_note    ("mode not understood");
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(turn back stdout)---------------*/
   if (strcmp (a_name, "stdout") == 0) {
      *r_file = stdout;
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check)--------------------------*/
   /*> rc = FILE__check (a_name, a_mode);                                             <* 
    *> DEBUG_FILE   yLOG_value   ("check"     , rc);                                  <* 
    *> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(open)---------------------------*/
   *r_file = fopen (a_name, x_mode);
   DEBUG_FILE   yLOG_point   ("*r_file"   , *r_file);
   --rce;  if (*r_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_note    ("file successfullly openned");
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__close               (FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file == NULL) {
      DEBUG_FILE   yLOG_note    ("file pointer grounded, nothing to do");
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flush)--------------------------*/
   fflush (*b_file);
   /*---(turn back stdout)---------------*/
   if (*b_file == stdout) {
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(close)--------------------------*/
   rc = fclose (*b_file);
   DEBUG_FILE   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(permissions)--------------------*/
   chmod (s_name, 0755);
   /*---(ground)-------------------------*/
   ystrlcpy (s_name, "", LEN_RECD);
   *b_file = NULL;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     read/write of drives                     ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVES_____________o (void) {;}

char
DB__drive_write    (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   int         x_bytes     =    0;
   tDRIVE     *x_drive     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write drive list)---------------*/
   x_drive = h_drive;
   while (x_drive != NULL) {
      DEBUG_INPT   yLOG_complex ("drive"     , "%2di, %p, %s", i, x_drive, x_drive->device);
      x_bytes = fwrite (x_drive  , sizeof (tDRIVE), 1, a_file);
      DEBUG_OUTP   yLOG_value   ("write"     , x_bytes);
      x_drive = x_drive->m_next;
      ++i;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__drive_read     (FILE *a_file, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   int         x_bytes     =    0;
   tDRIVE      x_temp;
   tDRIVE     *x_drive     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get count)----------------------*/
   --rce;  for (i = 0; i < n; ++i) {
      x_bytes = fread  (&x_temp   , sizeof (tDRIVE), 1, a_file);
      DEBUG_INPT   yLOG_value   ("read"      , x_bytes);
      rc = DRIVE_manual (&x_drive, x_temp.ref, x_temp.host, x_temp.serial, x_temp.device, x_temp.mpoint, x_temp.type, x_temp.size, x_temp.written);
      DEBUG_INPT   yLOG_complex ("manual"    , "%2di, %4drc, %p", i, rc, x_drive);
      if (rc < 0 || x_drive == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    read/write entries                        ----===*/
/*====================------------------------------------====================*/
static void      o___ENTRIES____________o (void) {;}

char         /*===[[ write dir to binary file ]]==========[ ------ [ ------ ]=*/
DB__entry_write    (FILE *a_file, tENTRY *a_entry, int *a_count)
{  /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_bytes     =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_point   ("a_entry"   , a_entry);
   --rce;  if (a_entry == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_point   ("a_count"   , a_count);
   --rce;  if (a_count == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(output)-------------------------*/
   clearerr (a_file);
   x_bytes = fwrite (a_entry, sizeof (tENTRY), 1, a_file);
   ++(*a_count);
   DEBUG_OUTP   yLOG_complex ("entry"     , "%4d, %d, %d, %s", *a_count, x_bytes, ferror (a_file), a_entry->name);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__entry_read          (FILE *a_file, int *a_count)
{  /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_bytes     =    0;
   tPTRS      *x_new       = NULL;
   tENTRY     *x_data      = NULL;
   int         x_tries     = NULL;
   tPTRS      *x_parent    = NULL;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_count"   , a_count);
   --rce;  if (a_count == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(malloc new data)----------------*/
   while (x_data == NULL) {
      if (++x_tries > 3)  break;
      x_data = (tENTRY*) malloc (sizeof (tENTRY));
   }
   DEBUG_INPT   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_INPT   yLOG_point   ("x_data"    , x_data);
   --rce;  if (x_data == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read data)----------------------*/
   clearerr (a_file);
   x_bytes = fread (x_data, sizeof (tENTRY), 1, a_file);
   DEBUG_INPT   yLOG_complex ("entry"     , "%4d, %d, %d, %d, %s", *a_count, x_bytes, ferror (a_file), errno, x_data->name);
   DEBUG_INPT   yLOG_value   ("x_bytes"   , x_bytes);
   --rce;  if (x_bytes <= 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("name"      , x_data->name);
   /*---(set level)----------------------*/
   DEBUG_INPT   yLOG_complex ("levels"    , "%ds, %d", s_level, x_data->lvl);
   /*> for (i = 0; i < LEN_HUND; ++i) {                                                                                                  <* 
    *>    if (s_stack [i] != NULL)   DEBUG_INPT   yLOG_complex ("s_stack"   , "%2d, %p, %s", i, s_stack [i], s_stack [i]->data->name);   <* 
    *> }                                                                                                                                 <*/
   for (i = x_data->lvl; i < LEN_HUND; ++i)   s_stack [i] = NULL;
   /*> for (i = 0; i < LEN_HUND; ++i) {                                                                                                  <* 
    *>    if (s_stack [i] != NULL)   DEBUG_INPT   yLOG_complex ("s_stack"   , "%2d, %p, %s", i, s_stack [i], s_stack [i]->data->name);   <* 
    *> }                                                                                                                                 <*/
   s_level = x_data->lvl;
   DEBUG_INPT   yLOG_value   ("s_level"   , s_level);
   /*---(set parent)---------------------*/
   if (x_data->lvl == 0)  x_parent = NULL;
   else                   x_parent = s_stack [s_level - 1];
   DEBUG_INPT   yLOG_point   ("x_parent"  , x_parent);
   --rce;  if (s_level != 0 && x_parent == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create new entry)---------------*/
   if (x_parent == NULL)  rc = ENTRY_root   (&x_new, h_drive);
   else                   rc = ENTRY_normal (&x_new, x_parent);
   DEBUG_INPT   yLOG_value   ("new"       , rc);
   DEBUG_INPT   yLOG_point   ("x_new"     , x_new);
   --rce;  if (rc < 0 || x_new == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(swap data elements)-------------*/
   free (x_new->data);
   x_new->data = x_data;
   /*---(update count)-------------------*/
   ++(*a_count);
   /*---(handle stack)-------------------*/
   DEBUG_INPT   yLOG_char    ("type"      , x_data->type);
   if (x_data->type == ENTRY_DIR) {
      s_stack [s_level] = x_new;
      DEBUG_INPT   yLOG_complex ("s_stack"   , "saving %p in position %d", x_new, s_level);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    read/write directories                    ----===*/
/*====================------------------------------------====================*/
static void      o___DIRECTORIES________o (void) {;}

char         /*===[[ write dir to binary file ]]==========[ ------ [ ------ ]=*/
DB__dir_write      (FILE *a_file, tPTRS *a_parent, int *a_count)
{  /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPTRS      *x_entry     = NULL;
   tENTRY     *x_data      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_parent"  , a_parent);
   --rce;  if (a_parent == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_value   ("children"  , a_parent->nchild);
   --rce;  if (a_parent->data == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("dir_name"  , a_parent->data->name);
   /*---(output)-------------------------*/
   x_entry = a_parent->c_head;
   DEBUG_OUTP   yLOG_point   ("head"      , x_entry);
   while (x_entry != NULL) {
      x_data = x_entry->data;
      if (x_data != NULL) {
         DEBUG_OUTP   yLOG_complex ("entry"     , "%p, %s, %c, %c", x_data, x_data->name, x_data->type, x_data->stype);
         DB__entry_write (a_file, x_data, a_count);
         if   (x_data->type == ENTRY_DIR) {
            DB__dir_write (a_file, x_entry, a_count);
         }
      }
      x_entry = x_entry->s_next;
      DEBUG_OUTP   yLOG_point   ("next"      , x_entry);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           database                           ----===*/
/*====================------------------------------------====================*/
static void      o___DATABASE___________o (void) {;}

/*> char         /+===[[ write dir to binary file ]]==========[ ------ [ ------ ]=+/   <* 
 *> FWRITE_dir         (                                                               <* 
 *>       /+---(formal parameters)+-------------+---------------------------------+/   <* 
 *>       FILE       *a_file      ,             /+ destination file               +/   <* 
 *>       tPTRS      *a_ptrs      ,             /+ parent of entries              +/   <* 
 *>       int        *a_count     )             /+ current line count             +/   <* 
 *> {  /+---(locals)-----------+-----------+-+/                                        <* 
 *>    tPTRS      *x_ptrs      = NULL;                                                 <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_OUTP   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    DEBUG_OUTP   yLOG_point   ("a_ptrs"    , a_ptrs);                               <* 
 *>    /+---(defense)------------------------+/                                        <* 
 *>    if (a_ptrs == NULL) {                                                           <* 
 *>       DEBUG_OUTP   yLOG_note    ("hit a null ptrs pointer, FAIL");                 <* 
 *>       DEBUG_OUTP   yLOG_exit    (__FUNCTION__);                                    <* 
 *>       return -1;                                                                   <* 
 *>    }                                                                               <* 
 *>    DEBUG_OUTP   yLOG_value   ("children"  , a_ptrs->nchild);                       <* 
 *>    if (a_ptrs->data == NULL) {                                                     <* 
 *>       DEBUG_OUTP   yLOG_note    ("hit a null data pointer, FAIL");                 <* 
 *>       DEBUG_OUTP   yLOG_exit    (__FUNCTION__);                                    <* 
 *>       return -1;                                                                   <* 
 *>    }                                                                               <* 
 *>    DEBUG_OUTP   yLOG_info    ("dir_name"  , a_ptrs->data->name);                   <* 
 *>    /+---(output)-------------------------+/                                        <* 
 *>    x_ptrs = a_ptrs->c_head;                                                        <* 
 *>    DEBUG_OUTP   yLOG_point   ("head"      , x_ptrs);                               <* 
 *>    while (x_ptrs != NULL) {                                                        <* 
 *>       if (x_ptrs->data != NULL) {                                                  <* 
 *>          fwrite (x_ptrs->data, sizeof (tENTRY), 1, a_file);                        <* 
 *>          ++(*a_count);                                                             <* 
 *>          if   (x_ptrs->data->type == ENTRY_DIR &&                                  <* 
 *>                strchr (".)", x_ptrs->data->stype) != NULL) {                       <* 
 *>             DEBUG_OUTP   yLOG_note    ("directory, recurse");                      <* 
 *>             FWRITE_dir (a_file, x_ptrs, a_count);                                  <* 
 *>          }                                                                         <* 
 *>       }                                                                            <* 
 *>       x_ptrs = x_ptrs->s_next;                                                     <* 
 *>       DEBUG_OUTP   yLOG_point   ("next"      , x_ptrs);                            <* 
 *>    }  /+---(done)------------------------+/                                        <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_OUTP   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
DB_write           (char *a_name, int *a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_db        = NULL;
   int         x_count     = 0;
   tDRIVE     *x_drive     = NULL;
   long        x_end       =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("h_ptrs"    , h_ptrs);
   --rce;  if (h_ptrs == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   rc = DB__open  (a_name, 'w', &x_db);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   DEBUG_OUTP   yLOG_point   ("x_db"      , x_db);
   --rce;  if (rc < 0 || x_db == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write header)-------------------*/
   rc = DB__head_write  (x_db, P_BASENAME, P_VERNUM, g_nconf, g_nmime, g_ndrive, g_nptrs, my.heartbeat);
   DEBUG_OUTP   yLOG_value   ("header"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write conf)---------------------*/
   rc = CONF_db_write (x_db);
   DEBUG_OUTP   yLOG_value   ("conf"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write mime)---------------------*/
   rc = MIME_db_write (x_db);
   DEBUG_OUTP   yLOG_value   ("mime"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write drives)-------------------*/
   rc = DB__drive_write (x_db);
   DEBUG_OUTP   yLOG_value   ("drives"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write root)---------------------*/
   rc = DB__entry_write (x_db, h_ptrs->data, &x_count);
   DEBUG_OUTP   yLOG_value   ("root"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write entries)------------------*/
   rc = DB__dir_write (x_db, h_ptrs, &x_count);
   DEBUG_OUTP   yLOG_value   ("dirs"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&x_db);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   DEBUG_OUTP   yLOG_point   ("x_db"      , x_db);
   --rce;  if (rc < 0 || x_db != NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   x_end  = time (NULL);
   if (my.run_as != IAM_UHELIOS)   printf ("# ran for %ld secs\n", x_end - my.runtime);
   DB_commas (x_count, t);
   if (my.run_as != IAM_UHELIOS)   printf ("# wrote %s recs\n", t);
   /*---(save back)----------------------*/
   if (a_count != NULL)  *a_count = x_count;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



char         /*===[[ read entries from database ]]========[ ------ [ ------ ]=*/
DB_read            (char *a_name, int *a_count)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_db        = NULL;          /* file pointer                   */
   tPTRS      *x_dir       = NULL;
   char        x_name      [LEN_LABEL] = "";
   char        x_ver       [LEN_TERSE] = "";
   char        x_heart     [LEN_HUND]  = "";
   int         x_nconf, x_nmime, x_ndrive, x_nentry;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(purges)-------------------------*/
   rc = ENTRY__purge ();
   rc = DRIVE__purge ();
   rc = CONF__purge ();
   rc = MIME__purge ();
   /*---(open)---------------------------*/
   rc = DB__open  (a_name, 'r', &x_db);
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   DEBUG_INPT   yLOG_point   ("x_db"      , x_db);
   --rce;  if (rc < 0 || x_db == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read header)--------------------*/
   rc = DB__head_read   (x_db, x_name, x_ver, &x_nconf, &x_nmime, &x_ndrive, &x_nentry, x_heart);
   DEBUG_OUTP   yLOG_value   ("header"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get conf)-----------------------*/
   rc = CONF_db_read (x_db, x_nconf);
   DEBUG_INPT   yLOG_complex ("conf"      , "%4drc, %dn", rc, x_nconf);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(get mime)-----------------------*/
   rc = MIME_db_read (x_db, x_nmime);
   DEBUG_INPT   yLOG_complex ("mime"      , "%4drc, %4de, %4da", rc, x_nmime, g_nmime);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(get drives)---------------------*/
   rc = DB__drive_read (x_db, x_ndrive);
   DEBUG_INPT   yLOG_complex ("drives"    , "%4drc, %4de, %4da", rc, x_ndrive, g_ndrive);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(process entries)----------------*/
   --rce;  while (1) {
      rc = DB__entry_read (x_db, a_count);
      if (rc < 0)  break;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&x_db);
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   DEBUG_INPT   yLOG_point   ("x_db"      , x_db);
   --rce;  if (rc < 0 || x_db != NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB_commas        (llong a_number, char *a_string)
{
   if (a_number == 0)  ystrlcpy (a_string, "-", LEN_LABEL);
   else  ystrl4comma (a_number, a_string, 0, 'c', '-', LEN_LABEL);
   return 0;
}



/*===============================[[ end-code ]]===============================*/
