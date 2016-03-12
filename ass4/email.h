#include <ctime>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <dirent.h>

class Email {
    struct tm time_;
    char* fromId;
    char* toId;
    char* body;
    int size;

public:
    Email() {
        fromId = NULL;
        toId = NULL;
        body = NULL;
        //size=0;
    }

    Email(struct tm t): time_(t) {};

    void setFromId(std::string t) {
        if (fromId != NULL) {
            free(fromId);
            fromId = NULL;
        }

        fromId = (char*)malloc(sizeof(char) * (strlen(t.c_str()) + 1));
        strcpy(fromId, t.c_str());
    }

    void setFromId(char* t) {
        if (fromId != NULL) {
            free(fromId);
            fromId = NULL;
        }

        fromId = (char*)malloc(sizeof(char) * (strlen(t) + 1));
        strcpy(fromId, t);
    }

    std::string getFromId() {
        return std::string(fromId);
    }
    //fromId

    void setToId(std::string t) {
        if (toId != NULL) {
            free(toId);
            toId = NULL;
        }

        toId = (char*)malloc(sizeof(char) * (strlen(t.c_str()) + 1));
        strcpy(toId, t.c_str());
    }

    void setToId(char* t) {
        if (toId != NULL) {
            free(toId);
            toId = NULL;
        }

        toId = (char*)malloc(sizeof(char) * (strlen(t) + 1));
        strcpy(toId, t);
    }

    std::string getToId() {
        return std::string(toId);
    }
    //toId

    void setBody(std::string t) {
        if (body != NULL) {
            free(body);
            body = NULL;
        }

        body = (char*)malloc(sizeof(char) * (strlen(t.c_str()) + 1));
        strcpy(body, t.c_str());
    }

    void setBody(char* t) {
        if (body != NULL) {
            free(body);
            body = NULL;
        }

        body = (char*)malloc(sizeof(char) * (strlen(t) + 1));
        strcpy(body, t);
    }

    std::string getBody() {
        return std::string(body);
    }
    //body

    void setTime(struct tm t) {
        time_ = t;
    }

    struct tm getTime() {
        return time_;
    }

    void setTimeNow() {
        time_t rawtime;
        struct tm* timeinfo;

        rawtime = time(0);
        timeinfo = localtime( &rawtime );
        mktime(timeinfo);

        timeinfo->tm_year += 1900;
        timeinfo->tm_mon += 1;
        this->setTime(*timeinfo);
    }
    //time

    ~Email() {
        if (fromId != NULL) {
            //free(title);
            fromId = NULL;
        }

        if (toId != NULL) {
            //free(content);
            toId = NULL;
        }

        if(body != NULL) {
            body = NULL;
        }
    }

    void serialiseEmail() {
        std::stringstream ss;
        ss << "./emailData/";
        ss << time_.tm_year << "_" <<  time_.tm_mon << "_" << time_.tm_mday << "_" <<
           time_.tm_hour << "_" << time_.tm_min << "_" << time_.tm_sec << "_" << std::string(fromId) << ".email";
        std::ofstream file(ss.str().c_str());
        file << std::string(fromId) << std::endl << std::string(toId) << std::endl
             << time_.tm_year << " " <<  time_.tm_mon << " " << time_.tm_mday << " " << time_.tm_wday
             << " " << time_.tm_yday << " " << time_.tm_hour << " " << time_.tm_min << " " << time_.tm_sec << std::endl
             << std::string(body) << std::endl;
        file.flush();
        file.close();
    }

    void deserialiseEmail(std::string file_name_c_str) {
        std::string file_name(file_name_c_str);
        std::ifstream file(file_name.c_str());
        std::stringstream ss;
        //file >> ID;
        std::string fromId;
        std::string toId;
        std::string body;
        std::getline(file, fromId);
        std::getline(file, toId);
        this->setFromId(fromId);
        this->setToId(toId);
        //std::cout<<"fromId->"<<this->fromId<<"\n";
        //std::cout<<"toId->"<<this->toId<<"\n";
        file >> time_.tm_year >> time_.tm_mon >> time_.tm_mday >> time_.tm_wday
             >> time_.tm_yday >> time_.tm_hour >> time_.tm_min >> time_.tm_sec;
        std::getline(file, body);

        while(file.good()) {
            std::getline(file, body);
            ss << body << std::endl;
        }

        this->setBody(ss.str());
        //std::cout<<"body->"<<this->body<<"\n";
    }


    bool operator < (Email& email) const {
        struct tm ktime = email.getTime();
        return (time_.tm_year < ktime.tm_year) || (time_.tm_mon < ktime.tm_mon) || (time_.tm_mday < ktime.tm_mday);
    }

    bool operator > (Email& email) const {
        struct tm ktime = email.getTime();
        return (time_.tm_year > ktime.tm_year) || (time_.tm_mon > ktime.tm_mon) || (time_.tm_mday < ktime.tm_mday);
    }

// static std::vector<News> ReadFromDir(const char * path_str) {
    // if ((dir = opendir(path_str)) != NULL) {
    //   /* print all the files and directories within directory */
    //   while ((ent = readdir(dir)) != NULL) {
    //         //std::cout << ent->d_name << "\n";
    //         int size = strlen(ent->d_name);
    //         // std::cout << size << "\n";
    //         if (size >= 5) {
    //         if(strcmp((ent->d_name) + size - 5, ".news")) {
    //             News khabar;
    //             khabar.deserialise(ent->d_name);
    //             v.push_back(khabar);
    //         }
    //     }
    //   }
    //   closedir(dir);
    // } else {
    //   /* could not open directory */
    //   perror ("Couldn't open directory");
    //   std::vector<News> emp;
    //   return emp;
    // }
    // }
};
