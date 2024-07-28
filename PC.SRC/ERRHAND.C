/************************* Start of ERRHAND.C ************************
 *
 * This is a general purpose error handler used with every program in
 * the book.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <cstdarg>
//#include "errhand.h"

void fatal_error(char* fmt,...)
{
    va_list argptr;

    va_start( argptr, fmt );
    printf( "Fatal error: " );
    vprintf( fmt, argptr );
    va_end( argptr );
    exit( -1 );
}

/************************** End of ERRHAND.C *************************/

