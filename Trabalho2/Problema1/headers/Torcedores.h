#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "utils.h"

#include "MonitorBanheiro.h"

void* thread_flamenguista(void* id);
void* thread_vascaino (void* id);