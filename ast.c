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
            node->tokens->tk = root->tk_info.tk;
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
        lst->sibling = NULL;

        NODE_AstTree res = applyRule(root,sibls);
        return res;
    }

    return NULL;
}

NODE_AstTree applyRule(TREE_NODE root, NODE_AstTree children){
    int rule = root->ruleNo+1;
    switch (rule) {
        case 1:{

            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;

            if(child1->tokens->tk->token == EPS){
                free(child1);
                return child2;
            }else{
                NODE_AstTree program = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                program->sibling = NULL;
                program->tokens = NULL;
                program->parent = NULl;
                program->child = children;
                child1->parent = program;
                child2->parent = program;
                return program;
            }

        }break;

        case 2:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            child2->sibling = NULL;
            free(child1);
            free(child3);
            return child2;
        }break;

        case 3:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = children->sibling;

            if(child2->tokens->tk->token = EPS){
                free(child2);
                child1->sibling = NULL;
                NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                res->sibling = NULL;
                res->tokens = NULL;
                res->parent = NULL;
                res->child = child1;
                res->child->sibling = NULL;
                child1->parent = res;
                return res;
            }else{
                NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                res->sibling = NULL;
                res->tokens = NULL;
                res->parent = NULL;
                res->child = child1;
                child1->parent = res;
                child2->parent = res;
                return res;
            }

        }break;

        case 4:{
            return children;
        }break;

        case 5:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->child = child1;
            res->child->sibling = child2;
            res->child->sibling->sibling = child3;
            res->child->sibling->sibling->sibling = child5;
            free(child4);
            free(child6);
            res->sibling = NULL;
            child1->parent = res;
            child2->parent = res;
            child3->parent = res;
            child5->parent = res;
            child5->sibling = NULL;
            return res;
        }break;

        case 6:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;

            child5->sibling = NULL;
            free(child1);
            free(child2);
            free(child3);
            free(child4);
            free(child6);
            return child5;

        }break;

        case 7:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;

            child5->sibling = NULL;
            free(child1);
            free(child2);
            free(child3);
            free(child4);
            free(child6);
            return child5;
        }break;
        case 8:{
            return children;
        }break;
        case 9:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;

            child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->next->tk = child2->tokens->tk;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->sibling = NULL;
            res->parent = NULL;
            res->tokens = NULL;
            res->child = child1;

            free(child2);
            if(child3->tokens->tk->token = EPS){
                child1->parent = res;
                child1->sibling = NULL;
                free(child3);
            }else{
                child1->parent = res;
                child3->parent = res;
                child3->sibling = NULL;
                child1->sibling = child3;
            }

            return res;


        }break;
        case 10:{
            return children;
        }break;
        case 11:{
            return children;
        }break;
        case 12:{
            return children;
        }break;
        case 13:{
            return children;
        }break;
        case 14:{
            NODE_AstTree
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

NODE_AstTree newNodewithAllChildren(NODE_AstTree children){

    NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
    res->tokens = NULL;
    res->parent = NULL;
    res->child = children;
    res->sibling = NULL;

    NODE_AstTree temp = children;
    while(temp!=NULL){
        temp->parent = res;
    }

    return res;
}
