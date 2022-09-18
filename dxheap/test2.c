#include <stdio.h>
////////////////////////////////////////////////////////////////////////
// TLV��ʽ��1���ֽڵı�ʶtag+4���ֽڵĳ���+���ȸ��ֽڵ�������
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
        //�ȴ�ǰ5���ֽڻ�ȡheader��tag��length
        char tag = dxheap_all[i];
        int length = dxheap_char2int(i);

        if (tag != '0' || size > length - 5)
        {
            //����tag��Ϊ0��ʾ��Ϊ�գ�������һ�鲻��������������һ��
            i = i + 5 + length;
            // forѭ�����Զ���1��������ǰ��1
            i = i - 1;
            continue;
        }
        dxheap_all[i] = '1';
        //���ȵ���Ϊsize
        dxheap_int2char(size, i);
        //���صĵ�ַ��Ҫ����header
        result = dxheap_all + i + 5;
        //��ʣ��Ŀռ���Ϊ��
        i = i + size + 5;
        dxheap_all[i] = '0';
        dxheap_int2char(length - size - 5, i);
        //������freeȥ�ϲ����е�����
        dxheap_free(&dxheap_all[i + 5]);
        return result;
    }
}

void dxheap_free(void* ptr)
{
    //���ж�ptr�Ƿ����������������ڵ�ָ��
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size];
    if (ptr < min || ptr > max)
    {
        return;
    }
    int index = ptr - min;
    //��ǰ5����header��tag
    dxheap_all[index - 5] = '0';
    //��ǰ4����header��length
    int length = dxheap_char2int(index - 5);
    memset(ptr, '\0', length);
    //����length������һ����ͷ
    int nexindex = index + length;
    if (ptr > max)
    {
        return;
    }
    if (dxheap_all[nexindex] == '0')
    {
        //�����һ����ҲΪ�գ���ϲ�����һ����
        int nextlength = dxheap_char2int(nexindex);
        length = length + nextlength + 5;
        dxheap_int2char(length, index - 5);
        memset(ptr, '\0', length);
    }
}
void dxheap_init()
{
    //��ʼ����������������Ϊ���ã�������Ҫ����5��header�ĳ��ȣ�
    int n = dxheap_all_size - 5;
    dxheap_all[0] = '0';
    dxheap_int2char(n, 0);
    memset(dxheap_all + 5, '\0', n);
}

void* dxheap_realloc(void* ptr, int size)
{
    if (!ptr)
    {
        // ptrΪ�ձ�ʾֱ��allocһ���ڴ�
        return dxheap_alloc(size);
    }
    //���ж�ptr�Ƿ����������������ڵ�ָ��,���ǵĻ�ֱ��alloc
    int min = &dxheap_all[0];
    int max = &dxheap_all[dxheap_all_size];
    if (ptr < min || ptr > max)
    {
        return dxheap_alloc(size);
    }
    //����һ���㹻����¿죬�����ݸ��ƹ�ȥ
    int index = ptr - min;
    int length = dxheap_char2int(index - 5);
    //�п���length��size��,���ﲻ��math.min
    int minLength = length;
    if (length > size)
    {
        minLength = size;
    }
    void* newBlock = dxheap_alloc(size);
    memcpy(newBlock, ptr, minLength);
    //����ͷžɵ�����
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
        printf("��%d��,�Ƿ����%d,����Ϊ%d\n", j, tag == '0', length);
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