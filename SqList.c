//GBK
#include <stdio.h>
#include <stdlib.h>


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


typedef int ElemType;

typedef struct
{
    ElemType *elem;
    int length;
    int listsize;
} SqList;


enum Status
{
    OK = 1,
    TRmeijuUE = 2,
    FALSE = 0,
    ERROR = 0,
    OVERFLOW = -1,

};

typedef enum Status Status;

Status InitList(SqList *L);

Status DestroyList(SqList *L);

Status ClearList(SqList *L);

int LocateElem(SqList L, ElemType e);

Status ListInsert(SqList *L, int i, ElemType e);

Status ListDelete(SqList *L, int i, ElemType *e);

Status ListPrint(SqList *L);

Status ListSort(SqList *L);

//数组有序性测试
int is_sorted(int array[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            return 0;
        }
    }
    return 1;
}

//初始化顺序表
Status InitList(SqList *L)
{

    L->elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));

    if (L->elem == NULL)
    {
        exit(OVERFLOW);
    }

    L->length = 0;
    L->listsize = LIST_INIT_SIZE;

    return OK;
}

//摧毁顺序表
Status DestroyList(SqList *L)
{
    if (L == NULL || L->elem == NULL)
    {
        return ERROR;
    }
    free(L->elem);
    L->elem = NULL;
    L->length = 0;
    L->listsize = 0;

    return OK;
}

//清空顺序表中元素
Status ClearList(SqList *L)
{
    if (L == NULL || L->elem == NULL)
    {
        return ERROR;
    }

    L->length = 0;

    return OK;
}

int ListLength(SqList *L)
{
    return L->length;
}

//在顺序表中查找某元素 e，若找到，则返回它在 L 中第一次出现的位置，
//否则返回 0
int LocateElem(SqList L, ElemType e)
{
    int i = 1;
    ElemType *temp;

    // 确保顺序表结构存在
    if (L.elem == NULL)
    {
        return ERROR;
    }

    temp = L.elem;
    //遍历顺序表同时比较
    while (i <= L.length && e != temp[i - 1])
    {
        i++;
    }

    if (i <= L.length)
    {
        return i;
    }
    return 0;
}

Status ListInsert(SqList *L, int i, ElemType e)
{
    ElemType *newbase;
    ElemType *temp, *insert_pos;

    // 确保顺序表结构存在
    if (L == NULL || L->elem == NULL)
    {
        return ERROR;
    }

    // i值越界
    if (i < 1 || i > L->length + 1)
    {
        return ERROR;
    }

    // 若存储空间已满，则增加新空间
    if (L->length >= L->listsize)
    {
        // 基于现有空间扩容
        newbase = (ElemType *)realloc(L->elem, (L->listsize + LISTINCREMENT) * sizeof(ElemType));
        if (newbase == NULL)
        {
            // 存储内存失败
            exit(OVERFLOW);
        }

        // 新基址
        L->elem = newbase;
        // 存的存储空间
        L->listsize += LISTINCREMENT;
    }

    // insert_pos为插入位置
    insert_pos = (L->elem + i - 1);

    // 右移元素，腾出位置
    for (temp = (L->elem + L->length - 1); temp >= insert_pos; temp--)
    {
        *(temp + 1) = *temp;
    }

    // 插入e
    *insert_pos = e;

    // 表长增1
    L->length++;

    return OK;
}


//在表 L 中删除位序为 i 的数据元素，并将删除的元素赋值给 e，成
//功返回 Ok；否则，返回 Error
Status ListDelete(SqList *L, int i, ElemType *e)
{
    ElemType *temp;

    if (L == NULL || L->elem == NULL)
    {
        return ERROR;
    }

    if (i < 1 || i > L->length)
    {
        return ERROR;
    }

    *e = L->elem[i - 1];

    temp = (L->elem + i - 1);

    for (; temp < (L->elem + L->length - 1); temp++)
    {
        *temp = *(temp + 1);
    }

    //表长减1
    L->length--;

    return OK;
}

//打印顺序表
Status ListPrint(SqList *L)
{
    int i;
    if (!L)
    {
        return ERROR;
    }
    printf("\n");
    for (i = 0; i < L->length; i++)
    {
        printf("%d ", L->elem[i]);
    }
    printf("\n");
    return OK;
}

//快排辅助函数
void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

//快排
void Qsort(int nums[], int low, int high)
{
    int i = low, j = high;
    if (low >= high)
        return;
    int pivot = nums[rand() % (high - low + 1) + low];
    do
    {
        while (nums[i] < pivot)
        {
            i++;
        }
        while (nums[j] > pivot)
        {
            j--;
        }
        if (i <= j)
        {
            swap(nums + i, nums + j);
            i++;
            j--;
        }

    } while (i <= j);
    Qsort(nums, i, high);
    Qsort(nums, low, j);
}

//顺序表排序
Status ListSort(SqList *L)
{
    if (L == NULL || L->elem == NULL || L->length < 1)
    {
        return ERROR;
    }

    Qsort(L->elem, 0, L->length - 1);
    return OK;
}

//归并 La 和 Lb 得到新的顺序表 Lc
void MergeSqList(SqList La, SqList Lb, SqList *Lc)
{
    ElemType *ptr_a, *ptr_b, *ptr_c;
    ElemType *ptr_a_last, *pb_last;

    ptr_a = La.elem; // 指向La第一个元素
    ptr_b = Lb.elem; // 指向Lb第一个元素

    Lc->listsize = Lc->length = La.length + Lb.length;
    ptr_c = Lc->elem = (ElemType *)malloc(Lc->listsize * sizeof(ElemType));
    if (ptr_c == NULL)
    {
        exit(OVERFLOW);
    }

    ptr_a_last = La.elem + La.length - 1; // 指向La最后一个元素
    pb_last = Lb.elem + Lb.length - 1;    // 指向Lb最后一个元素

    //拷贝操作
    while (ptr_a <= ptr_a_last)
    {
        *ptr_c++ = *ptr_a++;
    }
    while (ptr_b <= pb_last)
    {
        *ptr_c++ = *ptr_b++;
    }
    ListSort(Lc);
}

//求顺序表的并集
void union_sq(SqList *La, SqList Lb)
{
    int La_len, Lb_len;
    int i;
    ElemType e;

    // 求顺序表长度
    La_len = ListLength(La);
    Lb_len = ListLength(&Lb);

    for (i = 1; i <= Lb_len; i++)
    {
        // 取Lb中第i个元素赋给e
        e = Lb.elem[i - 1];

        // 若e不在La中则插入
        if (!LocateElem(*La, e))
        {
            La_len++;
            ListInsert(La, La_len, e);
        }
    }
}

int main()
{
    SqList temp;
    SqList La, Lb, Lc;
    ElemType e;
    int i;
    int a[] = {6, 5, 41, 22, 2, 1};
    int b[] = {12, 10, 8, 3, 4, 2};

    //以下为单元测试
    printf("测试InitList函数...\n");
    InitList(&temp);
    EXPECT_EQ_INT(LIST_INIT_SIZE, temp.listsize);

    printf("测试ListInsert函数...\n");
    EXPECT_EQ_INT(OK, ListInsert(&temp, 1, 2));

    printf("测试LocateElem函数...\n");
    EXPECT_EQ_INT(1, LocateElem(temp, 2));

    printf("测试ClearList函数...\n");
    ClearList(&temp);
    EXPECT_EQ_INT(0, temp.length);

    printf("测试ListLength函数...\n");
    EXPECT_EQ_INT(0, ListLength(&temp));
    
    printf("测试LocateElem函数...\n");
    for (i = 0; i < 11; i++) 
    {
        ListInsert(&temp, 1, i);
    }
    EXPECT_EQ_INT(0, LocateElem(temp, 11));

    printf("测试ListInsert函数...\n");
    EXPECT_EQ_INT(ERROR, ListInsert(&temp, ListLength(&temp) + 2, -1));

    printf("测试ListPrint函数...\n");
    ListPrint(&temp);

    printf("单元测试:个数%d/%d 通过率(%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);

    InitList(&La);
    InitList(&Lb);

    for (i = 0; i < 6; i++) 
    {
        ListInsert(&La, i + 1, a[i]);
        ListInsert(&Lb, i + 1, b[i]);
    }
    ListSort(&La);
    ListSort(&Lb);
    
    printf("测试ListSort函数...\n");
    EXPECT_EQ_INT(1, is_sorted(La.elem, La.length));
    EXPECT_EQ_INT(1, is_sorted(Lb.elem, Lb.length));

    printf("测试MergeSqList函数...\n");
    MergeSqList(La, Lb, &Lc);
    EXPECT_EQ_INT(La.length + Lb.length, Lc.length);

    printf("测试union_sq函数...\n");
    printf("La, Lb调用union_sq函数前:\n");
    ListPrint(&La);
    ListPrint(&Lb);
    union_sq(&La, Lb);
    printf("La, Lb调用union_sq函数后:\n");
    ListSort(&La);
    ListPrint(&La);
    ListPrint(&Lb);

    printf("单元测试:个数%d/%d 通过率(%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    system("pause");
    return 0;
}