#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

/* Whether logging functions are enabled
 * !! TURN OFF FOR DEPLOYMENT !!
 */
static int log_enabled = false;

/**
 * Change whether log will print to console
 * !!TURN OFF FOR DEPLOYMENT!!
 * @param enabled sets the mode of the logger
 * @return if log is enabled
 */
bool log_enable(bool enabled) {
    log_enabled = enabled;
    return log_enabled;
}

/**
 * Formats an int to a specified string length so 1 with a length of 4 would be 0001
 * @param stream Output stream
 * @param val The int to be formatted
 * @param min_length The minimum string length
 */
void print_format_int(struct _iobuf *stream, int val, int min_length) {
    for (int i = 1; i < min_length; i++) {
        if (val < pow(10, i)) { //when i = 1 val < 10
            fprintf(stream, "%d", 0);
        }
    }
    fprintf(stream, "%d", val);
}

/**
 * Prints the local time formatted '[h:m:s] '
 * @param stream Output stream
 */
void print_time(struct _iobuf *stream) {
    time_t _time = time(NULL);
    struct tm * local_time = localtime(&_time);
    fprintf(stream, "[");
    print_format_int(stdout, local_time->tm_hour, 2);
    fprintf(stream, ":");
    print_format_int(stdout, local_time->tm_min, 2);
    fprintf(stream, ":");
    print_format_int(stdout, local_time->tm_sec, 2);
    fprintf(stream, "] ");
}

/**
 * Logs info to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return
 */
int log_info(const char *format, ...) {
    if (!log_enabled) return -1;

    fprintf(stdout, "[INFO]");
    print_time(stdout);

    va_list argument_pointer;
    va_start(argument_pointer, format);
    int retVal = vfprintf(stdout, format, argument_pointer);
    va_end(argument_pointer);

    fprintf(stdout, "\n");
    return retVal;
}

/**
 * Logs errors to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return
 */
int log_error(char *format, ...) {
    if (!log_enabled) return -1;
    fprintf(stdout, "[ERROR]");
    print_time(stdout);

    va_list argument_pointer;
    va_start(argument_pointer, format);
    int retVal = vfprintf(stdout, format, argument_pointer);
    va_end(argument_pointer);

    fprintf(stdout, "\n");
    return retVal;
}

/**
 * Returns bool depending on val being null, error logs if it is
 * @param file Current file function is called from
 * @param line Current line in file
 * @param val Input value of any type to check
 * @return bool depending on whether val is NULL
 */
bool catch_null_pointer(char* file, int line, void* val) {
    if (val == NULL) {
        log_error("Null Pointer at %s, %d", file, line);
        return true;
    }

    return false;
}