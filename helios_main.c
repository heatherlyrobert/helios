/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



/*====================------------------------------------====================*/
/*===----                            main                              ----===*/
/*====================------------------------------------====================*/
static void      o___MAIN____________________o (void) {;}

int
main               (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----------+-*/
   int         rc          =    0;
   int         c           =    0;
   /*---(debugging)----------------------*/
   rc = PROG_urgents (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("urgents"   , rc);
   if (rc <  0) { PROG_shutdown (); return -1; }
   /*---(initialization)-----------------*/
   rc = PROG_startup (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("startup"   , rc);
   if (rc <  0) { PROG_shutdown (); return -2; }
   /*---(main)---------------------------*/
   rc = PROG_driver   ();
   /*---(complete)-----------------------*/
   PROG_shutdown ();
   return rc;
}


/*===============================[[ end-code ]]===============================*/
