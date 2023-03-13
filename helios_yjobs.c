/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
poly_yjobs_callback     (cchar a_req, cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_char    ("a_req"     , a_req);
   DEBUG_PROG    yLOG_info    ("a_data"    , a_data);
   /*---(handle)-------------------------*/
   /*> switch (a_req) {                                                               <* 
    *> case YJOBS_READ     :  rc = poly_yjobs_read     ();          break;            <* 
    *> case YJOBS_STATS    :  rc = poly_yjobs_stats    ();          break;            <* 
    *> case YJOBS_WRITE    :  rc = poly_yjobs_write    ();          break;            <* 
    *> case YJOBS_PULL     :  rc = poly_yjobs_pull     (a_data);    break;            <* 
    *> case YJOBS_CLEAR    :  rc = poly_yjobs_clear    (a_data);    break;            <* 
    *> case YJOBS_LOCALRPT :  rc = poly_yjobs_localrpt ();          break;            <* 
    *> case YJOBS_REPORT   :  rc = poly_yjobs_report   (a_data);    break;            <* 
    *> }                                                                              <*/
   /*---(trouble)------------------------*/
   if (rc < 0) {
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return 0;
}

