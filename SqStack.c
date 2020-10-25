#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//单元测试宏定义
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                           \
    do                                                                                                             \
    {                                                                                                              \
        test_count++;                                                                                              \
        if (equality)                                                                                              \
            test_pass++;                                                                                           \
        else                                                                                                       \
        {                                                                                                          \
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                          \
        }                                                                                                          \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define Stack_Init_Size 10
#define StackIncrement 10
typedef int SElemType; //元素类型
typedef struct
{
    SElemType *base; //动态空间基址
    int top;         //实际元素个数
    int stacksize;   //当前存储容量(以 sizeof(ElemType)为单位)
} SqStack;
static const char OP[] = {'+', '-', '*', '/', '(', ')', '#'};

enum Status //状态类型
{
    OK = 1,
    TRUE = 2,
    FALSE = 0,
    ERROR = -111,
    OVERFLOW = -1,

};

typedef enum Status Status;

Status InitStack(SqStack **S)
{
    if (S == NULL)
    {
        return ERROR;
    }
    //为栈分配空间
    *S = (SqStack *)malloc(sizeof(SqStack));
    if (*S == NULL)
    {
        return OVERFLOW;
    }
    //为栈中的数组分配空间
    (*S)->base = (SElemType *)malloc(sizeof(SElemType) * Stack_Init_Size);
    if ((*S)->base == NULL)
    {
        return OVERFLOW;
    }

    //初始化top, stacksize属性
    (*S)->top = 0;
    (*S)->stacksize = Stack_Init_Size;
    return OK;
}

Status Push(SqStack *S, SElemType e)
{
    if (S == NULL)
    {
        return ERROR;
    }

    (S->top)++;
    //栈空间不够时，开辟更大空间，并将旧栈中内容移动新栈
    if (S->top > S->stacksize)
    {
        S->stacksize += StackIncrement;
        S->base = (SElemType *)realloc(S->base, S->stacksize);
        if (S->base == NULL)
        {
            return OVERFLOW;
        }
    }
    //添加新元素
    *(S->base + S->top - 1) = e;
    return OK;
}


Status Pop(SqStack *S, SElemType *e)
{
    if ((S->top) == 0 || e == NULL)
    {
        return ERROR;
    }
    //弹出栈中元素
    *e = *(S->base + S->top - 1);
    //删除元素
    (S->top)--;
    return OK;
}

SElemType GetTop(SqStack S)
{
    if (S.top != 0)
    {
        //用偏移量返回栈顶的值
        return *(S.base + S.top - 1);
    }
    return ERROR;
}

int Operate(int a, int Operator, int b)
{
    switch (Operator)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    default:
        return ERROR;
    }
}

Status IsOperator(char c, int *code)
{
    
    int i;

    if (isdigit(c))
    {
        *code = c - '0';
        return FALSE;
    }
    //判断输入符号是否运算符
    for (i = 0; i < sizeof(OP) / sizeof(char); i++)
    {
        if (OP[i] == c)
        {
            return TRUE;
        }
    }
    return ERROR;
}


int find_index(int code)
{
    int i;
    //返回索引用于访问存放优先级的二维数组
    for (i = 0; i < sizeof(OP) / sizeof(char); i++) 
    {
        if (OP[i] == code)
            return i;
    }
    
}

int StackEmpty(SqStack tmp)
{
    return tmp.top == 0;
}

int Precede(int precode, int nextcode)
{
    //存放优先级
    const int levels[][7] = {{1, 1, -1, -1, -1, 1, 1},
                             {1, 1, -1, -1, -1, 1, 1},
                             {1, 1, 1, 1, -1, 1, 1},
                             {1, 1, 1, 1, -1, 1, 1},
                             {-1, -1, -1, -1, -1, 0, INT_MAX},
                             {1, 1, 1, 1, INT_MAX, 1, 1},
                             {-1, -1, -1, -1, -1, INT_MAX, 0}};

    int tmp;
    if (IsOperator(precode, &tmp) == ERROR || IsOperator(nextcode, &tmp) == ERROR)
        return ERROR;
    int pre_index = find_index(precode), next_index = find_index(nextcode);
    if (levels[pre_index][next_index] == 1)
    {
        return 1;//precode > nextcode
    }
    else if (levels[pre_index][next_index] == -1)
    {
        return -1;//precode < nextcode
    }
    else if (levels[pre_index][next_index] == 0)
    {
        return 0;//precode = nextcode
    }
    else
    {
        return 2;
    }
}

int EvaluateExpression(char exp[], int *ans)
{
    
    char c;
    SElemType theta, x, a, b;
    int tmp, pos = 0;
    SqStack *OPTR;
    SqStack *OPND;

    InitStack(&OPTR);
    InitStack(&OPND);
    Push(OPTR, '#');
    c = exp[pos++];
    
    while (c != '#' || GetTop(*OPTR) != '#')
    {
        if (!IsOperator(c, &tmp))
        {
            //不为运算符，压入数字栈
            Push(OPND, tmp);
            c = exp[pos++];
        }
        else
        {
            switch (Precede(GetTop(*OPTR), c))
            {
            case -1: //precode < nextcode
                //将运算符压入运算符栈
                //读入下一个字符
                Push(OPTR, c);
                c = exp[pos++];
                break;
            case 0: //precode = nextcode
                //弹出运算符栈栈顶元素
                //读入下一个字符
                Pop(OPTR, &x);
                c = exp[pos++];
                break;
            case 1:                 //precode > nextcode
                Pop(OPTR, &theta); // 弹出运算符
                Pop(OPND, &b);     // 弹出右边的操作数
                Pop(OPND, &a);     // 弹出左边的操作数
                Push(OPND, Operate(a, theta, b)); //运算
                break;
            case ERROR:
                return ERROR;
            }
        }
    }
    
    while (StackEmpty(*OPTR))
    {
        Pop(OPTR, &c);
        if (c != '#')
        {
            return ERROR;
        }
    }
    if (OPND->top > 1)
    {
        return ERROR;
    }
    
    Pop(OPND, &c);
    *ans = c;
    return OK;
}
#define test_num 5

int t_recr(int n)
{
    if (n == 0)
    {
        return 0;
    }
    
    return 2 * t_recr(n / 2) + n;
}// 0 1  2 3 7
// 0  1  4 5 17
int t_while(int n)
{
    SqStack *tmp;
    int ans = 0;
    int val;
    InitStack(&tmp);
    if (n == 0)
    {
        return 0;
    }
    do
    {
        Push(tmp, n);
        n /= 2;
    } while (n);
    
    
    while (Pop(tmp, &val)!= ERROR)
    {
        ans = 2 * ans + val;
    }
    return ans;
}

int main()
{
    int i;
    char string[][20] = {{"2*2*2#"},
    {"2*(1+1)*2#"}, {"1+8/4+1#"}, {"2*(8/4+1)#"}, {"9*(1+8)+1#"}};
    int answer[] = {8, 8, 4, 6, 82};
    int test_compute[] = {0, 1, 2, 3, 7};
    int Ans[] = {0, 1, 4, 5, 17};
    SqStack *tmp;
    char input[30];
    int a;
    int ans;
    
    
    //以下为单元测试
    printf("测试InitStack函数...\n\n");
    InitStack(&tmp);
    printf("初始化后的Stack:\nbase存放的地址:%p  stacksize: %d, top: %d\n", tmp->base, tmp->stacksize, tmp->top);

    printf("\n测试Push函数...\n\n");
    Push(tmp, 1);
    printf("執行一次Push操作后: stacksize: %d, top: %d\n", tmp->stacksize, tmp->top);
    for ( i = 0; i < 10; i++)
    {
        Push(tmp, i);
    }
    printf("執行十一次Push操作后: stacksize: %d, top: %d\n", tmp->stacksize, tmp->top);
    
    printf("\n测试Pop函数...\n\n");
    Pop(tmp, &a);
    EXPECT_EQ_INT(9, a);
    printf("Pop出的值為%d", a);

    for (i = 0; i < 10; i++) 
    {
        Pop(tmp, &a);
    }
    printf("\n测试StackEmpty函数...\n\n");
    EXPECT_EQ_INT(1, StackEmpty(*tmp));
    printf("清空Stack后 top: %d", tmp->top);

    printf("\n测试Operate函数...\n\n");
    printf("1 + 10 = %d\n", Operate(1, '+', 10));
    printf("30 * 2 = %d\n", Operate(30, '*', 2));
    
    
    for ( i = 0; i < test_num; i++)
    {
        EvaluateExpression(string[i], &ans);
        EXPECT_EQ_INT(answer[i], ans); 
        printf("第%d样例结果: %d\n",i + 1, ans);

    }
    
    for ( i = 0; i < sizeof(test_compute) / sizeof(int); i++)
    {
        EXPECT_EQ_INT(Ans[i], t_recr(test_compute[i]));
        EXPECT_EQ_INT(Ans[i], t_while(test_compute[i]));
    }
    printf("8\nInput Expression: ");
    gets(input);
    if (EvaluateExpression(input, &ans) != ERROR)
        printf("result = %d\n", ans);
    else
    {
        printf("\n表达式错误\n");
    }
    
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}