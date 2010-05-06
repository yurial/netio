#include <stdlib.h>
#include <string.h>
#include "cp.h"

char* cp(char* source)
{
size_t lenght;
char* result;

if ( !source )
    return;

lenght = strlen( source );
result = (char*)malloc( lenght );
if ( result == NULL )
    return;

strcpy( result, source );
return result;
}
