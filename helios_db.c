/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */


/*
 * METIS § wc4·· § change FILE_ functions to DB_ to keep consistent across programs       § N9J1jd §  · §
 *
 *
 *
 *
 */



static char    s_name    [LEN_RECD]  = "";
static int     s_level   = 0;
static tPTRS  *s_stack   [99];



/*====================------------------------------------====================*/
/*===----                       file open/close                        ----===*/
/*====================------------------------------------====================*/
static void      o___FILE____________________o (void) {;}

char
FILE__check             (char *a_name, char a_mode)
{  /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rci         =    0;
   tSTAT       st;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   ystrlcpy (s_name, "", LEN_RECD);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   /*---(get the file information)-------*/
   rci = lstat (a_name, &st);
   DEBUG_FILE   yLOG_value   ("lstat"     , rci);
   --rce; if (rci < 0) {
      if (a_mode == 'r') {
         DEBUG_FILE   yLOG_note    ("file does not exist, can not read");
         DEBUG_FILE   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_FILE   yLOG_note    ("writing data to a brand new file");
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return 0;
   } else {
      if (a_mode == 'w') {
         DEBUG_FILE   yLOG_note    ("file exists, writing over existing file");
      }
   }
   /*---(check regular file)-------------*/
   if (S_ISREG (st.st_mode)) {
      DEBUG_FILE   yLOG_note    ("refers to a regular file, perfect");
   }
   /*---(check symlink)------------------*/
   else if (S_ISLNK (st.st_mode)) {
      DEBUG_FILE   yLOG_note    ("file can not be a symlink to another file");
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   else  {
      if      (S_ISDIR (st.st_mode)) {
         DEBUG_FILE   yLOG_note    ("file is a directory, not allowwed");
      } else if (S_ISCHR (st.st_mode)) {
         DEBUG_FILE   yLOG_note    ("file is a char dev, not allowwed");
      } else if (S_ISBLK (st.st_mode)) {
         DEBUG_FILE   yLOG_note    ("file is a block dev, not allowwed");
      } else if (S_ISFIFO(st.st_mode)) {
         DEBUG_FILE   yLOG_note    ("file is a fifo/pipe, not allowwed");
      } else if (S_ISSOCK(st.st_mode)) {
         DEBUG_FILE   yLOG_note    ("file is a socket, not allowwed");
      } else  {
         DEBUG_FILE   yLOG_note    ("file is not recognized, not allowwed");
      }
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(save name)----------------------*/
   ystrlcpy (s_name, a_name, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__open                (FILE **a_file, char *a_name, char a_mode)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_mode      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("*a_file"   , *a_file);
   --rce;  if (*a_file != NULL) {
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
      *a_file = stdout;
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check)--------------------------*/
   rc = FILE__check (a_name, a_mode);
   DEBUG_FILE   yLOG_value   ("check"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   *a_file = fopen (a_name, x_mode);
   DEBUG_FILE   yLOG_point   ("*a_file"   , *a_file);
   --rce;  if (*a_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_note    ("file successfullly openned");
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__close               (FILE **a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yLOG_point   ("*a_file"   , *a_file);
   --rce;  if (*a_file == NULL) {
      DEBUG_FILE   yLOG_note    ("file pointer grounded, nothing to do");
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flush)--------------------------*/
   fflush (*a_file);
   /*---(turn back stdout)---------------*/
   if (*a_file == stdout) {
      DEBUG_FILE   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(close)--------------------------*/
   rc = fclose (*a_file);
   DEBUG_FILE   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(permissions)--------------------*/
   chmod (s_name, 0755);
   /*---(ground)-------------------------*/
   ystrlcpy (s_name, "", LEN_RECD);
   *a_file = NULL;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}




/*====================------------------------------------====================*/
/*===----                     read/write of drives                     ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVES__________________o (void) {;}

char
WRITE__drives      (FILE *a_file)
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
   /*---(write count)--------------------*/
   DEBUG_OUTP   yLOG_value   ("n_drive"   , n_drive);
   x_bytes = fwrite (&n_drive, sizeof (uchar), 1, a_file);
   DEBUG_OUTP   yLOG_value   ("write"     , x_bytes);
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
READ__drives       (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   uchar       n           =    0;
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
   x_bytes = fread  (&n, sizeof (uchar), 1, a_file);
   DEBUG_INPT   yLOG_value   ("read"      , x_bytes);
   DEBUG_INPT   yLOG_value   ("n"         , n);
   --rce;  if (n < 0 || n > 30) {
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
static void      o___ENTRIES_________________o (void) {;}

char         /*===[[ write dir to binary file ]]==========[ ------ [ ------ ]=*/
WRITE__entry       (FILE *a_file, tENTRY *a_entry, int *a_count)
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
READ__entry             (FILE *a_file, int *a_count)
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
   for (i = 0; i < 99; ++i) {
      if (s_stack [i] != NULL)   DEBUG_INPT   yLOG_complex ("s_stack"   , "%2d, %p, %s", i, s_stack [i], s_stack [i]->data->name);
   }
   for (i = x_data->lvl; i < 99; ++i)   s_stack [i] = NULL;
   for (i = 0; i < 99; ++i) {
      if (s_stack [i] != NULL)   DEBUG_INPT   yLOG_complex ("s_stack"   , "%2d, %p, %s", i, s_stack [i], s_stack [i]->data->name);
   }
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
static void      o___DIRECTORIES_____________o (void) {;}

char         /*===[[ write dir to binary file ]]==========[ ------ [ ------ ]=*/
WRITE__dir         (FILE *a_file, tPTRS *a_parent, int *a_count)
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
         WRITE__entry (a_file, x_data, a_count);
         if   (x_data->type == ENTRY_DIR) {
            WRITE__dir (a_file, x_entry, a_count);
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
static void      o___DATABASE________________o (void) {;}

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
WRITE_all          (char *a_name, int *a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_file      = NULL;
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
   rc = DB__open  (&x_file, a_name, 'w');
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   DEBUG_OUTP   yLOG_point   ("x_file"    , x_file);
   --rce;  if (rc < 0 || x_file == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write drives)-------------------*/
   rc = WRITE__drives (x_file);
   DEBUG_OUTP   yLOG_value   ("drives"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write root)---------------------*/
   rc = WRITE__entry (x_file, h_ptrs->data, &x_count);
   DEBUG_OUTP   yLOG_value   ("root"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write entries)------------------*/
   rc = WRITE__dir (x_file, h_ptrs, &x_count);
   DEBUG_OUTP   yLOG_value   ("dirs"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&x_file);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   DEBUG_OUTP   yLOG_point   ("x_file"    , x_file);
   --rce;  if (rc < 0 || x_file != NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   x_end  = time (NULL);
   printf ("# ran for %ld secs\n", x_end - my.runtime);
   FILE_commas (x_count, t);
   printf ("# wrote %s recs\n", t);
   /*---(save back)----------------------*/
   if (a_count != NULL)  *a_count = x_count;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



char         /*===[[ read entries from database ]]========[ ------ [ ------ ]=*/
READ_all           (char *a_name, int *a_count)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_file      = NULL;          /* file pointer                   */
   tPTRS      *x_dir       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(purges)-------------------------*/
   rc = ENTRY__purge ();
   rc = DRIVE__purge ();
   /*---(open)---------------------------*/
   rc = DB__open  (&x_file, a_name, 'r');
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (rc < 0 || x_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get drives)---------------------*/
   rc = READ__drives (x_file);
   DEBUG_INPT   yLOG_complex ("drives"    , "%4drc, %dn", rc, n_drive);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(process entries)----------------*/
   --rce;  while (1) {
      rc = READ__entry (x_file, a_count);
      if (rc < 0)  break;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&x_file);
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (rc < 0 || x_file != NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char 
FILE_percents      (long a_number, long a_total, char *a_string)
{
   /*---(locals)-----------+-----------+-*/
   double      x_percent   = 0;
   /*---(format)-------------------------*/
   if (a_number == 0) {
      strcpy (a_string, "-");
      return 0;
   }
   if (a_total  == 0) {
      strcpy (a_string, "-");
      return 0;
   }
   x_percent = ((double) a_number) / ((double) a_total);
   if (x_percent <= 0.009) {
      strcpy (a_string, "-");
      return 0;
   }
   if (x_percent > 1.0) {
      strcpy (a_string, "-");
      return 0;
   }
   sprintf (a_string, "%3.0lf", x_percent * 100.0);
   /*---(complete)-----------------------*/
   return 0;
}

char
FILE_commas        (llong a_number, char *a_string)
{
   if (a_number == 0)  ystrlcpy (a_string, "-", LEN_LABEL);
   else  ystrl4comma (a_number, a_string, 0, 'c', '-', LEN_LABEL);
   return 0;
}

char
FILE_uncommas      (char *a_string, llong *a_number)
{
   double     x_val;
   /*> if (strcmp (ystrltrim (a_string, ySTR_BOTH, LEN_LABEL), "-") == 0)  x_val = 0;   <* 
    *> else                              ystrl2comma (a_string, &x_val, LEN_LABEL);     <*/
   ystrl2comma (a_string, &x_val, LEN_LABEL);
   DEBUG_INPT   yLOG_double ("x_val"     , x_val);
   *a_number = x_val;
   DEBUG_INPT   yLOG_llong   ("a_number"  , *a_number);
   return 0;
}



/*===============================[[ end-code ]]===============================*/
