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
   int         rc          =    0;
   int         c           =    0;
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
      rc = DRIVE_inventory ();
   } else if (my.mimetree == 'y') {
      READ_all     (my.file_data, &c);
      MIME_tree    ();
   } else if (my.dirtree == 'y') {
      READ_all   (my.file_data, &c);
      if (rc >= 0)  rc = ENTRY_start ();
      /*> if (my.path != NULL)  DATA_start (my.path);                                 <*/
      RPTG_dirtree (1, h_ptrs, "");
   } else if (my.statistics == 'y') {
      READ_all   (my.file_data, &c);
      RPTG_summ  ();
   } else if (my.mime_table == 'y' && strcmp (my.regex, "") == 0) {
      READ_all   (my.file_data, &c);
      MIME_write   ('s', ' ');
   } else if (my.dump == 'y') {
      READ_all   (my.file_data, &c);
   } else {
      READ_all   (my.file_data, &c);
      if (rc >= 0)  rc = ENTRY_start ();
      if (rc >= 0)  rc = RPTG_walker (WALK_ALL);
      if (my.total > 0 && my.output != OUTPUT_COUNT)  RPTG_footer ();
      /*> if (my.mime_table == 'y') MIME_write   ('s', ' ');                          <*/
   }
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   PROG_end     ();
   return 0;
}


/*===============================[[ end-code ]]===============================*/
