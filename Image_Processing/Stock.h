#pragma once

//ģ������Ҫ����ͬһ��ͷ�ļ����棬����ʵ�ֵ���һ���ļ��Ļ��ᱨ �޷��������ⲿ���� ����

#include <deque>
using std::deque;

template<class T>
class Stock
{
public:
	Stock(int capacity):capacity(capacity) { stockSize = 0; curIdx = -1; }
	~Stock(void) { stock.clear(); }
	int add(T &obj);
	int getIdx() { return curIdx; }
	const int size() const { return stockSize; }
	bool cur(T &);
	bool pre(T &);
	bool next(T &);
	bool first(T &);
	void clear() { stock.clear(); curIdx = -1; stockSize = 0; }
private:
	deque<T> stock;
	int curIdx; //��ǰָ��
	int stockSize; //��ǰ����
	const int capacity; //�������
};

template<class T>
bool Stock<T>::cur(T &)
{
	if (size() > 0)
	{
		T = stock[curIdx];
		return true;
	}
	return false;
}

template <class T>
bool Stock<T>::pre(T &obj)
{
	if (curIdx <= size() && curIdx > 0)
	{
		curIdx--;
		obj = stock[curIdx]; //����T֧��operator=����
		return true;
	}
	return false;
}

template <class T>
bool Stock<T>::next(T &obj)
{
	if (curIdx < size() - 1)
	{
		curIdx++;
		obj = stock[curIdx];
		//stock[curIdx].CopyTo(img);
		return true;
	}
	return false;

}

template <class T>
bool Stock<T>::first(T &dst)
{
	if (size() > 0)
	{
		dst = stock[0];
		return true;
		//stock[0].CopyTo(dst);
	}
	return false;
}


template<class T>
int Stock<T>::add(T &obj)
{
	if (curIdx != size() - 1) //���м�λ��ִ����ӣ���ɾ���м�λ��֮��Ĵ洢
	{
		for (int i = 0; i != size() - curIdx - 1; ++i)
			stock.pop_back();
		stockSize -= (stockSize - curIdx - 1);
		++stockSize;
		curIdx++;
		stock.push_back(obj);
	}
	else//�������(β��)
	{
		if (size() > capacity - 1) //�ﵽ�����洢��Ŀ
		{
			stock.pop_front(); //��Ϊ�����õ���pop_front������Ҫ��deque��vector����
			stock.push_back(obj);
		}
		else //���������δ�ﵽ�����Ŀ
		{
			++stockSize;
			++curIdx;
			stock.push_back(obj);
		}
	}
	return size();
}