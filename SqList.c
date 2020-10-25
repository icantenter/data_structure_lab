//GBK
#include <stdio.h>
#include <stdlib.h>


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

//���������Բ���
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

//��ʼ��˳���
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

//�ݻ�˳���
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

//���˳�����Ԫ��
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

//��˳����в���ĳԪ�� e�����ҵ����򷵻����� L �е�һ�γ��ֵ�λ�ã�
//���򷵻� 0
int LocateElem(SqList L, ElemType e)
{
    int i = 1;
    ElemType *temp;

    // ȷ��˳���ṹ����
    if (L.elem == NULL)
    {
        return ERROR;
    }

    temp = L.elem;
    //����˳���ͬʱ�Ƚ�
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

    // ȷ��˳���ṹ����
    if (L == NULL || L->elem == NULL)
    {
        return ERROR;
    }

    // iֵԽ��
    if (i < 1 || i > L->length + 1)
    {
        return ERROR;
    }

    // ���洢�ռ��������������¿ռ�
    if (L->length >= L->listsize)
    {
        // �������пռ�����
        newbase = (ElemType *)realloc(L->elem, (L->listsize + LISTINCREMENT) * sizeof(ElemType));
        if (newbase == NULL)
        {
            // �洢�ڴ�ʧ��
            exit(OVERFLOW);
        }

        // �»�ַ
        L->elem = newbase;
        // ��Ĵ洢�ռ�
        L->listsize += LISTINCREMENT;
    }

    // insert_posΪ����λ��
    insert_pos = (L->elem + i - 1);

    // ����Ԫ�أ��ڳ�λ��
    for (temp = (L->elem + L->length - 1); temp >= insert_pos; temp--)
    {
        *(temp + 1) = *temp;
    }

    // ����e
    *insert_pos = e;

    // ����1
    L->length++;

    return OK;
}


//�ڱ� L ��ɾ��λ��Ϊ i ������Ԫ�أ�����ɾ����Ԫ�ظ�ֵ�� e����
//������ Ok�����򣬷��� Error
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

    //����1
    L->length--;

    return OK;
}

//��ӡ˳���
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

//���Ÿ�������
void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

//����
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

//˳�������
Status ListSort(SqList *L)
{
    if (L == NULL || L->elem == NULL || L->length < 1)
    {
        return ERROR;
    }

    Qsort(L->elem, 0, L->length - 1);
    return OK;
}

//�鲢 La �� Lb �õ��µ�˳��� Lc
void MergeSqList(SqList La, SqList Lb, SqList *Lc)
{
    ElemType *ptr_a, *ptr_b, *ptr_c;
    ElemType *ptr_a_last, *pb_last;

    ptr_a = La.elem; // ָ��La��һ��Ԫ��
    ptr_b = Lb.elem; // ָ��Lb��һ��Ԫ��

    Lc->listsize = Lc->length = La.length + Lb.length;
    ptr_c = Lc->elem = (ElemType *)malloc(Lc->listsize * sizeof(ElemType));
    if (ptr_c == NULL)
    {
        exit(OVERFLOW);
    }

    ptr_a_last = La.elem + La.length - 1; // ָ��La���һ��Ԫ��
    pb_last = Lb.elem + Lb.length - 1;    // ָ��Lb���һ��Ԫ��

    //��������
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

//��˳���Ĳ���
void union_sq(SqList *La, SqList Lb)
{
    int La_len, Lb_len;
    int i;
    ElemType e;

    // ��˳�����
    La_len = ListLength(La);
    Lb_len = ListLength(&Lb);

    for (i = 1; i <= Lb_len; i++)
    {
        // ȡLb�е�i��Ԫ�ظ���e
        e = Lb.elem[i - 1];

        // ��e����La�������
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

    //����Ϊ��Ԫ����
    printf("����InitList����...\n");
    InitList(&temp);
    EXPECT_EQ_INT(LIST_INIT_SIZE, temp.listsize);

    printf("����ListInsert����...\n");
    EXPECT_EQ_INT(OK, ListInsert(&temp, 1, 2));

    printf("����LocateElem����...\n");
    EXPECT_EQ_INT(1, LocateElem(temp, 2));

    printf("����ClearList����...\n");
    ClearList(&temp);
    EXPECT_EQ_INT(0, temp.length);

    printf("����ListLength����...\n");
    EXPECT_EQ_INT(0, ListLength(&temp));
    
    printf("����LocateElem����...\n");
    for (i = 0; i < 11; i++) 
    {
        ListInsert(&temp, 1, i);
    }
    EXPECT_EQ_INT(0, LocateElem(temp, 11));

    printf("����ListInsert����...\n");
    EXPECT_EQ_INT(ERROR, ListInsert(&temp, ListLength(&temp) + 2, -1));

    printf("����ListPrint����...\n");
    ListPrint(&temp);

    printf("��Ԫ����:����%d/%d ͨ����(%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);

    InitList(&La);
    InitList(&Lb);

    for (i = 0; i < 6; i++) 
    {
        ListInsert(&La, i + 1, a[i]);
        ListInsert(&Lb, i + 1, b[i]);
    }
    ListSort(&La);
    ListSort(&Lb);
    
    printf("����ListSort����...\n");
    EXPECT_EQ_INT(1, is_sorted(La.elem, La.length));
    EXPECT_EQ_INT(1, is_sorted(Lb.elem, Lb.length));

    printf("����MergeSqList����...\n");
    MergeSqList(La, Lb, &Lc);
    EXPECT_EQ_INT(La.length + Lb.length, Lc.length);

    printf("����union_sq����...\n");
    printf("La, Lb����union_sq����ǰ:\n");
    ListPrint(&La);
    ListPrint(&Lb);
    union_sq(&La, Lb);
    printf("La, Lb����union_sq������:\n");
    ListSort(&La);
    ListPrint(&La);
    ListPrint(&Lb);

    printf("��Ԫ����:����%d/%d ͨ����(%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    system("pause");
    return 0;
}