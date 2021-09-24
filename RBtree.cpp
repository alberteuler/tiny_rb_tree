#include"RBtree.h"
#include<assert.h>
#pragma once
/*
////：完成左旋和右旋函数
////完成节点的插入和删除函数。
TODO: 完成节点的搜索函数
TODO: 完成节点的删除和打印函数
TODO: 完成树的遍历函数
TODO: 写一个测试用例
*/

//~======================左旋和右旋函数==========================//
/* 
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      parent                          parent
 *     /                               /
 *    x                               y                
 *   /  \      --(左旋)-->           / \                
 *  a    y                          x  c     
 *     /   \                       /  \
 *    b     c                     a    b  
 *
 *
 */
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
   x->right = u;
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
        if(node->key < y->key){
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
    RBT_node<type> *node = new RBT_node(val);
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



/*
?================================appendix=================================?
?红黑树的特征:
?1. 每个节点不是黑色就是红色
?2. 根节点是黑色的
?3. 叶子节点（特指值为空的叶子节点）也是黑色的
?4. 如果当前节点是红色的，那么子节点必须是黑色的
?5、 从一个节点当该节点的子孙节点的所有路径上包含相同数目的黑色节点。
*/