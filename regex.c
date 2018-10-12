#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool NotOpreator(char c);
bool Unit(char c);
bool Character(char a, char b);
bool String(char **e, char**r);
bool LineEnd(char a);
bool DSMatch(char **expr, char **regex);
void Multiple(char **e, char **r);

/*
 * << 正则表达式解析器 >>
 * 正则表达式基本概念:
 *      这里首先构建一些基本的概念: 表达式(expr)与数据集(dataset)而表达式本身即是数据集即(expr = dataset)，
 *      由下文的算法思路所讲的文法得出我们可以将任意的表达式映射成expr -> expr + operation
 *      也就是说任意的数据集D均可映射成表达式(expr) + 相应的操作构建而成。
 *      而正则表达式的本质即是字符集(S)是否是数据集(dataset)中的一个子串，
 *      由此这里将正则表达式(expr)的验证转换为对于字符集(S)是否存在于数据集(dataset)的校验，
 *      而我们将字符集(S)抽象为函数(fn)所以将具体的过程变为 isExist = fn(S)
 * 包含以下基本正则表达式字符集：
 *      c   -> 单个非运算字符 例如: (a、b)
 *      \c  -> 匹配字符C的字面值 例如: c (由于字符本身具有转义所以具体使用时应在一个转义符例如: Match("c", "\\c"))
 *      "   -> 匹配字符串值 例如: "c" = "c"
 *      .   -> 除换行符以外的任意字符
 *      $   -> 匹配行末尾
 * 包含以下运算符：
 *      r*  -> 和表达式r匹配的零个或多个串连接成的串
 *
 * 算法思路及概括:
 *      对于任意的正则表达式我们都存在文法产生式: expr -> expr + operation
 *      也就是说对于任意的正则表达式expr 我们都存在有可以将其分解到最低层级expr(即基本的数据集形如s={a,b,c...})
 *      及相应的操作。由此我们利用分治法的思想将最低级表达式及其操作(expr + operation)与字符集(s)相匹配，继而向上冒泡
 *      完成正则匹配。
 * 缺陷:
 *      这里并没有对正则表达式本身的语法进行深入的检查。
 *      不考虑运算符的叠加例如: expr = c++
 *      不考虑后表达式是前表达式的子表达式例如: expr = c+8
 * */

/*
 * 正则匹配函数
 * char *expr: 需要匹配的字符串
 * char *regex: 正则表达式
 * */
bool Match(char *expr, char *regex)
{
    if (*regex == '\0') {
        printf("非法的正则表达式\n");
        exit(1);
    }
    
    bool result = false;
    char *dataset;
    
    while (*regex != '\0') {
        dataset = regex;                                    // 当前表达式数据集
        result = DSMatch(&expr, &regex);
        
        if (*expr != '\0') expr++;
        regex++;
        
        switch (*regex) {
            case '*':
                if (result) Multiple(&expr, &dataset);
                result = true;
                regex++;
                break;
            case '+':
                if(result) Multiple(&expr, &dataset);
                regex++;
            default:
                break;
        }
        
        if (!result) return false;
    }
    
    if (*expr != '\0') {return false;}                      // 正则匹配完毕，表达式未完结则失败
    return result;
}

/*
 * 基本数据集匹配
 * */
bool DSMatch(char **expr, char **regex)
{
    bool result = false;
    static const char operators[] = {'*', '+', '?', '{', '}', '|', '(', ')', '/'};
    
    switch (**regex) {
        case 'c':
            result = NotOpreator(**expr);
            break;
        case '.':
            result = Unit(**expr);
            break;
        case '\\':
            result = Character(**expr, *++(*regex));
            break;
        case '\"':
            result = String(expr, regex);
            break;
        case '$':
            result = LineEnd(**expr);
            break;
        default:
            for (int i=0;i<sizeof(operators)/sizeof(operators[0]);i++) {
                if (**regex == operators[i]) {
                    printf("语法错误, 错误的表达式符号%c缺乏运算分量\n", operators[i]);
                    exit(1);
                }
            }
            
            result = Character(**expr, **regex);
            break;
    }
    
    return result;
}

void Multiple(char **e, char **r)
{
    char *d = *r;
    while (DSMatch(e, &d)) {
        (*e)++;
        d = *r;
    }
}

/*
 * 字符匹配函数
 * 算法思路：
 *      形如字符("s")则对""对的内容解析为字符而不是正则表达式例如: "s" != "c"、"s" = "s"
 * */
bool String(char **e, char **r)
{
    char *closed = *r;
    bool result = false;
    
    do {
        closed++;
        if (*closed == '\"') break;
    } while (*closed != '\0');
    
    result = Character(**e, **r);
    if (*closed == '\"') {
        while (result) {
            (*e)++;
            (*r)++;
            result = Character(**e, **r);
            if (**r == '\"') break;
        }
    }else {
        printf("语法错误，缺少\"字符匹配\n");
        exit(1);
    }
    
    return result;
}

bool LineEnd(char a)
{
    return a == '\n';
}

bool Character(char a, char b)
{
    return a == b;
}

bool NotOpreator(char c)
{
    const char C_Opreators[] = {'+', '-', '*', '/', '%', '\0'};
    
    for (int i=0;i<sizeof(C_Opreators)/sizeof(C_Opreators[0]);i++) {
        if (C_Opreators[i] == c) return false;
    }
    return true;
}

bool Unit(char c)
{
    return c != '\n' && c != '\0';
}

int main()
{
    bool r = Match("", "\"ss\"*");
    
    printf("%s \n", r ? "true" : "false");
}

