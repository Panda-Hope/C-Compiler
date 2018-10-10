#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool NotOpreator(char c);
bool Unit(char c);
bool Character(char a, char b);

/*
 * << 正则表达式解析器 >>
 * 包含以下基本正则表达式字符集：
 *      c   -> 单个非运算字符 例如: (a、b)
 *      \c  -> 匹配字符C的字面值 例如: c (由于字符本身具有转义所以具体使用时应在一个转义符例如: Match("c", "\\c"))
 *      "   -> 匹配字符串值 例如: "c" = "c"
 *      .   -> 除换行符以外的任意字符
 *      $   -> 匹配行末尾
 * 包含以下运算符：
 *
 * 算法思路及概括:
 *      对于任意的正则表达式我们都存在文法: expr -> expr + operation
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
    
    bool result = true;
    while (*regex != '\0') {
        if (*expr == '\0') return false;                    // 正则匹配完毕，表达式已完结则失败
       
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
                do {
                    result = Character(*expr, *regex);
                    if (!result) return false;
                    
                    expr++;
                    regex++;
                    
                } while(*regex != '\"' && *regex != '\0');
//                printf("%c \n", *regex);
                if (*regex == '\0') {
                    printf("正则表达式语法错误, \" 缺少匹配\n");
                    exit(1);
                };
                break;
            default:
                result = Character(*expr, *regex);
                regex++;
                break;
        }
        expr++;
    }
    
    if (*expr != '\0') {return false;}                      // 正则匹配完毕，表达式未完结则失败
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
    bool r = Match("\"", "\"\"");
    
    printf("%s \n", r ? "true" : "false");
}

