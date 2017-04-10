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
//Overall data
extern int logicalStatements;
extern float averageComplexitty;
extern float getMainVolume (void);


float OverallIndex = 0.0;
/**  *****************************************************************************************************/

double getMIResult(int loc, double cyclomatic, double volume) {

    double result = 171 - ( 5.2*log(volume) ) - (0.23*cyclomatic) - (16.2*log(loc));

    return result;
}

void calculateOverallIndex(void) {

    OverallIndex = getMIResult(logicalStatements, averageComplexitty, getMainVolume());
}

void maintainabiltyMain(void) {

    calculateOverallIndex();

}
