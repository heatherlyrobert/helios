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
 * METIS § mv··· § use fstab to identify true / partition                                 § N9L1g1 §  · §
 *
 */


tDRIVE    *h_drive = NULL;                  /* drive tail entry               */
tDRIVE    *t_drive = NULL;                  /* drive tail entry               */
short      g_ndrive = 0;                    /* count of active drives         */
short      g_udrive = -1;                   /* max drive number used (ever)   */

#define    MAX_MOUNTS      50
typedef struct cMTAB tMTAB;
struct cMTAB {
   char        follow;
   char        device      [LEN_LABEL];
   char        path        [LEN_PATH];
   char        len;
   char        type        [LEN_LABEL];
   tDRIVE     *drive;
};
static tMTAB    s_mtabs [MAX_MOUNTS];
static int      s_nmtab    = 0;



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
DRIVE_init         (void)
{  /*---(local variables)--+-----------+-*/
   int         rc          = 0;
   int         i           = 0;
   h_drive = NULL;
   t_drive = NULL;
   g_ndrive = 0;
   g_udrive = -1;
   rc = gethostname (my.host, MAX_STR);
   /*---(mtab)---------------------------*/
   s_nmtab = 0;
   for (i = 0; i < MAX_MOUNTS; ++i) {
      s_mtabs [i].follow = '-';
      ystrlcpy (s_mtabs [i].device, "", LEN_LABEL);
      ystrlcpy (s_mtabs [i].path  , "", LEN_PATH);
      s_mtabs [i].len    = 0;
      ystrlcpy (s_mtabs [i].type  , "", LEN_LABEL);
      s_mtabs [i].drive  = NULL;
   }
   DRIVE__mtab_read ();
   /*---(complete)-----------------------*/
   return rc;
}

char
DRIVE_wrap              (void)
{
   return DRIVE__purge ();
}

char
DRIVE__fstab_root       (char r_root [LEN_LABEL])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD];
   char       *p;
   char       *q           = " ";
   char       *r           = NULL;
   int         x_len       =   0;
   char        x_root      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   strcpy (r_root, "");
   /*---(open mtab file)-----------------*/
   f = fopen ("/etc/fstab", "r");
   DEBUG_ENVI   yLOG_point   ("mtab"      , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find mount point)---------------*/
   DEBUG_ENVI   yLOG_note    ("find mount point");
   --rce;  while (1) {
      /*---(get next)--------------------*/
      fgets (x_recd, LEN_RECD, f);
      if (feof(f)) {
         DEBUG_ENVI   yLOG_note    ("hit EOF before finding mountpoint");
         rc = rce;
         break;
      }
      /*---(filter)----------------------*/
      x_len = strlen (x_recd);
      if (x_len <= 0)                       continue;
      x_recd [--x_len] = '\0';
      /*---(check partition)-------------*/
      p = strtok_r (x_recd, q, &r);
      if (p == NULL)                        continue;
      ystrlcpy  (x_root, p, LEN_LABEL);
      ystrltrim (x_root, ySTR_BOTH, LEN_LABEL);
      /*---(match mountpoint)------------*/
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ystrltrim (p, ySTR_BOTH, LEN_HUND);
      DEBUG_ENVI   yLOG_info    ("mpoint"    , p);
      if (strcmp (p, "/") != 0)  continue;
      /*---(done)------------------------*/
      ystrlcpy (r_root, x_root, LEN_LABEL);
      break;
   }
   /*---(close mtab file)----------------*/
   fclose (f);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
DRIVE__mtab_read   (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   FILE       *f           = NULL;
   int         fd          =   -1;
   char        x_recd      [LEN_RECD];
   char       *p; char       *q           = " ";
   char       *r           = NULL;
   int         x_len       =   0;
   char        x_root      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(open mtab file)-----------------*/
   f = fopen ("/etc/mtab", "r");
   DEBUG_ENVI   yLOG_point   ("mtab"      , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find mount point)---------------*/
   DEBUG_ENVI   yLOG_note    ("find mount point");
   --rce;  while (1) {
      /*---(get next)--------------------*/
      fgets (x_recd, LEN_RECD, f);
      if (feof(f)) {
         DEBUG_ENVI   yLOG_note    ("hit EOF before finding mountpoint");
         rc = rce;
         break;
      }
      /*---(filter)----------------------*/
      x_len = strlen (x_recd);
      if (x_len <= 0)                       continue;
      x_recd [--x_len] = '\0';
      /*---(check partition)-------------*/
      p = strtok_r (x_recd, q, &r);
      if (p == NULL)                        continue;
      ystrltrim (p, ySTR_BOTH, LEN_FULL);
      DEBUG_ENVI   yLOG_info    ("device"    , p);
      /*> if (p [0] != '/')  continue;                                                <*/
      if        (strcmp (p, "proc") == 0){
         DEBUG_ENVI   yLOG_note    ("proc, can not reasonably follow (wonderland)");
         s_mtabs [s_nmtab].follow = '-';
         continue;
      } else if (strcmp (p, "sys") == 0){
         DEBUG_ENVI   yLOG_note    ("sys, can not reasonably follow (wonderland)");
         s_mtabs [s_nmtab].follow = '-';
         continue;
      } else if (strcmp (p, "shm") == 0){
         DEBUG_ENVI   yLOG_note    ("dev/shm, can not reasonably follow (wonderland)");
         s_mtabs [s_nmtab].follow = '-';
         continue;
      } else if (p[0] != '/')  {
         DEBUG_ENVI   yLOG_note    ("not a device entry, but can follow");
         s_mtabs [s_nmtab].follow = 'y';
         continue;
      } else {
         DEBUG_ENVI   yLOG_note    ("device entry");
         s_mtabs [s_nmtab].follow = 'y';
      }
      ystrlcpy (s_mtabs [s_nmtab].device, p, LEN_LABEL);
      /*---(fix root)--------------------*/
      if (strcmp (p, "/dev/root") == 0) {
         DRIVE__fstab_root (x_root);
         if (strcmp (x_root, "") != 0) {
            ystrlcpy (s_mtabs [s_nmtab].device, x_root, LEN_LABEL);
         }
      }
      /*---(match mountpoint)------------*/
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ystrltrim (p, ySTR_BOTH, LEN_HUND);
      DEBUG_ENVI   yLOG_info    ("mpoint"    , p);
      ystrlcpy (s_mtabs [s_nmtab].path  , p, LEN_PATH);
      s_mtabs [s_nmtab].len = strlen (p);
      /*---(gather type)-----------------*/
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ystrltrim (p, ySTR_BOTH, LEN_LABEL);
      DEBUG_ENVI   yLOG_info    ("type"      , p);
      ystrlcpy (s_mtabs [s_nmtab].type  , p, LEN_LABEL);
      /*---(done)------------------------*/
      ++s_nmtab;
   }
   /*---(close mtab file)----------------*/
   fclose (f);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
DRIVE_mtab_find         (cchar *a_path, tDRIVE **a_drive, char *a_mtab, char *a_follow, char *a_device, char *a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =    0;
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   DEBUG_ENVI   yLOG_complex ("args"      , "%pp, %pd, %pm, %pf, %pd, %pt", a_path, a_drive, a_mtab, a_follow, a_device, a_type);
   /*---(prepare)------------------------*/
   if (a_drive  != NULL)   *a_drive = NULL;
   if (a_mtab   != NULL)   *a_mtab  = -1;
   if (a_follow != NULL)   *a_follow = '-';
   if (a_device != NULL)   ystrlcpy (a_device, "", LEN_LABEL);
   if (a_type   != NULL)   ystrlcpy (a_type  , "", LEN_LABEL);
   /*---(defense)------------------------*/
   --rce;  if (a_path == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_path"    , a_path);
   x_len = strlen (a_path);
   DEBUG_ENVI   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(search)-------------------------*/
   for (i = 0; i < s_nmtab; ++i) {
      if (s_mtabs [i].len != x_len)                   continue;
      if (strcmp (s_mtabs [i].path, a_path) != 0)     continue;
      if (a_drive  != NULL)   *a_drive  = s_mtabs [i].drive;
      if (a_mtab   != NULL)   *a_mtab   =  i;
      if (a_follow != NULL)   *a_follow = s_mtabs [i].follow;
      if (a_device != NULL)   ystrlcpy (a_device, s_mtabs [i].device, LEN_LABEL);
      if (a_type   != NULL)   ystrlcpy (a_type  , s_mtabs [i].type  , LEN_LABEL);
      DEBUG_ENVI   yLOG_note    ("found, successful");
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   DEBUG_ENVI   yLOG_note    ("not found, continue normally");
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     small support                            ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
DRIVE__wipe         (tDRIVE *a_drive)
{
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(clear entries)------------------*/
   ystrlcpy (a_drive->host  , my.host, LEN_LABEL);
   ystrlcpy (a_drive->serial, "-"    , LEN_LABEL);
   ystrlcpy (a_drive->device, "-"    , LEN_FULL);
   ystrlcpy (a_drive->mpoint, "-"    , LEN_FULL);
   ystrlcpy (a_drive->type  , "-"    , LEN_LABEL);
   a_drive->size           = 0;
   a_drive->written        = 0;
   /*---(statistics)---------------------*/
   a_drive->n_seen     = a_drive->b_seen           = 0;
   a_drive->n_kept     = a_drive->b_kept           = 0;
   a_drive->n_found    = a_drive->b_found          = 0;
   /*---(links)--------------------------*/
   a_drive->m_prev     = NULL;
   a_drive->m_next     = NULL;
   a_drive->root       = NULL;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
DRIVE__action           (char n, char a_action, llong a_bytes, tDRIVE **a_drive)
{
   /*---(local variables)--+-----+-----+-*/
   char        rce         = NULL;
   tDRIVE     *x_curr      = NULL;
   char        c           =   0;
   /*---(header)-------------------------*/
   DEBUG_MASS   yLOG_senter  (__FUNCTION__);
   DEBUG_MASS   yLOG_sint    (n);
   DEBUG_MASS   yLOG_schar   (a_action);
   DEBUG_MASS   yLOG_sint    (a_bytes);
   DEBUG_MASS   yLOG_spoint  (a_drive);
   /*---(prepare)------------------------*/
   if (a_drive != NULL)  *a_drive = NULL;
   /*---(defense)------------------------*/
   DEBUG_MASS   yLOG_sint    (g_ndrive);
   --rce;  if (n < 0 || n >= g_ndrive) {
      DEBUG_MASS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(find)---------------------------*/
   x_curr  = h_drive;
   while (x_curr != NULL) {
      if (c == n)  break;
      ++c;
      x_curr = x_curr->m_next;
   }
   /*---(save back)----------------------*/
   DEBUG_MASS   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_MASS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_MASS   yLOG_snote   ("save back");
   if (a_drive != NULL)  *a_drive = x_curr;
   /*---(act)----------------------------*/
   --rce;  switch (a_action) {
   case '-'  : 
      break;
   case 's'  : 
      DEBUG_MASS   yLOG_snote   ("seen");
      ++x_curr->n_seen;
      x_curr->b_seen  += a_bytes;
      DEBUG_MASS   yLOG_sint    (x_curr->n_seen);
      DEBUG_MASS   yLOG_sint    (x_curr->b_seen);
      break;
   case 'k'  : 
      DEBUG_MASS   yLOG_snote   ("kept");
      ++x_curr->n_kept;
      x_curr->b_kept  += a_bytes;
      DEBUG_MASS   yLOG_sint    (x_curr->n_kept);
      DEBUG_MASS   yLOG_sint    (x_curr->b_kept);
      break;
   case 'f'  : 
      DEBUG_MASS   yLOG_snote   ("found");
      ++x_curr->n_found;
      x_curr->b_found += a_bytes;
      DEBUG_MASS   yLOG_sint    (x_curr->n_found);
      DEBUG_MASS   yLOG_sint    (x_curr->b_found);
      break;
   default   :
      DEBUG_MASS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_MASS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char DRIVE_seen_add    (char a_ref, llong a_bytes) { return DRIVE__action (a_ref, 'a', a_bytes, NULL); }
char DRIVE_seen_kept   (char a_ref, llong a_bytes) { return DRIVE__action (a_ref, 'k', a_bytes, NULL); }
char DRIVE_seen_found  (char a_ref, llong a_bytes) { return DRIVE__action (a_ref, 'f', a_bytes, NULL); }



/*====================------------------------------------====================*/
/*===----                     memory allocation                        ----===*/
/*====================------------------------------------====================*/
static void  o___MALLOC__________o () { return; }

char
DRIVE_new               (tDRIVE **a_drive)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   tDRIVE     *x_new       = NULL;
   char        x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_drive"   , a_drive);
   --rce;  if (a_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create drive)-------------------*/
   while (x_new == NULL) {
      if (++x_tries > 3)  break;
      x_new = (tDRIVE*) malloc (sizeof (tDRIVE));
   }
   DEBUG_ENVI   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_ENVI   yLOG_point   ("x_new"     , x_new);
   if (x_new == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clean data)---------------------*/
   DEBUG_ENVI   yLOG_note    ("clean data elements");
   DRIVE__wipe (x_new);
   /*---(hook to overall list)-----------*/
   DEBUG_ENVI   yLOG_note    ("hook to overall list");
   x_new->m_next = NULL;
   if (h_drive == NULL)  {
      h_drive        = x_new;
      x_new->m_prev  = NULL;
   } else {
      t_drive->m_next  = x_new;
      x_new->m_prev  = t_drive;
   }
   t_drive = x_new;
   /*---(give a sequence number)---------*/
   ++g_udrive;
   x_new->ref     = g_udrive;
   ++g_ndrive;
   DEBUG_ENVI   yLOG_value   ("g_ndrive"   , g_ndrive);
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_new;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRIVE__remove           (tDRIVE **a_drive)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_drive"   , a_drive);
   --rce;  if (a_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook from main list)----------*/
   DEBUG_ENVI   yLOG_note    ("linked cell, unlinking now");
   if ((*a_drive)->m_next != NULL)   (*a_drive)->m_next->m_prev = (*a_drive)->m_prev;
   else                                t_drive                  = (*a_drive)->m_prev;
   if ((*a_drive)->m_prev != NULL)   (*a_drive)->m_prev->m_next = (*a_drive)->m_next;
   else                                h_drive                  = (*a_drive)->m_next;
   /*---(free main)----------------------*/
   DEBUG_ENVI   yLOG_note    ("freeing and nulling");
   DRIVE__wipe (*a_drive);
   free (*a_drive);
   *a_drive = NULL;
   /*---(update counts)------------------*/
   --g_ndrive;
   DEBUG_ENVI   yLOG_value   ("g_ndrive"   , g_ndrive);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;

}

char
DRIVE__purge            (void)
{  /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   tDRIVE     *x_curr      = NULL;
   x_curr = h_drive;
   while (x_curr != NULL) {
      DRIVE__remove (&x_curr);
      x_curr = h_drive;
   }
   g_udrive = -1;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   handle a single drive                      ----===*/
/*====================------------------------------------====================*/
static void  o___SINGLE__________o () { return; }

char
DRIVE_manual       (tDRIVE **a_drive, uchar a_ref, char *a_host, char *a_serial, char *a_device, char *a_mpoint, char *a_type, llong a_size, int a_written)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDRIVE     *x_drive     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_value   ("a_ref"     , a_ref);
   DEBUG_INPT   yLOG_value   ("g_udrive"   , g_udrive);
   --rce;  if (a_ref <= g_udrive || a_ref > 99) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("host"      , a_host);
   --rce;  if (a_host == NULL || strlen (a_host) < 1) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("serial"    , a_serial);
   /*> --rce;  if (a_serial == NULL || strlen (a_serial) < 8) {                       <* 
    *>    DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   --rce;  if (a_serial == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("device"    , a_device);
   /*> --rce;  if (a_device == NULL || strlen (a_device) < 8 || strncmp ("/dev/", a_device, 5) != NULL) {   <* 
    *>    DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);                                                    <* 
    *>    return rce;                                                                                       <* 
    *> }                                                                                                    <*/
   --rce;  if (a_device == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("mpoint"    , a_mpoint);
   --rce;  if (a_mpoint == NULL || strlen (a_mpoint) < 1) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("type"      , a_type);
   --rce;  if (a_type == NULL || strlen (a_type) < 4) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create new)---------------------*/
   rc = DRIVE_new (&x_drive);
   DEBUG_INPT   yLOG_value   ("x_drive"   , rc);
   --rce;  if (rc < 0 || x_drive == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(override ref number)------------*/
   x_drive->ref   = a_ref;
   DEBUG_INPT   yLOG_value   ("ref"       , x_drive->ref);
   if (a_ref > g_udrive) {
      g_udrive = a_ref;
      DEBUG_INPT   yLOG_value   ("g_udrive*"  , g_udrive);
   }
   /*---(write host)---------------------*/
   ystrlcpy (x_drive->host    , a_host   , LEN_LABEL);
   DEBUG_INPT   yLOG_info    ("host"      , x_drive->host);
   /*---(write serial)-------------------*/
   ystrlcpy (x_drive->serial  , a_serial , LEN_LABEL);
   DEBUG_INPT   yLOG_info    ("serial"    , x_drive->serial);
   /*---(write device)-------------------*/
   ystrlcpy (x_drive->device  , a_device , LEN_FULL);
   DEBUG_INPT   yLOG_info    ("device"    , x_drive->device);
   /*---(write mountpoint)---------------*/
   ystrlcpy (x_drive->mpoint  , a_mpoint , LEN_FULL);
   DEBUG_INPT   yLOG_info    ("mpoint"    , x_drive->mpoint);
   /*---(write mountpoint)---------------*/
   ystrlcpy (x_drive->type    , a_type   , LEN_LABEL);
   DEBUG_INPT   yLOG_info    ("type"      , x_drive->type);
   /*---(size and time)------------------*/
   x_drive->size    = a_size;
   DEBUG_INPT   yLOG_llong   ("size"      , x_drive->size);
   x_drive->written = a_written;
   DEBUG_INPT   yLOG_value   ("written"   , x_drive->written);
   /*---(prepare for use)----------------*/
   x_drive->root = NULL;
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_drive;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                              <* 
 *> DRIVE__mtab        (cchar *a_mount, char *a_part, char *a_type)                   <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    FILE       *f           = NULL;                                                <* 
 *>    int         fd          =   -1;                                                <* 
 *>    char        x_recd      [LEN_RECD];                                            <* 
 *>    char       *p;                                                                 <* 
 *>    char       *q           = " ";                                                 <* 
 *>    char       *r           = NULL;                                                <* 
 *>    int         x_len       =   0;                                                 <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_ENVI   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(prepare)------------------------+/                                       <* 
 *>    if (a_part != NULL)   ystrlcpy (a_part, "", LEN_FULL);                         <* 
 *>    if (a_type != NULL)   ystrlcpy (a_type, "", LEN_LABEL);                        <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_ENVI   yLOG_point   ("a_mount"   , a_mount);                             <* 
 *>    --rce;  if (a_mount == NULL) {                                                 <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_ENVI   yLOG_point   ("a_part"    , a_part);                              <* 
 *>    --rce;  if (a_part == NULL) {                                                  <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_ENVI   yLOG_point   ("a_type"    , a_type);                              <* 
 *>    --rce;  if (a_type == NULL) {                                                  <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(open mtab file)-----------------+/                                       <* 
 *>    f = fopen ("/etc/mtab", "r");                                                  <* 
 *>    DEBUG_ENVI   yLOG_point   ("mtab"      , f);                                   <* 
 *>    --rce;  if (f == NULL) {                                                       <* 
 *>       DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(find mount point)---------------+/                                       <* 
 *>    DEBUG_ENVI   yLOG_note    ("find mount point");                                <* 
 *>    --rce;  while (1) {                                                            <* 
 *>       fgets (x_recd, LEN_RECD, f);                                                <* 
 *>       if (feof(f)) {                                                              <* 
 *>          DEBUG_ENVI   yLOG_note    ("hit EOF before finding mountpoint");         <* 
 *>          rc = rce;                                                                <* 
 *>          break;                                                                   <* 
 *>       }                                                                           <* 
 *>       /+---(filter)----------------------+/                                       <* 
 *>       x_len = strlen (x_recd);                                                    <* 
 *>       if (x_len <= 0)                       continue;                             <* 
 *>       x_recd [--x_len] = '\0';                                                    <* 
 *>       /+---(check partition)-------------+/                                       <* 
 *>       p = strtok_r (x_recd, q, &r);                                               <* 
 *>       if (p == NULL)                        continue;                             <* 
 *>       ystrltrim (p, ySTR_BOTH, LEN_FULL);                                         <* 
 *>       DEBUG_ENVI   yLOG_info    ("device"    , p);                                <* 
 *>       if (p[0] != '/')  {                                                         <* 
 *>          DEBUG_ENVI   yLOG_note    ("not a device entry");                        <* 
 *>          continue;                                                                <* 
 *>       }                                                                           <* 
 *>       ystrlcpy (a_part, p, LEN_FULL);                                             <* 
 *>       /+---(match mountpoint)------------+/                                       <* 
 *>       p = strtok_r (NULL  , q, &r);                                               <* 
 *>       if (p == NULL)                        continue;                             <* 
 *>       ystrltrim (p, ySTR_BOTH, LEN_HUND);                                         <* 
 *>       DEBUG_ENVI   yLOG_info    ("mpoint"    , p);                                <* 
 *>       if (strcmp (p, a_mount) != 0) {                                             <* 
 *>          DEBUG_ENVI   yLOG_note    ("does not match requested mount point");      <* 
 *>          continue;                                                                <* 
 *>       }                                                                           <* 
 *>       /+---(gather type)-----------------+/                                       <* 
 *>       p = strtok_r (NULL  , q, &r);                                               <* 
 *>       if (p == NULL)                        continue;                             <* 
 *>       ystrltrim (p, ySTR_BOTH, LEN_LABEL);                                        <* 
 *>       DEBUG_ENVI   yLOG_info    ("type"      , p);                                <* 
 *>       ystrlcpy (a_type, p, LEN_LABEL);                                            <* 
 *>       /+---(done)------------------------+/                                       <* 
 *>       break;                                                                      <* 
 *>    }                                                                              <* 
 *>    /+---(close mtab file)----------------+/                                       <* 
 *>    fclose (f);                                                                    <* 
 *>    /+---(clean up if an error)-----------+/                                       <* 
 *>    if (rc < 0) {                                                                  <* 
 *>       ystrlcpy (a_part, "", LEN_FULL);                                            <* 
 *>       ystrlcpy (a_type, "", LEN_LABEL);                                           <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_ENVI   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/

char
DRIVE__stats       (cchar *a_part, llong *r_size, char *r_serial)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         rci         =    0;
   FILE       *f           = NULL;
   int         fd          =   -1;
   char        x_full      [LEN_FULL]  = "";
   char       *p;
   char       *q           = " ";
   char       *r           = NULL;
   int         x_len       =   0;
   struct      hd_driveid  hd;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (r_size   != NULL)   *r_size  = 0;
   if (r_serial != NULL)   ystrlcpy (r_serial, "", LEN_LABEL);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_part"    , a_part);
   --rce;  if (a_part == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_part"    , a_part);
   DEBUG_ENVI   yLOG_point   ("r_size"    , r_size);
   --rce;  if (r_size == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("r_serial"  , r_serial);
   --rce;  if (r_serial == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get size)-----------------------*/
   DEBUG_ENVI   yLOG_info    ("a_part"    , a_part);
   x_len = strlen (a_part);
   DEBUG_ENVI   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 3) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (strncmp ("/dev/sda", a_part, 8) == 0) {
      if (strchr ("1234567890", a_part [x_len - 1]) == NULL) {
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else if (strcmp ("/dev/root", a_part) != 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   fd = open (a_part, O_RDONLY);
   DEBUG_ENVI   yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rci = ioctl (fd, BLKGETSIZE64, r_size);
   DEBUG_ENVI   yLOG_value   ("ioctl"     , rci);
   close (fd);
   --rce;  if (rci <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_value   ("*r_size"   , *r_size);
   /*---(change device file)-------------*/
   DEBUG_ENVI   yLOG_note    ("get serial number");
   ystrlcpy (x_full, a_part, LEN_FULL);
   /*> x_len = strlen (x_full);                                                       <* 
    *> x_full [--x_len] = '\0';                                                       <* 
    *> if (x_full [x_len - 1] == '1')  x_full [--x_len] = '\0';                       <*/
   DEBUG_ENVI   yLOG_info    ("x_full"    , x_full);
   /*---(get serial number)--------------*/
   fd = open (x_full, O_RDONLY);
   DEBUG_ENVI   yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rci = ioctl (fd, HDIO_GET_IDENTITY, &hd);
   DEBUG_ENVI   yLOG_value   ("ioctl"     , rci);
   close (fd);
   --rce;  if (rci <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ystrlcpy  (r_serial, hd.serial_no, LEN_LABEL);
   ystrltrim (r_serial, ySTR_BOTH, LEN_LABEL);
   DEBUG_ENVI   yLOG_info    ("r_serial"  , r_serial);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] figure out unique environment -------------------*/
DRIVE_populate          (tDRIVE **a_drive, char *a_mount, long a_time, char *a_index)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_device    [LEN_FULL]  = "";
   char        x_type      [LEN_LABEL] = "";
   llong       x_size      =   0;
   char        x_serial    [LEN_LABEL] = "";
   tDRIVE     *x_drive     = NULL;
   char        x_ref       =   -1;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(find mount)---------------------*/
   DEBUG_ENVI   yLOG_info    ("a_mount"   , a_mount);
   rc = DRIVE_mtab_find (a_mount, &x_drive, &x_ref, NULL, x_device, x_type);
   DEBUG_ENVI   yLOG_complex ("mtab"      , "%p, %d, %p, %p, %d", x_drive, x_ref, x_device, x_type, rc);
   --rce;  if (rc < 1) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (rc > 0 && x_drive != NULL) {
      DEBUG_ENVI   yLOG_note    ("already created");
      if (a_drive != NULL)  *a_drive = x_drive;
      if (a_index != NULL)  *a_index = x_drive->ref;
      DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(get stats)----------------------*/
   if (x_device [0] == '/') {
      rc = DRIVE__stats (x_device, &x_size, x_serial);
      DEBUG_ENVI   yLOG_value   ("stats"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(append)-------------------------*/
   rc = DRIVE_new    (&x_drive);
   DEBUG_ENVI   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0 || x_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   ystrlcpy (x_drive->device   , x_device, LEN_FULL);
   ystrlcpy (x_drive->mpoint   , a_mount , LEN_FULL);
   ystrlcpy (x_drive->type     , x_type  , LEN_LABEL);
   ystrlcpy (x_drive->serial   , x_serial, LEN_LABEL);
   x_drive->size    = x_size;
   x_drive->written = a_time;
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_drive;
   if (a_index != NULL)  *a_index = x_drive->ref;
   /*---(save in mtab)-------------------*/
   s_mtabs [x_ref].drive = x_drive;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       main driver                            ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
DRIVE_inventory         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDRIVE     *x_drive     = NULL;
   tPTRS      *x_root      = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(configuration)------------------*/
   /*> yLOG_stage ('1');                                                              <*/
   rc = CONF_pull    (my.file_conf);
   DEBUG_ENVI   yLOG_value   ("config"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(establish drive)----------------*/
   /*> yLOG_stage ('2');                                                              <*/
   rc = DRIVE_populate (&x_drive, "/", my.runtime, NULL);
   DEBUG_ENVI   yLOG_value   ("drive"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear mime tables)--------------*/
   /*> yLOG_stage ('3');                                                              <*/
   rc = MIME_reset_all ();
   DEBUG_ENVI   yLOG_value   ("mime"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build root)---------------------*/
   /*> yLOG_stage ('4');                                                              <*/
   rc  = ENTRY_fullroot (&x_root, x_drive);
   DEBUG_ENVI   yLOG_value   ("root"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read in data)-------------------*/
   /*> yLOG_stage ('5');                                                              <*/
   rc = ENTRY__level_read (x_root, "", STYPE_NORMAL);
   DEBUG_ENVI   yLOG_value   ("levels"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create (empty) placeholder)-----*/
   /*> yLOG_stage ('6');                                                              <*/
   if (strcmp (my.path, "") == 0) {
      rc = ENTRY_tail (x_drive, x_root);
      DEBUG_ENVI   yLOG_value   ("(empty)"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(write the database)-------------*/
   /*> yLOG_stage ('7');                                                              <*/
   rc = DB_write  (my.file_data, NULL);
   DEBUG_ENVI   yLOG_value   ("write"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write mime file)----------------*/
   /*> yLOG_stage ('8');                                                              <*/
   rc = MIME_report (my.file_mime);
   DEBUG_ENVI   yLOG_value   ("mime"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}
uchar       ref;
char        host        [LEN_LABEL];
char        serial      [LEN_LABEL];
char        device      [LEN_FULL];
char        mpoint      [LEN_FULL];
char        type        [LEN_LABEL];
llong       size;
int         written;                     /* dir entry was written on       */

char
DRIVE_list              (void)
{
   char        rce         =  -10;
   tDRIVE     *x_curr      = NULL;
   char        t           [LEN_LABEL] = "";
   char        u           [LEN_LABEL] = "";
   char        v           [LEN_LABEL] = "";
   printf ("\n");
   x_curr = h_drive;
   printf ("## rf ---host-------- ---device----------- ---mpoint--------------------- --type-- --serial------- --size--\n");
   while (x_curr != NULL) {
      /*> DB_commas (x_curr->size, t);                                              <*/
      ystrlsize (x_curr->size  , 'u', t);
      /*> ystrlsize (x_curr->n_seen, 'u', u);                                         <*/
      /*> ystrlsize (x_curr->b_seen, 'u', v);                                         <*/
      printf ("## %-2d %-15.15s %-20.20s %-30.30s %-8.8s %-15.15s   %-4.4s  \n",
            x_curr->ref, x_curr->host, x_curr->device, x_curr->mpoint,
            x_curr->type, x_curr->serial, t);
      x_curr = x_curr->m_next;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char          unit_answer [LEN_RECD];

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
DRIVE__unit             (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   tDRIVE     *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "DRIVE            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp(a_question, "count"         ) == 0) {
      x_curr = h_drive; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->m_next; }
      x_curr = t_drive; while (x_curr != NULL) { ++x_back; x_curr = x_curr->m_prev; }
      snprintf (unit_answer, LEN_FULL, "DRIVE count      : num=%4d, fore=%4d, back=%4d, max =%4d", g_ndrive, x_fore, x_back, g_udrive);
   }
   else if (strcmp(a_question, "host"          ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DRIVE host       : %3d[%.40s]", strlen (my.host), my.host);
   }
   else if (strcmp(a_question, "mtab"          ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DRIVE mtab  (%2d) :  %c  %2d [%-20.20s] [%-30.30s] [%-10.10s]", 
            n, s_mtabs [n].follow, s_mtabs [n].len, s_mtabs [n].path, s_mtabs [n].device, s_mtabs [n].type);
   }
   else if (strcmp(a_question, "entry"         ) == 0) {
      x_curr  = h_drive;
      while (x_curr != NULL) {
         if (c == n)  break;
         ++c;
         x_curr = x_curr->m_next;
      }
      if (x_curr == NULL) 
         snprintf (unit_answer, LEN_FULL, "DRIVE entry (%2d) : no entry", n);
      else 
         snprintf (unit_answer, LEN_FULL, "DRIVE entry (%2d) : %-2d [%-10.10s] [%-10.10s] [%-10.10s] [%-10.10s] [%-10.10s]", 
               n, x_curr->ref, x_curr->host, x_curr->serial, x_curr->device, x_curr->mpoint, x_curr->type);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*===============================[[ end-code ]]===============================*/
