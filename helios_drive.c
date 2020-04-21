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
DRIVE__wipe         (tDRIVE *a_drive)
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
DRIVE_remove            (tDRIVE **a_drive)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_DRIVE  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DRIVE  yLOG_point   ("a_drive"   , a_drive);
   --rce;  if (a_drive == NULL) {
      DEBUG_DRIVE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook from main list)----------*/
   DEBUG_CELL   yLOG_note    ("linked cell, unlinking now");
   if ((*a_drive)->next != NULL)   (*a_drive)->next->prev = (*a_drive)->prev;
   else                            t_drive                = (*a_drive)->prev;
   if ((*a_drive)->prev != NULL)   (*a_drive)->prev->next = (*a_drive)->next;
   else                            h_drive                = (*a_drive)->next;
   /*---(free main)----------------------*/
   DEBUG_CELL   yLOG_note    ("freeing and nulling");
   DRIVE__wipe (*a_drive);
   free (*a_drive);
   *a_drive = NULL;
   /*---(update counts)------------------*/
   --n_drive;
   DEBUG_DRIVE  yLOG_value   ("n_drive"   , n_drive);
   /*---(complete)-----------------------*/
   DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
   return 0;

}

char
DRIVE_purge        (void)
{  /*---(local variables)--+-----------+-*/
   char        rce         =  -10;
   tDRIVE     *x_curr      = NULL;
   x_curr = h_drive;
   while (x_curr != NULL) {
      DRIVE_remove (&x_curr);
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
   DEBUG_DRIVE  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DRIVE  yLOG_point   ("a_drive"   , a_drive);
   --rce;  if (a_drive == NULL) {
      DEBUG_DRIVE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create drive)-------------------*/
   while (x_new == NULL) {
      if (++x_tries > 3)  break;
      x_new = (tDRIVE*) malloc (sizeof (tDRIVE));
   }
   DEBUG_CELL   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_CELL   yLOG_point   ("x_new"     , x_new);
   if (x_new == NULL) {
      DEBUG_DRIVE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clean data)---------------------*/
   DEBUG_DRIVE  yLOG_note    ("clean data elements");
   DRIVE__wipe (x_new);
   /*---(hook to overall list)-----------*/
   DEBUG_DRIVE  yLOG_note    ("hook to overall list");
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
   DEBUG_DRIVE  yLOG_value   ("n_drive"   , n_drive);
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_new;
   /*---(complete)-----------------------*/
   DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] figure out unique environment -------------------*/
DRIVE_populate     (tDRIVE **a_drive, char *a_mount, long a_time)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
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
   char        x_serial    [20]  = "";
   int         x_len       =   0;
   struct      hd_driveid  hd;
   tDRIVE     *x_drive     = NULL;
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
      if (strcmp (p, a_mount) != 0) {
         DEBUG_DRIVE  yLOG_note    ("does not match requested mount point");
         continue;
      }
      p = strtok_r (NULL  , q, &r);
      if (p == NULL)                        continue;
      ySTR_trim (p, ySTR_BOTH);
      DEBUG_DRIVE  yLOG_info    ("type"      , p);
      /*---(create the entry)---------------*/
      rc = DRIVE_append (&x_drive);
      strncpy (x_drive->device   , x_partition, 20);
      strncpy (x_drive->mpoint   , a_mount    , 50);
      strncpy (x_drive->type     , p          , 10);
      break;
   }
   --rce;  if (x_drive == NULL) {
      /*> printf ("could not find mountpoint %s in mtab\n", a_mount);                 <*/
      DEBUG_DRIVE  yLOG_note    ("never found mount point");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(get size)-----------------------*/
   DEBUG_DRIVE  yLOG_note    ("get partition size");
   fd = open (x_drive->device, O_RDONLY);
   DEBUG_DRIVE  yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      /*> printf ("could not open device %s in mtab\n", x_drive->device);             <*/
      DEBUG_DRIVE  yLOG_note    ("can not open device");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rci = ioctl (fd, BLKGETSIZE64, &(x_drive->size));
   close (fd);
   DEBUG_DRIVE  yLOG_value   ("rci"       , rci);
   --rce;  if (rci <  0) {
      /*> printf ("could not retrieve device file size\n");                           <*/
      DEBUG_DRIVE  yLOG_note    ("can not get partition size");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   FILE_commas (x_drive->size, dsize_s);
   DEBUG_DRIVE  yLOG_info    ("size"      , dsize_s);
   /*---(get serial number)--------------*/
   DEBUG_DRIVE  yLOG_note    ("get serial number");
   strcpy (x_serial, x_drive->device);
   x_len = strlen (x_serial);
   x_serial [--x_len] = '\0';
   if (x_serial [x_len - 1] == '1')  x_serial [--x_len] = '\0';
   fd = open (x_serial, O_RDONLY);
   DEBUG_DRIVE  yLOG_value   ("fd"        , fd);
   --rce;  if (fd <  0) {
      /*> printf ("could not open drive %s\n", x_serial);                             <*/
      DEBUG_DRIVE  yLOG_note    ("can not open full drive");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rci = ioctl (fd, HDIO_GET_IDENTITY, &hd);
   close (fd);
   DEBUG_DRIVE  yLOG_value   ("rci"       , rci);
   --rce;  if (rci <  0) {
      /*> printf ("could not get serial number of drive %s\n", x_serial);             <*/
      DEBUG_DRIVE  yLOG_note    ("can not get drive serial number");
      DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   strncpy   (x_serial, hd.serial_no, 20);
   ySTR_trim (x_serial, ySTR_BOTH);
   strncpy   (x_drive->serial, x_serial, 20);
   DEBUG_DRIVE  yLOG_info    ("serial id" , x_drive->serial);
   x_drive->written = a_time;
   /*---(save back)----------------------*/
   if (a_drive != NULL)  *a_drive = x_drive;
   /*---(complete)-----------------------*/
   DEBUG_DRIVE  yLOG_exit    (__FUNCTION__);
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
