/* getopt-map.c - v0.1 - 2014.04.26 12:17:30 GMT-3
 * License: BSD-3 clauses,  MIT or GPL2+ (any of them for derivatives)
 * Andre C. Barros - <andre.cbarros@yahoo.com>
 *
 * Improve the useability of getopt_long by applying
 * the c pre-processor stringification, id-ification
 * and enum automatic serialization.
 * 
 * See getopt-map.h for more details on how to use this file.
 */
#include <getopt-map.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct option opt_zero = _opt_zero_;

#if ! defined (GETOPT_AVOID_BLOAT)
struct option_map opt_map_zero = _opt_map_zero_;
#endif

struct option *option_p (struct option *o, int id)
{
  if (o == 0 || id <= _id_( _lim_inf ) || id >= _id_( _lim_sup ))
    return NULL;
    
  for ( ; ; o++)
    if (o->val == id)
      return o;
    else if (o->val >= _id_( _lim_sup ) || (o->val == _id_( _zero ) && o->name == 0))
      return NULL;
}

#if ! defined (GETOPT_AVOID_BLOAT)
struct option_map *option_map_p (struct option_map *m, int id)
{
  if (m == 0 || id == _id_( _zero ))
    return NULL;
    
  for ( ; ; m++)
    if (m->id == id)
      return m;
    else if (m->id == _id_( _zero ) && m->ch == 0 && m->msg == 0)
      return NULL;
}

int getopt_map (struct option_map *m, int id)
{
  if (m == 0 || id <= _id_( _lim_inf ) || id >= _id_( _lim_sup ))
      
  for ( ; ; m++)
    if (m->id == id)
      return m->ch;
    else if (m->id >= _id_( _lim_sup ) || (m->id == _id_( _zero ) && m->ch == 0 && m->msg == 0))
      return 0;
}

char *getopt_msg (struct option_map *m, int id)
{
  static struct option_map *offsm = 0;
  
  if (m == 0 || id == _id_( _zero )) {
    offsm = 0;
    return NULL;
  }
  if (id > _id_( _lim_sup ) && id < _id_( _lim_messages )) {
    if (offsm == 0) {
      for ( ; m->id != _id_( _lim_sup ) ; m++)
        if (m->id == _id_( _zero ) && m->ch == 0 && m->msg == 0)
            return NULL;

      offsm = m;
    }
    return offsm[id - _id_( _lim_sup )].msg;
  }
    
  for ( ; ; m++)
    if (m->id == id)
      return m->msg;
  else if (m->id == _id_( _zero ) && m->ch == 0 && m->msg == 0)
    return NULL;
}

void getopt_usage (char *app_name, char *app_version, char *app_license, 
                   char *short_opts, struct option *long_opts,
                   struct option_map *opts_maps, int exit_val)
{
  char *app, *msg, *chr, *argobl, *argopt;
  struct option *opt;
  
  // Application information
  if (app_name) {
    app = strrchr (app_name, '/');     // Strip path
    app = (app == NULL) ? app_name : app + 1;
    
    printf ("%s", app);
    if (app_version)
      printf (" - v.%s", app_version);
    if (app_license)
      printf (" - (c).%s", app_license);
    printf ("\n");
  }
  
  if (opts_maps && (short_opts || long_opts)) {
    // Usage header information
    if ((msg = getopt_msg (opts_maps, _id_( _app_header ))) != NULL)
      printf (msg, app);

    if ((argobl = getopt_msg (opts_maps, _id_( _arg_obligatory ))) == NULL)
      argobl = "<...>";
    if ((argopt = getopt_msg (opts_maps, _id_( _arg_optional ))) == NULL)
      argopt = "[...]";

    // Print mapped options
    for ( ; opts_maps->id < _id_( _lim_sup ) && (opts_maps->id || opts_maps->ch  || opts_maps->msg) ; opts_maps++) {
    
      if (! opts_maps->msg) // Hidden options
        continue;

      if (opts_maps->id > _id_( _lim_inf )) {
        // Type of parameter availabe from <struct option> element
        opt = option_p (long_opts, opts_maps->id);
        chr = &opts_maps->ch;
      }
      else {
        opt = NULL;
        // Type of parameter availabe from <short_opts> string
        chr = strchr (short_opts, opts_maps->ch);
      }
      
      if (opt || (chr && *chr)) {
        printf ("    ");
        
        // Short (char) option?
        if (chr && *chr)
          printf ("-%c%s", *chr, opt ? ", ": " ");

        // Long option?
        if (opt)
          printf ("--%s %s", opt->name, 
                  opt->has_arg != no_argument ? opt->has_arg == required_argument ? argobl: argopt: "");
        else if (chr && *chr)
          printf ("%s", chr[1] == ':' ? chr[2] == ':' ? argopt: argobl: "");
          
        if (*opts_maps->msg)
          printf("\n        %s\n", opts_maps->msg);
        else
          printf("\n");
      }
    }
    if ((msg = getopt_msg (opts_maps, _id_( _app_footer ))) != NULL)
      printf (msg, app);
  }
  exit (exit_val);
}
#endif /* GETOPT_AVOID_BLOAT */

#ifdef __cplusplus
}
#endif
