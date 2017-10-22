// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{	
	if(len<0)
		throw len;
	BitLen=len;
	MemLen=(len/(8*sizeof (TELEM)))+1;
	pMem=new TELEM[MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	//pMem=new TELEM [MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	int a;
	a=(int) n/(8*sizeof(TELEM));
	return a;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM a;
	a=1;
	a=a<<n%sizeof TELEM ;
	return a;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	int c = BitLen;
	return c;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n<0)||(n>BitLen))
		throw n;
	int i = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[i]|=mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n<0)||(n>BitLen))
		throw n;
	int i = GetMemIndex(n);
	TELEM mask =~GetMemMask(n);
	pMem[i]&=mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n<0)||(n>BitLen))
		throw n;
	int i = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	mask&=pMem[i];
	if (mask==0)
		return 0;
	else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{	
	if (MemLen!=bf.MemLen)
	{
		delete []pMem;
		MemLen=bf.MemLen;
		pMem=new TELEM [MemLen];
	}
	BitLen=bf.BitLen;
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen!=bf.BitLen)
		return 0;
	for (int i=0; i<MemLen-1; i++)
		if (bf.pMem[i]!=this->pMem[i])
			return 0;
	for (int i=BitLen-1; i>=BitLen - BitLen%32; i--)
		if (bf.GetBit(i)!=this->GetBit(i))
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
		//if (BitLen==bf.BitLen)
		//{
			TBitField temp(BitLen);
			for (int i=0; i<MemLen; i++)
				temp.pMem[i]=pMem[i]| bf.pMem[i];
			return temp;
		//}
		/*else 
		{   
			int c=bf.BitLen;
			int max, flag;
			if (BitLen>c)
			{
				max=BitLen;
				flag=1;
			}
			else 
			{ 
				max=c;
				flag=0;
			}
			TBitField temp(max);
			if (flag==1)
			{
				for (int i=0; i<bf.MemLen-1; i++)
					temp.pMem[i]=pMem[i]|bf.pMem[i];
				for (int i=bf.MemLen; i<MemLen; i++)
					temp.pMem[i]=pMem[i];
			}
			else
			{
				for (int i=0; i<MemLen-1; i++)
					temp.pMem[i]=pMem[i]|bf.pMem[i];
				for (int i=MemLen; i<bf.MemLen; i++)
					temp.pMem[i]=bf.pMem[i];
			}
		}*/
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField temp(BitLen);
	for (int i=0; i<MemLen; i++)
		temp.pMem[i]=pMem[i]&bf.pMem[i];
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for (int i=0; i<MemLen; i++)
		temp.pMem[i]=~pMem[i];
	return temp;
}

// ввод/вывод
#include <string>
using namespace std;
istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string st;
	istr>>st;
	for (int i=0; i<st.size(); i++)
	{
		if ((st[i]!='0')&&(st[i]!='1')) break;
		if (st[i]=='1') bf.SetBit(1);
		else bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i=0; i<bf.BitLen; i++)
		if (bf.GetBit(i)) ostr<<1;
		else ostr<<0;
		return ostr;
}
