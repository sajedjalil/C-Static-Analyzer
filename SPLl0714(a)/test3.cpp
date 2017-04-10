#include <cstdio>
#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

//method data
int totalMethod;
extern int tempTotalStatement;

stack <char> secondBraceSave;

bool methodBraceFlag = false;
bool multipleLineCommentFlagForMethod = false;
bool multipleLineCommentFlag2 = false;

/*
result data
  |  |   |
node for method wise data calculation
*/

struct Node {

    int blankLine ;
    int onlyCommentLine ;
    int logicalStatements ;
    int physicalLine;
    int commentAndStatementLines;
    int onlyStatmentLines;
    int tempTotalStatement;

} tempNode;

vector <Node> methodResult;  // method wise result calculation


/*
    File1 i.e. main file methods

    Use these methods to detect method wise calculation
    these result will be saved in a struct and will be pushed in a vector
*/
extern void singleLineAnalyzer (string &temp);
extern bool emptyLineCounter(string &temp, int startIndex, int finalIndex);
extern void assignLineType(bool &flagStatementLine, bool &flagCommentLine);




/*  *** Methods that are defined in this file *** */

//Initializes tempNode result to zero
void tempNodeInitializer (void) {

    tempNode.blankLine = 0;
    tempNode.onlyCommentLine = 0;
    tempNode.logicalStatements = 0;
    tempNode.physicalLine = 0;
    tempNode.commentAndStatementLines = 0;
    tempNode.onlyStatmentLines = 0;
    tempNode.tempTotalStatement = 0;

}

void resultPrinter(Node &tempNode) {

    printf("Method No: %d\n\n", totalMethod);
    printf("Physical Lines: %d\nBlank Lines: %d\nLines of Logical Statement: %d\nLines with only Comment: %d\nLines with both Comment and Statement: %d\nTotal Statements: %d\n\n"
           , tempNode.physicalLine,  tempNode.blankLine,  tempNode.onlyStatmentLines,  tempNode.onlyCommentLine,  tempNode.commentAndStatementLines,  tempNode.logicalStatements);
}

/* This method is same as singleLineAnalyzer in file1
    Defined here again because had problem in linking. I should have use OOP because of this -_-
*/
int onlyStatementCounter2(string &temp , int startIndex, int finalIndex ) {

    int len = finalIndex;
    int statementAfterCommentFlag = 0;  //will store the position of first occurring */
    tempTotalStatement=0;

    if(multipleLineCommentFlag2 == true) {

        for(int i=0;  i<(len-1);  i++) {
            if(temp[i]=='*' && temp[i+1]=='/') {
                statementAfterCommentFlag = i+2;  //will store the position of first occurring */
                break;
            }
        }                                                                   // call to method Multiple Comment & statement checker for  dfdsf*/ a+=b;

        if(statementAfterCommentFlag == 0) return tempTotalStatement;

    }

    stack <char > saveSymbol;

    bool flagOverCounting = false; // for avoiding Over Counting in cases like ( );

    for(int i=statementAfterCommentFlag; i<len; i++) {

        if(saveSymbol.size() == 0) {

            if(temp[i] ==';' && flagOverCounting == false ) tempTotalStatement++;
            else if(temp[i] == '(' ) saveSymbol.push(temp[i]);
            else if(temp[i] == '"' ) saveSymbol.push(temp[i]);
            else if(i != (len-1)  &&  temp[i]=='/'  &&  temp[i+1]=='*') saveSymbol.push('*'); //when we detect  /*  Comment part
            if(i != (len-1) && temp[i] == '/' && temp[i+1] =='/') return tempTotalStatement; //when we detect singleLine Comment

        }
        else {

            if(temp[i] == ')' && saveSymbol.top() == '(' ) {
                saveSymbol.pop();
                tempTotalStatement++;
                flagOverCounting = true;
            }
            else if(temp[i] == '"' && saveSymbol.top() == '"') {
                saveSymbol.pop();
            }
            else if(i != (len-1) && temp[i] == '*' && temp[i+1] =='/' && saveSymbol.top() == '*') saveSymbol.pop();
        }

    }

    return tempTotalStatement;
}

bool onlyCommentLineFinder2(string &temp , int startIndex, int finalIndex ) {

    int len = finalIndex-1;
    bool flag1 = false ,  flag2 = false ,  countAsCommentLine = false;//for not detecting printf("//");

    if(multipleLineCommentFlag2 == true) countAsCommentLine = true;


    for(int i=0; i<len; i++) {

        if(temp[i]==' ') continue;
        else if(flag2==false) { //When not inside a (" ");

            if(temp[i]=='/' && temp[i+1]=='/') return true;
            else if(temp[i]=='/' && temp[i+1]=='*') {
                multipleLineCommentFlag2 = true;
                countAsCommentLine = true;
            }
            else if(temp[i]=='*' && temp[i+1]=='/') {
                multipleLineCommentFlag2 = false;
            }
            else if(temp[i]== '"') flag2 = true;

        }
        else if(temp[i]== '"' && flag2 == true) flag2 = false;

    }

    if(countAsCommentLine == true) return true;
    return false;

}

void methodLevelCalculation(string &temp , int startIndex , int finalIndex){


    if(emptyLineCounter(temp, startIndex, finalIndex) == true ) tempNode.blankLine++;
    else {

            bool flagCommentLine  = false;
            bool flagStatementLine = false;
            tempNode.physicalLine++;

            if(onlyStatementCounter2(temp, startIndex, finalIndex) > 0) {
                    tempNode.logicalStatements+=tempTotalStatement;
                    flagStatementLine = true;

            }

            if(onlyCommentLineFinder2(temp, startIndex, finalIndex) == true) {
                    tempNode.onlyCommentLine++;
                    flagCommentLine = true;

            }


            // For Detection of lines with both comment & statements and also to count total onlyStatementLines
             if(flagStatementLine == true && flagCommentLine == true)  {
                    tempNode.commentAndStatementLines++;
                    tempNode.onlyCommentLine--;

            }
            else if(flagStatementLine == true) {
                    tempNode.onlyStatmentLines++;
            }
    }

}

/*
    This method separates the Start point and Endpoint of a method
    Enables to calculate method based LOC metric calculation
*/
void methodSeparator (string &temp) {

    bool doubleQuoteFlag = false;
    bool methodFinishFlag = false;
    int len = temp.size();
    int methodStartIndex = 0,  methodFinishIndex = len; //Flags the start & end index of a method


    for(int i=0; i<len; i++) {

         if(doubleQuoteFlag == false ) {


            if(multipleLineCommentFlagForMethod == false ) {
                //****************************************************************************************************************************
                if( (i+1) != len && temp[i] == '/' && temp[i+1]=='*')  multipleLineCommentFlagForMethod =  true;  /* ***Handles multiple line comment ***** */
                else if((i+1) != len && temp[i] == '/' && temp[i+1]=='/')   break;  /* ****Handles single line comment****** */
                else if(temp[i]=='"') doubleQuoteFlag = true;
                // when we find { ... it may be of method brace or block brace of other loops or function
                else if(temp[i] == '{') {
                    //  when the brace({) is the start brace for a method
                    if(secondBraceSave.empty()) {

                        tempNodeInitializer();  //Initializes  tempNode variables to zero

                        methodStartIndex = i;
                        methodBraceFlag = true;
                    }

                    secondBraceSave.push('{');
                }
                // when we find } ... it may be of method brace or block brace of other loops or function
                else if(temp[i] == '}') {
                    secondBraceSave.pop();
                    //  when the brace(}) is the end brace for a method
                    if(secondBraceSave.empty()) {
                        // ?? push
                        totalMethod++;
                        methodFinishIndex = i+1;  //adding 1 so that it gives the length from start to finishIndex
                        methodBraceFlag = false;
                        methodFinishFlag = true;
                    }
                }
                //****************************************************************************************************************************
            }
            else if((i+1) != len && temp[i] == '*' && temp[i+1]=='/' ) multipleLineCommentFlagForMethod =false;

        }
        else {
            if(temp[i] == '"') doubleQuoteFlag = false;
        }

    }

    /* *********************************************************** */
    methodLevelCalculation(temp , methodStartIndex , methodFinishIndex );
    /* Printing and Saving of result */
    if(methodFinishFlag == true ) {
            methodResult.push_back(tempNode);
            resultPrinter(tempNode);
            methodFinishFlag = false;
    }

}
