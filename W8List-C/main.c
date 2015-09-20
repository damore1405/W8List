#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "gumbo.h"

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    
    return size*nmemb;
}

int main(void)
{
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        struct string s;
        init_string(&s);
        
        //Massive ass url for the classes
        curl_easy_setopt(curl, CURLOPT_URL, "https://duapp2.drexel.edu/webtms_du/app?component=courseDetails&page=CourseList&service=direct&sp=ZH4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA&sp=SCI&sp=SCS&sp=S13301&sp=S140&sp=5");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
        res = curl_easy_perform(curl);
        
        // So now that i have the website, i need to use gumbo to parse out the information i need
        GumboOutput * output = gumbo_parse(s.ptr);
        
        //now... what do i do lol
        GumboVector children = output->root->v.document.children;

        printf("Length of root children: %u \n", children.length);
        GumboNode * body = children.data[1];
        
        if (body->v.element.tag == GUMBO_TAG_BODY) {
            puts("Successfully tore out the body of the html");
        }
        //Now make the children vector equal to the children of the body Element
        children = body->v.document.children;
        
        printf("The number of children in the body is: %u \n", children.length);
        
        for (int i = 0; i < children.length; i++) {
            GumboNode * node = children.data[i];
            if (node->type != GUMBO_NODE_WHITESPACE) {
                GumboVector nodeAttributes = node->v.element.attributes;
                for (int j = 0; j < nodeAttributes.length; ++j) {
                    GumboAttribute * a = nodeAttributes.data[j];
                    printf("attributes %s:%s \n", a->name , a->value);
                }
            }
        }
        
        free(s.ptr);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}

