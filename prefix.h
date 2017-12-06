#ifndef _PREFIX_H
#define _PREFIX_H

#define GLUE(X,Y) X##_##Y
#define JOIN(X,Y) GLUE(X,Y)
#define PREFIX(X) JOIN(FUNCPREFIX,X)

#endif
