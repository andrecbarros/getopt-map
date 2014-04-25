/* getopt-map.h - v0.1 - 2014.04.22 12:31:30 GMT-3
 * License: BSD-3 clauses,  MIT or GPL2+ (any of them for derivatives)
 * Andre C. Barros - <andre.cbarros@yahoo.com>
 *
 * Improve the useability of getopt_long by appliying
 * the c pre-processor stringification, id-ification
 * and enum automatic serialization.
 * 
 * >> Obs1: avoid using '_' on the start of your long options to
 *          avoid identifier/symbol clash (collision).
 *
 * >> Obs2: hidden options are possible (see below). It is a
 *          useful feature for when the code that deal with
 *          them is not ready yet.
 *
 * To make it work you must define the following structure on your
 * module with main function (program start point):
 *
 * #include <getopt-map.h>
 *
 * enum option_id {
 *     _id_default_header_
 *                     //-->> chr and str options <- no ' nor ".
 *     _id_( str_j1 ), //    Long option strings j1 to jn id-ification.
 *     ..              //
 *     _id_( str_jn ), //
 *
 *     _id_( chr_i1 ), //    Short option chars i1 to im id-ification
 *     ..              //    (not really needed but useful on special
 *     _id_( chr_im ), //    cases).
 *
 *     _id_default_footer_
 * };
 * 
 * // Only long options here
 * //
 * struct option app_opts[] = {
 *     _opt_default_header_        //-->> str <- no ' nor ".
 *
 *     _opt_( str_j1, is_needed ), //     is_needed must be <no>,
 *     ..                          //     <required> or <optional>.
 *     _opt_( str_jn, is_needed ), //     Optionally, you can use
 *                                 //     0, 1 and 2, respectively.
 *     _opt_default_footer_
 * };
 *
 * // The order correspond to what should be listed first.
 * // Hidden arguments must have { str, chr, 0 } structure (but not
 * // be all zeroed). { 0, 0, msg } can be used as continuation line.
 * //
 * struct option_map app_opts_maps[] = {
 *     _opt_map_default_header_          //-->> str <- no ' nor "; chr <- enclosed in ''.
 *                                       //-->> msg <- enclosed in "".
 *     _opt_map_( _zero, chr_i1, msg ),  //     Short option chars i1 to im with no long
 *     ..                                //     equivalents.
 *     _opt_map_( _zero, chr_im, msg ),  //
 *     ..
 *     _opt_map_( str_k1, chr_k1, msg ), //     Short option chars and long option strings
 *     ..                                //     k1 to kl equivalence.
 *     _opt_map_( str_kl, chr_kl, msg ), //
 *
 *     _opt_map_( str_j1, 0, msg ),      //     Long option strings j1 to jn with no short
 *     ..                                //     equivalents.
 *     _opt_map_( str_jn, 0, msg ),      //
 *
 *     _opt_map_default_footer_
 * };
 *
 */

#ifndef _GETOPT_MAP_H
#define _GETOPT_MAP_H

#include <getopt.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _to_str_(x...)       #x
#define _stringify_(x...)    _to_str_(x)
#define _id_(x)              _om_##x
#define _id2_(x,y)           _id_(x) = y
#define _idm_(x)             _om_##x##_ = _id_(x)

enum option_map_id {
    _id2_( _zero, 0 ),
    _id2_( _lim_inf, 1000 ),    // No overlap with regular chars (see getopt_long return values)
    _id2_( _lim_sup, 2098152 ), // Arbitrary but will work for all cases as it is a ceil on all 
                                // systems I know and we are counting args and not bytes (ARG_MAX)
    _id_( _app_header ),
    _id_( _app_footer ),
    _id_( _app_error ),
    _id_( _app_support ),
    
    _id_( _opt_missing ),
    _id_( _opt_invalid ),
    
    _id_( _arg_obligatory ),
    _id_( _arg_optional ),
    _id_( _arg_missing ),
    _id_( _arg_invalid )
};

#define _id_default_header_  _idm_( _zero ),           \
                             _idm_( _lim_inf ),
#define _id_default_footer_  _idm_( _lim_sup ),        \
                             _idm_( _app_header ),     \
                             _idm_( _app_footer ),     \
                             _idm_( _app_error ),      \
                             _idm_( _app_support ),    \
                                                       \
                             _idm_( _opt_missing ),    \
                             _idm_( _opt_invalid ),    \
                                                       \
                             _idm_( _arg_obligatory ), \
                             _idm_( _arg_optional ),   \
                             _idm_( _arg_missing ),    \
                             _idm_( _arg_invalid )

#ifndef no_argument
#define no_argument 0
#endif
#ifndef required_argument
#define required_argument 1
#endif
#ifndef optional_argument
#define optional_argument 2
#endif
#define _0_argument          no_argument
#define _1_argument          required_argument
#define _2_argument          optional_argument
#define _no_argument         no_argument
#define _required_argument   required_argument
#define _optional_argument   optional_argument

#define _opt_(x,is_needed)   { _stringify_(x), _##is_needed##_argument, 0, _id_(x) }
#define _opt_zero_           {0, 0, 0, 0}  // bound mark sentinel - obligatory
#define _opt_default_header_
#define _opt_default_footer_ _opt_zero_

/** Mapped options **
 */
struct option_map {
    int  id;
    char ch;       // Mapping id <-> ch
#ifdef GETOPT_FILE_TRANSLATIONS
    char *sid;     // Stringified id
#endif
    char *msg;
};
#ifdef GETOPT_FILE_TRANSLATIONS
#define _opt_map_(x,y,arg)       { _id_(x), y, _stringify_( _id_(x) ), arg }
#define _opt_map_zero_           {0, 0, 0, 0}  // bound mark sentinel - obligatory
#else
#define _opt_map_(x,y,arg)       { _id_(x), y, arg }
#define _opt_map_zero_           {0, 0, 0}
#endif
#define _opt_map_default_header_
#define _opt_map_default_footer_ _opt_map_( _lim_sup, 0, "" ),                                        \
                                 _opt_map_( _app_header, 0, "Usage: %s [options] <file> ([*] = optional, <*> = obligatory)\nWhere [options] are the following:\n"), \
                                 _opt_map_( _app_footer, 0, "See the application manual for a more detailed analyse of usage.\n"), \
                                 _opt_map_( _app_error, 0, "App internal error"),                     \
                                 _opt_map_( _app_support, 0, "Please, file a ticket for support.\n"), \
                                                                                                      \
                                 _opt_map_( _opt_missing, 0, "Missing obligatory option"),            \
                                 _opt_map_( _opt_invalid, 0, "Invalid option"),                       \
                                                                                                      \
                                 _opt_map_( _arg_obligatory, 0, "<value>"),                           \
                                 _opt_map_( _arg_optional, 0, "[value]"),                             \
                                 _opt_map_( _arg_missing, 0, "Missing argument to"),                  \
                                 _opt_map_( _arg_invalid, 0, "Invalid argument to"),                  \
                                 _opt_map_zero_

extern struct option opt_zero;
extern struct option_map opt_map_zero;

struct option *     option_p (struct option *opts, int id);
struct option_map * option_map_p (struct option_map *maps, int id);

int    getopt_map (struct option_map *maps, int id);
char * getopt_msg (struct option_map *maps, int id);
void   getopt_usage (char *app_name, char *app_version, char *app_license,
                     char *short_opts, struct option *long_opts,
                     struct option_map *opts_maps, int exit_val);

#ifdef GETOPT_FILE_TRANSLATIONS
int    getopt_map_write (FILE *f, struct option_map *msgs);
int    getopt_map_read  (FILE *f, struct option_map *msgs);
#endif

#ifdef __cplusplus
}
#endif

#endif
