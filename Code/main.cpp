#include <fstream>
#include "DecisionTree.h"

using namespace std;

int main() {
    string _path = "..";
    const string _inputPath = "/Data/dataInput.txt",
                 _ID3TreeOutPath = "/Result/ID3.txt",
                 _C45TreeOutPath = "/Result/C4_5.txt",
                 _ID3CheckPath = "/Result/Check_ID3.txt",
                 _C45CheckPath = "/Result/Check_C4_5.txt",
                 _ID3AnalyPath = "/Result/Analysis_ID3.txt",
                 _C45AnalyPath = "/Result/Analysis_C4_5.txt";
    cout << "请输入根目录路径：";
    cin >> _path;

    fstream dataInput(_path + _inputPath, fstream::in),
        ID3TreeOutput(_path + _ID3TreeOutPath, fstream::out),
        C45TreeOutput(_path + _C45TreeOutPath, fstream::out),
        ID3Check(_path + _ID3CheckPath, fstream::out),
        C45Check(_path + _C45CheckPath, fstream::out),
        ID3Analy(_path + _ID3AnalyPath, fstream::out),
        C45Analy(_path + _C45AnalyPath, fstream::out);

    ID3Tree dt;
    dataInput >> dt;
    ID3TreeOutput << dt;
    dt.testData(1, ID3Analy);
    dt.checkInputData(1, ID3Check);

    dataInput.seekg(0, ios::beg);

    C45Tree dt2;
    dataInput >> dt2;
    C45TreeOutput << dt2;
    dt2.testData(1, C45Analy);
    dt2.checkInputData(1, C45Check);

    return 0;
}