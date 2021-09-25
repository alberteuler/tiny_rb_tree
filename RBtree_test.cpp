#include<iostream>
#include"RBtree.h"
#include<vector>
using namespace std;
int main(){
    RBT<int> *tree = new RBT<int>();
    vector<int> input_data;
    //生成测试数据
    for(int i = 0; i < 10; i++){
        int value = i * 10;
        input_data.push_back(value);
    }
    //将测试数据插入
    for(int data : input_data){
        tree->insert(data);
        cout<<"inserting"<<" "<<data<<endl;
    }
    //测试搜索功能
    RBT_node<int> *treenode = tree->search(20);
    if(treenode != nullptr){
        std::cout<<"search result = "<<treenode<<std::endl;
    }else{
        std::cout<<"not found"<<endl;
    }
    //测试删除功能
    tree->remove(20);
    tree->postOrder();
    //测试打印功能
    tree->print();
    //测试销毁功能
    tree->destory();
    return 0;
};
