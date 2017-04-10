#include <iostream>
#include <cstdio>
#include "result.h"

using namespace std;


int fileTypeDetector (string fileName) {

	int temp = fileName.size();
	int dotPoisition=-1;

	for(int i=0; i<temp; i++) {

		if(fileName[i]=='.') {
            dotPoisition = i;
            break;
		}
	}

	string typeName = fileName.substr(dotPoisition, temp-dotPoisition);

	if(typeName == ".c") return 1;
	else if(typeName == ".cpp") return 2;
	else return 0;

}


int main (int argc, char *argv[]) {

    ios::sync_with_stdio(false);  //for making C++ IO faster

	for(int i=1; i<argc; i++) {

        int flag = fileTypeDetector(argv[i]);

        switch(flag) {
            case 1:
                mainCalculation(argv[i], 1);
                break;
            case 2:
                mainCalculation(argv[i], 2);
                break;
            default: printf("%s %d %s","Argument no.",i,"Provides invalid file type\n");
        }


    }


	return 0;
}
