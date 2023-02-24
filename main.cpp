#include <sys/socket.h>
#include "logging.h"

int main(int ac, char** av)
{
    auto sock = socket(PF_INET, SOCK_STREAM);
    if (sock == -1)
    {
        bravura_log_err("Failed to open server socket");
    }
    
    return 0;
}
