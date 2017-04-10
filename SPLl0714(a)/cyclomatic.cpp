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

using namespace std;

/******************************     Global variables ***************************************/
// Keywords we need to consider during creation of node
char commentFreeFile[] = "cleanFile.txt";
char fileForHalstead[] = "halstead.txt";

bool multipleCommentLineFlag = false;


/*******************************  Other file methods *************************************/
extern void findComplexity(void);


string commentFreeFileInput(string line) {

    if(line[0]=='#') {
        return "";             // Ignores lines starting with '#' like #include <iostream>
    }
    if(line.length() == 0 ) {
        return "";      // Ignores empty Lines
    }

    bool doubleQuote = false; //Keeps the track of first occurrence of a letter in a line

    int lineSize = line.size();
    for(int i=0; i < lineSize ; i++) {

        if(line[i] == '"' && doubleQuote == false) doubleQuote = true;/*Start of the double quote.
        We will not consider anything inside double quote */
        else if(doubleQuote == true) {

            if(line[i] == '"') doubleQuote = false;     //If the closing double quote is reached
            else continue;                              // else we ignore the character because it is inside the double quote

        }
        else if(multipleCommentLineFlag == true) {      //If the current character is inside of multiple line comment character

            if(line[i]=='*' && line[i+1] =='/') {
                multipleCommentLineFlag = false;
                line[i] = ' ';
                line[i+1]= ' ';
            }
            else line[i]=' ';

        }
        else if(line[i]=='/' && line[i+1] =='*') {      //If the closing of multiple line comment character
            multipleCommentLineFlag = true;
            line[i] = ' ';
            line[i+1] = ' ';
        }
        else if(line[i] == '/' && line[i+1] == '/') {   //if single line comment character is reached, then we ignore the next characters in that line

            for(int j=i ; j<lineSize; j++) line[j] = ' ';
            break;
        }

    }

    for(int i=0; i<lineSize; i++) {
        if(line[i] != ' ' && line[i] != 13) return line+"\n"; // carriage return may be present in line
    }

    return ""; //This is the case when the whole line is comprised of space or carriage return character
}

string buildContentFreeLine(string line) {

    int lineSize = line.size();

    bool flagDoubleQoute = false;
    bool flagSingleQoute = false;

    for(int i=0; i<lineSize; i++) {     /** removes the contents between single quote and double quote*/

        if( (line[i] == 39 || line[i] == '"') && flagDoubleQoute==false && flagSingleQoute == false ) {

            if(line[i] == 39) flagSingleQoute = true;
            else if(line[i] == '"') flagDoubleQoute = true;
            line[i] = ' ';
            continue;
        }
        else if( flagDoubleQoute == true ) {
            if(line[i] == '"') flagDoubleQoute = false;
            line[i] = ' ';
        }
        else if( flagSingleQoute == true ) {
            if(line[i] == 39) flagSingleQoute = false;
            line[i] = ' ';
        }

    }

    return line;
}
/***************************************************************************************************************/

void buildCommentAndContentFreeFile(char* fileName) {     //Input file as parameter

    ifstream iFile(fileName);   //input file
    ofstream oFile(commentFreeFile);   //opening of temporary comment and space free lines
    ofstream oFile2(fileForHalstead);  //Opening the file to save comment free file for halstead calculation

    if(iFile.is_open()) {

        string line;
        getline(iFile,line);


        while(!iFile.eof()){

            string tempLine = commentFreeFileInput(line);  /* Start of function that removes comments and blank lines
            Receives the string part that are not comment or blank line*/
            oFile2<<tempLine; /** clean file for halstead*/
            tempLine = buildContentFreeLine(tempLine);      //Erases the contents between first Braces()

            oFile<<tempLine;

            getline(iFile, line);

        }

        oFile2.close(); //Closes the file for halstead
        oFile.close();  //closing of temporary comment and space free lines
        iFile.close();  //closing of input file
    }
}



void cyclomatic(char* fileName) {   //Receives the file for which we want to calculate cyclomatic complexity
/******************* *************/

    multipleCommentLineFlag = false;
    buildCommentAndContentFreeFile(fileName);     // Makes a file so that I can take clear and comment free input
    findComplexity();                  // Create node for the call graph

    //remove(commentFreeFile);
}
