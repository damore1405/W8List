//
//  main.h
//  W8List
//
//  Created by Matthew D'Amore on 9/20/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#ifndef main_h
#define main_h

// Preprocesser code to link up an enumeration for the string tokens
#define FOREACH_FRUIT(SEMESTER_TOKEN) \
SEMESTER_TOKEN(H4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA)   \
SEMESTER_TOKEN(ZH4sIAAAAAAAAAFvzloG1uIhBPjWlVC%2BlKLUiNUcvs6hErzw1qSS3WC8lsSRRLyS1KJcBAhiZGJh9GNgTk0tCMnNTSxhEfLISyxL1iwtz9EECxSWJuQXWPgwcJUAtzvkpQBVCEBU5iXnp%2BsElRZl56TB5l9Ti5EKGOgamioKCEgY2IwNDUyNToJHhmXlAaYXA0sQiEG1oqmtoBgCbhSKKpgAAAA%3D%3D)  \
SEMESTER_TOKEN(ZH4sIAAAAAAAAAFvzloG1uIhBPjWlVC%2BlKLUiNUcvs6hErzw1qSS3WC8lsSRRLyS1KJcBAhiZGJh9GNgTk0tCMnNTSxhEfLISyxL1iwtz9EECxSWJuQXWPgwcJUAtzvkpQBVCEBU5iXnp%2BsElRZl56TB5l9Ti5EKGOgamioKCEgY2IwNDU2NToJHBBSBVCoGliUVAZQqGprqGZgAfTc62pgAAAA%3D%3D)   \
SEMESTER_TOKEN(ZH4sIAAAAAAAAADWMPQ6CQBCFR4w%2FtbGXC7iAERtLLWkMXGBkJ2QNizA7KJUn8mrewTXEV37ve%2B%2F9gZlj2JDulWYaqFaGRT3pKtYpjYKqILYwZhLANIMFllIYSwLr7IYPjFxXRz%2FgBG17zGApfnK6a2%2BsRqPGpopyYdNU%2F%2F5MruzgBcHQtgLzXZyk%2B9Rf5r21xOGlR%2FZamKTb5PAFy%2Bn%2BwKYAAAA%3D)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum FRUIT_ENUM {
    FALL_2015_TOKEN = 0,
    WINTER_2015_TOKEN,
    SPRING_2016_TOKEN,
    SUMMER_2016_TOKEN
};

static const char *SEMESTER_TOKENS[] = {
    FOREACH_FRUIT(GENERATE_STRING)
};
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <curl/curl.h>
#include "gumbo.h"

//static char * FALL_2015_TOKEN = "H4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA";


struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
void traverseHtml(GumboNode * root);

#endif /* main_h */
