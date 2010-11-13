#include <stdio.h>

#include "debug.h"

#include "params.h"

void debug_print()
{
int index;
fprintf( stderr, "params:\n" );
/*target*/
for (index = 0; index < p_targetc; ++index)
    {
    fprintf( stderr, "target %d: %s\n", index, p_targetv[index] );
    }
/*server*/
fprintf( stderr, "p_server: %d\n", p_server );
/*once*/
fprintf( stderr, "p_once: %d\n", p_once );
/*conqueue*/
fprintf( stderr, "p_connqueue: %d\n", p_connqueue );
/*cmd*/
fprintf( stderr, "p_cmd: %s\n", p_cmd );
/*incmd*/
fprintf( stderr, "p_incmd: %s\n", p_incmd );
/*inmode*/
fprintf( stderr, "p_inmode: %d\n", p_inmode );
/*outcmd*/
fprintf( stderr, "p_outcmd: %s\n", p_outcmd );
/*outmode*/
fprintf( stderr, "p_outmode: %d\n", p_outmode );
/*oicmd*/
fprintf( stderr, "p_iocmd: %s\n", p_iocmd );
/*iomode*/
fprintf( stderr, "p_iomode: %d\n", p_iomode );
/*chldterm*/
fprintf( stderr, "p_chldterm: %d\n", p_chldterm );
/*wait*/
fprintf( stderr, "p_wait: %d.%d\n", (int)p_wait.tv_sec, (int)p_wait.tv_usec );
/*sync*/
fprintf( stderr, "p_sync: %d\n", p_sync );
/*recvbuff*/
fprintf( stderr, "p_recvbuff: %d\n", p_recvbuff );
/*sendbuff*/
fprintf( stderr, "p_sendbuff: %d\n", p_sendbuff );
}

