/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



tDRIVE    *h_drive;
tDRIVE    *t_drive;
short      n_drive;
short      u_drive;


char
DRIVE_host         (void)
{  /*---(local variables)--+-----------+-*/
   tDRIVE     *x_drive     = NULL;
   int         rc          = 0;
   rc = gethostname (my.host, MAX_STR);
   /*---(complete)-----------------------*/
   return rc;
}

char
DRIVE_wipe         (tDRIVE *a_drive)
{
   strcpy (a_drive->host  , my.host);
   strcpy (a_drive->serial, "-");
   strcpy (a_drive->device, "-");
   strcpy (a_drive->mpoint, "-");
   strcpy (a_drive->type  , "-");
   a_drive->size           = 0;
   a_drive->written        = 0;
   return 0;
}

char
DRIVE_remove       (void)
{
   return 0;
}

char
DRIVE_purge        (void)
{
   return 0;
}

tDRIVE*
DRIVE_append       (void)
{  /*---(local variables)--+-----------+-*/
   tDRIVE     *x_drive     = NULL;          /* current entry                  */
   char        x_tries     = 0;             /* malloc attempts                */
   /*---(header)-------------------------*/
   DEBUG_DRIVE  yLOG_enter   (__FUNCTION__);
   /*---(create drive)-------------------*/
   DEBUG_DRIVE  yLOG_note    ("malloc drive");
   while (x_drive == NULL) {
      if (++x_tries > 3)  break;
      x_drive = (tDRIVE*) malloc (sizeof (tDRIVE));
   }
   if (x_drive == NULL) {
      DEBUG_DRIVE  yLOG_note    ("drive malloc failed, SKIPPED");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   DEBUG_DRIVE  yLOG_point   ("created"   , x_drive);
   /*---(clean data)---------------------*/
   DEBUG_DRIVE  yLOG_note    ("clean data elements");
   DRIVE_wipe (x_drive);
   /*---(hook to overall list)-----------*/
   DEBUG_DRIVE  yLOG_note    ("hook to overall list");
   x_drive->next = NULL;
   if (h_drive == NULL)  {
      h_drive        = x_drive;
      x_drive->prev  = NULL;
   } else {
      t_drive->next  = x_drive;
      x_drive->prev  = t_drive;
   }
   t_drive = x_drive;
   /*---(give a sequence number)---------*/
   x_drive->ref     = u_drive;
   ++u_drive;
   ++n_drive;
   /*---(complete)-----------------------*/
   DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
   return x_drive;
}

char
DRIVE_find         (void)
{
   return 0;
}

char             /* [------] figure out unique environment -------------------*/
DRIVE_populate     (tDRIVE *a_drive)
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f_mtab      = NULL;
   char        rce         =  -10;
   int         fd          =   -1;
   char        x_recd      [MAX_RECD];
   char       *p;
   char       *q           = " ";
   char       *r           = NULL;
   char        x_partition [20]       = "";
   int         rci         =   0;
   char        dsize_s     [100] = "";
   char        x_drive     [20]  = "";
   int         x_len       =   0;
   struct      hd_driveid  hd;
   char        x_serial    [20]  = "";
   /*---(header)-------------------------*/
   DEBUG_DRIVE  yLOG_enter   (__FUNCTION__);
   /*---(get device)---------------------*/
   f_mtab = fopen ("/etc/mtab", "r");
   DEBUG_DRIVE  yLOG_point   ("mtab"      , f_mtab);
   --rce;  if (f_mtab == NULL) {
      DEBUG_DRIVE  yLOG_note    ("failed to open mtab");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DRIVE  yLOG_note    ("successfully openned");
   /*---(find mount point)---------------*/
   DEBUG_DRIVE  yLOG_note    ("find mount point");
   while (1) {
      fgets (x_recd, MAX_RECD, f_mtab);
      if (feof(f_mtab))                     break;
      /*---(filter)----------------------*/
      x_len = strlen (x_recd);
      if (x_len <= 0)                       continue;
      x_recd [--x_len] = '\0';
      /*---(parse)-----------------------*/
      p = strtok_r (x_recd, q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_DRIVE  yLOG_info    ("device"    , p);
      if (p[0] != '/')  {
         DEBUG_DRIVE  yLOG_note    ("not a device entry");
         continue;
      }
      strncpy (x_partition, p, 20);
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_DRIVE  yLOG_info    ("mpoint"    , p);
      if (strcmp (p, my.mpoint) != 0) {
         DEBUG_DRIVE  yLOG_note    ("does not match requested mount point");
         continue;
      }
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_DRIVE  yLOG_info    ("type"      , p);
      strncpy (a_drive->device   , x_partition, 20);
      strncpy (a_drive->mpoint   , my.mpoint,   50);
      strncpy (a_drive->type     , p,           10);
      break;
   }
   --rce;  if (strcmp (a_drive->mpoint, my.mpoint) != 0) {
      printf ("could not find mountpoint %s in mtab\n", my.mpoint);
      DEBUG_DRIVE  yLOG_note    ("never found mount point");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(get size)-----------------------*/
   DEBUG_DRIVE  yLOG_note    ("get partition size");
   fd = open (a_drive->device, O_RDONLY);
   DEBUG_DRIVE  yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      printf ("could not open device %s in mtab\n", a_drive->device);
      DEBUG_DRIVE  yLOG_note    ("can not open device");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rci = ioctl (fd, BLKGETSIZE64, &(a_drive->size));
   close (fd);
   DEBUG_DRIVE  yLOG_value   ("rci"       , rci);
   --rce;  if (rci <  0) {
      printf ("could not retrieve device file size\n");
      DEBUG_DRIVE  yLOG_note    ("can not get partition size");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   FILE_commas (a_drive->size, dsize_s);
   DEBUG_DRIVE  yLOG_info    ("size"      , dsize_s);
   /*---(get serial number)--------------*/
   DEBUG_DRIVE  yLOG_note    ("get serial number");
   strcpy (x_drive, a_drive->device);
   x_len = strlen (x_drive);
   x_drive [--x_len] = '\0';
   if (x_drive [x_len - 1] == '1')  x_drive [--x_len] = '\0';
   fd = open (x_drive, O_RDONLY);
   DEBUG_DRIVE  yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      printf ("could not open drive %s\n", x_drive);
      DEBUG_DRIVE  yLOG_note    ("can not open full drive");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rci = ioctl (fd, HDIO_GET_IDENTITY, &hd);
   close (fd);
   DEBUG_DRIVE  yLOG_value   ("rci"       , rci);
   --rce;  if (rci <  0) {
      printf ("could not get serial number of drive %s\n", x_drive);
      DEBUG_DRIVE  yLOG_note    ("can not get drive serial number");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   strncpy   (x_serial, hd.serial_no, 20);
   ySTR_trim (x_serial, ySTR_BOTH);
   strncpy   (a_drive->serial, x_serial, 20);
   DEBUG_DRIVE  yLOG_info    ("serial id" , a_drive->serial);
   /*---(complete)-----------------------*/
   a_drive->written = my.runtime;
   DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
   return 0;
}




/*===============================[[ end-code ]]===============================*/
