#include "server.h"
#include "../logging.h"

int main(int ac, char** av)
{
    if (run_server())
    {
        return 1;
    }
    return 0;
}
