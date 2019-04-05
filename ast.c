#include "astDef.h"

NODE_AstTree buildAST(TREE_NODE root){
    if(root!=NULL){

        TREE_NODE Pchildren = root->child;

        if(Pchildren == NULL){
            NODE_AstTree node = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            node->child = NULL;
            node->sibling = NULL;
            node->parent = NULL;
            node->tokens = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            node->tokens->tk = root->node_info;
            node->tokens->next = NULL;
            return node;
        }

        NODE_AstTree sibLs = buildAST(Pchildren);
        Pchildren = Pchildren->next;
        NODE_AstTree lst = sibls;
        while(Pchildren!= NULL && lst!=NULL){
            lst->sibling = buildAST(Pchildren);
            lst = lst->sibling;
            Pchildren = Pchildren->next;
        }

        NODE_AstTree res = applyRule(root,sibls);
        return res;
    }

    return NULL;
}

NODE_AstTree applyRule(TREE_NODE root, NODE_AstTree children){
    int rule = root->ruleNo;
    switch (rule) {
        case 0:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = children->sibling;

            if(child1 == NULL){
                return child2;
            }else{
                NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                res->tokens = NULL;
                res->parent = NULL;
                res->child = children;
                child1->parent = res;
                child2->parent = res;
                res->sibling = NULL;
                return res;
            }

        }break;

        case 1:{
            NODE_AstTree child = children->sibling;
            return child;
        }break;

        case 2:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = children->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->child = children;
            res->sibling = NULL;

            if(child1!=NULL){
                child1->parent = res;
            }
            if(child2!= NULL){
                child2->parent = res;
            }

            return res;
        }break;

        case 3:{
            return NULL;
        }break;

        case 4:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child2 = child2->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->child = children;
            res->sibling = NULL;

            if(child1!=NULL){
                child1->parent = res;
            }
            if(child2!= NULL){
                child2->parent = res;
            }
            if(child3!= NULL){
                child3->parent = res;
            }

            return res;
        }break;

        case 5:{

        }break;

        case 6:{

        }break;
        case 7:{

        }break;
        case 8:{

        }break;
        case 9:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;
        case 0:{

        }break;

    }
}
