/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



tDRIVE    *h_drive = NULL;
tDRIVE    *t_drive = NULL;
short      n_drive = 0;
short      u_drive = 0;


char
DRIVE_init         (void)
{  /*---(local variables)--+-----------+-*/
   int         rc          = 0;
   h_drive = NULL;
   t_drive = NULL;
   n_drive = 0;
   u_drive = 0;
   rc = gethostname (my.host, MAX_STR);
   /*---(complete)-----------------------*/
   return rc;
}

char
DRIVE_wrap              (void)
{
   return DRIVE__purge ();
}

char
DRIVE__wipe         (tDRIVE *a_drive)
{
   strlcpy (a_drive->host  , my.host, LEN_LABEL);
   strlcpy (a_drive->serial, "-"    , LEN_LABEL);
   strlcpy (a_drive->device, "-"    , LEN_FULL);
   strlcpy (a_drive->mpoint, "-"    , LEN_FULL);
   strlcpy (a_drive->type  , "-"    , LEN_LABEL);
   a_drive->size           = 0;
   a_drive->written        = 0;
   return 0;
}

char
DRIVE__remove           (tDRIVE **a_drive)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ENVI   yLOG_point   ("a_drive"   , a_drive);
   --rce;  if (a_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook from main list)----------*/
   DEBUG_ENVI   yLOG_note    ("linked cell, unlinking now");
   if ((*a_drive)->next != NULL)   (*a_drive)->next->prev = (*a_drive)->prev;
   else                            t_drive                = (*a_drive)->prev;
   if ((*a_drive)->prev != NULL)   (*a_drive)->prev->next = (*a_drive)->next;
   else                            h_drive                = (*a_drive)->next;
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
   return 0;
}

char
DRIVE_append            (tDRIVE **a_drive)
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
   x_new->next = NULL;
   if (h_drive == NULL)  {
      h_drive        = x_new;
      x_new->prev  = NULL;
   } else {
      t_drive->next  = x_new;
      x_new->prev  = t_drive;
   }
   t_drive = x_new;
   /*---(give a sequence number)---------*/
   x_new->ref     = u_drive;
   ++u_drive;
   ++n_drive;
   DEBUG_ENVI   yLOG_value   ("n_drive"   , n_drive);
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_new;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
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
DRIVE_populate     (tDRIVE **a_drive, char *a_mount, long a_time)
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
   rc = DRIVE_append (&x_drive);
   DEBUG_ENVI   yLOG_value   ("append"    , rc);
   --rce;  if (rc < 0 || x_drive == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_drive->device   , x_part  , LEN_FULL);
   strlcpy (x_drive->mpoint   , a_mount , LEN_FULL);
   strlcpy (x_drive->type     , x_type  , LEN_LABEL);
   x_drive->size    = x_size;
   strlcpy (x_drive->serial   , x_serial, LEN_LABEL);
   x_drive->written = a_time;
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_drive;
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
      x_curr = h_drive; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->next; }
      x_curr = t_drive; while (x_curr != NULL) { ++x_back; x_curr = x_curr->prev; }
      snprintf (unit_answer, LEN_FULL, "DRIVE count      : num=%4d, fore=%4d, back=%4d", n_drive, x_fore, x_back);
   }
   else if (strcmp(a_question, "host"          ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DRIVE host       : %3d[%.40s]", strlen (my.host), my.host);
   }
   else if (strcmp(a_question, "entry"         ) == 0) {
      x_curr  = h_drive;
      while (x_curr != NULL) {
         if (c == n)  break;
         ++c;
         x_curr = x_curr->next;
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
