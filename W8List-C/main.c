//
//  main.c
//  W8List-C
//
//  Created by Matthew D'Amore on 9/19/15.
//  Copyright © 2015 Matthew Damore. All rights reserved.
//

#include <stdio.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <curl/curl.h>

struct string {
    char * ptr;
    size_t len;
};

size_t write_callback(char *ptr, size_t size, size_t nmemb, struct string *result);
void initString(struct string *s);

size_t write_callback(char *ptr, size_t size, size_t nmemb, struct string *result){
    
    
    
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
    CURL * curl;
    CURLcode res;
    struct string result;
    
    curl = curl_easy_init();
    if(curl){
        
        curl_easy_setopt(curl, CURLOPT_URL, "www.example.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            printf("Something bunked up!\n");
            exit(1);
        }
        
    }
    
    return 0;
}
