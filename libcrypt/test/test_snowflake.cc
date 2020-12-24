#include <stdio.h>
#include "crypt_snowflake.h"

int main()
{
    infra::crypt::Snowflake sf(NULL, "team0.1024",infra::crypt::Snowflake::Mode::SingleNode);
    for (int loop = 0; loop<10; loop++) {
    long id = sf.ID();
    printf("%lX\n", id);

    }
    return 0;
}
