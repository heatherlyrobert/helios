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

/*
 * METIS � wv4�� � create logic to verify mime file owner, group, and perms               � N9JLe3 �  � �
 * METIS � wv2�� � add logic to treat mime load like a pull request                       � N9JLeV �  � �
 * METIS � wg�#� � allow mime file name override for testing purposes                     � N9JLgC �  � �
 * METIS � wv�#� � merge cCAT and cMIME data structures (almost identical)                � N9M1Ij �  � �
 *
 */

typedef struct cLOAD tLOAD;
struct cLOAD {
   char        level;
   char        ext         [LEN_TERSE];
   char        cat;
   char        desc        [LEN_DESC];
   char        like;
};

static const tLOAD s_load [] = {
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'T', "TOTAL"     , 'T', "grand�total�values"                                , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "AUDIO"     , 'a', "auditory�formats�like�music"                       , '-' },
   { 'e', "aiff"      , 'a', "apple�standard�audio"                              , '-' },
   { 'e', "aac"       , 'a', "advanced�audio�coding"                             , '-' },
   { 'e', "au"        , 'a', "sun�standard�audio"                                , '-' },
   { 'e', "m4a"       , 'a', "apple�unprotected�coding"                          , '-' },
   { 'e', "mp3"       , 'a', "mpeg�layer�3�audio"                                , '@' },
   { 'e', "ogg"       , 'a', "open�source�vorbis"                                , '-' },
   { 'e', "ra"        , 'a', "realnetworks�real�audio"                           , '-' },
   { 'e', "raw"       , 'a', "raw�audio�for�testing"                             , '-' },
   { 'e', "wav"       , 'a', "microsoft�standard�audio"                          , '-' },
   { 'e', "wma"       , 'a', "microsoft�protected�audio"                         , '-' },
   { 'e', "mid"       , 'a', "midi�sound"                                        , '-' },
   { 'e', "midi"      , 'a', "midi�sound"                                        , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "VIDEO"     , 'v', "moving�visual�formats,�like�movies"                , '-' },
   { 'e', "flv"       , 'v', "adobe�flash�video"                                 , '-' },
   { 'e', "f4v"       , 'v', "adobe�flash�video�(updated)"                       , '-' },
   { 'e', "avi"       , 'v', "standard�video"                                    , '-' },
   { 'e', "mov"       , 'v', "quicktime�video"                                   , '-' },
   { 'e', "qt"        , 'v', "quicktime�video"                                   , '-' },
   { 'e', "wmv"       , 'v', "windows�media�video"                               , '-' },
   { 'e', "rm"        , 'v', "realnetworks�real�video"                           , '-' },
   { 'e', "mp4"       , 'v', "mpeg�layer�4�video"                                , '@' },
   { 'e', "m4p"       , 'v', "mpeg�layer�4�protected"                            , '-' },
   { 'e', "m4v"       , 'v', "mpeg�layer�4�video"                                , '-' },
   { 'e', "mpg"       , 'v', "mpeg�layer�1�video"                                , '-' },
   { 'e', "mpeg"      , 'v', "mpeg�layer�1�video"                                , '-' },
   { 'e', "mpe"       , 'v', "mpeg�layer�1�video"                                , '-' },
   { 'e', "mpv"       , 'v', "mpeg�layer�1�video"                                , '-' },
   { 'e', "swf"       , 'v', "flash�small�web"                                   , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "IMAGE"     , 'i', "still�visual�formats,�like�pictures"               , '-' },
   { 'e', "jpg"       , 'i', "jpeg�compressed�image"                             , '@' },
   { 'e', "jpeg"      , 'i', "jpeg�compressed�image"                             , '-' },
   { 'e', "tif"       , 'i', "tagged�image�file"                                 , '-' },
   { 'e', "tiff"      , 'i', "tagged�image�file"                                 , '-' },
   { 'e', "rif"       , 'i', "raw�image�format"                                  , '-' },
   { 'e', "gif"       , 'i', "graphics�interchange"                              , '-' },
   { 'e', "bmp"       , 'i', "microsoft�bitmap"                                  , '-' },
   { 'e', "png"       , 'i', "portable�net�graphics"                             , '@' },
   { 'e', "ppm"       , 'i', "portable�pixmap"                                   , '-' },
   { 'e', "pgm"       , 'i', "portable�pixmap"                                   , '-' },
   { 'e', "pbm"       , 'i', "portable�pixmap"                                   , '-' },
   { 'e', "pnm"       , 'i', "portable�pixmap"                                   , '-' },
   { 'e', "psd"       , 'i', "photoshop�drawing"                                 , '-' },
   { 'e', "xpm"       , 'i', "x-windows�pixmap"                                  , '-' },
   { 'e', "xbm"       , 'i', "x-windows�pixmap"                                  , '-' },
   { 'e', "xpm2"      , 'i', "x-windows�pixmap"                                  , '-' },
   { 'e', "xpm3"      , 'i', "x-windows�pixmap"                                  , '-' },
   { 'e', "svg"       , 'i', "xml�vector�graphics"                               , '-' },
   { 'e', "svgz"      , 'i', "xml�vector�compressed"                             , '-' },
   { 'e', "svgb"      , 'i', "xml�vector�binary"                                 , '-' },
   { 'e', "dwg"       , 'i', "autocad�drawing�file"                              , '-' },
   { 'e', "dxf"       , 'i', "autocad�drawing�exchange"                          , '-' },
   { 'e', "xcf"       , 'i', "gimp�drawing�file"                                 , '-' },
   { 'e', "tga"       , 'i', "targa�bitmap�file"                                 , '-' },
   { 'e', "pic"       , 'i', "picture�file"                                      , '-' },
   { 'e', "rgb"       , 'i', "silicongraphics�bitmap"                            , '-' },
   { 'e', "rgba"      , 'i', "silicongraphics�image"                             , '-' },
   { 'e', "sgi"       , 'i', "silicongraphics�bitmap"                            , '-' },
   { 'e', "aae"       , 'i', "apple�metadata�and�adjustment�file"                , '-' },
   { 'e', "ithmb"     , 'i', "apple�thumbnail�image�file"                        , '-' },
   { 'e', "bdf"       , 'i', "font�file�in�bit"                                  , '-' },
   { 'e', "sfd"       , 'i', "font�file�in�fontforge"                            , '-' },
   { 'e', "pcf"       , 'i', "font�file�in�pixel"                                , '-' },
   { 'e', "i_scan"    , 'i', "scan�and�document�archive"                         , '-' },
   { 'e', "i_wall"    , 'i', "exoteric�roach�wallpaper"                          , '-' },
   { 'e', "i_photo"   , 'i', "photo�from�phone�or�camera"                        , '-' },
   { 'e', "psf"       , 'i', "pc�screen�font�(normal)"                           , '-' },
   { 'e', "psfu"      , 'i', "pc�screen�font�(unicode)"                          , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "SOURCE"    , 's', "computer�language�files�(not�config)"              , '-' },
   { 'e', "ada"       , 's', "ada�source�code"                                   , '-' },
   { 'e', "asm"       , 's', "assembly�language�source�code"                     , '@' },
   { 'e', "s"         , 's', "assembly�language�source�code"                     , '-' },
   { 'e', "bas"       , 's', "basic�language�source�code"                        , '-' },
   { 'e', "bat"       , 's', "windows�batch�file"                                , '-' },
   { 'e', "vb"        , 's', "visual�basic�language�source�code"                 , '-' },
   { 'e', "vbs"       , 's', "visual�basic�language�source�code"                 , '-' },
   { 'e', "cls"       , 's', "visual�basic�language�source�code"                 , '-' },
   { 'e', "cob"       , 's', "cobol�language�source�code"                        , '-' },
   { 'e', "cbl"       , 's', "cobol�language�source�code"                        , '-' },
   { 'e', "cpp"       , 's', "c++�language�source�code"                          , '-' },
   { 'e', "c++"       , 's', "c++�language�source�code"                          , '-' },
   { 'e', "cc"        , 's', "c++�language�source�code"                          , '-' },
   { 'e', "cxx"       , 's', "c++�language�source�code"                          , '-' },
   { 'e', "cbp"       , 's', "c++�language�source�code"                          , '-' },
   { 'e', "hpp"       , 's', "c++�language�header�code"                          , '-' },
   { 'e', "hxx"       , 's', "c++�language�header�code"                          , '-' },
   { 'e', "h++"       , 's', "c++�language�header�code"                          , '-' },
   { 'e', "hh"        , 's', "c++�language�header�code"                          , '-' },
   { 'e', "c"         , 's', "ansi-c�source�code"                                , '@' },
   { 'e', "h"         , 's', "ansi-c�header"                                     , '@' },
   { 'e', "for"       , 's', "fortran�language�source�code"                      , '-' },
   { 'e', "ftn"       , 's', "fortran�language�source�code"                      , '-' },
   { 'e', "f"         , 's', "fortran�language�source�code"                      , '-' },
   { 'e', "f77"       , 's', "fortran�language�source�code"                      , '-' },
   { 'e', "f90"       , 's', "fortran�language�source�code"                      , '-' },
   { 'e', "java"      , 's', "java�language�source�code"                         , '-' },
   { 'e', "class"     , 's', "java�language�source�code"                         , '-' },
   { 'e', "jsm"       , 's', "javascript�code�module"                            , '-' },
   { 'e', "lisp"      , 's', "lisp�language�source�code"                         , '-' },
   { 'e', "m4"        , 's', "m4�language�source�code"                           , '-' },
   { 'e', "pas"       , 's', "pascal�language�source�code"                       , '-' },
   { 'e', "p"         , 's', "pascal�language,�python�pickle"                    , '-' },
   { 'e', "pp"        , 's', "pascal�language�source�code"                       , '-' },
   { 'e', "php"       , 's', "php�language�source�code"                          , '-' },
   { 'e', "php3"      , 's', "php�language�source�code"                          , '-' },
   { 'e', "php4"      , 's', "php�language�source�code"                          , '-' },
   { 'e', "php5"      , 's', "php�language�source�code"                          , '-' },
   { 'e', "phps"      , 's', "php�language�source�code"                          , '-' },
   { 'e', "pl"        , 's', "perl�language�source�code"                         , '-' },
   { 'e', "ph"        , 's', "perl�language�header�file"                         , '-' },
   { 'e', "pm"        , 's', "perl�language�source�code"                         , '-' },
   { 'e', "py"        , 's', "python�language�source�code"                       , '-' },
   { 'e', "rpy"       , 's', "python�language�source�code"                       , '-' },
   { 'e', "ptl"       , 's', "python�template�language�file"                     , '-' },
   { 'e', "pyt"       , 's', "python�declaration�file"                           , '-' },
   { 'e', "pyw"       , 's', "python�gui�source�script"                          , '-' },
   { 'e', "rb"        , 's', "ruby�language�source�code"                         , '-' },
   { 'e', "rbx"       , 's', "ruby�language�source�code"                         , '-' },
   { 'e', "rjs"       , 's', "ruby�language�source�code"                         , '-' },
   { 'e', "scm"       , 's', "scheme�language�source�code"                       , '-' },
   { 'e', "sh"        , 's', "bash�shell�language�source�code"                   , '@' },
   { 'e', "shh"       , 's', "bash�shell�language�source�code"                   , '-' },
   { 'e', "ws"        , 's', "windows�scripting�language"                        , '-' },
   { 'e', "wsf"       , 's', "windows�scripting,�studio�script"                  , '-' },
   { 'e', "bash"      , 's', "bash�shell�language�source�code"                   , '-' },
   { 'e', "vim"       , 's', "vim�script"                                        , '-' },
   { 'e', "awk"       , 's', "awk�script�file"                                   , '-' },
   { 'e', "in"        , 's', "automake/autoconf�file"                            , '-' },
   { 'e', "am"        , 's', "automake/autoconf�definition"                      , '-' },
   { 'e', "vcproj"    , 's', "microsoft�visual�project"                          , '-' },
   { 'e', "csproj"    , 's', "microsoft�visual�project"                          , '-' },
   { 'e', "vcp"       , 's', "microsoft�visual�project"                          , '-' },
   { 'e', "dsp"       , 's', "microsoft�developer�project"                       , '-' },
   { 'e', "dsw"       , 's', "microsoft�developer�workspace"                     , '-' },
   { 'e', "js"        , 's', "microsoft�studio�javascript"                       , '-' },
   { 'e', "cd"        , 's', "microsoft�studio�class�diagram"                    , '-' },
   { 'e', "config"    , 's', "microsoft�studio�configuration"                    , '-' },
   { 'e', "jsl"       , 's', "microsoft�studio�javascript�assembly"              , '-' },
   { 'e', "resx"      , 's', "microsoft�studio�resources�file"                   , '-' },
   { 'e', "res"       , 's', "microsoft�studio�resources�compiled"               , '-' },
   { 'e', "ico"       , 's', "microsoft�studio�icon�file"                        , '-' },
   { 'e', "cur"       , 's', "microsoft�studio�cursor�file"                      , '-' },
   { 'e', "rpt"       , 's', "microsoft�studio�crystal�report"                   , '-' },
   { 'e', "mdf"       , 's', "microsoft�studio�sql�database"                     , '-' },
   { 'e', "lua"       , 's', "lua�script�file"                                   , '-' },
   { 'e', "tmac"      , 's', "groff�macro"                                       , '-' },
   { 'e', "wbt"       , 's', "wilson�window�ware�batch�script"                   , '-' },
   { 'e', "htc"       , 's', "microsoft�html�component�script"                   , '-' },
   { 'e', "xul"       , 's', "xml�user�interface�language"                       , '-' },
   { 'e', "gcode"     , 's', "control�language"                                  , '-' },
   { 'e', "pc"        , 's', "developer�program�creation�file"                   , '-' },
   { 'e', "script"    , 's', "yVIKEYS�script�(also�apple�newton)"                , '-' },
   { 'e', "macro"     , 's', "yVIKEYS�macro�repository"                          , '-' },
   { 'e', "unit"      , 's', "koios�unit�test�source"                            , '@' },
   { 'e', "la"        , 's', "gun�libtool�library�description"                   , '-' },
   { 'e', "lo"        , 's', "gun�libtool�library�support�file"                  , '-' },
   { 'e', "nib"       , 's', "apple�interface�builder"                           , '-' },
   { 'e', "strings"   , 's', "used�to�localize�program"                          , '-' },
   { 'e', "actual"    , 's', "unstructured�unit�testing�results"                 , '-' },
   { 'e', "expect"    , 's', "unstructured�unit�testing�expects"                 , '-' },
   { 'e', "scrp"      , 's', "unstructured�unit�testing�script"                  , '-' },
   { 'e', "make"      , 's', "generic�make�file�extension"                       , '-' },
   { 'e', "makefile"  , 's', "generic�make�file"                                 , '@' },
   { 'e', "mmake"     , 's', "master�make�file"                                  , '@' },
   { 'e', "patch"     , 's', "quilt�patch�for�updating�code"                     , '-' },
   { 'e', "dpatch"    , 's', "debian�patch�for�updating�code"                    , '-' },
   { 'e', "stdin"     , 's', "unstructured�unit�testing�input"                   , '-' },
   { 'e', "stdout"    , 's', "unstructured�unit�testing�input"                   , '-' },
   { 'e', "tcl"       , 's', "tool�command�language"                             , '-' },
   { 'e', "initd"     , 's', "initialization�script"                             , '-' },
   { 'e', "confd"     , 's', "initialization�configuration"                      , '-' },
   { 'e', "eclass"    , 's', "bash�script�for�ebuild�environment"                , '-' },
   { 'e', "rpm"       , 's', "redhat�package�distribution�format"                , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "TEXT"      , 't', "ascii-text�files�(plain�and�structured)"           , '-' },
   { 'e', "txt"       , 't', "normal�text�file"                                  , '@' },
   { 'e', "rptg"      , 't', "text�output�file�(write�only)"                     , '-' },
   { 'e', "ctrl"      , 't', "inter-run�data�storage�(rw�db)"                    , '-' },
   { 'e', "csv"       , 't', "comma�seperated�values"                            , '-' },
   { 'e', "tsv"       , 't', "tab�seperated�values"                              , '-' },
   { 'e', "howto"     , 't', "heatherly�howto/notes"                             , '@' },
   { 'e', "conf"      , 't', "configuration�file"                                , '@' },
   { 'e', "t_conf"    , 't', "configuration�file�(no�ext)"                       , '-' },
   { 'e', "use"       , 't', "portage�use�flags"                                 , '-' },
   { 'e', "ini"       , 't', "windows�configuration�file"                        , '-' },
   { 'e', "defs"      , 't', "alternate�configuration�file"                      , '-' },
   { 'e', "cfg"       , 't', "alternate�configuration�file"                      , '-' },
   { 'e', "xml"       , 't', "extended�markup�language"                          , '-' },
   { 'e', "rules"     , 't', "custom�build�xml�rules�file"                       , '-' },
   { 'e', "tmpl"      , 't', "unix�template�file"                                , '-' },
   { 'e', "def"       , 't', "unix�definition�file"                              , '-' },
   { 'e', "rdf"       , 't', "firefox�and�database�xml-like�file"                , '-' },
   { 'e', "json"      , 't', "javascript�object�notation�format"                 , '-' },
   { 'e', "htm"       , 't', "hypertext�markup�language"                         , '-' },
   { 'e', "html"      , 't', "hypertext�markup�language"                         , '-' },
   { 'e', "xhtml"     , 't', "hypertext�markup�language"                         , '-' },
   { 'e', "rhtml"     , 't', "ruby�hypertext�markup�language�file"               , '-' },
   { 'e', "css"       , 't', "cascading�style�sheet"                             , '-' },
   { 'e', "epub"      , 't', "open�book�standard"                                , '@' },
   { 'e', "pdf"       , 't', "adobe�portable�document"                           , '@' },
   { 'e', "rtf"       , 't', "rich�text�format"                                  , '-' },
   { 'e', "eps"       , 't', "encapsulated�postscript"                           , '-' },
   { 'e', "ps"        , 't', "postscript"                                        , '-' },
   { 'e', "theme"     , 't', "application�theming�file"                          , '-' },
   { 'e', "ent"       , 't', "bittorrent�information�file"                       , '-' },
   { 'e', "dtd"       , 't', "document�type�definition�file"                     , '-' },
   { 'e', "xsl"       , 't', "xml�style�sheet"                                   , '-' },
   { 'e', "dsl"       , 't', "dsssl�style�sheet"                                 , '-' },
   { 'e', "xsd"       , 't', "xml�schema�description�file"                       , '-' },
   { 'e', "torrent"   , 't', "bittorrent�information�file"                       , '-' },
   { 'e', "nanorc"    , 't', "nano�editor�configuration"                         , '-' },
   { 'e', "vimrc"     , 't', "vim�editor�configuration"                          , '-' },
   { 'e', "mo"        , 't', "gnu�gettext�file"                                  , '-' },
   { 'e', "gmo"       , 't', "gnu�gettext�file"                                  , '-' },
   { 'e', "gyges"     , 't', "gyges�spreadsheet"                                 , '@' },
   { 'e', "texi"      , 't', "texinfo�documentation"                             , '-' },
   { 'e', "sgml"      , 't', "sgml�document�file"                                , '-' },
   { 'e', "inc"       , 't', "include�file"                                      , '-' },
   { 'e', "info"      , 't', "gnu�info�reader�file"                              , '-' },
   { 'e', "rst"       , 't', "python�restructured�text�doc�file"                 , '-' },
   { 'e', "rest"      , 't', "python�restructured�text�doc�file"                 , '-' },
   { 'e', "daily"     , 't', "heatherly�time�tracking"                           , '-' },
   { 'e', "url"       , 't', "internet�url"                                      , '-' },
   { 'e', "x"         , 't', "man�file"                                          , '-' },
   { 'e', "pod"       , 't', "perl�documentation�file"                           , '-' },
   { 'e', "arac"      , 't', "arachne�sequencing�file"                           , '-' },
   { 'e', "r_link"    , 't', "link�to�another�file"                              , '-' },
   { 'e', "tags"      , 't', "ctag�tags�file"                                    , '-' },
   { 'e', "changes"   , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "copying"   , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "readme"    , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "todo"      , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "copyright" , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "changelog" , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "install"   , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "license"   , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "manifest"  , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "md"        , 't', "generic�software�helper�file"                      , '-' },
   { 'e', "manual"    , 't', "man�documentation�file"                            , '@' },
   { 'e', "tasks"     , 't', "metis�task�management�data"                        , '-' },
   { 'e', "diff"      , 't', "linux�file�comparison�output"                      , '-' },
   { 'e', "uni"       , 't', "unicode�maps"                                      , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "DBASE"     , 'b', "databases�(binary,�relational,�etc)"               , '-' },
   { 'e', "db"        , 'b', "unix�database"                                     , '@' },
   { 'e', "dbf"       , 'b', "dbase�file"                                        , '-' },
   { 'e', "mdb"       , 'b', "microsoft�access�database"                         , '-' },
   { 'e', "acad"      , 'b', "autocad�database"                                  , '-' },
   { 'e', "odb"       , 'b', "openoffice�database"                               , '-' },
   { 'e', "dat"       , 'b', "generic�file�storage"                              , '-' },
   { 'e', "sqlite"    , 'b', "sqlite�database�file"                              , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "CRYPT"     , 'c', "compressed�and�encrypted�files"                    , '-' },
   { 'e', "gz"        , 'c', "gzip�compressed�file"                              , '@' },
   { 'e', "tgz"       , 'c', "gzip�compressed�tar"                               , '@' },
   { 'e', "bz2"       , 'c', "bzip2�compressed�file"                             , '-' },
   { 'e', "tbz"       , 'c', "bzip2�compressed�tar"                              , '-' },
   { 'e', "tbz2"      , 'c', "bzip2�compressed�tar"                              , '-' },
   { 'e', "hqx"       , 'c', "mac�binhex�compressed"                             , '-' },
   { 'e', "zip"       , 'c', "zipped�compressed�file"                            , '-' },
   { 'e', "zipx"      , 'c', "zipped�compressed�file"                            , '-' },
   { 'e', "sit"       , 'c', "stuffit�compressed"                                , '-' },
   { 'e', "sitx"      , 'c', "stuffit�compressed"                                , '-' },
   { 'e', "pit"       , 'c', "packit�compressed"                                 , '-' },
   { 'e', "cpio"      , 'c', "unix�cpio�archive"                                 , '-' },
   { 'e', "tar"       , 'c', "unix�tar�archive"                                  , '-' },
   { 'e', "iso"       , 'c', "unix�distribution"                                 , '-' },
   { 'e', "lz"        , 'c', "lzma�compressed�file"                              , '-' },
   { 'e', "lzma"      , 'c', "lzma�compressed�file"                              , '-' },
   { 'e', "xz"        , 'c', "lzma�compressed�file"                              , '-' },
   { 'e', "lzo"       , 'c', "lzo�compressed�file"                               , '-' },
   { 'e', "z"         , 'c', "huffman�compressed"                                , '-' },
   { 'e', "s7z"       , 'c', "7-zip�compressed"                                  , '-' },
   { 'e', "cab"       , 'c', "windows�compressed�file"                           , '-' },
   { 'e', "rar"       , 'c', "rar�compressed�file"                               , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "PROP"      , 'p', "proprietary�and�commercial�formats"                , '-' },
   { 'e', "ebuild"    , 'p', "gentoo�ebuild�for�portage"                         , '-' },
   { 'e', "xls"       , 'p', "ms�excel�(older)"                                  , '-' },
   { 'e', "xlsx"      , 'p', "ms�excel�(newer)"                                  , '-' },
   { 'e', "xlsm"      , 'p', "ms�excel"                                          , '-' },
   { 'e', "xlw"       , 'p', "ms�excel�workbook"                                 , '-' },
   { 'e', "wk3"       , 'p', "lotus�1-2-3�worksheet"                             , '-' },
   { 'e', "doc"       , 'p', "ms�word�(older)"                                   , '-' },
   { 'e', "docx"      , 'p', "ms�word�(newer)"                                   , '-' },
   { 'e', "dot"       , 'p', "ms�word�template"                                  , '-' },
   { 'e', "ppt"       , 'p', "ms�powerpoint�(older)"                             , '-' },
   { 'e', "pptm"      , 'p', "ms�powerpoint�(older)"                             , '-' },
   { 'e', "pptx"      , 'p', "ms�powerpoint�(newer)"                             , '-' },
   { 'e', "mpp"       , 'p', "ms�project�file"                                   , '-' },
   { 'e', "odc"       , 'p', "openoffice�file"                                   , '-' },
   { 'e', "odi"       , 'p', "openoffice�image"                                  , '-' },
   { 'e', "oti"       , 'p', "openoffice�image�template"                         , '-' },
   { 'e', "odp"       , 'p', "openoffice�presentation"                           , '-' },
   { 'e', "otp"       , 'p', "openoffice�presentation�template"                  , '-' },
   { 'e', "opf"       , 'p', "openoffice�document�interchange"                   , '-' },
   { 'e', "opg"       , 'p', "openoffice�drawing�file"                           , '-' },
   { 'e', "sda"       , 'p', "openoffice�drawing�file"                           , '-' },
   { 'e', "ops"       , 'p', "openoffice�spreadsheet"                            , '-' },
   { 'e', "ods"       , 'p', "openoffice�spreadsheet"                            , '-' },
   { 'e', "sdc"       , 'p', "openoffice�spreadsheet"                            , '-' },
   { 'e', "sxc"       , 'p', "openoffice�spreadsheet"                            , '-' },
   { 'e', "oos"       , 'p', "openoffice�spreadsheet"                            , '-' },
   { 'e', "opt"       , 'p', "openoffice�text�document"                          , '-' },
   { 'e', "oot"       , 'p', "openoffice�text�document"                          , '-' },
   { 'e', "sxw"       , 'p', "openoffice�text�document"                          , '-' },
   { 'e', "jar"       , 'p', "java�archive�file"                                 , '-' },
   { 'e', "gbr"       , 'p', "gimp�brush�file"                                   , '-' },
   { 'e', "slk"       , 'p', "silk�spreadsheet�file"                             , '-' },
   { 'e', "oleo"      , 'p', "oleo�spreadsheet�file"                             , '-' },
   { 'e', "font"      , 'p', "font�file"                                         , '-' },
   { 'e', "afm"       , 'p', "adobe�font�metrics"                                , '-' },
   { 'e', "ttf"       , 'p', "truetype�font"                                     , '-' },
   { 'e', "dict"      , 'p', "dictionary�file"                                   , '-' },
   { 'e', "cmake"     , 'p', "cmake�module�file"                                 , '-' },
   { 'e', "txf"       , 'p', "opengl�font"                                       , '-' },
   { 'e', "lnk"       , 'p', "microsoft�symbolic�link"                           , '-' },
   { 'e', "xpt"       , 'p', "firefox�component"                                 , '-' },
   { 'e', "dip"       , 'p', "diptrace�electronics�design�file"                  , '-' },
   { 'e', "xpi"       , 'p', "firefox�extension�installation�file"               , '-' },
   { 'e', "mf"        , 'p', "java�manifest�file�for�archives"                   , '-' },
   { 'e', "crt"       , 'p', "firefox�certificate"                               , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "EXEC"      , 'x', "directly�executable�files"                         , '-' },
   { 'e', "a"         , 'x', "unix�static�library"                               , '-' },
   { 'e', "a_debug"   , 'x', "unix�static�library�(debug)"                       , '-' },
   { 'e', "ar"        , 'x', "unix�static�library"                               , '-' },
   { 'e', "so"        , 'x', "unix�shared�library"                               , '@' },
   { 'e', "so_debug"  , 'x', "unix�shared�library�(debug)"                       , '@' },
   { 'e', "dll"       , 'x', "dynamic�library"                                   , '-' },
   { 'e', "bfd"       , 'x', "gnu�binary�descriptor"                             , '-' },
   { 'e', "exe"       , 'x', "executable�file"                                   , '-' },
   { 'e', "com"       , 'x', "executable�file"                                   , '-' },
   { 'e', "pyc"       , 'x', "compiled�python�script"                            , '-' },
   { 'e', "pyo"       , 'x', "compiled�python�file"                              , '-' },
   { 'e', "rake"      , 'x', "compiled�ruby�script"                              , '-' },
   { 'e', "pyd"       , 'x', "python�dynamic�module"                             , '-' },
   { 'e', "debug"     , 'x', "gnu�program�debug"                                 , '-' },
   { 'e', "x_exec"    , 'x', "unix�exec�(no�ext)"                                , '@' },
   { 'e', "x_debug"   , 'x', "unix�exec�(heatherly�debug)"                       , '@' },
   { 'e', "x_unit"    , 'x', "unix�exec�(heatherly�unit)"                        , '-' },
   { 'e', "c_dev"     , 'x', "unix�character�device"                             , '-' },
   { 'e', "c_link"    , 'x', "link�to�char�device"                               , '-' },
   { 'e', "b_dev"     , 'x', "unix�block�device"                                 , '-' },
   { 'e', "b_link"    , 'x', "link�to�another�block�device"                      , '-' },
   { 'e', "f_fifo"    , 'x', "unix�fifo�communications"                          , '-' },
   { 'e', "f_link"    , 'x', "link�to�fifo�file"                                 , '-' },
   { 'e', "s_socket"  , 'x', "unix�socket�file"                                  , '-' },
   { 'e', "s_link"    , 'x', "link�to�socket�file"                               , '-' },
   { 'e', "x_link"    , 'x', "link�to�executable"                                , '-' },
   { 'e', "x_hidden"  , 'x', "hidden�executable"                                 , '-' },
   { 'e', "img"       , 'x', "iso�binary�image�file"                             , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "DIR"       , 'd', "file�directories"                                  , '-' },
   { 'e', "d_dir"     , 'd', "directory"                                         , '@' },
   { 'e', "d_link"    , 'd', "link�to�another�directory"                         , '-' },
   { 'e', "d_hidden"  , 'd', "hidden�directory"                                  , '-' },
   { 'e', "d_gitroot" , 'd', "git�root�directories"                              , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "JUNK"      , 'j', "temporary�and�transcient�files"                    , '-' },
   { 'e', "swp"       , 'j', "swap�file"                                         , '-' },
   { 'e', "swo"       , 'j', "swap�file"                                         , '-' },
   { 'e', "backup"    , 'j', "simple/copied�file"                                , '-' },
   { 'e', "b_tilde"   , 'j', "tilde�suffix�file"                                 , '-' },
   { 'e', "old"       , 'j', "simple/copied�file"                                , '-' },
   { 'e', "sav"       , 'j', "simple/copied�file"                                , '-' },
   { 'e', "save"      , 'j', "simple/copied�file"                                , '-' },
   { 'e', "bak"       , 'j', "simple/copied�file"                                , '-' },
   { 'e', "cache"     , 'j', "temporary�storage"                                 , '-' },
   { 'e', "tmp"       , 'j', "temporary�file"                                    , '-' },
   { 'e', "temp"      , 'j', "temporary�file"                                    , '-' },
   { 'e', "log"       , 'j', "unix�log�file"                                     , '-' },
   { 'e', "urun"      , 'j', "yUNIT�unit�test�output�file"                       , '-' },
   { 'e', "ulog"      , 'j', "heatherly�log�file"                                , '-' },
   { 'e', "out"       , 'j', "unstructed�dump�file"                              , '-' },
   { 'e', "dmp"       , 'j', "program�crash�file"                                , '-' },
   { 'e', "dump"      , 'j', "text�reporting�file"                               , '-' },
   { 'e', "o"         , 'j', "complier�object�file"                              , '-' },
   { 'e', "os"        , 'j', "stripped�object�file"                              , '-' },
   { 'e', "obj"       , 'j', "microsoft�object�file"                             , '-' },
   { 'e', "po"        , 'j', "object�file�portable"                              , '-' },
   { 'e', "j_lib"     , 'j', "unistalled/dup�library"                            , '-' },
   { 'e', "j_exec"    , 'j', "unistalled/dup�exec"                               , '-' },
   { 'e', "j_unit"    , 'j', "compiled�unit�test"                                , '-' },
   { 'e', "calls"     , 'j', "polyhmnia�external�call�file"                      , '-' },
   { 'e', "lcalls"    , 'j', "polyhmnia�local�call�file"                         , '-' },
   { 'e', "code"      , 'j', "polyhmnia�code�extract"                            , '-' },
   { 'e', "flow"      , 'j', "polyhmnia�code�tracing"                            , '-' },
   { 'e', "asterion"  , 'j', "polyhmnia�input�for�asterion"                      , '-' },
   { 'e', "cgraph"    , 'j', "polyhmnia�input�for�asterion"                      , '-' },
   { 'e', "ylib"      , 'j', "polyhymnia�yLIB�coord�list"                        , '-' },
   { 'e', "mystry"    , 'j', "polyhymnia�issues�file"                            , '-' },
   { 'e', "htags"     , 'j', "polyhymnia�tags�file"                              , '-' },
   { 'e', "cs"        , 'j', "stripped�c�source�code"                            , '-' },
   { 'e', "j_c"       , 'j', "koios�generated�code"                              , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "OTHER"     , 'o', "file�held�apart�from�other�categories"             , '-' },
   { 'e', "o_ext"     , 'o', "uncategorized�(has�suffix)"                        , '-' },
   { 'e', "o_none"    , 'o', "unlabeled�(no�suffix)"                             , '-' },
   { 'e', "gitignore" , 'o', "git�helper�file"                                   , '-' },
   { 'e', "o_hidden"  , 'o', "files�intentionally�hidden"                        , '-' },
   { 'e', "o_git"     , 'o', "git�local�file"                                    , '-' },
   { 'e', "o_portage" , 'o', "gentoo/portage�dist�file"                          , '-' },
   { 'e', "o_kernel"  , 'o', "kernel�source�code"                                , '-' },
   { 'e', "o_cache"   , 'o', "compilation�cache�file"                            , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "HUH"       , '?', "just�and�simply�confusing"                         , '-' },
   { 'e', "u_link"    , '?', "link�to�another�unknown�thing"                     , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "PRIV"      , '!', "details�hidden/obscured"                           , '-' },
   { 'e', "!_priv"    , '!', "details�hidden�for�privacy"                        , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   { 'C', "EMPTY"     , 'e', "empty�drive�space"                                 , '-' },
   { 'e', "e_empty"   , 'e', "empty�drive�space"                                 , '-' },
   /*---  ---ext------  cat  ---desc---------------------------------------------  imp */
   {  0 , "---end---" , '-', "---end-of-list---"                                 , '-' },
};




tMIME       g_cats  [MAX_CAT];
int         g_ncat  = 0;
tMIME       g_mime  [MAX_MIME];
int         g_nmime = 0;


static char  s_head    [LEN_RECD]  = "";
static char  s_cols    [LEN_RECD]  = "";
static char  s_recd    [LEN_RECD]  = "";


/*---(simple file reading stats)---------*/
static    int s_seen  = 0;
static    int s_pass  = 0;
static    int s_fail  = 0;



/*====================------------------------------------====================*/
/*===----                        mass changes                          ----===*/
/*====================------------------------------------====================*/
static void  o___MASS____________o () { return; }

char
MIME__reset_all         (tMIME *a_curr)
{
   /*---(seen)--------------*/
   a_curr->u_seen   = 0;
   a_curr->n_seen   = 0;
   a_curr->b_seen   = 0;
   /*---(kept)--------------*/
   a_curr->u_kept   = 0;
   a_curr->n_kept   = 0;
   a_curr->b_kept   = 0;
   /*---(found)-------------*/
   a_curr->u_found  = 0;
   a_curr->n_found  = 0;
   a_curr->b_found  = 0;
   /*---(done)--------------*/
   return 0;
}

char
MIME__reset_found       (tMIME *a_curr)
{
   /*---(found)-------------*/
   a_curr->u_found  = 0;
   a_curr->n_found  = 0;
   a_curr->b_found  = 0;
   /*---(done)--------------*/
   return 0;
}

char
MIME__clear             (tMIME *a_curr)
{
   ystrlcpy (a_curr->ext , "", LEN_TERSE);
   a_curr->cat       = '-';
   ystrlcpy (a_curr->desc, "", LEN_DESC);
   a_curr->like      = '-';
   MIME__reset_all (a_curr);
   return 0;
}

char
MIME_reset_all          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =  0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   /*---(reset cats)---------------------*/
   DEBUG_CONF  yLOG_snote   ("cats");
   DEBUG_CONF  yLOG_sint    (g_ncat);
   for (i = 0; i < g_ncat;  ++i)   MIME__reset_all (&(g_cats [i]));
   /*---(reset mime)---------------------*/
   DEBUG_CONF  yLOG_snote   ("mime");
   DEBUG_CONF  yLOG_sint    (g_nmime);
   for (i = 0; i < g_nmime; ++i)    MIME__reset_all (&(g_mime [i]));
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MIME_reset_found        (void)
{
   int         i           =  0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   /*---(reset cats)---------------------*/
   DEBUG_CONF  yLOG_snote   ("cats");
   DEBUG_CONF  yLOG_sint    (g_ncat);
   for (i = 0; i < g_ncat;  ++i)   MIME__reset_found (&(g_cats [i]));
   /*---(reset mime)---------------------*/
   DEBUG_CONF  yLOG_snote   ("mime");
   DEBUG_CONF  yLOG_sint    (g_nmime);
   for (i = 0; i < g_nmime; ++i)    MIME__reset_found (&(g_mime [i]));
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
MIME__purge             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(purge cats)---------------------*/
   DEBUG_CONF  yLOG_note    ("initialize cats table");
   for (i = 0; i < MAX_CAT;  ++i)   MIME__clear  (&(g_cats [i]));
   /*---(purge mime)---------------------*/
   DEBUG_CONF  yLOG_note    ("initialize mime table");
   for (i = 0; i < MAX_MIME; ++i)   MIME__clear  (&(g_mime [i]));
   /*---(reset globals)------------------*/
   DEBUG_CONF  yLOG_note    ("reset global counters");
   g_ncat     = 0;
   g_nmime    = 0;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      program level                           ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
MIME_init          (void)
{
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(clear table)--------------------*/
   MIME__purge ();
   /*---(read config)--------------------*/
   /*> rc = MIME_pull (my.file_mime);                                                 <*/
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return rc;
}


/*====================------------------------------------====================*/
/*===----                     single entity action                     ----===*/
/*====================------------------------------------====================*/
static void  o___SINGLE__________o () { return; }

char
MIME_find               (char a_ext [LEN_TERSE], int *r_itotal, char r_total [LEN_TERSE], int *r_icat, char r_cat [LEN_TERSE], char *r_ccat, int *r_iext)
{
   /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         e           =   -1;
   int         c           =   -1;
   int         t           =   -1;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_itotal != NULL)  *r_itotal = -1;
   if (r_icat   != NULL)  *r_icat   = -1;
   if (r_iext   != NULL)  *r_iext   = -1;
   if (r_total  != NULL)  strcpy (r_total, "");
   if (r_cat    != NULL)  strcpy (r_cat  , "");
   if (r_ccat   != NULL)  *r_ccat   = '-';
   /*---(defense)------------------------*/
   DEBUG_CONF  yLOG_sint    (g_nmime);
   --rce;  if (g_nmime == 0) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_spoint  (a_ext);
   --rce;  if (a_ext == NULL) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_snote   (a_ext);
   /*---(find mime)----------------------*/
   for (i = 0; i < g_nmime; ++i) {
      if (g_mime [i].ext  [0] != a_ext [0])      continue;
      if (g_mime [i].ext  [1] != a_ext [1])      continue;
      if (strcmp (g_mime [i].ext , a_ext) != 0)  continue;
      e = i;
      c = g_mime [e].tie;
      t = g_mime [c].tie;
      if      (g_mime [e].level == 'T')   t = c = e;
      else if (g_mime [e].level == 'C')   c = e;
      break;
   }
   /*---(check trouble)---------------*/
   DEBUG_CONF  yLOG_sint    (e);
   --rce;  if (e < 0) {
      DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_sint    (c);
   DEBUG_CONF  yLOG_sint    (t);
   /*---(save-back indexes)-----------*/
   if (r_itotal != NULL)  *r_itotal = t;
   if (r_icat   != NULL)  *r_icat   = c;
   if (r_iext   != NULL)  *r_iext   = e;
   /*---(save-back names)-------------*/
   if (r_total  != NULL)  ystrlcpy (r_total, g_mime [t].ext, LEN_TERSE);
   if (r_cat    != NULL)  ystrlcpy (r_cat  , g_mime [c].ext, LEN_TERSE);
   if (r_ccat   != NULL)  *r_ccat   = g_mime [e].cat;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
   return 1;
}

char
MIME__action            (char a_ext [LEN_TERSE], char a_action, llong a_bytes, char *r_ccat)
{
   /*---(local variables)--+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =   -1;
   int         x_len       =    0;
   int         t, c, e;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(find ext)-----------------------*/
   rc = MIME_find (a_ext, &t, NULL, &c, NULL, r_ccat, &e);
   DEBUG_CONF  yLOG_value   ("find"      , rc);
   --rce;  if (rc <= 0) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF  yLOG_complex ("found"     , "%3dt, %3dc, %3de", t, c, e);
   /*---(act)----------------------------*/
   --rce;  switch (a_action) {
   case 's'  :  /*---(seen)--------------*/
      DEBUG_CONF  yLOG_note    ("handling a seen addition");
      /*---(used)------------------------*/
      if (g_mime [e].u_seen == 0) {
         DEBUG_CONF  yLOG_note    ("add into used");
         ++(g_mime [t].u_seen);
         ++(g_mime [c].u_seen);
         g_mime [e].u_seen = 1;
      }
      /*---(count)-----------------------*/
      ++(g_mime [t].n_seen);
      ++(g_mime [c].n_seen);
      ++(g_mime [e].n_seen);
      /*---(size)------------------------*/
      g_mime [t].b_seen  += a_bytes;
      g_mime [c].b_seen  += a_bytes;
      g_mime [e].b_seen  += a_bytes;
      /*---(done)------------------------*/
      break;
   case 'S'  : 
      DEBUG_CONF  yLOG_note    ("handling a seen deletion");
      /*---(used)------------------------*/
      if (g_mime [e].n_seen == 1) {
         --(g_mime [t].u_seen);
         --(g_mime [c].u_seen);
         g_mime [e].u_seen = 0;
      }
      /*---(count)-----------------------*/
      --(g_mime [t].n_seen);
      --(g_mime [c].n_seen);
      --(g_mime [e].n_seen);
      /*---(size)------------------------*/
      g_mime [t].b_seen  -= a_bytes;
      g_mime [c].b_seen  -= a_bytes;
      g_mime [e].b_seen  -= a_bytes;
      /*---(done)------------------------*/
      break;
   case 'k'  :  /*---(kept)--------------*/
      DEBUG_CONF  yLOG_note    ("handling a kept addition");
      /*---(used)------------------------*/
      if (g_mime [e].u_kept == 0) {
         ++(g_mime [t].u_kept);
         ++(g_mime [c].u_kept);
         g_mime [e].u_kept = 1;
      }
      /*---(count)-----------------------*/
      ++(g_mime [t].n_kept);
      ++(g_mime [c].n_kept);
      ++(g_mime [e].n_kept);
      /*---(size)------------------------*/
      g_mime [t].b_kept  += a_bytes;
      g_mime [c].b_kept  += a_bytes;
      g_mime [e].b_kept  += a_bytes;
      /*---(done)------------------------*/
      break;
   case 'f'  :  /*---(found)-------------*/
      DEBUG_CONF  yLOG_note    ("handling a found addition");
      /*---(used)------------------------*/
      if (g_mime [e].u_found == 0) {
         ++(g_mime [t].u_found);
         ++(g_mime [c].u_found);
         g_mime [e].u_found = 1;
      }
      /*---(count)-----------------------*/
      ++(g_mime [t].n_found);
      ++(g_mime [c].n_found);
      ++(g_mime [e].n_found);
      /*---(size)------------------------*/
      g_mime [t].b_found += a_bytes;
      g_mime [c].b_found += a_bytes;
      g_mime [e].b_found += a_bytes;
      /*---(done)------------------------*/
      break;
   default   :
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char MIME_add_seen    (char a_ext [LEN_TERSE], llong a_bytes, char *r_ccat) { return MIME__action (a_ext, 's', a_bytes, r_ccat); }
char MIME_add_kept    (char a_ext [LEN_TERSE], llong a_bytes)               { return MIME__action (a_ext, 'k', a_bytes, NULL  ); }
char MIME_add_found   (char a_ext [LEN_TERSE], llong a_bytes)               { return MIME__action (a_ext, 'f', a_bytes, NULL  ); }

char MIME_del_seen    (char a_ext [LEN_TERSE], llong a_bytes)               { return MIME__action (a_ext, 'S', a_bytes, NULL  ); }



/*====================------------------------------------====================*/
/*===----                       reading mime files                     ----===*/
/*====================------------------------------------====================*/
static void  o___INPUT___________o () { return; }

char
MIME__single            (short n, char a_level, char a_ext [LEN_TERSE], char a_cat, char a_desc [LEN_DESC], char a_like)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   char        t           [LEN_FULL]  = "";
   static char x_save      =  ' ';
   static short  c         =   -1;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(reset)-----------------------*/
   if (n < 0) {
      DEBUG_CONF  yLOG_note    ("reseting for load");
      MIME__purge ();
      x_save = ' ';
      c      =  0;
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   ++s_seen;
   /*---(defense)---------------------*/
   DEBUG_CONF   yLOG_point   ("a_ext"     , a_ext);
   --rce;  if (a_ext == NULL  || a_ext [0]  == '\0') {
      yURG_err ('f', "%3d suffix/extension was empty or null", n);
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce; if (strchr ("Tabcdefghijklmnopqrstuvwxyz?!-", a_cat) == NULL) {
      DEBUG_CONF  yLOG_note    ("FATAL, cat not allowed");
      yURG_err ('f', "%3d category �%c� not recognized", n, a_cat);
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_desc"    , a_desc);
   --rce;  if (a_desc == NULL || a_desc [0] == '\0') {
      yURG_err ('f', "%3d description was empty or null", n);
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(display)---------------------*/
   DEBUG_CONF   yLOG_complex ("load"      , "%3di %3dc %3dm  %c  %-10.10s  %c  %c  %s", n, g_ncat, g_nmime, a_level, a_ext, a_cat, a_like, a_desc);
   sprintf (t, "%3d  %c  loading �%s�", n, a_level, a_ext);
   yURG_msg ('-', "%-30.30s  %c  �%s�", t, a_cat, a_desc);
   /*---(check level)-----------------*/
   if (strchr ("TCe", a_level) == NULL) {
      DEBUG_CONF   yLOG_note    ("entry does not have legal level (TCe)");
      yURG_err ('f', "entry does not have legal level (TCe)");
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check total)-----------------*/
   if (g_nmime == 0 && a_level != 'T') {
      DEBUG_CONF   yLOG_note    ("TOTAL must come very first in mime file");
      yURG_err ('f', "TOTAL must come very first in mime file");
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check category)--------------*/
   if (g_nmime > 0 && a_level == 'e' && x_save != a_cat) {
      DEBUG_CONF   yLOG_note    ("CATEGORY must come immediately before extensions");
      yURG_err ('f', "CATEGORY must come immediately before extensions");
      c      =  0;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check room)------------------*/
   if (g_nmime >= MAX_MIME) {
      DEBUG_CONF   yLOG_note    ("overflowed g_nmime table");
      yURG_err ('f', "overflowed the g_mime table");
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check duplicate)-------------*/
   rc = MIME_find (a_ext, NULL, NULL, NULL, NULL, NULL, NULL);
   if (rc == 1) {
      DEBUG_CONF   yLOG_note    ("duplicate, matching extension already loaded");
      yURG_err ('f', "duplicate, matching extension already loaded");
      ++s_fail;
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)------------------------*/
   g_mime [g_nmime].level         = a_level;
   ystrlcpy (g_mime [g_nmime].ext , a_ext , LEN_TERSE);
   g_mime [g_nmime].cat           = a_cat;
   if (a_level == 'e')              g_mime [g_nmime].tie = c;
   else                             g_mime [g_nmime].tie = 0;
   ystrlcpy (g_mime [g_nmime].desc, a_desc, LEN_DESC);
   g_mime  [g_nmime].like         = a_like;
   ++s_pass;
   /*---(save category)---------------*/
   if (a_level == 'C') {
      x_save = a_cat;
      c      = g_nmime;
   }
   /*---(increment counters)----------*/
   ++g_nmime;
   if (a_level != 'e')   ++g_ncat;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME__append            (short a_useen, int a_nseen, llong a_bseen, short a_ukept, int a_nkept, llong a_bkept)
{
   /*---(seen)---------------------------*/
   g_mime [g_nmime - 1].u_seen  = a_useen;
   g_mime [g_nmime - 1].n_seen  = a_nseen;
   g_mime [g_nmime - 1].b_seen  = a_bseen;
   /*---(kept)---------------------------*/
   g_mime [g_nmime - 1].u_kept  = a_ukept;
   g_mime [g_nmime - 1].n_kept  = a_nkept;
   g_mime [g_nmime - 1].b_kept  = a_bkept;
   /*---(complete)-----------------------*/
   return 0;
}

char
MIME_standard           (void)
{  /*---(design notes)-------------------*/
   /*
    *  process will keep going if it hits trouble to give maximum feedback,
    *  but will return failure at the end if there are any fails
    *
    */
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "loading standard categorization data...");
   /*---(reset checks)-------------------*/
   s_seen = s_pass = s_fail = 0;
   /*---(purge the tables)---------------*/
   rc = MIME__single (-1, '-', "", '-', "", '-');
   DEBUG_CONF   yLOG_value   ("reset"     , rc);
   /*---(load all)-----------------------*/
   --rce;  while (s_load [i].level != 0) {
      rc = MIME__single (i, s_load [i].level, s_load [i].ext, s_load [i].cat, s_load [i].desc, s_load [i].like);
      ++i;
      /*---(done)------------------------*/
   }
   /*---(handle results)-----------------*/
   yURG_msg ('-', "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   DEBUG_CONF   yLOG_complex ("values"    , "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   --rce; if (s_seen <= 0) {
      yURG_err ('f', "zero mime records found");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce; if (s_fail > 0) {
      yURG_err ('f', "ended with failed mime records");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, mime file read");
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME__handler           (int a_line, uchar *a_verb)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   uchar       x_name      [LEN_LABEL] = "";
   uchar       x_cat       =  '-';
   uchar       x_desc      [LEN_DESC]  = "";
   uchar       x_like      =  '-';
   int         x_useen     =    0;
   int         x_nseen     =    0;
   llong       x_bseen     =    0;
   int         x_ukept     =    0;
   int         x_nkept     =    0;
   llong       x_bkept     =    0;
   int         i           =    0;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   DEBUG_CONF  yLOG_complex ("args"      , "%3d, %s", a_line, a_verb);
   /*---(read)---------------------------*/
   rc = yPARSE_scanf (a_verb, "TcDc,s,s;s,s,s;s", x_name, &x_cat, x_desc, &x_like, &x_useen, NULL, &x_nseen, NULL, &x_bseen, NULL, &x_ukept, NULL, &x_nkept, NULL, &x_bkept, NULL);
   DEBUG_CONF  yLOG_complex ("returns"   , "%-10.10s %c %-10.10s %c %10.0d %16.0ld %10.0d %16.0ld", x_name, x_cat, x_desc, x_like, x_nseen, x_bseen, x_nkept, x_bkept);
   /*---(filter)-------------------------*/
   if (strncmp (a_verb, "s_", 2) == 0 || strncmp (a_verb, "f_", 2) == 0) {
      DEBUG_CONF  yLOG_note    ("file header data");
      DEBUG_CONF  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle)-------------------------*/
   rc = MIME__single (a_line, a_verb [0], x_name, x_cat, x_desc, x_like);
   if (rc >= 0)  MIME__append (x_useen, x_nseen, x_bseen, x_ukept, x_nkept, x_bkept);
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_pull          (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading mime categorization file...");
   /*---(purge the tables)---------------*/
   rc = yPARSE_reset_in ();
   DEBUG_CONF   yLOG_value   ("purge_in"  , rc);
   rc = MIME__single (-1, '-', "", '-', "", '-');
   DEBUG_CONF   yLOG_value   ("purge"     , rc);
   s_seen = s_pass = s_fail = 0;
   /*---(read in configuration)----------*/
   rc = yPARSE_autoread (a_name, NULL, MIME__handler);
   DEBUG_CONF   yLOG_value   ("autoread"  , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed while openning mime file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yPARSE_close ();
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce; if (rc < 0) {
      yURG_err ('f', "yPARSE failed closing mime file");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle results)-----------------*/
   yURG_msg ('-', "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   DEBUG_CONF   yLOG_complex ("values"    , "%3d seen, %3d pass, %3d fail", s_seen, s_pass, s_fail);
   --rce; if (s_seen <= 0) {
      yURG_err ('f', "zero mime records found");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce; if (s_fail > 0) {
      yURG_err ('f', "ended with failed mime records");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, mime file read");
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       writing mime files                     ----===*/
/*====================------------------------------------====================*/
static void  o___OUTPUT__________o () { return; }

static int s_count = 0;

char
MIME__write_clear       (void)
{
   ystrlcpy (s_head, "", LEN_RECD);
   ystrlcpy (s_cols, "", LEN_RECD);
   ystrlcpy (s_recd, "", LEN_RECD);
   return 0;
}

int
MIME__write_callback    (int n, uchar *a_verb)
{
   char        rc          =    0;
   char        x_lvl       [LEN_TERSE] = "";
   uchar       x_cat       =  ' ';
   int         xu_spct, xn_spct, xb_spct;
   int         xu_kpct, xn_kpct, xb_kpct;
   int         xu_fpct, xn_fpct, xb_fpct;
   int         xn_pct , xb_pct;
   int         xn_pctf, xb_pctf;
   char       *x_spec      = "TcDc,s,s;s,s,s;sss,s,s;sss";
   char       *x_head      = "ext,t,desc,l,used,pct,seen,pct,seen-bytes,pct,used,pct,kept,pct,kept-bytes,pct,cnt,siz,used,pct,found,pct,found-bytes,pct,cnt,siz";
   static int  c           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   DEBUG_CONF  yLOG_complex ("args"      , "%3d, %s", n, a_verb);
   switch (g_mime [n].level) {
   case 'T' :
      s_count = 0;
      strcpy (x_lvl, "TOTAL");
      yPARSE_section    (g_mime [n].ext);
      yPARSE_columns (s_count, x_spec, x_head);
      break;
   case 'C' : 
      if (n > 1)  yPARSE_footer (s_count);
      s_count = 0;
      strcpy (x_lvl, "CAT"  );
      yPARSE_section    (g_mime [n].ext);
      yPARSE_columns (s_count, x_spec, x_head);
      break;
   case 'e' :
      strcpy (x_lvl, "ext"  );
      yPARSE_columns (s_count, x_spec, x_head);
      ++s_count;
      break;
   }
   /*---(seen ratios)--------------------*/
   xu_spct  = ((double) g_mime [n].u_seen  / g_mime [0].u_seen)  * 100;
   xn_spct  = ((double) g_mime [n].n_seen  / g_mime [0].n_seen)  * 100;
   xb_spct  = ((double) g_mime [n].b_seen  / g_mime [0].b_seen)  * 100;
   /*---(kept ratios)--------------------*/
   xu_kpct  = ((double) g_mime [n].u_kept  / g_mime [0].u_kept)  * 100;
   xn_kpct  = ((double) g_mime [n].n_kept  / g_mime [0].n_kept)  * 100;
   xb_kpct  = ((double) g_mime [n].b_kept  / g_mime [0].b_kept)  * 100;
   /*---(kept/seen ratios)---------------*/
   xn_pct   = ((double) g_mime [n].n_kept  / g_mime [n].n_seen)  * 100;
   xb_pct   = ((double) g_mime [n].b_kept  / g_mime [n].b_seen)  * 100;
   /*---(found ratios)-------------------*/
   xu_fpct  = ((double) g_mime [n].u_found / g_mime [0].u_found) * 100;
   xn_fpct  = ((double) g_mime [n].n_found / g_mime [0].n_found) * 100;
   xb_fpct  = ((double) g_mime [n].b_found / g_mime [0].b_found) * 100;
   /*---(found/seen ratios)--------------*/
   xn_pctf  = ((double) g_mime [n].n_found / g_mime [n].n_seen)  * 100;
   xb_pctf  = ((double) g_mime [n].b_found / g_mime [n].b_seen)  * 100;
   /*---(write out)----------------------*/
   yPARSE_printf (x_lvl, x_spec,
         g_mime [n].ext, g_mime [n].cat, g_mime [n].desc, g_mime [n].like,
         g_mime [n].u_seen, xu_spct,
         g_mime [n].n_seen, xn_spct, g_mime [n].b_seen, xb_spct,
         g_mime [n].u_kept, xu_kpct,
         g_mime [n].n_kept, xn_kpct, g_mime [n].b_kept, xb_kpct,
         xn_pct, xb_pct,
         g_mime [n].u_found, xu_fpct,
         g_mime [n].n_found, xn_fpct, g_mime [n].b_found, xb_fpct,
         xn_pctf, xb_pctf);
   /*---(process mime ext)---------------*/
   /*> for (i = 0; i < g_nmime; ++i) {                                                <* 
    *>    if (g_mime [i].cat != x_cat)  continue;                                     <* 
    *>    xn_spct  = ((double) g_mime [i].n_seen / g_cats [0].n_seen) * 100;          <* 
    *>    xb_spct  = ((double) g_mime [i].b_seen / g_cats [0].b_seen) * 100;          <* 
    *>    xn_kpct  = ((double) g_mime [i].n_kept / g_cats [0].n_kept) * 100;          <* 
    *>    xb_kpct  = ((double) g_mime [i].b_kept / g_cats [0].b_kept) * 100;          <* 
    *>    xn_pct   = ((double) g_mime [i].n_kept / g_mime [i].n_seen) * 100;          <* 
    *>    xb_pct   = ((double) g_mime [i].b_kept / g_mime [i].b_seen) * 100;          <* 
    *>    xn_fpct  = ((double) g_mime [i].n_found / g_cats [0].n_found) * 100;        <* 
    *>    xb_fpct  = ((double) g_mime [i].b_found / g_cats [0].b_found) * 100;        <* 
    *>    yPARSE_sect_break (s_count);                                                      <* 
    *>    yPARSE_printf ("ext","TcDc,s;s,s;sss,s;sss",                                <* 
    *>          g_mime [i].ext, g_mime [i].cat, g_mime [i].desc, g_mime [i].like,     <* 
    *>          g_mime [i].n_seen, xn_spct, g_mime [i].b_seen, xb_spct,               <* 
    *>          g_mime [i].n_kept, xn_kpct, g_mime [i].b_kept, xb_kpct,               <* 
    *>          xn_pct, xb_pct,                                                       <* 
    *>          g_mime [i].n_found, xn_fpct, g_mime [i].b_found, xb_fpct,             <* 
    *>          xn_pctf, xb_pctf);                                                    <* 
    *>    ++s_count;                                                                        <* 
    *>    /+---(done)------------------------+/                                       <* 
    *> }                                                                              <*/
   /*---(end category)-------------------*/
   /*> yPARSE_sect_end (s_count);                                                           <*/
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return s_count;
}

char
MIME__write_handlers    (void)
{
   int         i           =    0;
   s_count = 0;
   for (i = 0; i < g_nmime; ++i) {
      MIME__write_callback (i, g_mime [i].ext);
   }
   if (i > 0)  yPARSE_footer (s_count);
   /*> yPARSE_handler_plus ("TOTAL", "TcDc,s;s,s;sss,s;sss", i, NULL, MIME__write_callback, "ext,t,desc,l,seen,pct,seen-bytes,pct,kept,pct,kept-bytes,pct,cnt,siz,found,pct,found-bytes,pct,cnt,siz");   <*/
   /*> yPARSE_handler_plus ("CAT"  , "TcDc,s;s,s;sss,s;sss", i, NULL, MIME__write_callback, "ext,t,desc,l,seen,pct,seen-bytes,pct,kept,pct,kept-bytes,pct,cnt,siz,found,pct,found-bytes,pct,cnt,siz");   <*/
   /*> yPARSE_handler_plus ("ext"  , "TcDc,s;s,s;sss,s;sss", i, NULL, MIME__write_callback, "ext,t,desc,l,seen,pct,seen-bytes,pct,kept,pct,kept-bytes,pct,cnt,siz,found,pct,found-bytes,pct,cnt,siz");   <*/
   return 0;
}

char
MIME__write_header      (void)
{
   yPARSE_spacer (1);
   yPARSE_printf ("f_public"    , "c"     , my.pub               );
   yPARSE_printf ("f_path"      , "O"     , my.path              );
   yPARSE_printf ("f_level"     , "s"     , my.maxlevel          );
   yPARSE_printf ("f_regex"     , "O"     , my.regex             );
   yPARSE_printf ("f_types"     , "O"     , my.types             );
   yPARSE_printf ("f_mimes"     , "O"     , my.mimes             );
   yPARSE_printf ("f_sizes"     , "O"     , my.sizes             );
   yPARSE_printf ("f_ages"      , "O"     , my.ages              );
   yPARSE_printf ("f_ascii"     , "O"     , my.ascii             );
   yPARSE_printf ("f_super"     , "c"     , my.super             );
   return 0;
}

char
MIME_report        (char a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   yPARSE_planned  (NULL, a_name, NULL);
   yPARSE_header  (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, "mime configuration file", MIME__write_header);
   MIME__write_handlers ();
   yPARSE_write_all ();
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       writing tree files                     ----===*/
/*====================------------------------------------====================*/
static void  o___TREE____________o () { return; }

char
MIME__tree_header       (void)
{
   yPARSE_spacer (1);
   yPARSE_printf ("o_format"    , "O"     , "mimecat"            );
   yPARSE_printf ("o_label0"    , "O"     , "total bytes"        );
   yPARSE_printf ("o_label1"    , "O"     , "total count"        );
   yPARSE_printf ("o_label2"    , "O"     , "average"            );
   yPARSE_printf ("o_label3"    , "O"     , "primary"            );
   yPARSE_printf ("o_label4"    , "O"     , "secondary"          );
   yPARSE_printf ("o_label5"    , "O"     , "terciary"           );
   yPARSE_spacer (1);
   return 0;
}

char
MIME__tree_line         (char a_lvl, uchar *a_ext, llong a_size, llong a_count, char *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   static int  c           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ystrlcpy (s_recd, "", LEN_RECD);
   /*---(main)---------------------------*/
   DEBUG_CONF  yLOG_complex ("node"      , "%2d %-10.10s %10ld %10d %s", a_lvl, a_ext, a_size, a_count, a_desc);
   if (a_size > 0)  yPARSE_vprintf (c, "node", a_lvl, a_ext, a_desc, a_size, a_count, a_size / a_count, 0, 0, 0);
   ++c;
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME__tree_cat          (uchar a_cat, uchar *a_name, int *n)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_senter  (__FUNCTION__);
   if (n != NULL)   *n = -1;
   for (i = 0; i <= g_ncat; ++i) {
      if (strcmp (a_name, g_cats [i].ext) == 0) {
         DEBUG_CONF  yLOG_snote   ("FOUND ext");
         DEBUG_CONF  yLOG_sint    (i);
         if (n != NULL)   *n = i;
         DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      if (a_cat == g_cats [i].cat) {
         DEBUG_CONF  yLOG_snote   ("FOUND cat");
         DEBUG_CONF  yLOG_sint    (i);
         if (n != NULL)   *n = i;
         DEBUG_CONF  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
   }
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
}

char
MIME_tree          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         n           =    0;
   uchar       x_save      = MIME_TOTAL;
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(header)-------------------------*/
   yPARSE_planned (NULL, "stdout", NULL);
   yPARSE_header  (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, "file size by mime category", MIME__tree_header);
   yPARSE_handler_plus ("node", "sTDhlllll", 0.0, NULL, NULL, "lvl,type,description,size,count,avg,1st,2nd,3rd");
   /*---(root)---------------------------*/
   if      (my.mime_all != '-')     MIME__tree_line      (0, g_cats [n].ext, g_cats [n].b_seen , g_cats [n].n_seen , g_cats [n].desc);
   else if (g_cats [0].b_found > 0) MIME__tree_line      (0, g_cats [n].ext, g_cats [n].b_found, g_cats [n].n_found, g_cats [n].desc);
   else                             MIME__tree_line      (0, g_cats [n].ext, g_cats [n].b_kept , g_cats [n].n_kept , g_cats [n].desc);
   /*---(process all)--------------------*/
   for (i = 0; i < g_nmime; ++i) {
      if (g_mime [i].cat != x_save) {
         x_save = g_mime [i].cat;
         MIME__tree_cat (x_save, "�����", &n);
         if      (my.mime_all != '-')     MIME__tree_line  (1, g_cats [n].ext, g_cats [n].b_seen , g_cats [n].n_seen , g_cats [n].desc);
         else if (g_cats [0].b_found > 0) MIME__tree_line  (1, g_cats [n].ext, g_cats [n].b_found, g_cats [n].n_found, g_cats [n].desc);
         else                             MIME__tree_line  (1, g_cats [n].ext, g_cats [n].b_kept , g_cats [n].n_kept , g_cats [n].desc);
      }
      if      (my.mime_all != '-')     MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_seen , g_mime [i].n_seen , g_mime [i].desc);
      else if (g_cats [0].b_found > 0) MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_found, g_mime [i].n_found, g_mime [i].desc);
      else                             MIME__tree_line (2, g_mime [i].ext, g_mime [i].b_kept , g_mime [i].n_kept , g_mime [i].desc);
   }
   /*---(footer)-------------------------*/
   yPARSE_close ();
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      central database                        ----===*/
/*====================------------------------------------====================*/
static void  o___DATABASE________o () { return; }

char
MIME_db_write           (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "writing mime table to database...");
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      yURG_err ('f', "database for reading is not open");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP  yLOG_value   ("g_nmime"   , g_nmime);
   --rce;  if (g_nmime <= 0) {
      yURG_err ('f', "no existing/loaded data to be written");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   yURG_msg ('-', "expecting %d records", g_nmime);
   for (i = 0; i < g_nmime; ++i) {
      rc = fwrite (&(g_mime [i]), sizeof (tMIME), 1, a_file);
      if (rc != 1)   break;
   }
   yURG_msg ('-', "wrote out %d records", i);
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("i"         , i);
   --rce;  if (g_nmime != i) {
      yURG_err ('f', "wrote fewer than the expected records");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, wrote expected count of records");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MIME_db_read            (FILE *a_file, short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   int         c           =    0;
   tMIME       x_curr;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "reading mime table from database...");
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      yURG_err ('f', "database for reading is not open");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database is open and ready");
   /*---(purge existing)-----------------*/
   rc = MIME__purge ();
   DEBUG_INPT   yLOG_value   ("purge"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "existing data could not be purged");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "purged existing configuration");
   /*---(write all)----------------------*/
   yURG_msg ('-', "expecting %d records", a_count);
   for (i = 0; i < a_count; ++i) {
      rc = fread  (&(g_mime [i]), sizeof (tMIME), 1, a_file);
      if (rc != 1)   break;
      ++g_nmime;
      if (g_mime [i].level != 'e')  ++g_ncat;
      ++c;
   }
   yURG_msg ('-', "read in %d records", i);
   /*---(check)--------------------------*/
   DEBUG_INPT  yLOG_value   ("a_count"   , a_count);
   --rce;  if (c != a_count) {
      yURG_err ('f', "read fewer than the expected records");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "SUCCESS, read expected count of records");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char
MIME__unit_line         (int n, char r_out [LEN_FULL])
{
   char        t           [LEN_DESC]  = "";
   if (n < 0 || n >= MAX_MIME) {
      snprintf (r_out, LEN_FULL, "MIME entry (???) : � � ���  ���������  ��                      �  s[��,���,�������]  k[��,���,�������]  f[��,���,�������]");
      return 0;
   }
   if (n >= g_nmime || strchr ("TCe", g_mime [n].cat) == NULL) {
      snprintf (r_out, LEN_FULL, "MIME entry (%3d) : � � ���  ���������  ��                      �  s[��,���,�������]  k[��,���,�������]  f[��,���,�������]", n);
   }
   sprintf  (t, "�%-.20s�", g_mime [n].desc);
   snprintf (r_out, LEN_FULL, "MIME entry (%3d) : %c %c %3d  %-9.9s  %-22.22s  %c  s[%2d,%3d,%7ld]  k[%2d,%3d,%7ld]  f[%2d,%3d,%7ld]", n,
         g_mime [n].level  , g_mime [n].cat    , g_mime [n].tie    ,
         g_mime [n].ext    , t                 , g_mime [n].like ,
         g_mime [n].u_seen , g_mime [n].n_seen , g_mime [n].b_seen ,
         g_mime [n].u_kept , g_mime [n].n_kept , g_mime [n].b_kept ,
         g_mime [n].u_found, g_mime [n].n_found, g_mime [n].b_found);
   return 0;
}

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
MIME__unit              (char *a_question, char *a_ext, int n)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   tMIME      *x_curr      = NULL;
   short       x_fore      =    0;
   short       x_back      =    0;
   short       c           =    0;
   char        t           [LEN_DESC]  = "";
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "ENTRY            : question not understood");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "MIME count       : all  = %3d, cat  = %3d, ext  = %3d", g_nmime, g_ncat, g_nmime - g_ncat);
   }
   else if (strcmp (a_question, "entry"         ) == 0) {
      if (a_ext == NULL)   MIME__unit_line ( n, unit_answer);
      else {
         rc = MIME_find (a_ext, NULL, NULL, NULL, NULL, NULL, &n);
         if (rc < 0)       MIME__unit_line (-1, unit_answer);
         else              MIME__unit_line ( n, unit_answer);
      }
   }
   else if (strcmp (a_question, "s_head"        ) == 0) {
      ystrlencode (s_head, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_head      : [%.79s]", s_head);
   }
   else if (strcmp (a_question, "s_cols"        ) == 0) {
      ystrlencode (s_cols, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_cols      : [%.79s]", s_cols);
   }
   else if (strcmp (a_question, "s_recd"        ) == 0) {
      ystrlencode (s_recd, ySTR_NORM, LEN_RECD);
      snprintf (unit_answer, LEN_FULL, "MIME s_recd      : [%.79s]", s_recd);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);
   return unit_answer;
}


