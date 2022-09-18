#include <stdio.h>
////////////////////////////////////////////////////////////////////////
// TLV格式，1个字节的标识tag+可变字节的长度+长度个字节的数据区
#define dxheap_all_size 3000
static unsigned char dxheap_all[dxheap_all_size];

typedef struct
{
    // 0表示空闲，1表示占用
    int tag;
    //数据域的长度
    int length;
    //长度占用字节的个数，1个、2个，最多3个
    int count;
} dxheap_header;
int dxheap_header_encode(int tag, int length, int h_or_d, int index)
{
    // 00000100 10111001 01100000  以309600为例
    // 00010010 11100101 10000000  左移2位
    int b = length << 2;
    // 00010010 11100101 00100000  最后一个字节右移动2位
    unsigned char intchars[3] = { 0x00, 0x00, 0x00 };
    intchars[0] = (b >> 16) & 0xFF;
    intchars[1] = (b >> 8) & 0xFF;
    intchars[2] = (b) & 0xFF;
    intchars[2] = (intchars[2] >> 2);
    // 00100101 11001010 00100000  前2个字节转换成int然后再左移1位
    int c = ((intchars[0] << 8) | intchars[1]) << 1;
    intchars[0] = (c >> 8) & 0xFF;
    intchars[1] = (c) & 0xFF;
    // 00100101 01100101 00100000  中间这个字节右移动1位
    intchars[1] = (intchars[1] >> 1);

    int count = dxheap_get_rowcount(length);
    //第一个字节的左边第一位tag表示状态，0表示空闲，1表示占用
    if (tag)
    {
        intchars[2] = intchars[2] | 0x80; // 10000000
    }
    else
    {
        intchars[2] = intchars[2] & 0x7F; // 01111111
    }

    // 第一个字节左边第二位表示是否还需要用到下一个字节，为0表示不需要，为1表示需要
    // 第二个字节的左边第一位表示是否还需要用到下一个字节，为0表示不需要，为1表示需要
    if (count == 3)
    {
        intchars[2] = intchars[2] | 0x40; // 01000000
        intchars[1] = intchars[1] | 0x80; // 10000000
    }
    if (count == 2)
    {
        intchars[2] = intchars[2] | 0x40; // 01000000
        intchars[1] = intchars[1] & 0x7F; // 01111111
    }
    if (count == 1)
    {
        intchars[2] = intchars[2] & 0xBF; // 10111111
        intchars[1] = intchars[1] & 0x7F; // 01111111
    }
    //如果传过来的index是从数据区开始计算，则需要减去一个count
    if (!h_or_d)
    {
        index = index - count;
    }
    if (count == 3)
    {
        dxheap_all[index + 1] = intchars[1];
        dxheap_all[index + 2] = intchars[0];
    }
    if (count == 2)
    {
        dxheap_all[index + 1] = intchars[1];
    }
    dxheap_all[index] = intchars[2];
    return count;
}
/**
 *可变长度，有可能第2个和第3个char没有用
 *第一个字节的6bit，第二个字节的7bit，第三个字节的8bit，总共21位
 *第一个字节的左边第一位tag表示状态，0表示空闲，1表示占用，左边第二位表示是否还需要用到下一个字节，为0表示不需要，为1表示需要
 *第二个字节的左边第一位表示是否还需要用到下一个字节，为0表示不需要，为1表示需要
 */
dxheap_header dxheap_header_decode(int index)
{
    unsigned char intchars[3] = { 0x00, 0x00, 0x00 };
    dxheap_header header;
    unsigned char c1 = dxheap_all[index];
    header.tag = (c1 & (1 << 7)) >> 7;
    int isnext = (c1 & (1 << 6)) >> 6;
    intchars[2] = c1 & 0x3F; // 00111111

    if (isnext)
    {
        char c2 = dxheap_all[index + 1];
        int isnextnext = (c2 & (1 << 7)) >> 7;
        intchars[1] = c2 & 0x7F; // 01111111
        if (isnextnext)
        {
            header.count = 3;
            char c3 = dxheap_all[index + 2];
            intchars[0] = c3;
        }
        else
        {
            header.count = 2;
        }
    }
    else
    {
        header.count = 1;
    }
    //以309600为例
    // 00100101 11001010 00100000  中间这个字节左移动1位
    intchars[1] = (intchars[1] << 1);
    // 00010010 11100101 00100000  前2个字节转换成int然后再右移1位
    int d = ((intchars[0] << 8) | intchars[1]) >> 1;
    intchars[0] = (d >> 8) & 0xFF;
    intchars[1] = (d) & 0xFF;
    // 00010010 11100101 10000000  最后一个字节左移动2位
    intchars[2] = (intchars[2] << 2);
    // 00000100 10111001 01100000  整体右移动2位
    header.length = ((intchars[0] << 16) | intchars[1] << 8 | intchars[2]) >> 2;
    return header;
}
//从header开始
int dxheap_header_encode_h(int tag, int length, int index)
{
    return dxheap_header_encode(tag, length, 1, index);
}
//从数据区开始，只知道length，通过length来反推到开始起点
int dxheap_header_encode_d(int tag, int length, int data_index)
{
    return dxheap_header_encode(tag, length, 0, data_index);
}

void* dxheap_alloc(int size)
{
    void* result;
    int i;
    for (i = 0; i < dxheap_all_size; i++)
    {
        //先获取header的tag和length
        dxheap_header header = dxheap_header_decode(i);
        int tag = header.tag;
        int length = header.length;
        int count = header.count;
        if (tag || size > length - count)
        {
            //假如tag不为0表示不为空，或者这一块不够大，则跳过到下一块
            i = i + count + length;
            // for循环会自动加1，所以提前减1
            i = i - 1;
            continue;
        }
        //长度调整为size,标记调整为占用
        int newcount = dxheap_header_encode_h(1, size, i);
        //因为count发生变化，导致length也发生变化
        int newlength = length + count - newcount;
        //返回的地址需要跳过header
        result = dxheap_all + i + newcount;
        //把剩余的空间标记为空
        int leftlength = newlength - size;
        int nextcount = dxheap_getcount(leftlength);
        i = i + size + newcount;
        //最后能真正使用的大小是leftlength - nextcount
        dxheap_header_encode_h(0, leftlength - nextcount, i);
        //再利用free去合并空闲的区块
        dxheap_free(&dxheap_all[i + nextcount], leftlength - nextcount);
        return result;
    }
    printf("循环到头还没找到合适的内存,尝试GC\n");
    return -1;
}

void dxheap_gc()
{
    int i;
    for (i = 0; i < dxheap_all_size; i++)
    {
        //先获取header的tag和length
        dxheap_header header = dxheap_header_decode(i);
        int tag = header.tag;
        int length = header.length;
        int count = header.count;
        if (!tag)
        {
            dxheap_free(&dxheap_all[i + count], length);
            //往后length个是下一个块头
            int nexindex = i + count + length;
            if (nexindex >= dxheap_all_size)
            {
                return;
            }

            if (((dxheap_all[nexindex] & (1 << 7)) >> 7))
            {
                //如果下一个块不为空，跳下一个位置，否则下次循环还从这个i开始
                i = i + count + length;
            }
        }
        else
        {
            i = i + count + length;
        }
        i = i - 1;
    }
}
void dxheap_free(void* ptr, int osize)
{
    //先判断ptr是否是整个数组区域内的指针
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size];
    if (ptr < min || ptr > max)
    {
        return;
    }
    int index = ptr - min;
    //把标识改成空闲，顺便推算出osize用了几个字节
    int count = dxheap_header_encode_d(0, osize, index);
    memset(ptr, 0x00, osize);
    //往后length个是下一个块头
    int nexindex = index + osize;
    if (nexindex >= dxheap_all_size)
    {
        return;
    }

    if (!((dxheap_all[nexindex] & (1 << 7)) >> 7))
    {
        //如果下一个块也为空，则合并到上一个块
        dxheap_header nextheader = dxheap_header_decode(nexindex);
        //先算出二个数据块的完整长度，包括2个count，2个length
        int combinelenght = osize + nextheader.count + nextheader.length + count;
        int newcount = dxheap_getcount(combinelenght);
        dxheap_header_encode_h(0, combinelenght - newcount, index - count);
        memset(dxheap_all + nexindex, 0x00, nextheader.count + nextheader.length);
    }
}
//这里传递的length是原始长度，包括长度位的长度，也就是1，2，3个字节的长度位再加剩余的长度
int dxheap_getcount(int length)
{
    int count = 1;
    if (length > 64 && length < 8194)
    {
        count = 2;
    }
    else if (length >= 8194)
    {
        count = 3;
    }
    return count;
}
//这里传递的length是真正的数据长度
int dxheap_get_rowcount(int length)
{
    int count = 1;
    if (length >= 64 && length < 8192)
    {
        count = 2;
    }
    else if (length >= 8192)
    {
        count = 3;
    }
    return count;
}
void dxheap_init()
{
    //初始化，把整个数组标记为可用，长度需要减少1,2或3（header的长度）
    int count = dxheap_getcount(dxheap_all_size);
    int n = dxheap_all_size - count;
    dxheap_header_encode_h(0, n, 0);
    memset(dxheap_all + count, 0x00, n);
}

void* dxheap_realloc(void* ptr, int osize, int size)
{
    if (!ptr)
    {
        // ptr为空表示直接alloc一块内存
        void* block = dxheap_alloc(size);
        if (block == -1)
        {
            dxheap_gc(); // gc后再试一次
            block = dxheap_alloc(size);
        }
        if (block == -1)
        {
            printf("再次尝试，还是没有空闲，返回-1\n");
            return block;
        }
        return block;
    }
    //先判断ptr是否是整个数组区域内的指针,不是的话直接alloc
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size - 1];
    if (ptr < min || ptr > max)
    {
        void* block = dxheap_alloc(size);
        if (block == -1)
        {
            dxheap_gc(); // gc后再试一次
            block = dxheap_alloc(size);
        }
        if (block == -1)
        {
            printf("再次尝试，还是没有空闲，返回-1\n");
            return block;
        }
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
        dxheap_gc(); // gc后再试一次
        newBlock = dxheap_alloc(size);
    }
    if (newBlock == -1)
    {
        printf("再次尝试，还是没有空闲，返回-1\n");
        return newBlock;
    }
    memcpy(newBlock, ptr, minLength);
    //最后释放旧的区域
    dxheap_free(ptr, osize);
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
void printdetail(i, count, length)
{
    printf("\n");
    for (i = 0; i < dxheap_all_size; i++)
    {
        print_char(dxheap_all[i]);
    }
}
void printall()
{
    printf("\n");
    int i;
    int j = 0;
    for (i = 0; i < dxheap_all_size; i++)
    {
        dxheap_header header = dxheap_header_decode(i);
        char tag = header.tag;
        int length = header.length;
        int count = header.count;
        printf("第%d块,是否占用%d,头长度%d,数据长度为%d\n", j, tag, count, length);
        j++;
        i = i + count + length - 1;
    }
}
void print_bin(int number)
{
    int bit = sizeof(int) * 8;
    int i;
    for (i = bit - 1; i >= 0; i--)
    {
        int bin = (number & (1 << i)) >> i;
        printf("%d", bin);
        if (i % 8 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}
void print_char(unsigned char number)
{
    int bit = sizeof(unsigned char) * 8;
    int i;
    for (i = bit - 1; i >= 0; i--)
    {
        int bin = (number & (1 << i)) >> i;
        printf("%d", bin);
        if (i % 8 == 0)
        {
            printf(" ");
        }
    }
}
void test1()
{
    if (strlen(dxheap_all) == 0)
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
    // 00000000 00000111 00000010    1794
    // 00000000 00000001 11000010     450
    // 0. 3个char转换成int
    // 00000000 00000001 11000010
    // 1. 450 左移2位
    // 00000000 00000111 00001000
    // 2. char 0 右移2位
    // 00000000 00000111 00000010

    // 00000100 10111001 01100000  309600
    // 00010010 11100101 10000000  左移2位
    // 00010010 11100101 00100000  最后一个字节右移动2位
    // 00100101 11001010 00100000  前2个字节转换成int然后再左移1位
    // 00100101 01100101 00100000  中间这个字节右移动1位
    //反过来
    // 00100101 11001010 00100000  中间这个字节左移动1位
    // 00010010 11100101 00100000  前2个字节转换成int然后再右移1位
    // 00010010 11100101 10000000  最后一个字节左移动2位
    // 00000100 10111001 01100000  整体右移动2位

    int a = 309600;
    int b = a << 2;
    print_bin(b);
    unsigned char intchars[3] = { 0x00, 0x00, 0x00 };
    intchars[0] = (b >> 16) & 0xFF;
    intchars[1] = (b >> 8) & 0xFF;
    intchars[2] = (b) & 0xFF;
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n");
    intchars[2] = (intchars[2] >> 2);
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n");

    int c = ((intchars[0] << 8) | intchars[1]) << 1;

    print_bin(c);
    intchars[0] = (c >> 8) & 0xFF;
    intchars[1] = (c) & 0xFF;
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n");

    intchars[1] = (intchars[1] >> 1);
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n反过来\n");
    //反过来
    intchars[1] = (intchars[1] << 1);
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n");

    int d = ((intchars[0] << 8) | intchars[1]) >> 1;

    print_bin(d);
    intchars[0] = (d >> 8) & 0xFF;
    intchars[1] = (d) & 0xFF;
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n");

    intchars[2] = (intchars[2] << 2);
    print_char(intchars[0]);
    print_char(intchars[1]);
    print_char(intchars[2]);
    printf("\n");

    int e = ((intchars[0] << 16) | intchars[1] << 8 | intchars[2]) >> 2;
    printf("%d\n", e);
}
void test3()
{
    if (strlen(dxheap_all) == 0)
    {
        dxheap_init();
    }
    printall();
    void* a1 = dxheap_realloc(NULL, 0, 10);
    memset(a1, 0x00, 10);
    printall();
    printdetail();
    void* a2 = dxheap_realloc(NULL, 0, 64);
    printdetail();
    memset(a2, 0x00, 64);
    printall();
    printdetail();
}
void test4()
{
    if (strlen(dxheap_all) == 0)
    {
        dxheap_init();
    }
    printall();
    printf("-------------------------------申请798字节---------------------------\n");
    void* t1 = dxheap_realloc(NULL, 0, 798);
    memset(t1, 'a', 798);
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
void test5()
{
    int data = 536870912;
    unsigned char a[4] = { 0 };
    char* p = (char*)&data;

    printf("The data value:%d\n", data);

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        a[i] = (data >> i * 8) & 0xFF;
        //  printf("%x ", a[i]);
    }

    printf("\n");

    *p = 0;
    for (i = 3; i >= 0; i--, p++)
    {
        *p = a[i];
        //  printf("%x  %x\n", a[i], *p);
    }
    printf("The data new value:%d\n", data);
}
int main(int argc, char* argv[])
{

    test5();
}