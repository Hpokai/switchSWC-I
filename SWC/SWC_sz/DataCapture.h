#ifndef DATACAPTURE_H
#define DATACAPTURE_H

#include <pthread.h>
#include "Definition.h"
#include "Global.h"

void *DataCapture ( void *threadid );
char ParaBaudrate(int);
char ParaEqData(int);

void ParseData(int);

#endif

