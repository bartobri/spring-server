// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include "config.h"
#include "modules/log.h"

#define SERVER       1
#define CLIENT       2
#define ERRMSG_SIZE  200

// Static Variables
static FILE *logFile;
static char errmsg[ERRMSG_SIZE];

void log_init(void) {
	memset(errmsg, 0, ERRMSG_SIZE);
	logFile = NULL;
}

int log_open_server(void) {
	return log_open(SERVER);
}

int log_open_client(void) {
	return log_open(CLIENT);
}

int log_open(int logType) {
	int i;
	char *homeDir                  = getenv("HOME");
	char *logFilePath              = LOG_FILE_PATH;
	char *logFileName;
	char *logFilePathName;
	char *logFileFullPathName;
	struct stat sb;
	
	// Die if we don't have a home directory
	if (homeDir == NULL) {
		sprintf(errmsg, "Unable to read HOME environmental variable.");
		return -1;
	}
	
	// Do we use server of client log name?
	if (logType == SERVER) {
		logFileName = malloc(strlen(SERVER_LOG_NAME) + 1);
		strcpy(logFileName, SERVER_LOG_NAME);
	} else {
		logFileName = malloc(strlen(CLIENT_LOG_NAME) + 1);
		strcpy(logFileName, CLIENT_LOG_NAME);
	}
	
	// Combine log file path and name in to one string.
	logFilePathName = malloc(strlen(logFilePath) + strlen(logFileName) + 1);
	sprintf(logFilePathName, "%s%s", logFilePath, logFileName);
	
	// Build full log file path
	logFileFullPathName = malloc(strlen(logFilePathName) + strlen(homeDir) + 2);
	sprintf(logFileFullPathName, "%s/%s", homeDir, logFilePathName);
	
	// Open log file, auto-create or die if can't open, die if can't auto-create.
	if ((logFile = fopen(logFileFullPathName, "a")) == NULL) {

		// Can't open file. Let's check and create necessary subdirs.
		if (strchr(logFileFullPathName, '/') != NULL) {
			for (i = 0; logFileFullPathName[i] != '\0'; ++i) {
				if (i == 0)
					continue;
				if (logFileFullPathName[i] == '/') {
					logFileFullPathName[i] = '\0';
					if (stat(logFileFullPathName, &sb) != 0) {
						if (mkdir(logFileFullPathName, 0700) == -1) {
							sprintf(errmsg, "Unable to create log file directory (%s).", logFileFullPathName);
							free(logFileName);
							free(logFilePathName);
							free(logFileFullPathName);
							return -1;
						}
					}
					logFileFullPathName[i] = '/';
				}
			}
		}
	
		if ((logFile = fopen(logFileFullPathName, "a")) == NULL) {
			sprintf(errmsg, "Unable to open or create log file (%s). Please ensure directory and/or file are writable.", logFileFullPathName);
			free(logFileName);
			free(logFilePathName);
			free(logFileFullPathName);
			return -1;
		}
	}
	
	return 0;
}

void log_write(char *format, ...) {
	va_list argList;
	time_t timeNow;
	struct tm *timePtr;
	char timeString[20];

	timeNow = time(NULL);
	timePtr = localtime(&timeNow);
	strftime(timeString, 20, "%Y-%m-%d %H:%M:%S", timePtr);
	fprintf(logFile, "\n[%s] ", timeString);
	
	va_start(argList, format);
	vfprintf(logFile, format, argList);
	va_end(argList);
	
	fflush(logFile);
}

void log_print(char *format, ...) {
	va_list argList;
	time_t timeNow;
	struct tm *timePtr;
	char timeString[20];

	timeNow = time(NULL);
	timePtr = localtime(&timeNow);
	strftime(timeString, 20, "%Y-%m-%d %H:%M:%S", timePtr);
	fprintf(logFile, "\n[%s] ", timeString);
	
	va_start(argList, format);
	vfprintf(logFile, format, argList);
	va_end(argList);
	
	fflush(logFile);
	
	va_start(argList, format);
	vprintf(format, argList);
	va_end(argList);
	
	printf("\n");
}

void log_close(void) {
	if (logFile != NULL)
		fclose(logFile);
}

char *log_get_errmsg(void) {
	return errmsg;
}

