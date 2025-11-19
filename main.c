#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <errno.h>

/*
 * csv_stats.c
 *
 * A small C command-line tool that reads a CSV file and computes basic
 * statistics (count, min, max, mean, standard deviation) for a numeric column.
 *
 * Usage:
 *   ./csv_stats data.csv 3
 *     -> reads data.csv, uses column index 3 (1-based), prints stats
 *
 * If no arguments are provided, it defaults to:
 *   file: sample.csv
 *   column: 1
 */

#define LINE_BUFFER_SIZE 4096

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <csv-file> <column-index (1-based)>\n", prog);
    fprintf(stderr, "Example: %s sample.csv 2\n", prog);
}

int main(int argc, char *argv[]) {
    const char *filename = "sample.csv";
    int column_index = 1; /* 1-based index */

    if (argc >= 2) {
        filename = argv[1];
    }
    if (argc >= 3) {
        column_index = atoi(argv[2]);
        if (column_index <= 0) {
            fprintf(stderr, "Invalid column index: %s\n", argv[2]);
            usage(argv[0]);
            return 1;
        }
    }

    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Failed to open file");
        return 1;
    }

    char line[LINE_BUFFER_SIZE];
    int line_number = 0;
    int header_skipped = 0;

    long long count = 0;
    double sum = 0.0;
    double sumsq = 0.0;
    double min = DBL_MAX;
    double max = -DBL_MAX;

    while (fgets(line, sizeof(line), f)) {
        line_number++;

        /* Skip empty lines */
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') {
            continue;
        }

        /* Assuming first non-empty line is header row; skip it */
        if (!header_skipped) {
            header_skipped = 1;
            continue;
        }

        /* Strip trailing newline characters */
        line[strcspn(line, "\r\n")] = '\0';

        /* Tokenize by comma to get the desired column */
        char *tmp = strdup(line);
        if (!tmp) {
            fprintf(stderr, "Memory allocation error\n");
            fclose(f);
            return 1;
        }

        char *saveptr = NULL;
        char *token = NULL;
        int col = 0;
        char *target = NULL;

        for (token = strtok_r(tmp, ",", &saveptr);
             token != NULL;
             token = strtok_r(NULL, ",", &saveptr)) {

            col++;
            if (col == column_index) {
                target = token;
                break;
            }
        }

        if (!target) {
            fprintf(stderr, "Warning: line %d has fewer than %d columns, skipping\n",
                    line_number, column_index);
            free(tmp);
            continue;
        }

        /* Convert target to double */
        errno = 0;
        char *endptr = NULL;
        double value = strtod(target, endptr ? &endptr : NULL);

        if (errno != 0 || (endptr && *endptr != '\0')) {
            /* Not a clean numeric conversion */
            fprintf(stderr, "Warning: line %d, column %d is not numeric ('%s'), skipping\n",
                    line_number, column_index, target);
            free(tmp);
            continue;
        }

        /* Update stats */
        count++;
        sum += value;
        sumsq += value * value;
        if (value < min) min = value;
        if (value > max) max = value;

        free(tmp);
    }

    fclose(f);

    if (count == 0) {
        printf("No valid numeric values found in column %d of '%s'.\n",
               column_index, filename);
        return 0;
    }

    double mean = sum / (double)count;
    double variance = 0.0;
    if (count > 1) {
        variance = (sumsq - (sum * sum) / (double)count) / (double)(count - 1);
    }
    double stddev = (variance > 0.0) ? sqrt(variance) : 0.0;

    printf("CSV statistics for file '%s', column %d\n", filename, column_index);
    printf("--------------------------------------------------\n");
    printf("Count:   %lld\n", count);
    printf("Min:     %.6f\n", min);
    printf("Max:     %.6f\n", max);
    printf("Mean:    %.6f\n", mean);
    printf("Std dev: %.6f\n", stddev);

    return 0;
}
