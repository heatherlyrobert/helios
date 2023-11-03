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
/*===----                      shared functions                        ----===*/
/*====================------------------------------------====================*/
static void      o___SHARED__________________o (void) {;}

char
api_ysort__common      (char a_type, void *a_one  , void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_SORT   yLOG_spoint  (a_one);
   --rce;  if (a_one == NULL && a_type != 'i') {
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      return rce;
   }
   --rce;  if (a_one == a_two) {
      DEBUG_SORT   yLOG_snote   ("same, no action");
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    gnome-sort callbacks                      ----===*/
/*====================------------------------------------====================*/
static void      o___GNOME___________________o (void) {;}

char
api_ysort__cursor       (uchar a_type, void *a_head, void *a_tail, void *a_beg, void **a_new, char a_action)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tPTRS      *x_beg       = NULL;
   tPTRS      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_SORT   yLOG_point   ("a_beg"     , a_beg);
   DEBUG_SORT   yLOG_point   ("a_new"     , a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_SORT   yLOG_note    ("cast");
   if (a_beg != NULL)  x_beg    = (tPTRS *) a_beg;
   if (x_beg != NULL && x_beg->data != NULL) {
      DEBUG_SORT   yLOG_info    ("beg->name", x_beg->data->name);
   }
   /*---(update)-------------------------*/
   DEBUG_SORT   yLOG_char    ("a_action"  , a_action);
   switch (a_action) {
   case '>' : if (x_beg != NULL) { x_new = x_beg->s_next;   break; }
   case ']' : x_new = (tPTRS *) a_tail;                       break;
   case '<' : if (x_beg != NULL) { x_new = x_beg->s_prev;   break; }
   case '[' : x_new = (tPTRS *) a_head;                       break;
   }
   /*---(save back)----------------------*/
   if (x_new != NULL && x_new->data != NULL) {
      DEBUG_SORT   yLOG_info    ("new->name", x_new->data->name);
   }
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_ysort__checker      (uchar a_type, uchar a_lvl, void *a_one, void *a_two, uchar a_order)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPTRS      *x_one       = NULL;
   tPTRS      *x_two       = NULL;
   long        x_key       =   -1;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_complex ("args"      , "%c, %2d, %p, %p, %c", a_type, a_lvl, a_one, a_two, a_order);
   /*---(defense)------------------------*/
   rc = api_ysort__common ('c', a_one, a_two);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_one    = (tPTRS *) a_one;
   x_two    = (tPTRS *) a_two;
   /*---(handle object)------------------*/
   DEBUG_SORT   yLOG_value   ("x_one"     , x_one->data->name);
   DEBUG_SORT   yLOG_value   ("x_two"     , x_two->data->name);
   rc = strcmp (x_one->data->name + a_lvl, x_two->data->name + a_lvl);
   DEBUG_SORT   yLOG_value   ("compare"   , rc);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
api_ysort__unlinker     (uchar a_type, void **a_head, void **a_tail, void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPTRS      *x_head      = NULL;
   tPTRS      *x_tail      = NULL;
   tPTRS      *x_two       = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = api_ysort__common ('i', NULL, a_two);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_head   = * ((tPTRS **) a_head);
   x_tail   = * ((tPTRS **) a_tail);
   DEBUG_SORT   yLOG_complex ("pointers"  , "head %p, tail %p", x_head, x_tail);
   x_two    = (tPTRS *) a_two;
   /*---(unlink current from list)-------*/
   DEBUG_SORT   yLOG_note    ("unlink");
   if (x_two->s_next != NULL) x_two->s_next->s_prev = x_two->s_prev;
   else                       x_tail                = x_two->s_prev;
   if (x_two->s_prev != NULL) x_two->s_prev->s_next = x_two->s_next;
   else                       x_head                = x_two->s_next;
   /*---(ground pointers)----------------*/
   DEBUG_SORT   yLOG_note    ("ground pointers");
   x_two->s_next = NULL;
   x_two->s_prev = NULL;
   /*---(save back)----------------------*/
   DEBUG_SORT   yLOG_note    ("save back");
   *a_head = x_head;
   *a_tail = x_tail;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_ysort__linker       (uchar a_type, void **a_head, void **a_tail, void *a_one, void *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPTRS      *x_head      = NULL;
   tPTRS      *x_tail      = NULL;
   tPTRS      *x_one       = NULL;
   tPTRS      *x_two       = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = api_ysort__common ('i', a_one, a_two);
   --rce;  if (rc < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_head   = * ((tPTRS **) a_head);
   x_tail   = * ((tPTRS **) a_tail);
   x_one    = (tPTRS *) a_one;
   x_two    = (tPTRS *) a_two;
   /*---(insert back in)-----------------*/
   if (a_one == NULL) {
      if (x_head == NULL) {
         DEBUG_SORT   yLOG_note    ("add first");
         x_head           = x_two;
         x_two->s_prev  = NULL;
      } else {
         DEBUG_SORT   yLOG_note    ("append to tail");
         x_tail->s_next = x_two;
         x_two->s_prev  = x_tail;
      }
      x_tail           = x_two;
      x_two->s_next  = NULL;
   } else {
      DEBUG_SORT   yLOG_note    ("insert before");
      if (x_one->s_prev != NULL)   x_one->s_prev->s_next = x_two;
      else                         x_head                = x_two;
      x_two->s_prev  = x_one->s_prev;
      x_two->s_next  = x_one;
      x_one->s_prev  = x_two;
   }
   /*---(save back)----------------------*/
   DEBUG_SORT   yLOG_note    ("save back");
   *a_head = x_head;
   *a_tail = x_tail;
   /*---(output)-------------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    troll call-backs                          ----===*/
/*====================------------------------------------====================*/
static void      o___TROLL___________________o (void) {;}

char
api_ysort__slotter      (uchar a_lvl, void *a_two, uchar a_order)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tPTRS      *x_two       = NULL;
   int         x_len       =    0;
   char        t           [LEN_LABEL] = "";
   uchar       x_slot      =    0;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_value   ("a_lvl"     , a_lvl);
   --rce;  if (a_lvl < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_point   ("a_two"     , a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cast)---------------------------*/
   x_two = (tPTRS *) a_two;
   /*---(normal)-------------------------*/
   DEBUG_SORT   yLOG_info    ("data->name", x_two->data->name);
   x_len = strlen (x_two->data->name);
   DEBUG_SORT   yLOG_value   ("x_len"     , x_len);
   if (a_lvl < x_len)    x_slot = x_two->data->name [a_lvl];
   else                  x_slot = 0;
   /*---(slot)---------------------------*/
   DEBUG_SORT   yLOG_char    ("x_slot"    , ychrvisible (x_slot));
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return x_slot;
}

char
api_ysort__joiner       (void **a_bighead, void **a_bigtail, int *a_bigcount, void **a_subhead, void **a_subtail, int *a_subcount)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tPTRS      *x_one       = NULL;
   tPTRS      *x_two       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_point   ("a_subhead" , *a_subhead);
   --rce;  if (a_subhead == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initial load)-------------------*/
   DEBUG_SORT   yLOG_point   ("a_bighead" , *a_bighead);
   if (*a_bighead == NULL) {
      DEBUG_SORT   yLOG_note    ("initial load");
      *a_bighead   = *a_subhead;
   }
   /*---(appending load)-----------------*/
   else {
      DEBUG_SORT   yLOG_note    ("appending load");
      x_one = (tPTRS *) *a_bigtail;
      x_two = (tPTRS *) *a_subhead;
      x_one->s_next = x_two;
      x_two->s_prev = x_one;
   }
   /*---(common actions)-----------------*/
   *a_bigtail   = *a_subtail;
   *a_bigcount += *a_subcount;
   /*---(wipe sublist)-------------------*/
   *a_subhead   = NULL;
   *a_subtail   = NULL;
   *a_subcount  = 0;
   /*> DEBUG_SORT   ysort_mock_printer (*a_bighead);                                  <*/
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
api_ysort_init          (void)
{
   ySORT_config  (YSORT_TROLL, api_ysort__cursor, api_ysort__checker, api_ysort__unlinker, api_ysort__linker, api_ysort__slotter, api_ysort__joiner, NULL);
   return 0;
}



/*===============================[[ end-code ]]===============================*/
