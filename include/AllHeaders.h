// AllHeaders.h: what the contest template includes under -DLOCAL. cprun adds -DLOCAL and this folder.
#pragma once
#include "debug.h"
#define _FASTIO_LOCAL() ;   // templates from before cprun call this inside fastIO; cprun times every run itself
