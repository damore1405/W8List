//
//  main.h
//  W8List-C
//
//  Created by Matthew D'Amore on 9/19/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#ifndef main_h
#define main_h

#include <stdio.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <curl/curl.h>

struct string {
    char * ptr;
    size_t len;
};

//size_t write_callback(char *ptr, size_t size, size_t nmemb, struct string *result);
//void initString(struct string *s);
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out);
void dumpNode(TidyDoc doc, TidyNode tnod, int indent );



#endif /* main_h */
