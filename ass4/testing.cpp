#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include "email.h"
#include <wait.h>
#include <fcntl.h>

void fillEmail(std::vector<Email>& emailList) {
    system("ls emailData > temp.txt");
    std::ifstream readFile("temp.txt");
    std::string line;
    int counter1 = 0;

    while(std::getline(readFile, line)) {
        line = "/home/abinashmeher999/github/networksLab/ass4/emailData/" + line;
        int size1 = line.size();
        std::cout << line << "\n";

        if(line[size1 - 1] == 'l' && line[size1 - 2] == 'i' && line[size1 - 3] == 'a' && line[size1 - 4] == 'm' && line[size1 - 5] == 'e') {
            Email email;
            email.deserialiseEmail(line);
            //std::cout<<email.getBody()<<"\n";
            emailList.push_back(email);
            counter1++;
        }
    }
}

int main() {
    Email email;
    email.setFromId("sandeep@gmail.com");
    email.setToId("abhinash@mgmail.com");
    email.setSubject("hi there");
    email.setBody("This is a test\nFucked up at that :v ");
    email.setTimeNow();
    email.serialiseEmail();
    std::cout << email.getFromId() << " " << email.getToId() << " ";
    printf("\n");
    std::vector<Email> emailList;
    fillEmail(emailList);

    for(int i = 0; i < emailList.size(); i++) {
        std::cout << emailList[i].getFromId() << "\n";
        std::cout << emailList[i].getToId() << "\n";
        std::cout << emailList[i].getSubject() << "\n";
        std::cout << emailList[i].getBody() << "\n";
    }
}
