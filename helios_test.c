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




/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROG__unit_quiet     (void)
{
   char        rc          =    0;
   int         x_argc      =    1;
   char       *x_argv [1]  = { "helios" };
   rc = PROG_urgents   (x_argc, x_argv);
   rc = PROG_startup   (x_argc, x_argv);
   my.run_as = IAM_UHELIOS;
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
   my.run_as = IAM_UHELIOS;
   my.output = OUTPUT_SILENT;
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud      (void)
{
   char        rc          =    0;
   int         x_argc      =    6;
   char       *x_argv [20] = { "helios_unit", "@@kitchen", "@@args", "@@stats", "@@yparse", "@@YSORT" };
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


