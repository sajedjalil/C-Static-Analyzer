#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<stack>
#include<queue>
#include<map>

using namespace std;

string fileOne,fileTwo;
int fileNumber;
bool flagForComment;

/************Language Detecting of Given File*****************/

string languageDetection(string fileName)
{
    string language="";
    int lengthOfFileName=fileName.length();
    int positionOfDot;

    for(int i=0;i<lengthOfFileName;i++)
    {
        if(fileName[i]=='.')
        {
            positionOfDot=i;
            break;
        }
    }

    for(int i=positionOfDot+1;i<lengthOfFileName;i++)
    {
        language+=fileName[i];
    }

    return language;
}

/**********************FINISH OF LANGUAGE DETECTION********************************/
/**********************************************************************************/

string userInterection()
{
    fileNumber=-1;

    cout <<"Please Select File Number :\n\t"
        <<"1)One File\n\t"
        <<"2)Two Files\n\n"
        <<"Enter Option Number : ";

    cin >> fileNumber;

    while(fileNumber>2 && fileNumber<1)
    {
        cout<<"You entered wrong number.Please Try Again!\n";
        cout <<"Please Select File Number :\n\t1)One File\n\t2)Two Files\n\nEnter Option Number : ";
        cin >> fileNumber;
    }

    getline(cin,fileOne); //for taking extra enter

    if(fileNumber==1)
    {
        cout<<"Enter the file name :";  //taking fileName located at same place
        getline(cin,fileOne);

        string languageOfFileOne=languageDetection(fileOne);
        if(languageOfFileOne!="c" && languageOfFileOne!="cpp" && languageOfFileOne!="java")
        {
            while(1)
            {
                cout <<"You Entered wrong file! try Again\n";
                cout<<"Enter the file name :";
                getline(cin,fileOne);
                languageOfFileOne=languageDetection(fileOne);
                if(languageOfFileOne=="c" || languageOfFileOne=="cpp" || languageOfFileOne=="java")break;
            }
        }
        return languageOfFileOne;
    }
    else
    {
       while(1)
       {
           cout<<"Enter First file name : ";
            getline(cin,fileOne);

            string languageOfFileOne=languageDetection(fileOne);
            if(languageOfFileOne!="c" && languageOfFileOne!="cpp" && languageOfFileOne!="java")
            {
                while(1)
                {
                    cout <<"You Entered wrong file! try Again\n";
                    cout<<"Enter the file name :";
                    getline(cin,fileOne);
                    languageOfFileOne=languageDetection(fileOne);
                    if(languageOfFileOne=="c" || languageOfFileOne=="cpp" || languageOfFileOne=="java")break;
                }
            }

            cout<<"Enter Second file name : ";
            getline(cin,fileTwo);

            string languageOfFileTwo=languageDetection(fileTwo);
            if(languageOfFileTwo!="c" && languageOfFileTwo!="cpp" && languageOfFileTwo!="java")
            {
                while(1)
                {
                    cout <<"You Entered wrong file! try Again\n";
                    cout<<"Enter the file name :";
                    getline(cin,fileTwo);
                    languageOfFileTwo=languageDetection(fileTwo);
                    if(languageOfFileTwo=="c" || languageOfFileTwo=="cpp" || languageOfFileTwo=="java")break;
                }
            }

            if(languageOfFileOne==languageOfFileTwo)return languageOfFileOne;
            cout<<"Files Mismatched!\n";
        }

    }
}

/****************REMOVING COMMENT FROM FILE************************************/

string removingComment(string u)
{
    int doubleQuote=0;
    string edited="";
    for(int i=0;i<u.length();i++)
    {
        u[i]=tolower(u[i]);

        if(u[i]=='/' && (u[i+1]=='/' || u[i+1]=='*') && i+1<u.length() && flagForComment==false)
        {
            for(int j=i-1;j>-1;j--)
            {
                if(u[j]=='"')doubleQuote++;
            }

            if(doubleQuote%2==1)
            {
                edited+=u[i];
                doubleQuote=0;
                continue;
            }
            else
            {
                if(u[i]=='/' && u[i+1]=='/')
                    return edited;
                else if(u[i]=='/' && u[i+1]=='*')
                {
                    i++;
                    flagForComment=true;
                }
            }
        }
        else if(u[i]=='*' && u[i+1]=='/' && flagForComment==true && i+1<u.length())
        {
            i++;
            flagForComment=false;
        }
        else if(flagForComment==false)
        {
            edited+=u[i];
        }
    }
    return edited;
}

/******************Saving Data From File**************************************/

void savingDataFromFile(int currentFileNumber)
{
    int line=0;
    if ( currentFileNumber == 1 )
    {
        ifstream iFile;            //making ifile for input
        iFile.open ( fileOne.c_str() ); //streaming file for input

        ofstream oFile( "fileOneWithoutComment.txt" );   //creating file for output stream

        string lineFromMainCode,statementWithoutComment;

        while ( getline(iFile,lineFromMainCode))
        {
            line++;
            oFile<<removingComment(lineFromMainCode)<<endl;
        }

        iFile.close();
        oFile.close();

    }
    else
    {
        ifstream iFile;            //making ifile for input
        iFile.open ( fileTwo.c_str() ); //streaming file for input

        ofstream oFile( "fileTwoWithoutComment.txt" );   //creating file for output stream

        string lineFromMainCode,statementWithoutComment;

        while ( getline(iFile,lineFromMainCode))
        {

            oFile<<removingComment(lineFromMainCode)<<endl;
        }

        iFile.close();
        oFile.close();
    }


}

/***************Exact Statement Matching Part**************************************/
void recognisingStatement(vector<string> &statements ,string line,vector<int> &lineNumber,int lineN)
{
    string edited="";
    int semicolonCounter=0;
    bool flag1=false,flagForfor=false;

    for(int i=0;i<line.length();i++)
    {
        if(line[i]==' ')   //extra space baad
        {
            continue;
        }
        else if(line[i]=='f' && line[i+1]=='o' && line[i+2]=='r' && i+2<line.length()&& flagForfor==false)  //For(;;) er jonno
        {
            flagForfor=true;
        }
        else if (flagForfor==true)  // jodi for er moddhe thake to ) na pawa porjonto ber howa jabe na
        {
            if(line[i]==';')
            {
                semicolonCounter++;
            }
            if(semicolonCounter==2)
            {
                if(line[i]==')')
                {
                    flagForfor=false;
                    semicolonCounter=0;
                }
            }
        }
        else if (line[i]>='a' && line[i]<='z' && flag1==false && flagForfor==false)
        {
            if(i!=0)
            {
                if((line[i-1]=='{' || line[i-1]=='}' || line[i-1]==';' || line[i-1]==' ' || line[i-1]==')'))
                {
                    edited+=line[i];
                    flag1=true;
                }
                else continue;
            }
            else
            {
                edited+=line[i];
                flag1=true;
            }
        }
        else if (flag1==true)
        {
            if(line[i]==';')
            {
                flag1=false;
                edited+=line[i];
                statements.push_back(edited);
                lineNumber.push_back(lineN);
                edited=' ';
            }
            else edited+=line[i];
        }
    }
}

void puttingStatementsInVector(vector<string> &statements,int currentFileNumber,vector<int> &lineNumber)
{
    if(currentFileNumber==1)
    {
        ifstream ifile("fileOneWithoutComment.txt");
        string codeLine;
        int i=0;
        while(getline(ifile,codeLine))
        {
            i++;
            recognisingStatement(statements ,codeLine,lineNumber,i);
        }
        ifile.close();
    }
    else
    {
        ifstream ifile("fileTwoWithoutComment.txt");
        string codeLine;
        int i=0;
        while(getline(ifile,codeLine))
        {
            i++;
            recognisingStatement(statements , codeLine,lineNumber,i);
        }
        ifile.close();
    }

}

void lineMatching(vector<string> statements1 , vector<string> statements2,vector<int> lineNumber1,vector<int> lineNumber2)
{
    vector<int>MatchedLineNumbers;
    bool initialeLine[10000]={false};

    if(fileNumber==1)
    {
        for(int i=0;i<statements1.size();i++)
        {
            for(int j=i+1;j<statements2.size();j++)
            {
                if(statements1[i] == statements2[j])
                {
                    if(initialeLine[j]==false)
                    {
                        initialeLine[i]=true;
                        MatchedLineNumbers.push_back(lineNumber2[j]);
                    }
                }
            }

            if(MatchedLineNumbers.size()!=0)
            {
                cout<<"Match Found : "<<"\tLine Numbers: "<<lineNumber1[i]<<"\t";
                for(int m=0;m<MatchedLineNumbers.size();m++)
                {
                    cout<<MatchedLineNumbers[m]<<" ";
                }
                cout<<endl;
            }
            MatchedLineNumbers.clear();
        }
    }
    else
    {
        for(int i=0;i<statements1.size();i++)
        {
            for(int j=0;j<statements2.size();j++)
            {
                if(statements1[i] == statements2[j])
                {
                    MatchedLineNumbers.push_back(lineNumber2[j]);
                }
            }

            if(MatchedLineNumbers.size()!=0)
            {
                cout<<"Match Found : "<<"\tLine Numbers: "<<lineNumber1[i]<<"\tFile Two: ";
                for(int m=0;m<MatchedLineNumbers.size();m++)
                {
                    cout<<MatchedLineNumbers[m]<<" ";
                }
                cout<<endl;
            }
            MatchedLineNumbers.clear();
        }
    }
}

/*********************************End of Exact statement matching*******************************/
/************************************************************************************************/
/************************Start of Exact Threshold Matching***************************************/

void thresholdMatching(vector<string> statements1 , vector<string> statements2,vector<int> lineNumber1,vector<int> lineNumber2)
{
    int threshold;

    cout<<"Please enter the threshold for the statements:";
    cin>>threshold;

    int matchNumber=0;

    if(fileNumber==1)
     {
        int m;
        for(int i=0;i<statements1.size();i++)
        {
            m=i;
            for(int j=i+1;j<statements2.size();j++)
            {
                if(statements1[m]==statements2[j])
                {
                    matchNumber++;
                    m++;
                    if(matchNumber==threshold)
                    {
                        for(int k=m-threshold;k<m;k++)
                        {
                            cout<< lineNumber1[k]<<" ";
                        }
                        cout<<"\t\t";
                        for(int k=(j-threshold)+1;k<=j;k++)
                        {
                            cout<< lineNumber2[k]<<" ";
                        }
                        cout<<endl;
                        matchNumber=0;
                        m=i;
                    }
                }
                else if(statements1[m]!=statements2[j])
                {
                    j-=matchNumber;
                    matchNumber=0;
                    m=i;
                }
            }
        }
    }
    else
    {
        int m;
        for(int i=0;i<statements1.size();i++)
        {
            m=i;
            for(int j=0;j<statements2.size();j++)
            {
                if(statements1[m]==statements2[j])
                {
                    matchNumber++;
                    m++;
                    if(matchNumber==threshold)
                    {
                        for(int k=m-threshold;k<m;k++)
                        {
                            cout<< lineNumber1[k]<<" ";
                        }
                        cout<<"\t\t";
                        for(int k=(j-threshold)+1;k<=j;k++)
                        {
                            cout<< lineNumber2[k]<<" ";
                        }
                        cout<<endl;
                        matchNumber=0;
                        m=i;
                    }
                }
                else if(statements1[m]!=statements2[j])
                {
                    j-=matchNumber;
                    matchNumber=0;
                    m=i;
                }
            }
        }
    }
}

/*************Replace Symbols*************************************/
char operators[9] = {'+','-','*','/','(',')','{','}',';'};

string replaceSymbols(string line)
{
	for(int i=0;i<9;i++)
	{
		for(int j=0; j<line.length(); j++)
		{
			if(operators[i]==line[j]) line[j] = ' ';
		}
	}
	return line;
}

/******************Tokenize String*******************************/
vector<string> tokenize(string line)
{
    istringstream iss(line);
    vector<string>words;
    string word;
    while(iss>>word)
    {
        words.push_back(word);
    }
    return words;
}
/**************************************************************************************/
/***********************Method Recognising Process*************************************/

struct method{
    string methodName;
    int startLineNumber;
    int FinishLineNumber;
    string methodDatatype;
    string methodInOneLine_actVar;
};

string dataTypeName[9]={"int","double","float","char","void","int[]","float[]","double[]","char[]"};
bool structFlag=false;

void MethodRecognising(string line,vector<method> &methodOfFile,stack<char> &brackets,vector<string>&allLinesOfFile,int lineNumber)
{
    for(int i=0;i<line.length();i++)
    {
        if(line[i]=='{' && brackets.empty()==true)
        {
            string words=replaceSymbols(line);
            vector<string>word=tokenize(words);
            method one;
            /**********A NEW Method*****************/

            if(i==0 || word.size()==0)
            {
                string pastline=allLinesOfFile[lineNumber-2];
                words=replaceSymbols(pastline);
                word.clear();
                word=tokenize(words);
                lineNumber--;
            }

            if(word[0]=="struct")
            {
                structFlag=true;
                brackets.push(line[i]);
                continue;
            }

            /*******Making a METHOD Node************************/

            one.startLineNumber=lineNumber;
            for(int j=0;j<9;j++)
            {
                if(word[0]==dataTypeName[j])
                {
                    one.methodDatatype=word[0];
                    break;
                }
            }
            one.methodName=word[1];
            methodOfFile.push_back(one);
            /*****pushing the method into vector*******************/

            brackets.push(line[i]);     /**pushing bracket in stack**/
        }

        /***if '{' is found and stack is not empty, just push ****/
        else if(line[i]=='{' && brackets.empty()==false)
        {
            brackets.push(line[i]);
        }

        /**if '}' is found and stack is not empty , just pop****/
        else if(line[i]=='}' && brackets.empty()==false)
        {
            brackets.pop();
            if(brackets.empty()==true &&structFlag==false)
            {
                methodOfFile[methodOfFile.size()-1].FinishLineNumber=lineNumber;
            }
            else if(brackets.empty()==true &&structFlag==true)
            {
                structFlag=false;
            }
        }
    }
}

void MethodRecogniseHelper(vector<method> &methodOfFile,vector<string> &allLinesOfFile,string filename)
{
    string line;
    ifstream ifile(filename.c_str());
    stack<char>brackets;

    int lineNumber=0;
    while(getline(ifile,line))
    {
        allLinesOfFile.push_back(line);
    }
    ifile.close();

    for(int i=0;i<allLinesOfFile.size();i++)
    {
        lineNumber++;
        line=allLinesOfFile[i];
        MethodRecognising(line,methodOfFile,brackets,allLinesOfFile,lineNumber);
    }
    /*for(int i=0;i<methodOfFile.size();i++)
    {
        cout<<methodOfFile[i].methodName<<" "<<methodOfFile[i].methodDatatype<<" "<<methodOfFile[i].FinishLineNumber<<" "<<methodOfFile[i].startLineNumber<<endl;
    }*/
}
/****************************Method Recognising Method END *******************************************/
void makingStringOfMethod(vector<method> &methodOfFile , vector<string>allLineOfFile)
{
    bool flagForFirstLine;
    for(int i=0;i<methodOfFile.size();i++)
    {
        string mthd="";
        flagForFirstLine=false;
        for(int j=methodOfFile[i].startLineNumber-1;j<methodOfFile[i].FinishLineNumber ; j++)
        {
            vector<string>one=tokenize(allLineOfFile[j]);
            for(int k=0;k<one.size();k++)
            {
                if(flagForFirstLine==false && k==1)
                {
                    flagForFirstLine=true;
                    continue;
                }
                else mthd+=one[k];
            }
        }
        methodOfFile[i].methodInOneLine_actVar=mthd;
    }
}

void methodMatching(vector<method> &methodOne , vector<method> &methodTwo)
{
    if(fileNumber==1)
    {
        for(int i=0;i<methodOne.size();i++)
        {
            for(int j=i+1;j<methodTwo.size();j++)
            {
                if(methodOne[i].methodInOneLine_actVar==methodTwo[j].methodInOneLine_actVar)
                {
                    cout<<methodOne[i].methodDatatype<<" "<<methodOne[i].methodName<<"\t"<<methodOne[j].methodDatatype<<" "<<methodOne[j].methodName<<endl;
                }
            }
        }
    }
    else
    {
        for(int i=0;i<methodOne.size();i++)
        {
            for(int j=0;j<methodTwo.size();j++)
            {
                if(methodOne[i].methodInOneLine_actVar==methodTwo[j].methodInOneLine_actVar)
                {
                    cout<<methodOne[i].methodDatatype<<" "<<methodOne[i].methodName<<"\t"<<methodOne[j].methodDatatype<<" "<<methodOne[j].methodName<<endl;
                }
            }
        }
    }
}
/*****************************Mapping File************************************************************/
void MapEntireFile(string source,string destination)
{
    /*string line,editedLine;
    vector<string>words;

    ifstream iff(source.c_str());
    ofstream off(destination.c_str());
    while(getline(iff,line))
    {
        editedLine=replaceSymbols(line);
        words=tokenize(editedLine);
        for(int i=0;i<words.size();i++)
        {
            for(int j=0;j<9;j++)
            {
                if(words[i]==dataTypeName[j])
                {
                    switch(dataTypeName[j])
                    {
                        case "int":
                        case "int[]":
                            break;
                        case "double":
                        case "double[]":
                            break;
                        case "float":
                        case "float[]":
                            break;
                        case "char":
                        case "char[]":
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    iff.close();
    off.close();*/
}
/******************Functions called if total file number is ONE***************************************/
void oneFileFunction()
{
    vector<string>statementsOfFileOne;
    vector<int>lineNumberOfStatementsOfFileOne;
    vector<method>methodOfFileOne;
    vector<string>allLinesOfFileOne;

    savingDataFromFile(1);
    puttingStatementsInVector(statementsOfFileOne,1,lineNumberOfStatementsOfFileOne);
    lineMatching(statementsOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileOne);
    thresholdMatching(statementsOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileOne);
    MethodRecogniseHelper(methodOfFileOne,allLinesOfFileOne,"fileOneWithoutComment.txt");
    makingStringOfMethod(methodOfFileOne,allLinesOfFileOne);
    methodMatching(methodOfFileOne,methodOfFileOne);
    MapEntireFile("fileOneWithoutComment.txt","fileOneMapped.txt");
}

/******************Functions called if total file number is Two***************************************/
void twoFileFunction()
{
    vector<string>statementsOfFileOne;
    vector<string>statementsOfFileTwo;
    vector<int>lineNumberOfStatementsOfFileOne;
    vector<int>lineNumberOfStatementsOfFileTwo;
    vector<method>methodOfFileOne;
    vector<method>methodOfFileTwo;
    vector<string>allLinesOfFileOne;
    vector<string>allLinesOfFileTwo;

    savingDataFromFile(1);
    savingDataFromFile(2);
    puttingStatementsInVector(statementsOfFileOne,1,lineNumberOfStatementsOfFileOne);
    puttingStatementsInVector(statementsOfFileTwo,2,lineNumberOfStatementsOfFileTwo);
    lineMatching(statementsOfFileOne,statementsOfFileTwo,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileTwo);
    thresholdMatching(statementsOfFileOne,statementsOfFileTwo,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileTwo);
    MethodRecogniseHelper(methodOfFileOne,allLinesOfFileOne,"fileOneWithoutComment.txt");
    MethodRecogniseHelper(methodOfFileTwo,allLinesOfFileTwo,"fileTwoWithoutComment.txt");
    makingStringOfMethod(methodOfFileOne,allLinesOfFileOne);
    makingStringOfMethod(methodOfFileTwo,allLinesOfFileTwo);
}
/***************************Main Function********************************************************************/
int main()
{
    string languageOfTheCode = userInterection();
    if(fileNumber==1)
    {
        oneFileFunction();
    }
    else
    {
        twoFileFunction();
    }
     //remove("fileTwoWithoutComment.txt");
    //remove("fileOneWithOutComment.txt");
    return 0;
}
