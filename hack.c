#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

typedef void FILE;

static FILE * MAGIC_RESOLV_CONF = (FILE*)0xbabaca00babaca00;

void print(const char *s) {
	write(1, s, strlen(s));
}

FILE *fopen(const char *pathname, const char *mode) {
  static FILE *(*_fopen)(const char *pathname, const char *mode) = NULL;
  if (!_fopen) {
    _fopen = dlsym(RTLD_NEXT, "fopen");
  }
  if (strcmp(pathname, "/etc/resolv.conf") == 0 && strcmp(mode, "w") == 0) {
    print("Attempt to open resolv.conf, returning magic pointer\n");
    return MAGIC_RESOLV_CONF;
  }
  return _fopen(pathname, mode);
}

int fputc(int c, FILE *stream) {
  static int (*_fputc)(int c, FILE *stream) = NULL;
  if (!_fputc) {
    _fputc = dlsym(RTLD_NEXT, "fputc");
  }
  if (stream == MAGIC_RESOLV_CONF) {
    char s[2] = { c, 0 };
    print(s);
    return c;
  }
  return _fputc(c, stream);
}

int fclose(FILE *stream) {
  static int (*_fclose)(FILE *stream) = NULL;
  if (!_fclose) {
    _fclose = dlsym(RTLD_NEXT, "fclose");
  }
  if (stream == MAGIC_RESOLV_CONF) {
    print("Attempt to close resolv.conf, returning success\n");
    return 0;
  }
  return _fclose(stream);
}

// vim:sw=2:et:sta
