#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void initialize();                              //初始化函数
void sign_up();                                 //管理员注册函数
void sign_in();                                 //管理员登录
void MenuPanel();                               //管理面板
int find_uid(char[]);                           //根据学号查找uid
void draw_menu();                               //绘制菜单
_Bool is_conflict(char [][50]);                 //判断录入的学号以及姓名是否与已有数据冲突
void show_information(int);                     //给一个uid，然后输出该uid信息
void update_data();                             //将修改后数据写入文件

typedef struct student {
    int uid;            //注册id，即第一个为1，第二个为2，以此类推，与学号区分开来，可以保证查找的时候不会重复
    char name[10];      //名字
    char number[20];    //学号，没错，学号用字符数组，不然用整型的话可能会超过取值范围
    char college[50];   //二级学院
    char class[30];     //班级
} Student;
typedef struct administrator {
    char account[20];   //账户
    char password[20];  //密码
} Administrator;
Student students[100];
int student_amount = 0;
Administrator admin;

int main() {
    initialize();      //进行初始化
    return 0;
}

void initialize() {
    FILE *ftp1, *ftp2;
    ftp1 = fopen("D:\\admin.txt", "r+");
    ftp2 = fopen("D:\\data.txt", "r+");      //创建两个文件指针，分别指向存储管理员数据跟学生数据的文本
    if (ftp1 == NULL)                                        //如果管理员数据无法打开，则进行注册
        sign_up();
    else {
        if (ftp2 != NULL) {                                     //如果管理员文件跟学生数据文件都存在，则开始将数据从文件读取到变量里
            fscanf(ftp2, "%d\n", &student_amount);    //先读取学生个数，方便for循环读取具体数据
            for (int i = 0; i < student_amount; i++) {              //根据学生个数，逐行将数据读入结构体
                fscanf(ftp2, "%d %s %s %s %s\n", &students[i].uid, students[i].name, students[i].number,
                       students[i].college, students[i].class);
            }
            fclose(ftp2);                                   //完成读取后，关闭指针，释放文件占用
        }
        fscanf(ftp1, "%s %s\n", admin.account, admin.password);  //读取管理员账户密码
        fclose(ftp1);                                                   //同样是关闭指针
        sign_in();                                                      //数据加载完成，开始登录
    }
}

void sign_up() {
    printf("未注册管理员账户\n请输入管理员账户进行注册:");
    FILE *ftp;
    ftp = fopen("D:\\admin.txt", "w+");              //创建admin文件，并将指针指向这个文件
    char account[20], password[20], temp[20];
    scanf("%s", account);
    printf("请输入密码：");
    while (1) {                                                      //循环读取两次密码，当两次密码相同时，跳出循环，否则一直读取（把用户当傻子）
        scanf("%s", temp);
        printf("请再次输入密码:");
        scanf("%s", password);
        if (strcmp(temp, password) == 0)
            break;
        else printf("两次密码错误，请重新输入管理员密码：");
    }
    printf("注册完成，进入登录界面\n");
    strcpy(admin.account, account);
    strcpy(admin.password, password);                            //将读入的信息写入结构体
    fprintf(ftp, "%s %s", account, password);              //将结构体数据写入文件，防止程序中途意外关闭造成的数据丢失
    fopen("D:\\data.txt", "w+");                 //建立学生数据文件
    student_amount = 0;                                           //将学生数量设置为0
    fclose(ftp);
    sign_in();                                                  //引导至登陆界面
}

void sign_in() {
    printf("欢迎使用学生管理系统，请输入管理员账户：");
    char account[20], password[20];
    scanf("%s", account);
    printf("请输入管理员密码：");
    scanf("%s", password);
    if (strcmp(admin.account, account) == strcmp(admin.password, password)) {   //判断账户与密码是否匹配，匹配则引导至管理界面，不匹配则退出
        printf("登录成功\n");
        MenuPanel();
    } else {
        printf("管理员账户或密码错误，程序退出");
        exit(0);
    }
}

void MenuPanel() {
    //system("cls");
    while (1) {                                         //死循环，直到用户退出再结束程序
        draw_menu();                                   //绘制界面
        int choice = 0;
        scanf("%d", &choice);                  //等待用户输入操作
        switch (choice) {
            case 1:                                                     //查看学生信息
            {
                char temp[20];
                printf("请输入学生学号，或输入1查看全部学生数据");
                scanf("%s", temp);
                if (strcmp(temp, "1") == 0)                             //当用户选择查看全部时
                    for (int i = 0; i < student_amount; i++)
                        show_information(i);
                else {                                                  //当用户选择查看具体学生时
                    int i = find_uid(temp);                             //调用查找函数
                    if (i == -1) {                                      //当查找函数返回-1则没有这个人
                        printf("查无此人\n");
                    } else                                              //返回其他值则按照返回的uid进行输出信息
                        show_information(i);
                }
                break;                                                  //查找学生完成，跳出
            }
            case 2: {                                                   //新增学生信息
                char tem[10][50];
                char tips[][30] = {"请输入学生姓名：", "请输入学生学号:", "请输入学生所在学院：", "请输入学生所在班级\n"};
                while (1) {
                    for (int i = 0; i < 4; i++) {                      //将提示信息与存放信息的地方弄成数组，则可以通过循环遍历省去代码量
                        printf("%s", tips[i]);
                        scanf("%s", tem[i]);
                    }
                    if (!is_conflict(tem)) {                           //如果输入的学号或者姓名不冲突，则将录入的数据写入结构体变量
                        students[student_amount].uid = student_amount + 1;
                        strcpy(students[student_amount].name, tem[0]);
                        strcpy(students[student_amount].number, tem[1]);
                        strcpy(students[student_amount].college, tem[2]);
                        strcpy(students[student_amount].class, tem[3]);
                        student_amount++;
                        break;
                    } else {                                            //有冲突则询问是否重新录入或退出
                        printf("录入的数据与已有数据冲突，请重新录入或退出:1.再次录入 2.退出");
                        scanf("%d", &choice);
                        if (choice == 2)
                            break;
                    }
                }
                update_data();
                break;
            }
            case 3: {                                                   //修改学生信息
                char tem[50];
                int choices;
                int i;
                while (1) {
                    printf("请输入学生学号:");
                    scanf("%s", tem);
                    if ((i = find_uid(tem)) != -1) {                    //用输入的学号查找对应的uid，找到则开始修改
                        show_information(i);
                        printf("1.姓名，2.学号，3.学院 4.班级\n请输入要修改的选项:");
                        scanf("%d", &choices);
                        printf("请输入要修改的值");
                        scanf("%s", tem);
                        switch (choices) {
                            case 1:
                                strcpy(students[i].name, tem);
                                break;
                            case 2:
                                strcpy(students[i].number, tem);
                                break;
                            case 3:
                                strcpy(students[i].college, tem);
                                break;
                            case 4:
                                strcpy(students[i].class, tem);
                                break;
                            default:
                                printf("选择错误，退出修改\n");
                                break;
                        }
                        break;
                    } else {                                                    //找不到则询问用户下一步操作
                        printf("查无此人，退出或重新查询？\n1.重新查询 2.退出");
                        scanf("%d", &choices);
                        if (choices == 2)
                            break;
                    }
                }
                update_data();
                break;
            }
            case 4: {
                char tem[50];
                int uid, choices;
                while (1) {
                    printf("请输入删除学生的学号:");
                    scanf("%s", tem);
                    if ((uid = find_uid(tem)) != -1) {                                              //查找用户uid
                        show_information(uid);
                        printf("是否确认删除此学生信息？\n1.是 任意值.否");
                        scanf("%d", &choices);
                        if (choices == 1) {
                            for (int i = uid; i < student_amount - 1; i++) {
                                strcpy(students[i].name, students[i + 1].name);
                                strcpy(students[i].number, students[i + 1].number);
                                strcpy(students[i].college, students[i + 1].college);
                                strcpy(students[i].class, students[i + 1].class);
                                /*
                                 * ”删除“原理：先确定要删除的下标，然后将下标后一位的数据复制到这个位置，以此类推，将后面的数据全部这样复制到前一个
                                 * 最后长度student_amount减1，完成”删除“
                                 * 因为全部涉及到结构体变量访问的操作都是用student_amount进行控制的，所以将该变量控制到你想要的长度，就能完成”删除“
                                 * 比如 a={1,2,3,4} a_amount=4 删除a[1]
                                 * 删除后数组为 a={1,3,4,4} a_amount=3,忽略最后一个
                                 * */
                            }
                            student_amount--;
                            break;
                        }
                    } else {                                                                     //找不到则询问用户下一步操作
                        printf("查无此人，退出或重新删除？\n1.重新查询 任意值.退出");
                        scanf("%d", &choices);
                        if (choices != 1)
                            break;
                    }

                }
                update_data();
                break;
            }
            default:
                exit(0);
        }
    }
}

int find_uid(char number[20]) {
    for (int i = 0; i < student_amount; i++) {
        if (strcmp(students[i].number, number) == 0) {       //将输入的学号与结构体变量逐一比较，配对则返回下标
            return i;
        }
    }
    return -1;                                               //如果找不到配对的，则返回-1
}

void draw_menu() {
    for (int i = 0; i < 80; i++) {
        printf("*");
    }
    printf("\n\n\t\t\t\t1.查看学生数据");
    printf("\n\n\t\t\t\t2.新增学生数据");
    printf("\n\n\t\t\t\t3.修改学生数据");
    printf("\n\n\t\t\t\t4.删除学生数据");
    printf("\n\n请输入你的操作(任意值退出)：");
}

_Bool is_conflict(char tem[][50]) {
    for (int i = 0; i < student_amount; i++) {
        if (strcmp(students[i].name, tem[0]) == 0 || strcmp(students[i].number, tem[1]) == 0)  //如果学号或姓名相同则返回1（真）
            return 1;
    }
    return 0;                                                                                  //找不到相同的则返回0（假
}

void show_information(int i) {
    printf("%d %s %s %s %s\n", students[i].uid, students[i].name, students[i].number,
           students[i].college, students[i].class);
}

void update_data() {
    FILE *ftp = fopen("D:\\data.txt","w+");
    fprintf(ftp,"%d\n",student_amount);
    for(int i=0;i<student_amount;i++){
        fprintf(ftp, "%d %s %s %s %s\n",students[i].uid, students[i].name, students[i].number,
                students[i].college, students[i].class);
    }
    fclose(ftp);
}
