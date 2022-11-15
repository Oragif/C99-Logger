# Logger - C99

## Logging functions
### Info logging
Logs under the info tag\
Output: [INFO]  [12:30:00] Text
```
log_info(const char *message, ...)
```
### Error logging

Logs under the error tag\
Output: [Error] [12:30:00] Text
```
log_error(char *format, ...)
```

### Warn logging
Logs under the warn tag\
Output: [WARN]  [12:30:00] Text
```
log_warn(const char *message, ...)
```
## Utility functions
### Catch NULL
Checks if any val is NULL and logs error if NULL
```
catch_null(void* val)
```