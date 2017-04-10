#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
#include <algorithm>
#include <cstring>
#include <map>
#include <functional>
#include <cstdio>
#include <sstream>

#include "cyclomatic.h"
#include "loc.h"
#include "halstead.h"
#include "maintainabilityIndex.h"

using namespace std;

extern void locResultPrinter(void);
extern void locMethodBasedResult(int i);
extern void printHalsteadMain(void);
extern double getMIResult(int loc, double cyclomatic, double volume);

extern void printComplextiyResult(int key);
extern void printHalsteadResult(int i);
extern char commentFreeFile[];

extern int totalMethod; // total number of Detected methods in file
extern float averageComplexitty;
extern float OverallIndex;

extern int getLoc(int i);
extern int getCyclmatic(int i);
extern double getHasteadVolume(int i);

extern char commentFreeFile[];
extern char fileForHalstead[];
extern char methodAreaFile[];
extern char detailedHalsteadFile[];
char advancedResultFile[] = "advanced.txt";


void generalResultDisplay(void) {

    locResultPrinter(); //Overall LOC Result
    printf("Average Cyclomatic Complexity: %.2f\n\n", averageComplexitty);
    printHalsteadMain();
    printf("Maintainability Index: %.2f\n\n", OverallIndex);
}

void detailedResultDisplay(void) {
    //Detailed Result DisplayS
    for(int i=0; i<totalMethod; i++) {

        locMethodBasedResult(i);
        printComplextiyResult(i);
        printHalsteadResult(i);
        printf("Maintainability Index: %.2f\n", getMIResult(getLoc(i), getCyclmatic(i), getHasteadVolume(i)) );
    }
}

void advancedResultDisplay(void) {

    ifstream iFile(advancedResultFile);  //Opening the input file

    if(iFile.is_open()) {

        string line;
        getline(iFile,line);

        while(!iFile.eof()){    //Reads as long as we do not reach the end of file
            cout<<line<<endl;
            getline(iFile, line);

        }
        iFile.close();      //Closes the input file
    }
    else printf("File not found\n");
}

void insertAdvanceData(void) {

    ofstream oFile(advancedResultFile, ios_base::app); // Opens the output file in append mode

    if(oFile.is_open()) {
        /*File Cyclomatic data*/
        oFile<<"Comment Free and Content Free Source File: (Cyclomatic Complexity)\n\n";
        ifstream iFile(commentFreeFile);  //Opening the input file
        if(iFile.is_open()) {
            string line;
            getline(iFile,line);

            while(!iFile.eof()){    //Reads as long as we do not reach the end of file
                oFile<<line<<endl;
                //Saving file in string
                getline(iFile, line);

            }
            iFile.close();      //Closes the input file
        }

        /*File Method area data*/
        oFile<<"\n\n\nMethod Area range: (Line Number)\n\n";
        ifstream iFile1(methodAreaFile);  //Opening the input file
        if(iFile1.is_open()) {
            string line;
            getline(iFile1,line);

            while(!iFile1.eof()){    //Reads as long as we do not reach the end of file
                oFile<<line<<endl;
                //Saving file in string
                getline(iFile1, line);

            }
            iFile1.close();      //Closes the input file
        }


        /*File Halstead data*/
        oFile<<"\n\n\nComment Free Source File: (Halstead Complexity)\n\n";
        ifstream iFile2(fileForHalstead);  //Opening the input file
        if(iFile2.is_open()) {
            string line;
            getline(iFile2,line);

            while(!iFile2.eof()){    //Reads as long as we do not reach the end of file
                oFile<<line<<endl;
                //Saving file in string
                getline(iFile2, line);

            }
            iFile2.close();      //Closes the input file
        }


        /*File Detailed Halstead data*/
        oFile<<"\n\n\nDetailed Result: (Halstead Complexity)\n\n";
        ifstream iFile3(detailedHalsteadFile);  //Opening the input file
        if(iFile3.is_open()) {
            string line;
            getline(iFile3,line);

            while(!iFile3.eof()){    //Reads as long as we do not reach the end of file
                oFile<<line<<endl;
                //Saving file in string
                getline(iFile3, line);

            }
            iFile3.close();      //Closes the input file
        }

        oFile.close();
    }
}

void removeFiles(void) {

    remove(commentFreeFile);    /** */
    remove(fileForHalstead);
    remove(methodAreaFile);
    remove(detailedHalsteadFile);
    remove(advancedResultFile); /**Removes the advanced result file*/
}

void userMenu (char *fileName) {

    string key;

    do{
        printf("\nFile Name: %s\n", fileName);
        printf("\n1. Overall Result\n2. Method Level Result\n3. Advanced Result: \n4. Quit\n\nEnter a selection: ");
        cin>>key;

        if( key == "1" ) generalResultDisplay();
        else if( key == "2" ) detailedResultDisplay();
        else if( key == "3" ) advancedResultDisplay();
        else if( key != "4" ) printf("\nPlease enter a valid selection!!!\n\n");

    } while (key != "4");


}

void mainCalculation(char* fileName, int flag) {
    /* Calculation phase of different segments starts from here*/
    LinesOfCode(fileName, flag);
    cyclomatic(fileName);
    halstead();
    maintainabiltyMain();

    /*Inserts excess data for calculation*/
    insertAdvanceData();
    /*result display section*/
    userMenu(fileName);

    /*Excess item removal section*/
    removeFiles();

}
