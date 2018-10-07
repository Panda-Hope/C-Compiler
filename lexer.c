/*
 * 词法分析器
 * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LEXE_LEN 20
#define TOKEN_LEN 10

/* 词法单元结构 */
enum TokenTag {STRING, INT, FLOAT, UNDEF};
typedef struct Token Token;
union TokenAttr {
    char name[LEXE_LEN];
    int i;
    float f;
};
struct Token {
    char lexe[TOKEN_LEN];
    enum TokenTag tag;
    union TokenAttr attr;
};


/*
 * 词法单元生成器
 * */
Token* Scan(char *s)
{
    
    int i = 0;
    int line = 0;
    
    /* 剔除空白与注释, 行数累加器 */
    for (;;i++) {
        if (s[i] == ' ' || s[i] == '\t') {
            continue;
        }else if (s[i] == '\t') {
            line++;
            continue;
        }else if (s[i] == '/') {
            char next = s[i+1];
            
            if (next == s[i]) {
                i += 2;
                while (s[i] != '\n') {
                    i++;
                }
                continue;
            }else if (next == '*') {
                i += 2;
                while (s[i] != '*' && s[i+1] != '/') {
                    i++;
                }
                i++;
                continue;
            }
        }
        
        break;
    }
    
    if (s[i-1] == '\0') i--;                  // 防止内存溢出
    

    if (isdigit(s[i])) {
        float f = 10;
        float n = s[i]-48;

        i++;
        for (;;i++) {
            if (isdigit(s[i])) {
                n = n*f + (float)(s[i]-48);
                f *= 10;
            }else if (s[i] == '.') {
                f = 0.1;
            }else {
                break;
            }
        }

        Token *t = malloc(sizeof(Token));

        if (f > 0) {
            strcpy(t->lexe, "num");
            t->tag = INT;
            t->attr.i = (int)n;
        }else {
            strcpy(t->lexe, "float");
            t->tag = FLOAT;
            t->attr.f = n;
        }
        printf("%f ", n);
        return t;
    }
    
    
    return NULL;
//    printf("%c \n", s[i]);
}

int main()
{
    char s[] = "    /*he \nllo  a*/69pple";
    
    Scan(s);
    
    return 0;
}
