#include <stdio.h>
////////////////////////////////////////////////////////////////////////
// �̶���С��ʽ��ÿ��ռ��4���ֽڣ��ض��ĵ�Ƭ��ֻ֧�����ַ�ʽ
// �汾 20220913
//������ڴ��������
#define dxheap_block_count 64
//�ܹ�ռ�õ��ֽ�����Ҫ*4
static unsigned char dxheap[dxheap_block_count * 4];
//��Ҫ��count/8���ֽ�����ʶÿ������ռ�û��ǿ��У�1��bit����ʾһ������������0��ʾ���У�1��ʾռ��
static unsigned char dxheap_empty_flags[dxheap_block_count / 8];

/**
 * @brief �ڴ���ʶռ�û��ǿ���,��һ�������Ŀ��ǳ�ռ�û��ǿ���
 *
 * @param tag 0��ʾ���У�1��ʾռ��
 * @param start_block_index ��ʼ�Ŀ���ţ���0��ʼ
 * @param count ������ĸ���
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
//�жϵ�index�����Ƿ�ռ�� 0��ʾ���У�1��ʾռ��
int dxheap_flag_read(int index)
{
    char c = dxheap_empty_flags[index / 8];
    int pos = index % 8;
    return (c & (1 << pos)) >> pos;
}
//�жϴӵ�index���鿪ʼ������count���鶼Ϊ���򷵻�0������1
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
    //�ȼ�����Ҫ�Ŀ�����
    int count;
    if (size % 4 == 0)
    {
        count = size / 4;
    }
    else
    {
        //�ڴ�����˷�
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
            //���粻Ϊ��,����������һ��
            i++;
            continue;
        }
        //�ж��Ƿ����ҵ�count�����п�
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
    //���ж�ptr�Ƿ����������������ڵ�ָ��
    int min = &dxheap[0];
    int max = &dxheap[dxheap_block_count * 4 - 1];
    if (ptr < min || ptr > max)
    {
        return;
    }
    //�����ptr��Ӧ��ָ��ָ��ڼ���,����index��Ȼ�ǿ�������4�ģ��ǿ���׵�ַ�������Ҵ�һ����ַ����
    int index = ptr - min;
    index = index / 4;
    //����osize���ֽڶ�Ӧ�Ŀ�Ҳ��Ҫ�ÿ�
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
    //��ʼ����������������Ϊ���ã�
    memset(dxheap, 0x00, dxheap_block_count * 4);
}

void* dxheap_realloc(void* ptr, int osize, int size)
{
    if (!ptr)
    {
        // ptrΪ�ձ�ʾֱ��allocһ���ڴ�
        void* block = dxheap_alloc(size);
        return block;
    }
    //���ж�ptr�Ƿ����������������ڵ�ָ��,���ǵĻ�ֱ��alloc
    int min = &dxheap[0];
    int max = &dxheap[dxheap_block_count * 4 - 1];
    if (ptr < min || ptr > max)
    {
        void* block = dxheap_alloc(size);
        return block;
    }
    //����һ���㹻����¿죬�����ݸ��ƹ�ȥ
    int index = ptr - min;
    //�п���osize��size��,���ﲻ��math.min
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
    //����ͷžɵ�����
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
    printf("-------------------------------����100�ֽ�---------------------------\n");
    void* t1 = dxheap_realloc(NULL, 0, 100);
    memset(t1, 'a', 100);
    printall();
    printf("-------------------------------����64�ֽ�---------------------------\n");
    void* t2 = dxheap_realloc(NULL, 0, 64);
    memset(t2, 'b', 64);
    printall();
    printf("-------------------------------����032�ֽ�---------------------------\n");
    void* t3 = dxheap_realloc(NULL, 0, 32);
    memset(t3, 'c', 32);
    printall();
    printf("-------------------------------�ͷ�64�ֽ�---------------------------\n");
    dxheap_free(t2, 64);
    printall();
    printf("-------------------------------����040�ֽ�---------------------------\n");
    void* t4 = dxheap_realloc(NULL, 0, 40);
    printall();
    printf("-------------------------------����050�ֽ�(������ǰ��40�ֽ���չ)--------\n");
    void* t5 = dxheap_realloc(t4, 40, 50);
    memset(t5, 'c', 50);
    printall();
    printf("-------------------------------�ͷ�050�ֽ�---------------------------\n");
    dxheap_free(t5, 50);
    printall();
    printf("-------------------------------�ͷ�032�ֽ�---------------------------\n");
    dxheap_free(t3, 32);
    printall();
    // printf("-------------------------------����512�ֽ�---------------------------\n");
    // void *t6 = dxheap_realloc(NULL, 0, 512);
    // printall();
    // printf("-------------------------------����250�ֽ�---------------------------\n");
    // dxheap_realloc(NULL, 0, 250);
    // printall();
    // printf("-------------------------------����512�ֽ�---------------------------\n");
    // dxheap_realloc(NULL, 0, 512);
    // printall();
    // printf("-------------------------------����30000�ֽ�---------------------------\n");
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
    printf("��25=%d\n", dxheap_flag_read(25));
    printf("��29=%d\n", dxheap_flag_read(29));
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
    printf("-------------------------------����198�ֽ�---------------------------\n");
    void* t1 = dxheap_realloc(NULL, 0, 198);
    memset(t1, 'a', 198);
    printall();
    printf("-------------------------------����80�ֽ�---------------------------\n");
    void* t2 = dxheap_realloc(NULL, 0, 80);
    memset(t2, 'b', 80);
    printall();
    printf("-------------------------------����032�ֽ�---------------------------\n");
    void* t3 = dxheap_realloc(NULL, 0, 32);
    memset(t3, 'c', 32);
    printall();
    printf("-------------------------------����016�ֽ�---------------------------\n");
    void* t4 = dxheap_realloc(NULL, 0, 16);
    printall();
    printf("-------------------------------����32�ֽ�--------\n");
    void* t5 = dxheap_realloc(NULL, 0, 32);
    memset(t5, 'c', 32);
    printall();

    printf("-------------------------------����34�ֽ�--------\n");
    void* t6 = dxheap_realloc(NULL, 0, 34);
    memset(t6, 'c', 34);
    printall();

    printf("-------------------------------����24�ֽ�--------\n");
    void* t7 = dxheap_realloc(NULL, 0, 24);
    memset(t7, 'c', 24);
    printall();

    printf("-------------------------------����27�ֽ�--------\n");
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