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

        NODE_AstTree sibls = buildAST(Pchildren);
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
    int rule = root->rule_index+1;
    switch (rule) {
        case 1:{

            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;

            if(child1->tokens == NULL){
                NODE_AstTree program = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                program->sibling = NULL;
                program->tokens = NULL;
                program->parent = NULL;
                program->child = children;
                child1->parent = program;
                child2->parent = program;
                return program;
            }else{
                free(child1);
                child1->sibling = NULL;
                return child2;
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

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->sibling = NULL;
            res->tokens = NULL;
            res->parent = NULL;
            res->child = child1;

            if(child2->tokens == NULL){
                child1->parent = res;
                child2->parent = res;
                return res;
            }else{
                free(child2);
                child1->sibling = NULL;
                child1->parent = res;
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
            if(child3->tokens == NULL){
                child1->parent = res;
                child3->parent = res;
                child3->sibling = NULL;
                child1->sibling = child3;
            }else{
                child1->parent = res;
                child1->sibling = NULL;
                free(child3);
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
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;

            child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->next->tk = child2->tokens->tk;
            child1->sibling = NULL;
            child1->parent = NULL;
            child1->child = NULL;
            free(child2);
            return child1;


        }break;
        case 15:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            free(child1);
            return child2;
        }break;

        case 16:{
            return children;
        }break;

        case 17:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->sibling = NULL;
            res->child = child1;
            child1->parent = res;
            child2->parent = res;
            child3->parent = res;
            child4->parent = res;
            return res;
        }break;
        case 18:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = children;

            if(child2->tokens == NULL){
                child1->parent = res;
                child2->parent = res;
                child2->sibling = NULL;
            }else{
                child1->sibling = NULL;
                child1->parent = res;
                free(child2);
            }

            return res;
        }break;
        case 19:{
            return children;
        }break;
        case 20:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = children;

            child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->next->tk = child2->tokens->tk;
            child1->sibling = child3;
            free(child2);
            free(child4);
            free(child5);
            child1->parent = res;
            child3->parent = res;
            return res;
        }break;
        case 21:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = children;

            if(child3->tokens == NULL){
                child1->parent = res;
                child2->parent = res;
                child3->parent = res;
                child3->sibling = NULL;
            }else{
                child2->sibling = NULL;
                free(child3);
                child1->parent = res;
                child2->parent = res;
            }
            return res;
        }break;
        case 22:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;


            child2->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child2->tokens->next->tk = child4->tokens->tk;
            child2->sibling = NULL;
            free(child1);
            free(child3);
            free(child4);
            free(child5);
            return child2;

        }break;
        case 23:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;

            if(child2->tokens == NULL){
                return children;
            }else{
                child1->sibling =  NULL;
                free(child2);
                return children;
            }

        }break;
        case 24:{
            return children;
        }break;
        case 25:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = children;

            if(child2->tokens == NULL){
                child1->parent = res;
                child2->parent = res;
                child2->sibling = NULL;
            }else{
                child1->sibling = NULL;
                child1->parent = res;
                free(child2);
            }
            return res;
        }break;
        case 26:{
            return children;
        }break;
        case 27:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;

            child2->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child2->tokens->next->tk = child4->tokens->tk;
            child2->tokens->next->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child2->tokens->next->next->tk = child5->tokens->tk;
            free(child1);
            free(child3);
            free(child4);
            free(child5);
            free(child6);
            return child2;
        }break;
        case 28:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            free(child1);
            child2->sibling = NULL;
            return child2;
        }break;
        case 29:{
            return children;
        }break;
        case 30:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = children;

            if(child2->tokens == NULL){
                child1->parent = res;
                child2->parent = res;
                child2->sibling = NULL;
            }else{
                child1->sibling = NULL;
                child1->parent = NULL;
                free(child2);
            }
            return res;
        }break;
        case 31:{
            return children;
        }break;

        case 32:{

        }break;
        case 33:{

        }break;
        case 34:{

        }break;
        case 35:{

        }break;
        case 36:{

        }break;
        case 37:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child1;

            child1->sibling = child3;
            child3->sibling = NULL;
            child1->parent = res;
            child3->parent = res;

            free(child2); free(child4);
            return res;

        }break;
        case 38:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;

            child1->sibling = NULL;
            if(child2->tokens->tk->token == EPS){
                free(child2);
                return child1;
            }else{
                child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
                child1->tokens->next->tk = child2->tokens->tk;
                free(child2);
                return child1;
            }

        }break;

        case 39:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            free(child1);
            return child2;
        }break;
        case 40:{
            return children;
        }break;
        case 41:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;
            NODE_AstTree child7 = child6->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child1;

            child1->sibling = child3;
            child3->sibling = child6;
            child6->sibling = NULL;
            child1->parent = res;
            child3->parent = res;
            child6->parent = res;
            free(child2); free(child4); free(child5); free(child7);
            return res;

        }break;
        case 42:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            free(child3);
            free(child1);
            child2->sibling = NULL;
            return child2;
        }break;
        case 43:{
            return children;
        }break;
        case 44:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            free(child3);
            free(child1);
            child2->sibling = NULL;
            return child2;
        }break;

        case 45:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;
            NODE_AstTree child7 = child6->sibling;

            NODE_AstTree tmp = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            tmp->tokens = NULL;
            tmp->sibling = NULL;
            tmp->parent = NULL;
            tmp->child = child5;

            child5->sibling = child6;
            child6->sibling = NULL;
            child5->parent = tmp;
            child6->parent = tmp;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child3;

            child3->sibling = tmp;
            child3->parent = res;
            tmp->parent = res;

            free(child1); free(child2); free(child4); free(child7);

            return res;


        }break;
        case 46:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;
            NODE_AstTree child7 = child6->sibling;
            NODE_AstTree child8 = child7->sibling;

            NODE_AstTree tmp = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            tmp->tokens = NULL;
            tmp->sibling = NULL;
            tmp->parent = NULL;
            tmp->child = child6;

            child6->sibling = child7;
            child7->sibling = NULL;
            child6->parent = tmp;
            child7->parent = tmp;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child3;

            child3->sibling = tmp;
            tmp->sibling = child8;
            child8->sibling = NULL;
            child3->parent = res;
            tmp->parent = res;
            child8->parent = res;

            free(child1); free(child2); free(child4); free(child5);

            return res;


        }break;
        case 47:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child2;

            child2->sibling = child3;
            child3->sibling = NULL;
            child2->parent = res;
            child3->parent = res;

            free(child1); free(child4);
            return res;

        }break;
        case 48:{
            children->tokens = NULL;
            return children;
        }break;
        case 49:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;

            child1->sibling = NULL;
            child1->child = child3;
            child3->parent = NULL;
            child3->sibling = NULL;

            free(child2); free(child4); free(child5);

            return child1;

        }break;
        case 50:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;

            child1->sibling = NULL;
            child1->child = child3;
            child3->parent = NULL;
            child3->sibling = NULL;

            free(child2); free(child4); free(child5);

            return child1;
        }break;
        case 51:{

        }break;
        case 52:{

        }break;
        case 53:{
            return children;
        }break;
        case 54:{

        }break;
        case 55:{

        }break;
        case 56:{
            return children;
        }break;
        case 57:{

        }break;
        case 58:{
            return children;
        }break;
        case 59:{
            return children;
        }break;
        case 60:{
            return children;
        }break;
        case 61:{
            return children;
        }break;
        case 62:{
            return children;
        }break;
        case 63:{
            return children;
        }break;
        case 64:{
            return children;
        }break;
        case 65:{
            return children;
        }break;
        case 66:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;
            NODE_AstTree child5 = child4->sibling;
            NODE_AstTree child6 = child5->sibling;
            NODE_AstTree child7 = child6->sibling;

            child2->sibling = child6;
            child6->sibling = NULL;
            child2->parent = child4;
            child6->parent = child4;
            child4->child  = child2;
            child2->sibling = NULL;

            free(child1); free(child3); free(child5); free(child7);
            return child4;

        }break;
        case 67:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;

            child1->sibling = child3;
            child3->sibling = NULL;
            child1->parent = child2;
            child3->parent = child2;
            child2->child = child1;
            child2->sibling = NULL;

            return child2;

        }break;
        case 68:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            NODE_AstTree child4 = child3->sibling;

            child1->child = child3;
            child1->sibling = NULL;
            child3->sibling = NULL;
            child3->parent = child1;
            free(child2); free(child4);

            return child1;

        }break;

        case 69:{
            return children;
        }break;
        case 70:{
            return children;
        }break;
        case 71:{
            return children;
        }break;
        case 72:{
            return children;
        }break;
        case 73:{
            return children;
        }break;
        case 74:{
            return children;
        }break;
        case 75:{
            return children;
        }break;
        case 76:{
            return children;
        }break;

        case 77:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            free(child1);
            free(child3);
            child2->sibling = NULL;
            if(child2->tokens == NULL){
                NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                res->tokens = NULL;
                res->sibling = NULL;
                res->parent = NULL;
                res->child = child2;
                return res;
            }else{
                return child2;
            }

        }break;
        case 78:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            NODE_AstTree child3 = child2->sibling;
            free(child3);
            free(child1);
            child2->sibling = NULL;
            return child2;

        }break;
        case 79:{
            return children;
        }break;
        case 80:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;

            if(child2->tokens == NULL){
                return child1;
            }else{
                child1->sibling = NULL;
                free(child2);
                return child1;
            }

        }break;
        case 81:{
            NODE_AstTree child1 = children;
            NODE_AstTree child2 = child1->sibling;
            free(child1);
            child2->sibling = NULL;
            return child2;
        }break;
        case 82:{
            return children;
        }break;

    }
}
