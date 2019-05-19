#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

class DecisionTree {
    friend istream &operator>>(istream &os, DecisionTree &obj);
    friend ostream &operator<<(ostream &is, DecisionTree &obj);

   protected:
    struct Node {
        // 子节点个数
        int childNum;
        // 特征ID
        int featID;
        // 属性ID
        int attrID;
        // 子节点的指针
        vector<Node *> child;

        Node() : childNum(0), featID(-1), attrID(-1) {}
    };

    // 根节点
    Node *root;
    // 决策树维度
    int dimNum;
    // 样本个数
    int sampleNum;
    // 样本空间
    vector<vector<int>> sampleSpace;
    // 属性的名字
    vector<string> attribute;
    // 标签对ID的map
    vector<map<string, int>> dis;
    // ID对标签的map
    vector<map<int, string>> inv;

    // 输入的原始数据，用于检验决策树
    vector<vector<string>> dataInput;

    // 输入的测试数据
    vector<vector<string>> testInput;

    // 将输入的值序列化
    void _format(vector<vector<string>> &);
    // 删除无用样本（用于递归创建节点）
    vector<vector<int>> _removeSample(vector<vector<int>>, int);
    // 递归建决策树
    void _build(Node *&, vector<vector<int>>);
    // 判断样本空间内所有元素的结果是否一致
    bool _judgeSameResult(const vector<vector<int>> &);
    // 找到增益最大的属性
    int _findMaxGainAttr(const vector<vector<int>> &);
    // 寻找占数最多的label
    int _findMostLabel(vector<vector<int>> &);
    // 计算信息增益
    virtual double _calGain(const vector<vector<int>> &, int) = 0;
    // 清空决策树
    void _clear(Node *&);
    // 递归输出树
    void _printNode(ostream &, Node *, int);

   public:
    ~DecisionTree();
    void build();
    void checkInputData(bool showAns = 0, ostream &os = cout);
    string predict(vector<string> &);
    // 对测试数据进行测试
    void testData(bool showAns = 0, ostream &os = cout);
};

class ID3Tree : public DecisionTree {
    // friend istream &operator>>(istream &, ID3Tree &);
    // friend ostream &operator<<(ostream &, ID3Tree &);

   private:
    // 计算信息增益
    double _calGain(const vector<vector<int>> &, int);
};

class C45Tree : public DecisionTree {
   private:
    // 计算信息增益率
    double _calGain(const vector<vector<int>> &, int);
};