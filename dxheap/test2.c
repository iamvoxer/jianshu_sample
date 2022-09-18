#include <stdio.h>
////////////////////////////////////////////////////////////////////////
// TLV格式，1个字节的标识tag+4个字节的长度+长度个字节的数据区
#define dxheap_all_size 102400

static unsigned char dxheap_all[dxheap_all_size];
int dxheap_char2int(int i)
{
    return (dxheap_all[i + 1] << 24) | (dxheap_all[i + 2] << 16) | (dxheap_all[i + 3] << 8) | dxheap_all[i + 4];
}
void dxheap_int2char(int length, int index)
{
    dxheap_all[index + 1] = (length >> 24) & 0xFF;
    dxheap_all[index + 2] = (length >> 16) & 0xFF;
    dxheap_all[index + 3] = (length >> 8) & 0xFF;
    dxheap_all[index + 4] = length & 0xFF;
}
void* dxheap_alloc(int size)
{
    void* result;
    int i;
    for (i = 0; i < dxheap_all_size - 5; i++)
    {
        //先从前5个字节获取header的tag和length
        char tag = dxheap_all[i];
        int length = dxheap_char2int(i);

        if (tag != '0' || size > length - 5)
        {
            //假如tag不为0表示不为空，或者这一块不够大，则跳过到下一块
            i = i + 5 + length;
            // for循环会自动加1，所以提前减1
            i = i - 1;
            continue;
        }
        dxheap_all[i] = '1';
        //长度调整为size
        dxheap_int2char(size, i);
        //返回的地址需要跳过header
        result = dxheap_all + i + 5;
        //把剩余的空间标记为空
        i = i + size + 5;
        dxheap_all[i] = '0';
        dxheap_int2char(length - size - 5, i);
        //再利用free去合并空闲的区块
        dxheap_free(&dxheap_all[i + 5]);
        return result;
    }
}

void dxheap_free(void* ptr)
{
    //先判断ptr是否是整个数组区域内的指针
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size];
    if (ptr < min || ptr > max)
    {
        return;
    }
    int index = ptr - min;
    //往前5个是header的tag
    dxheap_all[index - 5] = '0';
    //往前4个是header的length
    int length = dxheap_char2int(index - 5);
    memset(ptr, '\0', length);
    //往后length个是下一个块头
    int nexindex = index + length;
    if (ptr > max)
    {
        return;
    }
    if (dxheap_all[nexindex] == '0')
    {
        //如果下一个快也为空，则合并到上一个快
        int nextlength = dxheap_char2int(nexindex);
        length = length + nextlength + 5;
        dxheap_int2char(length, index - 5);
        memset(ptr, '\0', length);
    }
}
void dxheap_init()
{
    //初始化，把整个数组标记为可用，长度需要减少5（header的长度）
    int n = dxheap_all_size - 5;
    dxheap_all[0] = '0';
    dxheap_int2char(n, 0);
    memset(dxheap_all + 5, '\0', n);
}

void* dxheap_realloc(void* ptr, int size)
{
    if (!ptr)
    {
        // ptr为空表示直接alloc一块内存
        return dxheap_alloc(size);
    }
    //先判断ptr是否是整个数组区域内的指针,不是的话直接alloc
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size];
    if (ptr < min || ptr > max)
    {
        return dxheap_alloc(size);
    }
    //申请一块足够大的新快，把内容复制过去
    int index = ptr - min;
    int length = dxheap_char2int(index - 5);
    //有可能length比size大,这里不用math.min
    int minLength = length;
    if (length > size)
    {
        minLength = size;
    }
    void* newBlock = dxheap_alloc(size);
    memcpy(newBlock, ptr, minLength);
    //最后释放旧的区域
    dxheap_free(ptr);
    return newBlock;
}
//////////////////////////////////////////////////////////////////////////////////////

void hexdump(const unsigned char* buf, const int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\n");
    return;
}
void printall()
{
    hexdump(dxheap_all, 60);
    int i;
    int j = 0;
    for (i = 0; i < dxheap_all_size - 5; i++)
    {
        char tag = dxheap_all[i];
        int length = dxheap_char2int(i);
        printf("第%d块,是否空闲%d,长度为%d\n", j, tag == '0', length);
        j++;
        i = i + 5 + length - 1;
    }
}
int main(int argc, char* argv[])
{
    int i = -100;

    printf("xxxxxxxxxxxx_%d\n", i);
    // if (strlen(dxheap_all) == 0)
    // {
    //     dxheap_init();
    // }
    // int min = &dxheap_all[0];
    // int max = &dxheap_all[dxheap_all_size];
    // printall();
    // void *t1 = dxheap_realloc(NULL, 10);
    // printall();
    // void *t2 = dxheap_realloc(NULL, 20);
    // printall();
    // void *t3 = dxheap_realloc(NULL, 3);
    // printall();
    // printf("%d,%d,%d,%d,%d\n", min, max, t1, t2, t3);
    // dxheap_free(t2);
    // printall();
    // void *t4 = dxheap_realloc(NULL, 4);
    // printall();
    // void *t5 = dxheap_realloc(t4, 5);
    // printall();
    // dxheap_free(t5);
    // printall();
    // dxheap_free(t3);
    // printall();
}