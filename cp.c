#include <stdlib.h>
#include <string.h>
#include "error.h"
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
    {
    error_malloc( errno );
    exit( EXIT_FAILURE );
    }

strcpy( result, source );
return result;
}
