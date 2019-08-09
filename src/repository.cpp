#include <vector>
#include <iostream>
#include "repository.h"
#include <curl/curl.h>
#include <cstring>
#include <mutex>


using namespace std;

repository::repository()
{
}

repository::~repository()
{
}
mutex m;
void repository::elastics(string record)
{
    m.lock();
    CURL *hnd = curl_easy_init();
    if (hnd){
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(hnd, CURLOPT_URL, "http://elastic:soheil30@localhost:9200/message-table/_doc");

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Postman-Token: 2bde4149-3157-4e34-a7da-14d6ce07f53e");
        headers = curl_slist_append(headers, "cache-control: no-cache");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
        string temp = record;
        char cstr[temp.size() + 1];
        strcpy(cstr, temp.c_str());

        cout << cstr << endl;

        curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, cstr );

        CURLcode ret = curl_easy_perform(hnd);
        if(ret != CURLE_OK){
             fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(ret));
        }
        cout << endl <<endl;
        curl_easy_cleanup(hnd);
    }
    curl_global_cleanup();
    m.unlock();
}

