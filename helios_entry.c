/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */


tNODIR      nodir [MAX_NODIR];
int         n_nodir;

tPTRS      *dir_stack  [MAX_DEPTH];
tPTRS      *root_stack [MAX_DEPTH];

tMIME       mime [500];
int         n_mime      = 0;

/*====================------------------------------------====================*/
/*===----                   binary file methods                        ----===*/
/*====================------------------------------------====================*/
static void      o___BINARY__________________o (void) {;}

tPTRS     *h_ptrs;
tPTRS     *t_ptrs;
int        n_ptrs;
int        n_ptrs_ever;

tPTRS     *h_ptrd;
tPTRS     *t_ptrd;
int        n_ptrd;

char         /*===[[ initialize data structures ]]========[ ------ [ ------ ]=*/
DATA_init          (void)
{
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(pointers)-----------------------*/
   h_ptrs             = NULL;
   t_ptrs             = NULL;
   n_ptrs             = 0;
   n_ptrs_ever        = 0;
   h_ptrd             = NULL;
   t_ptrd             = NULL;
   n_ptrd             = 0;
   n_mime             = 0;
   MIME_read ();
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ remove a single entry ]]=============[ ------ [ ------ ]=*/
DATA_remove        (
      /*---(formal parameters)+-------------+---------------------------------*/
      tPTRS      *a_ptrs      )             /* entry to remove                */
{  /*---(local variables)--+-----------+-*/
   tPTRS      *x_child     = NULL;          /* child                          */
   tPTRS      *x_next      = NULL;          /* child                          */
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(free data)----------------------*/
   if (a_ptrs->data != NULL)  free (a_ptrs->data);
   /*---(check children)-----------------*/
   x_child = a_ptrs->sib_head;
   while (x_child != NULL) {
      x_next  = x_child->sib_next;
      DATA_remove (x_child);
      x_child = x_next;
   }
   /*---(unhook from siblings)-----------*/
   if (a_ptrs->sib_prev == NULL) {
      if (a_ptrs->sib_next == NULL) {
         a_ptrs->parent->sib_head   = NULL;
         a_ptrs->parent->sib_tail   = NULL;
         a_ptrs->parent->nchildren  = 0;
      } else {
         a_ptrs->sib_next->sib_prev = NULL;
         a_ptrs->parent->sib_head   = a_ptrs->sib_next;
         --(a_ptrs->parent->nchildren);
      }
   } else {
      if (a_ptrs->sib_next == NULL) {
         a_ptrs->sib_prev->sib_next = NULL;
         a_ptrs->parent->sib_tail   = a_ptrs->sib_prev;
         --(a_ptrs->parent->nchildren);
      } else {
         a_ptrs->sib_next->sib_prev = a_ptrs->sib_prev;
         a_ptrs->sib_prev->sib_next = a_ptrs->sib_next;
         --(a_ptrs->parent->nchildren);
      }
   }
   /*---(unhook from overall)------------*/
   if (a_ptrs->prev == NULL) {
      if (a_ptrs->next == NULL) {
         h_ptrs                     = NULL;
         t_ptrs                     = NULL;
         n_ptrs                     = 0;
      } else {
         a_ptrs->next->prev         = NULL;
         h_ptrs                     = a_ptrs->next;
         --n_ptrs;
      }
   } else {
      if (a_ptrs->next == NULL) {
         a_ptrs->prev->next         = NULL;
         t_ptrs                     = a_ptrs->prev;
         --n_ptrs;
      } else {
         a_ptrs->next->prev = a_ptrs->prev;
         a_ptrs->prev->next = a_ptrs->next;
         --n_ptrs;
      }
   }
   /*---(free ptrs)----------------------*/
   if (a_ptrs != NULL)  free (a_ptrs);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ remove full data structure ]]========[ ------ [ ------ ]=*/
DATA_purge         (void)
{
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(pointers)-----------------------*/
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ wipe a data payload ]]===============[ ------ [ ------ ]=*/
DATA_wipe          (
      /*---(formal parameters)+-------------+---------------------------------*/
      tDATA      *a_curr      )             /* current data payload           */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(defense)------------------------*/
   --rce;  if (a_curr == NULL) return rce;
   /*---(header)-------------------------*/
   DEBUG_ENVIM  yLOG_senter  (__FUNCTION__);
   DEBUG_ENVIM  yLOG_spoint  (a_curr);
   /*---(clear types)--------------------*/
   a_curr->type       = '-';
   a_curr->stype      = '.';
   /*---(clear perms)--------------------*/
   a_curr->uid        =   0;
   a_curr->own        = '0';
   a_curr->gid        =   0;
   a_curr->grp        = '0';
   a_curr->oth        = '0';
   /*---(unique)-------------------------*/
   a_curr->drive      =   0;
   a_curr->inode      =  -1;
   a_curr->dnode      =  -1;
   /*---(times)--------------------------*/
   a_curr->changed    =  -1;
   /*---(sizes)--------------------------*/
   a_curr->bytes      =   0;
   a_curr->cum        =   0;
   a_curr->size       =   0;
   /*---(categorization)-----------------*/
   a_curr->cat        = 'u';
   a_curr->ext  [0]   = '\0';
   /*---(name)---------------------------*/
   a_curr->level      =   0;
   a_curr->ascii      = '-';
   a_curr->len        =   0;
   a_curr->name [0]   = '\0';
   /*---(complete)-----------------------*/
   DEBUG_ENVIM  yLOG_sexit   (__FUNCTION__);
   return 0;
}

tPTRS*       /*===[[ create a new data/ptrs ]]============[ ------ [ ------ ]=*/
DATA_create        (
      /*---(formal parameters)+-------------+---------------------------------*/
      tPTRS      *a_parent    )             /* parent of entry                */
{  /*---(local variables)--+-----------+-*/
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   tDATA      *x_data      = NULL;          /* current entry                  */
   char        x_tries     = 0;             /* malloc attempts                */
   /*---(header)-------------------------*/
   DEBUG_ENVIM  yLOG_enter   (__FUNCTION__);
   /*---(create ptrs)--------------------*/
   DEBUG_ENVIM  yLOG_note    ("malloc ptrs");
   while (x_ptrs == NULL) {
      if (++x_tries > 3)  break;
      x_ptrs = (tPTRS*) malloc (sizeof (tPTRS));
   }
   if (x_ptrs == NULL) {
      DEBUG_ENVIM  yLOG_note    ("ptrs malloc failed, SKIPPED");
      DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   DEBUG_ENVIM  yLOG_point   ("created"   , x_ptrs);
   /*---(create data payload)------------*/
   DEBUG_ENVIM  yLOG_note    ("malloc data");
   while (x_data == NULL) {
      if (++x_tries > 3)  break;
      x_data = (tDATA*) malloc (sizeof (tDATA));
   }
   if (x_data == NULL) {
      free (x_ptrs);
      DEBUG_ENVIM  yLOG_note    ("data malloc failed, SKIPPED");
      DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   DEBUG_ENVIM  yLOG_point   ("created"   , x_data);
   /*---(clean data)---------------------*/
   DEBUG_ENVIM  yLOG_note    ("clean and hook up data payload");
   x_ptrs->data  = x_data;
   DATA_wipe (x_data);
   /*---(hook to overall list)-----------*/
   DEBUG_ENVIM  yLOG_note    ("hook to overall list");
   x_ptrs->next = NULL;
   if (n_ptrs == 0)  {
      h_ptrs        = x_ptrs;
      x_ptrs->prev  = NULL;
   } else {
      t_ptrs->next  = x_ptrs;
      x_ptrs->prev  = t_ptrs;
   }
   t_ptrs = x_ptrs;
   ++n_ptrs;
   ++n_ptrs_ever;
   /*---(initialize children)------------*/
   DEBUG_ENVIM  yLOG_note    ("initialize children");
   x_ptrs->sib_head   = NULL;
   x_ptrs->sib_tail   = NULL;
   x_ptrs->nchildren  = 0;
   x_ptrs->sib_prev   = NULL;
   x_ptrs->sib_next   = NULL;
   /*---(hook up to parent)--------------*/
   DEBUG_ENVIM  yLOG_note    ("hook to parent dir");
   x_ptrs->parent      = a_parent;
   if (a_parent != NULL) {
      if (a_parent->sib_head  == NULL) {
         a_parent->sib_head           = x_ptrs;
         a_parent->sib_tail           = x_ptrs;
      } else {
         x_ptrs->sib_prev             = a_parent->sib_tail;
         a_parent->sib_tail->sib_next = x_ptrs;
         a_parent->sib_tail           = x_ptrs;
      }
      ++a_parent->nchildren;
      x_ptrs->data->dnode = a_parent->data->inode;
   }
   DEBUG_ENVIM  yLOG_value   ("dnode"     , x_ptrs->data->dnode);
   /*---(complete)-----------------------*/
   DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
   return x_ptrs;
}

tDATA*       /*===[[ find a directory entry ]]============[ ------ [ ------ ]=*/
DATA_find          (
      /*---(formal parameters)+-------------+---------------------------------*/
      tPTRS      *a_dir       ,             /* parent of entry                */
      char       *a_name      )             /* entry to be found              */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   /*---(defenses)-----------------------*/
   --rce;  if (a_dir  == NULL)   return NULL;
   --rce;  if (a_name == NULL)   return NULL;
   /*---(header)-------------------------*/
   /*> DEBUG_ENVIM  yLOG_enter   (__FUNCTION__);                                      <* 
    *> DEBUG_ENVIM  yLOG_point   ("a_dir"     , a_dir);                               <* 
    *> DEBUG_ENVIM  yLOG_point   ("a_dir>data", a_dir->data);                         <* 
    *> DEBUG_ENVIM  yLOG_info    ("dir_name"  , a_dir->data->name);                   <* 
    *> DEBUG_ENVIM  yLOG_info    ("a_name"    , a_name);                              <*/
   /*---(cycle children)-----------------*/
   x_ptrs = a_dir->sib_head;
   while (x_ptrs != NULL) {
      /*> DEBUG_ENVIM  yLOG_point   ("x_ptrs"    , x_ptrs);                           <* 
       *> DEBUG_ENVIM  yLOG_point   ("data"      , x_ptrs->data);                     <*/
      if (  x_ptrs->data       != NULL &&
            x_ptrs->data->name != NULL) {
         /*> DEBUG_ENVIM  yLOG_point   ("name"      , x_ptrs->data->name);            <*/
         if (strcmp (a_name, x_ptrs->data->name) == 0) {
            /*> DEBUG_ENVIM  yLOG_note    ("found it");                               <* 
             *> DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);                             <*/
            return x_ptrs;
         }
      }
      x_ptrs = x_ptrs->sib_next;
   }
   /*---(complete)-----------------------*/
   /*> DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);                                      <*/
   return NULL;
}

int          /*===[[ create entry statistics ]]===========[ ------ [ ------ ]=*/
DATA_mime          (
      /*---(formal parameters)+-------------+---------------------------------*/
      tDATA      *a_data      ,             /* file to be categorized         */
      char       *a_ext       )             /* extension to check             */
{  /*---(locals)-----------+-----------+-*/
   int         rc          = -1;            /* return code                    */
   int         x_len       =  0;            /* length of extension            */
   char        x_ext       [10] = "";       /* working copy of extension      */
   int         i           =  0;            /* generic iterator               */
   /*---(defence)------------------------*/
   x_len = strlen  (a_ext);
   if (x_len <= 0)    return -2;
   if (x_len >  7)    return -3;
   /*---(prepare)------------------------*/
   strcpy (x_ext, a_ext);
   for (i = 0; i < x_len; ++i)   x_ext [i] = tolower (x_ext [i]);
   DEBUG_MIME   yLOG_info    ("x_ext"     , x_ext);
   /*---(search for matche)--------------*/
   for (i = 0; i < n_mime; ++i) {
      /*---(filter)----------------------*/
      if (mime [i].ext  [0] != x_ext [0])      continue;
      if (mime [i].ext  [1] != x_ext [1])      continue;
      if (strcmp (mime [i].ext , x_ext) != 0)  continue;
      /*---(report)----------------------*/
      DEBUG_MIME   yLOG_value   ("found at"  , i);
      DEBUG_MIME   yLOG_info    ("found"     , mime [i].ext);
      /*---(update)----------------------*/
      a_data->cat = mime [i].cat;
      strcpy (a_data->ext, mime [i].ext);
      ++(mime [i].seen);
      mime [i].sbytes += a_data->bytes;
      /*---(get out)---------------------*/
      rc = i;
      break;
   }
   /*---(complete)-----------------------*/
   return rc;
}

int          /*===[[ create entry statistics ]]===========[ ------ [ ------ ]=*/
DATA_stats         (
      /*---(formal parameters)+-------------+---------------------------------*/
      tPTRS      *a_ptrs      ,             /* current entry                  */
      char       *a_full      )             /* current entry                  */
{  /*---(locals)-----------+-----------+-*/
   tDATA      *x_data      = NULL;
   char        rce         = -10;           /* return code for errors         */
   tSTAT       st;                          /* file stats                     */
   tSTAT       st2;                         /* file stats                     */
   char        c           = ' ';           /* generic char                   */
   char        s           [500];           /* generic string                 */
   int         rci         = 0;             /* return code as integer         */
   int         i           = 0;             /* generic iterator               */
   char       *basic       = "abcdefghijklmnopqrstuvwxyz0123456789_.";
   char       *upper       = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   char       *punct       = "-:;()[]+";
   char        x_warn      = 0;
   char       *p           = NULL;
   char       *q           = NULL;
   int         x_len       = 0;
   int         rc          = 0;
   char        x_dir       [10] = ".dir";
   char        x_link      [10] = ".symlink";
   char        x_back      [10] = ".backup";
   char        x_exe       [10] = "exe";
   char        x_other     [10] = "ooo";
   char        x_conf      [10] = "conf";
   char        x_guess     [10] = "---";
   /*---(header)-------------------------*/
   DEBUG_ENVIM  yLOG_enter   (__FUNCTION__);
   DEBUG_ENVIM  yLOG_point   ("a_ptrs"    , a_ptrs);
   /*---(defense : null command)---------*/
   --rce; if (a_ptrs == NULL) {
      DEBUG_ENVIM  yLOG_note    ("called with null ptrs pointer, exiting");
      DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_data = a_ptrs->data;
   DEBUG_ENVIM  yLOG_point   ("x_data"    , x_data);
   /*---(defense : null name)------------*/
   --rce; if (x_data->name == NULL) {
      DEBUG_ENVIM  yLOG_note    ("called with null entry name, exiting");
      DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_ENVIM  yLOG_info    ("name"      , x_data->name);
   DEBUG_ENVIM  yLOG_info    ("full_name" , a_full);
   /*---(get the file information)-------*/
   rci = lstat (a_full, &st);
   DEBUG_ENVIM  yLOG_value   ("lstat_rc"  , rci);
   /*---(defense : real file)------------*/
   --rce; if (rci < 0) {
      DEBUG_ENVIM  yLOG_note    ("lstat can not find file, exiting");
      DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(inode)--------------------------*/
   x_data->inode = st.st_ino;
   DEBUG_ENVIM  yLOG_value   ("inode"     , x_data->inode);
   /*---(uid and gid)--------------------*/
   x_data->uid   = st.st_uid;
   x_data->gid   = st.st_gid;
   /*---(type)---------------------------*/
   if      (S_ISDIR (st.st_mode))  c = 'd';
   else if (S_ISCHR (st.st_mode))  c = 'c';
   else if (S_ISBLK (st.st_mode))  c = 'b';
   else if (S_ISFIFO(st.st_mode))  c = 'f';
   else if (S_ISSOCK(st.st_mode))  c = 's';
   else if (S_ISREG (st.st_mode))  c = '-';
   else if (S_ISLNK (st.st_mode))  c = 'l';
   else                            c = '?';
   x_data->type  = c;
   DEBUG_ENVIM  yLOG_char    ("file_type" , x_data->type);
   /*---(update type, if link)-----------*/
   if (c == 'l') {
      rci = stat (a_full, &st2);
      if      (S_ISDIR (st2.st_mode))  c = 'd';
      else if (S_ISCHR (st2.st_mode))  c = 'c';
      else if (S_ISBLK (st2.st_mode))  c = 'b';
      else if (S_ISFIFO(st2.st_mode))  c = 'f';
      else if (S_ISSOCK(st2.st_mode))  c = 's';
      else if (S_ISREG (st2.st_mode))  c = '-';
      else if (S_ISLNK (st2.st_mode))  c = 'l';
      else                             c = '?';
      x_data->type  = c;
      x_data->stype = '>';
   }
   /*---(mounted filesystems)------------*/
   if (c == 'd' && x_data->inode == 1) {
      x_data->inode = -1;
   }
   if (c == 'd' && x_data->inode == 2 && strcmp (x_data->name, "/") != 0) {
      x_data->inode = -2;
   }
   /*---(mode)---------------------------*/
   sprintf (s, "%-10.10o", st.st_mode);
   x_data->own = s [7];
   x_data->grp = s [8];
   x_data->oth = s [9];
   /*---(timestamp)----------------------*/
   x_data->changed    = st.st_mtime;
   DEBUG_ENVIM  yLOG_value   ("changed"   , x_data->changed);
   /*---(size)---------------------------*/
   x_data->bytes = st.st_size;
   x_data->cum   = st.st_size;
   DEBUG_ENVIM  yLOG_value   ("bytes"     , x_data->bytes);
   sprintf (s, "%d", st.st_size);
   x_data->size  = strlen (s);
   DEBUG_ENVIM  yLOG_value   ("exponent"  , x_data->size);
   /*---(name length)--------------------*/
   x_data->len = strlen (x_data->name);
   /*---(name quality)-------------------*/
   x_warn = 0;
   for (i = 0; i < x_data->len; ++i) {
      if (strchr (basic, x_data->name [i]) != NULL)       continue;
      if (strchr (upper, x_data->name [i]) != NULL) {
         if (x_warn < 1)   x_warn = 1;
         continue;
      }
      if (strchr (punct, x_data->name [i]) != NULL) {
         if (x_warn < 2)   x_warn = 2;
         continue;
      }
      if (x_data->name [i] > 32 && x_data->name [i] <= 126) {
         if (x_warn < 3)   x_warn = 3;
         continue;
      }
      if (x_data->name [i] == " ") {
         if (x_warn < 4)   x_warn = 4;
         continue;
      }
      if (x_data->name [i] < 32 || x_data->name [i] > 126) {
         if (x_warn < 5)   x_warn = 5;
         continue;
      }
   }
   DEBUG_ENVIM  yLOG_value   ("x_warn"    , x_warn);
   switch (x_warn) {
   case 0 : x_data->ascii = '-';  break;
   case 1 : x_data->ascii = 'A';  break;
   case 2 : x_data->ascii = '+';  break;
   case 3 : x_data->ascii = '#';  break;
   case 4 : x_data->ascii = '>';  break;
   case 5 : x_data->ascii = 'X';  break;
   }
   /*---(get extension)-------------------*/
   DEBUG_MIME   yLOG_enter   ("mime testing");
   DEBUG_MIME   yLOG_info    ("name"      , x_data->name);
   p = strrchr (x_data->name, '.');
   DEBUG_MIME   yLOG_point   ("post p"    , p);
   /*---(fix for obvious types)-----------*/
   if      (x_data->type == 'd')                    p = x_dir;
   else if (x_data->stype == '>')                   p = x_link;
   else if (x_data->name [x_data->len - 1] == '~')  p = x_back;
   /*---(search extensions)---------------*/
   if (p != NULL) {
      /*---(try postfix extension)--------*/
      ++p;
      DEBUG_MIME   yLOG_info    ("post name" , p);
      x_len = strlen (p);
      DEBUG_MIME   yLOG_value   ("post len"  , x_len);
      rc = 0;
      if (x_len <= 7) {
         rc = DATA_mime (x_data, p);
         DEBUG_MIME   yLOG_value   ("post rc"   , rc);
      }
      /*---(try infix extension)----------*/
      if (rc <= 0) {
         p  = strchr (x_data->name, '.');
         DEBUG_MIME   yLOG_point   ("infix p"   , p);
         if (p != NULL) {
            ++p;
            q  = strchr (p           , '.');
            DEBUG_MIME   yLOG_point   ("infix q"   , q);
            DEBUG_MIME   yLOG_value   ("q - p"     , (long) (q - p));
            if (q != NULL && (q - p) > 0 && (q - p) <= 7) {
               *q = '\0';
               DEBUG_MIME   yLOG_info    ("infix name", p);
               rc = DATA_mime (x_data, p);
               DEBUG_MIME   yLOG_value   ("infix rc"  , rc);
               *q = '.';
            }
         }
      }
   }
   /*---(take care of executables)--------*/
   if (x_data->cat == 'u') {
      if ((  st.st_mode & S_IXUSR)  ||
            (st.st_mode & S_IXGRP)  ||
            (st.st_mode & S_IXOTH))  {
         ++(mime [my.mime_exe  ].seen);
         mime [my.mime_exe  ].sbytes += x_data->bytes;
         x_data->cat = 'x';
         strcpy (x_data->ext, x_exe);
         rc = my.mime_exe;
      }
   }
   /*---(file the rest properly)----------*/
   if (x_data->cat == 'u') {
      if (strncmp (a_full, "/etc/", 5) == 0) {
         ++(mime [my.mime_conf ].seen);
         mime [my.mime_conf ].sbytes += x_data->bytes;
         x_data->cat = 't';
         strcpy (x_data->ext, x_conf );
         rc = my.mime_conf;
      } else if (x_len > 0 && x_len <= 7) {
         ++(mime [my.mime_other].seen);
         mime [my.mime_other].sbytes += x_data->bytes;
         strcpy (x_data->ext, x_other);
         rc = my.mime_other;
      } else {
         ++(mime [my.mime_guess].seen);
         mime [my.mime_guess].sbytes += x_data->bytes;
         strcpy (x_data->ext, x_guess);
         rc = my.mime_guess;
      }
   }
   /*---(grand total)---------------------*/
   ++(mime [0].seen);
   mime [0].sbytes += x_data->bytes;
   DEBUG_MIME   yLOG_exit    ("mime testing");
   /*---(complete)------------------------*/
   DEBUG_ENVIM  yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*===[[ gather entries in dir ]]=============[ ------ [ ------ ]=*/
DATA_gather        (
      /*---(formal parameters)+-------------+---------------------------------*/
      int         a_level     ,             /* depth of search                */
      tDRIVE     *a_drive     ,             /* drive which houses directory   */
      tPTRS      *a_ptrs      ,             /* ptrs of directory to scan      */
      char       *a_path      ,             /* full path of directory to scan */
      char        a_keep      )             /* y = keey entries, - = size cum */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   DIR        *dir         = NULL;          /* directory pointer              */
   tDIRENT    *den         = NULL;          /* directory entry                */
   tDATA       *x_dir      = NULL;          /* directory data                 */
   tPTRS       *x_ptrs     = NULL;          /* current entry                  */
   tDATA       *x_data     = NULL;          /* current entry                  */
   int         rc          = 0;             /* generic return code            */
   int         rc2         = 0;             /* generic return code            */
   int         i           = 0;             /* generic iterator               */
   char        x_path      [500];
   char        x_full      [500];
   int         x_len;
   char        x_keep      = a_keep;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVIM  yLOG_value   ("a_level"   , a_level);
   DEBUG_ENVIM  yLOG_point   ("a_ptrs"    , a_ptrs);
   DEBUG_ENVIM  yLOG_char    ("a_keep"    , a_keep);
   /*---(defense)------------------------*/
   --rce;  if (a_level >  my.maxlevel) {
      DEBUG_ENVI   yLOG_note    ("max recursion reached, return");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (a_ptrs == NULL) {
      DEBUG_ENVI   yLOG_note    ("directory pointer NULL, failed");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_ENVIM  yLOG_point   ("dir_data"  , a_ptrs->data);
   --rce;  if (a_ptrs->data == NULL) {
      DEBUG_ENVI   yLOG_note    ("directory data payload NULL, failed");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_dir = a_ptrs->data;
   DEBUG_ENVIM  yLOG_point   ("dir_name"  , x_dir->name);
   --rce;  if (x_dir->name == NULL) {
      DEBUG_ENVI   yLOG_note    ("directory name NULL, failed");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("dir_name"  , x_dir->name);
   DEBUG_CONF   yLOG_info    ("dir_name"  , x_dir->name);
   DEBUG_ENVIM  yLOG_value   ("dir_len"   , x_dir->len);
   --rce;  if (x_dir->len >= MAX_NAME) {
      DEBUG_ENVI   yLOG_note    ("directory name too long, failed");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(name directory)-----------------*/
   if      (a_level == 1)  sprintf (x_path, "%s"   , x_dir->name);
   else if (a_level == 2)  sprintf (x_path, "/%s"  , x_dir->name);
   else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);
   x_len = strlen (x_path);
   DEBUG_ENVI   yLOG_info    ("x_path"    , x_path);
   OPT_VERBOSE  printf ("%s\n", x_path);
   /*---(filter from conf)---------------*/
   DEBUG_CONF   yLOG_note    ("checking dir against conf dir_ignore");
   DEBUG_CONF   yLOG_value   ("n_nodir"   , n_nodir);
   for (i = 0; i < n_nodir; ++i) {
      DEBUG_CONF   yLOG_info    ("dir"       , nodir [i].name);
      if (nodir [i].type == 'x') {
         if (strcmp (nodir [i].name, x_dir->name) == 0) {
            DEBUG_CONF   yLOG_note    ("directory blocked by config dir_never");
            DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
            return 0;
         }
         continue;
      }
      if (nodir [i].len != x_len)               continue;
      if (strcmp (nodir [i].name, x_path) != 0) continue;
      if (nodir [i].type == '#') {
         x_dir->stype = nodir [i].type;
         DEBUG_CONF   yLOG_note    ("directory blocked by config dir_ignore");
         DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      if (x_keep == 'y') {
         x_dir->stype = nodir [i].type;
         x_keep = '-';
      }
      break;
   }
   if (x_keep != 'y' && x_dir->stype == '.')  x_dir->stype = '|';
   /*---(open directory)-----------------*/
   dir = opendir (x_path);
   --rce;  if (dir == NULL) {
      DEBUG_ENVI   yLOG_note    ("could not open the location");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_ENVIM  yLOG_note    ("success, location open");
   /*---(cycle through the entries)------*/
   /*> DEBUG_ENVI   yLOG_sync    ();                                                  <*/
   --rce;
   while ((den = readdir(dir)) != NULL) {
      if (den->d_name   == NULL)            continue;
      if (strcmp (den->d_name, ".")  == 0)  continue;
      if (strcmp (den->d_name, "..") == 0)  continue;
      DEBUG_ENVI   yLOG_info    ("entry"     , den->d_name);
      /*---(check for existing)----------*/
      x_ptrs = NULL;
      x_ptrs = DATA_find (a_ptrs, den->d_name);
      /*---(create if required)----------*/
      if (x_ptrs == NULL) {
         x_ptrs = DATA_create (a_ptrs);
         DEBUG_ENVIM  yLOG_point   ("new"       , x_ptrs);
         if (x_ptrs == NULL) {
            DEBUG_ENVI   yLOG_note    ("could not allocate an entry");
            DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
            closedir (dir);
            return rce;
         }
      } else {
         DEBUG_ENVIM  yLOG_point   ("exists"    , x_ptrs);
      }
      x_data = x_ptrs->data;
      /*---(get name saved)--------------*/
      strncpy (x_data->name, den->d_name, MAX_NAME - 1);
      x_data->level = a_level;
      x_data->drive = x_ptrs->parent->data->drive;
      /*---(get attributes)--------------*/
      if (a_level == 1)  sprintf (x_full, "/%s"  , den->d_name);
      else               sprintf (x_full, "%s/%s", x_path, den->d_name);
      rc = DATA_stats (x_ptrs, x_full);
      /*---(recurse directories)---------*/
      if (x_data->type == 'd' && x_data->stype == '.') {
         rc2 = DATA_gather (a_level + 1, a_drive, x_ptrs, x_path, x_keep);
         if (rc2 < 0) {
            DEBUG_ENVI   yLOG_note    ("finished badly");
            DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
            closedir (dir);
            return rc;
         }
      }
      /*---(update cum)------------------*/
      x_ptrs->parent->data->cum += x_data->cum;
      /*---(update kept)-----------------*/
      if (a_keep != 'y' || x_dir->stype == '(') {
         DATA_remove (x_ptrs);
      } else if (rc > 0) {
         ++(mime [rc].kept);
         ++(mime [0 ].kept);
         mime [rc].kbytes += x_ptrs->data->bytes;
         mime [0 ].kbytes += x_ptrs->data->bytes;
      }
      /*---(done)------------------------*/
   }
   closedir (dir);
   GNOME_bucket (0, &(a_ptrs->sib_head), &(a_ptrs->sib_tail));
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

tPTRS*       /*--(create base entry)----------------------[ ------ [ ------ ]-*/
DATA_root          (tDRIVE *a_drive)
{  /*---(local variables)--+-----------+-*/
   int         rc          = 0;
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(create root)--------------------*/
   x_ptrs = DATA_create (NULL);
   DEBUG_ENVI   yLOG_point   ("root"      , x_ptrs);
   /*---(assign drive)-------------------*/
   strcpy (x_ptrs->data->name, a_drive->mpoint);
   x_ptrs->data->drive = a_drive->ref;
   /*---(fill in root entry)-------------*/
   rc = DATA_stats (x_ptrs, my.mpoint);
   if (rc > 0) {
      ++(mime [rc].kept);
      ++(mime [0 ].kept);
      mime [rc].kbytes += x_ptrs->data->bytes;
      mime [0 ].kbytes += x_ptrs->data->bytes;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return x_ptrs;
}

tPTRS*       /*--(create base entry)----------------------[ ------ [ ------ ]-*/
DATA_empty         (tDRIVE *a_drive, tPTRS *a_root)
{  /*---(local variables)--+-----------+-*/
   char        rc          = 0;
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*> debug.envi  = 'y';                                                             <* 
    *> debug.enviM = 'y';                                                             <*/
   x_ptrs = DATA_create (a_root);
   DEBUG_ENVI   yLOG_point   ("((empty))" , x_ptrs);
   DEBUG_ENVI   yLOG_point   ("parent"    , x_ptrs->parent);
   DEBUG_ENVI   yLOG_point   ("->data"    , x_ptrs->parent->data);
   DEBUG_ENVI   yLOG_info    ("->->name"  , x_ptrs->parent->data->name);
   DEBUG_ENVI   yLOG_value   ("->->level" , x_ptrs->parent->data->level);
   DEBUG_ENVI   yLOG_point   ("->tail"    , x_ptrs->parent->sib_tail);
   DEBUG_ENVI   yLOG_point   ("->->data"  , x_ptrs->parent->sib_tail->data);
   DEBUG_ENVI   yLOG_info    ("->->->name", x_ptrs->parent->sib_tail->data->name);
   strcpy (x_ptrs->data->name, "((empty))");
   x_ptrs->data->drive = a_drive->ref;
   x_ptrs->data->type  = 'd';
   x_ptrs->data->level =   1;
   x_ptrs->data->bytes = a_drive->size - a_root->data->cum;
   x_ptrs->data->cum   = a_drive->size - a_root->data->cum;
   a_root->data->cum   = a_drive->size;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return x_ptrs;
}

tPTRS*       /*===[[ process a starting point ]]==========[ ------ [ ------ ]=*/
DATA_start         (
      /*---(formal parameters)+-------------+---------------------------------*/
      char       *a_path      )             /* full path of directory to scan */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   char        x_path      [MAX_NAME];      /* strtok working copy            */
   char       *p           = NULL;          /* strtok current pointer         */
   char       *q           = "/";           /* strtok delimiters              */
   char       *s           = NULL;          /* strtok context                 */
   /*---(header)-------------------------*/
   DEBUG_ARGS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ARGS   yLOG_note    ("get rocking");
   DEBUG_ARGS   yLOG_point   ("a_path"    , a_path);
   --rce;  if (a_path == NULL) {
      DEBUG_ARGS   yLOG_note    ("start path NULL, return");
      DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_ARGS   yLOG_info    ("a_path"    , a_path);
   --rce;  if (a_path [0] != '/') {
      DEBUG_ARGS   yLOG_note    ("start path not absolute, return");
      DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(root)---------------------------*/
   x_ptrs = h_ptrs;
   --rce;  if (x_ptrs->data == NULL) {
      DEBUG_ARGS   yLOG_note    ("root does not have data, return");
      DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (x_ptrs->data->name == NULL) {
      DEBUG_ARGS   yLOG_note    ("root does not have a name, return");
      DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (strcmp (x_ptrs->data->name, "/") != 0) {
      DEBUG_ARGS   yLOG_note    ("root not /, return");
      DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   my.level = 0;
   root_stack [my.level] = x_ptrs;
   /*---(process)------------------------*/
   strcpy   (x_path, a_path + 1);
   p = strtok_r (x_path, q, &s);
   --rce;
   while (p != NULL) {
      DEBUG_ENVI   yLOG_info    ("segment"   , p);
      x_ptrs = DATA_find (x_ptrs, p);
      DEBUG_ENVI   yLOG_point   ("x_ptrs"    , x_ptrs);
      if (x_ptrs == NULL) {
         DEBUG_ARGS   yLOG_note    ("segment not found, return");
         DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      ++my.level;
      root_stack [my.level] = x_ptrs;
      p = strtok_r (NULL  , q, &s);
   }
   /*---(complete)-----------------------*/
   DEBUG_ARGS   yLOG_exit    (__FUNCTION__);
   return x_ptrs;
}



/*===============================[[ end-code ]]===============================*/
