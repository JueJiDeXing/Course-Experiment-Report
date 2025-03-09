#ifndef ANALYSIS_CALATTRIBUTE_H
#define ANALYSIS_CALATTRIBUTE_H

#include "TreeNode.h"
#include "Semantic.h"
#include "ExtendGrammar.h"

class CalAttribute {
public:
    Semantic *semantic;
    ExtendGrammar *extendGrammar;

    CalAttribute(Semantic &semantic, ExtendGrammar &extendGrammar) {
        this->semantic = &semantic;
        this->extendGrammar = &extendGrammar;
    }

    /**
     * 树遍历法计算属性
     * @param node 当前遍历的节点
     */
    void visitNode(TreeNode *node) {
        if (node == nullptr)return;
        cal(node); //计算属性
        for (auto &child: node->children) {
            visitNode(child);// 遍历子树
        }
        cal(node); //计算属性
    }

    /**
     * 计算属性
     * @param node 要计算的节点
     */
    void cal(TreeNode *node) {
        if (!isVn(node->name))return;// 终结符属性不需要计算
        int id = node->useProductionId;
        Production useProduction = extendGrammar->order(id);// node->children使用的产生式
        vector<string> &action = semantic->action[id];// node->children使用的语义动作
        for (auto &a: action) {
            // 解析语义动作
            vector<pair<string, string>> analysisAction = semantic->analysisAction(a);
            // 属性是否计算过了
            TreeNode *calNode = getNode(node, analysisAction[0].first);
            string &attributeName = analysisAction[0].second;
            if (isCaled(calNode, attributeName)) continue;
            // 确认属性能否计算
            vector<string> *args = canCal(node, analysisAction);
            if (args == nullptr) continue;
            // 计算属性结果
            calNode->attributes[attributeName] =  semantic->calAction(a, *args);
            free(args);
        }
    }

    /**
     * node的attributeName是否计算过了
     */
    static bool isCaled(TreeNode *node, const string &attributeName) {
        return node->attributes.find(attributeName) != node->attributes.end();
    }

    /**
     * node的语义动作是否能执行
     * @return 如果能执行, 返回语义动作右式的参数列表, 否则返回nullptr
     */
    static vector<string> *canCal(TreeNode *node, vector<pair<string, string>> analysisAction) {
        vector<string> *ans = new vector<string>();
        for (int i = 1; i < analysisAction.size(); i++) {// 语义动作右侧
            // 需要的节点和属性
            string nodeName = analysisAction[i].first;
            string attributeName = analysisAction[i].second;
            TreeNode *child = getNode(node, nodeName);// 根据节点名获取子节点
            // 节点的该属性是否存在
            if (child->attributes.find(attributeName) == child->attributes.end()) {
                return nullptr;
            }
            // 可以计算, 放入参数列表
            ans->push_back(child->attributes[attributeName]);
        }
        return ans;
    }

    /**
     * 根据节点名获取节点
     * @param node 父节点
     * @param nodeName 节点名和下标
     * @return  例如 E0->E1+E2, E0.code=E2.code||E1.code
     *          输入E0 -> node
     *          输入E1 -> node.children[0]
     *          输入E2 -> node.children[2]
     */
    static TreeNode *getNode(TreeNode *node, string nodeName) {
        if (isParentNode(node, nodeName)) {
            return node;
        }
        int order = 1;// 是同名节点中的第几个
        if (nodeName.size() > 1) {// nodeName带标号
            order = stoi(nodeName.substr(1));
            nodeName = nodeName.substr(0, 1);
            if (node->name != nodeName)order++;// 如果与父节点名相同, 次序相对-1
        }
        // 查找子节点的第order个
        for (auto &child: node->children) {
            if (child->name == nodeName) {
                order--;
                if (order == 0) return child;
            }
        }
        return nullptr;
    }

    static bool isParentNode(TreeNode *node, const string &nodeName) {
        if (nodeName.size() > 1) {// nodeName带标号
            int order = stoi(nodeName.substr(1));
            if (order == 0)return true;//就是父节点
        } else {
            if (node->name == nodeName)return true;
        }
        return false;
    }
};

#endif //ANALYSIS_CALATTRIBUTE_H
