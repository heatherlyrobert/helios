/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */


tNODIR      g_nodir [MAX_NODIR];
int         n_nodir;

tPTRS      *dir_stack   [MAX_DEPTH];
tPTRS      *root_stack  [MAX_DEPTH];

tMIME       mime        [MAX_MIME];
int         n_mime      = 0;

tPTRS     *h_ptrs;
tPTRS     *t_ptrs;
int        n_ptrs;
int        n_ptrs_ever;



/*====================------------------------------------====================*/
/*===----                         program level                        ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char         /*===[[ initialize data structures ]]========[ ------ [ ------ ]=*/
ENTRY_init              (void)
{
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(pointers)-----------------------*/
   h_ptrs             = NULL;
   t_ptrs             = NULL;
   n_ptrs             = 0;
   n_ptrs_ever        = 0;
   n_mime             = 0;
   MIME_read ();
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ENTRY_wrap              (void)
{
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     memory management                        ----===*/
/*====================------------------------------------====================*/
static void      o___MALLOC__________________o (void) {;}

char
ENTRY_new               (tPTRS **a_new, tPTRS *a_parent)
{  /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   tPTRS      *x_new       = NULL;          /* current entry                  */
   tENTRY     *x_data      = NULL;          /* current entry                  */
   char        x_tries     = 0;             /* malloc attempts                */
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_new != NULL)  *a_new = NULL;
   /*---(create ptrs)--------------------*/
   DEBUG_ENVI   yLOG_note    ("malloc pointers");
   while (x_new == NULL) {
      if (++x_tries > 3)  break;
      x_new = (tPTRS*) malloc (sizeof (tPTRS));
   }
   DEBUG_ENVI   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_ENVI   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create data payload)------------*/
   DEBUG_ENVI   yLOG_note    ("malloc data");
   x_tries = 0;
   while (x_data == NULL) {
      if (++x_tries > 3)  break;
      x_data = (tENTRY*) malloc (sizeof (tENTRY));
   }
   DEBUG_ENVI   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_ENVI   yLOG_point   ("x_data"    , x_data);
   if (x_data == NULL) {
      free (x_new);
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(clean data)---------------------*/
   DEBUG_ENVI   yLOG_note    ("clean and hook up data payload");
   x_new->data  = x_data;
   ENTRY__wipe (x_data);
   /*---(hook to overall list)-----------*/
   DEBUG_ENVI   yLOG_note    ("hook to overall list");
   x_new->next = NULL;
   if (h_ptrs == NULL)  {
      h_ptrs        = x_new;
      x_new->prev  = NULL;
   } else {
      t_ptrs->next  = x_new;
      x_new->prev  = t_ptrs;
   }
   t_ptrs = x_new;
   ++n_ptrs;
   ++n_ptrs_ever;
   /*---(initialize children)------------*/
   DEBUG_ENVI   yLOG_note    ("initialize children");
   x_new->sib_head   = NULL;
   x_new->sib_tail   = NULL;
   x_new->nchildren  = 0;
   x_new->sib_prev   = NULL;
   x_new->sib_next   = NULL;
   /*---(hook up to parent)--------------*/
   DEBUG_ENVI   yLOG_note    ("hook to parent dir");
   x_new->parent      = a_parent;
   if (a_parent != NULL) {
      if (a_parent->sib_head  == NULL) {
         a_parent->sib_head           = x_new;
         a_parent->sib_tail           = x_new;
      } else {
         x_new->sib_prev              = a_parent->sib_tail;
         a_parent->sib_tail->sib_next = x_new;
         a_parent->sib_tail           = x_new;
      }
      ++a_parent->nchildren;
      x_new->data->dnode = a_parent->data->inode;
   }
   DEBUG_ENVI   yLOG_value   ("dnode"     , x_new->data->dnode);
   /*---(save back)----------------------*/
   if (a_new != NULL)  *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ remove a single entry ]]=============[ ------ [ ------ ]=*/
ENTRY__free             (tPTRS **a_ptrs)
{  /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   tPTRS      *x_curr      = NULL;
   tPTRS      *x_child     = NULL;
   tPTRS      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_spoint  (a_ptrs);
   --rce;  if (a_ptrs == NULL) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_spoint  (*a_ptrs);
   --rce;  if (*a_ptrs == NULL) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_curr = *a_ptrs;
   /*---(free data)----------------------*/
   DEBUG_ENVI   yLOG_snote   ("free data");
   if (x_curr->data != NULL)  free (x_curr->data);
   /*---(check children)-----------------*/
   DEBUG_ENVI   yLOG_snote   ("free children");
   x_child = x_curr->sib_head;
   while (x_child != NULL) {
      x_next  = x_child->sib_next;
      ENTRY__free (&x_child);
      x_child = x_next;
   }
   /*---(unhook from siblings)-----------*/
   if (x_curr->parent != NULL) {
      DEBUG_ENVI   yLOG_snote   ("out of sibs");
      if (x_curr->sib_prev == NULL) {
         if (x_curr->sib_next == NULL) {
            x_curr->parent->sib_head   = NULL;
            x_curr->parent->sib_tail   = NULL;
            x_curr->parent->nchildren  = 0;
         } else {
            x_curr->sib_next->sib_prev = NULL;
            x_curr->parent->sib_head   = x_curr->sib_next;
            --(x_curr->parent->nchildren);
         }
      } else {
         if (x_curr->sib_next == NULL) {
            x_curr->sib_prev->sib_next = NULL;
            x_curr->parent->sib_tail   = x_curr->sib_prev;
            --(x_curr->parent->nchildren);
         } else {
            x_curr->sib_next->sib_prev = x_curr->sib_prev;
            x_curr->sib_prev->sib_next = x_curr->sib_next;
            --(x_curr->parent->nchildren);
         }
      }
   }
   /*---(unhook from overall)------------*/
   DEBUG_ENVI   yLOG_snote   ("out of master");
   if (x_curr->prev == NULL) {
      if (x_curr->next == NULL) {
         h_ptrs                     = NULL;
         t_ptrs                     = NULL;
         n_ptrs                     = 0;
      } else {
         x_curr->next->prev         = NULL;
         h_ptrs                     = x_curr->next;
         --n_ptrs;
      }
   } else {
      if (x_curr->next == NULL) {
         x_curr->prev->next         = NULL;
         t_ptrs                     = x_curr->prev;
         --n_ptrs;
      } else {
         x_curr->next->prev = x_curr->prev;
         x_curr->prev->next = x_curr->next;
         --n_ptrs;
      }
   }
   /*---(ground pointer)-----------------*/
   DEBUG_ENVI   yLOG_snote   ("ground");
   *a_ptrs = NULL;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ remove full data structure ]]========[ ------ [ ------ ]=*/
ENTRY__purge       (void)
{  /*---(local variables)--+-----------+-*/
   tENTRY     *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(pointers)-----------------------*/
   x_curr = h_ptrs;
   while (x_curr != NULL) {
      ENTRY__free (&x_curr);
      x_curr = h_ptrs;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ remove full data structure ]]========[ ------ [ ------ ]=*/
ENTRY__dirclear    (tPTRS *a_from)
{  /*---(local variables)--+-----------+-*/
   tENTRY     *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(pointers)-----------------------*/
   x_curr = a_from->sib_head;
   while (x_curr != NULL) {
      ENTRY__free (&x_curr);
      x_curr = a_from->sib_head;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ wipe a data payload ]]===============[ ------ [ ------ ]=*/
ENTRY__wipe             (tENTRY *a_curr)
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(defense)------------------------*/
   --rce;  if (a_curr == NULL) return rce;
   /*---(header)-------------------------*/
   DEBUG_ENVI_M yLOG_senter  (__FUNCTION__);
   DEBUG_ENVI_M yLOG_spoint  (a_curr);
   /*---(clear types)--------------------*/
   a_curr->type       = ENTRY_REG;
   a_curr->stype      = '.';
   /*---(clear perms)--------------------*/
   a_curr->uid        =   0;
   a_curr->own        =   0;
   a_curr->gid        =   0;
   a_curr->grp        =   0;
   a_curr->oth        =   0;
   a_curr->super      = '-';
   /*---(unique)-------------------------*/
   a_curr->drive      =   0;
   a_curr->inode      =  -1;
   a_curr->dnode      =  -1;
   /*---(times)--------------------------*/
   a_curr->changed    =   0;
   /*---(sizes)--------------------------*/
   a_curr->bytes      =   0;
   a_curr->cum        =   0;
   a_curr->size       =   0;
   /*---(categorization)-----------------*/
   a_curr->cat        = MIME_HUH;
   strlcpy (a_curr->ext, "-", LEN_TERSE);
   /*---(name)---------------------------*/
   a_curr->level      =   0;
   a_curr->ascii      = '-';
   a_curr->len        =   0;
   a_curr->name [0]   = '\0';
   /*---(complete)-----------------------*/
   DEBUG_ENVI_M yLOG_sexit   (__FUNCTION__);
   return 0;
}

char          /*===[[ find a directory entry ]]============[ ------ [ ------ ]=*/
ENTRY__find             (tPTRS *a_dir, char *a_name, tPTRS **a_found)
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVI   yLOG_complex ("args"      , "%p, %p, %p", a_dir, a_name, a_found);
   /*---(prepare)------------------------*/
   if (a_found != NULL)  *a_found = NULL;
   /*---(defenses)-----------------------*/
   --rce;  if (a_dir  == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("dir_name"  , a_dir->data->name);
   --rce;  if (a_name == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_name"    , a_name);
   --rce;  if (a_found == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cycle children)-----------------*/
   x_ptrs = a_dir->sib_head;
   --rce;  while (x_ptrs != NULL) {
      /*> DEBUG_ENVI   yLOG_point   ("x_ptrs"    , x_ptrs);                           <* 
       *> DEBUG_ENVI   yLOG_point   ("data"      , x_ptrs->data);                     <*/
      if (x_ptrs->data != NULL && x_ptrs->data->name != NULL) {
         /*> DEBUG_ENVI   yLOG_point   ("name"      , x_ptrs->data->name);            <*/
         if (strcmp (a_name, x_ptrs->data->name) == 0) {
            DEBUG_ENVI   yLOG_note    ("found it");
            *a_found = x_ptrs;
            DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
            return 1;
         }
      }
      x_ptrs = x_ptrs->sib_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit   (__FUNCTION__);
   return 0;
}

char
ENTRY__name_check       (cchar *a_name, char *a_warn, uchar *a_len)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   char        x_warn      =    0;
   int         i           =    0;
   uchar       x_result    = ASCII_BASIC;
   char       *x_basic     = "abcdefghijklmnopqrstuvwxyz0123456789_.";
   char       *x_upper     = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   char       *x_punct     = "-:;()[]+";
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_snote   (a_name);
   /*---(prepare)------------------------*/
   x_len = strlen (a_name);
   DEBUG_ENVI   yLOG_sint    (x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(check characters)---------------*/
   for (i = 0; i < x_len; ++i) {
      if (strchr (x_basic, a_name [i]) != NULL)       continue;
      if (strchr (x_upper, a_name [i]) != NULL) {
         if (x_warn < 1)   x_warn = 1;
         continue;
      }
      if (strchr (x_punct, a_name [i]) != NULL) {
         if (x_warn < 2)   x_warn = 2;
         continue;
      }
      if (a_name [i] > 32 && a_name [i] <= 126) {
         if (x_warn < 3)   x_warn = 3;
         continue;
      }
      if (a_name [i] == ' ') {
         if (x_warn < 4)   x_warn = 4;
         continue;
      }
      if (a_name [i] < 32 || a_name [i] > 126) {
         if (x_warn < 5)   x_warn = 5;
         continue;
      }
   }
   DEBUG_ENVI   yLOG_sint    (x_warn);
   /*---(categorize)---------------------*/
   switch (x_warn) {
   case 0 : x_result = ASCII_BASIC;   break;
   case 1 : x_result = ASCII_UPPER;   break;
   case 2 : x_result = ASCII_PUNCT;   break;
   case 3 : x_result = ASCII_EXTEND;  break;
   case 4 : x_result = ASCII_SPACE;   break;
   case 5 : x_result = ASCII_CRAZY;   break;
   }
   DEBUG_ENVI   yLOG_schar   (x_result);
   /*---(save result)--------------------*/
   if (a_warn != NULL)  *a_warn = x_result;
   if (a_len  != NULL)  *a_len  = x_len;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return x_result;
}

char
ENTRY__type_check       (cchar *a_name, tSTAT *a_stat, uchar *a_stype, uchar *a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rci         =    0;
   tSTAT       x_stat;
   tSTAT       x_stat_link;
   char        x_type      = ENTRY_REG;
   char        x_link      = STYPE_NORM;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_snote   (a_name);
   DEBUG_ENVI   yLOG_spoint  (a_stat);
   --rce;  if (a_stat == NULL) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_stat = *a_stat;
   /*---(type)---------------------------*/
   if      (S_ISDIR (x_stat.st_mode))     x_type = ENTRY_DIR;
   else if (S_ISCHR (x_stat.st_mode))     x_type = ENTRY_CDEV;
   else if (S_ISBLK (x_stat.st_mode))     x_type = ENTRY_BDEV;
   else if (S_ISFIFO(x_stat.st_mode))     x_type = ENTRY_FIFO;
   else if (S_ISSOCK(x_stat.st_mode))     x_type = ENTRY_SOCK;
   else if (S_ISREG (x_stat.st_mode))     x_type = ENTRY_REG;
   else if (S_ISLNK (x_stat.st_mode))     x_type = ENTRY_LINK;
   else                                   x_type = ENTRY_HUH;
   DEBUG_ENVI   yLOG_schar   (x_type);
   /*---(update type, if link)-----------*/
   if (x_type == ENTRY_LINK) {
      x_link = STYPE_LINK;
      stat (a_name, &x_stat_link);
      if      (S_ISDIR (x_stat_link.st_mode))  x_type = ENTRY_DIR;
      else if (S_ISCHR (x_stat_link.st_mode))  x_type = ENTRY_CDEV;
      else if (S_ISBLK (x_stat_link.st_mode))  x_type = ENTRY_BDEV;
      else if (S_ISFIFO(x_stat_link.st_mode))  x_type = ENTRY_FIFO;
      else if (S_ISSOCK(x_stat_link.st_mode))  x_type = ENTRY_SOCK;
      else if (S_ISREG (x_stat_link.st_mode))  x_type = ENTRY_REG;
      else if (S_ISLNK (x_stat_link.st_mode))  x_type = ENTRY_LINK;
      else                                     x_type = ENTRY_HUH;
   }
   DEBUG_ENVI   yLOG_schar   (x_link);
   /*---(save back)----------------------*/
   if (a_stype != NULL)  *a_stype = x_link;
   if (a_type  != NULL)  *a_type  = x_type;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ENTRY__perms_check      (tSTAT *a_stat, ushort *a_uid, char *a_own, ushort *a_gid, char *a_grp, char *a_oth, char *a_super)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tSTAT       x_stat;
   char        s           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_spoint  (a_stat);
   --rce;  if (a_stat == NULL) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_stat = *a_stat;
   /*---(uid and gid)--------------------*/
   if (a_uid != NULL)  *a_uid   = x_stat.st_uid;
   if (a_gid != NULL)  *a_gid   = x_stat.st_gid;
   /*---(mode)---------------------------*/
   sprintf (s, "%-10.10o", x_stat.st_mode);
   if (a_own != NULL)  *a_own = s [7] - '0';
   if (a_grp != NULL)  *a_grp = s [8] - '0';
   if (a_oth != NULL)  *a_oth = s [9] - '0';
   /*---(super mode)---------------------*/
   if (a_super != NULL) {
      if      (x_stat.st_mode & S_ISUID)   *a_super = '*';
      else if (x_stat.st_mode & S_ISGID)   *a_super = '!';
      else                                 *a_super = '-';
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ENTRY__mime_check       (cchar *a_full, cchar *a_name, tSTAT *a_stat, char a_stype, char a_type, char *a_ext, char *a_cat, long a_bytes)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tSTAT       x_stat;
   char        x_name      [LEN_RECD]  = "";
   char        x_ext       [LEN_TERSE] = "";
   char        x_cat       = MIME_HUH;
   int         x_index     = -1;
   char       *p           = NULL;
   int         x_len       = 0;
   int         i           = 0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_name"    , a_name);
   DEBUG_ENVI   yLOG_point   ("a_stat"    , a_stat);
   --rce;  if (a_stat == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_stat = *a_stat;
   /*---(prepare)-------------------------*/
   strlcpy (x_name, a_name, LEN_RECD);
   x_len = strlen (x_name);
   /*---(special types)-------------------*/
   if        (a_stype == STYPE_LINK) {
      DEBUG_ENVI   yLOG_note    ("symlink entry");
      switch (a_type) {
      case   ENTRY_DIR    :  p = EXT_DLINK;      break;
      case   ENTRY_REG    :  p = EXT_RLINK;      break;
      case   ENTRY_CDEV   :  p = EXT_CLINK;      break;
      case   ENTRY_BDEV   :  p = EXT_BLINK;      break;
      case   ENTRY_FIFO   :  p = EXT_FLINK;      break;
      case   ENTRY_SOCK   :  p = EXT_SLINK;      break;
      default             :  p = EXT_ULINK;      break;
      }
      rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
   } else if (strchr (ENTRY_DEVICES, a_type) != NULL) {
      DEBUG_ENVI   yLOG_note    ("device entry");
      switch (a_type) {
      case   ENTRY_DIR    :  p = EXT_DIR;        break;
      case   ENTRY_CDEV   :  p = EXT_CDEV;       break;
      case   ENTRY_BDEV   :  p = EXT_BDEV;       break;
      case   ENTRY_FIFO   :  p = EXT_FIFO;       break;
      case   ENTRY_SOCK   :  p = EXT_SOCK;       break;
      }
      rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
   } else if (x_name [x_len - 1] == '~') {
      DEBUG_ENVI   yLOG_note    ("backup/cache entry");
      p  = EXT_BACKUP;
      rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
   }
   /*---(get suffix extention)------------*/
   else {
      for (i = 0; i <= 2; ++i) {
         p = strrchr (x_name, '.');
         DEBUG_RPTG   yLOG_complex ("check ."   , "%d %p", i, p);
         if (p == NULL || p == x_name) {  /* bad or hidden file indicator */
            x_len = 0;
            p     = NULL;
            break;
         }
         x_len = strlen (p + 1);
         DEBUG_RPTG   yLOG_complex ("testing"   , "%d[%s]", x_len, p + 1);
         if (x_len <= 7) {
            DEBUG_RPTG   yLOG_snote   (p + 1);
            rc = MIME_find_cat (p + 1, &x_index, &x_cat, a_bytes);
            DEBUG_RPTG   yLOG_value   ("find"      , rc);
            if (rc == 0) {
               ++p;
               break;
            }
         }
         p [0] = '\0';
      }
   }
   /*---(override for executables)--------*/
   if (x_cat == MIME_HUH ||  p == EXT_RLINK) {
      if (  (x_stat.st_mode & S_IXUSR)  ||
            (x_stat.st_mode & S_IXGRP)  ||
            (x_stat.st_mode & S_IXOTH))  {
         DEBUG_ENVI   yLOG_note    ("executable entry");
         if (p == EXT_RLINK)  p  = EXT_ELINK;
         else                 p  = EXT_EXEC;
         rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
      }
   }
   /*---(file the rest properly)----------*/
   if (x_cat == MIME_HUH) {
      if (strncmp (a_full, "/etc/", 5) == 0) {
         DEBUG_ENVI   yLOG_note    ("configuration entry");
         p  = EXT_CONF;
         rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
      } else if (x_len > 0 && x_len <= 7) {
         DEBUG_ENVI   yLOG_note    ("other entry");
         p  = EXT_UNKNOWN;
         rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
      } else {
         DEBUG_ENVI   yLOG_note    ("unknown entry");
         p  = EXT_MYSTERY;
         rc = MIME_find_cat (p, &x_index, &x_cat, a_bytes);
      }
   }
   /*---(save_back)----------------------*/
   DEBUG_RPTG   yLOG_value   ("rc"        , rc);
   if (rc >= 0) {
      DEBUG_RPTG   yLOG_complex ("results"   , "%c, %s", x_cat, p);
      if (a_cat != NULL)  *a_cat  = x_cat;
      if (a_ext != NULL)  strlcpy (a_ext, p, LEN_TERSE);
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

int          /*===[[ create entry statistics ]]===========[ ------ [ ------ ]=*/
ENTRY__populate         (tPTRS *a_ptrs, char *a_full)
{  /*---(locals)-----------+-----------+-*/
   tENTRY      *x_data      = NULL;
   char        rce         = -10;           /* return code for errors         */
   tSTAT       st;                          /* file stats                     */
   tSTAT       st2;                         /* file stats                     */
   char        c           = ' ';           /* generic char                   */
   char        s           [500];           /* generic string                 */
   int         rci         = 0;             /* return code as integer         */
   int         i           = 0;             /* generic iterator               */
   char       *p           = NULL;
   char       *q           = NULL;
   int         x_len       = 0;
   int         rc          = 0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense : null command)---------*/
   DEBUG_ENVI   yLOG_point   ("a_ptrs"    , a_ptrs);
   --rce; if (a_ptrs == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_data = a_ptrs->data;
   DEBUG_ENVI   yLOG_point   ("x_data"    , x_data);
   /*---(defense : null name)------------*/
   DEBUG_ENVI   yLOG_info    ("full_name" , a_full);
   DEBUG_ENVI   yLOG_point   ("name"      , x_data->name);
   --rce; if (x_data->name == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("name"      , x_data->name);
   /*---(get the file information)-------*/
   rci = lstat (a_full, &st);
   DEBUG_ENVI   yLOG_value   ("lstat"     , rci);
   --rce; if (rci < 0) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(name quality)-------------------*/
   rc = ENTRY__name_check (x_data->name, &(x_data->ascii), &(x_data->len));
   --rce; if (rc < 0) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(type)---------------------------*/
   rc = ENTRY__type_check (a_full, &st, &(x_data->stype), &(x_data->type));
   --rce; if (rc < 0) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(perms)--------------------------*/
   rc = ENTRY__perms_check (&st, &(x_data->uid), &(x_data->own), &(x_data->gid), &(x_data->grp), &(x_data->oth), &(x_data->super));
   --rce; if (rc < 0) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(inode)--------------------------*/
   x_data->inode = st.st_ino;
   DEBUG_ENVI   yLOG_value   ("inode"     , x_data->inode);
   if (c == ENTRY_DIR && x_data->inode == 1) {
      x_data->inode = -1;
   }
   if (c == ENTRY_DIR && x_data->inode == 2 && strcmp (x_data->name, "/") != 0) {
      x_data->inode = -2;
   }
   /*---(timestamp)----------------------*/
   x_data->changed    = st.st_mtime;
   DEBUG_ENVI   yLOG_value   ("changed"   , x_data->changed);
   /*---(size)---------------------------*/
   x_data->bytes = st.st_size;
   x_data->cum   = st.st_size;
   DEBUG_ENVI   yLOG_value   ("bytes"     , x_data->bytes);
   sprintf (s, "%d", st.st_size);
   x_data->size  = strlen (s);
   DEBUG_ENVI   yLOG_value   ("exponent"  , x_data->size);
   /*---(mime category)-------------------*/
   rc = ENTRY__mime_check (a_full, x_data->name, &st, x_data->stype, x_data->type, x_data->ext, &(x_data->cat), x_data->bytes);
   --rce; if (rc < 0) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   ++(mime [0].seen);
   mime [0].sbytes += x_data->bytes;
   /*---(complete)------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ENTRY__level_prep       (int a_level, tPTRS *a_parent, char *a_path, char *a_newpath)
{  /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   tENTRY     *x_dir       = NULL;
   char        x_path      [LEN_PATH]  = "";
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVI   yLOG_complex ("args"      , "%2d, %p, %p, %p", a_level, a_parent, a_path, a_newpath);
   /*---(defaults)-----------------------*/
   if (a_newpath != NULL)  strlcpy (a_newpath, "", LEN_PATH);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_value   ("maxlevel"  , my.maxlevel);
   --rce;  if (a_level < 0 || a_level >  my.maxlevel) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_parent == NULL) {
      DEBUG_ENVI   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("data"      , a_parent->data);
   --rce;  if (a_parent->data == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_dir = a_parent->data;
   DEBUG_ENVI   yLOG_point   ("dir_name"  , x_dir->name);
   --rce;  if (x_dir->name == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("dir_name"  , x_dir->name);
   DEBUG_ENVI   yLOG_value   ("dir_len"   , x_dir->len);
   --rce;  if (x_dir->len >= MAX_NAME) {
      DEBUG_ENVI   yLOG_note    ("directory name too long, failed");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_path == NULL) {
      DEBUG_ENVI   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_newpath == NULL) {
      DEBUG_ENVI   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(name directory)-----------------*/
   if      (a_level == 0)  sprintf (x_path, "%s"   , x_dir->name);
   else if (a_level == 1)  sprintf (x_path, "/%s"  , x_dir->name);
   else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);
   x_len = strlen (x_path);
   DEBUG_ENVI   yLOG_complex ("path"      , "%d[%s]", x_len, x_path);
   /*---(save back)-----------------------*/
   if (a_newpath != NULL)  strlcpy (a_newpath, x_path, LEN_PATH);
   /*---(complete)------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ gather entries in dir ]]=============[ ------ [ ------ ]=*/
ENTRY__level_read       (int a_level, tPTRS *a_parent, char *a_path, char a_silent)
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   int         rc          = 0;             /* generic return code            */
   DIR        *x_dir       = NULL;          /* directory pointer              */
   tDIRENT    *x_entry     = NULL;          /* directory entry                */
   tENTRY     *x_parent    = NULL;          /* directory data                 */
   tPTRS      *x_curr      = NULL;          /* current entry                  */
   tENTRY     *x_data      = NULL;          /* current entry                  */
   int         rc2         = 0;             /* generic return code            */
   int         i           = 0;             /* generic iterator               */
   char        x_path      [LEN_PATH];
   char        x_full      [500];
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVI   yLOG_complex ("args"      , "%2d, %p, %p, %c", a_level, a_parent, a_path, a_silent);
   /*---(defense)------------------------*/
   rc = ENTRY__level_prep (a_level, a_parent, a_path, x_path);
   DEBUG_ENVI   yLOG_value   ("prep"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_parent = a_parent->data;
   /*---(filter)-------------------------*/
   rc = CONF_find (x_path, x_parent->name, &(x_parent->stype), &a_silent);
   DEBUG_ENVI   yLOG_value   ("find"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_parent->stype == DIR_PASS || x_parent->stype == DIR_NEVER) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open directory)-----------------*/
   x_dir = opendir (x_path);
   --rce;  if (x_dir == NULL) {
      DEBUG_ENVI   yLOG_note    ("could not open the location");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_ENVI   yLOG_note    ("success, location open");
   /*---(cycle through the entries)------*/
   --rce; while (1) {
      /*---(read)------------------------*/
      x_entry = readdir (x_dir);
      DEBUG_ENVI   yLOG_point   ("x_entry"   , x_entry);
      if (x_entry == NULL)                      break;
      /*---(filter)----------------------*/
      if (x_entry->d_name   == NULL)            continue;
      DEBUG_ENVI   yLOG_info    ("entry"     , x_entry->d_name);
      if (strcmp (x_entry->d_name, ".")  == 0)  continue;
      if (strcmp (x_entry->d_name, "..") == 0)  continue;
      /*---(check for existing)----------*/
      rc = ENTRY__find (a_parent, x_entry->d_name, &x_curr);
      DEBUG_ENVI   yLOG_complex ("find"      , "%d, %p", rc, x_curr);
      /*---(create if required)----------*/
      if (x_curr == NULL) {
         rc = ENTRY_new (&x_curr, a_parent);
         DEBUG_ENVI   yLOG_point   ("new"       , x_curr);
         if (x_curr == NULL)   continue;
      }
      x_data = x_curr->data;
      /*---(get name saved)--------------*/
      strncpy (x_data->name, x_entry->d_name, MAX_NAME - 1);
      x_data->level = a_level;
      x_data->drive = x_curr->parent->data->drive;
      /*---(get attributes)--------------*/
      if (a_level == 1)  sprintf (x_full, "/%s"  , x_entry->d_name);
      else               sprintf (x_full, "%s/%s", x_path, x_entry->d_name);
      rc = ENTRY__populate (x_curr, x_full);
      DEBUG_ENVI   yLOG_value   ("populate"  , rc);
      if (rc < 0) continue;
      /*---(recurse directories)---------*/
      if (x_data->type == ENTRY_DIR) {
         if        (x_data->stype == STYPE_LINK) {
            DEBUG_ENVI   yLOG_note    ("stop, do not recurse on symlink directories");
         } else if (x_parent->stype == DIR_LAST) {
            DEBUG_ENVI   yLOG_note    ("stop, do not recurse as this is dir_last");
         } else {
            DEBUG_ENVI   yLOG_note    ("found a sub-directory that needs exploring");
            rc2 = ENTRY__level_read (a_level + 1, x_curr, x_path, a_silent);
            DEBUG_ENVI   yLOG_value   ("recurse"   , rc);
            if (rc2 < 0) {
               DEBUG_ENVI   yLOG_note    ("hit a dir_never, erase entry");
               continue;
            }
         }
      }
      /*---(update cum)------------------*/
      DEBUG_ENVI   yLOG_note    ("add sizes and counts to mime");
      x_curr->parent->data->cum += x_data->cum;
      ++(mime [rc].kept);
      ++(mime [0 ].kept);
      mime [rc].kbytes += x_curr->data->bytes;
      mime [0 ].kbytes += x_curr->data->bytes;
      /*---(check silent)----------------*/
      DEBUG_ENVI   yLOG_note    ("check for silent treatment");
      if (a_silent == 'y') {
         DEBUG_ENVI   yLOG_note    ("silent entry, deleting after recursing/totalling");
         ENTRY__free (&x_curr);
      }
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   closedir (x_dir);
   /*---(sort)---------------------------*/
   rc = ySORT_troll (YSORT_NONE, YSORT_ASCEND, &(a_parent->sib_head), &(a_parent->sib_tail));
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

tPTRS*       /*--(create base entry)----------------------[ ------ [ ------ ]-*/
ENTRY__root             (tDRIVE *a_drive)
{  /*---(local variables)--+-----------+-*/
   int         rc          = 0;
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(create root)--------------------*/
   rc = ENTRY_new (&x_ptrs, NULL);
   DEBUG_ENVI   yLOG_point   ("root"      , x_ptrs);
   /*---(assign drive)-------------------*/
   strcpy (x_ptrs->data->name, a_drive->mpoint);
   x_ptrs->data->drive = a_drive->ref;
   /*---(fill in root entry)-------------*/
   rc = ENTRY__populate (x_ptrs, my.mpoint);
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
   rc = ENTRY_new (&x_ptrs, a_root);
   DEBUG_ENVI   yLOG_point   ("(empty)"   , x_ptrs);
   DEBUG_ENVI   yLOG_point   ("parent"    , x_ptrs->parent);
   DEBUG_ENVI   yLOG_point   ("->data"    , x_ptrs->parent->data);
   DEBUG_ENVI   yLOG_info    ("->->name"  , x_ptrs->parent->data->name);
   DEBUG_ENVI   yLOG_value   ("->->level" , x_ptrs->parent->data->level);
   DEBUG_ENVI   yLOG_point   ("->tail"    , x_ptrs->parent->sib_tail);
   DEBUG_ENVI   yLOG_point   ("->->data"  , x_ptrs->parent->sib_tail->data);
   DEBUG_ENVI   yLOG_info    ("->->->name", x_ptrs->parent->sib_tail->data->name);
   strcpy (x_ptrs->data->name, "(empty)");
   x_ptrs->data->drive = a_drive->ref;
   x_ptrs->data->type  = ENTRY_DIR;
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
      /*> x_ptrs = DATA_find (x_ptrs, p);                                             <*/
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



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ENTRY__unit             (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   tPTRS      *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "count"         ) == 0) {
      x_curr = h_ptrs; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->next; }
      x_curr = t_ptrs; while (x_curr != NULL) { ++x_back; x_curr = x_curr->prev; }
      snprintf (unit_answer, LEN_FULL, "ENTRY count      : num=%4d, fore=%4d, back=%4d", n_ptrs, x_fore, x_back);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      x_curr  = h_ptrs;
      while (x_curr != NULL) {
         if (c == n)  break;
         ++c;
         x_curr = x_curr->next;
      }
      if (x_curr == NULL) 
         snprintf (unit_answer, LEN_FULL, "ENTRY entry (%2d) : no entry", n);
      else 
         snprintf (unit_answer, LEN_FULL, "ENTRY entry (%2d) : %2d[%-20.20s]   %c %c   %c %-6.6s   %d %-10ld %-10ld   %ld", n,
               x_curr->data->len  , x_curr->data->name , 
               x_curr->data->type , x_curr->data->stype,
               x_curr->data->cat  , x_curr->data->ext  ,
               x_curr->data->size , x_curr->data->bytes, x_curr->data->cum  ,
               x_curr->data->changed);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*===============================[[ end-code ]]===============================*/
