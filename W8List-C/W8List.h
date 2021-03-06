//
//  main.h
//  W8List
//
//  Created by Matthew D'Amore on 9/20/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#ifndef W8List_h
#define W8List_h

// Preprocesser code to link up an enumeration for the string tokens
#define FOREACH_SEMESTER(SEMESTER_TOKEN) \
SEMESTER_TOKEN(ZH4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA)   \
SEMESTER_TOKEN(ZH4sIAAAAAAAAAFvzloG1uIhBPjWlVC%2BlKLUiNUcvs6hErzw1qSS3WC8lsSRRLyS1KJcBAhiZGJh9GNgTk0tCMnNTSxhEfLISyxL1iwtz9EECxSWJuQXWPgwcJUAtzvkpQBVCEBU5iXnp%2BsElRZl56TB5l9Ti5EKGOgamioKCEgY2IwNDMyNToJHhmXlAaYXA0sQiEG1opmtoDgAb98cdpgAAAA%3D%3D)  \
SEMESTER_TOKEN(ZH4sIAAAAAAAAAFvzloG1uIhBPjWlVC%2BlKLUiNUcvs6hErzw1qSS3WC8lsSRRLyS1KJcBAhiZGJh9GNgTk0tCMnNTSxhEfLISyxL1iwtz9EECxSWJuQXWPgwcJUAtzvkpQBVCEBU5iXnp%2BsElRZl56TB5l9Ti5EKGOgamioKCEgY2IwNDU2NToJHBBSBVCoGliUVAZQqGprqGZgAfTc62pgAAArA)   \
SEMESTER_TOKEN(ZH4sIAAAAAAAAADWMPQ6CQBCFR4w%2FtbGXC7iAERtLLWkMXGBkJ2QNizA7KJUn8mrewTXEV37ve%2B%2F9gZlj2JDulWYaqFaGRT3pKtYpjYKqILYwZhLANIMFllIYSwLr7IYPjFxXRz%2FgBG17zGApfnK6a2%2BsRqPGpopyYdNU%2F%2F5MruzgBcHQtgLzXZyk%2B9Rf5r21xOGlR%2FZamKTb5PAFy%2Bn%2BwKYAAAA)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum SEMESTER_ENUM {
    FALL_2017_TOKEN = 0,
    WINTER_2017_TOKEN,
    SPRING_2017_TOKEN,
    SUMMER_2017_TOKEN
};

static const char *SEMESTER_TOKENS[] = {
    FOREACH_SEMESTER(GENERATE_STRING)
};


#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <curl/curl.h>
#include "gumbo.h"


struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
void traverseHtml(GumboNode * root, int * closed);
const char * getCellData(GumboNode * dataRow);
void endrun(int signum);
int alertCourseStatus(int * closed, char * subject, char * courseNum);
#endif /* main_h */
