#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct HuffNode
{
	unsigned char ch;  //�ֽڷ�
	long count = 0;  //�ֽڳ���Ƶ��(Ȩֵ��
	int parent = -1; //���ڵ�
	int lch = -1;  //����
	int rch = -1;  //�Һ���
	char bits[256]; // ����������
};

void Menu();
void ReadFile(HuffNode HuffTreeNode[], char filename[], long *filelength);//��ȡԴ�ļ���ͳ���ֽ���
void SortHuff(HuffNode HuffTreeNode[], int *count);//�Թ�����������ɸѡ�����򣬲�ͳ�����д���0�ĸ���
void CreatHuffTree(HuffNode HuffTreeNode[], int count);//���ɹ�������
void CreatHuffCode(HuffNode HuffTreeNode[], int count);//���ɹ��������루ֻ��Ҷ�ӽڵ�ı��룩
void Compress(HuffNode HuffTreeNode[], char filename[], long filelength, int count);//ѹ���ļ�
void UnCompress();  //��ѹ�ļ�



int main()
{
	Menu();
	return 0;
}




void Menu()
{
	while (true)
	{
		time_t start, end;
		char FileName[20];//�ļ�����
		long FileLength;//�ļ�����
		int Count;//�ļ��������
		HuffNode HuffTreeNode[512];
		int choice;
		printf("*********************************************************\n");
		printf("*                                                       *\n");
		printf("*                   ѹ������ѹ��С����                  *\n");
		printf("*                                                       *\n");
		printf("*********************************************************\n");
		printf("*                                                       *\n");
		printf("*                       1.ѹ��                          *\n");
		printf("*                       2.��ѹ��                        *\n");
		printf("*                       3.�˳�                          *\n");
		printf("*                                                       *\n");
		printf("*********************************************************\n");
		printf("*  ˵������1�������˹���������                          *\n");
		printf("*        ��2��������λͼ��.bmp��                        *\n");
		printf("*********************************************************\n");
		printf("\n����������Ҫ�Ĺ�����ţ�");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			//ѹ���ļ�
			printf("��������Ҫѹ�����ļ����֣�");
			scanf("%s", FileName);
			start = clock();
			ReadFile(HuffTreeNode, FileName, &FileLength);
			SortHuff(HuffTreeNode, &Count);
			CreatHuffCode(HuffTreeNode, Count);
			Compress(HuffTreeNode, FileName, FileLength, Count);
			end = clock();
			printf("�ļ�ѹ������ʱ��Ϊ��%d���룡\n", end - start);
			system("pause");
			break;
		case 2:
			//��ѹ�ļ�
			UnCompress();
			system("pause");
			break;
		case 3:
			//�˳�����
			exit(0);
			break;
		default:
			printf("û�д˹��ܣ�������ѡ��\n");
			break;
		}
		system("cls");
	}
}

void ReadFile(HuffNode HuffTreeNode[], char filename[], long *filelength)
{
	FILE *in = fopen(filename, "rb");
	unsigned char str;      //���ն�ȡ��һ���ֽ�
	(*filelength) = 0;		//��ʼ���ļ�����Ϊ0
	if (in == NULL)
	{
		printf("\n\t�ļ���ʧ�ܣ�\n\n");
	}
	while (!feof(in))  //�ļ�û����ĩβ��ѭ��
	{
		fread(&str, 1, 1, in);
		HuffTreeNode[(int)str].count++;  //����strֵͬ�������±����ڵ�Ԫ��Ȩֵ��һ��
		HuffTreeNode[(int)str].ch = str;  //���ַ������������
		(*filelength)++;   //ͳ��ԭ�ļ��ĳ��ȣ�ÿ��ȡһ���ֽڼ�һ
	}
	fclose(in);
}

void SortHuff(HuffNode HuffTreeNode[], int *count)
{
	int i, j;
	int temp;
	long tempcount;
	unsigned char tempstr;
	(*count) = 0;		//��ʼ���ļ��������Ϊ0
	for (i = 0; i < 256; i++)//ͳ��count
	{
		if (HuffTreeNode[i].count > 0)
			(*count)++;
	}
	for (i = 0; i < (*count); i++)//ɸѡ������0���룬����ǰCountλ���������򣨰������Ӵ�С�� ʹ���˼�ѡ������
		                          //���ü�ѡ�����򣬽�HuffTreeNode����ǰ256��Ԫ�ذ�Ȩֵ�Ӵ�С���򣬴Ӷ��ҵ���Ȩֵ��Ԫ�ظ�������¼Ҷ�ӽڵ�ĸ������õ������������ܽ������
	{
		temp = i;
		for (j = i + 1; j < 256; j++)
			if (HuffTreeNode[j].count > HuffTreeNode[temp].count)
				temp = j;
		if (temp != i)   //�����±�Ϊi,j��countֵ��chֵ
		{
			tempcount = HuffTreeNode[i].count;
			HuffTreeNode[i].count = HuffTreeNode[temp].count;
			HuffTreeNode[temp].count = tempcount;
			tempstr = HuffTreeNode[i].ch;
			HuffTreeNode[i].ch = HuffTreeNode[temp].ch;
			HuffTreeNode[temp].ch = tempstr;
		}
	}
	for (i = 0; i < (*count) - 1; i++)//�ٴ�����Ƶ�ʴ�Ϊ�����ַ�СΪ�Σ�
	{
		temp = i;
		for (j = i + 1; j < (*count); j++)
			if (HuffTreeNode[temp].count == HuffTreeNode[j].count&&HuffTreeNode[temp].ch > HuffTreeNode[j].ch)
				temp = j;
		if (temp != i)
		{
			tempcount = HuffTreeNode[i].count;
			HuffTreeNode[i].count = HuffTreeNode[temp].count;
			HuffTreeNode[temp].count = tempcount;
			tempstr = HuffTreeNode[i].ch;
			HuffTreeNode[i].ch = HuffTreeNode[temp].ch;
			HuffTreeNode[temp].ch = tempstr;
		}
	}
}

void CreatHuffTree(HuffNode HuffTreeNode[], int count)
{
	long mincount;
	int minbottom;
	int i, j;
	for (i = count; i < 2 * count - 1; i++)
	{
		mincount = LONG_MAX;
		for (j = 0; j < i; j++)//ѡ����ǰδ������С��count
		{
			if (HuffTreeNode[j].parent != -1)//parent!=-1˵���ýڵ����ڹ���������
				continue;
			if (mincount > HuffTreeNode[j].count)
			{
				minbottom = j;
				mincount = HuffTreeNode[j].count;
			}
		}
		//������ѡ����ǰδ������С��count��������Ϊ�������������ӣ������������Ϣ
		HuffTreeNode[i].count = mincount;
		HuffTreeNode[i].lch = minbottom;
		HuffTreeNode[minbottom].parent = i;


		//������ѡ����ǰδ������С��count��������Ϊ�����������Һ��ӣ������������Ϣ
		mincount = LONG_MAX;
		for (j = 0; j < i; j++)
		{
			if (HuffTreeNode[j].parent != -1)
				continue;
			if (mincount > HuffTreeNode[j].count)
			{
				minbottom = j;
				mincount = HuffTreeNode[j].count;
			}
		}
		HuffTreeNode[i].count += mincount;  //�½ڵ�ȨֵΪ����Ҷ�ӽڵ�Ȩֵ֮��
		HuffTreeNode[i].rch = minbottom;
		HuffTreeNode[minbottom].parent = i;
	}
}

void CreatHuffCode(HuffNode HuffTreeNode[], int count)
{
	int i, tempchild, tempparent;
	for (i = 0; i < count; i++)//�Ը����ڵ���б���
	{
		tempchild = i;//��ʼ�����ӽڵ�λ��
		HuffTreeNode[i].bits[0] = '\0';//�Ƚ��ڵ���ΪNULL
		while (HuffTreeNode[tempchild].parent != -1)//��δ�����ڵ�
		{
			tempparent = HuffTreeNode[tempchild].parent;//��¼���׽ڵ�λ��
			if (HuffTreeNode[tempparent].lch == tempchild)//���Ϊ����
			{
				memmove((HuffTreeNode[i].bits) + 1, HuffTreeNode[i].bits, strlen(HuffTreeNode[i].bits) + 1);//����������ƣ��ճ���0λ
				HuffTreeNode[i].bits[0] = '0';//�������0λ��Ϊ1����Ϊ�����ӣ�
			}
			else//Ϊ�Һ���
			{
				memmove((HuffTreeNode[i].bits) + 1, HuffTreeNode[i].bits, strlen(HuffTreeNode[i].bits) + 1);
				HuffTreeNode[i].bits[0] = '1';// �������0λ��Ϊ1����Ϊ���Һ��ӣ�
			}
			tempchild = tempparent;//���߼��ϣ��ڵ�����
		}
	}
}

void Compress(HuffNode HuffTreeNode[], char filename[], long filelength, int count)
{
	long comflen = 0;//ѹ������ļ�����
	char file[20];//�ļ����֣��޺�׺����
	char suffname[5];//�ļ���׺��
	char buf[512];//���建�������ݴ����������
	char temphuff;//�ݴ��λ0-1�ַ�ת�ɵ�ʮ�������ַ�
	unsigned char str;
	buf[0] = '\0';//��ʼ��������
	int i, j, k;
	for (i = 0; filename[i] != '\0'; i++)
	{
		if (filename[i] == '.')
			break;
	}
	for (j = 0; j < i; j++)//��ȡ�ļ�������
		file[j] = filename[j];
	file[j] = '\0';
	k = 0;
	for (j = i + 1; filename[j] != '\0'; j++)//��ȡ�ļ��ĺ�׺��
		suffname[k++] = filename[j];
	suffname[k] = '\0';
	FILE *in = fopen(filename, "rb");
	if (!in)
	{
		printf("�ļ�%s.%s�򿪳�����ѹʧ�ܣ�\n", file, suffname);
		return;
	}
	strcat(file, ".huf");
	FILE *out = fopen(file, "wb");
	if (!out)
	{
		printf("�ļ�%s�򿪳�����ѹʧ�ܣ�\n", file);
		return;
	}
	i = strlen(suffname);
	fwrite(&i, sizeof(i), 1, out);//д��ͷ�ļ�����
	fwrite(&suffname, sizeof(suffname), strlen(suffname), out);//д���ļ���׺��
	fwrite(&filelength, sizeof(filelength), 1, out);//д���ļ�����
	fwrite(&count, sizeof(count), 1, out);//д��������
	for (i = 0; i < count; i++)//д������ֵ�ͳ���
	{
		fwrite(&HuffTreeNode[i].ch, sizeof(HuffTreeNode[i].ch), 1, out);
		fwrite(&HuffTreeNode[i].count, sizeof(HuffTreeNode[i].count), 1, out);
	}
	while (!feof(in))
	{
		fread(&str, 1, 1, in);
		for (i = 0; i < count; i++)
			if (HuffTreeNode[i].ch == str)
				break;
		strcat(buf, HuffTreeNode[i].bits);
		while (strlen(buf) >= 8)
		{
			temphuff = (buf[0] - '0') * 128 + (buf[1] - '0') * 64 + (buf[2] - '0') * 32
				+ (buf[3] - '0') * 16 + (buf[4] - '0') * 8 + (buf[5] - '0') * 4
				+ (buf[6] - '0') * 2 + (buf[7] - '0') * 1;//������bufǰ��λ0-1�ַ�ת��ʮ����
			fwrite(&temphuff, sizeof(temphuff), 1, out);//����д�뵽ѹ���ļ�
			comflen++;//ͳ��ѹ���ļ����ֽ���
			memmove(buf, buf + 8, strlen(buf) - 8 + 1);//��������buf�����ݼ�����ǰ�ƶ���λ
		}
	}
	if (strlen(buf) != 0)//�����λ��0
	{
		for (i = strlen(buf); i < 8; i++)
			buf[i] = '0';
		temphuff = (buf[0] - '0') * 128 + (buf[1] - '0') * 64 + (buf[2] - '0') * 32
			+ (buf[3] - '0') * 16 + (buf[4] - '0') * 8 + (buf[5] - '0') * 4
			+ (buf[6] - '0') * 2 + (buf[7] - '0') * 1;
		fwrite(&temphuff, sizeof(temphuff), 1, out);
		comflen++;
	}
	printf("�ļ�ѹ���ɹ���ѹ���ļ���Ϊ��%s\n��ѹ����Ϊ%f%\n", file, (((double)filelength - (double)comflen) / (double)filelength) * 100);
	fclose(in);
	fclose(out);
}

void UnCompress()
{
	time_t start, end;
	HuffNode HuffTreeNode[512];
	long filelength;//ѹ���ļ��ĳ���
	int count;//������Ŀ
	char uncomfile[20];//Ҫ��ѹ���ļ���������ļ���
	char suffname[5];//�ļ��ĺ�׺��
	unsigned char instr;
	int teninstr;
	char buf[512];//������
	long buflen = 0;//����������
	int i, j, k;
	int temp;
	char str;
	printf("��������Ҫ��ѹ���ļ�����");
	scanf("%s", uncomfile);
	start = clock();
	FILE *in = fopen(uncomfile, "rb");
	if (!in)
	{
		printf("�ļ�%s�򿪳�����ѹʧ�ܣ�\n", uncomfile);
		return;
	}
	//����ͷ����Ϣ
	fread(&temp, sizeof(temp), 1, in);
	fread(&suffname, sizeof(suffname), temp, in);
	suffname[strlen(suffname)] = '\0';
	fread(&filelength, sizeof(filelength), 1, in);
	fread(&count, sizeof(count), 1, in);
	for (i = 0; i < count; i++)
	{
		fread(&HuffTreeNode[i].ch, sizeof(HuffTreeNode[i].ch), 1, in);
		fread(&HuffTreeNode[i].count, sizeof(HuffTreeNode[i].count), 1, in);
	}

	CreatHuffTree(HuffTreeNode, count);//���½���
	CreatHuffCode(HuffTreeNode, count);//���½��й���������
	strcpy(uncomfile, "uncomfile.");
	strcat(uncomfile, suffname);//����ѹ�ļ�����ԭ�ļ��ĺ�׺��
	FILE *out = fopen(uncomfile, "wb");
	if (!out)
	{
		printf("�ļ�%s�򿪳�����ѹʧ�ܣ�\n", uncomfile);
		return;
	}
	while (!feof(in))
	{
		fread(&instr, 1, 1, in);//��ȡ�ַ�
		teninstr = (int)instr;
		for (i = 7; i >= 0; i--)//����ȡ�����ַ�ת�ɶ�����0-1�ַ����浽����������
		{
			buf[buflen + i] = (teninstr % 2) + '0';
			teninstr /= 2;
		}
		buf[buflen + 8] = '\0';
		buflen = strlen(buf);
		while (buflen > 256)//����256����Ϊ�����¹��������볤255λ
		{
			i = 2 * count - 2;//ָ�������������ڵ��Ӧ�±�
			j = 0;//�������±�
			while (HuffTreeNode[i].lch != -1 && HuffTreeNode[i].rch != -1)//ƥ�����
			{
				if (buf[j] == '0')//Ϊ0������ָ������
					i = HuffTreeNode[i].lch;
				else			//��������ָ���Һ���
					i = HuffTreeNode[i].rch;
				j++;//�������±�����
			}
			fwrite(&HuffTreeNode[i].ch, sizeof(HuffTreeNode[i].ch), 1, out);//ƥ�䵽���룬�����Ӧ���ַ�д�����ļ�
			filelength--;//��ѹһ���ַ���ԭ�ļ����ȼ�һ�������ж�ԭ�ļ��ַ��Ƿ��ѹ�꣩
			memmove(buf, buf + j, strlen(buf) - j + 1);//��������ǰ��ƥ�䵽�ı��볤��
			buflen = strlen(buf);//���»���������
		}
	}
	while (filelength > 0)//д��ԭ�ļ�ʣ���ַ�
	{
		i = 2 * count - 2;
		j = 0;
		while (HuffTreeNode[i].lch != -1 && HuffTreeNode[i].rch != -1)
		{
			if (buf[j] == '0')
				i = HuffTreeNode[i].lch;
			else
				i = HuffTreeNode[i].rch;
			j++;
		}
		fwrite(&HuffTreeNode[i].ch, sizeof(HuffTreeNode[i].ch), 1, out);
		filelength--;
		memmove(buf, buf + j, strlen(buf) - j + 1);
		buflen = strlen(buf);
	}
	end = clock();
	fclose(in);
	fclose(out);
	printf("�ļ���ѹ�ɹ�����ѹ�ļ���Ϊ��%s\n", uncomfile);
	printf("�ļ���ѹ����ʱ��Ϊ��%d���룡\n", end - start);
}