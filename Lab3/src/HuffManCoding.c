#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM 0xffffffff

typedef struct {
    char data;
    unsigned int weight,parent,lchild,rchild;
}HTNode,*HuffmanTree;
typedef char** HuffmanCode;
int nodeNum;

void initHuffmanTree(HuffmanTree *HT,HuffmanCode *HC);
void buildHuffmanTree(HuffmanTree *HT,HuffmanCode *HC);
void storeHuffmanTree(HuffmanTree *HT,HuffmanCode *HC);
void codeText(HuffmanTree *HT,HuffmanCode *HC);
void deCodeText(HuffmanTree *HT,HuffmanCode *HC);

int main() {
    int i;
    char func[10];
    HuffmanTree HT;//哈夫曼树HT
    HuffmanCode HC;//哈夫曼编码
    initHuffmanTree(&HT,&HC);//初始化空间及数据
    buildHuffmanTree(&HT,&HC);//建造哈夫曼树
    storeHuffmanTree(&HT,&HC);//储存哈夫曼树
    while(scanf("%s",&func)!=EOF) {
        getchar();
        if(strcmp(func,"code")==0)//编码
            codeText(&HT,&HC);
        else if(strcmp(func,"decode")==0)//译码
            deCodeText(&HT,&HC); 
    }
    return 0;
}

void initHuffmanTree(HuffmanTree *HT,HuffmanCode *HC) {
    int i;
    // 要编码的字符个数
    printf("how many characters:");
    scanf("%d",&nodeNum);
    if(nodeNum <= 1)
    {*HT = NULL;return;}
    *HT = (HuffmanTree)malloc(2*nodeNum*sizeof(HTNode));//HT的大小
    *HC = (char**)malloc((nodeNum+1)*sizeof(char*));//HC的大小
    getchar();
    // 双亲孩子初始化
    for(i = 1 ; i <= 2*nodeNum - 1 ; i++) {
        (*HT)[i].lchild = (*HT)[i].rchild = (*HT)[i].parent = 0; 
    }
    // 输入要编码的字符
    printf("input characters to be coded:");
    for(i = 1 ; i <= nodeNum ; i++) {
        scanf(" %c",&((*HT)[i].data));
    }
    getchar();
    // 输入每个字符的权值
    printf("input the weight of characters:");
    for(i = 1 ; i <= nodeNum ; i++) {
        scanf("%d",&((*HT)[i].weight));
    }
    getchar();
    return;
}

void buildHuffmanTree(HuffmanTree *HT,HuffmanCode *HC) {
    if(nodeNum <= 1)
        return;
    unsigned int min1,min2,l,r;
    int i,j;
    for(i = nodeNum + 1 ; i <= 2*nodeNum - 1 ; i++) {
        min1 = min2 = MAX_NUM;
        // 找左子树 即第一小的节点
        for(j = 1 ; j <= i - 1 ; j++) {
            if((*HT)[j].weight < min1 && (*HT)[j].parent == 0) {
                min1 = (*HT)[j].weight;
                l = j;
            }
        }
        (*HT)[l].parent = i;
        (*HT)[i].lchild = l;
        // 找右子树 即第二小的节点
        for(j = 1 ; j <= i - 1 ; j++) {
            if((*HT)[j].weight < min2 && (*HT)[j].parent == 0) {
                min2 = (*HT)[j].weight;
                r = j;
            }
        }
        (*HT)[r].parent = i;
        (*HT)[i].rchild = r;
        // 构建第i个节点
        (*HT)[i].data = '#';
        (*HT)[i].weight = min1 + min2;
    }

    //给每个字母编码
    int start,f;
    char* cd = (char*)malloc(nodeNum*sizeof(nodeNum));// 临时存储编码
    cd[nodeNum - 1] = '\0';
    for(i = 1; i <= nodeNum ; i++) {
        start = nodeNum - 1;
        for(j = i,f = (*HT)[i].parent ; f != 0 ; j = f,f = (*HT)[f].parent) {
            // f是j的父节点，一直往上直到根节点
            if((*HT)[f].lchild == j) 
                cd[--start] = '0';// 左孩子为0
            else
                cd[--start] = '1';// 右孩子为1
        }
        (*HC)[i] = (char*)malloc((nodeNum - start)*sizeof(char));
        strcpy((*HC)[i],&cd[start]);
    }
    free(cd);
    return;
}

void storeHuffmanTree(HuffmanTree *HT,HuffmanCode *HC) {
    FILE *fp;int i;
    char buf[200];
    fp = fopen("D:/C/DataStructure/Lab3/data/hfmTree.txt","w");
    // 按照“data 权值 左孩子 右孩子 双亲 编码”的顺序存储在文件hfmTree.txt里面
    for(i = 1 ; i <= 2*nodeNum - 1 ; i++) {
        // 有编码的情况
        if(i <= nodeNum) 
            sprintf(buf,"%c %d %d %d %d\t%s\n",(*HT)[i].data,(*HT)[i].weight,(*HT)[i].lchild,(*HT)[i].rchild,(*HT)[i].parent,(*HC)[i]);
        else
            sprintf(buf,"%c %d %d %d %d\n",(*HT)[i].data,(*HT)[i].weight,(*HT)[i].lchild,(*HT)[i].rchild,(*HT)[i].parent);
        fputs(buf,fp);
    }
    fclose(fp);
}

void codeText(HuffmanTree *HT,HuffmanCode *HC) {
    FILE *fpRead,*fpWrite;
    char buf;// 存的是从要翻译的文件读取的一个字符
    int i;
    fpRead = fopen("D:/C/DataStructure/Lab3/data/ToBeTran.txt","r");//要被编码的文件
    fpWrite = fopen("D:/C/DataStructure/Lab3/data/CodeFile.txt","w");//编码存放的文件

    while((buf = fgetc(fpRead)) != EOF) {
        if(buf == '\n'||buf == ' ')
            fputc(buf,fpWrite);
        else {
            for(i = 1 ; i <= nodeNum ; i++) {
                if((*HT)[i].data == buf) {
                    fputs((*HC)[i],fpWrite);
                }
            }
        }
    }
    fclose(fpRead);
    fclose(fpWrite);
    return;
}

void deCodeText(HuffmanTree *HT,HuffmanCode *HC) {
    FILE *fpRead,*fpWrite;
    HTNode htnode = (*HT)[2*nodeNum - 1];
    char buf;
    fpRead = fopen("D:/C/DataStructure/Lab3/data/CodeFile.txt","r");//编码的文件
    fpWrite = fopen("D:/C/DataStructure/Lab3/data/TextFile.txt","w");//译码的文件
    while((buf = fgetc(fpRead)) != EOF) {
        if(buf == '0') {// 走向左孩子
            htnode = (*HT)[htnode.lchild];
        }
        else if(buf == '1') {// 走向右孩子
            htnode = (*HT)[htnode.rchild];
        }
        if(htnode.lchild == 0 && htnode.rchild == 0) {// 叶子节点
            fputc(htnode.data,fpWrite);
            htnode = (*HT)[2*nodeNum - 1];
        }
        if(buf == '\n')
            fputc('\n',fpWrite);
    }
    return;
}