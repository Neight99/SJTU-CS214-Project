#include "DecisionTree.h"

istream &operator>>(istream &is, DecisionTree &obj) {
    int k;

    // 输入决策树维度
    is >> obj.dimNum;
    // 输入决策树样本数
    is >> obj.sampleNum;
    // 输入用于检测的样例个数
    is >> k;

    obj.sampleNum -= k;

    obj.attribute = vector<string>(obj.dimNum);
    obj.sampleSpace =
        vector<vector<int>>(obj.sampleNum, vector<int>(obj.dimNum + 1, 0));
    obj.dis = vector<map<string, int>>(obj.dimNum + 1, map<string, int>());
    obj.inv = vector<map<int, string>>(obj.dimNum + 1, map<int, string>());

    // 输入属性名称
    for (int i = 0; i < obj.dimNum; i++) {
        is >> obj.attribute[i];
    }

    vector<vector<string>> temp =
        vector<vector<string>>(obj.sampleNum, vector<string>(obj.dimNum + 1));

    // 输入数据
    for (int i = 0; i < obj.sampleNum; i++) {
        for (int j = 0; j < obj.dimNum + 1; j++) {
            is >> temp[i][j];
        }
    }

    obj.dataInput = temp;

    obj._format(temp);
    temp.clear();

    obj.build();

    vector<vector<string>> tempTest =
        vector<vector<string>>(k, vector<string>(obj.dimNum + 1));

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < obj.dimNum + 1; j++) {
            is >> tempTest[i][j];
        }
    }
    obj.testInput = tempTest;

    return is;
}

ostream &operator<<(ostream &os, DecisionTree &obj) {
    os << '\n';
    obj._printNode(os, obj.root, 0);

    return os;
}

DecisionTree::~DecisionTree() {
    if (root) {
        _clear(root);
    }
    inv.clear();
    attribute.clear();
    sampleSpace.clear();
    dis.clear();
    dataInput.clear();
}

void DecisionTree::build() { _build(root, sampleSpace); }

void DecisionTree::_format(vector<vector<string>> &vec) {
    for (int i = 0; i <= dimNum; i++) {
        int count = 1;
        for (int j = 0; j < sampleNum; j++) {
            if (!dis[i][vec[j][i]]) {
                dis[i][vec[j][i]] = count;
                inv[i][count++] = vec[j][i];
            }
            sampleSpace[j][i] = dis[i][vec[j][i]];
        }
    }
}

void DecisionTree::_build(Node *&node, vector<vector<int>> space) {
    node = new Node;

    // 检查剩下的Label是否一致
    if (_judgeSameResult(space)) {
        node->childNum = 0;
        node->attrID = space[0].back();
        return;
    }

    // 检查所有属性是否都被删除
    int countMinus = 0;
    for (auto i : space[0]) {
        countMinus += (i < 0) ? 1 : 0;
    }
    if (countMinus == dimNum) {
        node->childNum = 0;
        node->attrID = _findMostLabel(space);
        return;
    }

    node->attrID = _findMaxGainAttr(space);
    node->childNum = inv[node->attrID].size();
    vector<vector<int>> newSpace = _removeSample(space, node->attrID);

    for (int i = 0; i < node->childNum; i++) {
        // 子样本空间
        vector<vector<int>> sub;
        for (int j = 0; j < space.size(); j++) {
            if (space[j][node->attrID] == i + 1) {
                sub.push_back(newSpace[j]);
            }
        }

        if (!sub.empty()) {
            node->child.push_back(0);
            _build((node->child).back(), sub);
        }
        sub.clear();
    }

    node->childNum = node->child.size();

    space.clear();
    newSpace.clear();
}

vector<vector<int>> DecisionTree::_removeSample(vector<vector<int>> vec,
                                                int del) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i][del] = -1;
    }
    return vec;
}

bool DecisionTree::_judgeSameResult(const vector<vector<int>> &space) {
    int last = -1;
    for (auto i : space) {
        if (last == -1) {
            last = i.back();
        } else if (last != i.back()) {
            return 0;
        }
    }
    return 1;
}

int DecisionTree::_findMaxGainAttr(const vector<vector<int>> &space) {
    int ans = -1;
    double maxGain = -2e9;
    for (int i = 0; i < dimNum; i++) {
        if (space[0][i] >= 0) {
            double gain = _calGain(space, i);
            if (isnan(gain)) {
                gain = 0;
            }
            if (gain > maxGain) {
                maxGain = gain;
                ans = i;
            }
        }
    }

    return ans;
}

int DecisionTree::_findMostLabel(vector<vector<int>> &space) {
    int labelNum = inv[dimNum].size();
    vector<int> count(labelNum, 0);

    for (auto i : space) {
        count[i.back()]++;
    }

    int max = 0;
    for (int i = 1; i < count.size(); i++) {
        if (count[max] < count[i]) {
            max = i;
        }
    }

    count.clear();
    return (max == 0) ? 1 : max;
}

double DecisionTree::_calGain(const vector<vector<int>> &space, int attr) {
    double ans = 0;

    // 计算信息熵
    double ent = 0;
    int sampleNum = 0;
    vector<double> entropy(dis.back().size() + 1, 0);
    for (auto i : space) {
        entropy[i.back()]++;
        sampleNum++;
    }
    for (auto i : entropy) {
        i /= sampleNum;
        if (i) {
            ent -= (i * log2(i));
        }
    }

    // 计算各个分支节点的条件熵
    double env = 0;
    // 分支的概率 [分支个数][结果个数]
    vector<vector<double>> envVec(dis[attr].size() + 1,
                                  vector<double>(dis.back().size() + 1, 0));
    for (auto i : space) {
        if (i[attr] <= 0) {
            return -2e9;
        } else {
            envVec[i[attr]][i.back()]++;
        }
    }

    // 计算信息增益
    for (int i = 1; i < envVec.size(); i++) {
        double d = 0, envTemp = 0;
        for (int j = 1; j < envVec[i].size(); j++) {
            d += envVec[i][j];
        }
        for (int j = 1; j < envVec[i].size(); j++) {
            envVec[i][j] /= d;
            if (envVec[i][j]) {
                envTemp -= (envVec[i][j] * log2(envVec[i][j]));
            }
        }
        env -= ((d / sampleNum) * envTemp);
    }

    ans = ent + env;

    envVec.clear();
    entropy.clear();
    return ans;
}

void DecisionTree::_clear(Node *&node) {
    for (DecisionTree::Node *i : node->child) {
        _clear(i);
    }
    delete node;
    node = 0;
}

void DecisionTree::_printNode(ostream &os, Node *node, int level) {
    for (int i = 0; i < level; i++) {
        os << ' ';
    }

    if (!node->childNum) {
        os << inv[dimNum][node->attrID] << '\n';
    } else {
        int len = 0;

        os << attribute[node->attrID] << endl;
        level += attribute[node->attrID].length();

        for (auto i : inv[node->attrID]) {
            len = (len < i.second.length()) ? i.second.length() : len;
        }

        for (int i = 0; i < node->childNum; i++) {
            for (int j = 0; j < level; j++) {
                os << ' ';
            }
            os << setw(len) << inv[node->attrID][i + 1] << endl;
            _printNode(os, (node)->child[i], level + len + 1);
        }
    }
}

void DecisionTree::checkInputData(bool showAns, ostream &os) {
    double trueAns = 0;
    for (auto i : dataInput) {
        string ans = predict(i);
        if (showAns) {
            os << i.back() << '\t' << ans << '\t' << (i.back() == ans) << '\n';
        }
        if (ans == i.back()) {
            trueAns++;
        }
    }
    os << "正确预测: " << trueAns << "组" << endl;
    os << "正确率为: " << (trueAns / sampleNum) * 100 << '%' << endl;
}

string DecisionTree::predict(vector<string> &input) {
    Node *temp = root;
    while (temp->childNum) {
        int route = dis[temp->attrID][input[temp->attrID]];
        if (route <= temp->child.size()) {
            temp = temp->child.at(route - 1);
        } else {
            temp = temp->child.at(0);
        }
    }
    return inv.back()[temp->attrID];
}

void DecisionTree::testData(bool showAns, ostream &os) {
    double trueAns = 0;
    int testSize = testInput.size();
    for (auto i : testInput) {
        string ans = predict(i);
        if (showAns) {
            os << i.back() << '\t' << ans << '\t' << (i.back() == ans) << '\n';
        }
        if (i.back() == ans) {
            trueAns++;
        }
    }
    os << "正确预测: " << trueAns << "组" << endl;
    os << "正确率为: " << (trueAns / testSize) * 100 << '%' << endl;
}