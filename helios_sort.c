/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



/*====================------------------------------------====================*/
/*===----                 lazy teleporting gnome sort                  ----===*/
/*====================------------------------------------====================*/
static void      o___GNOME___________________o (void) {;}

char         /*===[[ compare two nodes ]]=================[ leaf   [ ------ ]=*/
GNOME_comp         (
      /*---(params)-----------+----------------*------------------------------*/
      char        a_type1     ,
      char       *a_name1     ,
      char        a_type2     ,
      char       *a_name2     )
{  /*---(locals)-----------+-----------+-*/
   char        x_cmp       = 0;
   /*---(check for type)-----------------*/
   if (a_type1 != a_type2) {
      if (a_type1 == 'd')                              return  -4;
      if (a_type2 == 'd')                              return   4;
   }
   /*---(most likely cases)--------------*/
   if (a_name1 [0]  < a_name2 [0])                     return  -1;
   if (a_name1 [0]  > a_name2 [0])                     return   1;
   /*---(check for nulls)----------------*/
   if (a_name1 [0] == '\0' && a_name2 [0] == '\0')     return   0;
   /*---(next most likely cases)---------*/
   if (a_name1 [1]  < a_name2 [1])                     return  -2;
   if (a_name1 [1]  > a_name2 [1])                     return   2;
   /*---(finally the hard work)----------*/
   x_cmp = strcmp (a_name1 , a_name2);
   if (x_cmp < 0)                                      return  -3;
   if (x_cmp > 0)                                      return   3;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*===[[ insert node before another ]]========[ leaf   [ ------ ]=*/
GNOME_move         (
      /*---(params)-----------+----------------*------------------------------*/
      tPTRS     **a_head      ,                /* head of doubly-linked list  */
      tPTRS     **a_tail      ,                /* tail of doubly-linked list  */
      tPTRS      *a_ins       ,                /* node to insert              */
      tPTRS      *a_bef       )                /* before this node            */
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   /*---(header)-----------+-----------+-*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_info    ("a_ins"     , a_ins->data->name);
   DEBUG_SORT   yLOG_point   ("->sib_prev", a_ins->sib_prev);
   DEBUG_SORT   yLOG_point   ("->sib_next", a_ins->sib_next);
   DEBUG_SORT   yLOG_info    ("a_bef"     , a_bef->data->name);
   DEBUG_SORT   yLOG_point   ("->sib_prev", a_bef->sib_prev);
   DEBUG_SORT   yLOG_point   ("->sib_next", a_bef->sib_next);
   /*---(defense)------------------------*/
   --rce;  if (a_ins   == NULL)      return rce; /* not given the first node  */
   --rce;  if (a_bef   == NULL)      return rce; /* not given the second node */
   if         (a_ins == a_bef)       return 0;   /* no swap necessary         */
   /*---(remove insert node)-------------*/
   if (a_ins->sib_next == NULL)   *a_tail                    = a_ins->sib_prev;
   else                            a_ins->sib_next->sib_prev = a_ins->sib_prev;
   if (a_ins->sib_prev == NULL)   *a_head                    = a_ins->sib_next;
   else                            a_ins->sib_prev->sib_next = a_ins->sib_next;
   /*---(cleanse insert node)------------*/
   a_ins->sib_prev  = NULL;
   a_ins->sib_next  = NULL;
   /*---(insert insert node)-------------*/
   if (a_bef->sib_prev == NULL)   *a_head                    = a_ins;
   else                            a_bef->sib_prev->sib_next = a_ins;
   a_ins->sib_next        = a_bef;
   a_ins->sib_prev        = a_bef->sib_prev;
   a_bef->sib_prev        = a_ins;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ lazy teleporting gnome sort ]]=======[ ------ [ ------ ]=*/
GNOME_sort         (
      /*---(formal-parameters)+----------------+------------------------------*/
      int         a_level     ,                /* level of recursion          */
      tPTRS     **a_head      ,                /* head of doubly-linked list  */
      tPTRS     **a_tail      )                /* tail of doubly-linked list  */
{
   /*---(local-variables)--+-----------+-*/
   char        rc          =   0;              /* generic return code         */
   char        rce         = -10;              /* return code for errors      */
   tPTRS      *x_curr      =  NULL;            /* current node                */
   tPTRS      *x_next      =  NULL;            /* next node to be processed   */
   tPTRS      *x_tele      =  NULL;            /* teleportation point         */
   tPTRS      *x_mark      =  NULL;            /* laziest exchange point      */
   tPTRS      *x_sort      =  NULL;            /* sorting temp                */
   int         x_count     = 0;
   /*---(defenses)-----------------------*/
   --rce;  if (a_head  == NULL)      return rce; /* not given the head        */
   --rce;  if (*a_head == NULL)      return rce; /* list is empty             */
   --rce;  if (a_tail  == NULL)      return rce; /* not given the tail        */
   --rce;  if (*a_tail == NULL)      return rce; /* list is empty             */
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(sort)----------------------------------*/
   x_tele = x_curr = *a_head;
   while (x_curr != NULL && x_curr->sib_next != NULL) {
      /*---(current order)-------------------------*/
      DEBUG_SORT   yLOG_note    ("display order");
      x_sort = *a_head;
      x_count = 0;
      while (x_sort != NULL) {
         DEBUG_SORT   yLOG_complex ("entry"     , "%2d) ptrs %p, data %p, name %s", x_count, x_sort, x_sort->data, x_sort->data->name);
         ++x_count;
         x_sort = x_sort->sib_next;
      }
      DEBUG_SORT   yLOG_value   ("x_count"   , x_count);
      /*---(prepare)---------------------*/
      DEBUG_SORT   yLOG_info    ("x_curr"    , x_curr->data->name);
      x_next = x_curr->sib_next;
      DEBUG_SORT   yLOG_info    ("x_next"    , x_next->data->name);
      if (x_tele == x_curr)     x_tele = x_next;
      DEBUG_SORT   yLOG_info    ("x_tele"    , x_tele->data->name);
      /*---(compare)---------------------*/
      rc = GNOME_comp (x_curr->data->type, x_curr->data->name, x_tele->data->type, x_tele->data->name);
      DEBUG_SORT   yLOG_value   ("comp_rc"   , rc);
      if (rc >=  0) {
         DEBUG_SORT   yLOG_note    ("delayed swap identified");
         x_mark = x_curr;
         DEBUG_SORT   yLOG_info    ("x_mark"    , x_mark->data->name);
         if (x_curr->sib_prev != NULL) {
            DEBUG_SORT   yLOG_note    ("fall back another position");
            x_curr = x_curr->sib_prev;
            continue;
         }
         DEBUG_SORT   yLOG_note    ("hit the head, handle swap now");
      } else {
         DEBUG_SORT   yLOG_note    ("no swap necessary");
      }
      /*---(no swap)---------------------*/
      if (x_mark == NULL) {
         DEBUG_SORT   yLOG_note    ("no mark so TELEPORT");
         x_curr = x_tele;
         continue;
      }
      /*---(lazy swap)-------------------*/
      x_curr = x_tele->sib_next;
      DEBUG_SORT   yLOG_note    ("exchange teleport and mark");
      GNOME_move (a_head, a_tail, x_tele, x_mark);
      x_tele = x_curr;
      x_mark = NULL;
      if (x_curr != NULL)  x_curr = x_curr->sib_prev;

   }  /*---(done)------------------------*/
   /*---(current order)-------------------------*/
   DEBUG_SORT   yLOG_note    ("display order");
   x_sort = *a_head;
   while (x_sort != NULL) {
      DEBUG_SORT   yLOG_complex ("entry"     , "ptrs %p, data %p, name %s", x_sort, x_sort->data, x_sort->data->name);
      x_sort = x_sort->sib_next;
   }
   /*---(complete)------------------------------*/
   return 0;
}

char         /*==> divide list into buckets ==============[ leaf   [ ------ ]=*/
GNOME_scatter      (
      /*---(params)-----------+----------------*------------------------------*/
      int         a_level     ,                /* level of recursion          */
      tPTRS     **a_head      ,                /* head of doubly-linked list  */
      tPTRS     **a_tail      ,                /* tail of doubly-linked list  */
      tSLOT       a_slots     [SEVENBIT])      /* buckets for sorting into    */
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;             /* generic iterator               */
   int         x_off       = 0;             /* offset to slot                 */
   tPTRS      *x_curr      = NULL;          /* current pointer                */
   tPTRS      *x_next      = NULL;          /* next pointer                   */
   /*---(walk through the list)----------*/
   x_curr = *a_head;
   while (x_curr != NULL) {
      /*---(ready)-----------------------*/
      x_next = x_curr->sib_next; /* save before changing links     */
      /*---(detatch from head)-----------*/
      *a_head = x_next;
      if ( x_curr->sib_next           == NULL)  *a_tail = NULL;
      else x_curr->sib_next->sib_prev =  NULL; /* becomes new head of old list   */
      /*---(cleanse sib pointers)--------*/
      x_curr->sib_next = NULL;
      x_curr->sib_prev = NULL;
      /*---(identify slot)---------------*/
      x_off = 0;
      if (x_curr->data->len >= a_level)    x_off = x_curr->data->name [a_level];
      if (x_off >= SEVENBIT || x_off < 0)  x_off = SEVENBIT - 1;
      /*---(attach to tail of slot)------*/
      if (a_slots [x_off].tail == NULL) {
         a_slots [x_off].tail           = x_curr;
         a_slots [x_off].head           = x_curr;
      } else {
         a_slots [x_off].tail->sib_next = x_curr;
         x_curr->sib_prev               = a_slots [x_off].tail;
         a_slots [x_off].tail           = x_curr;
      }
      ++a_slots [x_off].count;
      /*---(prepare for next)------------*/
      x_curr = x_next;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*==> unite buckets into a list =============[ leaf   [ ------ ]=*/
GNOME_gather       (
      /*---(params)-----------+----------------*------------------------------*/
      int         a_level     ,                /* level of recursion          */
      tPTRS     **a_head      ,                /* head of doubly-linked list  */
      tPTRS     **a_tail      ,                /* tail of doubly-linked list  */
      tSLOT       a_slots     [SEVENBIT])      /* buckets for sorting into    */
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;             /* generic iterator               */
   tPTRS      *x_curr      = NULL;          /* current pointer                */
   tPTRS      *x_next      = NULL;          /* next pointer                   */
   /*---(consolidate)--------------------*/
   for (i = 0; i < SEVENBIT; ++i) {
      x_curr   = a_slots [i].head;
      /*---(handle contents)-------------*/
      while (x_curr != NULL) {
         x_next  = x_curr->sib_next;
         /*---(detach from head)---------*/
         if (x_curr->sib_next == NULL) {
            a_slots [i].head = NULL;
            a_slots [i].tail = NULL;
         } else {
            a_slots [i].head = x_next;
         }
         --a_slots [i].count;
         /*---(cleanse sib pointers)-----*/
         x_curr->sib_next         = NULL;
         x_curr->sib_prev         = NULL;
         /*---(attach to tail)-----------*/
         if (*a_head == 0) {
            *a_head                 = x_curr;
         } else {
            (*a_tail)->sib_next     = x_curr;
            x_curr->sib_prev        = *a_tail;
         }
         *a_tail = x_curr;
         /*---(prepare for next)---------*/
         x_curr = x_next;
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*==> bucket sort ===========================[ ------ [ ------ ]=*/
GNOME_bucket       (
      /*---(params)-----------+----------------*------------------------------*/
      int         a_level,                     /* level of recursion          */
      tPTRS     **a_head      ,                /* head of doubly-linked list  */
      tPTRS     **a_tail      )                /* tail of doubly-linked list  */
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;             /* generic iterator               */
   tSLOT       x_slots     [SEVENBIT];      /* buckets for sorting            */
   /*---(initialize)---------------------*/
   for (i = 0; i < SEVENBIT; ++i) {
      x_slots [i].count = 0;
      x_slots [i].head  = NULL;
      x_slots [i].tail  = NULL;
   }
   /*---(scatter into slots)-------------*/
   GNOME_scatter (a_level, a_head, a_tail, x_slots);
   /*---(review results)-----------------*/
   for (i = 1; i < SEVENBIT; ++i) {
      if (x_slots[i].count <=  1) continue;
      if (x_slots[i].count <= 10) {
         GNOME_sort   (a_level    , &(x_slots[i].head), &(x_slots[i].tail));
      } else {
         GNOME_bucket (a_level + 1, &(x_slots[i].head), &(x_slots[i].tail));
      }
   }
   /*---(gather back from slots)---------*/
   GNOME_gather (a_level, a_head, a_tail, x_slots);
   /*---(complete)-----------------------*/
   return 0;
}



/*===============================[[ end-code ]]===============================*/
