#include <string.h>
#include "cp.h"

char* cp(char* source)
{
int length;
char* result;

if ( !source )
    return;

length = strlen( source );
result = malloc( length );
if ( result == NULL )
    return;

strcpy( result, source );
return result;
}
