//
//  W8List.c
//  W8List
//
//  Created by Matthew D'Amore on 9/20/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//
// TODO: NEED TO EMPLEMENT ERROR CHECKING SO THAT WHEN THE WEBSITE IS DOWN, SO NOBODY GETS NOTIFIED

#include "W8List.h"


int main(int argc, char ** argv)
{
//    //TODO: lets go ahead and daemonize
//    pid_t pid, sid;
//    
//    /* Fork off the parent process */
//    pid = fork();
//    if (pid < 0) {
//        exit(EXIT_FAILURE);
//    }
//    /* If we got a good PID, then we can exit the parent process. */
//    if (pid > 0) {
//        puts("parent process closing");
//        exit(EXIT_SUCCESS);
//    }
//    
//    /* Change the file mode mask */
//    umask(0);
//    
//    // Set up the logger
//    setlogmask(LOG_UPTO(LOG_NOTICE));
//    openlog("W8List Daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
//    syslog(LOG_NOTICE, "W8List has started");
//    
//    /* Create a new SID for the child process */
//    sid = setsid();
//    if (sid < 0) {
//        /* Log any failures here */
//        exit(EXIT_FAILURE);
//    }
//    
//    
//    /* Change the current working directory */
//    if ((chdir("/")) < 0) {
//        /* Log any failures here */
//        exit(EXIT_FAILURE);
//    }
//    
//    /* Close out the standard file descriptors */
//    close(STDIN_FILENO);
//    close(STDOUT_FILENO);
//    close(STDERR_FILENO);
//    
//    /* Daemon-specific initialization goes here */
//    
//    /* The Big Loop */
//    syslog(LOG_NOTICE, "The class watcher has started");
//    while (1) {
//        /* Do some task here ... */
//        
//        sleep(10);
//        
//    }
    
//    exit(EXIT_SUCCESS);
    
    CURL *curl;
    CURLcode res;
    int opt;

    
    curl = curl_easy_init();
    if(curl) {
        struct string s;
        init_string(&s);
        char * baseUrl = malloc(sizeof(char) * 100000000);
        
        //TODO: Case based on user input for which semester to select...
        
        
        //TODO: Code implementation for Schools... maybe another string enumeration like the one used for semesters
        
        
        
        /*  TODO: Implementaion of user input processing... we need
         *      - School
         *      - Subject
         *      - CRN
         *      - Course Number
        */
//        puts(SEMESTER_TOKENS[WINTER_2015_TOKEN]);
        sprintf(baseUrl, "https://duapp2.drexel.edu/webtms_du/app?component=courseDetails2&page=CourseList&service=direct&sp=%s&sp=SCI&sp=SCS&sp=S16024&sp=S613&sp=5", SEMESTER_TOKENS[FALL_2015_TOKEN]);
        puts(baseUrl);
        //Massive ass url for the classes
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
        res = curl_easy_perform(curl);
        
//        if (res < 0) {
//            //The request failed...
//            syslog(LOG_ALERT, "The reqest to drexel TMS failed... check the configuration");
//            return 1;
//        }
        
        // So now that i have the website, i need to use gumbo to parse out the information i need
        GumboOutput * output = gumbo_parse(s.ptr);
        GumboVector children = output->root->v.document.children;
        
        printf("Length of root children: %u \n", children.length);
        GumboNode * body = (GumboNode *) children.data[1];
        
        if (body->v.element.tag == GUMBO_TAG_BODY) {
            puts("Successfully tore out the body of the html");
        }
        char * str = malloc(sizeof(char) * 100);
        long int val = 0;
        val = strtol("25", NULL, 10);
        printf("This is what strtol returnes: %ld \n", val);
        //Now make the children vector equal to the children of the body Element
        children = body->v.document.children;
        
        printf("The number of children in the body is: %u \n", children.length);
        
        traverseHtml(body);
        
        free(s.ptr);
//        free(baseUrl);
        free(output);
        curl_easy_cleanup(curl);
    }
    return 0;
}

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

void traverseHtml(GumboNode * root){
    if (root->parent != NULL && root->type != GUMBO_NODE_WHITESPACE && root->type == GUMBO_NODE_TEXT) {
        if (!strcasecmp(root->v.text.text, "enroll")) {
            
            //MAX ENROLL 1
            //NUMBER     3
            
//            GumboNode * tableRow = root->parent->parent;
//            GumboNode ** kiddoes = (GumboNode** )tableRow->v.element.children.data;
//            
//            GumboNode * maxEnroll = kiddoes[1];
//            GumboNode * number    = kiddoes[3];
//            
//            GumboNode * maxEnrollText = maxEnroll->v.element.children.data[0];
//            GumboNode * numberText    = number   ->v.element.children.data[0];
//            
//            puts(maxEnrollText->v.text.text);
//            puts(numberText->v.text.text);
            
            GumboNode * tableRow = root->parent->parent;
            const char * data = getCellData(tableRow);
            printf("enroll: %s \n", data);
            return;
        }
        else if (!strcasecmp(root->v.text.text, "max enroll")) {
            GumboNode * tableRow = root->parent->parent;
            const char * data = getCellData(tableRow);
            printf("max enroll: %s \n", data);
            return;
        }
    }
                                                            /* vvv  Why is this needed?????????  vvv */
    if (root->v.element.children.length > 0 && root->v.element.children.length <= root->v.element.children.capacity) {
        for (int i = 0; i < root->v.element.children.length ; ++i) {
            traverseHtml((GumboNode *)root->v.element.children.data[i]);
        }
    }
    
    else return;
}

const char * getCellData(GumboNode * dataRow){
    GumboNode ** kiddoes = (GumboNode** )dataRow->v.element.children.data;
    GumboNode * value    = kiddoes[3]->v.element.children.data[0];
    
    
    return value->v.text.text;
}