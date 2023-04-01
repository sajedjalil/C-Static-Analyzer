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
#include <cmath>

using namespace std;

struct methodArea{
    int firstLine;
    int lastLine;
} tempArea;

struct resultNode{
    int uniqueOperators;
    int uniqueOperands;

    int operators;
    int operands;

    float programLength;
    float volume;
    float difficulty;
    float effort;
    float timeRequired;
    float numberOfBugs;

} tempResult, overallHalsteadResult;

vector <string> fileSave;
vector <methodArea> areaSave; // Saves the first and last line of the method area
vector <resultNode> savedResult; //method based calculation are stored here
vector <string> keyWords; /*Required in times of operands counting */

/*************** Getters *************************************/

float getMainVolume (void) {

    return overallHalsteadResult.volume;
}

/*************** Other file methods  & variables**************/
extern string commentFreeFileInput(string line);
extern char commentFreeFile[];
extern char fileForHalstead[];
extern char methodAreaFile[];
char detailedHalsteadFile[] = "detailed.txt"; //stores the operator and operands temporarily

/*************** Result Generation Related Methods **************/

void printHalsteadResult(int i) {

    printf("Unique Operators: %d\n",savedResult[i].uniqueOperators);
    printf("Unique Operands: %d\n",savedResult[i].uniqueOperands);
    printf("Total Operators: %d\n",savedResult[i].operators);
    printf("Total Operands: %d\n",savedResult[i].operands);

    printf("Program Length: %.2f\n",savedResult[i].programLength);
    printf("Volume: %.2f\n",savedResult[i].volume);
    printf("Difficulty: %.2f\n",savedResult[i].difficulty);
    printf("Effort: %.2f\n",savedResult[i].effort);
    printf("Time Required: %.2f\n",savedResult[i].timeRequired);
    printf("Number of Bugs: %.2f\n\n",savedResult[i].numberOfBugs);


}

void printHalsteadMain(void) {

        int totalUniqueOperators = 0;
        int totalUniqueOperands = 0;
        int totalOperators = 0;
        int totalOperands = 0;
        int programVocabulary = 0;
        for(int i=0; i < savedResult.size(); i++){
            totalUniqueOperators += savedResult[i].uniqueOperators;
            totalUniqueOperands += savedResult[i].uniqueOperands;
            totalOperators += savedResult[i].operators;
            totalOperands += savedResult[i].operands;
        }
        programVocabulary = totalUniqueOperators + totalUniqueOperands;

        printf("Unique Operators: %d\n",totalUniqueOperators);
        printf("Unique Operands: %d\n",totalUniqueOperands);
        printf("Total Operators: %d\n",totalOperators);
        printf("Total Operands: %d\n",totalOperands);
        printf("Program Length: %.2f\n", overallHalsteadResult.programLength);
        printf("Program Vocabulary:%d\n", programVocabulary);
        printf("Volume: %.2f\n", overallHalsteadResult.volume);
        printf("Difficulty: %.2f\n", overallHalsteadResult.difficulty);
        printf("Effort: %.2f\n", overallHalsteadResult.effort);
        printf("Time Required: %.2f\n", overallHalsteadResult.timeRequired);
        printf("Number of Bugs: %.2f\n\n", overallHalsteadResult.numberOfBugs);
}

double getHasteadVolume(int i) {

    return savedResult[i].volume;
}

void removeMapExtraSymbols (map <string, int> &mymap, string key) {

    map<string,int>::iterator it;
    it = mymap.find(key);
    if (it != mymap.end()) mymap.erase (it);

}

int getTotalCountOfElements(map <string, int> &anyMap){

    int counter = 0;
    for (map<string,int>::iterator it=anyMap.begin(); it!=anyMap.end(); ++it){
        counter += it->second;
    }

    return counter;
}


void saveBlockBasedResult(map <string, int> &methodOperators, map <string, int> &methodOperands) {

    removeMapExtraSymbols(methodOperators, ")");
    removeMapExtraSymbols(methodOperators,  "}");

    float n1, n2, n; /*unique operators, operands, total*/
    float N1, N2, N; /*Total operators, operands, total*/

    n1 = methodOperators.size();
    n2 = methodOperands.size();
    n  = n1 + n2;

    N1 = getTotalCountOfElements(methodOperators);
    N2 = getTotalCountOfElements(methodOperands);
    N  = N1 + N2;

    tempResult.uniqueOperators = n1;
    tempResult.uniqueOperands = n2;
    tempResult.operators = N1;
    tempResult.operands = N2;

    tempResult.programLength = (n1*log2(n1)) + (n2*log2(n2));
    tempResult.volume = N*(log2(n));
    tempResult.difficulty = (n1/2.0) * (N2/n2);
    tempResult.effort = tempResult.volume * tempResult.difficulty;
    tempResult.timeRequired = (tempResult.effort) / 18.0;
    tempResult.numberOfBugs = (pow(tempResult.effort, 2.0/3.0)) / 3000.0;

    savedResult.push_back(tempResult);

    /**Increment overall Result*/
    overallHalsteadResult.programLength += tempResult.programLength;
    overallHalsteadResult.volume += tempResult.volume;
    overallHalsteadResult.difficulty += tempResult.difficulty;
    overallHalsteadResult.effort += tempResult.effort;
    overallHalsteadResult.timeRequired += tempResult.timeRequired;
    overallHalsteadResult.numberOfBugs += tempResult.numberOfBugs;

}

/***************  Newly defined methods   **************/
void wordRemover(int lineNumber, string word) {

    int pos = fileSave[lineNumber].find(word);
    int wordSize = word.size()+pos ;

    for(int i = pos; i<wordSize; i++) fileSave[lineNumber][i] = ' ';
}

void getNumbers(map <string,int> &methodOperands, int lineNumber) {

    stringstream iss(fileSave[lineNumber]);
    string word;

    while(iss>>word) {

        string temp = word;
        int len = temp.size();

        int dotCounter = 0;
        bool flag = true;

        for(int i=0; i<len; i++) {
            if(temp[i] == '.' ) dotCounter++;       //Counts total number of dots
        }

        if(temp[len-1] == '.'  || dotCounter>1 ) flag= false;   //checks if the dot is at the last of word i.e. invalid float

        for(int i=0; i<len; i++) {
            if( (temp[i] <'0'  || temp[i] >'9') && temp[i] != '.') flag = false;      //checks for occurrences of characters other than numerical digit
        }

        if(flag==true) {

            pair<map<string,int>::iterator,bool> ret;
            ret = methodOperands.insert ( pair<string,int>(word, 1) );
            if (ret.second==false)  ret.first->second++ ;

            wordRemover(lineNumber, word);
        }
    }

}

void insertSpaces(int lineNumber) { /*Inserts spaces before and after of comma and semi colon*/

    string symbols [5] = {" ; ",  " , ",  " ( ",  " ) ", "="};

    for(int j=0; j<5; j++) {
        vector <int> positions;        //Saves the location of the symbols

        int lineSize = fileSave[lineNumber].size();
        for(int i=0; i<lineSize; i++) {

            if(fileSave[lineNumber][i] == symbols[j][1] ) {

                fileSave[lineNumber][i] = ' ';
                positions.push_back(i); //position of the comma
            }

        }

        int tempSize = positions.size();
        int increment = 1;
        for(int i=0; i<tempSize; i++)  {
            fileSave[lineNumber].insert( (positions[i]+increment) , symbols[j]);
            increment+=3;
        }

    }

}

void addQuotedOperands(map <string,int> &methodOperands, string line){

    int lineSize = line.size();

    bool flagDoubleQoute = false;
    bool flagSingleQoute = false;

    int startPosition = -1;
    int totalCharacters = 0;

    for(int i=0; i<lineSize; i++) {     /** removes the contents between single quote and double quote*/

        if( (line[i] == 39 || line[i] == '"') && flagDoubleQoute==false && flagSingleQoute == false ) {

            if(line[i] == 39) {
                flagSingleQoute = true;
                startPosition = i;
            }
            else if(line[i] == '"') {
                flagDoubleQoute = true;
                startPosition = i;
            }
            totalCharacters++;
            continue;
        }
        else if( flagDoubleQoute == true ) {

            totalCharacters++;

            if(line[i] == '"') {
                flagDoubleQoute = false;

                string temp = line.substr(startPosition,totalCharacters);
                pair<map<string,int>::iterator,bool> ret;
                ret = methodOperands.insert ( pair<string,int>(temp, 1) );
                if (ret.second==false)  ret.first->second++ ;

                totalCharacters = 0;
            }

        }
        else if( flagSingleQoute == true ) {

            totalCharacters++;

            if(line[i] == 39) {
                flagSingleQoute = false;

                string temp = line.substr(startPosition,totalCharacters);
                pair<map<string,int>::iterator,bool> ret;
                ret = methodOperands.insert ( pair<string,int>(temp, 1) );
                if (ret.second==false)  ret.first->second++ ;

                totalCharacters = 0;
            }

        }

    }
}

void separateOperands(map <string,int> &methodOperands, int lineNumber) {

    stringstream iss(fileSave[lineNumber]);
    string word;

    while (iss >> word) {

        pair<map<string,int>::iterator,bool> ret;
        ret = methodOperands.insert ( pair<string,int>(word, 1) );
        if (ret.second==false)  ret.first->second++ ;
        wordRemover(lineNumber, word);

    }

}

void separateOperators(map <string,int> &methodOperators, int lineNumber) {

    stringstream iss(fileSave[lineNumber]);
    string word, previous;
    int totalKeys = keyWords.size();

    while(iss>>word) {

        bool flagKey = false;
        for(int i=0; i<totalKeys ; i++) {

            if(keyWords[i] == word) {
                flagKey = true;

                pair<map<string,int>::iterator,bool> ret;
                ret = methodOperators.insert ( pair<string,int>(word, 1) );
                if (ret.second==false)  ret.first->second++ ;

                wordRemover(lineNumber, word);
                break;
            }
        }
        if (flagKey == true) continue;      /** If detected word is a keyword then we skip it*/

        /** For detecting method call with the presence of () */
        if(word[0] == '(' && (previous[0] != '(' && previous[0] != '=' ) ) {

            pair<map<string,int>::iterator,bool> ret;
            ret = methodOperators.insert ( pair<string,int>(previous, 1) );
            if (ret.second==false)  ret.first->second++ ;
            wordRemover(lineNumber, previous);

        }
        previous = word;

        /** add extra symbols*/
        if(word.size() == 1) {

            if(word[0] == '(' || word[0] == ')' || word[0] == '=') {

                pair<map<string,int>::iterator,bool> ret;
                ret = methodOperators.insert ( pair<string,int>(word, 1) );
                if (ret.second==false)  ret.first->second++ ;
                wordRemover(lineNumber, word);
            }
            else if(word[0] == ',' || word[0] == ';' ) wordRemover(lineNumber, word);
        }
    }

}

void removeGarbage (map <string, int> &methodOperators, map <string, int> &methodOperands, int lineNumber) {

    string doubleOperators[15]  = { "++", "--", "+=", "-=", "*=", "/=", "%=", "==", "!=", ">=", "<=",  "<<", ">>","||", "&&" };
    string singleOperators[16] = { "+", "-", "*", "/", "{", "}", "[", "]", "%", "!", "<", ">", "|","&", "~", "^" };
    int lineSize = fileSave[lineNumber].size();

    for(int i=0; i<15; i++) {

        for(int j=1; j<lineSize; j++) {  //it takes two characters at a time

            if(fileSave[lineNumber][j-1] == doubleOperators[i][0]  && fileSave[lineNumber][j] == doubleOperators[i][1]) {

                fileSave[lineNumber][j-1] = ' ';      //replace the symbol with space
                fileSave[lineNumber][j] = ' ';      //replace the symbol with space

                /** Add the symbol to map*/
                pair<map<string,int>::iterator,bool> ret;
                ret = methodOperators.insert ( pair<string,int>(doubleOperators[i], 1) );

                if (ret.second==false)  ret.first->second++ ;
            }
        }
    }

    for(int i=0; i<16; i++) {

        for(int j=0; j<lineSize; j++) {

            if(fileSave[lineNumber][j] == singleOperators[i][0]) {

                fileSave[lineNumber][j] = ' ';      //replace the symbol with space

                /** Add the symbol to map*/
                pair<map<string,int>::iterator,bool> ret;
                ret = methodOperators.insert ( pair<string,int>(singleOperators[i], 1) );

                if (ret.second==false)  ret.first->second++ ;
            }
        }
    }

//    for (map<string,int>::iterator it=methodOperators.begin(); it!=methodOperators.end(); ++it) cout << (string)it->first << " => " << it->second << '\n';
//    cout<<endl;
}
void lineAnalyzer(map <string, int> &methodOperators, map <string, int> &methodOperands, int lineNumber)
 {

    removeGarbage(methodOperators, methodOperands, lineNumber);
    insertSpaces(lineNumber); //Inserts spaces before and after of comma and semi colon
    getNumbers(methodOperands, lineNumber); // get numbers as operands

    separateOperators(methodOperators, lineNumber); //Maps the operators
    separateOperands(methodOperands, lineNumber); //Maps the operands

}

void blockBasedCalculation(int firstLine, int lastLine) {

    map <string, int> methodOperators;
    map <string, int> methodOperands;

    for(int i=firstLine;  i<=lastLine;  i++) {
        lineAnalyzer(methodOperators, methodOperands, i);
//        cout<<fileSave[i]<<endl;
    }

    ifstream iFile(fileForHalstead); //Calculate double Quoted and single quoted operands
    if(iFile.is_open()) {

        string line;
        getline(iFile,line);

        while(!iFile.eof()){
            addQuotedOperands(methodOperands, line);
            getline(iFile, line);
        }

        iFile.close();  //closing of input file
    }


    saveBlockBasedResult(methodOperators, methodOperands);  //Save the method based results

    ofstream oFile(detailedHalsteadFile, ios_base::app);
    if(oFile.is_open()) {

        oFile<<"Operators:"<<endl;
        for (map<string,int>::iterator it=methodOperators.begin(); it!=methodOperators.end(); ++it) oFile << (string)it->first << " => " << it->second << '\n';
        oFile<<endl;
        oFile<<"Operands:"<<endl;
        for (map<string,int>::iterator it=methodOperands.begin(); it!=methodOperands.end(); ++it) oFile << (string)it->first << " => " << it->second << '\n';
        oFile<<endl<<".......................................................\n";

        oFile.close();
    }

}

void ResultGenerator(void){

    int tempSize = areaSave.size();
    for(int i=0; i<tempSize; i++) {

        blockBasedCalculation(areaSave[i].firstLine, areaSave[i].lastLine);
    }
//
//    for(int i=0; i<savedResult.size(); i++) {
//        cout<<savedResult[i].numberOfBugs<<" ** "<<endl;
//    }
}

void pushKeys(void) {

    string temp [14] = {"int", "char", "float", "double", "void", "long", "short", "signed", "unsigned", "const", "register", "bool", "string", "return"};

    for(int i=0; i<14; i++) keyWords.push_back(temp[i]);

}

void getMethodArea(char* fileName) {

    ifstream iFile(fileName);

    if( iFile.is_open() ) {

        int starting, finishing;
        iFile>>starting>>finishing;

        while(!iFile.eof()){

            tempArea.firstLine = starting;
            tempArea.lastLine = finishing;
            areaSave.push_back(tempArea);

            iFile>>starting>>finishing;
        }
        iFile.close();
    }
    else cout<<"File fetching error\n";

}

void getFileData(char* fileName) {
    ifstream iFile(fileName);

    if( iFile.is_open() ) {

        string line;
        getline(iFile,line);

        while(!iFile.eof()){

            fileSave.push_back(line);
            getline(iFile, line);
        }
        iFile.close();
    }
    else cout<<"Could not open file/n";
}

void initializeHalstead(void) {

    areaSave.clear();
    savedResult.clear();
    keyWords.clear();

    overallHalsteadResult.programLength = 0;
    overallHalsteadResult.difficulty = 0;
    overallHalsteadResult.effort = 0;
    overallHalsteadResult.numberOfBugs = 0;
    overallHalsteadResult.volume = 0;
    overallHalsteadResult.timeRequired = 0;

}

void halstead(void) {

    initializeHalstead(); //Initializing all result calculating variables
    getFileData(commentFreeFile);
    getMethodArea(methodAreaFile);
    pushKeys();     /*Required in times of operands counting */

    ResultGenerator();

    fileSave.clear(); // Clearing the vector fileSave
}
