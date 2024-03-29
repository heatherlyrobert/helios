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



tPTRS      *dir_stack   [MAX_DEPTH];
tPTRS      *root_stack  [MAX_DEPTH];

tPTRS     *h_ptrs;
tPTRS     *t_ptrs;
int        g_nptrs;
int        g_uptrs;


int     g_target  =    0;
int     g_looked  =    0;
int     g_matched =    0;
tENTRY *g_found   = NULL;
char    g_path    [LEN_RECD] = "";


static tENTRY *s_start   = NULL;
static char    g_spath   [LEN_RECD] = "";


/*====================------------------------------------====================*/
/*===----                       small support                          ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;}

char         /*===[[ wipe a data payload ]]===============[ ------ [ ------ ]=*/
ENTRY__wipe             (tPTRS *a_curr)
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tENTRY     *x_data      = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_curr == NULL) return rce;
   /*---(header)-------------------------*/
   DEBUG_ENVI_M yLOG_senter  (__FUNCTION__);
   DEBUG_ENVI_M yLOG_spoint  (a_curr);
   /*---(ground pointers)----------------*/
   a_curr->parent     = NULL;
   a_curr->nchild     = 0;
   a_curr->c_head     = NULL;
   a_curr->c_tail     = NULL;
   a_curr->s_prev     = NULL;
   a_curr->s_next     = NULL;
   a_curr->m_prev     = NULL;
   a_curr->m_next     = NULL;
   /*---(wipe data)----------------------*/
   x_data = a_curr->data;
   /*---(clear types)--------------------*/
   x_data->lvl        =   0;
   x_data->type       = ENTRY_REG;
   x_data->stype      = STYPE_NORMAL;
   /*---(clear perms)--------------------*/
   x_data->uid        =   0;
   x_data->own        =   0;
   x_data->gid        =   0;
   x_data->grp        =   0;
   x_data->oth        =   0;
   x_data->super      = '-';
   /*---(unique)-------------------------*/
   x_data->drive      =   0;
   x_data->inode      =  -1;
   x_data->dnode      =  -1;
   /*---(times)--------------------------*/
   x_data->changed    =   0;
   /*---(sizes)--------------------------*/
   x_data->size       =   0;
   x_data->bytes      =   0;
   x_data->bcum       =   0;
   x_data->count      =   0;
   x_data->ccum       =   0;
   /*---(categorization)-----------------*/
   x_data->cat        = MIME_HUH;
   ystrlcpy (x_data->ext, "-", LEN_TERSE);
   /*---(name)---------------------------*/
   x_data->ascii      = '-';
   x_data->len        =   0;
   x_data->name [0]   = '\0';
   /*---(complete)-----------------------*/
   DEBUG_ENVI_M yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     memory management                        ----===*/
/*====================------------------------------------====================*/
static void      o___MALLOC__________________o (void) {;}

char
ENTRY__new              (tPTRS **a_new, tPTRS *a_parent, tDRIVE *a_drive)
{  /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   tPTRS      *x_new       = NULL;          /* current entry                  */
   tENTRY     *x_data      = NULL;          /* current entry                  */
   char        x_tries     = 0;             /* malloc attempts                */
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_new != NULL)  *a_new = NULL;
   /*---(defence)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_parent"  , a_parent);
   DEBUG_ENVI   yLOG_point   ("a_drive"   , a_drive);
   --rce;  if (a_parent == NULL && a_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_drive  != NULL) {
      DEBUG_ENVI   yLOG_point   ("root"      , a_drive->root);
      if (a_drive->root != NULL) {
         DEBUG_ENVI   yLOG_note    ("drives root already assigned");
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      DEBUG_ENVI   yLOG_info    ("parent"    , a_parent->data->name);
   }
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
   ENTRY__wipe (x_new);
   /*---(hook to overall list)-----------*/
   DEBUG_ENVI   yLOG_note    ("hook to overall list");
   x_new->m_next = NULL;
   if (h_ptrs == NULL)  {
      h_ptrs          = x_new;
      x_new->m_prev   = NULL;
   } else {
      t_ptrs->m_next  = x_new;
      x_new->m_prev   = t_ptrs;
   }
   t_ptrs = x_new;
   ++g_nptrs;
   ++g_uptrs;
   /*---(hook to drive)------------------*/
   if (a_drive != NULL) {
      a_drive->root  = x_new;
      x_data->drive  = a_drive->ref;
   }
   /*---(hook to parent dir)-------------*/
   if (a_parent != NULL) {
      if (a_parent->c_head  == NULL) {
         DEBUG_ENVI   yLOG_note    ("first child, add to front");
         a_parent->c_head         = x_new;
         a_parent->c_tail         = x_new;
         a_parent->nchild         = 1;
      } else {
         DEBUG_ENVI   yLOG_note    ("append child, add to end");
         x_new->s_prev            = a_parent->c_tail;
         a_parent->c_tail->s_next = x_new;
         a_parent->c_tail         = x_new;
         ++(a_parent->nchild);
      }
      DEBUG_ENVI   yLOG_value   ("nchild"    , a_parent->nchild);
      x_new->parent  = a_parent;
      x_data->drive  = a_parent->data->drive;
      x_data->lvl    = a_parent->data->lvl + 1;
      x_data->dnode  = a_parent->data->inode;
   } else {
      DEBUG_ENVI   yLOG_note    ("root directory");
      x_data->lvl    = 0;
      x_data->dnode  = 0;
   }
   DEBUG_ENVI   yLOG_value   ("dnode"     , x_data->dnode);
   /*---(save back)----------------------*/
   if (a_new != NULL)  *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ remove a single entry ]]=============[ ------ [ ------ ]=*/
ENTRY__free             (tPTRS **a_ptrs)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   tPTRS      *x_curr      = NULL;
   tPTRS      *x_child     = NULL;
   tPTRS      *x_next      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_ptrs"    , a_ptrs);
   --rce;  if (a_ptrs == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("*a_ptrs"   , *a_ptrs);
   --rce;  if (*a_ptrs == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_curr = *a_ptrs;
   /*---(free data)----------------------*/
   DEBUG_ENVI   yLOG_note    ("free data");
   if (x_curr->data != NULL)  free (x_curr->data);
   /*> printf ("entry destroy %s\n", x_curr->data->name);                             <*/
   /*---(check children)-----------------*/
   DEBUG_ENVI   yLOG_note    ("free children");
   x_child = x_curr->c_head;
   DEBUG_ENVI   yLOG_complex ("child"     , "%3dc, %3dn, %p", c, x_curr->nchild, x_child);
   while (x_child != NULL) {
      x_next  = x_child->s_next;
      ENTRY__free (&x_child);
      DEBUG_ENVI   yLOG_point   ("x_child"   , x_child);
      x_child = x_next;
      ++c;
      DEBUG_ENVI   yLOG_complex ("child"     , "%3dc, %3dn, %p", c, x_curr->nchild, x_child);
   }
   /*---(unhook from siblings)-----------*/
   DEBUG_ENVI   yLOG_point   ("parent"    , x_curr->parent);
   if (x_curr->parent != NULL) {
      DEBUG_ENVI   yLOG_note    ("out of sibs");
      if (x_curr->s_next != NULL)   x_curr->s_next->s_prev = x_curr->s_prev;
      else                          x_curr->parent->c_tail = x_curr->s_prev;
      if (x_curr->s_prev != NULL)   x_curr->s_prev->s_next = x_curr->s_next;
      else                          x_curr->parent->c_head = x_curr->s_next;
      --(x_curr->parent->nchild);
      x_curr->s_next = x_curr->s_prev = NULL;
   }
   /*---(unhook from overall)------------*/
   DEBUG_ENVI   yLOG_note    ("out of master");
   if (x_curr->m_next != NULL)   x_curr->m_next->m_prev = x_curr->m_prev;
   else                          t_ptrs                 = x_curr->m_prev;
   if (x_curr->m_prev != NULL)   x_curr->m_prev->m_next = x_curr->m_next;
   else                          h_ptrs                 = x_curr->m_next;
   x_curr->m_next = x_curr->m_prev = NULL;
   --g_nptrs;
   DEBUG_ENVI   yLOG_complex ("global"   , "%3d, head=%p, tail=%p", g_nptrs, h_ptrs, t_ptrs);
   /*---(ground pointer)-----------------*/
   DEBUG_ENVI   yLOG_note    ("ground");
   free (x_curr);
   *a_ptrs = NULL;
   DEBUG_ENVI   yLOG_point   ("*a_ptrs"   , *a_ptrs);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       simplifiers                            ----===*/
/*====================------------------------------------====================*/
static void      o___SIMPLIFY________________o (void) {;}

char
ENTRY_root     (tPTRS **a_new, tDRIVE *a_drive)
{
   char        rc          =    0;
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   rc = ENTRY__new (a_new, NULL    , a_drive);
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ENTRY_fullroot (tPTRS **a_new, tDRIVE *a_drive)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_rlevel    =    0;
   char        x_slevel    =    0;
   char        x_diff      =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(create the entry)---------------*/
   rc = ENTRY__new (a_new, NULL    , a_drive);
   DEBUG_ENVI   yLOG_value   ("manual"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name that entry)----------------*/
   rc = ENTRY_manual    ((*a_new)->data, a_drive->mpoint, ENTRY_DIR, STYPE_NORMAL, MIME_DIR, "d_dir");
   DEBUG_ENVI   yLOG_value   ("manual"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate statistics)------------*/
   rc = ENTRY__populate ((*a_new), a_drive->mpoint);
   DEBUG_ENVI   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update depth)-------------------*/
   DEBUG_ENVI   yLOG_value   ("maxlevel"  , my.maxlevel);
   if (my.maxlevel < MAX_LEVEL) {
      ystrlbase (a_drive->mpoint, NULL, NULL, NULL, &x_rlevel);
      DEBUG_ENVI   yLOG_complex ("x_rlevel"  , "%2d, %s", x_rlevel, a_drive->mpoint);
      ystrlbase (my.path        , NULL, NULL, NULL, &x_slevel);
      DEBUG_ENVI   yLOG_complex ("x_slevel"  , "%2d, %s", x_slevel, my.path);
      if (x_slevel > x_rlevel)  my.maxlevel = x_slevel - x_rlevel + my.maxlevel;
      if (my.maxlevel > MAX_LEVEL)  my.maxlevel = MAX_LEVEL;
      DEBUG_ENVI   yLOG_value   ("updated"   , my.maxlevel);
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ENTRY_normal   (tPTRS **a_new, tPTRS *a_parent)
{
   char        rc          =    0;
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   rc = ENTRY__new (a_new, a_parent, NULL);
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ENTRY_submount (tPTRS **a_new, tPTRS *a_parent, tDRIVE *a_drive)
{
   char        rc          =    0;
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   return ENTRY__new (a_new, a_parent, a_drive);
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}



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
   h_ptrs     = NULL;
   t_ptrs     = NULL;
   g_nptrs    = 0;
   g_uptrs    = 0;
   /*---(searching)----------------------*/
   g_target   = 0;
   g_looked   = 0;
   g_matched  = 0;
   g_found    = NULL;
   ystrlcpy (g_path, "", LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*===[[ remove full data structure ]]========[ ------ [ ------ ]=*/
ENTRY__purge       (void)
{  /*---(local variables)--+-----------+-*/
   tPTRS      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(pointers)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("head/root" , h_ptrs);
   ENTRY__free (&h_ptrs);
   DEBUG_ENVI   yLOG_point   ("h_ptrs"    , h_ptrs);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ remove full data structure ]]========[ ------ [ ------ ]=*/
ENTRY__dirclear    (tPTRS *a_from)
{  /*---(local variables)--+-----------+-*/
   tPTRS      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(pointers)-----------------------*/
   x_curr = a_from->c_head;
   while (x_curr != NULL) {
      ENTRY__free (&x_curr);
      DEBUG_ENVI   yLOG_point   ("x_curr"    , x_curr);
      x_curr = a_from->c_head;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ENTRY_wrap              (void)
{
   ENTRY__purge ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      verify/collect                          ----===*/
/*====================------------------------------------====================*/
static void      o___CHECKS__________________o (void) {;}

char         /*-> check file name for complexity --------------[ leaf������ ]-*/ /*-�fcg��3�3���᷷�ᷴE17�8���21L12���1�1�������᷷����������������>?eE#�7�7����������11X��-*/
ENTRY__name_check       (cchar *a_name, char *a_warn, uchar *a_len)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_name      [LEN_HUND]  = "";
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
   ystrlcpy (x_name, a_name, LEN_HUND);
   x_len = strlen (a_name);
   DEBUG_ENVI   yLOG_sint    (x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_ENVI   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   if (x_name [x_len - 1] == '~')  x_name [x_len--] = '\0';
   if (x_len == 1 && x_name [0] == '/')  x_len = 0;
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
   char        x_link      = STYPE_NORMAL;
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



/*====================------------------------------------====================*/
/*===----                       populate entries                       ----===*/
/*====================------------------------------------====================*/
static void      o___POPULATE________________o (void) {;}

int          /*===[[ create entry statistics ]]===========[ ------ [ ------ ]=*/
ENTRY__populate         (tPTRS *a_ptrs, char *a_full)
{  /*---(locals)-----------+-----------+-*/
   tENTRY      *x_data      = NULL;
   char        rce         = -10;           /* return code for errors         */
   tSTAT       st;                          /* file stats                     */
   tSTAT       st2;                         /* file stats                     */
   char        c           = ' ';           /* generic char                   */
   char        s           [LEN_LABEL];     /* generic string                 */
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
   x_data->bcum  = st.st_size;
   DEBUG_ENVI   yLOG_value   ("bytes"     , x_data->bytes);
   sprintf (s, "%ld", x_data->bytes);
   x_data->size  = strlen (s);
   if (x_data->bytes < 1)  x_data->size = 0;
   DEBUG_ENVI   yLOG_value   ("exponent"  , x_data->size);
   x_data->count = 1;
   x_data->ccum  = 1;
   /*---(mime category)-------------------*/
   rc = EXT_categorize (a_full, x_data->name, &st, x_data->stype, x_data->type, x_data->bytes, &(x_data->cat), x_data->ext);
   --rce; if (rc < 0) {
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(name trouble fix)----------------*/
   if  (x_data->ascii == ASCII_CRAZY) {
      DEBUG_ENVI   yLOG_note    ("name has unprintable characters, resave");
      ystrlmark (x_data->name, ySTR_PRINT, LEN_HUND);
      DEBUG_ENVI   yLOG_info    ("new_name"  , x_data->name);
   }
   /*---(complete)------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ENTRY_manual       (tENTRY *a_entry, char *a_name, char a_type, char a_stype, char a_cat, char *a_ext)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDRIVE     *x_drive     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_entry"   , a_entry);
   --rce;  if (a_entry == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) < 1) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   ystrlcpy (a_entry->name, a_name, LEN_HUND);
   a_entry->len = strlen (a_name);
   DEBUG_INPT   yLOG_value   ("a_type"    , a_type);
   if (a_type  !=  0)    a_entry->type  = a_type;
   DEBUG_INPT   yLOG_value   ("a_stype"   , a_stype);
   if (a_stype !=  0)    a_entry->stype = a_stype;
   DEBUG_INPT   yLOG_value   ("a_cat"     , a_cat);
   if (a_cat   !=  0)    a_entry->cat = a_cat;
   DEBUG_INPT   yLOG_point   ("a_ext"     , a_ext);
   if (a_ext != NULL) {
      ystrlcpy (a_entry->ext , a_ext, LEN_TERSE);
      DEBUG_INPT   yLOG_info    ("a_ext"     , a_ext);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     act on whole level                       ----===*/
/*====================------------------------------------====================*/
static void      o___LEVEL___________________o (void) {;}

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
   x_ptrs = a_dir->c_head;
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
      x_ptrs = x_ptrs->s_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit   (__FUNCTION__);
   return 0;
}

char
ENTRY__level_prep       (tPTRS *a_parent, char *a_path, char *a_newpath, char *a_begin)
{  /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   tENTRY     *x_dir       = NULL;
   char        x_path      [LEN_PATH]  = "";
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVI   yLOG_complex ("args"      , "%p, %p, %p", a_parent, a_path, a_newpath);
   /*---(defaults)-----------------------*/
   if (a_newpath != NULL)  ystrlcpy (a_newpath, "", LEN_PATH);
   /*---(defense)------------------------*/
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
   DEBUG_ENVI   yLOG_value   ("maxlevel"  , my.maxlevel);
   DEBUG_ENVI   yLOG_value   ("lvl"       , x_dir->lvl);
   --rce;  if (x_dir->lvl < 0 || x_dir->lvl >  my.maxlevel) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (x_dir->lvl == my.maxlevel) {
      DEBUG_ENVI   yLOG_note    ("matches lowest level, do not read this directory");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 1;
   }
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
   --rce;  if (x_dir->lvl != 0 && a_path == NULL) {
      DEBUG_ENVI   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_newpath == NULL) {
      DEBUG_ENVI   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(name directory)-----------------*/
   DEBUG_ENVI   yLOG_info     ("parent"    , x_dir->name);
   if      (x_dir->lvl == 0) {
      DEBUG_ENVI   yLOG_note    ("root of this filesystem");
      sprintf (x_path, "%s"  , x_dir->name);
   } else if (x_dir->lvl == 1 && strcmp (a_path, "/") == 0) {
      DEBUG_ENVI   yLOG_note    ("top level");
      sprintf (x_path, "/%s"  , x_dir->name);
   } else {
      DEBUG_ENVI   yLOG_note    ("below root");
      sprintf (x_path, "%s/%s", a_path, x_dir->name);
   }
   x_len = strlen (x_path);
   DEBUG_ENVI   yLOG_complex ("path"      , "%d[%s]", x_len, x_path);
   /*---(save back)-----------------------*/
   if (a_newpath != NULL)  ystrlcpy (a_newpath, x_path, LEN_PATH);
   /*---(check begin)---------------------*/
   if (strcmp (my.path, "") != 0) {
      if (strncmp (my.path, x_path, strlen (my.path)) == 0) {
         DEBUG_ENVI   yLOG_note    ("--start fulfilled, record everything");
         *a_begin = 'y';
      } else if (strncmp (my.path, x_path, x_len) == 0) {
         DEBUG_ENVI   yLOG_note    ("on path of --start, record this directory only");
         *a_begin = 'd';
      } else {
         DEBUG_ENVI   yLOG_note    ("not on path of --start, record nothing");
         *a_begin = '-';
      }
   } else {
      DEBUG_ENVI   yLOG_note    ("no --start used, record nothing");
      *a_begin = 'y';
   }
   /*---(complete)------------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ gather entries in dir ]]=============[ ------ [ ------ ]=*/
ENTRY__level_read       (tPTRS *a_parent, char *a_path, char a_silent)
{  /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         rc2         =    0;
   DIR        *x_dir       = NULL;          /* directory pointer              */
   tDRIVE     *x_drive     = NULL;
   tDIRENT    *x_entry     = NULL;          /* directory entry                */
   tENTRY     *x_parent    = NULL;          /* directory data                 */
   tPTRS      *x_curr      = NULL;          /* current entry                  */
   tENTRY     *x_data      = NULL;          /* current entry                  */
   int         c           =    0;
   char        x_path      [LEN_PATH];
   char        x_full      [500];
   char        x_begin     =  'y';
   char        x_follow    =  '-';
   char        x_index     =   -1;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVI   yLOG_complex ("args"      , "%p, %p, %c", a_parent, a_path, a_silent);
   /*---(defense)------------------------*/
   rc = ENTRY__level_prep (a_parent, a_path, x_path, &x_begin);
   DEBUG_ENVI   yLOG_value   ("prep"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_value   ("x_begin"   , rc);
   --rce;  if (x_begin == '-' || a_parent->data->stype == STYPE_LINK) {
      DEBUG_ENVI   yLOG_note    ("dir does not fall on --start path do not recurse");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_parent = a_parent->data;
   /*---(check filter)-------------------*/
   rc = CONF_find (x_path, x_parent->name, &(x_parent->stype), &a_silent);
   DEBUG_ENVI   yLOG_value   ("find"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(filter and remove)--------------*/
   --rce;  if (x_parent->stype == STYPE_AVOID_FULL) {
      DEBUG_ENVI   yLOG_note    ("avoid_full, get out and remove");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(filter but retain)--------------*/
   --rce;  if (x_parent->stype == STYPE_AVOID || x_parent->stype == STYPE_AVOID_EVERY) {
      DEBUG_ENVI   yLOG_note    ("avoid and avoid_every, get out and retain");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (x_parent->stype == STYPE_PRIVATE) {
      if (my.pub == 'y') {
         DEBUG_ENVI   yLOG_note    ("private, get out and retain");
         DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
         return 0;
      }
   }
   DEBUG_ENVI   yLOG_complex ("head child", "%s, %s, %dn, %dc", x_path, a_parent->data->name, a_parent->nchild, c);
   /*---(add drive if needed)------------*/
   rc = DRIVE_mtab_find (x_path, &x_drive, NULL, &x_follow, NULL, NULL);
   DEBUG_ENVI   yLOG_complex ("mtab"      , "%p, %c, %d", x_drive, x_follow, rc);
   --rce;  if (rc == 1) {
      if (x_follow == '-') {
         DEBUG_ENVI   yLOG_note    ("not marked to follow (artificial)");
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return 0;
      }
      if (x_drive == NULL)  rc = DRIVE_populate (&x_drive, x_path, my.runtime, &x_index);
      x_parent->drive = x_drive->ref;
   }
   x_drive = x_parent->drive;
   /*---(open directory)-----------------*/
   x_dir = opendir (x_path);
   --rce;  if (x_dir == NULL) {
      DEBUG_ENVI   yLOG_note    ("could not open the location");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_ENVI   yLOG_note    ("success, location open");
   /*---(cycle through the entries)------*/
   --rce; while (1) {
      /*---(read)------------------------*/
      x_entry = readdir (x_dir);
      DEBUG_ENVI   yLOG_point   ("X_ENTRY"   , x_entry);
      if (x_entry == NULL)  {
         DEBUG_ENVI   yLOG_note    ("break at end of list");
         break;
      }
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
         rc = ENTRY_normal (&x_curr, a_parent);
         DEBUG_ENVI   yLOG_point   ("new"       , x_curr);
         if (x_curr == NULL)   continue;
      }
      x_data = x_curr->data;
      ++c;
      DEBUG_ENVI   yLOG_complex ("children"  , "%3dn, %3dc", a_parent->nchild, c);
      /*---(get name saved)--------------*/
      strncpy (x_data->name, x_entry->d_name, MAX_NAME - 1);
      x_data->lvl   = x_curr->parent->data->lvl + 1;
      x_data->drive = x_drive;
      /*---(get attributes)--------------*/
      if (strcmp ("/", x_parent->name) == NULL) 
         sprintf (x_full, "/%s"  , x_entry->d_name);
      else
         sprintf (x_full, "%s/%s", x_path, x_entry->d_name);
      DEBUG_ENVI   yLOG_info    ("x_full"    , x_full);
      rc = ENTRY__populate (x_curr, x_full);
      DEBUG_ENVI   yLOG_value   ("populate"  , rc);
      if (rc < 0) continue;
      /*> printf ("%2d, %-30.30s, %c %c %c, %s\n", x_data->lvl, x_data->name, x_data->type, x_data->stype, a_silent, x_path);   <*/
      /*---(recurse directories)---------*/
      if (x_data->type == ENTRY_DIR) {
         if        (x_begin != 'd' && x_data->stype   == STYPE_LINK) {
            DEBUG_ENVI   yLOG_note    ("stop, do not recurse on symlink directories");
         } else if (x_data->stype   == STYPE_AVOID_FULL) {
            DEBUG_ENVI   yLOG_note    ("stop, do not recurse as this is avoid_full");
         } else if (x_data->stype   == STYPE_SILENT_EVERY) {
            DEBUG_ENVI   yLOG_note    ("stop, do not recurse as this is silent_every");
         } else if (x_parent->stype == STYPE_AVOID_UNDER) {
            DEBUG_ENVI   yLOG_note    ("stop, do not recurse as this is avoid_under");
         } else {
            DEBUG_ENVI   yLOG_note    ("found a silent_under sub-directory that needs exploring");
            if (x_parent->stype == STYPE_SILENT_UNDER || x_parent->stype == STYPE_SILENT_EVERY) {
               rc2 = ENTRY__level_read (x_curr, x_path, 'y');
            } else {
               rc2 = ENTRY__level_read (x_curr, x_path, a_silent);
            }
            DEBUG_ENVI   yLOG_value   ("recurse"   , rc);
            if (rc2 < 0) {
               DEBUG_ENVI   yLOG_note    ("silent_never or avoid_full, no totals, erase entry");
               DEBUG_ENVI   yLOG_info    ("killing"   , x_data->name);
               MIME_del_seen (x_data->ext, x_data->bytes);
               ENTRY__free (&x_curr);
               continue;
            }
         }
      }
      /*---(check silent)----------------*/
      DEBUG_ENVI   yLOG_note    ("check for silent treatment");
      if (x_data->type == ENTRY_DIR && (x_data->stype == STYPE_AVOID_FULL || x_data->stype == STYPE_SILENT_EVERY)) {
         DEBUG_ENVI   yLOG_note    ("avoid_full or silent_every entry, deleting after recursing, remove from total");
         MIME_del_seen (x_data->ext, x_data->bytes);
         ENTRY__free (&x_curr);
      }
      /*---(check before start)----------*/
      else if (x_begin != 'y') {
         if (x_data->type == ENTRY_DIR && x_begin != 'd') {
            DEBUG_ENVI   yLOG_note    ("dir falls outside --start <dir>");
            MIME_del_seen (x_data->ext, x_data->bytes);
            ENTRY__free (&x_curr);
         }
         else if (x_data->type != ENTRY_DIR) {
            DEBUG_ENVI   yLOG_note    ("file/non-dir falls outside --start <dir>");
            MIME_del_seen (x_data->ext, x_data->bytes);
            ENTRY__free (&x_curr);
         }
         else {
            DEBUG_ENVI   yLOG_note    ("falls in line before --start");
            MIME_del_seen (x_data->ext, x_data->bytes);
            x_curr->parent->data->bytes = 0.0;
            x_curr->parent->data->count = 0;
            x_curr->parent->data->bcum  = x_data->bcum;
            x_curr->parent->data->ccum  = x_data->ccum;
         }
      }
      /*---(check on kept)------------------*/
      else {
         /*---(update cumulatives)----------*/
         DEBUG_ENVI   yLOG_note    ("add sizes to dir structure");
         x_curr->parent->data->bcum += x_data->bcum;
         x_curr->parent->data->ccum += x_data->ccum;
         switch (a_silent) {
         case  'p' :
            DEBUG_ENVI   yLOG_note    ("dir_silent entry, deleting after recursing, leave in total");
            DEBUG_ENVI   yLOG_info    ("killing"   , x_data->name);
            MIME_del_seen (x_data->ext, x_data->bytes);
            ystrlcpy (x_data->ext, EXT_PRIVATE, LEN_TERSE);
            MIME_add_seen (EXT_PRIVATE, x_data->bytes, NULL);
            ENTRY__free (&x_curr);
            break;
         case  'y' :
            DEBUG_ENVI   yLOG_note    ("dir_silent entry, deleting after recursing, leave in total");
            DEBUG_ENVI   yLOG_info    ("killing"   , x_data->name);
            ENTRY__free (&x_curr);
            break;
         default   :
            MIME_add_kept (x_data->ext, x_data->bytes);
            break;
         }
      }
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   closedir (x_dir);
   DEBUG_ENVI   yLOG_complex ("tail child", "%s, %s, %dn, %dc", x_path, a_parent->data->name, a_parent->nchild, c);
   /*---(sort)---------------------------*/
   DEBUG_ENVI   yLOG_complex ("for sort"  , "%p, %p", a_parent->c_head, a_parent->c_tail);
   rc = ySORT_troll (YSORT_NONE, YSORT_ASCEND, &(a_parent->c_head), &(a_parent->c_tail));
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         main driver                          ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

/*> char         /+--(create base entry)----------------------[ ------ [ ------ ]-+/   <* 
 *> ENTRY_root              (tPTRS **a_root, tDRIVE *a_drive)                          <* 
 *> {  /+---(local variables)--+-----------+-+/                                        <* 
 *>    char        rce         =  -10;                                                 <* 
 *>    int         rc          =    0;                                                 <* 
 *>    tPTRS      *x_root      = NULL;                                                 <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_ENVI   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(defense)------------------------+/                                        <* 
 *>    DEBUG_ENVI   yLOG_point   ("a_drive"   , a_drive);                              <* 
 *>    --rce;  if (a_drive == NULL) {                                                  <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    DEBUG_ENVI   yLOG_point   ("a_root"    , a_root);                               <* 
 *>    --rce;  if (a_root == NULL) {                                                   <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(create root)--------------------+/                                        <* 
 *>    rc = ENTRY__new (&x_root, NULL, a_drive);                                        <* 
 *>    DEBUG_ENVI   yLOG_value   ("new"       , rc);                                   <* 
 *>    DEBUG_ENVI   yLOG_point   ("x_root"    , x_root);                               <* 
 *>    --rce;  if (rc < 0 || x_root == NULL) {                                         <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    DEBUG_ENVI   yLOG_point   ("data"      , x_root->data);                         <* 
 *>    --rce;  if (x_root->data == NULL) {                                             <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(fill in root entry)-------------+/                                        <* 
 *>    rc = ENTRY__populate (x_root, a_drive->mpoint);                                 <* 
 *>    DEBUG_ENVI   yLOG_value   ("populate"  , rc);                                   <* 
 *>    --rce;  if (rc < 0) {                                                           <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(save back)----------------------+/                                        <* 
 *>    *a_root = x_root;                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_ENVI   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char         /*--(create base entry)----------------------[ ------ [ ------ ]-*/
ENTRY_tail         (tDRIVE *a_drive, tPTRS *a_root)
{  /*---(local variables)--+-----------+-*/
   char        rc          = 0;
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   rc = ENTRY_normal (&x_ptrs, a_root);
   /*> DEBUG_ENVI   yLOG_point   ("(empty)"   , x_ptrs);                               <* 
    *> DEBUG_ENVI   yLOG_point   ("parent"    , x_ptrs->parent);                       <* 
    *> DEBUG_ENVI   yLOG_point   ("->data"    , x_ptrs->parent->data);                 <* 
    *> DEBUG_ENVI   yLOG_info    ("->->name"  , x_ptrs->parent->data->name);           <* 
    *> DEBUG_ENVI   yLOG_value   ("->->lvl"   , x_ptrs->parent->data->lvl);            <* 
    *> DEBUG_ENVI   yLOG_point   ("->tail"    , x_ptrs->parent->c_tail);               <* 
    *> DEBUG_ENVI   yLOG_point   ("->->data"  , x_ptrs->parent->c_tail->data);         <* 
    *> DEBUG_ENVI   yLOG_info    ("->->->name", x_ptrs->parent->c_tail->data->name);   <*/
   strcpy (x_ptrs->data->name, "((empty))");
   x_ptrs->data->drive = a_drive->ref;
   x_ptrs->data->type  = ENTRY_DIR;
   x_ptrs->data->lvl   =   1;
   x_ptrs->data->bytes = a_drive->size - a_root->data->bcum;
   x_ptrs->data->bcum  = a_drive->size - a_root->data->bcum;
   a_root->data->bcum  = a_drive->size;
   x_ptrs->data->cat   = MIME_EMPTY;
   strcpy (x_ptrs->data->ext, "e_empty");
   MIME_add_seen (x_ptrs->data->ext, x_ptrs->data->bytes, NULL);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ENTRY__callback   (char a_serious, tENTRY *a_data, char *a_full)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_complex ("_callback" , "%p, %p", a_data, a_full);
   /*---(defense)------------------------*/
   --rce;  if (a_data == NULL) {
      return rce;
   }
   --rce;  if (a_full == NULL) {
      return rce;
   }
   /*---(filter non-dirs)----------------*/
   if (a_data->type != ENTRY_DIR)       return 0;
   /*---(check for path)-----------------*/
   if (my.path [0] != a_full [0])       return 0;
   if (strcmp (my.path, a_full) != 0)   return 0;
   ystrlcpy (g_spath, a_full, LEN_RECD);
   DEBUG_DATA   yLOG_complex ("final"     , "%s, %s, %s", a_data->name, a_full, g_spath);
   /*---(complete)-----------------------*/
   return 1;
}

char
ENTRY_start             (void)
{
   /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_start = NULL;
   ystrlcpy (g_spath, "", LEN_RECD);
   /*---(defense)------------------------*/
   if (strcmp (my.path, "") == 0) {
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DATA   yLOG_info    ("my.path"  , my.path);
   --rce;  if (my.path [0] != '/') {
      fprintf (stderr, "ERROR, start directory [--start %s] not absolute, ie, doesn't begin with '/'\n", my.path);
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find)---------------------------*/
   rc = ENTRY_walk (WALK_FIRST, ENTRY__callback);
   /*---(error handling)-----------------*/
   --rce;  if (rc <= 0) {
      fprintf (stderr, "ERROR, start directory [--start %s] not found in database\n", my.path);
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return rc;
}

/*> tPTRS*       /+===[[ process a starting point ]]==========[ ------ [ ------ ]=+/            <* 
 *> DATA_start         (                                                                        <* 
 *>       /+---(formal parameters)+-------------+---------------------------------+/            <* 
 *>       char       *a_path      )             /+ full path of directory to scan +/            <* 
 *> {  /+---(local variables)--+-----------+-+/                                                 <* 
 *>    char        rce         = -10;           /+ return code for errors         +/            <* 
 *>    tPTRS      *x_ptrs      = NULL;          /+ current entry                  +/            <* 
 *>    char        x_path      [MAX_NAME];      /+ strtok working copy            +/            <* 
 *>    char       *p           = NULL;          /+ strtok current pointer         +/            <* 
 *>    char       *q           = "/";           /+ strtok delimiters              +/            <* 
 *>    char       *s           = NULL;          /+ strtok context                 +/            <* 
 *>    /+---(header)-------------------------+/                                                 <* 
 *>    DEBUG_ARGS   yLOG_enter   (__FUNCTION__);                                                <* 
 *>    /+---(defense)------------------------+/                                                 <* 
 *>    DEBUG_ARGS   yLOG_note    ("get rocking");                                               <* 
 *>    DEBUG_ARGS   yLOG_point   ("a_path"    , a_path);                                        <* 
 *>    --rce;  if (a_path == NULL) {                                                            <* 
 *>       DEBUG_ARGS   yLOG_note    ("start path NULL, return");                                <* 
 *>       DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                             <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    DEBUG_ARGS   yLOG_info    ("a_path"    , a_path);                                        <* 
 *>    --rce;  if (a_path [0] != '/') {                                                         <* 
 *>       DEBUG_ARGS   yLOG_note    ("start path not absolute, return");                        <* 
 *>       DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                             <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(root)---------------------------+/                                                 <* 
 *>    x_ptrs = h_ptrs;                                                                         <* 
 *>    --rce;  if (x_ptrs->data == NULL) {                                                      <* 
 *>       DEBUG_ARGS   yLOG_note    ("root does not have data, return");                        <* 
 *>       DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                             <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    --rce;  if (x_ptrs->data->name == NULL) {                                                <* 
 *>       DEBUG_ARGS   yLOG_note    ("root does not have a name, return");                      <* 
 *>       DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                             <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    --rce;  if (strcmp (x_ptrs->data->name, "/") != 0) {                                     <* 
 *>       DEBUG_ARGS   yLOG_note    ("root not /, return");                                     <* 
 *>       DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                             <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    my.level = 0;                                                                            <* 
 *>    root_stack [my.level] = x_ptrs;                                                          <* 
 *>    /+---(process)------------------------+/                                                 <* 
 *>    strcpy   (x_path, a_path + 1);                                                           <* 
 *>    p = strtok_r (x_path, q, &s);                                                            <* 
 *>    --rce;                                                                                   <* 
 *>    while (p != NULL) {                                                                      <* 
 *>       DEBUG_ENVI   yLOG_info    ("segment"   , p);                                          <* 
 *>       /+> x_ptrs = DATA_find (x_ptrs, p);                                             <+/   <* 
 *>       DEBUG_ENVI   yLOG_point   ("x_ptrs"    , x_ptrs);                                     <* 
 *>       if (x_ptrs == NULL) {                                                                 <* 
 *>          DEBUG_ARGS   yLOG_note    ("segment not found, return");                           <* 
 *>          DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                          <* 
 *>          return rce;                                                                        <* 
 *>       }                                                                                     <* 
 *>       ++my.level;                                                                           <* 
 *>       root_stack [my.level] = x_ptrs;                                                       <* 
 *>       p = strtok_r (NULL  , q, &s);                                                         <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    DEBUG_ARGS   yLOG_exit    (__FUNCTION__);                                                <* 
 *>    return x_ptrs;                                                                           <* 
 *> }                                                                                           <*/



/*====================------------------------------------====================*/
/*===----                     data stucture walker                     ----===*/
/*====================------------------------------------====================*/
static void      o___WALKER__________________o (void) {;}

ENTRY__walk_handler     (char a_trigger, tPTRS *a_curr, tPTRS *a_parent, char *a_path, char *a_full, void *a_callback)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tENTRY     *x_data      = NULL;
   char      (*x_callback) (char a_serious, tENTRY *a_entry, char *a_path);
   char        x_serious   =  '-';
   /*---(prepare)------------------------*/
   x_callback = a_callback;
   x_data = a_curr->data;
   if (x_data->lvl == 0) 
      sprintf (a_full, "%s"   ,         x_data->name);
   else if (x_data->lvl == 1 && a_parent != NULL && strcmp (a_parent->data->name, "/") == 0)
      sprintf (a_full, "/%s"  ,         x_data->name);
   else
      sprintf (a_full, "%s/%s", a_path, x_data->name);
   /*---(callback)-----------------------*/
   DEBUG_DATA   yLOG_complex ("prep2call" , "%-20.20s, %2d, %p, %s, %s", x_data->name, x_data->lvl, a_parent, a_path, a_full);
   switch (a_trigger) {
   case WALK_ALL     : case WALK_FIRST : case WALK_UPTO :
      x_serious = 'y';
      break;
   case WALK_INDEXED :
      if (g_matched == g_target)  x_serious = 'y';
      break;
   }
   rc = x_callback (x_serious, x_data, a_full);
   ++g_looked;
   /*---(check result)-------------------*/
   switch (a_trigger) {
   case WALK_ALL     : 
      if (rc == 1) {
         DEBUG_DATA   yLOG_note    ("WALK_ALL mode, found one, continue on");
      }
      break;
   case WALK_FIRST   : 
      if (rc == 1) {
         DEBUG_DATA   yLOG_note    ("WALK_FIRST mode, found, stopping");
         s_start = a_curr;
         return 1;
      }
      break;
   case WALK_INDEXED :
      if (x_serious == 'y' && rc == 1) {
         DEBUG_DATA   yLOG_note    ("WALK_INDEXED mode, found index, stopping");
         return 1;
      }
      break;
   case WALK_UPTO    :
      if (g_matched == g_target) {
         DEBUG_DATA   yLOG_note    ("WALK_UPTO mode, found enough, stopping");
         return 1;
      }
      break;
   }
   if (rc == 1)  ++g_matched;
   /*---(complete)-----------------------*/
   return 0;
}

char
ENTRY__walker            (char a_trigger, tPTRS *a_dir, char *a_path, void *a_callback)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPTRS      *x_curr      = NULL;
   tENTRY     *x_data      = NULL;
   char        x_path      [LEN_RECD];
   char        x_full      [LEN_RECD];
   int         c           =    0;
   char        x_priv      =    0;
   char        l           =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_complex ("args"      , "%c, %2d, %s, %s", a_trigger, a_dir->data->lvl, a_dir->data->name, a_path);
   /*---(check if private)---------------*/
   x_priv = CONF_private (a_path);
   --rce;  if (x_priv < 0) {
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(check root as necessary)--------*/
   if (a_dir == h_ptrs) {
      rc = ENTRY__walk_handler (a_trigger, a_dir, NULL, a_path, x_full, a_callback);
      DEBUG_DATA   yLOG_value   ("handler"   , rc);
   /*> } else {                                                                       <*/
      /*> ystrlbase (a_path, x_path, NULL, NULL, NULL);                                    <* 
       *> l = strlen (x_path);                                                             <* 
       *> if (l > 1)  x_path [--l] = '\0';                                                 <* 
       *> rc = ENTRY__walk_handler (a_trigger, a_dir, NULL, x_path, x_full, a_callback);   <*/
      /*> DEBUG_DATA   yLOG_value   ("handler"   , rc);                               <*/
   }
   /*---(return if found)----------------*/
   if (rc == 1) {
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(spin through entries)-----------*/
   DEBUG_DATA   yLOG_complex ("ENTRY"     , "%3d of %3d for %s, %p", c, a_dir->nchild, a_dir->data->name, a_dir->c_head);
   x_curr = a_dir->c_head;
   while (x_curr != NULL && rc != 1) {
      x_data = x_curr->data;
      if (x_data != NULL) {
         rc = ENTRY__walk_handler (a_trigger, x_curr, x_curr->parent, a_path, x_full, a_callback);
         DEBUG_DATA   yLOG_value   ("handler"   , rc);
         if (rc == 1) break;
         /*---(dive on dirs)-------------*/
         if (x_data->type == ENTRY_DIR && x_data->lvl < my.maxlevel) {
            rc = FILTER_by_dir  (x_data->uid, x_data->gid, x_data->own, x_data->grp, x_data->oth, NULL);
            if (rc > 0) {
               rc = ENTRY__walker (a_trigger, x_curr, x_full, a_callback);
               DEBUG_DATA   yLOG_value   ("walker"    , rc);
               if (rc == 1) break;
            }
         }
         /*---(done)---------------------*/
      }
      ++c;
      DEBUG_DATA   yLOG_complex ("ENTRY"     , "%3d of %3d for %s, %p", c, a_dir->nchild, a_dir->data->name, x_curr->s_next);
      x_curr = x_curr->s_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ENTRY_walk              (char a_trigger, void *a_callback)
{
   char        x_path      [LEN_RECD]  = "";
   char        rc          =    0;
   tPTRS      *x_begin     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(reset)--------------------------*/
   g_looked   = 0;
   g_matched  = 0;
   g_found    = NULL;
   ystrlcpy (g_path, "", LEN_RECD);
   /*---(execute walk)-------------------*/
   DEBUG_DATA   yLOG_point   ("h_ptrs"    , h_ptrs);
   DEBUG_DATA   yLOG_point   ("s_start"   , s_start);
   if (s_start != NULL) {
      DEBUG_DATA   yLOG_note    ("start has been identified");
      x_begin = s_start;
      ystrlcpy (x_path, g_spath, LEN_RECD);
   } else if (h_ptrs != NULL) {
      DEBUG_DATA   yLOG_note    ("no start, use head");
      x_begin = h_ptrs;
      ystrlcpy (x_path, h_ptrs->data->name, LEN_RECD);
   }
   DEBUG_DATA   yLOG_info    ("x_path"    , x_path);
   rc = ENTRY__walker (a_trigger, x_begin, x_path, a_callback);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return rc;
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
      x_curr = h_ptrs; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->m_next; }
      x_curr = t_ptrs; while (x_curr != NULL) { ++x_back; x_curr = x_curr->m_prev; }
      snprintf (unit_answer, LEN_FULL, "ENTRY count      : num=%4d, fore=%4d, back=%4d", g_nptrs, x_fore, x_back);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      x_curr  = h_ptrs;
      while (x_curr != NULL) {
         if (c == n)  break;
         ++c;
         x_curr = x_curr->m_next;
      }
      if (x_curr == NULL) 
         snprintf (unit_answer, LEN_FULL, "ENTRY entry (%2d) : no entry", n);
      else 
         snprintf (unit_answer, LEN_FULL, "ENTRY entry (%2d) : %-2d  %-2d %2d[%-20.20s]   %c %c   %c %-6.6s   %d %-6ld   %2d %2d", n,
               x_curr->data->drive ,x_curr->data->lvl   ,
               x_curr->data->len   , x_curr->data->name , 
               x_curr->data->type  , x_curr->data->stype,
               x_curr->data->cat   , x_curr->data->ext  ,
               x_curr->data->size  , x_curr->data->bytes,
               x_curr->data->count , x_curr->data->ccum  );
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
TREE__unit_callback     (char a_serious, tENTRY *a_data, char *a_path)
{
   DEBUG_DATA   yLOG_complex ("entry"     , "%2d, %2d, %-20.20s", g_matched, g_target, a_data->name);
   if (a_serious == 'y') {
      g_found = a_data;
      ystrlcpy (g_path, a_path, LEN_RECD);
   }
   return 1;
}

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
TREE__unit              (tPTRS *a_focus, char *a_question, int n)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   tPTRS      *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "TREE             : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "nchild"        ) == 0) {
      if (a_focus == NULL) {
         snprintf (unit_answer, LEN_FULL, "TREE nchild      : no such parent");
      } else {
         x_curr = a_focus->c_head; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->s_next; }
         x_curr = a_focus->c_tail; while (x_curr != NULL) { ++x_back; x_curr = x_curr->s_prev; }
         if (a_focus->nchild == 0) {
            snprintf (unit_answer, LEN_FULL, "TREE nchild      : %2dn, %2df, %2db    [%-20s] [%-20s]", a_focus->nchild, x_fore, x_back, "-", "-");
         } else {
            snprintf (unit_answer, LEN_FULL, "TREE nchild      : %2dn, %2df, %2db    [%-20s] [%-20s]", a_focus->nchild, x_fore, x_back, a_focus->c_head->data->name, a_focus->c_tail->data->name);
         }
      }
   }
   else if (strcmp (a_question, "walk"          ) == 0) {
      g_target = n;
      rc = ENTRY_walk (WALK_INDEXED, TREE__unit_callback);
      if (rc < 1)   snprintf (unit_answer, LEN_FULL, "TREE walk   (%2d) : no such entry", n);
      else          snprintf (unit_answer, LEN_FULL, "TREE walk   (%2d) : %-2d  %-2d %2d[%-20.20s]   %c %c   %c %-6.6s   [%.40s]", n,
            g_found->drive, g_found->lvl  ,
            g_found->len  , g_found->name , 
            g_found->type , g_found->stype,
            g_found->cat  , g_found->ext  ,
            g_path);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*===============================[[ end-code ]]===============================*/
