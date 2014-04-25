getopt-map
==========

Improve the useability of getopt_long

--------------------------------------------------

This is a very small project that tries to improve
the useability of getopt_long by appliying the
c pre-processor stringification, id-ification
and enum automatic serialization.
 
Take a look on getopt-map-example.c to see how it works.

Of course, there are specialized cases that can not
be achieved by the techniques applied here but they
are probably of seldom use.

Compile with:

gcc -DVERSION=1.1 -DLICENSE=MIT -o example -I. getopt-map-example.c

After that try, for example:

./example --hidden=1 -H -g=1 -z --required
