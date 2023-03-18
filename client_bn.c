#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "statistics.h"

#define FIB_DEV "/dev/fibonacci"
#define N 50

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

    double ut_samp[N], kt_samp[N], k2u_samp[N];
    double ut, kt, k2u;
    int threshold = 2;

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        for (int j = 0; j < N; j++) {
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
            long long sz = read(fd, buf, 1);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
            if (sz) {
                perror("returned message was truncated!\n");
                exit(1);
            }

            ut_samp[j] = (double) ((end.tv_sec - start.tv_sec) * 1000000000LL +
                                   (end.tv_nsec - start.tv_nsec));
            kt_samp[j] = (double) write(fd, buf, 1);
            k2u_samp[j] = (double) write(fd, buf, 2);
        }

        ut = data_processing(ut_samp, N, threshold);
        kt = data_processing(kt_samp, N, threshold);
        k2u = data_processing(k2u_samp, N, threshold);

        fprintf(fib, "fib(%d) = %s\n", i, buf);
        fprintf(fp_plot, "%-8d %-20f %-20f %-20f\n", i, ut, kt, k2u);
    }

    fclose(fib);
    fclose(fp_plot);
    close(fd);
    return 0;
}
