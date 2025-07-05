#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static double total = 1024;  /* MB */
static double speed = 1;     /* MB */

#define PROGRESS_BAR_BUFFER_SIZE 101
#define PROGRESS_BAR_STYLE '#'

void download();
void progressBar(double current , double end);