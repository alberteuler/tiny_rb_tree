#include<stdio.h>
#include<iostream>
#include<assert.h>
#include<string>
// #pragma once

using namespace std;
enum RBT_color{RED, BLACK};

template <class type> 
class RBT_node {
public:
    RBT_color color;
    type val;
    RBT_node *left, *right;
    RBT_node *parent;
    RBT_node(type val__, RBT_color color__, RBT_node *l, RBT_color *r, RBT_color *p):val(val__),color(color__),left(l),right(r),parent(p) {}
    RBT_node(type val__):val(val__),color(BLACK),left(nullptr),right(nullptr),parent(nullptr){}
};

template <class type>
class RBT {
private:
    RBT_node<type> *root;
public:
    RBT():root(nullptr){}  //构造函数
    ~RBT(){
        destory(this->root);
        printf("this RB tree is delected safely.\n");
    } //析构函数

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
    RBT_node<type> *prev(RBT_node<type> *node);
    RBT_node<type> *next(RBT_node<type> *node);
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
    void postOrder(RBT_node<type> *root) const;
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
    void insertFixUp(RBT_node<type>* &root, RBT_node<type>* node);
    //删除值和删除修正函数
    void remove(RBT_node<type>* &root, RBT_node<type>* node);
    void removeFixUp(RBT_node<type>* &root, RBT_node<type>* node, RBT_node<type>* parent);
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


template<class type>
void RBT<type>::leftRotate(RBT_node<type>* &root, RBT_node<type> *x){
    /*
    ^对x进行左旋，意味着将x变成一个左节点。
    ^也就是说，如果x存在一个**右孩子**y，那么左旋后x会成为y的**左孩子**
    */
    RBT_node<type> *y = x->right;   //设置x的右孩子y
    if(y->left!=nullptr) y->left->parent = x;   //将y的左孩子设置为x的右孩子。也就是图中的b节点。
    y->parent = x->parent;          //将y的父亲设置为x的父亲
    if(x->parent == nullptr) root = y;
    else{
        if(x->parent->left == x){
            x->parent->left = y;
        }else{
            x->parent->right = y;
        }
    }
    //将x设置为y的左孩子
    y->left = x;        
    //将y设置为x的父节点            
    x->parent = y;

}
/* 
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(右旋)-->            /  \                     
 *        x   ry                           lx   y  
 *       / \                                   / \                   
 *      lx  rx                                rx  ry
 * 
 */
template<class type>
void RBT<type>::rightRotate(RBT_node<type>* &root, RBT_node<type> *y){
    /*
    ^看图说话就是了
    */
   RBT_node<type> *x = y->left; //x是y的左孩子
   x->parent = y->parent;
   if(y->parent == nullptr){
       root = x;
   }else{
       if(y == y->parent->left){
           y->parent->left = x;
       }else{
           y->parent->right = x;
       }
   }
   x->right = y;
   y->left = x->right;
}



//~============================插入和删除函数=========================//
/*
*插入函数分为以下三部分：
*1. 将红黑树当作一棵二叉查找树插入
*2. 将插入节点的颜色改为红色
*3. 通过一系列的旋转和着色操作使之重新成为黑树。
*/
template<class type>
void RBT<type>::insert(RBT_node<type>* &root, RBT_node<type>* node){
    //^ 预处理
    RBT_node<type> *y = nullptr;
    RBT_node<type> *x = root;
    
    //^1. 将红黑树当作一颗二叉查找树插入。
    while(x!=nullptr) {
        y = x;
        if(node->val < x->val){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    node->parent = y;

    if(y!=nullptr){
        if(node->val < y->val){
            y->left = node;
        }else{
            y->right = node;
        }
    }else{
        root = node;
    }

    //^2. 将插入节点的颜色改为红色。
    node->color = RED;
    //^3。 二叉树的修正
    insertFixUp(root,node);
}
/*
*以下是插入函数的外部接口
*/
template<class type> 
void RBT<type>::insert(type val){
    RBT_node<type> *node = new RBT_node<type>(val);
    assert(node!=nullptr);
    insert(root,node);
}

/*
*删除函数分为以下两部分：
*1. 将红黑树当作一棵二叉查找树删除
*2. 通过修正操作使之重新成为红黑树
*/

template<class type>
void RBT<type>::remove(RBT_node<type>* &root, RBT_node<type>* node){
    RBT_node<type> *child, *parent;
    RBT_color color;
    // 被删除节点的"左右孩子都不为空"的情况。
    if ( (node->left!=NULL) && (node->right!=NULL) ) 
    {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        RBT_node<type> *replace = node;
        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } 
        else 
            // "node节点"是根节点，更新根节点。
            root = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代节点"的颜色
        color = rb_color(replace);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node)
        {
            parent = replace;
        } 
        else
        {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else 
        child = node->right;

    parent = node->parent;
    // 保存"取代节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}
//删除函数的接口
template<class type>
void RBT<type>::remove(type val){
    RBT_node<type> *node;
    node = search(root, val);
    if(node != nullptr) remove(root,node);
}


//~========================插入和删除操作的修正函数====================//


/*
*修正的过程中，其重点在于保证特征4： 红色节点的子节点必须是黑色的
*/
template<class type>
void RBT<type>::insertFixUp(RBT_node<type>* &root, RBT_node<type>* node){
    RBT_node<type> *parent, *grand_parent;
    parent = rb_parent(root);
    //^若父节点存在切父节点的颜色是红色
    while(parent!=nullptr && rb_is_red(parent)){
        grand_parent = rb_parent(grand_parent);
        //^情况1. 父节点是祖父的左孩子
        if(parent = grand_parent->left){
            //情况1a : 叔叔节点是红色
            RBT_node<type> *uncle = grand_parent->right;    //叔叔节点是祖父节点的另外一个孩子
            if(uncle != nullptr && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(grand_parent);
                node = grand_parent;
                continue;
            }
            else if(parent->right == node){
                //情况1b: 叔叔节点是黑色，且当前节点是右孩子
                RBT_node<type> *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }else{
                //情况1c：叔叔节点是黑色，且当前节点是左孩子
                rb_set_black(parent);
                rb_set_red(grand_parent);
                rightRotate(root, grand_parent);
            }
            
        }else {
            //^情况2. 父节点是祖父的右孩子
            //^大致思路是一致的，不过需要让操作对称着来就行
            RBT_node<type> *uncle = grand_parent->left;
            if(uncle != nullptr && rb_is_red(uncle)){
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(grand_parent);
                node = grand_parent;
                continue;
            }else if(parent->right == node){
                RBT_node<type> *tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }else{
                rb_set_black(parent);
                rb_set_red(grand_parent);
                leftRotate(root,grand_parent);
            }
        }
    }
}

template <class type>
void RBT<type>::removeFixUp(RBT_node<type>* &root, RBT_node<type> *node, RBT_node<type> *parent)
{
    RBT_node<type> *other;

    while ((node!=nullptr || rb_is_black(node)) && node != root)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}


//~================================辅助性函数================================

//~----------------------------遍历函数-----------------------------------
template<class type>
void RBT<type>::preOrder(RBT_node<type> *root) const {
    if(root == nullptr) return;
    // preOrder(root->left);
    std::cout<<root->val<<std::endl;
    RBT<type>::preOrder(root->left);
    RBT<type>::preOrder(root->right);
} 

template<class type>
void RBT<type>::inOrder(RBT_node<type> *root) const {
    if(root == nullptr) return;
    RBT<type>::inOrder(root->left);
    std::cout<<root->val<<std::endl;
    RBT<type>::inOrder(root->right);
}

template<class type>
void RBT<type>::postOrder(RBT_node<type> *root) const{
    if(root == nullptr) return;
    RBT<type>::postOrder(root->left);
    RBT<type>::postOrder(root->right);
    std::cout<<root->val<<std::endl;
}

//遍历函数的接口

template<class type>
void RBT<type>::preOrder(){
    RBT<type>::preOrder(root);
}

template<class type>
void RBT<type>::inOrder(){
    RBT<type>::inOrder(root);
}

template<class type>
void RBT<type>::postOrder(){
    RBT<type>::postOrder(root);
}

//~---------------------------搜索函数-------------------------------------
template<class type>
RBT_node<type> *RBT<type>::search(RBT_node<type> *root, type val) const{
    if(root == nullptr) return nullptr;
    if(root->val == val) return root;
    else if(root->val > val){
        return RBT<type>::search(root->left, val);
    }else{
        return RBT<type>::search(root->right, val);
    }
}

template<class type>
RBT_node<type> *RBT<type>::iter_search(RBT_node<type> *root, type val) const {
    while(root->val!=val && root!=nullptr){
        if(root->val > val){
            root = root->right;
        }else if(root->val < val){
            root = root->left;
        }
    }
    return root;
}

//搜索函数的接口
template<class type>
RBT_node<type> *RBT<type>::search(type val){
    return RBT<type>::search(root, val);
}

template<class type>
RBT_node<type> *RBT<type>::iter_search(type val){
    return RBT<type>::iter_search(root, val);
}

//~--------------------------最小和最大值---------------------------------
template<class type>
RBT_node<type> *RBT<type>::min_val(RBT_node<type> *root){
    while(root->right!=nullptr && root != nullptr){
        root = root->right;
    }
    return root;
}

template<class type>
RBT_node<type> *RBT<type>::max_val(RBT_node<type> *root){
    while(root->left!=nullptr && root != nullptr){
        root = root->left;
    }
    return root;
}
//函数接口
template<class type>
type RBT<type>::min_val(){
    return RBT<type>::min_val(this->root)->val;
}

template<class type>
type RBT<type>::max_val(){
    return RBT<type>::max_val(this->root)->val;
}

//~--------------------------上一个和下一个节点---------------------------
template<class type> 
RBT_node<type> *RBT<type>::prev(RBT_node<type> *node){
    if(node->left != nullptr){
        return RBT<type>::max_val(node->left);
    }else{
        RBT_node<type> par = node->parent;
        while(par!=nullptr && node == par->right){
            node = par;
            par = par->parent;
        }
        return par;
    }
}

template<class type>
RBT_node<type> *RBT<type>::next(RBT_node<type> *node){
    if(node->right != nullptr){
        return RBT<type>::min_val(node->right);
    }else{
        RBT_node<type> par = node->parent;  //node的父节点
        while(par!=nullptr && node == par->left){
            node = par;
            par = par->parent;
        }
        return par;
    }
}

//~--------------------------销毁函数------------------------------------
template<class type>
void RBT<type>::destory(RBT_node<type> *&root){
    RBT_node<type> *tree_root = this->root;
    if(root == nullptr) return;
    if(root->left == nullptr && root->right == nullptr){
        RBT<type>::remove(tree_root, root);
    }
    destory(root->left);
    destory(root->right);
}
//销毁函数的接口
template<class type>
void RBT<type>::destory(){
    RBT<type>::destory(this->root);
}

//~--------------------------打印红黑树----------------------------------
template<class type>
void RBT<type>::print(RBT_node<type> *&root) {
    std::string str_color;
    type parent_val;
    if(root->parent == nullptr){
        parent_val = -1;
    }else{
        parent_val = root->parent->val;
    }
    if(root == nullptr){
        cout<<"print stop."<<endl;
        return;
    }
    if(root->color == RED){
        str_color = "red";
    }else{
        str_color = "black";
    }
    print(root->left);
    print(root->right);
    std::cout<<root<<" "<<str_color<<" "<<"  "<<root->val<<"    "<<parent_val<<std::endl;
    cout<<"testing message"<<endl;
}
//打印整颗红黑树
template<class type>
void RBT<type>::print() { 
    std::cout<<"address  color value parent"<<std::endl;
    std::cout<<"--------------------------"<<std::endl;
    RBT<type>::print(root);
}