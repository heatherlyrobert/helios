/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



char         /*===[[ gather entries in dir ]]=============[ ------ [ ------ ]=*/
RPTG_regex         (
      /*---(formal parameters)+-------------+---------------------------------*/
      int         a_level     ,             /* depth of search                */
      tPTRS      *a_ptrs      ,             /* ptrs of directory to scan      */
      char       *a_path      )             /* full path of directory to scan */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tDATA      *x_dir       = NULL;          /* directory data                 */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   tDATA      *x_data      = NULL;          /* current entry data             */
   char        rc          = 0;
   char        x_path      [500];
   int         i           = 0;
   char        x_allowed   = '-';
   char        x_comma     [20] = "";
   long        x_days      =   0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_value   ("a_level"   , a_level);
   DEBUG_GRAF   yLOG_point   ("a_ptrs"    , a_ptrs);
   DEBUG_GRAF   yLOG_info    ("a_path"    , a_path);
   /*---(defense)------------------------*/
   --rce;  if (a_level >  my.maxlevel) {
      DEBUG_GRAF   yLOG_note    ("max recursion reached, return");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (my.regex_len  <= 0) {
      DEBUG_GRAF   yLOG_note    ("regex not loaded, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_ptrs == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory pointer NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_point   ("dir_data"  , a_ptrs->data);
   --rce;  if (a_ptrs->data == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory data payload NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_dir = a_ptrs->data;
   DEBUG_GRAF   yLOG_point   ("dir_name"  , x_dir->name);
   --rce;  if (x_dir->name == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory name NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_info    ("dir_name"  , x_dir->name);
   /*---(name directory)--------------*/
   if      (a_level == 1)  sprintf (x_path, "%s"   , x_dir->name);
   else if (a_level == 2)  sprintf (x_path, "/%s"  , x_dir->name);
   else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);
   DEBUG_GRAF   yLOG_info    ("x_path"    , x_path);
   OPT_VERBOSE  printf ("%s\n", x_path);
   /*---(check start path)---------------*/
   DEBUG_GRAF   yLOG_value   ("my.level"  , my.level);
   if (a_level <= my.level) {
      DEBUG_GRAF   yLOG_note    ("using --start path");
      x_ptrs = root_stack [a_level];
      x_data = x_ptrs->data;
      DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
      RPTG_regex (a_level + 1, x_ptrs, x_path);
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(spin through entries)-----------*/
   x_ptrs = a_ptrs->sib_head;
   while (x_ptrs != NULL) {
      x_data = x_ptrs->data;
      if (x_data != NULL) {
         DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
         x_days = (my.runtime - x_data->changed) / (365 * 24 * 60);
         if (x_data->type != 'd') {
            /*---(non-regex filtering)---*/
            if (my.find == 'y') {
               /*---(days)---------------*/
               if      (my.find_days  == 'j' && x_days >=   2) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_days  == 'w' && x_days >=   7) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_days  == 'm' && x_days >=  30) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_days  == 'y' && x_days >= 365) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_days  == 'o' && x_days <= 365) { x_ptrs = x_ptrs->sib_next; continue; }
               /*---(size)---------------*/
               if      (my.find_size  == 's' && x_data->size >=   3) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_size  == 'k' && (x_data->size < 4 || x_data->size > 6)) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_size  == 'm' && (x_data->size < 7 || x_data->size > 9)) { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_size  == 'g' && x_data->size <=   9) { x_ptrs = x_ptrs->sib_next; continue; }
               /*---(name quality)-------*/
               if      (my.find_name  == 'p' && x_data->ascii != '+') { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_name  == 'e' && x_data->ascii != '#') { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_name  == 's' && x_data->ascii != '>') { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_name  == 'j' && x_data->ascii != 'X') { x_ptrs = x_ptrs->sib_next; continue; }
               else if (my.find_name  == 'b' && strchr ("+#>X", x_data->ascii) == NULL) { x_ptrs = x_ptrs->sib_next; continue; }
               /*---(mime-cat)-----------*/
               if (my.find_cat != '-') {
                  if (my.find_cat != x_data->cat) {
                     x_ptrs = x_ptrs->sib_next;
                     continue;
                  }
               }
               /*---(mime-type)----------*/
               if (my.find_mime[0] != '\0') {
                  if (my.find_mime [0] != x_data->ext [0] || strcmp (my.find_mime, x_data->ext) != 0) {
                     x_ptrs = x_ptrs->sib_next;
                     continue;
                  }
               }
               /*---(done)---------------*/
            }
            /*---(regex filtering)-------*/
            rc = regexec (&(my.regex_comp), x_data->name, 0, NULL, 0);
            if (rc == 0 && my.total < my.limit) {
               ++my.total;
               if (my.count != 'y' && (my.number == 0  || my.number == my.total)) {
                  if (my.show_cat   == 'y')  printf ("%c  "    , x_data->cat);
                  if (my.show_mime  == 'y')  printf ("%-5s  "  , x_data->ext);
                  if (my.show_days  == 'y')  printf ("%4d  "   , x_days);
                  if (my.show_size  == 'y')  printf ("%1d  "   , x_data->size);
                  if (my.show_bytes == 'y') { FILE_commas (x_data->bytes, x_comma); printf ("%11.11s  ", x_comma); }
                  if (my.show_level == 'y')  printf ("%2d  "   , x_data->level);
                  if (my.show_ascii == 'y')  printf ("%c  "    , x_data->ascii);
                  printf ("%s/%s\n", x_path, x_data->name);
               }
               for (i = 0; i < n_mime; ++i) {
                  if (mime [i].cat       != x_data->cat)        continue;
                  if (strcmp (mime [i].ext, x_data->ext) != 0)  continue;
                  DEBUG_GRAF   yLOG_info    ("type"      , mime [i].ext);
                  ++(mime [i].found);
                  mime [i].fbytes += x_data->bytes;
                  ++(mime [0].found);
                  mime [0].fbytes += x_data->bytes;
                  break;
               }
            }
         } else {
            x_allowed = '-';
            if (my.uid == 0)                                                        x_allowed = 'r';
            else if ((((x_data->oth - '0') % 4) % 2) == 1)                          x_allowed = 'o';
            else if (x_data->gid == my.gid && (((x_data->grp - '0') % 4) % 2) == 1) x_allowed = 'g';
            else if (x_data->uid == my.uid && (((x_data->own - '0') % 4) % 2) == 1) x_allowed = 'u';
            DEBUG_PERMS  printf ("%c   ", x_allowed);
            DEBUG_PERMS  printf ("%c %d   ", x_data->oth, ((x_data->oth - '0') % 4) % 2);
            DEBUG_PERMS  printf ("%4d %4d %c %d   ", my.gid, x_data->gid, x_data->grp, ((x_data->grp - '0') % 4) % 2);
            DEBUG_PERMS  printf ("%4d %4d %c %d   ", my.uid, x_data->uid, x_data->own, ((x_data->own - '0') % 4) % 2);
            DEBUG_PERMS  printf ("%s/%s\n", x_path, x_data->name);
            if (x_allowed != '-')  RPTG_regex (a_level + 1, x_ptrs, x_path);
         }
      }
      x_ptrs = x_ptrs->sib_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ output the dir tree ]]===============[ ------ [ ------ ]=*/
RPTG_dirtree       (
      /*---(formal parameters)+-------------+---------------------------------*/
      int         a_level     ,             /* depth of search                */
      tPTRS      *a_ptrs      ,             /* ptrs of directory to scan      */
      char       *a_path      )             /* full path of directory to scan */
{  /*---(local variables)--+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tDATA      *x_dir       = NULL;          /* directory data                 */
   tPTRS      *x_ptrs      = NULL;          /* current entry                  */
   tDATA      *x_data      = NULL;          /* current entry data             */
   char        rc          = 0;
   char        x_path      [500];
   int         i           = 0;
   char        x_prefix    [100] = "";
   char        x_temp      [200] = "";
   char        x_cum       [20]  = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_value   ("a_level"   , a_level);
   DEBUG_GRAF   yLOG_point   ("a_ptrs"    , a_ptrs);
   DEBUG_GRAF   yLOG_info    ("a_path"    , a_path);
   /*---(defense)------------------------*/
   --rce;  if (a_level >  my.maxlevel) {
      DEBUG_GRAF   yLOG_note    ("max recursion reached, return");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (a_ptrs == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory pointer NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_point   ("dir_data"  , a_ptrs->data);
   --rce;  if (a_ptrs->data == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory data payload NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_dir = a_ptrs->data;
   DEBUG_GRAF   yLOG_point   ("dir_name"  , x_dir->name);
   --rce;  if (x_dir->name == NULL) {
      DEBUG_GRAF   yLOG_note    ("directory name NULL, failed");
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_GRAF   yLOG_info    ("dir_name"  , x_dir->name);
   /*---(header)-------------------------*/
   if (a_level == 1) {
      printf ("#!/usr/local/bin/hyleoroi\n");
      printf ("#   hyleoroi -- tree visualization input file\n");
      printf ("\n\n\n");
      printf ("SIMPLE\n");
      printf ("\n\n\n");
      printf ("#--context  ---values------------------------------- \n");
      printf ("source      heilos-phaeton                           \n");
      printf ("label       full directory tree for 2017-05-21       \n");
      printf ("format      dirtree                                  \n");
      printf ("\n\n\n");
   }
   /*---(prefix)-------------------------*/
   for (i = 1; i < a_level; ++i)  strcat (x_prefix, "   ");
   /*---(name directory)--------------*/
   if      (a_level == 1)  sprintf (x_path, "%s"   , x_dir->name);
   else if (a_level == 2)  sprintf (x_path, "/%s"  , x_dir->name);
   else                    sprintf (x_path, "%s/%s", a_path, x_dir->name);
   DEBUG_GRAF   yLOG_info    ("x_path"    , x_path);
   OPT_VERBOSE  printf ("%s\n", x_path);
   /*> printf ("%s%s\n", x_prefix, x_path);                                           <*/
   sprintf (x_temp, "%s%s", x_prefix, x_dir->name);
   /*> FILE_commas (x_dir->cum, x_cum);                                               <*/
   /*> printf  ("%-100.100s %14s\n", x_temp, x_cum);                                  <*/
   printf  ("%-100.100s | %14ld\n", x_temp, x_dir->cum);
   /*---(check start path)---------------*/
   DEBUG_GRAF   yLOG_value   ("my.level"  , my.level);
   if (a_level <= my.level) {
      DEBUG_GRAF   yLOG_note    ("using --start path");
      x_ptrs = root_stack [a_level];
      x_data = x_ptrs->data;
      DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
      RPTG_dirtree (a_level + 1, x_ptrs, x_path);
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(spin through entries)-----------*/
   x_ptrs = a_ptrs->sib_head;
   while (x_ptrs != NULL) {
      x_data = x_ptrs->data;
      if (x_data != NULL) {
         DEBUG_GRAF   yLOG_info    ("entry"     , x_data->name);
         if (x_data->type == 'd') {
            RPTG_dirtree (a_level + 1, x_ptrs, x_path);
         }
      }
      x_ptrs = x_ptrs->sib_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_summ          (void)
{
   /*---(local variables)--+-----------+-*/
   long        x_dseen     = 0;
   long        x_dkept     = 0;
   long        x_fseen     = 0;
   long        x_fkept     = 0;
   char        x_comma     [20] = "";
   int         i           = 0;
   struct      tm         *curr  = NULL;;
   time_t      x_written   = 0;
   char        x_time      [100] = "";
   tDRIVE     *x_drive     = NULL;
   /*---(process all)--------------------*/
   for (i = 1; i < n_mime; ++i) {
      if (mime [i].cat == 'd' && strcmp (mime [i].ext, "dir") == 0) {
         x_dseen  += mime [i].seen;
         x_dkept  += mime [i].kept;
      } else {
         x_fseen  += mime [i].seen;
         x_fkept  += mime [i].kept;
      }
   }
   /*---(output)-------------------------*/
   printf ("database %s:\n", my.database);
   printf ("\n");
   printf ("ref-   ---host--- --serial-- ---device------ ---mount-point------ --type-- ---bytes------------   ---written----------\n");
   x_drive = h_drive;
   while (x_drive != NULL) {
      x_written = x_drive->written;
      curr = localtime (&x_written);
      if (curr == NULL) printf ("local time failed\n");
      else              strftime (x_time, 100, "%y.%m.%d %H:%M:%S %U", curr);
      FILE_commas (x_drive->size , x_comma);
      printf ("[%02d]   %-10.10s %-10.10s %-15.15s %-20.20s %-8.8s %20.20s   %s\n",
            x_drive->ref   , x_drive->host  ,
            x_drive->serial, x_drive->device, x_drive->mpoint,
            x_drive->type  , x_comma        , x_time);
      x_drive = x_drive->next;
   }
   printf ("\n");
   printf ("---type----- ---seen----- ---kept----- ---each----- ---bytes----\n");
   printf ("directories ");
   FILE_commas (x_dseen, x_comma);
   printf (" %12.12s");
   FILE_commas (x_dkept, x_comma);
   printf (" %12.12s");
   FILE_commas (sizeof (tDATA), x_comma);
   printf (" %12.12s");
   FILE_commas (x_dkept * sizeof (tDATA), x_comma);
   printf (" %12.12s\n");
   printf ("files       ");
   FILE_commas (x_fseen, x_comma);
   printf (" %12.12s");
   FILE_commas (x_fkept, x_comma);
   printf (" %12.12s");
   FILE_commas (sizeof (tDATA), x_comma);
   printf (" %12.12s");
   FILE_commas (x_fkept * sizeof (tDATA), x_comma);
   printf (" %12.12s\n");
   printf ("             ------------ ------------              ------------\n");
   printf ("   total    ");
   FILE_commas (x_dseen + x_fseen, x_comma);
   printf (" %12.12s");
   FILE_commas (x_dkept + x_fkept, x_comma);
   printf (" %12.12s");
   printf ("             ");
   FILE_commas ((x_dkept + x_fkept) * sizeof (tDATA), x_comma);
   printf (" %12.12s\n");
   printf ("                                                    ------------\n");
   printf ("                  with device entries (appoximate) ");
   FILE_commas (sizeof (short) + (n_drive * sizeof (tDRIVE)) + ((x_dkept + x_fkept) * sizeof (tDATA)), x_comma);
   printf (" %12.12s\n");
   /*---(complete)-----------------------*/
   return 0;
}



/*===============================[[ end-code ]]===============================*/
