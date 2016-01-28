/*
CMPUT 379 Assignment 1
Chongyang YE
Student ID:1264608
Unix ID: cye2
*/
#ifndef MEMCHUNK_H
#define MEMCHUNK_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/user.h>
struct memchunk {
	void *start;
	unsigned long length;
	int RW;
};
void handler(int sig);
void initialHandler();
int get_mem_layout (struct memchunk *chunk_list, int size);
#endif
