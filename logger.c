#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <io.h>
#include <libgen.h>

//============== Logger Settings ===============
//Whether logging functions are enabled
static int log_enabled = false;
//Whether logging to file functions are enabled
static int log_file_enabled = false;
//Log file location
static char* log_file_name = "newest_log.txt";
static char* log_file_dir = "log";
static char log_file_path[100];
//==============================================

int log_enable(int enabled) {
    log_enabled = enabled;
    return log_enabled;
}

int log_to_file(int enabled) {
    log_file_enabled = enabled;
    return log_file_enabled;
}

/**
 * Formats an int to a specified string length so 1 with a length of 4 would be 0001
 * @param stream Output stream
 * @param val The int to be formatted
 * @param min_length The minimum string length
 */
void print_format_int(STREAM stream, int val, int min_length) {
    int check_val = 10;
    for (int i = 1; i < min_length; i++) {
        if (val < check_val)
            fprintf(stream, "%d", 0);
        else
            break;
        check_val *= 10;
    }
    fprintf(stream, "%d", val);
}

/**
 * Prints the local time formatted 'h:m:s'
 * @param stream Output stream
 */
void print_time(STREAM stream) {
    time_t _time = time(NULL);
    struct tm * local_time = localtime(&_time);
    print_format_int(stream, local_time->tm_hour, 2);
    fprintf(stream, ":");
    print_format_int(stream, local_time->tm_min, 2);
    fprintf(stream, ":");
    print_format_int(stream, local_time->tm_sec, 2);
}

/**
 * Changes pointer of path to the filename instead of full path
 * @param path of file
 */
void get_base_name(char **path) {
    char* dup_path = strdup(*path);
    *path = basename(dup_path);
}

/**
 * Print info '[hh:mm:ss] [TAG/File:Line]'
 * @param stream output stream
 * @param path of file, function is called from
 * @param line of file, function is called from
 * @param tag to print under
 */
void fprint_info(STREAM stream, char* path, int line, char* tag) {
    fprintf(stream, "[");
    print_time(stream);
    //Get name of file log was called from
    get_base_name(&path);
    fprintf(stream, "] [%s/%s:%d] ", tag, path, line);
}

//Load file or explode
FILE* open_file(char* path, char* mode) {
    FILE* file = fopen(path, mode);
    if(file == NULL) {
        log_to_file(false);
        fprint_info(stdout, "logger.c", 118, "ERROR");
        fprintf(stdout, "File at %s couldn't be loaded\n", path);
        return NULL;
    }
    return file;
}

/**
 * Updates log file path, and creates a new empty file in path of ../dir/name.txt
 * @param name file name
 * @param dir directory to save log files to (Depth 1)
 */
void update_log_path(char* name, char* dir) {
    log_file_dir  = dir;
    log_file_name = name;

    strcpy(log_file_path, log_file_dir);
    strcat(log_file_path, "/");
    strcat(log_file_path, log_file_name);

    mkdir(log_file_dir);

    FILE* file = open_file(log_file_path, "w");
    if (file != NULL) {
        fprintf(file, "===== Log started at ");
        print_time(file);
        fprintf(file, " =====\n");
    }
    fclose(file);
}

/**
 * Enables parts or all of logger
 * @param to_console bool, whether to log to console
 * @param to_file bool, whether to save to log file
 * @param date_file bool, whether to create unique timestamped file
 */
void log_start(int to_console, int to_file, int date_file) {
    log_file_enabled = to_file;

    char name[100];
    strcpy(name, log_file_name);

    if (log_file_enabled == true) {
        //If dated files are enabled update name with time and date stamp
        if (date_file) {
            strcpy(name, "log-");
            time_t _time = time(NULL);
            char time[100];
            strftime(time, 28, "D-%d-%m-%Y-T-%H-%M-%S.txt", gmtime(&_time));
            strcat(name, time);
        }
        update_log_path(name, log_file_dir);
    }
    log_enable(to_console);
}

/**
 * !!! Should not be used use log_'type' instead !!!\n
 * Logs info to console if log_enable(bool) is set to true
 * @param stream output stream
 * @param file_name where the functions is called from
 * @param line where the functions is called from
 * @param tag to print under
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of vfprintf
 */
int logger(STREAM stream, char* file_name, int line, char* tag, const char *format, ...) {
    if (!log_enabled && !log_file_enabled) return -1;
    int retVal;

    //get argument pointers if any is parsed
    va_list argument_pointer;
    va_start(argument_pointer, format);
    //Log info to console if enabled
    if (log_enabled) {
        fprint_info(stream, file_name, line, tag);
        retVal = vfprintf(stream, format, argument_pointer);
        fprintf(stream, "\n");
    }
    //Append info to file if enabled
    if (log_file_enabled) {
        FILE *file = open_file(log_file_path, "a");
        if (file != NULL) {
            fprint_info(file, file_name, line, tag);
            retVal = vfprintf(file, format, argument_pointer);
            fprintf(file, "\n");
        }
        fclose(file);
    }
    va_end(argument_pointer);
    return retVal;
}

/**
 * !!! Shouldn't be used us catch_null() instead !!!\n
 * Returns bool depending on val being null, error logs if it is
 * @param path Current path function is called from
 * @param line Current line in path
 * @param name of the pointer
 * @param val Input value of any type to check
 * @return bool depending on whether val is NULL
 */
int catch_null_pointer(char* path, int line, char* name, void* val) {
    if (val == NULL) {
        //Get name of path log was called from
        get_base_name(&path);
        logger(stdout, path, line, "NULLPTR","Null Pointer in %s, line: %d, var: %s", path, line, name);
        return true;
    }
    return false;
}

/**
 * Prints an array to stream
 * @param stream output stream
 * @param name of array
 * @param length of array
 * @param format format for fprintf
 * @param array the array to print
 * @param p_func print function
 */
void print_array(STREAM stream, char* name, int length, void* array, void (*p_func)(STREAM, void*, int)) {
    if (catch_null(array)) return;
    fprintf(stream, "%s = {", name);
    p_func(stream, array, 0);
    for (int i = 1; i < length; ++i) {
        fprintf(stream, ", ");
        p_func(stream, array, i);
    }
    fprintf(stream, "}\n");
}

//============================== Array printer utility functions ==============================
void print_float_from_array(STREAM stream, void* array, int index){
    fprintf(stream, "%f", ((float*)array)[index]);
}

void print_double_from_array(STREAM stream, void* array, int index){
    fprintf(stream, "%lf", ((double*)array)[index]);
}

void print_short_from_array(STREAM stream, void* array, int index){
    fprintf(stream, "%hd", ((short*)array)[index]);
}

void print_int_from_array(STREAM stream, void* array, int index){
    fprintf(stream, "%d", ((int*)array)[index]);
}

void print_long_from_array(STREAM stream, void* array, int index){
    fprintf(stream, "%ld", ((long*)array)[index]);
}

void print_bool_from_array(STREAM stream, void* array, int index) {
    char *boolean = (((_Bool *) array)[index] == true) ? "True" :  "False";
    fprintf(stream, "%s", boolean);
}

void print_int_bool_from_array(STREAM stream, void* array, int index){
    int val = ((int*)array)[index];
    char* boolean = (val == 1) ? "True" : (val == 0) ? "False" : NULL;
    if (boolean != NULL)
        fprintf(stream, "%s", boolean);
    else
        fprintf(stream, "%d", val);
}
//============================= /Array printer utility functions/ =============================

/**
 * Selects which array printer function to use depending on array_type
 * @param stream output stream
 * @param name name of array
 * @param array array to print
 * @param size size of array
 * @param type of array_type
 * @return bool depending on success
 */
bool print_arr_selector(STREAM stream, char* name, void* array, int size, array_type type) {
    switch (type) {
        case A_BOOL:
            if (((int*)array)[0] > 1)
                print_array(stream, name, size, array, print_bool_from_array);
            else
                print_array(stream, name, size, array, print_int_bool_from_array);
            return true;
        case A_DOUBLE:
            print_array(stream, name, size, array, print_double_from_array);
            return true;
        case A_INT:
            print_array(stream, name, size, array, print_int_from_array);
            return true;
        case A_FLOAT:
            print_array(stream, name, size, array, print_float_from_array);
            return true;
        case A_LONG:
            print_array(stream, name, size, array, print_long_from_array);
            return true;
        case A_SHORT:
            print_array(stream, name, size, array, print_short_from_array);
            return true;
        default:
            return false;
    }
}

/**
 * !!! Should not be used, use log_array instead !!!\n
 * Logs array to console if log_enable(bool) is set to true
 * @param stream output stream
 * @param file_name where the functions is called from
 * @param line where the functions is called from
 * @param name of the array
 * @param array the array to print
 * @param size of the array
 * @param type of the array using array_type
 * @return true if successful otherwise false
 */
int logger_array(STREAM stream, char* file_name, int line, char* name, void* array, int size, array_type type) {
    if (!log_enabled && !log_file_enabled) return -1;
    int retVal;

    //Log info to console if enabled
    if (log_enabled) {
        fprint_info(stream, file_name, line, "ARRAY");
        retVal = print_arr_selector(stream, name, array, size, type);
    }

    //Append info to file if enabled
    if (log_file_enabled) {
        FILE *file = open_file(log_file_path, "a");
        if (file != NULL) {
            fprint_info(file, file_name, line, "ARRAY");
            retVal = print_arr_selector(file, name, array, size, type);
        }
        fclose(file);
    }

    return retVal;
}