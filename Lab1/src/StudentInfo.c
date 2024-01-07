#include<stdio.h>
#include<string.h>
#define NUM 2//每次输入的学生信息数量

typedef struct{
    char id[10];
    char name[10];
    char age[4];
}STUDENT;

void func1(STUDENT *info);
void func2(FILE *fp,STUDENT *info);
void func3(FILE *fp);

int main(){
    char func[10];//用户输入想使用哪一个功能
    STUDENT info[NUM];
    FILE *fp;
    while(scanf("%s",&func)!=EOF){
        getchar();
        if(strcmp(func,"func1")==0){
            func1(info);
        }
        else if(strcmp(func,"func2")==0){
            fp=fopen("D:/C/DataStructure/Lab1/dat/input.dat","a");//以追加模式打开一个文件
            func2(fp,info);
        }
        else if(strcmp(func,"func3")==0){
            fp=fopen("D:/C/DataStructure/Lab1/dat/input.dat","r");//以只读模式打开一个文件
            func3(fp);
        }
    }
    return 0;
}
//功能1:从键盘输入学生信息存入结构数组，可按照学号依次输入从本人开始的5名同学信息
void func1(STUDENT *info){
    int i;
    for(i=0;i<NUM;i++){
        fgets(info[i].id,10,stdin);
        fgets(info[i].name,10,stdin);
        fgets(info[i].age,4,stdin);
    }
    return;
}
//功能2:输入命令可将结构数组中的数据将保存在input.dat文件中
void func2(FILE *fp,STUDENT *info){
    int i;
    for(i=0;i<NUM;i++){
        fputs(info[i].id,fp);
        fputs(info[i].name,fp);
        fputs(info[i].age,fp);
    }
    fclose(fp);
    return;
}
//功能3:输入命令可将信息从input.dat文件中读入到结构数字，然后反向顺序输出到output.dat文件中
void func3(FILE *fp){
    int i;
    FILE *fpOut;
    STUDENT temp[NUM];
    //先读取input.dat里的内容，把数据倒序存进结构数组中
    for(i=NUM-1;i>=0;i--){
        fgets(temp[i].id,10,fp);
        fgets(temp[i].name,10,fp);
        fgets(temp[i].age,4,fp);
    }
    //再把结构数组里面的内容写进output.dat
    fpOut=fopen("D:/C/DataStructure/Lab1/dat/output.dat","a");
    func2(fpOut,temp);
    return;
}