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
   if (rc >= 0)  rc = PROG_init    (argc, argv);
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc <  0) {
      PROG_end ();
      return -1;
   }
   /*---(main)---------------------------*/
   rc = PROG_driver ();
   /*---(complete)-----------------------*/
   rc = PROG_end     ();
   return 0;
}


/*===============================[[ end-code ]]===============================*/
