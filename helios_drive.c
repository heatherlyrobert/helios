/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



tDRIVE    *h_drive = NULL;                  /* drive tail entry               */
tDRIVE    *t_drive = NULL;                  /* drive tail entry               */
short      n_drive = 0;                     /* count of active drives         */
short      u_drive = -1;                    /* max drive number used (ever)   */





/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
DRIVE_init         (void)
{  /*---(local variables)--+-----------+-*/
   int         rc          = 0;
   h_drive = NULL;
   t_drive = NULL;
   n_drive = 0;
   u_drive = -1;
   rc = gethostname (my.host, MAX_STR);
   /*---(complete)-----------------------*/
   return rc;
}

char
DRIVE_wrap              (void)
{
   return DRIVE__purge ();
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
   strlcpy (a_drive->host  , my.host, LEN_LABEL);
   strlcpy (a_drive->serial, "-"    , LEN_LABEL);
   strlcpy (a_drive->device, "-"    , LEN_FULL);
   strlcpy (a_drive->mpoint, "-"    , LEN_FULL);
   strlcpy (a_drive->type  , "-"    , LEN_LABEL);
   a_drive->size           = 0;
   a_drive->written        = 0;
   a_drive->root           = NULL;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     memory allocation                        ----===*/
/*====================------------------------------------====================*/
static void  o___MALLOC__________o () { return; }

char
DRIVE_new               (tDRIVE **a_drive)
{  /*---(local variables)--+-----------+-*/
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
   ++u_drive;
   x_new->ref     = u_drive;
   ++n_drive;
   DEBUG_ENVI   yLOG_value   ("n_drive"   , n_drive);
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
   --n_drive;
   DEBUG_ENVI   yLOG_value   ("n_drive"   , n_drive);
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
   u_drive = -1;
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
   DEBUG_INPT   yLOG_value   ("u_drive"   , u_drive);
   --rce;  if (a_ref <= u_drive || a_ref > 99) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("host"      , a_host);
   --rce;  if (a_host == NULL || strlen (a_host) < 1) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("serial"    , a_serial);
   --rce;  if (a_serial == NULL || strlen (a_serial) < 8) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("device"    , a_device);
   --rce;  if (a_device == NULL || strlen (a_device) < 8 || strncmp ("/dev/", a_device, 5) != NULL) {
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
   if (a_ref > u_drive) {
      u_drive = a_ref;
      DEBUG_INPT   yLOG_value   ("u_drive*"  , u_drive);
   }
   /*---(write host)---------------------*/
   strlcpy (x_drive->host    , a_host   , LEN_LABEL);
   DEBUG_INPT   yLOG_info    ("host"      , x_drive->host);
   /*---(write serial)-------------------*/
   strlcpy (x_drive->serial  , a_serial , LEN_LABEL);
   DEBUG_INPT   yLOG_info    ("serial"    , x_drive->serial);
   /*---(write device)-------------------*/
   strlcpy (x_drive->device  , a_device , LEN_FULL);
   DEBUG_INPT   yLOG_info    ("device"    , x_drive->device);
   /*---(write mountpoint)---------------*/
   strlcpy (x_drive->mpoint  , a_mpoint , LEN_FULL);
   DEBUG_INPT   yLOG_info    ("mpoint"    , x_drive->mpoint);
   /*---(write mountpoint)---------------*/
   strlcpy (x_drive->type    , a_type   , LEN_LABEL);
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

char
DRIVE__mtab        (cchar *a_mount, char *a_part, char *a_type)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   int         fd          =   -1;
   char        x_recd      [MAX_RECD];
   char       *p;
   char       *q           = " ";
   char       *r           = NULL;
   int         x_len       =   0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_part != NULL)   strlcpy (a_part, "", LEN_FULL);
   if (a_type != NULL)   strlcpy (a_type, "", LEN_LABEL);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_mount"   , a_mount);
   --rce;  if (a_mount == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("a_part"    , a_part);
   --rce;  if (a_part == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("a_type"    , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
      fgets (x_recd, MAX_RECD, f);
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
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_ENVI   yLOG_info    ("device"    , p);
      if (p[0] != '/')  {
         DEBUG_ENVI   yLOG_note    ("not a device entry");
         continue;
      }
      strlcpy (a_part, p, LEN_FULL);
      /*---(match mountpoint)------------*/
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_ENVI   yLOG_info    ("mpoint"    , p);
      if (strcmp (p, a_mount) != 0) {
         DEBUG_ENVI   yLOG_note    ("does not match requested mount point");
         continue;
      }
      /*---(gather type)-----------------*/
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_ENVI   yLOG_info    ("type"      , p);
      strlcpy (a_type, p, LEN_LABEL);
      /*---(done)------------------------*/
      break;
   }
   /*---(close mtab file)----------------*/
   fclose (f);
   /*---(clean up if an error)-----------*/
   if (rc < 0) {
      strlcpy (a_part, "", LEN_FULL);
      strlcpy (a_type, "", LEN_LABEL);
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
DRIVE__stats       (cchar *a_part, llong *a_size, char *a_serial)
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
   if (a_size   != NULL)   *a_size  = 0;
   if (a_serial != NULL)   strlcpy (a_serial, "", LEN_LABEL);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_part"    , a_part);
   --rce;  if (a_part == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("a_size"    , a_size);
   --rce;  if (a_size == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("a_serial"  , a_serial);
   --rce;  if (a_serial == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get size)-----------------------*/
   DEBUG_ENVI   yLOG_info    ("a_part"    , a_part);
   x_len = strlen (a_part);
   DEBUG_ENVI   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 3 || strchr ("1234567890", a_part [x_len - 1]) == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   fd = open (a_part, O_RDONLY);
   DEBUG_ENVI   yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rci = ioctl (fd, BLKGETSIZE64, a_size);
   DEBUG_ENVI   yLOG_value   ("ioctl"     , rci);
   close (fd);
   --rce;  if (rci <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_value   ("*a_size"   , *a_size);
   /*---(change device file)-------------*/
   DEBUG_ENVI   yLOG_note    ("get serial number");
   strlcpy (x_full, a_part, LEN_FULL);
   x_len = strlen (x_full);
   x_full [--x_len] = '\0';
   if (x_full [x_len - 1] == '1')  x_full [--x_len] = '\0';
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
   strlcpy   (a_serial, hd.serial_no, LEN_LABEL);
   ySTR_trim (a_serial, ySTR_BOTH);
   DEBUG_ENVI   yLOG_info    ("a_serial"  , a_serial);
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] figure out unique environment -------------------*/
DRIVE__populate         (tDRIVE **a_drive, char *a_mount, long a_time)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_part      [LEN_FULL]  = "";
   char        x_type      [LEN_LABEL] = "";
   llong       x_size      =   0;
   char        x_serial    [LEN_LABEL] = "";
   tDRIVE     *x_drive     = NULL;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(find mount)---------------------*/
   rc = DRIVE__mtab (a_mount, x_part, x_type);
   DEBUG_ENVI   yLOG_value   ("mtab"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get stats)----------------------*/
   rc = DRIVE__stats (x_part, &x_size, x_serial);
   DEBUG_ENVI   yLOG_value   ("stats"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append)-------------------------*/
   rc = DRIVE_new    (&x_drive);
   DEBUG_ENVI   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0 || x_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_drive->device   , x_part  , LEN_FULL);
   strlcpy (x_drive->mpoint   , a_mount , LEN_FULL);
   strlcpy (x_drive->type     , x_type  , LEN_LABEL);
   strlcpy (x_drive->serial   , x_serial, LEN_LABEL);
   x_drive->size    = x_size;
   x_drive->written = a_time;
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_drive;
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
   yLOG_stage ('1');
   rc = CONF_read    ();
   DEBUG_ENVI   yLOG_value   ("config"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(establish drive)----------------*/
   yLOG_stage ('2');
   rc = DRIVE__populate (&x_drive, my.mpoint, my.runtime);
   DEBUG_ENVI   yLOG_value   ("drive"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear mime tables)--------------*/
   yLOG_stage ('3');
   rc = MIME_reset_to_zeros ();
   DEBUG_ENVI   yLOG_value   ("mime"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build root)---------------------*/
   yLOG_stage ('4');
   rc  = ENTRY_fullroot (&x_root, x_drive);
   DEBUG_ENVI   yLOG_value   ("root"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read in data)-------------------*/
   yLOG_stage ('5');
   rc = ENTRY__level_read (x_root, "", STYPE_NORMAL);
   DEBUG_ENVI   yLOG_value   ("levels"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create (empty) placeholder)-----*/
   yLOG_stage ('6');
   /*> rc = ENTRY_tail (x_drive, x_root);                                             <* 
    *> DEBUG_ENVI   yLOG_value   ("(empty)"   , rc);                                  <* 
    *> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(write the database)-------------*/
   yLOG_stage ('7');
   rc = WRITE_all (my.database, NULL);
   DEBUG_ENVI   yLOG_value   ("write"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write mime file)----------------*/
   yLOG_stage ('8');
   rc = MIME_write ('m' , '');
   DEBUG_ENVI   yLOG_value   ("mime"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char          unit_answer [LEN_FULL];

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
      snprintf (unit_answer, LEN_FULL, "DRIVE count      : num=%4d, fore=%4d, back=%4d, u_drive=%4d", n_drive, x_fore, x_back, u_drive);
   }
   else if (strcmp(a_question, "host"          ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DRIVE host       : %3d[%.40s]", strlen (my.host), my.host);
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
