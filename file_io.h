#pragma once
#include <stdio.h>
#include "student.h"

int save(Student *head, const char *fp);
int reload(Student **head, const char *fp);