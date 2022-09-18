#include <stdio.h>
////////////////////////////////////////////////////////////////////////
// TLV��ʽ��1���ֽڵı�ʶtag+�ɱ��ֽڵĳ���+���ȸ��ֽڵ�������
#define dxheap_all_size 3000
static unsigned char dxheap_all[dxheap_all_size];

typedef struct
{
    // 0��ʾ���У�1��ʾռ��
    int tag;
    //������ĳ���
    int length;
    //����ռ���ֽڵĸ�����1����2�������3��
    int count;
} dxheap_header;
int dxheap_header_encode(int tag, int length, int h_or_d, int index)
{
    // 00000100 10111001 01100000  ��309600Ϊ��
    // 00010010 11100101 10000000  ����2λ
    int b = length << 2;
    // 00010010 11100101 00100000  ���һ���ֽ����ƶ�2λ
    unsigned char intchars[3] = { 0x00, 0x00, 0x00 };
    intchars[0] = (b >> 16) & 0xFF;
    intchars[1] = (b >> 8) & 0xFF;
    intchars[2] = (b) & 0xFF;
    intchars[2] = (intchars[2] >> 2);
    // 00100101 11001010 00100000  ǰ2���ֽ�ת����intȻ��������1λ
    int c = ((intchars[0] << 8) | intchars[1]) << 1;
    intchars[0] = (c >> 8) & 0xFF;
    intchars[1] = (c) & 0xFF;
    // 00100101 01100101 00100000  �м�����ֽ����ƶ�1λ
    intchars[1] = (intchars[1] >> 1);

    int count = dxheap_get_rowcount(length);
    //��һ���ֽڵ���ߵ�һλtag��ʾ״̬��0��ʾ���У�1��ʾռ��
    if (tag)
    {
        intchars[2] = intchars[2] | 0x80; // 10000000
    }
    else
    {
        intchars[2] = intchars[2] & 0x7F; // 01111111
    }

    // ��һ���ֽ���ߵڶ�λ��ʾ�Ƿ���Ҫ�õ���һ���ֽڣ�Ϊ0��ʾ����Ҫ��Ϊ1��ʾ��Ҫ
    // �ڶ����ֽڵ���ߵ�һλ��ʾ�Ƿ���Ҫ�õ���һ���ֽڣ�Ϊ0��ʾ����Ҫ��Ϊ1��ʾ��Ҫ
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
    //�����������index�Ǵ���������ʼ���㣬����Ҫ��ȥһ��count
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
 *�ɱ䳤�ȣ��п��ܵ�2���͵�3��charû����
 *��һ���ֽڵ�6bit���ڶ����ֽڵ�7bit���������ֽڵ�8bit���ܹ�21λ
 *��һ���ֽڵ���ߵ�һλtag��ʾ״̬��0��ʾ���У�1��ʾռ�ã���ߵڶ�λ��ʾ�Ƿ���Ҫ�õ���һ���ֽڣ�Ϊ0��ʾ����Ҫ��Ϊ1��ʾ��Ҫ
 *�ڶ����ֽڵ���ߵ�һλ��ʾ�Ƿ���Ҫ�õ���һ���ֽڣ�Ϊ0��ʾ����Ҫ��Ϊ1��ʾ��Ҫ
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
    //��309600Ϊ��
    // 00100101 11001010 00100000  �м�����ֽ����ƶ�1λ
    intchars[1] = (intchars[1] << 1);
    // 00010010 11100101 00100000  ǰ2���ֽ�ת����intȻ��������1λ
    int d = ((intchars[0] << 8) | intchars[1]) >> 1;
    intchars[0] = (d >> 8) & 0xFF;
    intchars[1] = (d) & 0xFF;
    // 00010010 11100101 10000000  ���һ���ֽ����ƶ�2λ
    intchars[2] = (intchars[2] << 2);
    // 00000100 10111001 01100000  �������ƶ�2λ
    header.length = ((intchars[0] << 16) | intchars[1] << 8 | intchars[2]) >> 2;
    return header;
}
//��header��ʼ
int dxheap_header_encode_h(int tag, int length, int index)
{
    return dxheap_header_encode(tag, length, 1, index);
}
//����������ʼ��ֻ֪��length��ͨ��length�����Ƶ���ʼ���
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
        //�Ȼ�ȡheader��tag��length
        dxheap_header header = dxheap_header_decode(i);
        int tag = header.tag;
        int length = header.length;
        int count = header.count;
        if (tag || size > length - count)
        {
            //����tag��Ϊ0��ʾ��Ϊ�գ�������һ�鲻��������������һ��
            i = i + count + length;
            // forѭ�����Զ���1��������ǰ��1
            i = i - 1;
            continue;
        }
        //���ȵ���Ϊsize,��ǵ���Ϊռ��
        int newcount = dxheap_header_encode_h(1, size, i);
        //��Ϊcount�����仯������lengthҲ�����仯
        int newlength = length + count - newcount;
        //���صĵ�ַ��Ҫ����header
        result = dxheap_all + i + newcount;
        //��ʣ��Ŀռ���Ϊ��
        int leftlength = newlength - size;
        int nextcount = dxheap_getcount(leftlength);
        i = i + size + newcount;
        //���������ʹ�õĴ�С��leftlength - nextcount
        dxheap_header_encode_h(0, leftlength - nextcount, i);
        //������freeȥ�ϲ����е�����
        dxheap_free(&dxheap_all[i + nextcount], leftlength - nextcount);
        return result;
    }
    printf("ѭ����ͷ��û�ҵ����ʵ��ڴ�,����GC\n");
    return -1;
}

void dxheap_gc()
{
    int i;
    for (i = 0; i < dxheap_all_size; i++)
    {
        //�Ȼ�ȡheader��tag��length
        dxheap_header header = dxheap_header_decode(i);
        int tag = header.tag;
        int length = header.length;
        int count = header.count;
        if (!tag)
        {
            dxheap_free(&dxheap_all[i + count], length);
            //����length������һ����ͷ
            int nexindex = i + count + length;
            if (nexindex >= dxheap_all_size)
            {
                return;
            }

            if (((dxheap_all[nexindex] & (1 << 7)) >> 7))
            {
                //�����һ���鲻Ϊ�գ�����һ��λ�ã������´�ѭ���������i��ʼ
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
    //���ж�ptr�Ƿ����������������ڵ�ָ��
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size];
    if (ptr < min || ptr > max)
    {
        return;
    }
    int index = ptr - min;
    //�ѱ�ʶ�ĳɿ��У�˳�������osize���˼����ֽ�
    int count = dxheap_header_encode_d(0, osize, index);
    memset(ptr, 0x00, osize);
    //����length������һ����ͷ
    int nexindex = index + osize;
    if (nexindex >= dxheap_all_size)
    {
        return;
    }

    if (!((dxheap_all[nexindex] & (1 << 7)) >> 7))
    {
        //�����һ����ҲΪ�գ���ϲ�����һ����
        dxheap_header nextheader = dxheap_header_decode(nexindex);
        //������������ݿ���������ȣ�����2��count��2��length
        int combinelenght = osize + nextheader.count + nextheader.length + count;
        int newcount = dxheap_getcount(combinelenght);
        dxheap_header_encode_h(0, combinelenght - newcount, index - count);
        memset(dxheap_all + nexindex, 0x00, nextheader.count + nextheader.length);
    }
}
//���ﴫ�ݵ�length��ԭʼ���ȣ���������λ�ĳ��ȣ�Ҳ����1��2��3���ֽڵĳ���λ�ټ�ʣ��ĳ���
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
//���ﴫ�ݵ�length�����������ݳ���
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
    //��ʼ����������������Ϊ���ã�������Ҫ����1,2��3��header�ĳ��ȣ�
    int count = dxheap_getcount(dxheap_all_size);
    int n = dxheap_all_size - count;
    dxheap_header_encode_h(0, n, 0);
    memset(dxheap_all + count, 0x00, n);
}

void* dxheap_realloc(void* ptr, int osize, int size)
{
    if (!ptr)
    {
        // ptrΪ�ձ�ʾֱ��allocһ���ڴ�
        void* block = dxheap_alloc(size);
        if (block == -1)
        {
            dxheap_gc(); // gc������һ��
            block = dxheap_alloc(size);
        }
        if (block == -1)
        {
            printf("�ٴγ��ԣ�����û�п��У�����-1\n");
            return block;
        }
        return block;
    }
    //���ж�ptr�Ƿ����������������ڵ�ָ��,���ǵĻ�ֱ��alloc
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size - 1];
    if (ptr < min || ptr > max)
    {
        void* block = dxheap_alloc(size);
        if (block == -1)
        {
            dxheap_gc(); // gc������һ��
            block = dxheap_alloc(size);
        }
        if (block == -1)
        {
            printf("�ٴγ��ԣ�����û�п��У�����-1\n");
            return block;
        }
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
        dxheap_gc(); // gc������һ��
        newBlock = dxheap_alloc(size);
    }
    if (newBlock == -1)
    {
        printf("�ٴγ��ԣ�����û�п��У�����-1\n");
        return newBlock;
    }
    memcpy(newBlock, ptr, minLength);
    //����ͷžɵ�����
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
        printf("��%d��,�Ƿ�ռ��%d,ͷ����%d,���ݳ���Ϊ%d\n", j, tag, count, length);
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
    // 00000000 00000111 00000010    1794
    // 00000000 00000001 11000010     450
    // 0. 3��charת����int
    // 00000000 00000001 11000010
    // 1. 450 ����2λ
    // 00000000 00000111 00001000
    // 2. char 0 ����2λ
    // 00000000 00000111 00000010

    // 00000100 10111001 01100000  309600
    // 00010010 11100101 10000000  ����2λ
    // 00010010 11100101 00100000  ���һ���ֽ����ƶ�2λ
    // 00100101 11001010 00100000  ǰ2���ֽ�ת����intȻ��������1λ
    // 00100101 01100101 00100000  �м�����ֽ����ƶ�1λ
    //������
    // 00100101 11001010 00100000  �м�����ֽ����ƶ�1λ
    // 00010010 11100101 00100000  ǰ2���ֽ�ת����intȻ��������1λ
    // 00010010 11100101 10000000  ���һ���ֽ����ƶ�2λ
    // 00000100 10111001 01100000  �������ƶ�2λ

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
    printf("\n������\n");
    //������
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
    printf("-------------------------------����798�ֽ�---------------------------\n");
    void* t1 = dxheap_realloc(NULL, 0, 798);
    memset(t1, 'a', 798);
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