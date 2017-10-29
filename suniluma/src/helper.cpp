#include<stdlib.h>
#include<iostream>
#include<string>
#include <unistd.h>
#include <cstdlib>
#include<stdio.h>
#include <cstring>
#include <sstream>

using namespace std;

#include "../include/helper.h"

string UBITNAME = "suniluma";
int BACKLOG = 10;
int STDIN = 0;
int BUFFER_SIZE = 512;
int port_value = 0;



//Helper Functions

bool is_upper_string(string cmd) {
	int i = 0;
	while(cmd[i]) {
		if(islower(cmd[i])) return false;
		i++;
	}
	return true;
}

string string_input() {
	string cmd;
	cin>>cmd;
	while(!is_upper_string(cmd)) {
		cout<<"Input is not all caps"<<endl;
		cin>>cmd;
	}
	return cmd;
}

void success_log(string cmd) {
	printf("[%s:SUCCESS]\n", cmd.c_str());
}

void error_log(string cmd) {
	printf("[%s:ERROR]\n", cmd.c_str());
}

void end_log(string cmd) {
	printf("[%s:END]\n", cmd.c_str());
}



string splitter(string str, string delimiter = " ", int index = 0) {

	size_t pos = 0;
	string token;
	int i = 0;
	while ((pos = str.find(delimiter)) != string::npos) {
	    token = str.substr(0, pos);
	    if (i == index) {
			return token;

		}
	    str.erase(0, pos + delimiter.length());
		i++;
	}
	if(i == index) {
		return str;
	}
	return "";
}

string splitter_v2(string str, string delimiter = " ", int index = 0) {

	size_t pos = 0;
	string token;
	int i = 0;
	while ((pos = str.find(delimiter)) != string::npos) {
	    token = str.substr(pos + delimiter.length(), str.length());
	    if (i == index) {
			return token;

		}
	    str.erase(0, pos + delimiter.length());
		i++;
	}
	if(i == index) {
		return str;
	}
	return "";
}


char * str_to_char(string str) {
    return (char *)str.c_str();
}
