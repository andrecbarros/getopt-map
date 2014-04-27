/* getopt-map-example2.c
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

#define myapp_version _stringify_(VERSION)
#define myapp_license _stringify_(LICENSE)

int main (int ac, char *av[])
{
  char *short_opts = ":o::H::g:Fh";
  int opt, optidx;
  char *a_flag = 0, *a_optional = 0, *a_required = 0;
  
  //app_maps = opts_maps; // initialize the messages (may read them through 'getopt_msg_read'
                          // or write to a files with 'getopt_msg_write')
                            
  opterr = 0; // No default error message
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
        printf ("%s -%c\n", "Argument missing for", optopt);
      else if (optind)
        printf ("%s %s\n", "Argument missing for", av[optind-1]);
      else
        printf ("%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", "Argument missing", 
                                                         optopt, optind, optarg, optidx);
      exit (1);
      
    case '?':
      if (optopt && optopt < _id_( _lim_inf ))
        printf ("%s -%c\n", "Unknown option", optopt);
      else if (optind)
        printf ("%s %s\n", "Unknown option", av[optind-1]);
      else
        printf ("%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", "Unknown option", 
                                                         optopt, optind, optarg, optidx);
      exit (1);
      
    case _id_( help ): case 'h':
      printf ("%s\n", "The help. :-|");
      break;
    
    case 0: // there is no value stored on long_opts[optind].val, check *(long_opts[optind].flag)
      printf ("%s --%s\n", "Not enumerated", long_opts[optidx].name);
      break;

    case 1:  // iterate through all arguments. Specialized case not automatically handled
      printf ("%s %s\n", "Should iterate", optarg);
      break;
      
    default:
      if (opt > _id_( _lim_inf ) && opt < _id_( _lim_sup ))
        printf ("%s --%s\n", "Unhandled option", long_opts[optidx].name);
      else if (isascii(opt))
        printf (isgraph(opt) ? "%s -%c\n" : "%s (%d)\n", "Unhandled option", opt);
      else
        printf ("%s <%s> ('%s','%s',%d): %d,%d\n", "Unhandled option", av[optind-1],
                                                   __FILE__, __func__, __LINE__, opt, optidx);
    }
  }
  
  exit (0);
}
