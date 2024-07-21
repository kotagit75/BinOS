#include "env/cushion/cushion.hpp"
#include "env/cushionmodel.hpp"
#include "console.hpp"

int EC_log(ELogLevel level,const char *format,...){
  if (level > EC.loglevel) {
    return 0;
  }
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}