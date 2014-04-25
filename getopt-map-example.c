#include <getopt-map.h>
#include <stdlib.h>
#include <stdio.h>

enum option_id {
    _id_default_header_

    _id_( optional ),
    _id_( hidden ),
    _id_( required ),
    _id_( flag ),
    _id_( help ),
    
    _id_default_footer_
};

struct option long_opts[] = {
    _opt_default_header_
    
    _opt_( optional, optional),
    _opt_( hidden, optional),
    _opt_( required, required),
    _opt_( flag, no),
    _opt_( help, 0),
    
    _opt_default_footer_
};

struct option_map opts_maps[] = {
    _opt_map_default_header_
    
    _opt_map_( _zero, 'g', "Char only option with required argument"),
    _opt_map_( optional, 'o', "Optional argument to option"),
    _opt_map_( hidden, 'H', 0), // Not ready yet
    _opt_map_( required, 0, "Required argument to option"),
    _opt_map_( flag, 'F', "Takes no option"),
    _opt_map_( help, 'h', "Display this help and exit"),

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
                            
  opterr = 0; // No default error message
  while ((opt = getopt_long(ac, av, short_opts, long_opts, &optidx)) != -1) {
    const char *apperr = getopt_msg (opts_maps, _id_( _app_error ));
    const char *optmis = getopt_msg (opts_maps, _id_( _opt_missing ));
    const char *optinv = getopt_msg (opts_maps, _id_( _opt_invalid ));
    const char *argmis = getopt_msg (opts_maps, _id_( _arg_missing ));
    const char *arginv = getopt_msg (opts_maps, _id_( _arg_invalid ));

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
        printf ("%s -%c\n", argmis, optopt);
      else if (optind)
        printf ("%s %s\n", argmis, av[optind-1]);
      else
        printf ("%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", argmis, optopt, optind, optarg, optidx);
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 1);
      
    case '?':
      if (optopt && optopt < _id_( _lim_inf ))
        printf ("%s -%c\n", optinv, optopt);
      else if (optind)
        printf ("%s %s\n", optinv, av[optind-1]);
      else
        printf ("%s (opt,ind,arg,idx): %d,%d,'%s',%d\n", optinv, optopt, optind, optarg, optidx);
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 1);
      
    case _id_( help ): case 'h':
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 1);

    default:
      printf ("%s ('%s','%s',%d):%c\n", apperr, __FILE__, __func__, __LINE__, opt);
      getopt_usage (av[0], myapp_version, myapp_license, short_opts, long_opts, opts_maps, 0);
    }
  }
  
  exit (0);
}
