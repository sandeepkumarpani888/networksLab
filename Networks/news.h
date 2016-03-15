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

class News{
    int ID;
    char* title;
    struct tm time_;
    char* content;
    char* category;
    int size;

public:
    News() {
        title = NULL;
        content = NULL;
    }

    News(struct tm t): time_(t) {};

    void set_ID(int id) {
        ID = id;
    }

    int get_ID() {
        return ID;
    }

    void set_title(std::string t) {
        if (title != NULL) {
            free(title);
            title = NULL;
        }
        title = (char *)malloc(sizeof(char)*(strlen(t.c_str())+1));
        strcpy(title, t.c_str());
    }

    void set_title(char* t) {
        if (title != NULL) {
            free(title);
            title = NULL;
        }
        title = (char *)malloc(sizeof(char)*(strlen(t)+1));
        strcpy(title, t);
    }

    std::string get_title() {
        return std::string(title);
    }

    void set_content(char* cont) {
        if (content != NULL) {
            free(content);
            content = NULL;
        }
        content = (char *)malloc(sizeof(char)*(strlen(cont)+1));
        strcpy(content, cont);
    }


    void set_content(std::string cont) {
        if (content != NULL) {
            free(content);
            content = NULL;
        }
        content = (char *)malloc(sizeof(char)*(strlen(cont.c_str())+1));
        strcpy(content, cont.c_str());
    }

    std::string get_content() {
        return std::string(content);
    }

    void set_time(struct tm t) {
        time_ = t;
    }

    struct tm get_time() {
        return time_;
    }

    void set_time_now() {
        time_t rawtime;
        struct tm * timeinfo;

        rawtime = time(0);
        timeinfo = localtime( &rawtime );
        mktime(timeinfo);

        timeinfo->tm_year += 1900;
        timeinfo->tm_mon += 1;
        this->set_time(*timeinfo);
    }

    ~News() {
        if (title != NULL) {
            //free(title);
            title = NULL;
        }
        if (content != NULL) {
            //free(content);
            content = NULL;
        }
    }

    void serialise() {
        std::stringstream ss;
        ss << time_.tm_year << "_" <<  time_.tm_mon << "_" << time_.tm_mday << "_" << ID << "_" << std::string(title) << ".news";
        std::ofstream file(ss.str().c_str());
        file << ID << std::endl << std::string(title) << std::endl << time_.tm_year << " " <<  time_.tm_mon << " " << time_.tm_mday << " " << time_.tm_wday
            << " " << time_.tm_yday << " " << time_.tm_hour << " " << time_.tm_min << " " << time_.tm_sec << std::endl << std::string(content) << std::endl;
        file.flush();
        file.close();
    }

    void serialise_a(){
        std::stringstream ss;
        ss<<"/home/sandeep/Desktop/Networks/Academic/";
        ss << time_.tm_year << "_" <<  time_.tm_mon << "_" << time_.tm_mday << "_" << ID << "_" << std::string(title) << ".news";
        std::ofstream file(ss.str().c_str());
        file << ID << std::endl << std::string(title) << std::endl << time_.tm_year << " " <<  time_.tm_mon << " " << time_.tm_mday << " " << time_.tm_wday
            << " " << time_.tm_yday << " " << time_.tm_hour << " " << time_.tm_min << " " << time_.tm_sec << std::endl << std::string(content) << std::endl;
        file.flush();
        file.close();
    }

    void serialise_na(){
        std::stringstream ss;
        ss<<"/home/sandeep/Desktop/Networks/Non-Academic/";
        ss << time_.tm_year << "_" <<  time_.tm_mon << "_" << time_.tm_mday << "_" << ID << "_" << std::string(title) << ".news";
        std::ofstream file(ss.str().c_str());
        file << ID << std::endl << std::string(title) << std::endl << time_.tm_year << " " <<  time_.tm_mon << " " << time_.tm_mday << " " << time_.tm_wday
            << " " << time_.tm_yday << " " << time_.tm_hour << " " << time_.tm_min << " " << time_.tm_sec << std::endl << std::string(content) << std::endl;
        file.flush();
        file.close();
    }

   void deserialise(std::string file_name_c_str) {
        std::string file_name(file_name_c_str);
        std::ifstream file(file_name.c_str());
        std::stringstream ss;
        file >> ID;
        std::string title_string;
        std::string content_string;
        std::getline(file, title_string);
        std::getline(file, title_string);
        this->set_title(title_string);
        file >> time_.tm_year >> time_.tm_mon >> time_.tm_mday >> time_.tm_wday
            >> time_.tm_yday >> time_.tm_hour >> time_.tm_min >> time_.tm_sec;
        std::getline(file, content_string);
        while(file.good()) {
            std::getline(file, content_string);
            ss << content_string << std::endl;
        }
        this->set_content(ss.str());
    }


    bool operator < (News& khabar) const
    {
        struct tm ktime = khabar.get_time();
        return (time_.tm_year < ktime.tm_year) || (time_.tm_mon < ktime.tm_mon) || (time_.tm_mday < ktime.tm_mday);
    }

    bool operator > (News& khabar) const
    {
        struct tm ktime = khabar.get_time();
        return (time_.tm_year > ktime.tm_year) || (time_.tm_mon > ktime.tm_mon) || (time_.tm_mday < ktime.tm_mday);
    }

static std::vector<News> ReadFromDir(const char * path_str) {
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
    }
};
