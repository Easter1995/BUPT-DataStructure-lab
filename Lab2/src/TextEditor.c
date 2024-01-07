#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_LEN 100//每行最大长度
#define MAX_NUM 50//最大行数
typedef struct line {
    char str[MAX_LEN];//每行字符串最多100个
    int len;//行长度
    struct line *next;//指向下一行
    struct line *prior;//指向前一行
}LINE;
typedef struct {
    LINE *head;//头指针
    LINE *tail;//尾指针
    int lineLen;//总行数
}LIST;
//栈
typedef struct sqstack {
    char data;//字符
    struct sqstack *next;//下一个
    struct sqstack *prior;//上一个
}SqStack;

void readText(LIST *list);//打开并读入文件
void outputText(LIST *list);//在屏幕上输出指定范围的行
void lineInsert(LIST *list);//行插入
void lineDelete(LIST *list);//行删除
void strInsert(LIST *list);//行内文本插入
void strDelete(LIST *list);//行内文本删除
void strSearch(LIST *list);//行内文本查找指定行里面某个字符串第一次出现的位置
void testMatch(LIST *list);//检查文本的括号是否匹配
void textSave(LIST *list);//文本文件保存

int main() {
    char func[10];
    LIST list;//用链表链接每一行
    list.head = NULL;
    list.tail = NULL;
    list.lineLen = 0;
    while(scanf("%s",&func)!=EOF) {
        getchar();
        if(strcmp(func,"func1")==0)
            readText(&list);
        else if(strcmp(func,"func2")==0)
            outputText(&list);
        else if(strcmp(func,"func3")==0)
            lineInsert(&list);
        else if(strcmp(func,"func4")==0)
            lineDelete(&list);
        else if(strcmp(func,"func5")==0)
            strInsert(&list);
        else if(strcmp(func,"func6")==0)
            strDelete(&list);
        else if(strcmp(func,"func7")==0)
            strSearch(&list);
        else if(strcmp(func,"func8")==0)
            testMatch(&list);
        else if(strcmp(func,"func9")==0)
            textSave(&list);
    }
    return 0;
}

//func1
void readText(LIST *list) {
    FILE *fp;
    fp = fopen("D:/C/DataStructure/Lab2/data/data.txt","r");//只读模式打开一个文件
    char buf[MAX_LEN];
    while(fgets(buf,MAX_LEN-1,fp)) {
        list->lineLen++;
        LINE *p = (LINE*)malloc(sizeof(LINE));
        strcpy(p->str,buf);
        p->next = NULL;
        p->len = strlen(buf);
        //若p不是第一个节点
        if(list->head) {
            list->tail->next = p;
            p->prior = list->tail;
            list->tail = p;
        }
        //若p是第一个节点
        else {
            p->prior = NULL;
            list->head = p;
            list->tail = p;
        }
    }
    fclose(fp);
    return;
}

//func2
void outputText(LIST *list) {
    int lineNums[MAX_NUM];//存储要打印输出的行号
    int i=0,nums=0,lines=1;
    LINE *p;
    p=list->head;
    while(scanf("%d",&lineNums[nums++])) {
        if(getchar()=='\n')
            break;
    }
    //nums表示有效的行号数
    while(i<nums && p) {
        //遍历链表(每一行)
        if(lineNums[i] <= 0 || lineNums[i] > list->lineLen) {
            printf("第%d行不存在\n",lineNums[i]);
            i++;
        }
        else {
           if(lines==lineNums[i]) {
                printf("%s",p->str);
                i++;
            }
            //后面的行数比1前面大，还要再遍历一次
            if(lineNums[i-1]>lineNums[i]) {
                lineNums[i-1] = 0;//防止进入死循环
                p = list->head;
                lines = 0;
            }
            else
                p = p->next;
            lines++; 
        }
    }
    return;
}

//func3
void lineInsert(LIST *list) {
    int insertNum,lineNum=1;//指在insertNum这个行号后面插入一行
    scanf("%d",&insertNum);
    LINE *p,*q,*s;//p是inserNum对应的行,q是插入的行
    p = list->head;
    if(insertNum < 0 || insertNum > list->lineLen)
        printf("第%d行不存在",insertNum);
    else {
        q = (LINE*)malloc(sizeof(LINE));
        if(insertNum == 0) {
            q->prior = NULL;
            p->prior = q;
            q->next = p;
            list->head = q;
        }
        else {
            while(lineNum!=insertNum) {
            p=p->next;
            lineNum++;
            }
            //初始化q的信息
            q->next = p->next;
            q->next->prior = q;
            p->next = q;
            q->prior = p;
            
        }
        q->len = 1;
        q->str[0] = '\n';
        q->str[1] = '\0';
    }
    return;
}

//func4
void lineDelete(LIST *list) {
    int lineNum,total;//lineNums表示开始删除的行号,total表示从那一行开始总共要删除的连续行数
    int lines=1,i;
    LINE *p;
    p=list->head;
    scanf("%d %d",&lineNum,&total);
    while(lines != lineNum) {
        p = p->next;
        lines++;
    }
    //要删除的是第一行 需要移动头指针
    if(lineNum == 1) {
        for(i = 0 ; i<total && p ; i++) {
            list->head = p->next;
            p = p->next;
            list->head->prior = NULL;
            free(p->prior);
        }
    }
    else {
        for(i=lineNum ; (i<lineNum+total) && p ; i++) {
            p->prior->next = p->next;
            if(p->next) {
                p->next->prior = p->prior;
            }
            LINE *q = p;
            p = p->next;
            free(q);
        }
    }
    return;
}

//func5
void strInsert(LIST *list) {
    int lineNum,chNum,lines=1;
    int i,j=0;
    char insertStr[MAX_LEN];
    scanf("%d %d",&lineNum,&chNum);//要插入的行号和列号
    getchar();
    if(lineNum > list->lineLen)
        printf("第%d行不存在\n",lineNum);
    else {
        scanf("%[^\n]",insertStr);//要插入的字符串
        getchar();
        LINE *p = list->head;
        //找到要插入的行
        while(lines != lineNum && p) {
            p = p->next;
            lines++;
        } 
        int lenNewStr = strlen(insertStr);
        int len = p->len + lenNewStr;
        if(len >= MAX_LEN)//结尾有\0
            printf("你输入的字符串长度不合法\n");
        else { 
            if(chNum >= p->len) {
                chNum = p->len - 1;//最后一个字符是\n,所以应该插入到p->len-2后面,\n前面 
            }
            for(i = p->len ; i >= chNum ; i--)//将包括\0的原字符向后面移lenNewStr位
                p->str[i + lenNewStr] = p->str[i];
            for(i = chNum ; i < lenNewStr + chNum ; i++)
                p->str[i] = insertStr[j++];
            p->len = len;
        }
    }
    return;
}

//func6
void strDelete(LIST *list) {
    int lineNum,chNum,total,lines=1;
    int i,j=0;
    scanf("%d %d %d",&lineNum,&chNum,&total);//要插入的行号、列号(包括)、总共要删除的字符数量
    getchar();
    if(lineNum > list->lineLen)
        printf("第%d行不存在\n",lineNum);
    else {
        LINE *p = list->head;
        //找到要删除的行
        while(lines != lineNum && p) {
            p = p->next;
            lines++;
        } 
        if(chNum >= p->len || chNum <= 0)//p->len为换行符,不能删除
            printf("要删除的字符不存在\n");
        else {
            if(total > p->len - chNum)//删除的总数超过了能删除的个数
                total = p->len - chNum;//把能删除的都删除了(除换行符)
            for(i = chNum + total - 1 ; i <= p->len ; i++)
                p->str[i - total] = p->str[i];
            p->len -= total;
        }   
    }
    return;
}

//func7 KMP算法
void strSearch(LIST *list) {
    int lineNum,lines=1;
    int i,j;
    LINE *p = list->head;
    scanf("%d",&lineNum);//要查找的行号
    getchar();
    if(lineNum > list->lineLen)
        printf("第%d行不存在\n",lineNum);
    else {    
        //找到要查找的行
        while(lines != lineNum && p) {
            p = p->next;
            lines++;
        }

        //获取子串(从1开始)
        char searchStr[MAX_LEN];
        char ch;
        i=1;
        ch=getchar();
        while(ch!='\n')
        {
            searchStr[i]=ch;
            i++;
            scanf("%c",&ch);
        }
        searchStr[0]=i-1;
        
        // 计算next数组
        int next[searchStr[0]+1];
        i = 1;j = 0;//i表示后缀,j表示前缀
        next[1] = 0;
        while(i < searchStr[0]) {
            if(j==0 || searchStr[i] == searchStr[j]) {
                j++;
                i++;
                next[i] = j;
            }
            else
                j = next[j];//回溯
        }

        // 模式匹配
        i = 1;j = 1;//i表示主串下标,j表示模式串下标
        while(i <= p->len - 1) {
            while(j!=0 && p->str[i-1]!=searchStr[j]) 
                j = next[j];//回溯
            if(j==0 || p->str[i-1] == searchStr[j])
                j++;//继续匹配
            if(j == searchStr[0]+1) {
                printf("该字符串在第%d位首次出现\n",i-searchStr[0]+1);
                return;
            }
            i++;
        }
        printf("第%d行不包含此字符串\n",lineNum);
    }
    return;
}

//func8
void testMatch(LIST *list) {
    //创建一个栈来存储括号,每次遇到右括号就弹出一对
    //栈中什么都没有了就表示括号匹配对了
    SqStack *top = NULL;//栈顶指针
    LINE *p = list->head;
    int i = 0,line = 1;
    while(p) {
        for(i = 0 ; i < p->len ; i++) {
            if(p->str[i] == '(' || p->str[i] == '{') {
                //左括号入栈
                SqStack *s;
                s = (SqStack*)malloc(sizeof(SqStack));
                s->data = p->str[i];
                s->prior = NULL;
                if(top) {
                    top->prior = s;
                    s->next = top;
                    top = s;
                }
                else {
                    top = s;
                    s->next = NULL;
                    s->prior = NULL;
                }
            }
            else if(p->str[i] == ')') {
                //遇到右括号,栈顶元素出栈
                if(top->data != '(') {
                    printf("第%d行第%d列圆括号匹配失败\n",line,i+1);
                    return;
                }
                else 
                    top = top->next;
            }
            else if(p->str[i] == '}') {
                //遇到右括号,栈顶元素出栈
                if(top->data != '{') {
                    printf("第%d行第%d列花括号匹配失败\n",line,i+1);
                    return;
                }
                else 
                    top = top->next;
            }
        }
        p = p->next; //换下一行
        line++;
    }
    if(top) 
        printf("括号匹配失败\n");
    else
        printf("括号匹配成功\n");
    return;
}

//func9
void textSave(LIST *list) {
    FILE *fp;
    fp = fopen("D:/C/DataStructure/Lab2/data/output.txt","w");
    LINE *p;
    p = list->head;
    while(p) {
        fputs(p->str,fp);
        p = p->next;
    }
    fclose(fp);
}