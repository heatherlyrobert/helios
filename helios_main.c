/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



/*====================------------------------------------====================*/
/*===----                            main                              ----===*/
/*====================------------------------------------====================*/
static void      o___MAIN____________________o (void) {;}

int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;
   tPTRS      *x_ptrs      = NULL;
   int         i           = 0;
   tDRIVE     *x_drive     = NULL;
   /*---(startup)------------------------*/
   if (rc >= 0)  rc = yURG_logger  (argc, argv);
   if (rc >= 0)  rc = yURG_urgs    (argc, argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc <  0) {
      PROG_end ();
      return -1;
   }
   /*---(main)---------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   if (my.updatedb == 'y' || strcmp (argv [0], "helios_update") == 0 || strcmp (argv [0], "updatedb") == 0) {
      if (rc == 0) rc = CONF_read    ();
      if (rc == 0) rc = DRIVE_populate (&x_drive, my.mpoint, my.runtime);
      if (rc == 0) {
         for (i = 0; i < n_mime; ++i) {
            mime [i].seen   = 0;
            mime [i].sbytes = 0;
            mime [i].kept   = 0;
            mime [i].kbytes = 0;
            mime [i].found  = 0;
            mime [i].fbytes = 0;
         }
         x_ptrs  = ENTRY__root       (x_drive);
         rc      = ENTRY__level_read (1, x_ptrs, "", 'y');
         DATA_empty  (x_drive, x_ptrs);
         if (rc >= 0) {
            FWRITE_all (x_ptrs);
            MIME_write ('m' , '');
         }
      }
   } else if (my.mimetree == 'y') {
      FREAD_all  ();
      MIME_tree    ();
   } else if (my.dirtree == 'y') {
      FREAD_all  ();
      if (my.path != NULL)  DATA_start (my.path);
      RPTG_dirtree (1, h_ptrs, "");
   } else if (my.statistics == 'y') {
      FREAD_all  ();
      RPTG_summ  ();
   } else if (my.mime_table == 'y' && strcmp (my.regex, ".") == 0) {
      FREAD_all  ();
      MIME_write   ('s', ' ');
   } else if (my.dump == 'y') {
      FREAD_all  ();
   } else {
      FREAD_all  ();
      if (my.path != NULL)  DATA_start (my.path);
      RPTG_regex (1, h_ptrs, "");
      if (my.count == 'y')  printf ("%d\n", my.total);
      if (my.mime_table == 'y') MIME_write   ('s', ' ');
   }
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   PROG_end     ();
   return 0;
}


/*===============================[[ end-code ]]===============================*/
