#pragma once

//模板类需要放在同一个头文件里面，否则实现单独一个文件的话会报 无法解析的外部符号 错误

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
	int curIdx; //当前指针
	int stockSize; //当前数量
	const int capacity; //最大容量
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
		obj = stock[curIdx]; //必须T支持operator=才行
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
	if (curIdx != size() - 1) //在中间位置执行添加，先删除中间位置之后的存储
	{
		for (int i = 0; i != size() - curIdx - 1; ++i)
			stock.pop_back();
		stockSize -= (stockSize - curIdx - 1);
		++stockSize;
		curIdx++;
		stock.push_back(obj);
	}
	else//常规添加(尾部)
	{
		if (size() > capacity - 1) //达到了最大存储数目
		{
			stock.pop_front(); //因为这里用到了pop_front所以需要用deque，vector不行
			stock.push_back(obj);
		}
		else //常规添加且未达到最大数目
		{
			++stockSize;
			++curIdx;
			stock.push_back(obj);
		}
	}
	return size();
}