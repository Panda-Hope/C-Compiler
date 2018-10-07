#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 定义一个子、兄弟结点结构的森林 */
typedef struct Forest Forest;
struct Forest {
    Forest *child;
    Forest *nextSibling;
    char info;
};

bool isChild = true;                                // 新插入节点是否子节点
Forest *CreateForest(char info);
bool InsertNode(Forest *f, char info);
void S(char **lookhead);
void Match(char symbol, char **lookhead);


/*
 * 文法预测分析器，执行以下文法翻译
 * s->+ss | -ss | a
 * */
void S(char **lookhead)
{
    /* 文法预测first(A)匹配 */
    switch (**lookhead) {
        case '+':
            isChild = true;
            Match('+', **lookhead);
            S(**lookhead);
            S(**lookhead);
            break;
        case '-':
            isChild = true;
            Match('-', **lookhead);
            S(**lookhead);
            S(**lookhead);
            break;
        case 'a':
            isChild = true;
            Match('a', **lookhead);
            break;
        default:
            printf("语法错误");
            exit();
            break;
    }
}

void Match(char symbol, char **lookhead)
{
    if (**lookhead == symbol) {
        *lookhead++;
        
        if (isChild) {
            InsertNode(child, symbol);
        }else{
            InsertNode(sibling, symbol);
        }
    }else {
        printf("语法错误");
        exit(1);
    }
}

int main(void)
{
    char *str = "+aa";
    
    Forest *f = CreateForest('s');                  // 创建语法树根节点
    S(*str);
    
    return 0;
}


