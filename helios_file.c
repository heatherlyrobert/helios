/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */



/*
 *
 *
 * status of dir_normal means helios will recurce into all sub-directories
 * and record files, attributes, and sizes
 *
 * allowed verbs...
 *
 *    dir_silent  : instructs helios to recurse into, but not record names below
 *                  this level, not useful, just capture cumulative sizes
 *
 *    dir_pass    : instructs helios to not recurse into this path, due to
 *                  value, risk, technical, or security reasons
 *
 *    dir_last    : instructs helios to recurse into, but not below this path,
 *                  due to value, risk, technical, or security reasons
 *
 *    dir_never   : instructs helios to completely ignore ALL instances of
 *                  directories using this name (example .git)
 *
 *
 *    mountpoint  : provides helios with a default starting point for database
 *                  craetion and update
 *
 *
 */

#define     MAX_CAT         50
typedef     struct      cCAT        tCAT;
struct cCAT {
   char        cat;
   char        desc        [LEN_DESC];
};
tCAT        s_cats [MAX_CAT] = {
   /*-cat-  ---desc----------------------------------*/
   {  MIME_AUDIO ,  "audio"                                  },
   {  MIME_VIDEO ,  "video"                                  },
   {  MIME_IMAGE ,  "image"                                  },
   {  MIME_CODE  ,  "source code"                            },
   {  MIME_ASCII ,  "plain ascii text"                       },
   {  MIME_CRYPT ,  "archive/encrypted"                      },
   {  MIME_PROP  ,  "proprietary/closed"                     },
   {  MIME_EXEC  ,  "executable"                             },
   {  MIME_TEMP  ,  "transient/temp"                         },
   {  MIME_DIR   ,  "dir"                                    },
   {  MIME_OTHER ,  "other/misc"                             },
   {  MIME_HUH   ,  "unknown"                                },
   {  ' '        ,  "---end---"                              },
};


/*====================------------------------------------====================*/
/*===----                           database                           ----===*/
/*====================------------------------------------====================*/
static void      o___DATABASE________________o (void) {;}

char         /*===[[ write dir to binary file ]]==========[ ------ [ ------ ]=*/
FWRITE_dir         (
      /*---(formal parameters)+-------------+---------------------------------*/
      FILE       *a_file      ,             /* destination file               */
      tPTRS      *a_ptrs      ,             /* parent of entries              */
      int        *a_count     )             /* current line count             */
{  /*---(locals)-----------+-----------+-*/
   tPTRS      *x_ptrs      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_point   ("a_ptrs"    , a_ptrs);
   /*---(defense)------------------------*/
   if (a_ptrs == NULL) {
      DEBUG_OUTP   yLOG_note    ("hit a null ptrs pointer, FAIL");
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   DEBUG_OUTP   yLOG_value   ("children"  , a_ptrs->nchildren);
   if (a_ptrs->data == NULL) {
      DEBUG_OUTP   yLOG_note    ("hit a null data pointer, FAIL");
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   DEBUG_OUTP   yLOG_info    ("dir_name"  , a_ptrs->data->name);
   /*---(output)-------------------------*/
   x_ptrs = a_ptrs->sib_head;
   DEBUG_OUTP   yLOG_point   ("head"      , x_ptrs);
   while (x_ptrs != NULL) {
      if (x_ptrs->data != NULL) {
         fwrite (x_ptrs->data, sizeof (tENTRY), 1, a_file);
         ++(*a_count);
         if   (x_ptrs->data->type == ENTRY_DIR &&
               strchr (".)", x_ptrs->data->stype) != NULL) {
            DEBUG_OUTP   yLOG_note    ("directory, recurse");
            FWRITE_dir (a_file, x_ptrs, a_count);
         }
      }
      x_ptrs = x_ptrs->sib_next;
      DEBUG_OUTP   yLOG_point   ("next"      , x_ptrs);
   }  /*---(done)------------------------*/
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
FWRITE_all         (
      /*---(formal parameters)+-------------+---------------------------------*/
      tPTRS      *a_ptrs      )             /* parent of entries              */
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f_out       = NULL;          /* file pointer - commands        */
   int         x_count     = 0;
   tDRIVE     *x_drive     = NULL;
   /*---(open)---------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_point   ("a_ptrs"    , a_ptrs);
   DEBUG_OUTP   yLOG_info    ("output"    , my.database);
   f_out = fopen (my.database, "wb");
   DEBUG_OUTP   yLOG_point   ("f"         , f_out);
   if (f_out == NULL) {
      DEBUG_OUTP   yLOG_note    ("can not open output file");
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      return -1;
   }
   /*---(write header)-------------------*/
   fwrite (&n_drive, sizeof (short), 1, f_out);
   x_drive = h_drive;
   DEBUG_OUTP   yLOG_value   ("n_drive"   , n_drive);
   while (x_drive != NULL) {
      DEBUG_OUTP   yLOG_info    ("device"    , x_drive->device);
      fwrite (x_drive  , sizeof (tDRIVE), 1, f_out);
      x_drive = x_drive->next;
   }
   /*---(write entries)------------------*/
   fwrite (a_ptrs->data, sizeof (tENTRY), 1, f_out);
   ++x_count;
   FWRITE_dir (f_out, a_ptrs, &x_count);
   /*---(wrapup)-------------------------*/
   fclose (f_out);
   chmod (my.database, 0755);
   long x_end  = time (NULL);
   printf ("# ran for %ld secs\n", x_end - my.runtime);
   printf ("# wrote %d recs\n", x_count);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ read entries from database ]]========[ ------ [ ------ ]=*/
FREAD_all          (void)
{
   /*---(locals)-----------+-----------+-*/
   FILE       *f_in        = NULL;          /* file pointer                   */
   char        rce         = -10;
   tPTRS      *x_dir       = NULL;
   tPTRS      *x_ptrs      = NULL;
   tENTRY      *x_data      = NULL;
   int         x_tries     = 0;
   int         rc          = 0;
   int         x_dnode     = 0;
   int         x_dnodes    = -666;
   int         x_count     = 0;
   int         i           = 0;
   short       x_limit     = 0;
   tDRIVE     *x_drive     = NULL;
   tDRIVE      x_temp;
   tDRIVE     *x_prev      = NULL;
   tDRIVE     *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   if (my.dump == 'y') {
      printf ("## produced by /usr/bin/helios --dump\n");
   }
   /*---(open)---------------------------*/
   DEBUG_INPT   yLOG_note    ("open helios db file");
   f_in = fopen (my.database, "rb");
   DEBUG_INPT   yLOG_point   ("f_in"      , f_in);
   if (f_in == NULL) {
      printf ("can not open database %s\n", my.database);
      DEBUG_INPT   yLOG_note    ("can not open helios data file");
      return -1;
   }
   DEBUG_INPT   yLOG_note    ("file successfullly openned");
   /*---(get drive)----------------------*/
   fread  (&x_limit, sizeof (short), 1, f_in);
   DEBUG_INPT   yLOG_value   ("x_limit"   , x_limit);
   for (i = 0; i < x_limit; ++i) {
      rc = DRIVE_append (&x_drive);
      fread  (&x_temp   , sizeof (tDRIVE), 1, f_in);
      x_drive->ref   = x_temp.ref;
      strcpy (x_drive->host    , x_temp.host);
      strcpy (x_drive->serial  , x_temp.serial);
      strcpy (x_drive->device  , x_temp.device);
      strcpy (x_drive->mpoint  , x_temp.mpoint);
      strcpy (x_drive->type    , x_temp.type);
      x_drive->size    = x_temp.size;
      x_drive->written = x_temp.written;
      DEBUG_INPT   yLOG_info    ("host"      , x_drive->host);
      DEBUG_INPT   yLOG_info    ("serial"    , x_drive->serial);
      DEBUG_INPT   yLOG_info    ("device"    , x_drive->device);
      DEBUG_INPT   yLOG_info    ("mpoint"    , x_drive->mpoint);
      DEBUG_INPT   yLOG_info    ("type"      , x_drive->type);
      DEBUG_INPT   yLOG_llong   ("size"      , x_drive->size);
      DEBUG_INPT   yLOG_value   ("written"   , x_drive->written);
   }
   /*---(process entries)----------------*/
   DEBUG_INPT   yLOG_note    ("start reading records");
   --rce;
   while (1) {
      DEBUG_INPT   yLOG_enter   ("read_loop");
      DEBUG_INPT   yLOG_value   ("count"     , x_count);
      /*---(create data payload)------------*/
      DEBUG_INPT   yLOG_note    ("malloc local data payload");
      x_data  = NULL;
      x_tries = 0;
      while (x_data == NULL) {
         if (++x_tries > 3)  break;
         x_data = (tENTRY*) malloc (sizeof (tENTRY));
      }
      if (x_data == NULL) {
         DEBUG_INPT   yLOG_note    ("data malloc failed, DONE");
         DEBUG_INPT   yLOG_exit    ("read_loop");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_INPT   yLOG_value   ("tries"     , x_tries);
      DEBUG_INPT   yLOG_point   ("local data", x_data);
      DEBUG_INPT   yLOG_note    ("binary read");
      rc = fread (x_data, sizeof (tENTRY), 1, f_in);
      DEBUG_INPT   yLOG_value   ("fread_rc"  , rc);
      if (rc == 0) {
         DEBUG_INPT   yLOG_note    ("zero indicates end of file");
         DEBUG_INPT   yLOG_exit    ("read_loop");
         break;
      }
      DEBUG_INPT   yLOG_value   ("inode"     , x_data->inode);
      DEBUG_INPT   yLOG_value   ("dnode"     , x_data->dnode);
      DEBUG_INPT   yLOG_note    ("find dnode");
      /*> x_dnode = x_data->dnode;                                                    <* 
       *> if (x_data->level == 0) {                                                   <* 
       *>    DEBUG_INPT   yLOG_note    ("root, so no parent dnode");                  <* 
       *>    x_dir == NULL;                                                           <* 
       *> }  else if (x_dnode == x_dnodes) {                                          <* 
       *>    DEBUG_INPT   yLOG_note    ("same as last dnode, reuse");                 <* 
       *> } else {                                                                    <* 
       *>    x_dir = dir_stack [(int) x_data->level - 1];                             <* 
       *>    DEBUG_INPT   yLOG_point   ("x_dir"     , x_dir);                         <* 
       *>    if (x_dir == NULL) {                                                     <* 
       *>       DEBUG_INPT   yLOG_note    ("NULL parent means skip record");          <* 
       *>       DEBUG_INPT   yLOG_exit    ("read_loop");                              <* 
       *>       continue;                                                             <* 
       *>    }                                                                        <* 
       *> }                                                                           <*/
      /*> x_dnodes = x_dnode;                                                         <*/
      DEBUG_INPT   yLOG_info    ("name"      , x_data->name);
      if (x_data->level == 0) {
         x_dir == NULL;
      } else {
         x_dir = dir_stack [(int) x_data->level - 1];
         DEBUG_INPT   yLOG_point   ("x_dir"     , x_dir);
         DEBUG_INPT   yLOG_point   ("->data"    , x_dir->data);
         DEBUG_INPT   yLOG_info    ("->->name"  , x_dir->data->name);
      }
      DEBUG_INPT   yLOG_note    ("create ptrs and data");
      rc = ENTRY_new (&x_ptrs, x_dir);
      DEBUG_INPT   yLOG_point   ("new ptrs"  , x_ptrs);
      if (x_ptrs == NULL) {
         DEBUG_INPT   yLOG_note    ("failed creating ptrs");
         DEBUG_INPT   yLOG_exit    ("read_loop");
         break;
      }
      DEBUG_INPT   yLOG_point   ("new data"  , x_ptrs->data);
      DEBUG_INPT   yLOG_note    ("free new data node");
      free (x_ptrs->data);
      DEBUG_INPT   yLOG_note    ("attach local malloc'd data node");
      x_ptrs->data = x_data;
      DEBUG_INPT   yLOG_point   ("data now"  , x_ptrs->data);
      DEBUG_INPT   yLOG_note    ("handle if a dir");
      if (x_data->type == ENTRY_DIR && strchr (".)", x_data->stype) != NULL) {
         dir_stack [(int) x_data->level] = x_ptrs;
      }
      if (my.dump == 'y') {
         if ((x_count %  5) == 0) printf ("\n");
         if ((x_count % 45) == 0) printf ("#-count- dr ty prm uid- gid- -inode-- -dnode-- -changed-- sz ---bytes---- ---cum-------- c ext- lv a len ---name--------------------\n\n");
         printf ("%-8d %2d %c%c %d%d%d %4d %4d %8d %8d %10d %2d %12ld %14ld %c %-4.4s %2d %c %3d %s\n",
               x_count          ,
               x_data->drive    , x_data->type     , x_data->stype    ,
               x_data->own      , x_data->grp      , x_data->oth      ,
               x_data->uid      , x_data->gid      ,
               x_data->inode    , x_data->dnode    ,
               x_data->changed  ,
               x_data->size     , x_data->bytes    , x_data->cum      ,
               x_data->cat      , x_data->ext      ,
               x_data->level    , x_data->ascii    , x_data->len      ,
               x_data->name     );
         switch (x_data->cat) {
         case MIME_AUDIO  : ++my.n_audio;   break;
         case MIME_VIDEO  : ++my.n_video;   break;
         case MIME_IMAGE  : ++my.n_image;   break;
         case MIME_CODE   : ++my.n_code;    break;
         case MIME_ASCII  : ++my.n_ascii;   break;
         case MIME_DBASE  : ++my.n_dbase;   break;
         case MIME_CRYPT  : ++my.n_crypt;   break;
         case MIME_PROP   : ++my.n_prop;    break;
         case MIME_EXEC   : ++my.n_exec;    break;
         case MIME_DIR    : ++my.n_dir;     break;
         case MIME_TEMP   : ++my.n_temp;    break;
         case MIME_OTHER  : ++my.n_other;   break;
         case MIME_HUH    : ++my.n_huh;     break;
         }
      }
      ++x_count;
      DEBUG_INPT   yLOG_exit    ("read_loop");
   }
   if (my.dump == 'y') {
      if ((x_count %  5) != 0) printf ("\n");
      if ((x_count % 45) != 0) printf ("#-count- dr perms uid- gid- -inode-- -dnode-- -changed-- sz ---bytes---- ---cum-------- c ext- lv a len ---name--------------------\n\n");
   }
   /*---(wrapup)-------------------------*/
   fclose (f_in);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char 
FILE_percents      (long a_number, long a_total, char *a_string)
{
   /*---(locals)-----------+-----------+-*/
   double      x_percent   = 0;
   /*---(format)-------------------------*/
   if (a_number == 0) {
      strcpy (a_string, "-");
      return 0;
   }
   if (a_total  == 0) {
      strcpy (a_string, "-");
      return 0;
   }
   x_percent = ((double) a_number) / ((double) a_total);
   if (x_percent <= 0.009) {
      strcpy (a_string, "-");
      return 0;
   }
   if (x_percent > 1.0) {
      strcpy (a_string, "-");
      return 0;
   }
   sprintf (a_string, "%3.0lf", x_percent * 100.0);
   /*---(complete)-----------------------*/
   return 0;
}

char
FILE_commas        (llong a_number, char *a_string)
{
   strl4comma (a_number, a_string, 0, 'c', '-', LEN_LABEL);
   return 0;
}

char
FILE_uncommas      (char *a_string, llong *a_number)
{
   double     x_val;
   strl2comma (a_string, &x_val, LEN_LABEL);
   *a_number = x_val;
   return 0;
}

char
MIME_read          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   FILE       *f_mime      = NULL;          /* file pointer                   */
   int         f_lines     =    0;          /* line counter                   */
   char        x_recd      [MAX_RECD];      /* input record                   */
   int         x_len       =    0;          /* input length                   */
   char       *p           =    0;          /* strtok pointer                 */
   char       *q           = "";          /* strtok delimeter               */
   char       *s           = NULL;          /* strtok context                 */
   double      v           =  0.0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   f_mime = fopen (FILE_MIME, "r");
   DEBUG_CONF  yLOG_point   ("file"      , f_mime);
   --rce;  if (f_mime == NULL) {
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(read lines)---------------------*/
   DEBUG_CONF  yLOG_note    ("read lines");
   while (1) {
      /*---(read and clean)--------------*/
      ++f_lines;
      DEBUG_CONF  yLOG_value   ("line"      , f_lines);
      fgets (x_recd, MAX_RECD, f_mime);
      if (feof(f_mime))           break;
      if (x_recd [0] == '#')      continue;
      if (x_recd [0] == '\0')     continue;
      if (x_recd [0] == ' ')      continue;
      x_len = strlen (x_recd);
      if (x_len <= 0)             continue;
      x_recd [--x_len] = '\0';
      DEBUG_CONF  yLOG_value   ("n_mime"    , n_mime);
      DEBUG_CONF  yLOG_info    ("fixed"     , x_recd);
      /*---(extension)-------------------*/
      p = strtok_r (x_recd, q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      strcpy (mime [n_mime].ext , p);
      DEBUG_CONF  yLOG_info    ("ext"       , mime [n_mime].ext);
      /*---(category)--------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      mime [n_mime].cat = p [0];
      DEBUG_CONF  yLOG_char    ("cat"       , mime [n_mime].cat);
      /*---(desc)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      strcpy (mime [n_mime].desc , p);
      DEBUG_CONF  yLOG_info    ("desc"      , mime [n_mime].desc);
      /*---(like)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      mime [n_mime].like = p [0];
      DEBUG_CONF  yLOG_char    ("like"      , mime [n_mime].like);
      /*---(seen)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(mime [n_mime].seen));
      DEBUG_CONF  yLOG_value   ("seen"      , mime [n_mime].seen);
      /*---(seen percent)----------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(seen bytes)------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(mime [n_mime].sbytes));
      DEBUG_CONF  yLOG_value   ("sbytes"    , mime [n_mime].sbytes);
      /*---(seen bytes percent)----------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(kept)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(mime [n_mime].kept));
      DEBUG_CONF  yLOG_value   ("kept"      , mime [n_mime].kept);
      /*---(kept percent)----------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(kept bytes)------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(mime [n_mime].kbytes));
      DEBUG_CONF  yLOG_value   ("kbytes"    , mime [n_mime].kbytes);
      /*---(kept bytes percent)----------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(found)-----------------------*/
      mime [n_mime].found  = 0;
      mime [n_mime].fbytes = 0;
      /*---(save)------------------------*/
      if (mime[n_mime].cat == MIME_DIR   && strcmp (mime[n_mime].ext, EXT_DLINK  ) == 0) {
         my.mime_link  = n_mime;
      }
      if (mime[n_mime].cat == MIME_TEMP  && strcmp (mime[n_mime].ext, EXT_RLINK  ) == 0) {
         my.mime_link  = n_mime;
      }
      if (mime[n_mime].cat == MIME_EXEC  && strcmp (mime[n_mime].ext, EXT_EXEC   ) == 0) {
         my.mime_exe   = n_mime;
      }
      if (mime[n_mime].cat == MIME_ASCII && strcmp (mime[n_mime].ext, EXT_CONF   ) == 0) {
         my.mime_conf  = n_mime;
      }
      if (mime[n_mime].cat == MIME_HUH   && strcmp (mime[n_mime].ext, EXT_UNKNOWN) == 0) {
         my.mime_other = n_mime;
      }
      if (mime[n_mime].cat == MIME_HUH   && strcmp (mime[n_mime].ext, EXT_MYSTERY) == 0) {
         my.mime_guess = n_mime;
      }
      /*---(ready for next)--------------*/
      ++n_mime;
   }
   /*---(wrapup)-------------------------*/
   fclose (f_mime);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_subhead       (FILE *a_mime, char a_space, char a_cat)
{
   /*---(locals)-----------+-----------+-*/
   char        x_spacer1   [30] = "";
   char        x_spacer2   [30] = "";
   char        x_spacer3   [30] = "";
   /*---(prepare)------------------------*/
   if (a_space != ' ') {
      strcpy (x_spacer1, "============");
      strcpy (x_spacer3, "=======");
   } else {
      strcpy (x_spacer2, "  ");
   }
   /*---(major heading)------------------*/
   fprintf (a_mime, "\n\n");
   switch (a_cat) {
   case MIME_AUDIO  : fprintf (a_mime, "#=== audio formats ===============");  break;
   case MIME_VIDEO  : fprintf (a_mime, "#=== video formats ===============");  break;
   case MIME_IMAGE  : fprintf (a_mime, "#=== image formats ===============");  break;
   case MIME_CODE   : fprintf (a_mime, "#=== source code formats =========");  break;
   case MIME_ASCII  : fprintf (a_mime, "#=== text/doc formats ============");  break;
   case MIME_DBASE  : fprintf (a_mime, "#=== database formats ============");  break;
   case MIME_CRYPT  : fprintf (a_mime, "#=== compress/encrypted formats ==");  break;
   case MIME_PROP   : fprintf (a_mime, "#=== proprietary formats =========");  break;
   case MIME_EXEC   : fprintf (a_mime, "#=== executable formats ==========");  break;
   case MIME_TEMP   : fprintf (a_mime, "#=== transient formats ===========");  break;
   case MIME_DIR    : fprintf (a_mime, "#=== directories =================");  break;
   case MIME_OTHER  : fprintf (a_mime, "#=== other/misc formats ==========");  break;
   case MIME_HUH    : fprintf (a_mime, "#=== unknown formats =============");  break;
   case '-'         : fprintf (a_mime, "#=== GRAND TOTALS ================");  break;
   case '#'         : fprintf (a_mime, "#=== END-OF-REPORT ===============");  break;
   }
   /*---(rest of heading)----------------*/
   fprintf (a_mime, "========================================================================================================================%s#", x_spacer1);
   fprintf (a_mime, "      %s#===========================================%s#\n", x_spacer2, x_spacer3);
   /*---(sub totals)---------------------*/
   if (a_cat != '#') {
      fprintf (a_mime, "#-ext----- %c t %c ---desc--------------------------------- %c l %c ", a_space, a_space, a_space, a_space);
      fprintf (a_mime, "seen------ %c pct %c seen-bytes-------- %c pct %c "                  , a_space, a_space, a_space, a_space);
      fprintf (a_mime, "kept------ %c pct %c kept-bytes-------- %c pct %c "                  , a_space, a_space, a_space, a_space);
      fprintf (a_mime, "     %c found----- %c pct %c found-bytes------- %c pct %c"           , a_space, a_space, a_space, a_space, a_space);
      fprintf (a_mime, "\n\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}

/*---(mime grand totals)--------------*/
static long t_nseen     = 0;
static long t_bseen     = 0;
static long t_nkept     = 0;
static long t_bkept     = 0;
static long t_nfound    = 0;
static long t_bfound    = 0;

/*---(mime sub totals)----------------*/
static long s_nseen     = 0;
static long s_bseen     = 0;
static long s_nkept     = 0;
static long s_bkept     = 0;
static long s_nfound    = 0;
static long s_bfound    = 0;

char
MIME_subfoot       (FILE *a_mime, char a_space)
{
   /*---(locals)-----------+-----------+-*/
   char        x_comma     [20];
   char        x_percent   [20];
   /*---(lead)---------------------------*/
   fprintf (a_mime, "   total   %c   %c                                          %c   %c ",
         a_space, a_space, a_space, a_space);
   /*---(seen part)----------------------*/
   FILE_commas   (s_nseen                    , x_comma  );
   FILE_percents (s_nseen         , t_nseen  , x_percent);
   fprintf (a_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
   FILE_commas   (s_bseen                    , x_comma  );
   FILE_percents (s_bseen         , t_bseen  , x_percent);
   fprintf (a_mime, "%18.18s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
   /*---(kept part)----------------------*/
   FILE_commas   (s_nkept                    , x_comma  );
   FILE_percents (s_nkept         , t_nkept  , x_percent);
   fprintf (a_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
   FILE_commas   (s_bkept                    , x_comma  );
   FILE_percents (s_bkept         , t_bkept  , x_percent);
   fprintf (a_mime, "%18.18s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
   /*---(found lead)---------------------*/
   fprintf (a_mime, "     %c "               , a_space);
   /*---(found part)---------------------*/
   FILE_commas   (s_nfound, x_comma);
   FILE_percents (s_nfound, t_nfound , x_percent);
   fprintf (a_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
   FILE_commas   (s_bfound, x_comma);
   FILE_percents (s_bfound, t_bfound , x_percent);
   fprintf (a_mime, "%18.18s %c %3.3s %c\n"  , x_comma, a_space, x_percent, a_space);
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME_write         (
      /*---(formal parameters)+-------------+---------------------------------*/
      char        a_dest      ,             /* destination type               */
      char        a_space     )             /* seperator                      */
{  /*---(locals)-----------+-----------+-*/
   FILE       *f_mime      = NULL;          /* file pointer                   */
   char        rce         =  -10;
   int         i           =    0;
   char        x_comma     [20];
   char        x_percent   [20];
   char        x_save      =  ' ';
   int         x_count     =    0;
   /*---(grand totals)-------------------*/
   t_nseen     = mime [0].seen;
   t_bseen     = mime [0].sbytes; 
   t_nkept     = mime [0].kept;
   t_bkept     = mime [0].kbytes; 
   t_nfound    = mime [0].found;
   t_bfound    = mime [0].fbytes; 
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   DEBUG_CONF   yLOG_char    ("a_dest"    , a_dest);
   DEBUG_CONF   yLOG_char    ("a_space"   , a_space);
   /*---(open file)----------------------*/
   if (a_dest == 's') {
      f_mime = stdout;
      fprintf (f_mime, "HELIOS-PHAETON (locate) -- filesystem searching and indexing services\n");
      fprintf (f_mime, "report : mime type summary\n");
   }
   else {
      /*---(open file)----------------------*/
      f_mime = fopen (FILE_MIME, "w");
      DEBUG_CONF   yLOG_point   ("f_mime"    , f_mime);
      DEBUG_CONF   yLOG_point   ("file"      , f_mime);
      --rce;  if (f_mime == NULL) {
         DEBUG_CONF   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_CONF   yLOG_note    ("print header");
      fprintf (f_mime, "#!/usr/local/bin/helios\n");
      fprintf (f_mime, "#   recognized mime types for use in helios\n");
   }
   DEBUG_CONF   yLOG_point   ("f_mime"    , f_mime);
   /*---(process all)--------------------*/
   for (i = 0; i < n_mime; ++i) {
      /*---(formatting)------------------*/
      if ((x_count %  5) == 0 ) fprintf (f_mime, "\n");
      if (mime [i].cat != x_save) {
         if ((x_count %  5) != 0 ) fprintf (f_mime, "\n");
         if (x_count != 0 && x_save != '-') {
            MIME_subfoot (f_mime, a_space);
         }
         x_count  = 0;
         s_nseen  = 0;
         s_bseen  = 0;
         s_nkept  = 0;
         s_bkept  = 0;
         s_nfound = 0;
         s_bfound = 0;
         MIME_subhead (f_mime, a_space, mime [i].cat);
      }
      ++x_count;
      /*---(general part)----------------*/
      fprintf (f_mime, "%-10.10s %c %c %c %-40.40s %c %c %c ",
            mime [i].ext     , a_space, mime [i].cat     , a_space,
            mime [i].desc    , a_space, mime [i].like    , a_space);
      FILE_commas   (mime [i].seen  , x_comma  );
      FILE_percents (mime [i].seen  , t_nseen  , x_percent);
      fprintf (f_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (mime [i].sbytes, x_comma  );
      FILE_percents (mime [i].sbytes, t_bseen  , x_percent);
      fprintf (f_mime, "%18.18s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (mime [i].kept  , x_comma  );
      FILE_percents (mime [i].kept  , t_nkept  , x_percent);
      fprintf (f_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (mime [i].kbytes, x_comma  );
      FILE_percents (mime [i].kbytes, t_bkept  , x_percent);
      fprintf (f_mime, "%18.18s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (mime [i].found , x_comma);
      FILE_percents (mime [i].found , t_nfound , x_percent);
      /*---(found part)------------------*/
      fprintf (f_mime, "     %c "               , a_space);
      fprintf (f_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (mime [i].fbytes, x_comma );
      FILE_percents (mime [i].fbytes, t_bfound, x_percent);
      fprintf (f_mime, "%18.18s %c %3.3s %c\n"  , x_comma, a_space, x_percent, a_space);
      /*---(grand totals)----------------*/
      s_nseen  += mime [i].seen;
      s_nkept  += mime [i].kept;
      s_nfound += mime [i].found;
      s_bseen  += mime [i].sbytes;
      s_bkept  += mime [i].kbytes;
      s_bfound += mime [i].fbytes;
      x_save    = mime [i].cat;
      /*---(done)------------------------*/
   }
   /*---(wrapup)-------------------------*/
   fprintf (f_mime, "\n");
   MIME_subfoot (f_mime, a_space);
   MIME_subhead (f_mime, a_space, '#');
   fclose (f_mime);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_treehead      (void)
{
   printf ("\n#---name------------------------------------------  --------size---  -------count---  ---desc------------------------------------------- \n");
   return 0;
}

char
MIME_treecat       (char a_cat)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   llong       x_total     =    0;
   llong       x_count     =    0;
   char        x_title     [500]       = "";
   char        x_cat       [500]       = "n/a";
   int         c           =    0;
   /*---(summary run)--------------------*/
   /*> printf ("\n\nrunning cat %c\n", a_cat);                                        <*/
   for (i = 0; i < n_mime; ++i) {
      if (mime [i].cat != a_cat)  continue;
      /*> printf ("      %-44.44s | %15lld\n", mime[i].desc, mime [i].sbytes);        <*/
      x_count += mime[i].seen;
      x_total += mime[i].sbytes;
   }
   for (i = 0; i < MAX_CAT; ++i) {
      if (s_cats [i].cat == ' '  ) break;
      if (s_cats [i].cat != a_cat) continue;
      strcpy (x_cat, s_cats [i].desc);
   }
   MIME_treehead ();
   printf ("   %-44.44s     %15lld  %15lld  %-50.50s \n", x_cat, x_total, x_count, x_cat);
   /*---(detail run)---------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (mime [i].cat != a_cat)  continue;
      if ((c % 5) == 0) printf ("\n");
      sprintf (x_title, "%s, %s", mime [i].ext, mime [i].desc);
      printf  ("      %-44.44s  %15lld  %15lld  %-50.50s \n", mime [i].ext, mime [i].sbytes, mime [i].seen, x_title); 
      ++c;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME_tree          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        x_save      = mime [i].cat;
   /*---(header)-------------------------*/
   printf ("#!/usr/local/bin/hyleoroi\n");
   printf ("#   hyleoroi -- tree visualization input file\n");
   printf ("#   mime-type summary written by helios\n");
   printf ("\n\n\n");
   printf ("SIMPLE-FORMAT\n");
   printf ("\n\n\n");
   printf ("#--context  ---values------------------------------- \n");
   printf ("source      helios-phaeton                           \n");
   printf ("label       total file size by mime category         \n");
   printf ("format      mimecat                                  \n");
   printf ("\n\n\n");
   /*---(root)---------------------------*/
   MIME_treehead ();
   printf ("%-44.44s        %15lld  %15lld  %-50.50s \n"   , h_drive->device, h_drive->size, mime [0].seen, h_drive->device);
   /*---(process all)--------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (mime [i].cat == x_save)  continue;
      x_save = mime [i].cat;
      MIME_treecat (x_save);
   }
   /*---(footer)-------------------------*/
   printf ("   %-44.44s     %15lld  %15lld  %-50.50s \n", "(empty)", h_drive->size - mime [0].sbytes, 0, "(empty)");
   MIME_treehead ();
   printf ("# END-OF-FILE\n");
   /*---(complete)-----------------------*/
   return 0;
}

char         /*===[[ create entry statistics ]]===========[ ------ [ ------ ]=*/
MIME_find_cat           (cchar *a_ext, int *a_index, char *a_cat, long a_bytes)
{  /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          = -1;            /* return code                    */
   int         x_len       =  0;            /* length of extension            */
   char        x_ext       [LEN_TERSE] = "";
   int         i           =  0;            /* generic iterator               */
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_index != NULL)  *a_index = 0;
   if (a_cat   != NULL)  *a_cat   = MIME_HUH;
   /*---(defence)------------------------*/
   x_len = strlen  (a_ext);
   DEBUG_CONF   yLOG_complex ("ext"       , "%d, %s", x_len, a_ext);
   --rce;  if (x_len <= 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len >=  LEN_TERSE) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strcpy (x_ext, a_ext);
   for (i = 0; i < x_len; ++i)   x_ext [i] = tolower (x_ext [i]);
   /*---(search for matche)--------------*/
   for (i = 0; i < n_mime; ++i) {
      /*---(filter)----------------------*/
      if (mime [i].ext  [0] != x_ext [0])      continue;
      if (mime [i].ext  [1] != x_ext [1])      continue;
      if (strcmp (mime [i].ext , x_ext) != 0)  continue;
      /*---(update)----------------------*/
      if (a_index != NULL)  *a_index = i;
      if (a_cat   != NULL)  *a_cat   = mime [i].cat;
      ++(mime [i].seen);
      mime [i].sbytes += a_bytes;
      /*---(get out)---------------------*/
      rc = 0;
      break;
   }
   DEBUG_CONF   yLOG_value   ("rc"        , rc);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
MIME__clearall     (void)
{  /*---(locals)-----------+-----+-----+-*/
   int         i           =  0;
   for (i = 0; i < n_mime; ++i) {
      mime [i].seen    = 0;
      mime [i].sbytes  = 0;
      mime [i].kept    = 0;
      mime [i].kbytes  = 0;
      mime [i].found   = 0;
      mime [i].fbytes  = 0;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   run-time configuration                     ----===*/
/*====================------------------------------------====================*/
static void  o___CONFIG__________o () { return; }

static uchar  s_verbs     [LEN_RECD] = " dir_silent dir_pass dir_last dir_never mountpoint ";
static uchar  s_field     [ 20][LEN_HUND];
static uchar  s_nfield    = 0;

char         /*===[[ read configuration ]]================[ ------ [ ------ ]=*/
CONF__parse             (cchar *a_recd)
{  /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p;
   char       *q           = "\x1F";
   char       *r           = NULL;
   char       *u           = NULL;
   uchar       x_recd      [LEN_RECD]  = "";
   uchar       x_verb      [LEN_RECD]  = "";
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_nfield = 0;
   for (i = 0; i < 20; ++i) {
      strncpy (s_field [i], "", LEN_HUND);
   }
   /*---(defense)------------------------*/
   DEBUG_CONF    yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_recd, a_recd, LEN_RECD);
   x_len = strlen (x_recd);
   DEBUG_CONF    yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 25) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
   --rce;  if (x_recd [0] <= ' ' || x_recd [0] >= '~') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_recd [0] == '#' || x_recd [0] == ' ') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get fields)------------------*/
   p = strtok_r (x_recd, q, &r);
   for (i = 0; i < 20; ++i) {
      if (p == NULL)  break;
      ySTR_trim (p, ySTR_BOTH);
      strncpy (s_field [i], p, LEN_HUND);
      DEBUG_CONF   yLOG_complex ("field"     , "%2d, %2d[%s]", i, strlen (s_field [i]), s_field [i]);
      ++s_nfield;
      p = strtok_r (NULL  , q, &r);
   }
   DEBUG_CONF   yLOG_value   ("s_nfield"  , s_nfield);
   /*---(filter recd types)-----------*/
   sprintf (x_verb, " %s ", s_field [0]);
   DEBUG_CONF   yLOG_info    ("x_verb"    , x_verb);
   u = strstr (s_verbs, x_verb);
   DEBUG_CONF    yLOG_point   ("u"         , u);
   --rce;  if (u == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CONF__handle            (void)
{  /*---(locals)-----------+-----------+-*/
   FILE       *f_conf      = NULL;
   char        rc          = 0;
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_CONF    yLOG_enter   (__FUNCTION__);
   DEBUG_CONF    yLOG_info    ("s_field 0" , s_field [0]);
   /*---(directory types)----------------*/
   if (s_field [0][0] == 'd') {
      switch (s_field [0][4]) {
      case 's' : g_nodir [n_nodir].type = DIR_SILENT;
                 DEBUG_CONF   yLOG_note    ("adding a silent type");
                 break;
      case 'p' : g_nodir [n_nodir].type = DIR_PASS;
                 DEBUG_CONF   yLOG_note    ("adding a pass type");
                 break;
      case 'l' : g_nodir [n_nodir].type = DIR_LAST;
                 DEBUG_CONF   yLOG_note    ("adding a last type");
                 break;
      case 'n' : g_nodir [n_nodir].type = DIR_NEVER;
                 DEBUG_CONF   yLOG_note    ("adding a never type");
                 break;
      default  : /* unknown verb */
                 DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
                 return rce;
      }
   }
   /*---(mountpoint)---------------------*/
   else if (strcmp (s_field [0], "mountpoint") == 0) {
      DEBUG_CONF   yLOG_note    ("mountpoint default found in conf");
      if (my.mpoint [0] != '\0') {
         DEBUG_CONF   yLOG_note    ("but, mountpoint already set by arg");
      } else {
         DEBUG_CONF   yLOG_note    ("mpoint currently null");
         strcpy (my.mpoint, s_field [1]);
      }
      DEBUG_CONF   yLOG_info    ("mpoint"    , my.mpoint);
   }
   /*---(common)-------------------------*/
   strncpy (g_nodir [n_nodir].name, s_field [1], MAX_NAME);
   g_nodir [n_nodir].len = strlen (s_field [1]);
   ++n_nodir;
   DEBUG_CONF   yLOG_value   ("n_nodir"   , n_nodir);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ read configuration ]]================[ ------ [ ------ ]=*/
CONF_read          (void)
{  /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   FILE       *f_conf      = NULL;
   char        x_recd      [MAX_RECD];           /* input record              */
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(check for option)---------------*/
   if (my.conf != 'y') {
      DEBUG_CONF   yLOG_note    ("elected not to process configuration file");
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open configuration)-------------*/
   DEBUG_CONF   yLOG_info    ("conf_file" , my.conf_file);
   f_conf = fopen (my.conf_file, "r");
   DEBUG_CONF   yLOG_point   ("file point", f_conf);
   --rce;  if (f_conf == NULL) {
      printf ("fatal, helios configuration file %s could not be openned\n", my.conf_file);
      DEBUG_CONF   yLOG_note    ("failed to open file");
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CONF   yLOG_note    ("successfully openned");
   /*---(read)---------------------------*/
   while (1) {
      /*---(get line)--------------------*/
      fgets (x_recd, MAX_RECD, f_conf);
      if (feof(f_conf))                     break;
      /*---(parse)-----------------------*/
      rc = CONF__parse (x_recd);
      DEBUG_CONF   yLOG_value   ("parse"     , rc);
      if (rc < 0)                           continue;
      /*---(act)-------------------------*/
      rc = CONF__handle ();
      DEBUG_CONF   yLOG_value   ("handle"    , rc);
      /*---(done)------------------------*/
   }
   /*---(close configuration)------------*/
   DEBUG_CONF   yLOG_note    ("closing configuration file");
   rc = fclose (f_conf);
   DEBUG_CONF   yLOG_value   ("close rc"  , rc);
   --rce;  if (rc != 0) {
      DEBUG_CONF   yLOG_note    ("failed to close file");
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CONF   yLOG_note    ("successfully closed configuration file");
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CONF_find               (char *a_full, char *a_name, char *a_stype, char *a_silent)
{
   /*---(defense)------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   int         x_len2      =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   DEBUG_CONF   yLOG_complex ("args"      , "%p, %p, %p", a_full, a_stype, a_silent);
   /*---(defense)------------------------*/
   DEBUG_CONF    yLOG_point   ("a_full"    , a_full);
   --rce;  if (a_full == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len  = strlen (a_full);
   DEBUG_CONF    yLOG_complex ("full"      , "%2d[%s]", x_len , a_full);
   DEBUG_CONF    yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len2 = strlen (a_name);
   DEBUG_CONF    yLOG_complex ("name"      , "%2d[%s]", x_len2, a_name);
   DEBUG_CONF    yLOG_point   ("a_stype"   , a_stype);
   --rce;  if (a_stype == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF    yLOG_char    ("*a_stype"  , *a_stype);
   DEBUG_CONF    yLOG_point   ("a_silent"  , a_silent);
   --rce;  if (a_silent == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF    yLOG_char    ("*a_silent" , *a_silent);
   /*---(walk entries)-------------------*/
   DEBUG_CONF   yLOG_value   ("n_nodir"   , n_nodir);
   --rce;  for (i = 0; i < n_nodir; ++i) {
      /*---(filter)----------------------*/
      DEBUG_CONF   yLOG_info    ("dir"       , g_nodir [i].name);
      /*---(name-only options)-----------*/
      if (g_nodir [i].len == x_len2 && strcmp (g_nodir [i].name, a_name) == 0) {
         /*---(never option)-------------*/
         if (g_nodir [i].type == DIR_NEVER) {
            DEBUG_CONF    yLOG_note    ("found a dir_never rule");
            rc = 1;
            break;
         }
      }
      /*---(full-path options)-----------*/
      if (g_nodir [i].len == x_len  && strcmp (g_nodir [i].name, a_full) == 0) {
         /*---(silent)-------------------*/
         if (g_nodir [i].type == DIR_SILENT) {
            DEBUG_CONF    yLOG_note    ("found a dir_silent rule");
            *a_silent = 'y';
            rc = 1;
            break;
         }
         /*---(ignore)-------------------*/
         if (g_nodir [i].type == DIR_PASS) {
            DEBUG_CONF    yLOG_note    ("found a dir_past rule");
            rc = 1;
            break;
         }
         /*---(last)---------------------*/
         if (g_nodir [i].type == DIR_LAST) {
            DEBUG_CONF    yLOG_note    ("found a dir_last rule");
            rc = 1;
            break;
         }
      }
   }
   /*---(update)-------------------------*/
   if (rc != 0) {
      *a_stype = g_nodir [i].type;
      DEBUG_CONF    yLOG_char    ("*a_stype"  , *a_stype);
      DEBUG_CONF    yLOG_char    ("*a_silent" , *a_silent);
   } else {
      DEBUG_CONF    yLOG_note    ("not matching rules found");
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
MIME__unit              (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   tMIME      *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME count       : num=%4d", n_mime);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      if (n >= n_mime) 
         snprintf (unit_answer, LEN_FULL, "MIME entry (%3d) : no entry", n);
      else 
         x_curr = &(mime [n]);
      snprintf (unit_answer, LEN_FULL, "MIME entry (%3d) : %c %-7.7s %-10.10s  %c  %6.6lds %6.6ldk %6.6ldf", n,
            x_curr->cat  , x_curr->ext , x_curr->desc, x_curr->like ,
            x_curr->seen , x_curr->kept, x_curr->found);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
CONF__unit              (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   tMIME      *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "field"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "CONF field  (%2d) : %2d[%s]", n,
            strlen (s_field [n]), s_field [n]);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      if (n >= n_nodir) {
         snprintf (unit_answer, LEN_FULL, "CONF entry  (%2d) : no entry", n);
      } else {
         snprintf (unit_answer, LEN_FULL, "CONF entry  (%2d) : %c %2d[%.50s]", n,
               g_nodir [n].type, g_nodir [n].len,  g_nodir [n].name);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}





/*===============================[[ end-code ]]===============================*/
