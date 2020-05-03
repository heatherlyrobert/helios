/*===============================[[ beg-code ]]===============================*/
#include    "helios.h"       /* LOCAL  : main header                          */


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



char
MIME_init          (void)
{
   n_mime             = 0;
   MIME_read ();
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
      strcpy (g_mime [n_mime].ext , p);
      DEBUG_CONF  yLOG_info    ("ext"       , g_mime [n_mime].ext);
      /*---(category)--------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      g_mime [n_mime].cat = p [0];
      DEBUG_CONF  yLOG_char    ("cat"       , g_mime [n_mime].cat);
      /*---(desc)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      strcpy (g_mime [n_mime].desc , p);
      DEBUG_CONF  yLOG_info    ("desc"      , g_mime [n_mime].desc);
      /*---(like)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      g_mime [n_mime].like = p [0];
      DEBUG_CONF  yLOG_char    ("like"      , g_mime [n_mime].like);
      /*---(seen)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(g_mime [n_mime].seen));
      DEBUG_CONF  yLOG_value   ("seen"      , g_mime [n_mime].seen);
      /*---(seen percent)----------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(seen bytes)------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(g_mime [n_mime].sbytes));
      DEBUG_CONF  yLOG_value   ("sbytes"    , g_mime [n_mime].sbytes);
      /*---(seen bytes percent)----------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(kept)------------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(g_mime [n_mime].kept));
      DEBUG_CONF  yLOG_value   ("kept"      , g_mime [n_mime].kept);
      /*---(kept percent)----------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(kept bytes)------------------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      ySTR_trim (p, ySTR_BOTH);
      FILE_uncommas (p, &(g_mime [n_mime].kbytes));
      DEBUG_CONF  yLOG_value   ("kbytes"    , g_mime [n_mime].kbytes);
      /*---(kept bytes percent)----------*/
      p = strtok_r (NULL  , q, &s);
      if (p == NULL)              continue;
      /*---(found)-----------------------*/
      g_mime [n_mime].found  = 0;
      g_mime [n_mime].fbytes = 0;
      /*---(save)------------------------*/
      if (g_mime[n_mime].cat == MIME_DIR   && strcmp (g_mime[n_mime].ext, EXT_DLINK  ) == 0) {
         my.mime_link  = n_mime;
      }
      if (g_mime[n_mime].cat == MIME_TEMP  && strcmp (g_mime[n_mime].ext, EXT_RLINK  ) == 0) {
         my.mime_link  = n_mime;
      }
      if (g_mime[n_mime].cat == MIME_EXEC  && strcmp (g_mime[n_mime].ext, EXT_EXEC   ) == 0) {
         my.mime_exe   = n_mime;
      }
      if (g_mime[n_mime].cat == MIME_ASCII && strcmp (g_mime[n_mime].ext, EXT_CONF   ) == 0) {
         my.mime_conf  = n_mime;
      }
      if (g_mime[n_mime].cat == MIME_HUH   && strcmp (g_mime[n_mime].ext, EXT_UNKNOWN) == 0) {
         my.mime_other = n_mime;
      }
      if (g_mime[n_mime].cat == MIME_HUH   && strcmp (g_mime[n_mime].ext, EXT_MYSTERY) == 0) {
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
   t_nseen     = g_mime [0].seen;
   t_bseen     = g_mime [0].sbytes; 
   t_nkept     = g_mime [0].kept;
   t_bkept     = g_mime [0].kbytes; 
   t_nfound    = g_mime [0].found;
   t_bfound    = g_mime [0].fbytes; 
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
      if (g_mime [i].cat != x_save) {
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
         MIME_subhead (f_mime, a_space, g_mime [i].cat);
      }
      ++x_count;
      /*---(general part)----------------*/
      fprintf (f_mime, "%-10.10s %c %c %c %-40.40s %c %c %c ",
            g_mime [i].ext     , a_space, g_mime [i].cat     , a_space,
            g_mime [i].desc    , a_space, g_mime [i].like    , a_space);
      FILE_commas   (g_mime [i].seen  , x_comma  );
      FILE_percents (g_mime [i].seen  , t_nseen  , x_percent);
      fprintf (f_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (g_mime [i].sbytes, x_comma  );
      FILE_percents (g_mime [i].sbytes, t_bseen  , x_percent);
      fprintf (f_mime, "%18.18s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (g_mime [i].kept  , x_comma  );
      FILE_percents (g_mime [i].kept  , t_nkept  , x_percent);
      fprintf (f_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (g_mime [i].kbytes, x_comma  );
      FILE_percents (g_mime [i].kbytes, t_bkept  , x_percent);
      fprintf (f_mime, "%18.18s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (g_mime [i].found , x_comma);
      FILE_percents (g_mime [i].found , t_nfound , x_percent);
      /*---(found part)------------------*/
      fprintf (f_mime, "     %c "               , a_space);
      fprintf (f_mime, "%10.10s %c %3.3s %c "   , x_comma, a_space, x_percent, a_space);
      FILE_commas   (g_mime [i].fbytes, x_comma );
      FILE_percents (g_mime [i].fbytes, t_bfound, x_percent);
      fprintf (f_mime, "%18.18s %c %3.3s %c\n"  , x_comma, a_space, x_percent, a_space);
      /*---(grand totals)----------------*/
      s_nseen  += g_mime [i].seen;
      s_nkept  += g_mime [i].kept;
      s_nfound += g_mime [i].found;
      s_bseen  += g_mime [i].sbytes;
      s_bkept  += g_mime [i].kbytes;
      s_bfound += g_mime [i].fbytes;
      x_save    = g_mime [i].cat;
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
      if (g_mime [i].cat != a_cat)  continue;
      /*> printf ("      %-44.44s | %15lld\n", g_mime[i].desc, g_mime [i].sbytes);        <*/
      x_count += g_mime[i].seen;
      x_total += g_mime[i].sbytes;
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
      if (g_mime [i].cat != a_cat)  continue;
      if ((c % 5) == 0) printf ("\n");
      sprintf (x_title, "%s, %s", g_mime [i].ext, g_mime [i].desc);
      printf  ("      %-44.44s  %15lld  %15lld  %-50.50s \n", g_mime [i].ext, g_mime [i].sbytes, g_mime [i].seen, x_title); 
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
   char        x_save      = g_mime [i].cat;
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
   printf ("%-44.44s        %15lld  %15lld  %-50.50s \n"   , h_drive->device, h_drive->size, g_mime [0].seen, h_drive->device);
   /*---(process all)--------------------*/
   for (i = 0; i < n_mime; ++i) {
      if (g_mime [i].cat == x_save)  continue;
      x_save = g_mime [i].cat;
      MIME_treecat (x_save);
   }
   /*---(footer)-------------------------*/
   printf ("   %-44.44s     %15lld  %15lld  %-50.50s \n", "(empty)", h_drive->size - g_mime [0].sbytes, 0, "(empty)");
   MIME_treehead ();
   printf ("# END-OF-FILE\n");
   /*---(complete)-----------------------*/
   return 0;
}

char         /*===[[ create entry statistics ]]===========[ ------ [ ------ ]=*/
MIME_find_by_ext        (cchar *a_ext, int *a_index, char *a_cat, long a_bytes)
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
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_ext"     , a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
      if (g_mime [i].ext  [0] != x_ext [0])      continue;
      if (g_mime [i].ext  [1] != x_ext [1])      continue;
      if (strcmp (g_mime [i].ext , x_ext) != 0)  continue;
      /*---(update)----------------------*/
      if (a_index != NULL)  *a_index = i;
      if (a_cat   != NULL)  *a_cat   = g_mime [i].cat;
      /*---(update mime)-----------------*/
      if (a_bytes > 0) {
         /*---(entry)----------*/
         ++(g_mime [i].seen);
         g_mime [i].sbytes += a_bytes;
         /*---(totals)---------*/
         ++(g_mime [0].seen);
         g_mime [0].sbytes += a_bytes;
         /*---(counters)-------*/
         switch (g_mime [i].cat) {
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
         /*---(done)-----------*/
      }
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
MIME_reset_to_zeros     (void)
{
   int         i           =  0;
   for (i = 0; i < n_mime; ++i) {
      g_mime [i].seen    = 0;
      g_mime [i].sbytes  = 0;
      g_mime [i].kept    = 0;
      g_mime [i].kbytes  = 0;
      g_mime [i].found   = 0;
      g_mime [i].fbytes  = 0;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
MIME__unit              (char *a_question, char *a_ext, int n)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
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
         x_curr = &(g_mime [n]);
      snprintf (unit_answer, LEN_FULL, "MIME entry (%3d) : %c %-7.7s %-10.10s  %c  [%3d,%6lds] [%3d,%6ldk] [%3d,%6ldf]", n,
            x_curr->cat   , x_curr->ext , x_curr->desc, x_curr->like ,
            x_curr->seen  , x_curr->sbytes ,
            x_curr->kept  , x_curr->kbytes ,
            x_curr->found , x_curr->fbytes);
   }
   else if (strcmp (a_question, "ext"           ) == 0) {
      if (a_ext == NULL) {
         snprintf (unit_answer, LEN_FULL, "MIME ext   (---) : no ext given");
      } else {
         rc = MIME_find_by_ext (a_ext, &n, NULL, 0);
         if (rc < 0)  snprintf (unit_answer, LEN_FULL, "MIME entry (---) : - %-7.7s no entry found", a_ext);
         else {
            x_curr = &(g_mime [n]);
            snprintf (unit_answer, LEN_FULL, "MIME ext   (%3d) : %c %-7.7s %-10.10s  %c  [%3d,%6lds] [%3d,%6ldk] [%3d,%6ldf]", n,
                  x_curr->cat   , x_curr->ext , x_curr->desc, x_curr->like ,
                  x_curr->seen  , x_curr->sbytes ,
                  x_curr->kept  , x_curr->kbytes ,
                  x_curr->found , x_curr->fbytes);
         }
      }
   }
   else if (strcmp (a_question, "summary"       ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME summary    : %2da %2dv %2di %2ds %2dt %2db %2dc %2dp %2dx %2dd %2dz %2du %2d?",
            my.n_audio, my.n_video, my.n_image, my.n_code , my.n_ascii, 
            my.n_dbase, my.n_crypt, my.n_prop , my.n_exec , my.n_dir  , 
            my.n_temp , my.n_other, my.n_huh  );
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return unit_answer;
}


