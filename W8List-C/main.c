//
//  main.c
//  W8List
//
//  Created by Matthew D'Amore on 9/20/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#include "main.h"

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
void inOrderTraversal(GumboNode * root){
    if (root->parent != NULL && root->type != GUMBO_NODE_WHITESPACE && root->type == GUMBO_NODE_TEXT) {
        if (!strcasecmp(root->v.text.text, "enroll")) {
            
            //MAX ENROLL 1
            //NUMBER     3
            
//            printf("Enroll found, it is child: %zu\n", root->parent->index_within_parent);
            
            GumboNode * tableRow = root->parent->parent;
            GumboNode ** kiddoes = (GumboNode** )tableRow->v.element.children.data;
            
            GumboNode * maxEnroll = kiddoes[1];
            GumboNode * number    = kiddoes[3];
            
            GumboNode * maxEnrollText = maxEnroll->v.element.children.data[0];
            GumboNode * numberText    = number   ->v.element.children.data[0];
            
            puts(maxEnrollText->v.text.text);
            puts(numberText->v.text.text);
            
            return;
        }
        if (!strcasecmp(root->v.text.text, "max enroll")) {
            
            //MAX ENROLL 1
            //NUMBER     3
            
//            printf("Enroll found, it is child: %zu\n", root->parent->index_within_parent);
            
            GumboNode * tableRow = root->parent->parent;
            GumboNode ** kiddoes = (GumboNode** ) tableRow->v.element.children.data;
            
            GumboNode * maxEnroll = kiddoes[1];
            GumboNode * number    = kiddoes[3];
            
            //Magic numbers here, not much i can do about it, "0" is simply the location of the first child
            GumboNode * maxEnrollText = maxEnroll->v.element.children.data[0];
            GumboNode * numberText    = number   ->v.element.children.data[0];
            
            puts(maxEnrollText->v.text.text);
            puts(numberText->v.text.text);
            
            return;
        }
    }                                                           /* vvv  Why is this needed?????????  vvv */
    if (root->v.element.children.length > 0 && root->v.element.children.length <= root->v.element.children.capacity) {
        for (int i = 0; i < root->v.element.children.length ; ++i) {
            inOrderTraversal((GumboNode *)root->v.element.children.data[i]);
        }
    }
    else return;
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
        curl_easy_setopt(curl, CURLOPT_URL,
                         "https://duapp2.drexel.edu/webtms_du/app?component=courseDetails&page=CourseList&service=direct&sp=ZH4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA&sp=SCI&sp=SCS&sp=S13301&sp=S140&sp=5");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
        res = curl_easy_perform(curl);
        
        // So now that i have the website, i need to use gumbo to parse out the information i need
        GumboOutput * output = gumbo_parse(s.ptr);
        
        //now... what do i do lol
        GumboVector children = output->root->v.document.children;

        printf("Length of root children: %u \n", children.length);
        GumboNode * body = (GumboNode *) children.data[1];
        
        if (body->v.element.tag == GUMBO_TAG_BODY) {
            puts("Successfully tore out the body of the html");
        }
        //Now make the children vector equal to the children of the body Element
        children = body->v.document.children;
        
        printf("The number of children in the body is: %u \n", children.length);
        
        inOrderTraversal(body);
        
        free(s.ptr);
        gumbo_destroy_output(NULL, output);
        curl_easy_cleanup(curl);
    }
    return 0;
}

