#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define True 11
#define False 0
#define Ok 111
#define Error -111
#define List_Init_Size 10
#define ListIncrement 10
#define LIST_INIT_SIZE 10
#define LISTINCREMENT 10
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


typedef struct{
 int num; //专家号，不重复，可用于查找专家
 char name[30]; //专家姓名，可能重复
}ElemType; //元素类型-----专家信息数据类型
typedef ElemType ET; 
typedef struct Lnode
{
 ElemType data;
 struct Lnode *next;
}LNode, *LinkList; //链表结点类型定义


enum Status
{
    OK = 1,
    TRUE = 2,
    FALSE = 0,
    ERROR = 0,
    OVERFLOW = -1,

};
//枚举类型
typedef enum Status Status;


//L是二重指针,才能修改main函数中的指针变量
Status InitList_L(LinkList *L)
{
    if (L == NULL)
    {
        return ERROR;
    }
    //开辟空间
    *L = (LNode *)malloc(sizeof(LNode));
    //开辟是否成功
    if (*L == NULL)
    {
        return ERROR;
    }
    (*L)->next = NULL;
    return OK;
}

//O(n)
int ListLength_L(LinkList L)
{
    int counter = 0;
    if (L == NULL)
    {
        return ERROR;
    }
    
    LNode *temp = L->next;
    //用计数器counter,延时实现ListLength
    while (temp)
    {
        counter++;
        temp = temp->next;
    }
    return counter;
}

//O(n)
Status GetElem_L(LinkList L, int i, ElemType *e)
{
    int j;
    //判断无效情况(为空，越界)
    if (L == NULL || i < 1 || i > ListLength_L(L) || e == NULL)
    {
        return ERROR;
    }
    LNode *temp = L->next;
    //j作为计数器，找到对应位置元素
    for (j = 1; temp; j++)
    {
        if (j == i)
        {
            *e = temp->data;
            break;
        }
        //更新temp的值
        temp = temp->next;
    }
    return OK;
}

//O(n)
Status ListInsert_L(LinkList L , int i , ElemType e)
{
    int j;
    //判断无效情况(为空，越界)
    if (L == NULL || i < 1 || i > ListLength_L(L) + 1)
    {
        return ERROR;
    }
    LNode *temp = L;
    LNode *insert;
    for ( j = 1; j <= ListLength_L(L) + 1; j++)
    {
        if (j == i)
        {
            insert = (LNode *)malloc(sizeof(LNode));
            if (insert == NULL)
            {
                return ERROR;
            }
            //后移插入位置元素
            insert->next = temp->next;
            //连接插入前驱元素和插入元素
            temp->next = insert;
            insert->data = e;
            break;
        }
        //更新temp的值
        temp = temp->next;
    }
    return OK;
}

//O(n)
Status ListDelete_L(LinkList L , int i , ElemType *e)
{
    int j;
    LNode *prev = L; //保存当前节点的前驱节点
    //判断无效情况(为空，越界)
    if (L == NULL || i < 1 || i > ListLength_L(L))
    {
        return ERROR;
    }
    LNode *temp = L->next;
    for (j = 1; temp; j++)
    {
        if (j == i)
        {
            *e = temp->data;
            //连接删除节点的前后节点
            prev->next = temp->next;
            free(temp);
            break;
        }
        //更新prev,temp的值
        prev = temp;
        temp = temp->next;
    }
    return OK;
}

//对ElemType提供比较函数
Status Compare(ElemType a, ElemType b)
{
    if (strcmp(a.name, b.name) == 0 && a.num == b.num)
    {
        return True;
    }
    return FALSE;
}

//O(n)
Status PriorElem_L(LinkList L , ElemType e , ElemType *pre_e)
{
    int j;
    LNode *prev = L; //保存当前节点的前驱节点
    LNode *temp = L->next;

    //判断无效情况(为空，只有一个节点)
    if (L == NULL || ListLength_L(L) < 2)
    {
        return ERROR;
    }
    for (j = 1; temp; j++)
    {
        //使用提供的比较函数，查找对应值节点
        if (Compare(temp->data, e))
        {
            *pre_e = prev->data;
            return OK;
        }
        //指向下一个节点
        prev = temp;
        temp = temp->next;
    }
    return ERROR;
}


//O(n)
Status PrintList_L(LinkList L)
{
    if (L == NULL)
    {
        return ERROR;
    }
    //遍历链表的同时打印
    LNode *temp = L->next;
    while (temp)
    {
        printf("%d %s\t", temp->data.num, temp->data.name);
        temp = temp->next;
    }
    printf("\n\n");
    return OK;
}

int main()
{
    int i;
    LinkList temp;
    //测试用例
    ElemType test[] = {{2, "李华"}, {3, "小明"}, {9, "李雷"}, {6, "王明"}};
    ElemType tmp, tmp2;

    //以下为单元测试
    printf("测试InitList_L函数...\n\n");
    InitList_L(&temp);
    EXPECT_EQ_INT(0, ListLength_L(temp));

    printf("测试ListInsert_L和ListPrint函数...\n");
    for ( i = 0; i < 4; i++)
    {
        EXPECT_EQ_INT(OK, ListInsert_L(temp, i + 1, test[i]));
    }
    printf("插入四个样例后的链表为\n");
    PrintList_L(temp);

    printf("测试GetElem_L函数...\n");
    EXPECT_EQ_INT(OK, GetElem_L(temp, 1, &tmp));
    printf("取出的第一个节点的元素为:%d %s\n\n", tmp.num, tmp.name);
    
    printf("测试ListLength函数...\n\n");
    EXPECT_EQ_INT(4, ListLength_L(temp));
    
    printf("测试PriorElem_L函数...\n");
    EXPECT_EQ_INT(OK, GetElem_L(temp, 2, &tmp));
    EXPECT_EQ_INT(OK, PriorElem_L(temp, tmp, &tmp2));
    printf("链表当前内容为\n");
    PrintList_L(temp);
    printf("取出的第二个节点的前驱元素为:%d %s\n\n", tmp2.num, tmp2.name);

    printf("测试ListDelete_L函数...\n\n");
    EXPECT_EQ_INT(OK, ListDelete_L(temp, 3, &tmp));
    printf("删除了第三个节点  %d %s\n删除后的链表内容为:\n", tmp.num, tmp.name);
    PrintList_L(temp);

    
    printf("单元测试:个数%d/%d 通过率(%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    system("pause");
    return 0;
}