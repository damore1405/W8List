//
//  main.c
//  W8List-C
//
//  Created by Matthew D'Amore on 9/19/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#include "main.h"

//size_t write_callback(char *ptr, size_t size, size_t nmemb, struct string *result){
//    
//    size_t newLen = result->len + size * nmemb;
//    result->ptr = realloc(result->ptr, newLen+1);
//    
//    /*
//     *  Copy the data into the string we are building, from the point of the old length adding any
//     *  new data that the request has returned.
//     */
//    memcpy( result->ptr + result->len, ptr, size*nmemb);
//    
//    result->ptr[newLen] = '\0';
//    result->len = newLen;
//    
//    //Needs to return the amount of data it processed
//    return size * nmemb;
//}


// Not really needed for now
//void initString(struct string *s){
//    s->len = 0;
//    s->ptr = malloc(s->len + 1);
//    if (! s->ptr) {
//        printf("Something went horribly wrong!");
//        exit(EXIT_FAILURE);
//    }
//    //Initialize a "Blank" string with a string terminator
//    s->ptr[0] = '\0';
//}


uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out){
    uint r;
    r = size * nmemb;
    tidyBufAppend(out, in, r);
    return r;
}
void dumpNode(TidyDoc doc, TidyNode tnod, int indent )
{
    TidyNode child;
    for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
    {
        ctmbstr name = tidyNodeGetName( child );
        if ( name )
        {
            /* if it has a name, then it's an HTML tag ... */
            TidyAttr attr;
            printf( "%*.*s%s ", indent, indent, "<", name);
            /* walk the attribute list */
            for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
                printf(tidyAttrName(attr));
                tidyAttrValue(attr)?printf("=\"%s\" ",
                                           tidyAttrValue(attr)):printf(" ");
            }
            printf( ">\n");
        }
        else {
            /* if it doesn't have a name, then it's probably text, cdata, etc... */
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
            printf("%*.*s\n", indent, indent, buf.bp?(char *)buf.bp:"");
            tidyBufFree(&buf);
        }
        dumpNode( doc, child, indent + 4 ); /* recursive */ 
    }
}

int main(int argc, const char * argv[]) {
    
    // Init the curl request, the result code, and the result string
    CURL * curl;
    TidyDoc tdoc;
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    int err;
    struct string result;
    
    curl = curl_easy_init();
    if(curl){
        
        curl_easy_setopt(curl, CURLOPT_URL,
"https://duapp2.drexel.edu/webtms_du/app?component=subjectDetails&page=CollegesSubjects&service=direct&sp=ZH4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA&sp=SCI&sp=SCS&sp=5"
        );
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.5");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, tidy_errbuf);
        
        tdoc = tidyCreate();
        tidyOptSetBool(tdoc, TidyForceOutput, yes);
        tidyOptSetInt(tdoc, TidyWrapLen, 4096);
        tidySetErrorBuffer(tdoc, &tidy_errbuf);
        tidyBufInit(&docbuf);
        
        err = curl_easy_perform(curl);
        
        if ( !err ){
            err = tidyParseBuffer(tdoc, &docbuf);
            
            if (err >= 0) {
                err = tidyCleanAndRepair(tdoc);
                
                if (err >= 0 ) {
                    tidyRunDiagnostics(tdoc);
                    if (err >= 0) {
                        dumpNode( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */
                        fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */
                    }
                }
            }
            
        }
        
        printf("%s\n", result.ptr);
        curl_easy_cleanup(curl);

        
        
    }
    
    return 0;
}
