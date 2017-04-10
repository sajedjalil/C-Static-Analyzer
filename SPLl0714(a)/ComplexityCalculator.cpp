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

#define limit 5000      // We assume that no single file will be more than 5000 lines without comments and space
using namespace std;



/********************************* variables ***************************/
extern char commentFreeFile[];
extern char advancedResultFile[];
extern void nameAssign(string name ,int methodNumber);


/**** function detection data***********/
char methodAreaFile[] = "methodArea.txt";
string savedFile[limit];
int saveFileSize = 0;

struct node{

    int startLine;
    int finishLine;
    int complexity;
    string name;
    bool nameFlag; // true if exists in the line containing double brace

} tempNode;

vector <node> methodTracer;
float averageComplexitty;

/*************************************/

void saveMethodArea(void) {
    remove(methodAreaFile);
    ofstream oFile(methodAreaFile);

    if( oFile.is_open() ) {

        int temp = methodTracer.size();
        for(int i=0; i<temp; i++) {

            if(methodTracer[i].nameFlag == true) oFile<<methodTracer[i].startLine;
            else oFile<<(methodTracer[i].startLine-1);

            oFile<<" "<<methodTracer[i].finishLine<<endl;
        }

        oFile.close();
    }
    else cout<<"File fetching error\n";
}

void printComplextiyResult(int key) {

    printf("Cyclomatic Complexity : %d\n\n", methodTracer[key].complexity);

}

int getCyclmatic(int i) {

    return methodTracer[i].complexity;
}

void calculateAverageComplexity() {

    averageComplexitty = 0.0;
    int tempTotal = methodTracer.size();

    for(int i=0; i<tempTotal; i++) {
        averageComplexitty += methodTracer[i].complexity;
    }

    averageComplexitty = (float)averageComplexitty / tempTotal;

}

//int detectMethodCall( string line ) {
//
//    int tempSize = methodTracer.size();
//    int counter = 0;
//
//    for(int i=0; i<tempSize; i++) {
//
//        if(line.find(methodTracer[i].name) != string::npos){
//            counter+=methodTracer[i].complexity;
//        }
//    }
//
//    return counter;
//}

int countDoubleCharacaterPredicates(int firstLineNo, int lastLineNo) {

    int tempComplexity = 0;
    for(int i= firstLineNo; i <= lastLineNo; i++) {     /* Only for two character operators*/

        int tempLineSize = savedFile[i].size() - 1; // We are checking two characters at a time
        for(int j=0; j<tempLineSize; j++) {

            if(savedFile[i][j] == '=' && savedFile[i][j+1]== '=') tempComplexity++;
            else if(savedFile[i][j] == '>' && savedFile[i][j+1]== '=') tempComplexity++;
            else if(savedFile[i][j] == '<' && savedFile[i][j+1]== '=') tempComplexity++;
            else if(savedFile[i][j] == '!' && savedFile[i][j+1]== '=') tempComplexity++;
        }
    }

    return tempComplexity;
}

int countSingleCharacaterPredicates(int firstLineNo, int lastLineNo) {

    int tempComplexity = 0;
    for(int i= firstLineNo; i <= lastLineNo; i++) {

        int tempLineSize = savedFile[i].size() - 1; // We are checking one characters at a time for > and <
        for(int j=0; j<tempLineSize; j++) {

            if(savedFile[i][j] == '>' && savedFile[i][j+1]== '>') continue;
            else if(savedFile[i][j] == '>' && savedFile[i][j+1]== '=') continue;
            else if(savedFile[i][j] == '<' && savedFile[i][j+1]== '=') continue;
            else if(savedFile[i][j] == '<' && savedFile[i][j+1]== '<') continue;
            else if(savedFile[i][j] == '!' && savedFile[i][j+1]== '=') continue;
            else if(savedFile[i][j] == '<') tempComplexity++;
            else if(savedFile[i][j] == '>') tempComplexity++;
            else if(savedFile[i][j] == '!') tempComplexity++;
        }
    }

    return tempComplexity;
}

int countPredicates(int firstLineNo, int lastLineNo) {

    int tempComplexity = 1;         // Initial Complexity of a method

    /* Only for two character operators Detection*/
    tempComplexity += countDoubleCharacaterPredicates(firstLineNo, lastLineNo);
    /* Only for single character operators Detection*/
    tempComplexity+= countSingleCharacaterPredicates(firstLineNo, lastLineNo);
    /* For switch Statements*/
    for(int i=firstLineNo; i<=lastLineNo; i++){

        if(savedFile[i].find("case") != string::npos ) {
            tempComplexity++;
        }
        else if(savedFile[i].find("stack") != string::npos) {
            tempComplexity-=2;
        }
    }

    /* Adding the complexity of other methods that are called inside it
    for (int i=firstLineNo; i<=lastLineNo; i++){
        tempComplexity+= (savedFile[i]);
    }*/

    return tempComplexity;
}

/* Find total number of occurrences of a key in a word*/
int findTotalOccurences(string word, string key) {

    int keyLen = key.size();
    int lineLen = word.size() - keyLen + 1;
    int counter = 0; // stores the total number of occurrences of a key

    for(int i=0; i<lineLen; i++) {

        bool flag = true;
        for(int j=0; j<keyLen; j++) {
            if(word[i+j] != key[j]) {
                flag = false;
                break;
            }
        }

        if(flag == true) counter++;
    }

    return counter;
}

//Checks the keyWords occurrences
int findTotalKeyWords(string line) {

    string keys []  = {  "if(",  "for(",  "while(",  "case",  "&&",  "||"};
    int counter = 0;

    for(int i=0; i<6; i++) { /* Total number of keys = 3 */

        stringstream iss(line);
        string word;
        while(iss>>word) {
            counter += findTotalOccurences(word, keys[i]);
        }

    }

    return counter;
}

/**Calculates complexity using another approach*/
int countPredicatesOther(int firstLineNo, int lastLineNo) {

    int tempComplexity = 1;         // Initial Complexity of a method

    for(int i = firstLineNo; i <= lastLineNo; i++) {
        tempComplexity += findTotalKeyWords(savedFile[i]);
    }

    return tempComplexity;
}

void calculation(void) {

    int tempSize = methodTracer.size();         //Total number of detected methods

    for(int i=0; i<tempSize; i++) {

        int firstLineNo = methodTracer[i].startLine;
        int lastLineNo  = methodTracer[i].finishLine;

        int complexity1 = countPredicates(firstLineNo, lastLineNo);  // Returns the complexity of a method
        int complexity2 = countPredicatesOther(firstLineNo, lastLineNo); // Returns the complexity of a method in another approach

        methodTracer[i].complexity = max(complexity1, complexity2);    //Assigns the highest complexity of the two approach
    }

}

void saveInString(char* inputFile) {

    saveFileSize=0; //Initialize the number of lines
    ifstream iFile(inputFile);  //Opening the input file

    if(iFile.is_open()) {

        string line;
        getline(iFile,line);

        while(!iFile.eof()){    //Reads as long as we do not reach the end of file

            savedFile[saveFileSize++] = line;
            //Saving file in string
            getline(iFile, line);

        }
        iFile.close();      //Closes the inout file
    }
    else printf("File not found\n");    //If no input file is found for the correspondent fileName

}

/******************************* Method detection and naming **************************************/

string findStringBeforeFirstBrace(string line){

    int firstBracePosition = -1;    //Location of (
    int len = line.size();

    for(int i=0; i<len; i++){

        if( line[i] == '(' ) {
            firstBracePosition = i;
        }
        else if( line[i] == '{' ) {
            line[i] = ' ';  //Making the brace empty
            break;
        }
    }

    if(firstBracePosition != -1 ) return line.substr(0,firstBracePosition);
    else return "";
}


void assignName(void){

    int tempSize = methodTracer.size();//Size of total methods
    for(int i=0; i<tempSize; i++) {

        int firstLine = methodTracer[i].startLine;  //First line number of a method
        string tempWords[10];   // to store words obtained form analyzing the lines

        int counter;
        string word;
        /** If we found the method name in the line of second brace*/
        istringstream iss( findStringBeforeFirstBrace(savedFile[firstLine]) );  /** Check whether name exits with the second brace*/
        for(counter = 0 ; iss>>word; counter++) {
            tempWords[counter] = word;
        }

        if(counter > 1) {
            methodTracer[i].name = tempWords[1]; /** method name will exist as second word*/
            nameAssign(tempWords[1], i);
            methodTracer[i].nameFlag = true;
            continue;
        }


        /** If we found the method name before the line of second brace*/
        istringstream iss1( findStringBeforeFirstBrace(savedFile[firstLine-1]) );  /** Check whether name exits above the second brace*/
        for(counter = 0 ; iss1>>word; counter++) {
            tempWords[counter] = word;
            nameAssign(tempWords[1], i);
        }
        if(counter > 1) {
            methodTracer[i].name = tempWords[1]; /** method name will exist as second word*/
            methodTracer[i].nameFlag = false;

        }


    }

}


void methodDetector(char* inputFile) {

    saveInString(inputFile);    /* Keeps a copy of the comment free line in string array*/

    stack <char> doubleQoute;   //Saves double quotes
    stack <char> firstBracket;  //Saves first braces
    methodTracer.clear();       /** Clears the method tracer */

    for(int i=0; i<saveFileSize; i++) {

        int strLen = savedFile[i].size();


        for(int j=0; j< strLen; j++) {

            if( doubleQoute.empty() && savedFile[i][j]=='{' && firstBracket.size() == 2) {
                tempNode.startLine = i;
                doubleQoute.push('{');

            }
            else if(doubleQoute.size() == 1 && savedFile[i][j]=='}' && firstBracket.size() == 2) {
                tempNode.finishLine = i;
                methodTracer.push_back(tempNode);
                doubleQoute.pop();
                firstBracket.pop();
                firstBracket.pop();

            }
            else if(savedFile[i][j] == '{') {

                doubleQoute.push('{');
            }
            else if(savedFile[i][j] == '}') {

                doubleQoute.pop();
            }
            /**Checks for () Because no struct or array contains () but it contains {} */
            else if(savedFile[i][j] == '(' && doubleQoute.empty() && firstBracket.empty() ) {
                firstBracket.push('(');
            }
            else if(savedFile[i][j] == ')' && doubleQoute.empty() && firstBracket.size()==1) {
                firstBracket.push(')');
            }

        }

    }
    /*** Detects methods and pushes the nodes keeping those info**/

    /** Assign name to each methods*/
    assignName();
}

void findComplexity(void) {

    //removeDoubleQuoteContents(commentFreeFile);      /**  Removes contents inside double quotes  */
    methodDetector(commentFreeFile);    /**Detects method start and finish location and saves it*/
    calculation();
    saveMethodArea();  //Saves the method area range
    calculateAverageComplexity(); //Calculates the average complexity result
}
