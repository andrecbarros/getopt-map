/* getopt-map-example.c
 *
 * Too much hassle perhaps but way better than try to remember
 * short options x long options associations or assign #defines
 * by hand to handle applications with a large number of
 * options. Things can be handled as easy as:
 *
 * while ((opt = getopt_long(ac, av, short_opts, long_opts, &optidx)) != -1) {
 *   switch (opt) {
 *   case _id_( <meaningful name #1> ): case '<associated char>':
 *     ..
 *     break;
 *     
 *   case _id_( <meaningful name #n> ):
 *     ..
 *     break;
 *     
 *   case ':': // Arguments missing for some options
 *     if (optopt && optopt < _id_( _lim_inf ))
 *       // Cope with it for short or user defined options inspecting optopt
 *     else if (optind)
 *       // Cope with it for long options by inspecting optind, av[optind-1] (its name) or optidx
 *     else
 *       // Dont think this is a possibility
 *     
 *   case '?': // Unknown options
 *     if (optopt && optopt < _id_( _lim_inf ))
 *       // Cope with it for short or user defined options inspecting optopt
 *     else if (optind)
 *       // Cope with it for long options by inspecting optind, av[optind-1] (its name) or optidx
 *     else
 *       // Dont think this is a possibility
 *     
 *   case 0: 
 *     // There is no value stored on <long options>[optind].val
 *     // probably using *(<long options>[optind].flag)
 *     break;
 *
 *   case 1:
 *     // iteration through options was requested. Seldom used, specialized case.
 *     break;
 *     
 *   default: // Unhandled options - forgot to set them on 'case _id_( <its name> ):' statements here
 *     if (opt < _id_( _lim_inf ))
 *       // Warning about the need to fix this application. Short user defined options
 *     else if (optind)
 *       // Warning about the need to fix this application. Long options
 *     else
 *       // Dont think this is a possibility
 *   }
 *
 */
#include <getopt-map.h>
#include <stdlib.h>
#include <stdio.h>

// Except for _id_default_header_ and _id_default_footer_, 
// the order of appearance of the identifiers has no importance.
//
enum option_id {
    _id_default_header_

    _id_( optional ),
    _id_( hidden ),
    _id_( required ),
    _id_( flag ),
    _id_( help ),
    _id_( simple_test ),
    
    _id_default_footer_
};

struct option long_opts[] = {
    _opt_default_header_
    
    _opt_( optional, optional),
    _opt_( hidden, optional),
    _opt_( required, required),
    _opt_( flag, no),
    _opt_( help, 0),
    _opt_( simple_test, 0),                // Make both --simple_test and --simple-test
    _oph_( simple_test, "simple-test", 0), // acceptable
    
    _opt_default_footer_
};

struct option_map opts_maps[] = {
    _opt_map_default_header_
    
    _opt_map_( 0,           'g', "Char only option with required argument"),
    _opt_map_( optional,    'o', "Optional argument to option"),
    _opt_map_( hidden,      'H', 0), // Not ready yet
    _opt_map_( required,      0, "Required argument to option"),
    _opt_map_( flag,        'F', "Takes no option"),
    _opt_map_( help,        'h', "Display this help and exit"),
    _opt_map_( simple_test, 't', "Simple conformity test"),

    _opt_map_default_footer_
};

#define myapp_version _stringify_(VERSION)
#define myapp_license _stringify_(LICENSE)

int main (int ac, char *av[])
{
  char *short_opts = ":o::H::g:Fh";
  int opt, optidx;
  char *a_flag = 0, *a_optional = 0, *a_required = 0;
  
  //app_maps = opts_maps; // initialize the messages (may read them through 'getopt_msg_read'
                          // or write to a files with 'getopt_msg_write')
                            
  // opterr = 0; // No default error message
  while ((opt = getopt_long(ac, av, short_opts, long_opts, &optidx)) != -1) {
    switch (opt) {
    case 'g':
      a_flag = optarg;
      printf ("g = '%s'\n", optarg);
      break;
      
    case _id_( flag ): case 'F':
      a_flag = optarg;
      printf ("flag\n");
      break;
      
    case _id_( optional ): case 'o':
      a_optional = optarg;
      printf ("optional = '%s'\n", optarg);
      break;
      
    case _id_( hidden ): case 'H':
      a_optional = optarg;
      printf ("hidden = '%s'\nHey, this is for internal tests only!\n", optarg);
      break;
      
    case _id_( required ):
      a_required = optarg;
      printf ("required = '%s'\n", optarg);
      break;
      
    case ':':
      if (optopt && optopt < _id_( _lim_inf ))
        printf ("%s -%c\n", getopt_msg (opts_maps, _id_( _arg_missing )), optopt);
      else if (optind)
        printf ("%s %s\n", getopt_msg (opts_maps, _id_( _arg_missing )), av[optind-1]);
      else
        printf ("%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", getopt_msg (opts_maps, _id_( _arg_missing )), 
                                                         optopt, optind, optarg, optidx);
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 1);
      
    case '?':
      if (optopt && optopt < _id_( _lim_inf ))
        printf ("%s -%c\n", getopt_msg (opts_maps, _id_( _opt_unknown )), optopt);
      else if (optind)
        printf ("%s %s\n", getopt_msg (opts_maps, _id_( _opt_unknown )), av[optind-1]);
      else
        printf ("%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", getopt_msg (opts_maps, _id_( _opt_unknown )), 
                                                         optopt, optind, optarg, optidx);
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 1);
      
    case _id_( help ): case 'h':
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 1);
    
    case 0: // there is no value stored on long_opts[optind].val, check *(long_opts[optind].flag)
      printf ("%s --%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", getopt_msg (opts_maps, _id_( _opt_unhandled )), long_opts[optidx].name,
                                                            optopt, optind, optarg, optidx);
      break;

    case 1:  // iterate through all arguments. Specialized case not automatically handled
      printf ("%s %s (opt,ind,idx): %d,%d,%d\n", getopt_msg (opts_maps, _id_( _opt_iteration )), optarg,
                                                 optopt, optind, optidx);
      break;
      
    default:
      if (opt > _id_( _lim_inf ) && opt < _id_( _lim_sup ))
        printf ("%s --%s (%d)\n", getopt_msg (opts_maps, _id_( _opt_unhandled )), long_opts[optidx].name, opt);
      else if (isascii(opt))
        printf (isgraph(opt) ? "%s -%s -%c\n" : "%s %s (%d)\n", getopt_msg (opts_maps, _id_( _opt_unhandled )), long_opts[optidx].name, opt);
      else
        printf ("%s <%s> ('%s','%s',%d): %d,%d\n", getopt_msg (opts_maps, _id_( _app_error )), av[optind-1],
                                                   __FILE__, __func__, __LINE__, opt, optidx);
    }
  }
  
  exit (0);
}
