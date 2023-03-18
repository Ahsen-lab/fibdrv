typedef struct {
    double mean;
    double stdev;
    double variance;
} stats_t;

double calc_mean(const double *data, int n, int outlier);
void calc_stats(double *data, int n, stats_t *st);
double data_processing(double *data, int n, int threshold);
