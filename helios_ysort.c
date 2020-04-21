/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



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
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_SORT   yLOG_spoint  (a_beg);
   DEBUG_SORT   yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_SORT   yLOG_snote   ("cast");
   x_beg    = (tPTRS *) a_beg;
   if (x_beg != NULL)  DEBUG_SORT   yLOG_snote   (x_beg->data->name);
   /*---(update)-------------------------*/
   DEBUG_SORT   yLOG_schar   (a_action);
   switch (a_action) {
   case '>' : if (x_beg != NULL) { x_new = x_beg->sib_next;   break; }
   case ']' : x_new = (tPTRS *) a_tail;                       break;
   case '<' : if (x_beg != NULL) { x_new = x_beg->sib_prev;   break; }
   case '[' : x_new = (tPTRS *) a_head;                       break;
   }
   /*---(save back)----------------------*/
   if (x_new != NULL)  DEBUG_SORT   yLOG_snote   (x_new->data->name);
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
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
   if (x_two->sib_next != NULL) x_two->sib_next->sib_prev = x_two->sib_prev;
   else                         x_tail                    = x_two->sib_prev;
   if (x_two->sib_prev != NULL) x_two->sib_prev->sib_next = x_two->sib_next;
   else                         x_head                    = x_two->sib_next;
   /*---(ground pointers)----------------*/
   DEBUG_SORT   yLOG_note    ("ground pointers");
   x_two->sib_next = NULL;
   x_two->sib_prev = NULL;
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
         x_two->sib_prev  = NULL;
      } else {
         DEBUG_SORT   yLOG_note    ("append to tail");
         x_tail->sib_next = x_two;
         x_two->sib_prev  = x_tail;
      }
      x_tail           = x_two;
      x_two->sib_next  = NULL;
   } else {
      DEBUG_SORT   yLOG_note    ("insert before");
      if (x_one->sib_prev != NULL)   x_one->sib_prev->sib_next = x_two;
      else                           x_head                    = x_two;
      x_two->sib_prev  = x_one->sib_prev;
      x_two->sib_next  = x_one;
      x_one->sib_prev  = x_two;
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
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_sint    (a_lvl);
   --rce;  if (a_lvl < 0) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cast)---------------------------*/
   x_two = (tPTRS *) a_two;
   /*---(normal)-------------------------*/
   DEBUG_SORT   yLOG_snote   (x_two->data->name);
   x_len = strlen (x_two->data->name);
   DEBUG_SORT   yLOG_sint    (x_len);
   if (a_lvl < x_len)    x_slot = x_two->data->name [a_lvl];
   else                  x_slot = 0;
   /*---(slot)---------------------------*/
   DEBUG_SORT   yLOG_schar   (chrvisible (x_slot));
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
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
      x_one->sib_next = x_two;
      x_two->sib_prev = x_one;
   }
   /*---(common actions)-----------------*/
   *a_bigtail   = *a_subtail;
   *a_bigcount += *a_subcount;
   /*---(wipe sublist)-------------------*/
   *a_subhead   = NULL;
   *a_subtail   = NULL;
   *a_subcount  = 0;
   DEBUG_SORT   ysort_mock_printer (*a_bighead);
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



/*====================------------------------------------====================*/
/*===----                 lazy teleporting gnome sort                  ----===*/
/*====================------------------------------------====================*/
static void      o___OLD_____________________o (void) {;}

/*> char         /+===[[ compare two nodes ]]=================[ leaf   [ ------ ]=+/   <* 
 *> GNOME_comp         (                                                               <* 
 *>       /+---(params)-----------+----------------*------------------------------+/   <* 
 *>       char        a_type1     ,                                                    <* 
 *>       char       *a_name1     ,                                                    <* 
 *>       char        a_type2     ,                                                    <* 
 *>       char       *a_name2     )                                                    <* 
 *> {  /+---(locals)-----------+-----------+-+/                                        <* 
 *>    char        x_cmp       = 0;                                                    <* 
 *>    /+---(check for type)-----------------+/                                        <* 
 *>    if (a_type1 != a_type2) {                                                       <* 
 *>       if (a_type1 == 'd')                              return  -4;                 <* 
 *>       if (a_type2 == 'd')                              return   4;                 <* 
 *>    }                                                                               <* 
 *>    /+---(most likely cases)--------------+/                                        <* 
 *>    if (a_name1 [0]  < a_name2 [0])                     return  -1;                 <* 
 *>    if (a_name1 [0]  > a_name2 [0])                     return   1;                 <* 
 *>    /+---(check for nulls)----------------+/                                        <* 
 *>    if (a_name1 [0] == '\0' && a_name2 [0] == '\0')     return   0;                 <* 
 *>    /+---(next most likely cases)---------+/                                        <* 
 *>    if (a_name1 [1]  < a_name2 [1])                     return  -2;                 <* 
 *>    if (a_name1 [1]  > a_name2 [1])                     return   2;                 <* 
 *>    /+---(finally the hard work)----------+/                                        <* 
 *>    x_cmp = strcmp (a_name1 , a_name2);                                             <* 
 *>    if (x_cmp < 0)                                      return  -3;                 <* 
 *>    if (x_cmp > 0)                                      return   3;                 <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+===[[ insert node before another ]]========[ leaf   [ ------ ]=+/   <* 
 *> GNOME_move         (                                                               <* 
 *>       /+---(params)-----------+----------------*------------------------------+/   <* 
 *>       tPTRS     **a_head      ,                /+ head of doubly-linked list  +/   <* 
 *>       tPTRS     **a_tail      ,                /+ tail of doubly-linked list  +/   <* 
 *>       tPTRS      *a_ins       ,                /+ node to insert              +/   <* 
 *>       tPTRS      *a_bef       )                /+ before this node            +/   <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    char        rce         = -10;                                                  <* 
 *>    /+---(header)-----------+-----------+-+/                                        <* 
 *>    DEBUG_SORT   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    DEBUG_SORT   yLOG_info    ("a_ins"     , a_ins->data->name);                    <* 
 *>    DEBUG_SORT   yLOG_point   ("->sib_prev", a_ins->sib_prev);                      <* 
 *>    DEBUG_SORT   yLOG_point   ("->sib_next", a_ins->sib_next);                      <* 
 *>    DEBUG_SORT   yLOG_info    ("a_bef"     , a_bef->data->name);                    <* 
 *>    DEBUG_SORT   yLOG_point   ("->sib_prev", a_bef->sib_prev);                      <* 
 *>    DEBUG_SORT   yLOG_point   ("->sib_next", a_bef->sib_next);                      <* 
 *>    /+---(defense)------------------------+/                                        <* 
 *>    --rce;  if (a_ins   == NULL)      return rce; /+ not given the first node  +/   <* 
 *>    --rce;  if (a_bef   == NULL)      return rce; /+ not given the second node +/   <* 
 *>    if         (a_ins == a_bef)       return 0;   /+ no swap necessary         +/   <* 
 *>    /+---(remove insert node)-------------+/                                        <* 
 *>    if (a_ins->sib_next == NULL)   *a_tail                    = a_ins->sib_prev;    <* 
 *>    else                            a_ins->sib_next->sib_prev = a_ins->sib_prev;    <* 
 *>    if (a_ins->sib_prev == NULL)   *a_head                    = a_ins->sib_next;    <* 
 *>    else                            a_ins->sib_prev->sib_next = a_ins->sib_next;    <* 
 *>    /+---(cleanse insert node)------------+/                                        <* 
 *>    a_ins->sib_prev  = NULL;                                                        <* 
 *>    a_ins->sib_next  = NULL;                                                        <* 
 *>    /+---(insert insert node)-------------+/                                        <* 
 *>    if (a_bef->sib_prev == NULL)   *a_head                    = a_ins;              <* 
 *>    else                            a_bef->sib_prev->sib_next = a_ins;              <* 
 *>    a_ins->sib_next        = a_bef;                                                 <* 
 *>    a_ins->sib_prev        = a_bef->sib_prev;                                       <* 
 *>    a_bef->sib_prev        = a_ins;                                                 <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_SORT   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+===[[ lazy teleporting gnome sort ]]=======[ ------ [ ------ ]=+/                                                          <* 
 *> GNOME_sort         (                                                                                                                      <* 
 *>       /+---(formal-parameters)+----------------+------------------------------+/                                                          <* 
 *>       int         a_level     ,                /+ level of recursion          +/                                                          <* 
 *>       tPTRS     **a_head      ,                /+ head of doubly-linked list  +/                                                          <* 
 *>       tPTRS     **a_tail      )                /+ tail of doubly-linked list  +/                                                          <* 
 *> {                                                                                                                                         <* 
 *>    /+---(local-variables)--+-----------+-+/                                                                                               <* 
 *>    char        rc          =   0;              /+ generic return code         +/                                                          <* 
 *>    char        rce         = -10;              /+ return code for errors      +/                                                          <* 
 *>    tPTRS      *x_curr      =  NULL;            /+ current node                +/                                                          <* 
 *>    tPTRS      *x_next      =  NULL;            /+ next node to be processed   +/                                                          <* 
 *>    tPTRS      *x_tele      =  NULL;            /+ teleportation point         +/                                                          <* 
 *>    tPTRS      *x_mark      =  NULL;            /+ laziest exchange point      +/                                                          <* 
 *>    tPTRS      *x_sort      =  NULL;            /+ sorting temp                +/                                                          <* 
 *>    int         x_count     = 0;                                                                                                           <* 
 *>    /+---(defenses)-----------------------+/                                                                                               <* 
 *>    --rce;  if (a_head  == NULL)      return rce; /+ not given the head        +/                                                          <* 
 *>    --rce;  if (*a_head == NULL)      return rce; /+ list is empty             +/                                                          <* 
 *>    --rce;  if (a_tail  == NULL)      return rce; /+ not given the tail        +/                                                          <* 
 *>    --rce;  if (*a_tail == NULL)      return rce; /+ list is empty             +/                                                          <* 
 *>    /+---(header)-------------------------+/                                                                                               <* 
 *>    DEBUG_SORT   yLOG_enter   (__FUNCTION__);                                                                                              <* 
 *>    /+---(sort)----------------------------------+/                                                                                        <* 
 *>    x_tele = x_curr = *a_head;                                                                                                             <* 
 *>    while (x_curr != NULL && x_curr->sib_next != NULL) {                                                                                   <* 
 *>       /+---(current order)-------------------------+/                                                                                     <* 
 *>       DEBUG_SORT   yLOG_note    ("display order");                                                                                        <* 
 *>       x_sort = *a_head;                                                                                                                   <* 
 *>       x_count = 0;                                                                                                                        <* 
 *>       while (x_sort != NULL) {                                                                                                            <* 
 *>          DEBUG_SORT   yLOG_complex ("entry"     , "%2d) ptrs %p, data %p, name %s", x_count, x_sort, x_sort->data, x_sort->data->name);   <* 
 *>          ++x_count;                                                                                                                       <* 
 *>          x_sort = x_sort->sib_next;                                                                                                       <* 
 *>       }                                                                                                                                   <* 
 *>       DEBUG_SORT   yLOG_value   ("x_count"   , x_count);                                                                                  <* 
 *>       /+---(prepare)---------------------+/                                                                                               <* 
 *>       DEBUG_SORT   yLOG_info    ("x_curr"    , x_curr->data->name);                                                                       <* 
 *>       x_next = x_curr->sib_next;                                                                                                          <* 
 *>       DEBUG_SORT   yLOG_info    ("x_next"    , x_next->data->name);                                                                       <* 
 *>       if (x_tele == x_curr)     x_tele = x_next;                                                                                          <* 
 *>       DEBUG_SORT   yLOG_info    ("x_tele"    , x_tele->data->name);                                                                       <* 
 *>       /+---(compare)---------------------+/                                                                                               <* 
 *>       rc = GNOME_comp (x_curr->data->type, x_curr->data->name, x_tele->data->type, x_tele->data->name);                                   <* 
 *>       DEBUG_SORT   yLOG_value   ("comp_rc"   , rc);                                                                                       <* 
 *>       if (rc >=  0) {                                                                                                                     <* 
 *>          DEBUG_SORT   yLOG_note    ("delayed swap identified");                                                                           <* 
 *>          x_mark = x_curr;                                                                                                                 <* 
 *>          DEBUG_SORT   yLOG_info    ("x_mark"    , x_mark->data->name);                                                                    <* 
 *>          if (x_curr->sib_prev != NULL) {                                                                                                  <* 
 *>             DEBUG_SORT   yLOG_note    ("fall back another position");                                                                     <* 
 *>             x_curr = x_curr->sib_prev;                                                                                                    <* 
 *>             continue;                                                                                                                     <* 
 *>          }                                                                                                                                <* 
 *>          DEBUG_SORT   yLOG_note    ("hit the head, handle swap now");                                                                     <* 
 *>       } else {                                                                                                                            <* 
 *>          DEBUG_SORT   yLOG_note    ("no swap necessary");                                                                                 <* 
 *>       }                                                                                                                                   <* 
 *>       /+---(no swap)---------------------+/                                                                                               <* 
 *>       if (x_mark == NULL) {                                                                                                               <* 
 *>          DEBUG_SORT   yLOG_note    ("no mark so TELEPORT");                                                                               <* 
 *>          x_curr = x_tele;                                                                                                                 <* 
 *>          continue;                                                                                                                        <* 
 *>       }                                                                                                                                   <* 
 *>       /+---(lazy swap)-------------------+/                                                                                               <* 
 *>       x_curr = x_tele->sib_next;                                                                                                          <* 
 *>       DEBUG_SORT   yLOG_note    ("exchange teleport and mark");                                                                           <* 
 *>       GNOME_move (a_head, a_tail, x_tele, x_mark);                                                                                        <* 
 *>       x_tele = x_curr;                                                                                                                    <* 
 *>       x_mark = NULL;                                                                                                                      <* 
 *>       if (x_curr != NULL)  x_curr = x_curr->sib_prev;                                                                                     <* 
 *>                                                                                                                                           <* 
 *>    }  /+---(done)------------------------+/                                                                                               <* 
 *>    /+---(current order)-------------------------+/                                                                                        <* 
 *>    DEBUG_SORT   yLOG_note    ("display order");                                                                                           <* 
 *>    x_sort = *a_head;                                                                                                                      <* 
 *>    while (x_sort != NULL) {                                                                                                               <* 
 *>       DEBUG_SORT   yLOG_complex ("entry"     , "ptrs %p, data %p, name %s", x_sort, x_sort->data, x_sort->data->name);                    <* 
 *>       x_sort = x_sort->sib_next;                                                                                                          <* 
 *>    }                                                                                                                                      <* 
 *>    /+---(complete)------------------------------+/                                                                                        <* 
 *>    return 0;                                                                                                                              <* 
 *> }                                                                                                                                         <*/

/*> char         /+==> divide list into buckets ==============[ leaf   [ ------ ]=+/      <* 
 *> GNOME_scatter      (                                                                  <* 
 *>       /+---(params)-----------+----------------*------------------------------+/      <* 
 *>       int         a_level     ,                /+ level of recursion          +/      <* 
 *>       tPTRS     **a_head      ,                /+ head of doubly-linked list  +/      <* 
 *>       tPTRS     **a_tail      ,                /+ tail of doubly-linked list  +/      <* 
 *>       tSLOT       a_slots     [SEVENBIT])      /+ buckets for sorting into    +/      <* 
 *> {                                                                                     <* 
 *>    /+---(locals)-----------+-----------+-+/                                           <* 
 *>    int         i           = 0;             /+ generic iterator               +/      <* 
 *>    int         x_off       = 0;             /+ offset to slot                 +/      <* 
 *>    tPTRS      *x_curr      = NULL;          /+ current pointer                +/      <* 
 *>    tPTRS      *x_next      = NULL;          /+ next pointer                   +/      <* 
 *>    /+---(walk through the list)----------+/                                           <* 
 *>    x_curr = *a_head;                                                                  <* 
 *>    while (x_curr != NULL) {                                                           <* 
 *>       /+---(ready)-----------------------+/                                           <* 
 *>       x_next = x_curr->sib_next; /+ save before changing links     +/                 <* 
 *>       /+---(detatch from head)-----------+/                                           <* 
 *>       *a_head = x_next;                                                               <* 
 *>       if ( x_curr->sib_next           == NULL)  *a_tail = NULL;                       <* 
 *>       else x_curr->sib_next->sib_prev =  NULL; /+ becomes new head of old list   +/   <* 
 *>       /+---(cleanse sib pointers)--------+/                                           <* 
 *>       x_curr->sib_next = NULL;                                                        <* 
 *>       x_curr->sib_prev = NULL;                                                        <* 
 *>       /+---(identify slot)---------------+/                                           <* 
 *>       x_off = 0;                                                                      <* 
 *>       if (x_curr->data->len >= a_level)    x_off = x_curr->data->name [a_level];      <* 
 *>       if (x_off >= SEVENBIT || x_off < 0)  x_off = SEVENBIT - 1;                      <* 
 *>       /+---(attach to tail of slot)------+/                                           <* 
 *>       if (a_slots [x_off].tail == NULL) {                                             <* 
 *>          a_slots [x_off].tail           = x_curr;                                     <* 
 *>          a_slots [x_off].head           = x_curr;                                     <* 
 *>       } else {                                                                        <* 
 *>          a_slots [x_off].tail->sib_next = x_curr;                                     <* 
 *>          x_curr->sib_prev               = a_slots [x_off].tail;                       <* 
 *>          a_slots [x_off].tail           = x_curr;                                     <* 
 *>       }                                                                               <* 
 *>       ++a_slots [x_off].count;                                                        <* 
 *>       /+---(prepare for next)------------+/                                           <* 
 *>       x_curr = x_next;                                                                <* 
 *>    }                                                                                  <* 
 *>    /+---(complete)-----------------------+/                                           <* 
 *>    return 0;                                                                          <* 
 *> }                                                                                     <*/

/*> char         /+==> unite buckets into a list =============[ leaf   [ ------ ]=+/   <* 
 *> GNOME_gather       (                                                               <* 
 *>       /+---(params)-----------+----------------*------------------------------+/   <* 
 *>       int         a_level     ,                /+ level of recursion          +/   <* 
 *>       tPTRS     **a_head      ,                /+ head of doubly-linked list  +/   <* 
 *>       tPTRS     **a_tail      ,                /+ tail of doubly-linked list  +/   <* 
 *>       tSLOT       a_slots     [SEVENBIT])      /+ buckets for sorting into    +/   <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    int         i           = 0;             /+ generic iterator               +/   <* 
 *>    tPTRS      *x_curr      = NULL;          /+ current pointer                +/   <* 
 *>    tPTRS      *x_next      = NULL;          /+ next pointer                   +/   <* 
 *>    /+---(consolidate)--------------------+/                                        <* 
 *>    for (i = 0; i < SEVENBIT; ++i) {                                                <* 
 *>       x_curr   = a_slots [i].head;                                                 <* 
 *>       /+---(handle contents)-------------+/                                        <* 
 *>       while (x_curr != NULL) {                                                     <* 
 *>          x_next  = x_curr->sib_next;                                               <* 
 *>          /+---(detach from head)---------+/                                        <* 
 *>          if (x_curr->sib_next == NULL) {                                           <* 
 *>             a_slots [i].head = NULL;                                               <* 
 *>             a_slots [i].tail = NULL;                                               <* 
 *>          } else {                                                                  <* 
 *>             a_slots [i].head = x_next;                                             <* 
 *>          }                                                                         <* 
 *>          --a_slots [i].count;                                                      <* 
 *>          /+---(cleanse sib pointers)-----+/                                        <* 
 *>          x_curr->sib_next         = NULL;                                          <* 
 *>          x_curr->sib_prev         = NULL;                                          <* 
 *>          /+---(attach to tail)-----------+/                                        <* 
 *>          if (*a_head == 0) {                                                       <* 
 *>             *a_head                 = x_curr;                                      <* 
 *>          } else {                                                                  <* 
 *>             (*a_tail)->sib_next     = x_curr;                                      <* 
 *>             x_curr->sib_prev        = *a_tail;                                     <* 
 *>          }                                                                         <* 
 *>          *a_tail = x_curr;                                                         <* 
 *>          /+---(prepare for next)---------+/                                        <* 
 *>          x_curr = x_next;                                                          <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+==> bucket sort ===========================[ ------ [ ------ ]=+/   <* 
 *> GNOME_bucket       (                                                               <* 
 *>       /+---(params)-----------+----------------*------------------------------+/   <* 
 *>       int         a_level,                     /+ level of recursion          +/   <* 
 *>       tPTRS     **a_head      ,                /+ head of doubly-linked list  +/   <* 
 *>       tPTRS     **a_tail      )                /+ tail of doubly-linked list  +/   <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    int         i           = 0;             /+ generic iterator               +/   <* 
 *>    tSLOT       x_slots     [SEVENBIT];      /+ buckets for sorting            +/   <* 
 *>    /+---(initialize)---------------------+/                                        <* 
 *>    for (i = 0; i < SEVENBIT; ++i) {                                                <* 
 *>       x_slots [i].count = 0;                                                       <* 
 *>       x_slots [i].head  = NULL;                                                    <* 
 *>       x_slots [i].tail  = NULL;                                                    <* 
 *>    }                                                                               <* 
 *>    /+---(scatter into slots)-------------+/                                        <* 
 *>    GNOME_scatter (a_level, a_head, a_tail, x_slots);                               <* 
 *>    /+---(review results)-----------------+/                                        <* 
 *>    for (i = 1; i < SEVENBIT; ++i) {                                                <* 
 *>       if (x_slots[i].count <=  1) continue;                                        <* 
 *>       if (x_slots[i].count <= 10) {                                                <* 
 *>          GNOME_sort   (a_level    , &(x_slots[i].head), &(x_slots[i].tail));       <* 
 *>       } else {                                                                     <* 
 *>          GNOME_bucket (a_level + 1, &(x_slots[i].head), &(x_slots[i].tail));       <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    /+---(gather back from slots)---------+/                                        <* 
 *>    GNOME_gather (a_level, a_head, a_tail, x_slots);                                <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/



/*===============================[[ end-code ]]===============================*/
