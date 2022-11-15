# C99-Logger

### Logging functions
```
Logs under the info tag
log_info(const char *message, ...)

Logs under the error tag
log_error(char *format, ...)
```
### Utility functions
```
Checks if a any val is null and throws error if so
catch_null(void* val)
```