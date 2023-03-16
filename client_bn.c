#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

int main()
{
    char buf[300];
    int offset = 200; /* TODO: try test something bigger than the limit */
    struct timespec start, end;

    FILE *fib = fopen("bn_fib_output", "w");
    FILE *fp_plot = fopen("bn_fib_plot", "w");
    if (!fib || !fp_plot) {
        perror("Failed to open file");
        exit(1);
    }

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    fprintf(fp_plot, "#fib(n) | user | kernel | kernel_to_user\n");

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        long long sz = read(fd, buf, 1);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        if (sz)
            printf("returned message was truncated!\n");
        fprintf(fib, "fib(%d) = %s\n", i, buf);

        long long kt = write(fd, buf, 1);
        long long k2u = write(fd, buf, 2);

        long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL +
                               (end.tv_nsec - start.tv_nsec);
        fprintf(fp_plot, "%-8d %-8lld %-8lld %-8lld\n", i, elapsed_ns, kt, k2u);
    }

    fclose(fib);
    fclose(fp_plot);
    close(fd);
    return 0;
}
