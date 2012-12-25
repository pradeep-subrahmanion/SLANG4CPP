#ifndef JS_RUN_INCLUDED
#define JS_RUN_INCLUDED

#include "external/include/v8.h"
#include <iostream>

using namespace v8;
using namespace std;

namespace JS_Run {

void run_js(const char *file_path);

};

#endif
