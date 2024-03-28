#include <stdio.h>
#include <time.h>

int main() {
    time_t timestamp = 1681332720;
    struct tm *timeinfo;

    timeinfo = localtime(&timestamp);

    printf("Data e Hora: %s", asctime(timeinfo));

    return 0;
}
