#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct HuffNode
{
	unsigned char ch;  //字节符
	long count = 0;  //字节出现频度(权值）
	int parent = -1; //父节点
	int lch = -1;  //左孩子
	int rch = -1;  //右孩子
	char bits[256]; // 哈夫曼编码
};

void Menu();
void ReadFile(HuffNode HuffTreeNode[], char filename[], long *filelength);//读取源文件并统计字节数
void SortHuff(HuffNode HuffTreeNode[], int *count);//对哈夫曼树进行筛选、排序，并统计其中大于0的个数
void CreatHuffTree(HuffNode HuffTreeNode[], int count);//生成哈夫曼树
void CreatHuffCode(HuffNode HuffTreeNode[], int count);//生成哈夫曼编码（只有叶子节点的编码）
void Compress(HuffNode HuffTreeNode[], char filename[], long filelength, int count);//压缩文件
void UnCompress();  //解压文件



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
		char FileName[20];//文件名称
		long FileLength;//文件长度
		int Count;//文件编码个数
		HuffNode HuffTreeNode[512];
		int choice;
		printf("*********************************************************\n");
		printf("*                                                       *\n");
		printf("*                   压缩、解压缩小工具                  *\n");
		printf("*                                                       *\n");
		printf("*********************************************************\n");
		printf("*                                                       *\n");
		printf("*                       1.压缩                          *\n");
		printf("*                       2.解压缩                        *\n");
		printf("*                       3.退出                          *\n");
		printf("*                                                       *\n");
		printf("*********************************************************\n");
		printf("*  说明：（1）采用了哈夫曼编码                          *\n");
		printf("*        （2）适用于位图（.bmp）                        *\n");
		printf("*********************************************************\n");
		printf("\n请输入您想要的功能序号：");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			//压缩文件
			printf("请输入你要压缩的文件名字：");
			scanf("%s", FileName);
			start = clock();
			ReadFile(HuffTreeNode, FileName, &FileLength);
			SortHuff(HuffTreeNode, &Count);
			CreatHuffCode(HuffTreeNode, Count);
			Compress(HuffTreeNode, FileName, FileLength, Count);
			end = clock();
			printf("文件压缩所用时间为：%d毫秒！\n", end - start);
			system("pause");
			break;
		case 2:
			//解压文件
			UnCompress();
			system("pause");
			break;
		case 3:
			//退出程序
			exit(0);
			break;
		default:
			printf("没有此功能，请重新选择！\n");
			break;
		}
		system("cls");
	}
}

void ReadFile(HuffNode HuffTreeNode[], char filename[], long *filelength)
{
	FILE *in = fopen(filename, "rb");
	unsigned char str;      //接收读取的一个字节
	(*filelength) = 0;		//初始化文件长度为0
	if (in == NULL)
	{
		printf("\n\t文件打开失败！\n\n");
	}
	while (!feof(in))  //文件没到达末尾则循环
	{
		fread(&str, 1, 1, in);
		HuffTreeNode[(int)str].count++;  //把与str同值的数组下标所在的元素权值加一。
		HuffTreeNode[(int)str].ch = str;  //把字符存入哈夫曼树
		(*filelength)++;   //统计原文件的长度，每读取一个字节加一
	}
	fclose(in);
}

void SortHuff(HuffNode HuffTreeNode[], int *count)
{
	int i, j;
	int temp;
	long tempcount;
	unsigned char tempstr;
	(*count) = 0;		//初始化文件编码个数为0
	for (i = 0; i < 256; i++)//统计count
	{
		if (HuffTreeNode[i].count > 0)
			(*count)++;
	}
	for (i = 0; i < (*count); i++)//筛选出大于0编码，放在前Count位，初步排序（按次数从大到小） 使用了简单选择排序
		                          //采用简单选择排序，将HuffTreeNode数组前256个元素按权值从大到小排序，从而找到有权值的元素个数，记录叶子节点的个数，得到哈夫曼树的总结点数。
	{
		temp = i;
		for (j = i + 1; j < 256; j++)
			if (HuffTreeNode[j].count > HuffTreeNode[temp].count)
				temp = j;
		if (temp != i)   //交换下标为i,j的count值和ch值
		{
			tempcount = HuffTreeNode[i].count;
			HuffTreeNode[i].count = HuffTreeNode[temp].count;
			HuffTreeNode[temp].count = tempcount;
			tempstr = HuffTreeNode[i].ch;
			HuffTreeNode[i].ch = HuffTreeNode[temp].ch;
			HuffTreeNode[temp].ch = tempstr;
		}
	}
	for (i = 0; i < (*count) - 1; i++)//再次排序（频率大为主，字符小为次）
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
		for (j = 0; j < i; j++)//选出当前未建树最小的count
		{
			if (HuffTreeNode[j].parent != -1)//parent!=-1说明该节点已在哈夫曼树中
				continue;
			if (mincount > HuffTreeNode[j].count)
			{
				minbottom = j;
				mincount = HuffTreeNode[j].count;
			}
		}
		//上面已选出当前未建树最小的count，把它设为哈夫曼树的左孩子，并更新相关信息
		HuffTreeNode[i].count = mincount;
		HuffTreeNode[i].lch = minbottom;
		HuffTreeNode[minbottom].parent = i;


		//下面再选出当前未建树最小的count，把它设为哈夫曼树的右孩子，并更新相关信息
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
		HuffTreeNode[i].count += mincount;  //新节点权值为两个叶子节点权值之和
		HuffTreeNode[i].rch = minbottom;
		HuffTreeNode[minbottom].parent = i;
	}
}

void CreatHuffCode(HuffNode HuffTreeNode[], int count)
{
	int i, tempchild, tempparent;
	for (i = 0; i < count; i++)//对各个节点进行编码
	{
		tempchild = i;//初始化孩子节点位置
		HuffTreeNode[i].bits[0] = '\0';//先将节点置为NULL
		while (HuffTreeNode[tempchild].parent != -1)//还未到根节点
		{
			tempparent = HuffTreeNode[tempchild].parent;//记录父亲节点位置
			if (HuffTreeNode[tempparent].lch == tempchild)//如果为左孩子
			{
				memmove((HuffTreeNode[i].bits) + 1, HuffTreeNode[i].bits, strlen(HuffTreeNode[i].bits) + 1);//编码整体后移，空出第0位
				HuffTreeNode[i].bits[0] = '0';//将编码第0位置为1（因为是左孩子）
			}
			else//为右孩子
			{
				memmove((HuffTreeNode[i].bits) + 1, HuffTreeNode[i].bits, strlen(HuffTreeNode[i].bits) + 1);
				HuffTreeNode[i].bits[0] = '1';// 将编码第0位置为1（因为是右孩子）
			}
			tempchild = tempparent;//（逻辑上）节点上移
		}
	}
}

void Compress(HuffNode HuffTreeNode[], char filename[], long filelength, int count)
{
	long comflen = 0;//压缩完的文件长度
	char file[20];//文件名字（无后缀名）
	char suffname[5];//文件后缀名
	char buf[512];//定义缓冲区，暂存哈夫曼编码
	char temphuff;//暂存八位0-1字符转成的十进制数字符
	unsigned char str;
	buf[0] = '\0';//初始化缓冲区
	int i, j, k;
	for (i = 0; filename[i] != '\0'; i++)
	{
		if (filename[i] == '.')
			break;
	}
	for (j = 0; j < i; j++)//获取文件的名字
		file[j] = filename[j];
	file[j] = '\0';
	k = 0;
	for (j = i + 1; filename[j] != '\0'; j++)//获取文件的后缀名
		suffname[k++] = filename[j];
	suffname[k] = '\0';
	FILE *in = fopen(filename, "rb");
	if (!in)
	{
		printf("文件%s.%s打开出错，解压失败！\n", file, suffname);
		return;
	}
	strcat(file, ".huf");
	FILE *out = fopen(file, "wb");
	if (!out)
	{
		printf("文件%s打开出错，解压失败！\n", file);
		return;
	}
	i = strlen(suffname);
	fwrite(&i, sizeof(i), 1, out);//写入头文件长度
	fwrite(&suffname, sizeof(suffname), strlen(suffname), out);//写入文件后缀名
	fwrite(&filelength, sizeof(filelength), 1, out);//写入文件长度
	fwrite(&count, sizeof(count), 1, out);//写入编码个数
	for (i = 0; i < count; i++)//写入编码的值和长度
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
				+ (buf[6] - '0') * 2 + (buf[7] - '0') * 1;//将缓冲buf前八位0-1字符转成十进制
			fwrite(&temphuff, sizeof(temphuff), 1, out);//将其写入到压缩文件
			comflen++;//统计压缩文件的字节数
			memmove(buf, buf + 8, strlen(buf) - 8 + 1);//将缓冲区buf的数据集体往前移动八位
		}
	}
	if (strlen(buf) != 0)//不足八位补0
	{
		for (i = strlen(buf); i < 8; i++)
			buf[i] = '0';
		temphuff = (buf[0] - '0') * 128 + (buf[1] - '0') * 64 + (buf[2] - '0') * 32
			+ (buf[3] - '0') * 16 + (buf[4] - '0') * 8 + (buf[5] - '0') * 4
			+ (buf[6] - '0') * 2 + (buf[7] - '0') * 1;
		fwrite(&temphuff, sizeof(temphuff), 1, out);
		comflen++;
	}
	printf("文件压缩成功！压缩文件名为：%s\n其压缩率为%f%\n", file, (((double)filelength - (double)comflen) / (double)filelength) * 100);
	fclose(in);
	fclose(out);
}

void UnCompress()
{
	time_t start, end;
	HuffNode HuffTreeNode[512];
	long filelength;//压缩文件的长度
	int count;//编码数目
	char uncomfile[20];//要解压的文件名（输出文件）
	char suffname[5];//文件的后缀名
	unsigned char instr;
	int teninstr;
	char buf[512];//缓冲区
	long buflen = 0;//缓冲区长度
	int i, j, k;
	int temp;
	char str;
	printf("请输入你要解压的文件名：");
	scanf("%s", uncomfile);
	start = clock();
	FILE *in = fopen(uncomfile, "rb");
	if (!in)
	{
		printf("文件%s打开出错，解压失败！\n", uncomfile);
		return;
	}
	//读回头部信息
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

	CreatHuffTree(HuffTreeNode, count);//重新建树
	CreatHuffCode(HuffTreeNode, count);//重新进行哈夫曼编码
	strcpy(uncomfile, "uncomfile.");
	strcat(uncomfile, suffname);//给解压文件加上原文件的后缀名
	FILE *out = fopen(uncomfile, "wb");
	if (!out)
	{
		printf("文件%s打开出错，解压失败！\n", uncomfile);
		return;
	}
	while (!feof(in))
	{
		fread(&instr, 1, 1, in);//读取字符
		teninstr = (int)instr;
		for (i = 7; i >= 0; i--)//将读取出来字符转成二进制0-1字符串存到缓冲区里面
		{
			buf[buflen + i] = (teninstr % 2) + '0';
			teninstr /= 2;
		}
		buf[buflen + 8] = '\0';
		buflen = strlen(buf);
		while (buflen > 256)//大于256是因为最坏情况下哈夫曼编码长255位
		{
			i = 2 * count - 2;//指到哈夫曼树根节点对应下标
			j = 0;//缓冲区下标
			while (HuffTreeNode[i].lch != -1 && HuffTreeNode[i].rch != -1)//匹配编码
			{
				if (buf[j] == '0')//为0，则将其指到左孩子
					i = HuffTreeNode[i].lch;
				else			//否则，则将其指到右孩子
					i = HuffTreeNode[i].rch;
				j++;//缓冲区下标右移
			}
			fwrite(&HuffTreeNode[i].ch, sizeof(HuffTreeNode[i].ch), 1, out);//匹配到编码，将其对应的字符写出到文件
			filelength--;//解压一个字符，原文件长度减一（用来判断原文件字符是否解压完）
			memmove(buf, buf + j, strlen(buf) - j + 1);//将缓冲区前移匹配到的编码长度
			buflen = strlen(buf);//更新缓冲区长度
		}
	}
	while (filelength > 0)//写入原文件剩余字符
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
	printf("文件解压成功！解压文件名为：%s\n", uncomfile);
	printf("文件解压所用时间为：%d毫秒！\n", end - start);
}