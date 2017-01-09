//
//  W8List.c
//  W8List
//
//  Created by Matthew D'Amore on 9/20/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#include "W8List.h"

static volatile int run = 1;
static const int tenMin = 60 * 10;

int main(int argc, char ** argv) {
    
    if (argc != 6){
        puts("Incorrect arg count, please see usage below...");
        puts("./W8list <Email> <Quarter> <School> <Subject> <CRN> <CourseId>");
        exit(EXIT_FAILURE);
    }
    
    // Example Args: ./W8list winter CI CS 20116 260
    char * quarter = argv[1];
    char * school = argv[2];
    char * subject = argv[3];
    char * crn = argv[4];
    char * courseNum = argv[5];
    enum SEMESTER_ENUM token;
    
    if      (strcasecmp("fall"  , quarter) == 0) token = FALL_2017_TOKEN;
    else if (strcasecmp("winter", quarter) == 0) token = WINTER_2017_TOKEN;
    else if (strcasecmp("spring", quarter) == 0) token = SPRING_2017_TOKEN;
    else                                         token = SUMMER_2017_TOKEN;
    
    
    // Args are sorted out, daemonize...
    pid_t pid, sid;
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        puts("parent process closing");
        exit(EXIT_SUCCESS);
    }
    
    /* Change the file mode mask */
    umask(0);
    
    // Set up the logger
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("W8List Daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    puts("LOGGED");
    syslog(LOG_NOTICE, "W8List has started");
    
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log any failures here */
        exit(EXIT_FAILURE);
    }
    
    
    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* Log any failures here */
        exit(EXIT_FAILURE);
    }
    
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    syslog(LOG_NOTICE, "The class watcher has started");
    signal(SIGINT, endrun);
    signal(SIGTERM, endrun);
    
    while (run == 1) {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        
        if(curl) {
            char * baseUrl = malloc(sizeof(char) * 10000);
            struct string s;
            init_string(&s);
            sprintf(baseUrl, "https://duapp2.drexel.edu/webtms_du/app?component=courseDetails2&page=CourseList&service=direct&sp=%s&sp=S%s&sp=S%s&sp=S%s&sp=S%s&sp=0", SEMESTER_TOKENS[token], school, subject, crn, courseNum);
            //Massive ass url for the classes
            syslog(LOG_NOTICE, "Using url: %s",baseUrl);
            curl_easy_setopt(curl, CURLOPT_URL, baseUrl);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
            
            res = curl_easy_perform(curl);
            
            // So now that i have the website, i need to use gumbo to parse out the information i need
            GumboOutput * output = gumbo_parse(s.ptr);
            GumboVector children = output->root->v.document.children;
            GumboNode * body = (GumboNode *) children.data[1];
            
            // Now make the children vector equal to the children of the body Element
            children = body->v.document.children;
            
            int * closed = malloc(sizeof(int)); *closed = -1;
            traverseHtml(body, closed);
            
            char * message;
            if( *closed == 1 ){
                message = "OH DANG ITS CLOSED";
                syslog(LOG_NOTICE, "Looks like its closed");
            } else if (*closed == 0){
                message = "WOAH NELLY ITS OPEN";
                syslog(LOG_NOTICE, "Looks like its open");
            } else {
                message = "Looks like something went wrong with the page load";
                syslog(LOG_NOTICE, "error on page-load");
            }
            
            alert(message);
            free(closed);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
            free(s.ptr);
            free(baseUrl);
            curl_easy_cleanup(curl);
            sleep(tenMin);
        }
    }
    alert("guh ive been shot");
    free(quarter);
    free(school);
    free(subject);
    free(crn);
    free(courseNum);
    return EXIT_SUCCESS;
}

void traverseHtml(GumboNode * root, int * closed){
    if (root->type == GUMBO_NODE_TEXT) {
        if (!strcasecmp(root->v.text.text, "enroll")) {
            GumboNode * tableRow = root->parent->parent;
            const char * data = getCellData(tableRow);
            *closed = !strcasecmp("CLOSED", data) ? 1 : 0;
            return;
        }
    }

    if (root->v.element.children.length > 0 && root->v.element.children.length <= root->v.element.children.capacity) {
        for (int i = 0; i < root->v.element.children.length ; ++i) {
            traverseHtml((GumboNode *)root->v.element.children.data[i], closed);
        }
    }
}

const char * getCellData(GumboNode * dataRow){
    GumboNode ** kiddoes = (GumboNode** )dataRow->v.element.children.data;
    GumboNode * value    = kiddoes[3]->v.element.children.data[0];
    
    return value->v.text.text;
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

int alert(char * alert){
    // Use applescript to send email
    char * sysCall = "osascript -e 'display notification \"\" with title \"%s\"'";
    char * alertString = malloc(sizeof(char) * (strlen(sysCall) + strlen(alert)));
    sprintf(alertString, sysCall, alert);
    int returnVal = system(alertString);
    syslog(LOG_NOTICE, "%s", alertString);
    free(alertString);
    return returnVal;
}

void endrun(int signum) {run = 0;}


