#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool NotOpreator(char c);
bool Unit(char c);
bool Character(char a, char b);
bool String(char **e, char**r);

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
 *      这里并没有对正则表达式本身的语法进行深入的检查。
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
    while (*regex != '\0') {
        switch (*regex) {
            case 'c':
                result = NotOpreator(*expr);
                regex++;
                break;
            case '.':
                result = Unit(*expr);
                regex++;
                break;
            case '\\':
                result = Character(*expr, *++regex);
                regex++;
                break;
            case '\"':
                result = String(&expr, &regex);
                regex++;
                break;
            case '*':
//                result = Multiple(&expr, *regex);
                break;
            default:
                result = Character(*expr, *regex);
                regex++;
                break;
        }
        
        if (!result) return false;
        switch (*regex) {
            case '*':
                result = Multiple(*expr, *regex);
                break;
            default:
                break;
        }
        expr++;
    }
    
    if (*expr != '\0') {return false;}                      // 正则匹配完毕，表达式未完结则失败
    return result;
}

bool Multiple(char **expr, char *regex)
{
    dataset = DS(*(regex-1));
    
    while (**expr in dataset) {
        *expr++;
    }
    return true;
    
    
}


bool *DS(char *r)
{
//    switch (*) {
//        case 'c':
//            return NotOpreator(*expr);
//            break;
//        case '.':
//            return Unit(*expr);
//            regex++;
//            break;
//        case '\\':
//            result = Character(*expr, *++regex);
//            regex++;
//            break;
//        case '\"':
//            result = String(&expr, &regex);
//            regex++;
//            break;
//        case '*':
//            result = Multiple(&expr, *regex);
//            break;
//        default:
//            result = Character(*expr, *regex);
//            regex++;
//            break;
//    }
    return false;
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



bool Character(char a, char b)
{
    return a == b;
}

bool NotOpreator(char c)
{
    const char C_Opreators[] = {'+', '-', '*', '/', '%'};
    bool r = true;
    
    for (int i=0;i<sizeof(C_Opreators)/sizeof(C_Opreators[0]);i++) {
        if (C_Opreators[i] == c) return false;
    }
    return r;
}

bool Unit(char c)
{
    return c != '\n';
}

int main()
{
    bool r = Match("\"s\"s", "\"s\"c");
    
    printf("%s \n", r ? "true" : "false");
}

