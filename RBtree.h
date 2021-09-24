#include<stdio.h>
#include<iostream>
#pragma once

enum RBT_color{RED, BLACK};

template <class type> 
class RBT_node {
public:
    RBT_color color;
    type val;
    RBT_node *left, *right;
    RBT_node *parent;
    RBT_node(type val__, RBT_color color__, RBT_node *l, RBT_color *r, RBT_color *p):val(v),color(color__),left(l),right(r),parent(p) {}
};

template <class type>
class RBT {
private:
    RBT_node<type> *root;
public:
    RBT();  //构造函数
    ~RBT(); //析构函数

    //前序，中序和序遍历
    void preOrder();
    void inOrder();
    void postOrder();

    //搜索值(递归版)
    RBT_node<type> *search(type val);
    //搜索值（非递归
    RBT_node<type> *iter_search(type val);
    //寻找极小值和极大值
    type min_val();
    type max_val();
    //上一个节点和下一个节点
    RBT_node<type> prev(RBT_node<type> *node);
    RBT_node<type> next(RBT_node<type> *node);
    //插入节点
    void insert(type val);
    //删除节点
    void remove(type val);
    //销毁红黑树
    void destory();
    //打印红黑树
    void print();

private:
    //前序，中序和后序遍历
    void preOrder(RBT_node<type> *root) const;
    void inOrder(RBT_node<type> *root) const;
    void posrOrder(RBT_node<type> *root) const;
    //搜索值
    RBT_node<type> *search(RBT_node<type> *root, type val) const;
    RBT_node<type> *iter_search(RBT_node<type> *root, type val) const;
    //最小和最大值
    RBT_node<type> *min_val(RBT_node<type> *root);
    RBT_node<type> *max_val(RBT_node<type> *root);
    //左旋和右旋
    void leftRotate(RBT_node<type>* &root, RBT_node<type> *x);
    void rightRotate(RBT_node<type>* &root, RBT_node<type> *y);
    //插入值和插入修正函数
    void insert(RBT_node<type>* &root, RBT_node<type>* node);
    void insertFixUp(RBT_node<type>* &root, RBT_node* node);
    //删除值和删除修正函数
    void remove(RBT_node<type>* &root, RBT_node<type>* node);
    void removeFixUp(RBT_node<type>* &root, RBT_node<type>* node);
    //删除和打印红黑树
    void destory(RBT_node<type>* &root);
    void print(RBT_node<type>* &root);


    //一堆宏函数，其实可以Inline
#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)
};