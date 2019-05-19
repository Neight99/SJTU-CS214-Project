#include "DecisionTree.h"

double C45Tree::_calGain(const vector<vector<int>> &space, int attr) {
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
    // 分裂信息度量
    double splitInformation = 0;

    for (auto i : space) {
        if (i[attr] <= 0) {
            return -2e9;
        } else {
            envVec[i[attr]][i.back()]++;
        }
    }

    // 计算信息增益与分裂信息度量
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
        splitInformation -= (d / sampleNum) * log2(d / sampleNum);
    }

    // 计算信息增益率
    ans = ent + env;
    ans /= splitInformation;

    envVec.clear();
    entropy.clear();
    return ans;
}