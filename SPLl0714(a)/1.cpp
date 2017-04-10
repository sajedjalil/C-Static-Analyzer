#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<stack>
#include<queue>
#include<map>
#include<cstdlib>
#include<string>

using namespace std;

string fileOne,fileTwo;
int fileNumber;
bool flagForComment;
map<string,string>methodVariableOfOneFile[100];
map<string,string>methodVariableOfTwoFile[100];
/***                                 Language Detecting of Given File                        **/

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

/**                                 FINISH OF LANGUAGE DETECTION                                **/

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
        if(languageOfFileOne!="c")
        {
            while(1)
            {
                cout <<"You Entered wrong file! try Again\n";
                cout<<"Enter the file name :";
                getline(cin,fileOne);
                languageOfFileOne=languageDetection(fileOne);
                if(languageOfFileOne=="c")break;
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
            if(languageOfFileOne!="c")
            {
                while(1)
                {
                    cout <<"You Entered wrong file! try Again\n";
                    cout<<"Enter the file name :";
                    getline(cin,fileOne);
                    languageOfFileOne=languageDetection(fileOne);
                    if(languageOfFileOne=="c")break;
                }
            }

            cout<<"Enter Second file name : ";
            getline(cin,fileTwo);

            string languageOfFileTwo=languageDetection(fileTwo);
            if(languageOfFileTwo!="c")
            {
                while(1)
                {
                    cout <<"You Entered wrong file! try Again\n";
                    cout<<"Enter the file name :";
                    getline(cin,fileTwo);
                    languageOfFileTwo=languageDetection(fileTwo);
                    if(languageOfFileTwo=="c" )break;
                }
            }

            if(languageOfFileOne==languageOfFileTwo)return languageOfFileOne;
            cout<<"Files Mismatched!\n";
        }

    }
}

/**                                     REMOVING COMMENT FROM FILE                                 **/

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

/**                             Saving Data From File                                    **/

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

/**                         Exact Statement Matching Part                               **/
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

void puttingStatementsInVector(vector<string> &statements,string fileName,vector<int> &lineNumber)
{
    ifstream ifile(fileName.c_str());
    string codeLine;
    int i=0;
    while(getline(ifile,codeLine))
    {
        i++;
        recognisingStatement(statements ,codeLine,lineNumber,i);
    }
    ifile.close();

}

void lineMatching(vector<string> statements1 , vector<string> statements2,vector<int> lineNumber1,vector<int> lineNumber2,vector<string> &allLine)
{
    vector<int>MatchedLineNumbers;
    bool initialeLine[10000]={false};

    cout<<"\nStatement Matching Part:\n\n";

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
                cout<<"\t"<<allLine[lineNumber1[i]-1]<<endl;
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
                cout<<"\t"<<allLine[lineNumber1[i]-1]<<endl;
            }
            MatchedLineNumbers.clear();
        }
    }
}

/**                         End of Exact statement matching                                     **/

/**                         Start of Exact Threshold Matching                                      **/

void thresholdMatching(vector<string> statements1 , vector<string> statements2,vector<int> lineNumber1,vector<int> lineNumber2,vector<string> &allLine)
{
    int threshold;
    bool flag=false;
    cout<<"\n\nThreshold Matching Part :\n\nPlease enter the threshold for the statements:";
    cin>>threshold;

    int matchNumber=0;
    if(fileNumber==1)
    {
        cout<<"There are total "<<statements1.size()<<" statements in File Number One.\n\n";
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
                        if(flag==false)flag=true;

                        cout<<"Match Found :";
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

                        for(int k=m-threshold;k<m;k++)
                        {
                            cout<<allLine[lineNumber1[k]-1]<<endl;
                        }
                        cout<<"\n";

                        matchNumber=0;
                        m=i;
                    }
                    if(m>=statements1.size())
                    {
                        matchNumber=0;
                        m=i;
                        break;
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
        if(flag==false)
        {
            cout<<"No match found for threshold "<<threshold<<endl;

        }
    }
    else
    {
        int m;
        cout<<"There are total "<<statements1.size()<<" statements in File Number One.\n";
        cout<<"There are total "<<statements2.size()<<" statements in File Number Two.\n\n";

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
                        cout<<"Match Found :";
                        if(flag==false)flag=true;
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
                        for(int k=m-threshold;k<m;k++)
                        {
                            cout<<allLine[lineNumber1[k]-1]<<endl;
                        }
                        cout<<"\n";
                        matchNumber=0;
                        m=i;
                    }
                    if(m>=statements1.size())
                    {
                        matchNumber=0;
                        m=i;
                        break;
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
        if(flag==false)
        {
            cout<<"No match found for threshold "<<threshold<<endl;

        }
    }
}

/**                                 Replace Symbols                           ****/
char operatorsSet1[9] = {'+','-','/','(',')','{','}',';',','};

string replaceSymbolsSet1(string line)
{
	for(int i=0;i<9;i++)
	{
		for(int j=0; j<line.length(); j++)
		{
			if(operatorsSet1[i]==line[j]) line[j] = ' ';
		}
	}
	return line;
}

/**                         Tokenize String                                       **/
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


/**                     Method Recognising Process                              ***/

struct method{
    string methodName;
    int startLineNumber;
    int FinishLineNumber;
    string methodDatatype;
    string methodInOneLine_actVar;
    vector<pair<string,string> >parameters;
};

string dataTypeName[9]={"int","double","float","char","void","int[]","float[]","double[]","char[]"};
bool structFlag=false;

void MethodRecognising(string line,vector<method> &methodOfFile,stack<char> &brackets,vector<string>&allLinesOfFile,int lineNumber)
{
    for(int i=0;i<line.length();i++)
    {
        if(line[i]=='{' && brackets.empty()==true)
        {
            string words=replaceSymbolsSet1(line);
            vector<string>word=tokenize(words);
            method one;
            /**********A NEW Method*****************/

            if(i==0 || word.size()==0)
            {
                string pastline=allLinesOfFile[lineNumber-2];
                words=replaceSymbolsSet1(pastline);
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

            for(int mi=2;mi<word.size();mi+=2)
            {
                one.parameters.push_back(make_pair(word[mi],word[mi+1]));
            }

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
}

/**                         Method Recognising Method END                                       **/

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

void methodMatching(vector<method> &methodOne , vector<method> &methodTwo,vector<string> &allLine)
{
    cout<<"\n\nMethod Matching Part :\n\n";
    bool flag=false;
    if(fileNumber==1)
    {
        for(int i=0;i<methodOne.size();i++)
        {
            for(int j=i+1;j<methodTwo.size();j++)
            {
                if(methodOne[i].methodInOneLine_actVar==methodTwo[j].methodInOneLine_actVar)
                {
                    if(flag==false)flag=true;

                    cout<<"Method Matched :";
                    cout<<methodOne[i].methodDatatype<<" "<<methodOne[i].methodName<<"\t"<<methodOne[j].methodDatatype<<" "<<methodOne[j].methodName<<endl;
                    for(int k=methodOne[i].startLineNumber;k<=methodOne[i].FinishLineNumber;k++)
                    {
                        cout<<allLine[k-1]<<endl;
                    }
                    cout<<endl;
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
                    if(flag==false)flag=true;

                    cout<<"Method Matched :";
                    cout<<methodOne[i].methodDatatype<<" "<<methodOne[i].methodName<<"\t"<<methodOne[j].methodDatatype<<" "<<methodOne[j].methodName<<endl;
                    for(int k=methodOne[i].startLineNumber;k<=methodOne[i].FinishLineNumber;k++)
                    {
                        cout<<allLine[k-1]<<endl;
                    }
                cout<<endl;
                }
            }
        }
    }
    if(flag==false)
    {
        cout<<"No method matched\n";
    }
}

/**                                 Mapping File                                              **/

char operatorsSet2[11] = {'+','-','*','/','%','[',']',',',';','=','&'};

string convertingIntToString(int a)     /**convert int to string**/
{
    stringstream ss;
    ss << a;
    string str = ss.str();
    return str;
}

/**                             Mapping Global Variable                                      ***/

void MapGlobalVariables(vector<method> &methods,vector<string> &allstatements,vector<int> &lineNumber,int currentFileNumber,vector<string> &allLineOfFile)
{
    int start=0,variables=0,endPoint;
    string typeOfData,line,editedLine,value;
    bool flag=false;

    for(int i=0;i<methods.size();i++)
    {
        endPoint=methods[i].startLineNumber;
        for(int j=0;j<allstatements.size();j++)
        {
            if(lineNumber[j]>=start && lineNumber[j]<=endPoint)
            {
                line=allLineOfFile[lineNumber[j]-1];
                for(int m=0;m<line.length();m++)
                {
                    if(line[m]=='(')
                    {
                        flag=true;
                        break;
                    }
                }

                if(flag==true)
                {
                    flag=false;
                    continue;
                }
                else
                {
                    editedLine=line;
                    for(int il=0;il<11;il++)
                    {
                        for(int jl=0; jl<editedLine.length(); jl++)
                        {
                            if(operatorsSet2[il]==editedLine[jl]) editedLine[jl] = ' ';

                        }
                    }
                    vector<string>one=tokenize(editedLine);
                    for(int k=0;k<5;k++)
                    {
                        if(one[0]==dataTypeName[k])
                        {
                            typeOfData=dataTypeName[k];
                            break;
                        }
                    }
                    /** got data Type**/

                   for(int k=1;k<one.size();k++)
                    {
                        if(atof(one[k].c_str())==0 && one[k]!="0.0" && one[k]!="0" )
                        {
                            if(currentFileNumber==1)    //if file number is One then for file One
                            {
                                for(int l=0;l<methods.size();l++)
                                {
                                    if(methodVariableOfOneFile[l].count(one[k])!=0) //if the variable is already in map
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        value="";
                                        value+=typeOfData[0];
                                        value+='a';
                                        methodVariableOfOneFile[l].insert(make_pair(one[k],value));
                                        //cout<<one[k]<<" "<<value<<endl;
                                    }
                                }
                            }
                            else
                            {
                                for(int l=0;l<methods.size();l++)
                                {
                                    if(methodVariableOfTwoFile[l].count(one[k])!=0) //if the variable is already in map
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        value="";
                                        value+=typeOfData[0];
                                        value+='a';
                                        methodVariableOfTwoFile[l].insert(make_pair(one[k],value));
                                        //cout<<one[k]<<" "<<value<<endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        start=methods[i].FinishLineNumber;
    }
}

/**                    Map Method Inner Variable                **/

void MapMethodVariable(vector<method> &methods,vector<string> &allstatements,vector<int> &lineNumber,int currentFileNumber,vector<string> &allLineOfFile)
{
    string typeOfData,line,editedLine,value,var;

    for(int i=0;i<methods.size();i++)                       /**  Method parameter Variable **/
    {
        /** Taking into map the parameters      **/
        for(int m=0;m<methods[i].parameters.size();m++)
        {
            for(int il=0;il<11;il++)
            {
                for(int jl=0; jl<methods[i].parameters[m].second.length(); jl++)
                {
                    if(operatorsSet2[il]==methods[i].parameters[m].second[jl]) methods[i].parameters[m].second[jl] = ' ';
                }
            }
            vector<string>one=tokenize(methods[i].parameters[m].second);
            var=one[0];
            /** Taking out variable name from parameter     **/
            if(currentFileNumber==1)
            {
                if(methodVariableOfOneFile[i].count(var)!=0) //if the variable is already in map
                {
                    continue;
                }
                else
                {
                    value="";
                    value+=methods[i].parameters[m].first[0];
                    value+='a';
                    methodVariableOfOneFile[i].insert(make_pair(var,value));
                    //cout<<var<<" "<<value<<endl;
                }
            }
            else
            {
                if(methodVariableOfTwoFile[i].count(var)!=0) //if the variable is already in map
                {
                    continue;
                }
                else
                {
                    value="";
                    value+=methods[i].parameters[m].first[0];
                    value+='a';
                    methodVariableOfTwoFile[i].insert(make_pair(var,value));
                   // cout<<var<<" "<<value<<endl;
                }
            }

        }

        for(int j=0;j<allstatements.size();j++)     /** Method inner Variable **/
        {
            if(lineNumber[j]<methods[i].startLineNumber)
            {
                continue;
            }
            else if(lineNumber[j]>methods[i].FinishLineNumber)
            {
                break;
            }
            else
            {
                line=allLineOfFile[lineNumber[j]-1];
                editedLine=line;
                typeOfData="";

                for(int il=0;il<10;il++)
                {
                    for(int jl=0; jl<editedLine.length(); jl++)
                    {
                        if(operatorsSet2[il]==editedLine[jl]) editedLine[jl] = ' ';

                    }
                }
                vector<string>one=tokenize(editedLine);
                for(int k=0;k<5;k++)
                {
                    if(one[0]==dataTypeName[k])
                    {
                        typeOfData=dataTypeName[k];
                        break;
                    }
                }
                    /** got data Type**/

                if(typeOfData=="")
                {
                    continue;
                }
                else
                {
                    for(int k=1;k<one.size();k++)
                    {
                        if(atof(one[k].c_str())==0 && one[k]!="0.0" && one[k]!="0" )
                        {
                            if(currentFileNumber==1)    //if file number is One then for file One
                            {
                                if(methodVariableOfOneFile[i].count(one[k])!=0) //if the variable is already in map
                                {
                                    continue;
                                }
                                else
                                {
                                    value="";
                                    value+=typeOfData[0];
                                    value+='a';
                                    methodVariableOfOneFile[i].insert(make_pair(one[k],value));
                                    //cout<<one[k]<<" "<<value<<endl;
                                }
                            }
                            else
                            {

                                if(methodVariableOfTwoFile[i].count(one[k])!=0) //if the variable is already in map
                                {
                                    continue;
                                }
                                else
                                {
                                    value="";
                                    value+=typeOfData[0];
                                    value+='a';
                                    methodVariableOfTwoFile[i].insert(make_pair(one[k],value));
                                    //cout<<one[k]<<" "<<value<<endl;
                                }
                            }
                        }
                    }
                }

            }
        }
    }
}

char operatorsSet3[19] = {'+','-','*','/','%','[',']',',',';','=','(',')','{','}','<','>','&','|','!'};

void makingFileWithReplacedVariables(vector<string> &allLinesOfSourceFile,vector<string> &allLinesOfDestinationFile,vector<method> &allMethods,string destination,int currentFileNumber)
{
    ofstream off(destination.c_str());
    allLinesOfDestinationFile=allLinesOfSourceFile;
    string line,editedLine,word,replacedLine;
    int startPoint=0,endPoint;

    for(int i=0;i<allMethods.size();i++)        //for global variable
    {
        endPoint=allMethods[i].startLineNumber-1;
        for(int j=0;j<allLinesOfDestinationFile.size();j++)
        {
            if(j >=startPoint && j <endPoint)
            {
                line=allLinesOfDestinationFile[j];
                editedLine=line;                    //Replace Oparators
                for(int il=0;il<19;il++)
                {
                    for(int jl=0;jl<editedLine.length();jl++)
                    {
                        if(editedLine[jl] ==operatorsSet3[il])
                        {
                            editedLine[jl]=' ';
                        }
                    }
                }
                word="";
                replacedLine="";
                int doubleQuote=0;

                for(int k=0;k<editedLine.length();k++)
                {
                    if(editedLine[k]==' ')
                    {
                        if(word!="")                    //count double quote if the word is in printf
                        {
                            if(methodVariableOfOneFile[0].count(word)!=0 && currentFileNumber==1)       //If it is in the map
                            {
                                replacedLine+=methodVariableOfOneFile[0][word];
                            }
                            else if(methodVariableOfTwoFile[0].count(word)!=0 && currentFileNumber==2)
                            {
                                replacedLine+=methodVariableOfTwoFile[0][word];
                            }
                            else                                        //else take the word
                            {
                                replacedLine+=word;
                            }
                        }
                    replacedLine+=line[k];
                    word="";
                    }
                    else                                //if it is not space
                    {
                        word+=editedLine[k];
                    }
                }
            allLinesOfDestinationFile[j]=replacedLine;
            }
            if(j>endPoint)break;
        }
        startPoint=allMethods[i].FinishLineNumber+1;
    }

    for(int i=0;i<allMethods.size();i++)            //for methods and method body
    {
        startPoint=allMethods[i].startLineNumber-1;
        endPoint=allMethods[i].FinishLineNumber;
        for(int j=0;j<allLinesOfDestinationFile.size();j++)
        {
            if(j >=startPoint && j <=endPoint)
            {
                line=allLinesOfDestinationFile[j];
                editedLine=line;                    //Replace Oparators
                for(int il=0;il<19;il++)
                {
                    for(int jl=0;jl<editedLine.length();jl++)
                    {
                        if(editedLine[jl] ==operatorsSet3[il])
                        {
                            editedLine[jl]=' ';
                        }
                    }
                }

                word="";
                replacedLine="";
                int doubleQuote=0;

                for(int k=0;k<editedLine.length();k++)
                {
                    if(editedLine[k]==' ')
                    {
                        if(word!="")                    //count double quote if the word is in printf
                        {
                            doubleQuote=0;
                            for(int l=0;l<k;l++)
                            {
                                if(editedLine[l]=='"')doubleQuote++;
                            }
                            if(doubleQuote%2==0)            //if it is not in printf
                            {
                                if(methodVariableOfOneFile[i].count(word)!=0 && currentFileNumber==1)       //If it is in the map
                                {
                                    replacedLine+=methodVariableOfOneFile[i][word];
                                }
                                else if(methodVariableOfTwoFile[i].count(word)!=0 && currentFileNumber==2)       //If it is in the map
                                {
                                    replacedLine+=methodVariableOfTwoFile[i][word];
                                }
                                else                                        //else take the word
                                {
                                    replacedLine+=word;
                                }
                            }
                            else                            //if it is in printf just take it
                            {
                                replacedLine+=word;
                            }
                        }
                    replacedLine+=line[k];
                    word="";
                    }
                    else                                //if it is not space
                    {
                        word+=editedLine[k];
                    }
                }
            allLinesOfDestinationFile[j]=replacedLine;
            }
            if(j>endPoint)break;
        }
    }
    for(int i=0;i<allLinesOfDestinationFile.size();i++)
    {
        off<<allLinesOfDestinationFile[i]<<endl;
    }

    off.close();
}

struct forLoop{
    int startNumber;
    int finishNumber;
};
/*void forBlockDetection(vector<string> &allLines,vector<forLoop> &forBlockCollection)
{
    string line,editedLine;
    int update,semi;
    stack<char>brackets;
    bool done;

    for(int i=0;i<allLines.size();i++)
    {
        done=false;
        line=allLines[i];
        editedLine=replaceSymbolsSet1(line);
        if(editedLine.length()==0)continue;
        vector<string>word=tokenize(editedLine);
        for(int k=0;k<
        )
        if(word[0]=="for" && brackets.empty()==true)
        {
            forLoop one;
            one.startNumber=i;
            update=i;
            semi=0;

            if(line[line.length()-1]==';')
            {
                one.finishNumber=i;
                forBlockCollection.push_back(one);
                continue;
            }
/*label1:     for(int j=0;j<line.length();j++)
            {
                if(line[j]==';')semi++;
                if(semi==3)
                {
                    one.finishNumber=i;
                    break;
                }
                if(line[j]=='{')brackets.push('{');
                else if(line[j]=='}' && brackets.empty()!=true)
                {
                    brackets.pop();
                    one.finishNumber=i;
                    done=true;
                    break;
                }
            }
            if(done!=true && brackets.size()==0 && semi!=3)
            {
                line=allLines[i++];
                goto label1;
            }
            forBlockCollection.push_back(one);
        }
        else if(brackets.size()!=0)
        {
            for(int j=0;j<line.length();j++)
            {
                if(line[j]=='{')brackets.push('{');
                else if(line[j]=='}' && brackets.empty()!=true)
                {
                    brackets.pop();
                }
                if(brackets.empty()==true)
                {
                    forBlockCollection[forBlockCollection.size()-1].finishNumber=i;
                    i=update;
                    break;
                }
            }
        }
    }
    //for(int i=0;i<forBlockCollection.size();i++)
    //{
    //    cout<<forBlockCollection[i].startNumber<<" "<<forBlockCollection[i].finishNumber<<endl;
    //}
}*/
/******************Functions called if total file number is ONE***************************************/

void oneFileFunction()
{
    vector<string>statementsOfFileOne;
    vector<int>lineNumberOfStatementsOfFileOne;
    vector<method>methodOfFileOne;
    vector<string>allLinesOfFileOne;
    vector<forLoop>forLoopCollection;

    vector<string>allLinesOfFileOne_Rep_Var;
    vector<method>methodOfFileOne_Rep_Var;
    vector<string>statementsOfFileOne_Rep_Var;
    vector<int>lineNumberOfStatementsOfFileOne_Rep_Var;

    savingDataFromFile(1);
    MethodRecogniseHelper(methodOfFileOne,allLinesOfFileOne,"fileOneWithoutComment.txt");
    puttingStatementsInVector(statementsOfFileOne,"fileOneWithoutComment.txt",lineNumberOfStatementsOfFileOne);
    lineMatching(statementsOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileOne,allLinesOfFileOne);
    thresholdMatching(statementsOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileOne,allLinesOfFileOne);

    //forBlockDetection(allLinesOfFileOne,forLoopCollection);
    makingStringOfMethod(methodOfFileOne,allLinesOfFileOne);
    methodMatching(methodOfFileOne,methodOfFileOne,allLinesOfFileOne);

    cout<<"Semantic Matching Part :"<<endl;
    MapGlobalVariables(methodOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,1,allLinesOfFileOne);
    MapMethodVariable(methodOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,1,allLinesOfFileOne);
    makingFileWithReplacedVariables(allLinesOfFileOne,allLinesOfFileOne_Rep_Var,methodOfFileOne,"fileOneReplacedVariables.txt",1);

    allLinesOfFileOne_Rep_Var.clear();

    MethodRecogniseHelper(methodOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var,"fileOneReplacedVariables.txt");
    puttingStatementsInVector(statementsOfFileOne_Rep_Var,"fileOneReplacedVariables.txt",lineNumberOfStatementsOfFileOne_Rep_Var);
    lineMatching(statementsOfFileOne_Rep_Var,statementsOfFileOne_Rep_Var,lineNumberOfStatementsOfFileOne_Rep_Var,lineNumberOfStatementsOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var);
    thresholdMatching(statementsOfFileOne_Rep_Var,statementsOfFileOne_Rep_Var,lineNumberOfStatementsOfFileOne_Rep_Var,lineNumberOfStatementsOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var);

    makingStringOfMethod(methodOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var);
    methodMatching(methodOfFileOne_Rep_Var,methodOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var);
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

    vector<string>allLinesOfFileOne_Rep_Var;
    vector<string>allLinesOfFileTwo_Rep_Var;
    vector<string>statementsOfFileOne_Rep_Var;
    vector<string>statementsOfFileTwo_Rep_Var;
    vector<int>lineNumberOfStatementsOfFileOne_Rep_Var;
    vector<int>lineNumberOfStatementsOfFileTwo_Rep_Var;
    vector<method>methodOfFileOne_Rep_Var;
    vector<method>methodOfFileTwo_Rep_Var;

    savingDataFromFile(1);
    savingDataFromFile(2);
    MethodRecogniseHelper(methodOfFileOne,allLinesOfFileOne,"fileOneWithoutComment.txt");
    MethodRecogniseHelper(methodOfFileTwo,allLinesOfFileTwo,"fileTwoWithoutComment.txt");

    puttingStatementsInVector(statementsOfFileOne,"fileOneWithoutComment.txt",lineNumberOfStatementsOfFileOne);
    puttingStatementsInVector(statementsOfFileTwo,"fileTwoWithoutComment.txt",lineNumberOfStatementsOfFileTwo);
    lineMatching(statementsOfFileOne,statementsOfFileTwo,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileTwo,allLinesOfFileOne);
    thresholdMatching(statementsOfFileOne,statementsOfFileTwo,lineNumberOfStatementsOfFileOne,lineNumberOfStatementsOfFileTwo,allLinesOfFileOne);

    makingStringOfMethod(methodOfFileOne,allLinesOfFileOne);
    makingStringOfMethod(methodOfFileTwo,allLinesOfFileTwo);
    methodMatching(methodOfFileOne,methodOfFileTwo,allLinesOfFileOne);

    MapGlobalVariables(methodOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,1,allLinesOfFileOne);
    MapGlobalVariables(methodOfFileTwo,statementsOfFileTwo,lineNumberOfStatementsOfFileTwo,2,allLinesOfFileTwo);
    MapMethodVariable(methodOfFileOne,statementsOfFileOne,lineNumberOfStatementsOfFileOne,1,allLinesOfFileOne);
    MapMethodVariable(methodOfFileTwo,statementsOfFileTwo,lineNumberOfStatementsOfFileTwo,2,allLinesOfFileTwo);
    makingFileWithReplacedVariables(allLinesOfFileOne,allLinesOfFileOne_Rep_Var,methodOfFileOne,"fileOneReplacedVariables.txt",1);
    makingFileWithReplacedVariables(allLinesOfFileTwo,allLinesOfFileTwo_Rep_Var,methodOfFileTwo,"fileTwoReplacedVariables.txt",2);

    allLinesOfFileOne_Rep_Var.clear();
    allLinesOfFileTwo_Rep_Var.clear();

    MethodRecogniseHelper(methodOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var,"fileOneReplacedVariables.txt");
    MethodRecogniseHelper(methodOfFileTwo_Rep_Var,allLinesOfFileTwo_Rep_Var,"fileTwoReplacedVariables.txt");

    puttingStatementsInVector(statementsOfFileOne_Rep_Var,"fileOneReplacedVariables.txt",lineNumberOfStatementsOfFileOne_Rep_Var);
    puttingStatementsInVector(statementsOfFileTwo_Rep_Var,"fileTwoReplacedVariables.txt",lineNumberOfStatementsOfFileTwo_Rep_Var);

    lineMatching(statementsOfFileOne_Rep_Var,statementsOfFileTwo_Rep_Var,lineNumberOfStatementsOfFileOne_Rep_Var,lineNumberOfStatementsOfFileTwo_Rep_Var,allLinesOfFileOne_Rep_Var);
    thresholdMatching(statementsOfFileOne_Rep_Var,statementsOfFileTwo_Rep_Var,lineNumberOfStatementsOfFileOne_Rep_Var,lineNumberOfStatementsOfFileTwo_Rep_Var,allLinesOfFileOne_Rep_Var);

    makingStringOfMethod(methodOfFileOne_Rep_Var,allLinesOfFileOne_Rep_Var);
    makingStringOfMethod(methodOfFileTwo_Rep_Var,allLinesOfFileTwo_Rep_Var);
    methodMatching(methodOfFileOne_Rep_Var,methodOfFileTwo_Rep_Var,allLinesOfFileOne_Rep_Var);

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
