#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H
#include <stdio.h>
typedef struct _iobuf *STREAM;

//Contains all supported types of arrays for the log_array function
typedef enum {
    /*If int* array[0] != 1 or 0, output will be wrong.
     If any value is not 1 or 0 it will be displayed as it's int value.
     Also supports bool* (_Bool*). */
    A_BOOL,
    A_SHORT,
    A_INT,
    A_LONG,
    A_FLOAT,
    A_DOUBLE
} array_type;

/* ====================================================================
 *                        Settings & file utility
 * ====================================================================
 */

int log_enable(int enabled);
int log_to_file(int enabled);

void update_log_path(char* name, char* dir);
void log_start(int to_console, int to_file, int date_file);

/* ====================================================================
 *                              Utility
 * ====================================================================
 */
int catch_null_pointer(char* path, int line, char* name, void* val);

/**
 * Returns bool depending on val being null, error logs if it is
 * @param val Input value of any type to check
 */
#define catch_null(val) catch_null_pointer(__FILE__, __LINE__, #val, val)

void print_format_int(STREAM stream, int val, int min_length);
void print_time(STREAM stream);

/* ====================================================================
 *                               Logger
 * ====================================================================
 */

int logger(STREAM stream, char* file_name, int line, char* type, const char *format, ...);

/**
 * Logs info to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_info(format, ...) logger(stdout, __FILE__, __LINE__, "INFO", format, ##__VA_ARGS__)

/**
 * Logs error to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_error(format, ...) logger(stdout, __FILE__, __LINE__, "ERROR", format, ##__VA_ARGS__)

/**
 * Logs warn to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_warn(format, ...) logger(stdout, __FILE__, __LINE__, "WARN", format, ##__VA_ARGS__)

/**
 * Logs with custom tag to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_custom(name, format, ...) logger(stdout, __FILE__, __LINE__, name, format, ##__VA_ARGS__)

#define size_of_array(array) (sizeof(array) / sizeof(array)[0])

int logger_array(STREAM stream, char* file_name, int line, char* name, void* array, int size, array_type type);
/**
 * Logs an array to console/file if enabled
 * @param arr the array see array_type for supported types
 * @param type use array_type\n
 * A_SHORT, A_INT, A_LONG, A_FLOAT, A_DOUBLE, A_BOOL
 */
#define log_array(arr, type) logger_array(stdout, __FILE__, __LINE__, #arr, arr, size_of_array(arr), type)
/**
 * Logs a pointer array to console/file if enabled
 * @param arr the array see array_type for supported types
 * @param size of the array
 * @param type use array_type\n
 * A_SHORT, A_INT, A_LONG, A_FLOAT, A_DOUBLE, A_BOOL
 */
#define log_pointer_array(arr, size, type) if(!catch_null(arr)) logger_array(stdout, __FILE__, __LINE__, #arr, arr, size, type)
#endif //LOGGER_LOGGER_H