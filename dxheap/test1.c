#include <stdio.h>
////////////////////////////////////////////////////////////////////////
// 固定大小格式，每块占用4个字节（特定的单片机只支持这种方式
// 版本 20220913
//申请的内存区块个数
#define dxheap_block_count 64
//总共占用的字节数需要*4
static unsigned char dxheap[dxheap_block_count * 4];
//需要用count/8个字节来标识每个区块占用还是空闲，1个bit来表示一个区块的情况，0表示空闲，1表示占用
static unsigned char dxheap_empty_flags[dxheap_block_count / 8];

/**
 * @brief 内存块标识占用还是空闲,把一段连续的块标记成占用还是空闲
 *
 * @param tag 0表示空闲，1表示占用
 * @param start_block_index 开始的块序号，从0开始
 * @param count 连续块的个数
 */
void dxheap_flag_write(int tag, int start_block_index, int count)
{
    int index;
    for (index = start_block_index; index < start_block_index + count; index++)
    {
        if (tag)
        {
            dxheap_empty_flags[index / 8] = dxheap_empty_flags[index / 8] | (1 << (index % 8));
        }
        else
        {
            dxheap_empty_flags[index / 8] = dxheap_empty_flags[index / 8] &= ~(1 << (index % 8));
        }
    }
}
//判断第index个块是否被占用 0表示空闲，1表示占用
int dxheap_flag_read(int index)
{
    char c = dxheap_empty_flags[index / 8];
    int pos = index % 8;
    return (c & (1 << pos)) >> pos;
}
//判断从第index个块开始，连续count个块都为空则返回0，否则1
int dxheap_has_count_empty(int index, int count)
{
    int notfound = 0;
    int i = index;
    while (notfound == 0 && i < dxheap_block_count && i <= (index + count))
    {
        notfound = notfound + dxheap_flag_read(i);
        i++;
    }
    return notfound == 0;
}
void* dxheap_alloc(int size)
{
    //先计算需要的块数量
    int count;
    if (size % 4 == 0)
    {
        count = size / 4;
    }
    else
    {
        //内存可能浪费
        count = (size / 4) + 1;
    }
    if (count > dxheap_block_count)
    {
        printf("not enough memory found\n");
        return -1;
    }
    void* result;
    int i = 0;
    while (i < dxheap_block_count)
    {
        int flag = dxheap_flag_read(i);
        if (flag)
        {
            //假如不为空,则跳过到下一块
            i++;
            continue;
        }
        //判断是否能找到count个空闲块
        int isfound = dxheap_has_count_empty(i, count);
        if (isfound)
        {
            dxheap_flag_write(1, i, count);
            return dxheap + i * 4;
        }
        else
        {
            i++;
            continue;
        }
    }
    printf("not enough memory found\n");
    return -1;
}

void dxheap_free(void* ptr, int osize)
{
    //先判断ptr是否是整个数组区域内的指针
    int min = &dxheap[0];
    int max = &dxheap[dxheap_block_count * 4 - 1];
    if (ptr < min || ptr > max)
    {
        return;
    }
    //先算出ptr对应的指针指向第几块,这里index必然是可以整除4的，是块的首地址。除非乱传一个地址过来
    int index = ptr - min;
    index = index / 4;
    //后面osize个字节对应的块也需要置空
    int count = osize / 4;
    if (osize % 4 != 0)
    {
        count++;
    }
    dxheap_flag_write(0, index, count);
    memset(ptr, 0x00, count * 4);
}

void dxheap_init()
{
    //初始化，把整个数组标记为可用，
    memset(dxheap, 0x00, dxheap_block_count * 4);
}

void* dxheap_realloc(void* ptr, int osize, int size)
{
    if (!ptr)
    {
        // ptr为空表示直接alloc一块内存
        void* block = dxheap_alloc(size);
        return block;
    }
    //先判断ptr是否是整个数组区域内的指针,不是的话直接alloc
    int min = &dxheap[0];
    int max = &dxheap[dxheap_block_count * 4 - 1];
    if (ptr < min || ptr > max)
    {
        void* block = dxheap_alloc(size);
        return block;
    }
    //申请一块足够大的新快，把内容复制过去
    int index = ptr - min;
    //有可能osize比size大,这里不用math.min
    int minLength = osize;
    if (osize > size)
    {
        minLength = size;
    }
    void* newBlock = dxheap_alloc(size);
    if (newBlock == -1)
    {
        return newBlock;
    }
    memcpy(newBlock, ptr, minLength);
    //最后释放旧的区域
    dxheap_free(ptr, osize);
    return newBlock;
}

// //////////////////////////////////////////////////////////////////////////////////////

// void hexdump(const unsigned char *buf, const int num)
// {
//     int i;
//     for (i = 0; i < num; i++)
//     {
//         printf("%02X ", buf[i]);
//     }
//     printf("\n");
//     return;
// }
// void printdetail(i, count, length)
// {
//     printf("\n");
//     for (i = 0; i < dxheap_size; i++)
//     {
//         print_char(dxheap[i]);
//     }
// }
void printall()
{
    int i;
    for (i = 0; i < dxheap_block_count / 8; i++)
    {
        print_char(dxheap_empty_flags[i]);
    }
    printf("\n");
}
// void print_bin(int number)
// {
//     int bit = sizeof(int) * 8;
//     int i;
//     for (i = bit - 1; i >= 0; i--)
//     {
//         int bin = (number & (1 << i)) >> i;
//         printf("%d", bin);
//         if (i % 8 == 0)
//         {
//             printf(" ");
//         }
//     }
//     printf("\n");
// }
void print_char(unsigned char number)
{
    int bit = sizeof(unsigned char) * 8;
    int i;
    for (i = 0; i < bit; i++)
    {
        int bin = (number & (1 << i)) >> i;
        printf("%d", bin);
        if (i != 0 && i % 7 == 0)
        {
            printf(" ");
        }
    }
}
void test1()
{
    if (strlen(dxheap) == 0)
    {
        dxheap_init();
    }
    printall();
    printf("-------------------------------申请100字节---------------------------\n");
    void* t1 = dxheap_realloc(NULL, 0, 100);
    memset(t1, 'a', 100);
    printall();
    printf("-------------------------------申请64字节---------------------------\n");
    void* t2 = dxheap_realloc(NULL, 0, 64);
    memset(t2, 'b', 64);
    printall();
    printf("-------------------------------申请032字节---------------------------\n");
    void* t3 = dxheap_realloc(NULL, 0, 32);
    memset(t3, 'c', 32);
    printall();
    printf("-------------------------------释放64字节---------------------------\n");
    dxheap_free(t2, 64);
    printall();
    printf("-------------------------------申请040字节---------------------------\n");
    void* t4 = dxheap_realloc(NULL, 0, 40);
    printall();
    printf("-------------------------------申请050字节(基于以前的40字节扩展)--------\n");
    void* t5 = dxheap_realloc(t4, 40, 50);
    memset(t5, 'c', 50);
    printall();
    printf("-------------------------------释放050字节---------------------------\n");
    dxheap_free(t5, 50);
    printall();
    printf("-------------------------------释放032字节---------------------------\n");
    dxheap_free(t3, 32);
    printall();
    // printf("-------------------------------申请512字节---------------------------\n");
    // void *t6 = dxheap_realloc(NULL, 0, 512);
    // printall();
    // printf("-------------------------------申请250字节---------------------------\n");
    // dxheap_realloc(NULL, 0, 250);
    // printall();
    // printf("-------------------------------申请512字节---------------------------\n");
    // dxheap_realloc(NULL, 0, 512);
    // printall();
    // printf("-------------------------------申请30000字节---------------------------\n");
    // dxheap_realloc(NULL, 0, 30000);
    // printall();
}
void test2()
{

    int i;
    for (i = 0; i < 8; i++)
    {
        print_char(dxheap_empty_flags[i]);
    }
    printf("\n");
    dxheap_flag_write(1, 20, 40);

    for (i = 0; i < 8; i++)
    {
        print_char(dxheap_empty_flags[i]);
    }
    printf("\n");
    dxheap_flag_write(0, 22, 5);
    for (i = 0; i < 8; i++)
    {
        print_char(dxheap_empty_flags[i]);
    }

    printf("\n");
    printf("第25=%d\n", dxheap_flag_read(25));
    printf("第29=%d\n", dxheap_flag_read(29));
}
void test3()
{
    if (strlen(dxheap) == 0)
    {
        dxheap_init();
    }
    printall();
    void* a1 = dxheap_realloc(NULL, 0, 10);
    memset(a1, 0x00, 10);
    printall();
    void* a2 = dxheap_realloc(NULL, 0, 64);
    memset(a2, 0x00, 64);
    printall();
}
void test4()
{
    if (strlen(dxheap) == 0)
    {
        dxheap_init();
    }
    printall();
    printf("-------------------------------申请198字节---------------------------\n");
    void* t1 = dxheap_realloc(NULL, 0, 198);
    memset(t1, 'a', 198);
    printall();
    printf("-------------------------------申请80字节---------------------------\n");
    void* t2 = dxheap_realloc(NULL, 0, 80);
    memset(t2, 'b', 80);
    printall();
    printf("-------------------------------申请032字节---------------------------\n");
    void* t3 = dxheap_realloc(NULL, 0, 32);
    memset(t3, 'c', 32);
    printall();
    printf("-------------------------------申请016字节---------------------------\n");
    void* t4 = dxheap_realloc(NULL, 0, 16);
    printall();
    printf("-------------------------------申请32字节--------\n");
    void* t5 = dxheap_realloc(NULL, 0, 32);
    memset(t5, 'c', 32);
    printall();

    printf("-------------------------------申请34字节--------\n");
    void* t6 = dxheap_realloc(NULL, 0, 34);
    memset(t6, 'c', 34);
    printall();

    printf("-------------------------------申请24字节--------\n");
    void* t7 = dxheap_realloc(NULL, 0, 24);
    memset(t7, 'c', 24);
    printall();

    printf("-------------------------------申请27字节--------\n");
    void* t8 = dxheap_realloc(NULL, 0, 27);
    memset(t8, 'c', 27);
    printall();
}
int main(int argc, char* argv[])
{
    // test1();
    // test3();
    test4();
}