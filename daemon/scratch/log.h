

#define DEF_LOG_PERMS 0777

int log_open(char *file, size_t numlines, size_t linesize);
int log_write(char *str);
