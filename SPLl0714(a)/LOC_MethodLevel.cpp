#include <cstdio>
#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

//method data
int totalMethod;
extern int tempTotalStatement;

stack <char> secondBraceSave;
stack <char> firstBrace;    //separates structs and arrays from method as methods have () but array and struct don't

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

    string name;

} locNode;

vector <Node> methodResult;  // method wise result calculation


/**

    Use these methods to detect method wise calculation
    these result will be saved in a struct and will be pushed in a vector
*/
extern void singleLineAnalyzer (string &temp);
extern bool emptyLineCounter(string &temp, int startIndex, int finalIndex);
extern void assignLineType(bool &flagStatementLine, bool &flagCommentLine);



/*  *** Methods that are defined in this file *** */

bool findPositionOfMethodName(string temp) {

    int tempLen = temp.size();
    for(int i=0; i<tempLen; i++) {

        if(temp[i] == '{') {
            temp[i] = ' ';
            temp = temp.substr(0, i);
            break;
        }
    }

    stringstream iss(temp);
    string word;
    int counter =0;

    while(iss>>word) {
        counter++;
    }

    if(counter > 1 ) return true;
    else return false;
}

//Clear data for methods
void clearLoc(void) {
    methodResult.clear();
}
//Initializes locNode result to zero
void locNodeInitializer (string temp) {

    if( findPositionOfMethodName(temp) == true ) locNode.logicalStatements = -1;
    else locNode.logicalStatements = 0;

    locNode.blankLine = 0;
    locNode.onlyCommentLine = 0;
    locNode.physicalLine = 0;
    locNode.commentAndStatementLines = 0;
    locNode.onlyStatmentLines = 0;
    locNode.tempTotalStatement = 0;

}

/* Assigns the name of the method calculated from cyclomatic complexity*/
void nameAssign (string name ,int methodNumber) {
    methodResult[methodNumber].name = name;
}

void locMethodBasedResult(int i){

    cout<<"Method Name: "<<methodResult[i].name<<endl<<endl;
    printf("Physical Lines: %d\n", methodResult[i].physicalLine );
    printf("Blank Lines: %d\n", methodResult[i].blankLine);
    printf("Lines of Logical Statement: %d\n", methodResult[i].logicalStatements);
    printf("Lines with only Comment: %d\n", methodResult[i].onlyCommentLine);
    printf("Lines with both Comment and Statement: %d\n", methodResult[i].commentAndStatementLines);
    printf("Total Statement Lines: %d\n\n",  methodResult[i].onlyStatmentLines);

}

int getLoc(int i) {

    return methodResult[i].logicalStatements;
}

int findTheEndOfComment2(string &temp , int startIndex, int len ) {

    if(multipleLineCommentFlag2 == true) {

        for(int i=startIndex;  i<(len-1);  i++) {
            if(temp[i]=='*' && temp[i+1]=='/') {

                return (i+2);  //will store the position of first occurring
            }
        }
    }

    return 0;
}

/* This method is same as singleLineAnalyzer in file1
    Defined here again because had problem in linking. I should have use OOP because of this -_-
*/
int onlyStatementCounter2(string &temp , int startIndex, int finalIndex ) {

    int len = finalIndex;
    int statementAfterCommentFlag = 0;  //will store the position of first occurring */
    tempTotalStatement=0;

    statementAfterCommentFlag =  findTheEndOfComment2(temp, startIndex, len);

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

bool onlyCommentLineFinder2(string &temp , int startIndex, int finalIndex ) {

    int len = finalIndex-1;
    bool flag2 = false ,  countAsCommentLine = false;//for not detecting print f("//");

    if(multipleLineCommentFlag2 == true) countAsCommentLine = true;


    for(int i=startIndex ; i<len; i++) {

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


    if(emptyLineCounter(temp, startIndex, finalIndex) == true ) locNode.blankLine++;
    else {

            bool flagCommentLine  = false;
            bool flagStatementLine = false;
            locNode.physicalLine++;

            if(onlyStatementCounter2(temp, startIndex, finalIndex) > 0) {
                    locNode.logicalStatements+=tempTotalStatement;
                    flagStatementLine = true;

            }

            if(onlyCommentLineFinder2(temp, startIndex, finalIndex) == true) {
                    locNode.onlyCommentLine++;
                    flagCommentLine = true;

            }


            // For Detection of lines with both comment & statements and also to count total onlyStatementLines
             if(flagStatementLine == true && flagCommentLine == true)  {
                    locNode.commentAndStatementLines++;
                    locNode.onlyCommentLine--;

            }
            else if(flagStatementLine == true) {
                    locNode.onlyStatmentLines++;
            }
    }

}

/**
    This method separates the Start point and Endpoint of a method
    Enables to calculate method based LOC metric calculation
*/
/** Ignores the contents inside double quote*/
int doubleQouteContentRemover(string temp, int currentPosition, int len) {

    for(int i=currentPosition+1;  i<len;  i++) {
        if(temp[i] == '"') {
            currentPosition=i+1;
            return currentPosition;
        }
    }

    return currentPosition;
}

void methodSeparator (string &temp) {

    //bool doubleQuoteFlag = false;
    bool methodFinishFlag = false;
    int len = temp.size();
    int methodStartIndex = 0,  methodFinishIndex = len; //Flags the start & end index of a method


    for(int i=0; i<len; i++) {
        /** Ignores the contents inside double quote*/
        if(multipleLineCommentFlagForMethod == false && temp[i] =='"' ) {
            i = doubleQouteContentRemover(temp, i, len);         /** Returns the location after the end of double quote*/

        }

        if(multipleLineCommentFlagForMethod == false ) {


           /****************************************************************************************************************************/
            if( (i+1) != len && temp[i] == '/' && temp[i+1]=='*')  multipleLineCommentFlagForMethod =  true;  /* ***Handles multiple line comment ***** */
            else if((i+1) != len && temp[i] == '/' && temp[i+1]=='/')   break;  /* ****Handles single line comment****** */
            // when we find { ... it may be of method brace or block brace of other loops or function
            else if(temp[i] == '(' && temp[i+1] != 39 && firstBrace.empty() && secondBraceSave.empty() ) {
                firstBrace.push('(');
                //cout<<temp<<" "<<firstBrace.size()<<endl;
            }
            else if(temp[i] == ')' && temp[i+1] != 39 && firstBrace.size()==1 && secondBraceSave.empty() ){
                firstBrace.push(')');
            }
            else if(temp[i] == '{' && temp[i+1] != 39  ){      /*  39 is equivalent to '  */
                //  when the brace({) is the start brace for a method
                if(secondBraceSave.empty() && firstBrace.size()==2 ) {

                    locNodeInitializer(temp);  //Initializes  locNode variables to zero

                    methodStartIndex = i;
                    methodBraceFlag = true;
                }

                secondBraceSave.push('{');
            }
            // when we find } ... it may be of method brace or block brace of other loops or function
            else if(temp[i] == '}'  && temp[i+1] != 39  ) {    /*  39 is equivalent to '  */

                if( !secondBraceSave.empty()) secondBraceSave.pop();
                //  when the brace(}) is the end brace for a method
                if(secondBraceSave.empty() && firstBrace.size()==2 ) {

                    /** Clears the stack of firstBrace */
                    firstBrace.pop();
                    firstBrace.pop();
                    // ?? push
                    totalMethod++;
                    methodFinishIndex = i+1;  //adding 1 so that it gives the length from start to finishIndex
                    methodBraceFlag = false;
                    methodFinishFlag = true;
                }
            }
            //****************************************************************************************************************************
        }
        else if ((i+1) != len && temp[i] == '*' && temp[i+1]=='/' ) {
                multipleLineCommentFlagForMethod =false;
        }

    }

    /* *********************************************************** */
    methodLevelCalculation(temp , methodStartIndex , methodFinishIndex );
    /* Printing and Saving of result */
    if(methodFinishFlag == true ) {
            methodResult.push_back(locNode);
            methodFinishFlag = false;
    }
}

