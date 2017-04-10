#include <cstdio>
#include <string>
#include <stack>
#include <fstream>
#include <iostream>

using namespace std;

bool multipleLineCommentFlag;

//result data
/*
    Overall result data
*/
int blankLine;
int physicalLine;
int logicalStatements;
int onlyCommentLine;
int commentAndStatementLines;
int onlyStatmentLines;

int tempTotalStatement;    //for counting total logical Statements in a line


// Other file Data
extern void methodSeparator (string &temp);
extern int totalMethod;
extern void clearLoc(void);
//Start of Own file Methods

bool onlyCommentLineFinder(string &temp , int startIndex, int finalIndex ) {

    int len = finalIndex-1;
    bool flag2 = false ,  countAsCommentLine = false;//for not detecting printf("//");

    if(multipleLineCommentFlag == true) countAsCommentLine = true;


    for(int i=startIndex; i<len; i++) {

        if(temp[i]==' ') continue;
        else if(flag2==false) { //When not inside a (" ");

            if(temp[i]=='/' && temp[i+1]=='/') return true;
            else if(temp[i]=='/' && temp[i+1]=='*') {
                multipleLineCommentFlag = true;
                countAsCommentLine = true;
            }
            else if(temp[i]=='*' && temp[i+1]=='/') {
                multipleLineCommentFlag = false;
            }
            else if(temp[i]== '"') flag2 = true;

        }
        else if(temp[i]== '"' && flag2 == true) flag2 = false;

    }

    if(countAsCommentLine == true) return true;
    return false;

}

bool emptyLineCounter(string &temp, int startIndex, int finalIndex) {

    if(finalIndex == 0) return true;
    else {

        for(int i=startIndex; i<finalIndex; i++) {

            if(temp[i] == ' ' ) continue;
            else return false;
        }

        return false;
    }
}


int findTheEndOfComment(string &temp , int startIndex, int len ) {

    if(multipleLineCommentFlag == true) {

        for(int i=startIndex;  i<(len-1);  i++) {
            if(temp[i]=='*' && temp[i+1]=='/') {

                return (i+2);  //will store the position of first occurring
            }
        }
    }

    return 0;
}

int onlyStatementCounter(string &temp , int startIndex, int finalIndex ) {

    int len = finalIndex;
    int statementAfterCommentFlag = 0;  //will store the position of first occurring */
    tempTotalStatement=0;

    statementAfterCommentFlag =  findTheEndOfComment(temp, startIndex, len);
    statementAfterCommentFlag =  findTheEndOfComment(temp, startIndex, len);

    stack <char> saveSymbol;

    for(int i=statementAfterCommentFlag; i<len; i++) {

        if( saveSymbol.empty() ) {

            if(temp[i] ==';'  && temp[i-1] != ')' ) {
                tempTotalStatement++;
            }
            else if(temp[i] == '(' ) saveSymbol.push(temp[i]);
            else if(temp[i] == '"' ) saveSymbol.push(temp[i]);
            else if(i != (len-1)  &&  temp[i]=='/'  &&  temp[i+1]=='*') saveSymbol.push('*'); //when we detect  /*  Comment part
            if(i != (len-1) && temp[i] == '/' && temp[i+1] =='/') return tempTotalStatement; //when we detect singleLine Comment

        }
        else {

            if(temp[i] == ')' && saveSymbol.top() == '(' ) {
                saveSymbol.pop();
                tempTotalStatement++;
            }
            else if(temp[i] == '"' && saveSymbol.top() == '"') {
                saveSymbol.pop();
            }
            else if(i != (len-1) && temp[i] == '*' && temp[i+1] =='/' && saveSymbol.top() == '*')  saveSymbol.pop();
        }

    }

    return tempTotalStatement;
}

void assignLineType(bool &flagStatementLine, bool &flagCommentLine) {

    if(flagStatementLine == true && flagCommentLine == true)  {
            commentAndStatementLines++;
            onlyCommentLine--;

    }
    else if(flagStatementLine == true) {
            onlyStatmentLines++;

    }
}

void singleLineAnalyzer (string &temp, int startIndex, int finalIndex) {


    if(emptyLineCounter( temp, startIndex, finalIndex )== true) blankLine++;
    else {

        bool flagCommentLine = false;
        bool flagStatementLine = false;
        physicalLine++;

        if(onlyStatementCounter(temp , startIndex, finalIndex )>0) {

            logicalStatements+=tempTotalStatement;
            flagStatementLine = true;

        }

        if(onlyCommentLineFinder(temp , startIndex, finalIndex ) == true) {

            onlyCommentLine++;
            flagCommentLine = true;
        }


        assignLineType( flagStatementLine , flagCommentLine );  //Checks whether each line contains both methods and comment
    }


    /**
        ************************************************************
        This portion directs the program to calculate per method result*****
        ************************************************************
    */
    methodSeparator(temp);     /* If line contains no comment*/

}

void locResultPrinter(void) {
    printf("\n\n");

    printf("Physical Lines: %d\n", physicalLine );
    printf("Blank Lines: %d\n", blankLine);
    printf("Lines of Logical Statement: %d\n", logicalStatements);
    printf("Lines with only Comment: %d\n", onlyCommentLine);
    printf("Lines with both Comment and Statement: %d\n", commentAndStatementLines);
    printf("Total Statement Lines: %d\n",  onlyStatmentLines);
    printf("Total Methods: %d\n\n", totalMethod);

}

void cAnalyzer(char* fileName) {

    ifstream iFile(fileName);

    if(iFile.is_open()) {
        string temp;

        getline(iFile,temp);

        while(!iFile.eof()){

            singleLineAnalyzer(temp, 0 , temp.size());
            getline(iFile, temp);

        }

        iFile.close();
    }
    else printf("File not found\n");


}

void cPlusPlusAnalyzer(char* fileName) {

    ifstream iFile(fileName);

    if(iFile.is_open()) {

        string temp;
        getline(iFile,temp);

        while(!iFile.eof()){

            singleLineAnalyzer(temp, 0 , temp.size());
            getline(iFile, temp);

        }

        iFile.close();

    }
    else printf("File not found\n");
}


void allVariableInitializer ( void ) {

    multipleLineCommentFlag = false;
    tempTotalStatement=0;
    blankLine=0;
    onlyCommentLine=0;
    logicalStatements =0;
    physicalLine = 0;
    commentAndStatementLines = 0 ;
    onlyStatmentLines =0;

    totalMethod = 0;
    clearLoc();
}

int LinesOfCode(char* filename, int analyzer) {


    allVariableInitializer( );

    if(analyzer == 1) {
        cAnalyzer(filename);
    }
    else if(analyzer ==2 ) {
        cPlusPlusAnalyzer(filename);
    }


	return 0;
}
