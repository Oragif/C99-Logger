#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <io.h>
#include <libgen.h>

//Whether logging functions are enabled
static int log_enabled = false;

/**
 * Change whether log will print to console
 * !!TURN OFF FOR DEPLOYMENT!!
 * @param enabled sets the mode of the logger
 * @return if log is enabled
 */
int log_enable(int enabled) {
    log_enabled = enabled;
    return log_enabled;
}

//Whether logging to file functions are enabled
static int log_file_enabled = false;

/**
 * Change whether log will save to file
 * @param enabled sets the mode of the log to file
 * @return if log to file is enabled
 */
int log_to_file(int enabled) {
    log_file_enabled = enabled;
    return log_file_enabled;
}

//Log file location
static char* log_file_name = "newest_log.txt";
static char* log_file_dir = "log";
static char log_file_path[100];

/**
 * Sets the name of where to save log file
 * @param name of where to save
 * @return if log file is enabled
 */
int set_log_file_name(char* name) {
    log_file_name = name;
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

void fprint_info(STREAM stream, char* path, char* tag) {
    //Print current time
    fprintf(stream, "[");
    print_time(stream);
    //Get name of file log was called from
    char* dup_path = strdup(path);
    char* base_name = basename(dup_path);
    //Print rest of info
    fprintf(stream, "] [%s/%s] ", tag, base_name);
}

//Load file or explode
FILE* open_file(char* path, char* mode) {
    FILE* file = fopen(path, mode);
    if(file == NULL) {
        log_to_file(false);
        fprint_info(stdout, "logger.c", "[ERROR] ");
        fprintf(stdout, "File at %s couldn't be loaded\n", path);
        return NULL;
    }
    return file;
}

/**
 * Clears the currently used log file
 */
void clear_log_file() {
    FILE* file = open_file(log_file_path, "w");
    if (file != NULL) {
        fprintf(file, "===== Log started at ");
        print_time(file);
        fprintf(file, " =====\n");
    }
    fclose(file);
}

/**
 * Updates the path to log file, important to call if dir or name was changed
 */
void update_log_path() {
    strcpy(log_file_path, log_file_dir);
    strcat(log_file_path, "/");
    strcat(log_file_path, log_file_name);
}

/**
 * Enables parts or all of logger
 * @param to_console bool, whether to log to console
 * @param to_file bool, whether to save to log file
 * @param date_file bool, whether to create unique timestamped file
 */
void log_start(int to_console, int to_file, int date_file) {
    log_file_enabled = to_file;
    if (log_file_enabled == true) {
        //Try to create log folder
        mkdir("log");

        //If dated files are enabled update name with time and date stamp
        if (date_file) {
            char name[100] = "log-";
            time_t _time = time(NULL);
            char time[100];
            strftime(time, 28, "D-%d-%m-%Y-T-%H-%M-%S.txt", gmtime(&_time));
            strcat(name, time);
            set_log_file_name(name);
        }
        update_log_path();
        clear_log_file();
    }
    log_enable(to_console);
}

/**
 * Logs info to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of vfprintf
 */
int logger(STREAM stream, char* file_name, char* tag, const char *format, ...) {
    if (!log_enabled && !log_file_enabled) return -1;
    int retVal;

    //get argument pointers if any is parsed
    va_list argument_pointer;
    va_start(argument_pointer, format);

    //Log info to console if enabled
    if (log_enabled) {
        fprint_info(stream, file_name, tag);
        retVal = vfprintf(stream, format, argument_pointer);
        fprintf(stream, "\n");
    }

    //Append info to file if enabled
    if (log_file_enabled) {
        FILE *file = open_file(log_file_path, "a");
        if (file != NULL) {
            fprint_info(file, file_name, tag);
            retVal = vfprintf(file, format, argument_pointer);
            fprintf(file, "\n");
        }
        fclose(file);
    }

    va_end(argument_pointer);

    return retVal;
}


/**
 * Returns bool depending on val being null, error logs if it is
 * @param path Current path function is called from
 * @param line Current line in path
 * @param val Input value of any type to check
 * @return bool depending on whether val is NULL
 */
int catch_null_pointer(char* path, int line, void* val) {
    if (val == NULL) {
        //Get name of path log was called from
        char* dup_path = strdup(path);
        char* base_name = basename(dup_path);

        log_error("Null Pointer in %s, line %d", base_name, line);
        return true;
    }

    return false;
}