# CS214 算法与复杂性 2019年春 大作业

## 基本要求
利用课上介绍的机器学习方法对[UCI Machine Learning Repository: Data Sets](http://archive.ics.uci.edu/ml/datasets/)中某个数据集进行学习，要求至少实现两种方法，不得使用已有的机器学习函数库，需要自己编程完成。

## 文件列表
./Code  
C4_5.cpp C4.5算法的信息增益率计算函数实现  
DecisionTree.cpp 两种决策树算法共通函数的代码实现  
DecisionTree.h 两种决策树算法共通函数的声明  
ID3.cpp ID3算法的信息增益计算函数实现  
main macOS下编译后的可执行文件  
main.cpp 主程序  

./Data  
dataInput.txt 输入文件

./Result  
Analysis_C4_5.txt C4.5算法训练后决策新数据的结果及正确率  
Analysis_ID3.txt ID3算法训练后决策新数据的结果及正确率  
C4_5.txt C4.5算法训练后得到的决策树  
Check_C4_5.txt C4.5算法训练后检验原数据的结果及正确率  
Check_ID3.txt ID3算法训练后检验原数据的结果及正确率  
ID3.txt ID3算法训练后得到的决策树  

## 输入文件格式
在dataInput.txt中  
第一行为三个数字，依次是数据维度，数据组数以及用于决策的新数据组数，以制表符分隔；  
第二行为各个维度的标签，以制表符分隔；  
第三行开始每行为一组数据，各个标签及结果间均以制表符分隔。

## 使用说明
1.由于编写环境为macOS，因而未生成exe可执行文件。所以在windows系统下使用时需要在main.cpp中加入 system("PAUSE"); 语句，同时因为使用了部分C++11的特性以及多文件，因此编译时使用g++ *.cpp -o main.exe -std=c++11 命令来进行编译。  
2.使用前请保证dataInput.txt文件已存在且内容符合文件格式中的要求。  
3.运行程序后将所有输出数据输出到./Result文件夹下。  