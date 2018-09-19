#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>

#include <vector>
#include <fstream>
#include <fstream>
using namespace std;

#define QINT_BITS 128
#define INT_BITS 32
#define QINT_SIZES 4

//Trong mã ASCII, kí tự = số + 48


// QInt 16 byte = 128 bit		
// Phạm vi biểu diễn :					-2^127	:	(2^127) - 1 
// -170141183460469231731687303715884105728		:	170141183460469231731687303715884105727

struct QInt
{
	// kiểu int 4 bytes, mảng 4 phần tử => 16 bytes
	int data[QINT_SIZES];
};

QInt operator + (const QInt &a, const QInt &b);
QInt operator << (const QInt &a, int n);
QInt operator >> (const QInt &a, int n);

// cho tất cả bit QInt = 0		
void SetAllBitTo0(QInt &x)
{
	// thiết lập toàn bộ 128 bit là 0
	for (int i = 0; i < QINT_SIZES; i++)
	{
		x.data[i] = 0;
	}
}


// cho tất cả bit QInt = 1		
void SetAllBitTo1(QInt &x)
{
	// thiết lập toàn bộ 128 bit là 1
	for (int i = 0; i < QINT_SIZES; i++)
	{
		x.data[i] = -1;
	}
}


// Gán giá trị 1 tại bit thứ i có thể suy ra từ pos		
void SetBit(QInt &x, int pos, bool bit)
{
	// phần tử kiểu int 4 byte => 32 bit
	if (bit == 1)
	{
		x.data[QINT_SIZES - 1 - pos / 32] = (1 << (pos % 32)) | x.data[QINT_SIZES - 1 - pos / 32];
	}
}


// Gán giá trị 0 tại bit thứ i có thể suy ra từ pos		
void SetBit0(QInt &x, int pos, bool bit)
{
	if (bit == 0)
	{
		x.data[QINT_SIZES - 1 - pos / 32] = (~(1 << (pos % 32))) & x.data[QINT_SIZES - 1 - pos / 32];
	}
}


// Lấy giá trị tại bit thứ i có thể suy ra từ pos		
bool GetBit(const QInt &x, int pos)
{
	return (bool)((x.data[QINT_SIZES - 1 - pos / 32] >> (pos % 32)) & 1);
}


// xóa đi những số 0 thừa ở đầu (số có dấu) - dạng thập phân	
string RemoveExTraNumberZero(string &x)
{

	int i;
	if (x[0] == '-' || x[0] == '+')
		i = 1;	// i = 0 là dấu nên không xét
	else
		i = 0;

	int n = 0;	// số lượng số 0
	int L = x.length();
	if (L > 2)
		while (x[i] == '0' && i < L - 1)		// không xóa số 0 cuối cùng (nếu có)
		{
			n++;
			i++;
		}
	x.erase(1, n);
	return x;
}


// hàm chia 1 dãy số nguyên có dấu dạng chuỗi cho 2		
string StringDivide2(string s)
{
	string res = "";
	int i = 0;		// chỉ số của chuỗi s

	if (s[i] == '-' || s[i] == '+')
	{
		res += s[i++];		// gán dấu của s qua chuỗi kết quả;
	}

	int n = s.length();
	int temp = 0;

	while (i < n)
	{
		if (temp != 0)
		{
			temp *= 10;
		}

		if ((s[i] - 48) < 2 && temp == 0)
		{
			res += '0';
			temp = (s[i] - 48) * 10;
			i++;
		}

		if (i < n)
		{
			temp += (s[i] - 48);
			res += (temp / 2) + 48;
			temp = temp % 2;
			i++;
		}
	}
	RemoveExTraNumberZero(res);
	return res;
}


// Hàm nhân 1 dãy số nguyên không dấu dạng chuỗi với 2	
string StringMult2(string s)
{
	string res = "";
	int n = s.length();
	int mem = 0;	// biến nhớ
	char num;
	for (int i = n - 1; i >= 0; i--)	// Nhân tử phải qua 
	{
		int temp = (s[i] - 48) * 2 + mem;
		if (temp > 9)
		{
			mem = 1;		// nhân 2 => biến nhớ chỉ có thể là 1
			num = (temp % 10) + 48;	// lấy hàng đơn vị đổi số sang kí tự

		}
		else
		{
			mem = 0;
			num = temp + 48;	// đổi số sang kí tự
		}
		res = num + res;	// nối chuỗi, số sau bên trái số trước
	}

	if (mem == 1)
	{
		res = "1" + res;
	}

	return res;
}


// Hàm tính 2 lũy thừa dương x của 1 số dạng chuỗi		
string TwoPowerX(int x)
{
	string res = "1";
	// 2 lũy thừa x => 2*2*2...*2 x lần
	for (int i = 1; i <= x; i++)
	{
		res = StringMult2(res);		// res = res * 2;
	}
	return res;
}


// Hàm tính tổng 2 số thập phân dạng chuỗi		
string StringAddString(string a, string b)
{
	string sum;

	// trường hợp 2 số dương + nhau
	if (a[0] != '-')
	{
		int len1 = a.length();
		int len2 = b.length();
		int maxL = len1 < len2 ? len2 : len1;	// tìm kích thước chuỗi dài hơn trong 2 chuỗi

		sum.resize(maxL);
		// Gán bit 0 vào chuỗi ngắn hơn
		if (len1 < maxL)
		{
			a.resize(maxL);
			a.insert(a.begin(), maxL - len1, '0');
		}
		else if (len2 < maxL)
		{
			b.resize(maxL);
			b.insert(b.begin(), maxL - len2, '0');
		}

		// tính tổng 2 chuỗi
		int mem = 0;	// biến nhớ
		int value1, value2;	// biến dùng để lưu giá trị số thứ i của a và b
		int temp;

		for (int i = maxL - 1; i >= 0; i--)
		{
			value1 = a[i] - 48;
			value2 = b[i] - 48;
			temp = value1 + value2 + mem;

			if (temp > 9)
			{
				mem = 1;
				sum[i] = (temp % 10) + 48;
			}
			else
			{
				mem = 0;
				sum[i] = temp + 48;
			}
		}

		if (mem == 1)
		{
			sum = '1' + sum;
		}

		return sum;
	}

	// trường hợp 1 trong 2 số là số âm
	// trong hàm này chỉ xét trường hợp số âm đó là -2^127, nằm ở chuỗi a
	else				// (a[0] == '-')
	{
		a.erase(a.begin());	// xóa đi dấu -

		int maxL = a.length();
		sum.resize(maxL);
		b.insert(b.begin(), maxL - b.length(), '0');

		// tính a - b
		int mem = 0;	// biến nhớ
		int value1, value2;	// biến dùng để lưu giá trị số thứ i của a và b
		int temp;
		for (int i = maxL - 1; i >= 0; i--)
		{
			value1 = a[i] - 48;
			value2 = b[i] - 48;
			temp = value1 - value2 - mem;
			if (temp < 0)
			{
				mem = 1;
				temp += 10;
			}
			else
			{
				mem = 0;
			}
			sum[i] = temp + 48;
		}

		// kết quả là 1 số âm
		return ("-" + sum);

	}
}


// Đảo bit => Số bù 1		
QInt ReverseBit(const QInt &x)
{
	QInt res;
	for (int i = 0; i < QINT_SIZES; i++)
	{
		res.data[i] = ~x.data[i];
	}
	return res;
}


// Số bù 2 = Số bù 1 + 1		
QInt TwoComplement(const QInt &x)
{
	QInt comp1, temp;

	comp1 = ReverseBit(x);

	SetAllBitTo0(temp);
	SetBit(temp, 0, 1);	// x.data[3] = 1;

						// cộng số bù 1 với 1 
	return comp1 + temp;
}


// Hàm chuyển đổi số dạng chuỗi về kiểu QInt		
QInt StringToQInt(string num)
{
	QInt x;
	SetAllBitTo0(x);	// Set 128 bits = 0

						// i là vị trí đếm từ phải qua bắt đầu từ 0 => 0 là vị trí bên phải đầu tiên
	int i = 0;
	while (num != "+0" && num != "-0")
	{
		int bit = (num[num.length() - 1] - 48) % 2;	// lấy phần dư, phần dư này do số cuối cùng quyết định
		SetBit(x, i, bit);	// gán phần dư vào QInt
		num = StringDivide2(num);	// chia 2 chuỗi
		i++;	// di chuyển qua bit kế tiếp (bên trái)
	}

	// nếu là số âm, chuyển số vừa đổi được sang bù 2
	if (num[0] == '-')
	{
		x = TwoComplement(x);
	}

	return x;
}


// Hàm xuất số nhị phân		
void PrintBin(bool *x, ostream &os)
{
	int i;
	// trường hợp số dương phía có thể không cần in số 0 dư phía trước, còn số âm phải in đủ
	for (i = 0; i < QINT_BITS - 1; i++)
		if (x[i] != 0)
			break;

	for (; i < QINT_BITS; i++)
	{
		os << x[i];
	}
	os << endl;
}


// Hàm in số dạng thập lục		
void PrintHex(char *x, ostream& os)
{
	int i;
	int SizeHex = QINT_BITS / 4;

	// trường hợp số dương phía có thể không cần in số 0 dư phía trước, còn số âm phải in đủ
	for (i = 0; i < SizeHex - 1; i++)
		if (x[i] != '0')
			break;

	for (; i < SizeHex; i++)
	{
		os << x[i];
	}
	os << endl;
}


// Dịch trái 2 biến A,Q
void ShiftLeftAQ(QInt &A, QInt &Q)
{
	for (int i = QINT_BITS - 1; i > 0; i--)
	{
		if (GetBit(A, i - 1) == 1)
			SetBit(A, i, 1);
		else
			SetBit0(A, i, 0);
	}

	if (GetBit(Q, 127) == 1)
		SetBit(A, 0, 1);
	else
		SetBit0(A, 0, 0);

	for (int i = QINT_BITS - 1; i > 0; i--)
	{
		if (GetBit(Q, i - 1) == 1)
			SetBit(Q, i, 1);
		else
			SetBit0(Q, i, 0);
	}
	SetBit0(Q, 0, 0);
}


// Hàm chuyển đổi QInt thập lục sang QInt thập phân
QInt HexToDec(char* Hex)
{
	QInt res;

	int n = strlen(Hex);

	SetAllBitTo0(res);


	int k = 0;
	for (int i = n - 1; i >= 0; i--)
	{
		switch (Hex[i])
		{
		case '0':	// 0000
			k += 4;
			break;
		case '1':	// 0001
			SetBit(res, k, 1);
			k += 4;
			break;
		case '2':	// 0010
			k++;
			SetBit(res, k, 1);
			k += 3;
			break;
		case '3':	// 0011
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k += 3;
			break;
		case '4':	// 0100
			k += 2;
			SetBit(res, k, 1);
			k += 2;
			break;
		case '5':	// 0101
			SetBit(res, k, 1);
			k += 2;
			SetBit(res, k, 1);
			k += 2;
			break;
		case '6':	// 0110
			k++;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k += 2;
			break;
		case '7':	// 0111
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k += 2;
			break;
		case '8':	// 1000
			k += 3;
			SetBit(res, k, 1);
			k++;
			break;
		case '9':	// 1001
			SetBit(res, k, 1);
			k += 3;
			SetBit(res, k, 1);
			k++;
			break;
		case 'A':	// 1010
			k++;
			SetBit(res, k, 1);
			k += 2;
			SetBit(res, k, 1);
			k++;
			break;
		case 'B':	// 1011
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k += 2;
			SetBit(res, k, 1);
			k++;
			break;
		case 'C':	// 1100
			k += 2;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			break;
		case 'D':	// 1101
			SetBit(res, k, 1);
			k += 2;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			break;
		case 'E':	// 1110
			k++;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			break;
		case 'F':	// 1111
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			SetBit(res, k, 1);
			k++;
			break;
		}
	}

	return res;
}


// Hàm nhập số nhị phân
void ScanBin(bool *&Bin, string a)
{
	/*string a;
	fflush(stdin);
	getline(cin, a);*/
	int n = a.length();
	a.resize(QINT_BITS);

	a.insert(a.begin(), QINT_BITS - n, '0');
	Bin = (bool*)(malloc(QINT_BITS * sizeof(bool)));
	for (int i = 0; i < QINT_BITS; i++)
		Bin[i] = a[i] - 48;

}


// Hàm nhập số thập lục
void ScanHex(char *&Hex, string a)
{
	//string a;
	//fflush(stdin);
	//getline(cin, a);

	int n = a.length();
	int SizeHex = QINT_BITS / 4;
	a.resize(SizeHex);

	a.insert(a.begin(), SizeHex - n, '0');
	Hex = (char*)(malloc(SizeHex * sizeof(char)));
	for (int i = 0; i < SizeHex; i++)
		Hex[i] = a[i];

}

// a. Hàm nhập		
void ScanQInt(QInt &x, string a)
{
	//string a;
	//fflush(stdin);
	//getline(cin, a);	// nhập số dưới dạng chuỗi

	if (a[0] != '-' && a[0] != '+')	// nếu không phải là số âm và người dùng không nhập dấu +
	{
		a = "+" + a;	// thêm dấu + vào số vừa nhập
	}
	x = StringToQInt(a);	// đổi số kiểu chuỗi sang kiểu QInt
}


// b. Hàm xuất	
void PrintQInt(const QInt &x, ostream& os)
{
	string res = "0";

	for (int i = 0; i < QINT_BITS - 1; i++)	// không xét bit dầu tiên
	{
		if (GetBit(x, i) == 1)
		{
			res = StringAddString(res, TwoPowerX(i));
		}
	}

	// nếu là số âm thì - 2^127 
	if (GetBit(x, 127) == 1)
	{
		string temp = "-" + TwoPowerX(127);
		res = StringAddString(temp, res);
	}

	RemoveExTraNumberZero(res);
	os << res << endl;
}


// c. Hàm chuyển đổi QInt thập phân sang nhị phân		
bool* DecToBin(const QInt &x)
{
	bool *Bin;
	Bin = (bool*)(malloc(QINT_BITS * sizeof(bool)));
	for (int i = 0; i < QINT_BITS; i++)
	{
		Bin[QINT_BITS - 1 - i] = GetBit(x, i);
	}

	return (Bin);
}


// d. Hàm chuyển đổi QInt nhị phân sang thập phân		
QInt BinToDec(bool* Bin)
{
	QInt Dec;

	SetAllBitTo0(Dec);

	for (int i = 0; i < QINT_BITS; i++)
		SetBit(Dec, i, Bin[QINT_BITS - 1 - i]);

	return Dec;
}


// e. Hàm chuyển đối số QInt sang thập lục		
char* BinToHex(bool* Bin)
{
	char *Hex;
	int SizeHex = QINT_BITS / 4;
	Hex = (char*)(malloc(SizeHex * sizeof(char) + 1));

	int i = 0;
	for (int j = 0; j < SizeHex; j++)
	{
		char temp = 0;
		for (int k = 4 - 1; k >= 0; k--)
			temp += (char)Bin[i++] * pow(2.0, k);

		if (temp < 10)
		{
			Hex[j] = temp + 48;
		}
		else
		{
			switch (temp)
			{
			case 10:
				Hex[j] = 'A';
				break;
			case 11:
				Hex[j] = 'B';
				break;
			case 12:
				Hex[j] = 'C';
				break;
			case 13:
				Hex[j] = 'D';
				break;
			case 14:
				Hex[j] = 'E';
				break;
			case 15:
				Hex[j] = 'F';
				break;
			}
		}
	}
	Hex[SizeHex] = '\0';
	return Hex;
}


// f. Hàm chuyển đổi số QInt thập phân sang thập lục		
char* DecToHex(QInt x)
{
	char *Hex;

	bool *Bin = DecToBin(x);

	Hex = BinToHex(Bin);

	free(Bin);
	return Hex;
}


// g. Các operator toán tử +, -, *, /

// Phép + 2 số kiểu QInt
// thuật toán cộng có dấu và không dấu là như nhau
QInt operator + (const QInt &a, const QInt &b)
{
	QInt sum; // biến chứa kết quả
	SetAllBitTo0(sum);	// gán 128 bit = 0

						// tiến hành cộng 2 số nhị phân, cộng từ phải qua trái

	bool memBit = 0;	// biến dùng để lưu bit nhớ
	bool value1, value2;	// biến dùng để lưu giá trị bit thứ i của a và b
	char temp;

	for (int i = 0; i <= QINT_BITS - 1; i++)	// i là vị trí đếm từ trái qua bắt đầu từ 0 => 127 là vị trí bên phải đầu tiên
	{
		value1 = GetBit(a, i);
		value2 = GetBit(b, i);
		temp = memBit + value1 + value2;

		// temp = 3 => gán bit 1 và nhớ 1
		if (temp == 3)
		{
			SetBit(sum, i, 1);	// gán bit 1 vào vị trí suy ra từ i
			memBit = 1;
		}
		// temp = 2 => gán bit 0 và nhớ 1 
		else if (temp == 2)
		{
			memBit = 1;
		}
		// temp = 1 => gán bit 1 và nhớ 0
		else if (temp == 1)
		{
			SetBit(sum, i, 1);
			memBit = 0;
		}
		// temp = 0 => gán bit 0 và nhớ 0
		else if (temp == 0)
		{
			memBit = 0;
		}
	}

	return sum;
}


// Phép - 2 số kiểu QInt
// Tìm cách chuyển về phép +, các trường hợp khi tính a - b:
// 1) Số bị trừ (b) là số dương => a - b => a + bù2(b)
// 2) Số bị trừ (b) là số âm => a - (-b) = a + b => chuyển b từ dạng bù 2 sang số dương rồi lấy a + b 
QInt operator - (const QInt &a, const QInt &b)
{
	QInt res, temp, b2;

	SetAllBitTo1(temp);

	// Th1 : b là số dương
	if (GetBit(b, 127) == 0)
	{
		// tìm số bù 2 của b
		b2 = TwoComplement(b);
	}
	// Th2 : b là số âm
	else
	{
		// tìm số b ở dạng dương
		// trừ đi 1 để về dạng bù 1
		b2 = b + temp;
		// đảo bit
		b2 = ReverseBit(b2);
	}

	res = a + b2;
	return res;
}


//// Phép * 2 số kiểu QInt
QInt operator * (const QInt&x, const QInt &y)
{
	QInt A, Q, M;
	bool Q1;

	SetAllBitTo0(A);	// khởi tạo A = 0
	Q = x;
	M = y;
	Q1 = 0;

	for (int k = QINT_BITS; k > 0; k--)
	{
		if (GetBit(Q, 0) == 1 && Q1 == 0)
		{
			A = A - M;
		}
		else if (GetBit(Q, 0) == 0 && Q1 == 1)
		{
			A = A + M;
		}

		// SHR [A,Q,Q1]
		Q1 = GetBit(Q, 0);			// Gán bit cuối Q qua Q1
		Q = Q >> 1;							// Dịch phải Q
		if (GetBit(A, 0) == 1)		// Gán bit cuối A qua Q
			SetBit(Q, 127, 1);
		else
			SetBit0(Q, 127, 0);
		A = A >> 1;							// Dịch phải A
	}

	return Q;
}


//// Phép / 2 số kiểu QInt (a / b) <=> (Q / M)
QInt operator / (const QInt&a, const QInt &b)
{
	bool sign = 0;
	QInt A, Q, M, temp;
	Q = a;
	M = b;

	SetAllBitTo1(temp);
	SetAllBitTo0(A);

	if (GetBit(Q, 127) == 1 && GetBit(M, 127) == 1)
	{
		Q = Q + temp;
		Q = ReverseBit(Q);

		M = M + temp;
		M = ReverseBit(M);
	}

	else if (GetBit(Q, 127) == 1 && GetBit(M, 127) == 0)
	{
		sign = 1;
		Q = Q + temp;
		Q = ReverseBit(Q);
	}

	else if (GetBit(Q, 127) == 0 && GetBit(M, 127) == 1)
	{
		sign = 1;
		M = M + temp;
		M = ReverseBit(M);
	}

	for (int k = QINT_BITS; k > 0; k--)
	{
		// Shift left [A,Q]
		ShiftLeftAQ(A, Q);

		A = A - M;

		if (GetBit(A, 127) == 1)
		{
			SetBit0(Q, 0, 0);	// gán bit cuối Q = 0
			A = A + M;
		}
		else
		{
			SetBit(Q, 0, 1);	// gán bit cuối Q = 1
		}
	}

	// 1 trong hai số là số âm
	if (sign == 1)
	{
		Q = TwoComplement(Q);
	}

	return Q;
}


// h. Các toán tử And(&) - Or(|) - Xor(^) - Not(~)

// Phép And(&)
QInt operator & (const QInt &a, const QInt &b)
{
	QInt res;
	SetAllBitTo0(res);
	for (int i = 0; i < QINT_BITS; i++)
	{
		bool temp;
		temp = GetBit(a, i) & GetBit(b, i);
		SetBit(res, i, temp);
	}
	return res;
}


// Phép Or (|)
QInt operator | (const QInt &a, const QInt &b)
{
	QInt res;
	SetAllBitTo0(res);
	for (int i = 0; i < QINT_BITS; i++)
	{
		bool temp;
		temp = GetBit(a, i) | GetBit(b, i);
		SetBit(res, i, temp);
	}
	return res;
}


// Phép Xor (^)
QInt operator ^ (const QInt &a, const QInt &b)
{
	QInt res;
	SetAllBitTo0(res);
	for (int i = 0; i < QINT_BITS; i++)
	{
		bool temp;
		temp = GetBit(a, i) ^ GetBit(b, i);
		SetBit(res, i, temp);
	}
	return res;
}


// Phép Not (~)
QInt operator ~ (const QInt &a)
{
	return ReverseBit(a);
}


// i. Các toán tử dịch trái << và dịch phải >> 
// Do QInt là số có dấu nên phép dịch ở đây là dịch số học

// Toán tử dịch trái (<<)		
QInt operator << (const QInt &a, int n)
{
	QInt res;
	SetAllBitTo0(res);
	SetBit(res, 127, GetBit(a, 127));

	// dịch bit
	for (int i = QINT_BITS - 2; i >= 0; i--)
	{
		bool temp;
		if (i - n >= 0)
		{
			temp = GetBit(a, i - n);
		}
		else
		{
			temp = 0;
		}
		if (temp == 1)
			SetBit(res, i, 1);
		else
			SetBit0(res, i, 0);
	}
	return res;
}


// Toán tử dịch phải (>>)		
QInt operator >> (const QInt &a, int n)
{
	bool sign = GetBit(a, 127);

	// lưu lại bit dấu
	QInt res;
	SetAllBitTo0(res);
	SetBit(res, 127, sign);

	// dịch bit
	for (int i = 0; i < QINT_BITS - 1; i++)
	{
		bool temp;
		if (i + n < QINT_BITS)
		{
			temp = GetBit(a, i + n);
		}
		else
		{
			temp = sign;
		}
		if (temp == 1)
			SetBit(res, i, 1);
		else
			SetBit0(res, i, 0);
	}

	return res;
}


vector<string> Strtok_Space(string str)
{
	char *sdup = _strdup(str.c_str());
	char *tmp = NULL;
	vector<string> res;
	tmp = strtok(sdup, " ");
	if (tmp)
		res.push_back((string)tmp);
	while (tmp = strtok(NULL, " "))
		res.push_back((string)tmp);
	delete[] sdup;
	return res;
}


void Export(string tmp, ostream& os)
{

	QInt q1, q2, res;
	vector<string> token = Strtok_Space(tmp);


	int type = stoi(token[0]); //Chuyển loại hệ sang số nguyên

	if (token[2] == "+" || token[2] == "-" || token[2] == "*" || token[2] == "/" || token[2] == "&" || token[2] == "|" || token[2] == "^")
	{
		switch (type)
		{
		case 2:
			bool* bin1, *bin2;
			ScanBin(bin1, token[1]);
			q1 = BinToDec(bin1);
			ScanBin(bin2, token[3]);
			q2 = BinToDec(bin2);
			break;
		case 10:
			ScanQInt(q1, token[1]);
			ScanQInt(q2, token[3]);
			break;
		case 16:
			char *hex1, *hex2;
			ScanHex(hex1, token[1]);
			q1 = HexToDec(hex1);
			ScanHex(hex2, token[3]);
			q2 = HexToDec(hex2);
			break;
		default:
			exit(0);
		}
		if (token[2] == "+")
		{
			res = q1 + q2;
		}
		else if (token[2] == "-")
		{
			res = q1 - q2;
		}
		else if (token[2] == "*")
		{
			res = q1 * q2;
		}
		else if (token[2] == "/")
		{
			res = q1 / q2;
		}
		else if (token[2] == "&")
		{
			res = q1 & q2;
		}
		else if (token[2] == "|")
		{
			res = q1 | q2;
		}
		else if (token[2] == "^")
		{
			res = q1 ^ q2;
		}

		switch (type)
		{
		case 2:
			PrintBin(DecToBin(res), os);
			break;
		case 10:
			PrintQInt(res, os);
			break;
		case 16:
			PrintHex(DecToHex(res), os);
			break;
		default:
			exit(0);
		}
	}

	else if (token[1] == "~")
	{
		switch (type)
		{
		case 2:
			bool *bin;
			ScanBin(bin, token[2]);
			res = BinToDec(bin);
			res = ~res;
			PrintBin(DecToBin(res), os);
			break;
		case 10:
			res = ~res;
			PrintQInt(res, os);
			break;
		case 16:
			char* hex;
			ScanHex(hex, token[2]);
			res = HexToDec(hex);
			res = ~res;
			PrintHex(DecToHex(res), os);
			break;
		default:
			exit(0);
		}
	}
	else if (token[2] == "<<" || token[2] == ">>")	//<<, >>
	{
		int n = stoi(token[3]);
		switch (type)
		{
		case 2:
			bool *x;
			ScanBin(x, token[1]);
			res = BinToDec(x);
			break;
		case 10:
			ScanQInt(res, token[1]);
			break;
		case 16:
			char *hex;
			ScanHex(hex, token[1]);
			res = HexToDec(hex);
			break;
		default:
			exit(0);
		}
		if (token[2] == ">>")
		{
			res = res >> n;
		}
		else if (token[2] == "<<")
		{
			res = res << n;
		}
		switch (type)
		{
		case 2:
			PrintBin(DecToBin(res), os);
			break;
		case 10:
			PrintQInt(res, os);
			break;
		case 16:
			PrintHex(DecToHex(res), os);
			break;
		default:
			exit(0);
		}
	}
	else	//Chuyển đổi hệ
	{
		int type2 = stoi(token[1]);
		if (type != type2)
		{
			switch (type)
			{
			case 2:
				bool *bin;
				ScanBin(bin, token[2]);
				if (type2 == 10)
				{
					res = BinToDec(bin);
					PrintQInt(res, os);
				}
				else if (type2 == 16)
				{
					char* hex_res;
					hex_res = BinToHex(bin);
					PrintHex(hex_res, os);
				}
				break;

			case 10:
				ScanQInt(q1, token[2]);
				if (type2 == 2)
				{
					bool* bin_res = DecToBin(q1);
					PrintBin(bin_res, os);
				}
				else if (type2 == 16)
				{
					char* hex_res;
					hex_res = DecToHex(q1);
					PrintHex(hex_res, os);
				}
				break;

			case 16:
				char* hex;
				ScanHex(hex, token[2]);
				if (type2 == 10)
				{
					res = HexToDec(hex);
					PrintQInt(res, os);
				}
				else if (type2 == 2)
				{
					bool* bin_res;
					res = HexToDec(hex);
					bin_res = DecToBin(res);
					PrintBin(bin_res, os);
				}
				break;
			default:
				exit(0);
			}
		}
	}

}

void ImportFromFile(vector<string> &line, string inputName)
{
	ifstream fIN;
	fIN.open(inputName, ios::in);
	if (fIN.fail())
	{
		cout << "Input not found!" << endl;
	}

	string tmp;

	while (!fIN.eof())
	{
		getline(fIN, tmp);
		if (tmp != "")
			line.push_back(tmp);
	}

	fIN.close();
}

void ExportToFile(string exp, string outputName)
{
	ofstream fOUT;
	fOUT.open(outputName, ios::app);
	Export(exp, fOUT);
	fOUT.close();
}

int main(int argc, char *argv[])
{
	//while (true)
	//{
	system("cls");
	cout << "\t\tDO AN 1" << endl;
	cout << "\tXu li so nguyen lon" << endl;
	//cout << "Menu:\n\t 1. Input tu file" << endl;
	//cout << "\t 2. Input tu ban phim" << endl;
	//cout << "Lua chon: ";

	//int choose = 0;
	//cin >> choose;
	//cin.ignore();

	string tmp;

	//switch (choose)
	//{
	//case 1:				////Nhập xuất qua file
	//{
	vector<string> line;

	ImportFromFile(line, argv[1]);

	remove(argv[2]);
	for (auto i : line)
	{
		ExportToFile(i, argv[2]);
	}
	cout << "Thanh cong!" << endl;
	/*	system("pause");*/
	//	break;
	//}
	//case 2:			////Nhập từ bàn phím, Xuất ra màn hình
	//{
	//	cout << "Nhap bieu thuc: " << endl;
	//	getline(cin, tmp);
	//	cout << "Ket qua: ";
	//	Export(tmp, cout);


	//	system("pause");
	//	break;
	//}
	//default:
	//	exit(0);
	//	//}
	//}



	return 0;
}

