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
//��Ԫ���Ժ궨��
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
 int num; //ר�Һţ����ظ��������ڲ���ר��
 char name[30]; //ר�������������ظ�
}ElemType; //Ԫ������-----ר����Ϣ��������
typedef ElemType ET; 
typedef struct Lnode
{
 ElemType data;
 struct Lnode *next;
}LNode, *LinkList; //���������Ͷ���


enum Status
{
    OK = 1,
    TRUE = 2,
    FALSE = 0,
    ERROR = 0,
    OVERFLOW = -1,

};
//ö������
typedef enum Status Status;


//L�Ƕ���ָ��,�����޸�main�����е�ָ�����
Status InitList_L(LinkList *L)
{
    if (L == NULL)
    {
        return ERROR;
    }
    //���ٿռ�
    *L = (LNode *)malloc(sizeof(LNode));
    //�����Ƿ�ɹ�
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
    //�ü�����counter,��ʱʵ��ListLength
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
    //�ж���Ч���(Ϊ�գ�Խ��)
    if (L == NULL || i < 1 || i > ListLength_L(L) || e == NULL)
    {
        return ERROR;
    }
    LNode *temp = L->next;
    //j��Ϊ���������ҵ���Ӧλ��Ԫ��
    for (j = 1; temp; j++)
    {
        if (j == i)
        {
            *e = temp->data;
            break;
        }
        //����temp��ֵ
        temp = temp->next;
    }
    return OK;
}

//O(n)
Status ListInsert_L(LinkList L , int i , ElemType e)
{
    int j;
    //�ж���Ч���(Ϊ�գ�Խ��)
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
            //���Ʋ���λ��Ԫ��
            insert->next = temp->next;
            //���Ӳ���ǰ��Ԫ�غͲ���Ԫ��
            temp->next = insert;
            insert->data = e;
            break;
        }
        //����temp��ֵ
        temp = temp->next;
    }
    return OK;
}

//O(n)
Status ListDelete_L(LinkList L , int i , ElemType *e)
{
    int j;
    LNode *prev = L; //���浱ǰ�ڵ��ǰ���ڵ�
    //�ж���Ч���(Ϊ�գ�Խ��)
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
            //����ɾ���ڵ��ǰ��ڵ�
            prev->next = temp->next;
            free(temp);
            break;
        }
        //����prev,temp��ֵ
        prev = temp;
        temp = temp->next;
    }
    return OK;
}

//��ElemType�ṩ�ȽϺ���
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
    LNode *prev = L; //���浱ǰ�ڵ��ǰ���ڵ�
    LNode *temp = L->next;

    //�ж���Ч���(Ϊ�գ�ֻ��һ���ڵ�)
    if (L == NULL || ListLength_L(L) < 2)
    {
        return ERROR;
    }
    for (j = 1; temp; j++)
    {
        //ʹ���ṩ�ıȽϺ��������Ҷ�Ӧֵ�ڵ�
        if (Compare(temp->data, e))
        {
            *pre_e = prev->data;
            return OK;
        }
        //ָ����һ���ڵ�
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
    //���������ͬʱ��ӡ
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
    //��������
    ElemType test[] = {{2, "�"}, {3, "С��"}, {9, "����"}, {6, "����"}};
    ElemType tmp, tmp2;

    //����Ϊ��Ԫ����
    printf("����InitList_L����...\n\n");
    InitList_L(&temp);
    EXPECT_EQ_INT(0, ListLength_L(temp));

    printf("����ListInsert_L��ListPrint����...\n");
    for ( i = 0; i < 4; i++)
    {
        EXPECT_EQ_INT(OK, ListInsert_L(temp, i + 1, test[i]));
    }
    printf("�����ĸ������������Ϊ\n");
    PrintList_L(temp);

    printf("����GetElem_L����...\n");
    EXPECT_EQ_INT(OK, GetElem_L(temp, 1, &tmp));
    printf("ȡ���ĵ�һ���ڵ��Ԫ��Ϊ:%d %s\n\n", tmp.num, tmp.name);
    
    printf("����ListLength����...\n\n");
    EXPECT_EQ_INT(4, ListLength_L(temp));
    
    printf("����PriorElem_L����...\n");
    EXPECT_EQ_INT(OK, GetElem_L(temp, 2, &tmp));
    EXPECT_EQ_INT(OK, PriorElem_L(temp, tmp, &tmp2));
    printf("����ǰ����Ϊ\n");
    PrintList_L(temp);
    printf("ȡ���ĵڶ����ڵ��ǰ��Ԫ��Ϊ:%d %s\n\n", tmp2.num, tmp2.name);

    printf("����ListDelete_L����...\n\n");
    EXPECT_EQ_INT(OK, ListDelete_L(temp, 3, &tmp));
    printf("ɾ���˵������ڵ�  %d %s\nɾ�������������Ϊ:\n", tmp.num, tmp.name);
    PrintList_L(temp);

    
    printf("��Ԫ����:����%d/%d ͨ����(%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    system("pause");
    return 0;
}