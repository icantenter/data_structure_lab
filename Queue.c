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

enum Status //状态类型
{
    OK = 1,
    TRUE = 2,
    FALSE = 0,
    ERROR = -111,
    OVERFLOW = -1,

};

typedef enum Status Status;

typedef int QElemType; //元素类型
typedef struct
{
    QElemType *base; //动态存储空间首址
    int front;       //头下标，队头元素下标
    int rear;        //尾下标，队尾元素的下一个位置
    size_t qsize;    //队列占用的空间（以元素为单位）
} SqQueue;

Status QueueEmpty(SqQueue queue);
Status InitQueue(SqQueue *queue, size_t size)
{

    if (queue == NULL)
    {
        return ERROR;
    }
    //为队列中的数组分配空间
    queue->base = (QElemType *)malloc(sizeof(QElemType) * (size + 1));
    if (queue->base == NULL)
    {
        return OVERFLOW;
    }

    //初始化front, rear, qsize属性
    queue->front = 0;
    queue->rear = 0;
    queue->qsize = size;
    return OK;
}

Status EnQueue(SqQueue *queue, QElemType e)
{

    if (queue == NULL)
    {
        return ERROR;
    }
    int flag = (queue->rear + 1) % (queue->qsize + 1);

    if (flag == queue->front)
    {
        return ERROR;
    }
    queue->base[queue->rear] = e;
    queue->rear = flag;
    return OK;
}

Status Dequeue(SqQueue *queue, QElemType *e)
{
    if (queue == NULL)
    {
        return ERROR;
    }

    if (QueueEmpty(*queue))
    {
        return ERROR;
    }
    *e = queue->base[queue->front];
    queue->front = (queue->front + 1) % (queue->qsize + 1);
    return OK;
}

Status QueueEmpty(SqQueue queue)
{
    if (queue.front == queue.rear)
    {
        return TRUE;
    }
    return FALSE;
}

QElemType GetHead(SqQueue queue)
{
    if (!QueueEmpty(queue))
    {
        return queue.base[queue.front];
    }
}

int QueueLength(SqQueue queue)
{
    if (queue.base == NULL)
    {
        return 0;
    }

    // 队列长度
    return (queue.rear - queue.front + queue.qsize + 1) % (queue.qsize + 1);
}

Status Pop(SqQueue *queue, QElemType *e)
{

    if (queue == NULL)
    {
        return ERROR;
    }
    if (QueueEmpty(*queue))
    {
        return ERROR;
    }
    //弹出队列中元素
    int tmp = (queue->rear - 1 + queue->qsize + 1) % (queue->qsize + 1);
    *e = queue->base[tmp];
    //删除元素
    queue->rear = tmp;
    return OK;
}

Status Insert(SqQueue *queue, QElemType e)
{
    if (queue == NULL)
    {
        return ERROR;
    }
    if ((queue->rear + 1) % (queue->qsize + 1) == queue->front)
    {
        return ERROR;
    }
    //插入的位置
    queue->front = (queue->front - 1 + queue->qsize + 1) % (queue->qsize + 1);
    //添加元素
    queue->base[queue->front] = e;

    return OK;
}

Status DeleteQueue(SqQueue *queue)
{
    if (queue == NULL)
    {
        return ERROR;
    }
    free(queue->base);
    return OK;
}

int main()
{
    SqQueue queue;
    int i, x, n, m, start;

    InitQueue(&queue, 10);
    for (i = 0; i < 10; i++)
    {
        EnQueue(&queue, i);
    }
    EXPECT_EQ_INT(10, QueueLength(queue));
    EXPECT_EQ_INT(ERROR, EnQueue(&queue, -1));
    Dequeue(&queue, &x);
    Insert(&queue, -1);
    Dequeue(&queue, &x);
    EXPECT_EQ_INT(-1, x);
    Pop(&queue, &x);
    EXPECT_EQ_INT(9, x);
    EXPECT_EQ_INT(1, GetHead(queue));
    for (i = 0; i < 9; i++)
    {
        Dequeue(&queue, &x);
    }
    EXPECT_EQ_INT(TRUE, QueueEmpty(queue));

    int test[] = {1, 2, 3};
    for (i = 0; i < sizeof(test) / sizeof(int); i++)
    {
        Insert(&queue, test[i]);
    }

    for (i = 0; i < sizeof(test) / sizeof(int); i++)
    {
        Pop(&queue, &x);
        printf("%d ", x);
    }
    putchar('\n');

    DeleteQueue(&queue);
    scanf("%d%d%d", &n, &m, &start);
    InitQueue(&queue, n);
    for ( i = 0; i < n; i++)
    {
        EnQueue(&queue, i + 1);
    }
    
    while (!QueueEmpty(queue))
    {
        i = start;
        while (i++ != m)
        {
            Dequeue(&queue, &x);
            EnQueue(&queue, x);
        }
        Dequeue(&queue, &x);
        printf("%d ", x);
    }

    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}