#include "statistics.h"
#include <math.h>
#include <stdio.h>

double calc_mean(const double *data, int n, int outlier)
{
    int i;
    double sum = 0.0;
    for (i = 0; i < n; i++)
        sum += data[i];
    return sum / (n - outlier);
}

void calc_stdev(double *data, int n, stats_t *st)
{
    double mean, variance = 0.0;
    int i;

    mean = calc_mean(data, n, 0);
    st->mean = mean;

    for (i = 0; i < n; i++)
        variance += pow(data[i] - mean, 2);

    variance /= n;
    st->variance = variance;
    st->stdev = sqrt(variance);
}

double data_processing(double *data, int n, int threshold)
{
    int i, outlier = 0;
    stats_t st;
    calc_stdev(data, n, &st);

    for (i = 0; i < n; i++) {
        if (fabs(data[i] - st.mean) / st.stdev > threshold) {
            data[i] = 0.0;
            outlier++;
        }
    }
    return calc_mean(data, n, outlier);
}
