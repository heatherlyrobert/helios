/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */




/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROG__unit_quiet     (void)
{
   char        rc          =    0;
   int         x_argc      =    1;
   char       *x_argv [1]  = { "helios_unit" };
   rc = PROG_urgents   (x_argc, x_argv);
   rc = PROG_startup   (x_argc, x_argv);
   my.output = OUTPUT_SILENT;
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_mute      (void)
{
   char        rc          =    0;
   int         x_argc      =    2;
   char       *x_argv [2]  = { "helios_unit", "@@mute" };
   rc = PROG_urgents   (x_argc, x_argv);
   rc = PROG_startup   (x_argc, x_argv);
   my.output = OUTPUT_SILENT;
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud      (void)
{
   char        rc          =    0;
   int         x_argc      =    4;
   char       *x_argv [20] = { "helios_unit", "@@kitchen", "@@args", "@@stats" };
   rc = PROG_urgents   (x_argc, x_argv);
   rc = PROG_startup   (x_argc, x_argv);
   my.output = OUTPUT_SILENT;
   return 0;
}

char         /*-> set up program urgents/debugging ---[ light  [uz.210.001.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_end       (void)
{
   PROG_shutdown  ();
   return 0;
}


