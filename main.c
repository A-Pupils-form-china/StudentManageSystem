#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void initialize();                              //��ʼ������
void sign_up();                                 //����Աע�ắ��
void sign_in();                                 //����Ա��¼
void MenuPanel();                               //�������
int find_uid(char[]);                           //����ѧ�Ų���uid
void draw_menu();                               //���Ʋ˵�
_Bool is_conflict(char [][50]);                 //�ж�¼���ѧ���Լ������Ƿ����������ݳ�ͻ
void show_information(int);                     //��һ��uid��Ȼ�������uid��Ϣ
void update_data();                             //���޸ĺ�����д���ļ�

typedef struct student {
    int uid;            //ע��id������һ��Ϊ1���ڶ���Ϊ2���Դ����ƣ���ѧ�����ֿ��������Ա�֤���ҵ�ʱ�򲻻��ظ�
    char name[10];      //����
    char number[20];    //ѧ�ţ�û��ѧ�����ַ����飬��Ȼ�����͵Ļ����ܻᳬ��ȡֵ��Χ
    char college[50];   //����ѧԺ
    char class[30];     //�༶
} Student;
typedef struct administrator {
    char account[20];   //�˻�
    char password[20];  //����
} Administrator;
Student students[100];
int student_amount = 0;
Administrator admin;

int main() {
    initialize();      //���г�ʼ��
    return 0;
}

void initialize() {
    FILE *ftp1, *ftp2;
    ftp1 = fopen("D:\\admin.txt", "r+");
    ftp2 = fopen("D:\\data.txt", "r+");      //���������ļ�ָ�룬�ֱ�ָ��洢����Ա���ݸ�ѧ�����ݵ��ı�
    if (ftp1 == NULL)                                        //�������Ա�����޷��򿪣������ע��
        sign_up();
    else {
        if (ftp2 != NULL) {                                     //�������Ա�ļ���ѧ�������ļ������ڣ���ʼ�����ݴ��ļ���ȡ��������
            fscanf(ftp2, "%d\n", &student_amount);    //�ȶ�ȡѧ������������forѭ����ȡ��������
            for (int i = 0; i < student_amount; i++) {              //����ѧ�����������н����ݶ���ṹ��
                fscanf(ftp2, "%d %s %s %s %s\n", &students[i].uid, students[i].name, students[i].number,
                       students[i].college, students[i].class);
            }
            fclose(ftp2);                                   //��ɶ�ȡ�󣬹ر�ָ�룬�ͷ��ļ�ռ��
        }
        fscanf(ftp1, "%s %s\n", admin.account, admin.password);  //��ȡ����Ա�˻�����
        fclose(ftp1);                                                   //ͬ���ǹر�ָ��
        sign_in();                                                      //���ݼ�����ɣ���ʼ��¼
    }
}

void sign_up() {
    printf("δע�����Ա�˻�\n���������Ա�˻�����ע��:");
    FILE *ftp;
    ftp = fopen("D:\\admin.txt", "w+");              //����admin�ļ�������ָ��ָ������ļ�
    char account[20], password[20], temp[20];
    scanf("%s", account);
    printf("���������룺");
    while (1) {                                                      //ѭ����ȡ�������룬������������ͬʱ������ѭ��������һֱ��ȡ�����û���ɵ�ӣ�
        scanf("%s", temp);
        printf("���ٴ���������:");
        scanf("%s", password);
        if (strcmp(temp, password) == 0)
            break;
        else printf("������������������������Ա���룺");
    }
    printf("ע����ɣ������¼����\n");
    strcpy(admin.account, account);
    strcpy(admin.password, password);                            //���������Ϣд��ṹ��
    fprintf(ftp, "%s %s", account, password);              //���ṹ������д���ļ�����ֹ������;����ر���ɵ����ݶ�ʧ
    fopen("D:\\data.txt", "w+");                 //����ѧ�������ļ�
    student_amount = 0;                                           //��ѧ����������Ϊ0
    fclose(ftp);
    sign_in();                                                  //��������½����
}

void sign_in() {
    printf("��ӭʹ��ѧ������ϵͳ�����������Ա�˻���");
    char account[20], password[20];
    scanf("%s", account);
    printf("���������Ա���룺");
    scanf("%s", password);
    if (strcmp(admin.account, account) == strcmp(admin.password, password)) {   //�ж��˻��������Ƿ�ƥ�䣬ƥ����������������棬��ƥ�����˳�
        printf("��¼�ɹ�\n");
        MenuPanel();
    } else {
        printf("����Ա�˻���������󣬳����˳�");
        exit(0);
    }
}

void MenuPanel() {
    //system("cls");
    while (1) {                                         //��ѭ����ֱ���û��˳��ٽ�������
        draw_menu();                                   //���ƽ���
        int choice = 0;
        scanf("%d", &choice);                  //�ȴ��û��������
        switch (choice) {
            case 1:                                                     //�鿴ѧ����Ϣ
            {
                char temp[20];
                printf("������ѧ��ѧ�ţ�������1�鿴ȫ��ѧ������");
                scanf("%s", temp);
                if (strcmp(temp, "1") == 0)                             //���û�ѡ��鿴ȫ��ʱ
                    for (int i = 0; i < student_amount; i++)
                        show_information(i);
                else {                                                  //���û�ѡ��鿴����ѧ��ʱ
                    int i = find_uid(temp);                             //���ò��Һ���
                    if (i == -1) {                                      //�����Һ�������-1��û�������
                        printf("���޴���\n");
                    } else                                              //��������ֵ���շ��ص�uid���������Ϣ
                        show_information(i);
                }
                break;                                                  //����ѧ����ɣ�����
            }
            case 2: {                                                   //����ѧ����Ϣ
                char tem[10][50];
                char tips[][30] = {"������ѧ��������", "������ѧ��ѧ��:", "������ѧ������ѧԺ��", "������ѧ�����ڰ༶\n"};
                while (1) {
                    for (int i = 0; i < 4; i++) {                      //����ʾ��Ϣ������Ϣ�ĵط�Ū�����飬�����ͨ��ѭ������ʡȥ������
                        printf("%s", tips[i]);
                        scanf("%s", tem[i]);
                    }
                    if (!is_conflict(tem)) {                           //��������ѧ�Ż�����������ͻ����¼�������д��ṹ�����
                        students[student_amount].uid = student_amount + 1;
                        strcpy(students[student_amount].name, tem[0]);
                        strcpy(students[student_amount].number, tem[1]);
                        strcpy(students[student_amount].college, tem[2]);
                        strcpy(students[student_amount].class, tem[3]);
                        student_amount++;
                        break;
                    } else {                                            //�г�ͻ��ѯ���Ƿ�����¼����˳�
                        printf("¼����������������ݳ�ͻ��������¼����˳�:1.�ٴ�¼�� 2.�˳�");
                        scanf("%d", &choice);
                        if (choice == 2)
                            break;
                    }
                }
                update_data();
                break;
            }
            case 3: {                                                   //�޸�ѧ����Ϣ
                char tem[50];
                int choices;
                int i;
                while (1) {
                    printf("������ѧ��ѧ��:");
                    scanf("%s", tem);
                    if ((i = find_uid(tem)) != -1) {                    //�������ѧ�Ų��Ҷ�Ӧ��uid���ҵ���ʼ�޸�
                        show_information(i);
                        printf("1.������2.ѧ�ţ�3.ѧԺ 4.�༶\n������Ҫ�޸ĵ�ѡ��:");
                        scanf("%d", &choices);
                        printf("������Ҫ�޸ĵ�ֵ");
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
                                printf("ѡ������˳��޸�\n");
                                break;
                        }
                        break;
                    } else {                                                    //�Ҳ�����ѯ���û���һ������
                        printf("���޴��ˣ��˳������²�ѯ��\n1.���²�ѯ 2.�˳�");
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
                    printf("������ɾ��ѧ����ѧ��:");
                    scanf("%s", tem);
                    if ((uid = find_uid(tem)) != -1) {                                              //�����û�uid
                        show_information(uid);
                        printf("�Ƿ�ȷ��ɾ����ѧ����Ϣ��\n1.�� ����ֵ.��");
                        scanf("%d", &choices);
                        if (choices == 1) {
                            for (int i = uid; i < student_amount - 1; i++) {
                                strcpy(students[i].name, students[i + 1].name);
                                strcpy(students[i].number, students[i + 1].number);
                                strcpy(students[i].college, students[i + 1].college);
                                strcpy(students[i].class, students[i + 1].class);
                                /*
                                 * ��ɾ����ԭ����ȷ��Ҫɾ�����±꣬Ȼ���±��һλ�����ݸ��Ƶ����λ�ã��Դ����ƣ������������ȫ���������Ƶ�ǰһ��
                                 * ��󳤶�student_amount��1����ɡ�ɾ����
                                 * ��Ϊȫ���漰���ṹ��������ʵĲ���������student_amount���п��Ƶģ����Խ��ñ������Ƶ�����Ҫ�ĳ��ȣ�������ɡ�ɾ����
                                 * ���� a={1,2,3,4} a_amount=4 ɾ��a[1]
                                 * ɾ��������Ϊ a={1,3,4,4} a_amount=3,�������һ��
                                 * */
                            }
                            student_amount--;
                            break;
                        }
                    } else {                                                                     //�Ҳ�����ѯ���û���һ������
                        printf("���޴��ˣ��˳�������ɾ����\n1.���²�ѯ ����ֵ.�˳�");
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
        if (strcmp(students[i].number, number) == 0) {       //�������ѧ����ṹ�������һ�Ƚϣ�����򷵻��±�
            return i;
        }
    }
    return -1;                                               //����Ҳ�����Եģ��򷵻�-1
}

void draw_menu() {
    for (int i = 0; i < 80; i++) {
        printf("*");
    }
    printf("\n\n\t\t\t\t1.�鿴ѧ������");
    printf("\n\n\t\t\t\t2.����ѧ������");
    printf("\n\n\t\t\t\t3.�޸�ѧ������");
    printf("\n\n\t\t\t\t4.ɾ��ѧ������");
    printf("\n\n��������Ĳ���(����ֵ�˳�)��");
}

_Bool is_conflict(char tem[][50]) {
    for (int i = 0; i < student_amount; i++) {
        if (strcmp(students[i].name, tem[0]) == 0 || strcmp(students[i].number, tem[1]) == 0)  //���ѧ�Ż�������ͬ�򷵻�1���棩
            return 1;
    }
    return 0;                                                                                  //�Ҳ�����ͬ���򷵻�0����
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
