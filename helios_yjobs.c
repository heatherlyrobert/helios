/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"

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
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
helios_yjobs            (cchar a_req, cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_char    ("a_req"     , a_req);
   DEBUG_PROG    yLOG_info    ("a_data"    , a_data);
   /*---(handle)-------------------------*/
   switch (a_req) {
   case YJOBS_READ     :
      DEBUG_PROG    yLOG_note    ("Ô) called database read");
      /*> rc = CONF_pull    (my.file_conf);                                           <*/
      if (rc >= 0)  rc = MIME_pull    (my.file_mime);
      DB_read      (my.file_data, &c);
      /*> rc = poly_yjobs_read     ();                                                <*/
      break;
   case YJOBS_STATS    :
      DEBUG_PROG    yLOG_note    ("s) called database statistics");
      break;
   case YJOBS_GATHER   :
      DEBUG_PROG    yLOG_note    ("g) called full/world database gathering");
      rc = DRIVE_inventory ();
      break;
   case YJOBS_AUDIT    :
      DEBUG_PROG    yLOG_note    ("a) called environment audit");
      break;
   case YJOBS_REPORT   :
      DEBUG_PROG    yLOG_note    (" ) called database reporting");
      break;
   case YJOBS_WRITE    :
      DEBUG_PROG    yLOG_note    ("Õ) called database write");
      break;
   case YJOBS_PULL     :
      DEBUG_PROG    yLOG_note    ("×) called pull");
      /*> rc = CONF_pull    (my.file_conf);                                           <*/
      /*> if (rc >= 0)  rc = MIME_pull    (my.file_mime);                             <+/   <*/
      /*> if (rc >= 0)  rc = DRIVE_inventory ();                                      <*/
      break;
   case YJOBS_CLEAR    :
      DEBUG_PROG    yLOG_note    ("x) called clear");
      break;
   case YJOBS_LOCALRPT :
      DEBUG_PROG    yLOG_note    ("l) called local file reporting");
      break;
   }
   /*---(trouble)------------------------*/
   if (rc < 0) {
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return 0;
}

