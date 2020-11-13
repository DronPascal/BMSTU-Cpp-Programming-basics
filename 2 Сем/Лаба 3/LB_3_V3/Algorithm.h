#pragma once
template <class _IterType, class _Pred>
_IterType FindIf(_IterType beg, _IterType end, _Pred p_Pred)
{
	for (; beg != end; ++beg)
		if (p_Pred(*beg))
			return beg;
	return beg;
}

template <class _IterType>
_IterType MinElement(_IterType beg, _IterType end)
{
	auto p_Min = *beg;
	_IterType p_MinPointer = beg;
	++beg;
	for (; beg != end; ++beg)
		if (*beg < p_Min)
		{
			p_Min = *beg;
			p_MinPointer = beg;
		}
	return p_MinPointer;
}

template <class _IterType>
_IterType MaxElement(_IterType beg, _IterType end)
{
	auto p_Max = *beg;
	_IterType p_MaxPointer = beg;
	++beg;
	for (; beg != end; ++beg)
		if (*beg > p_Max)
		{
			p_Max = *beg;
			p_MaxPointer = beg;
		}
	return p_MaxPointer;
}

template <class _Iter, class _Func>
void ForEach(_Iter beg, _Iter end, _Func p_op)
{
	for (; beg != end; ++beg)
		p_op(*beg);
}

template < class _Iter>
void Sort(_Iter beg, _Iter end)
{
	auto tmp = *beg;
	for (; end != beg; --end)
		for (_Iter Iter = beg; Iter != end; ++Iter)
		{
			auto cur = *Iter;
			if (cur < *(++Iter))
			{
				--Iter;
				tmp = *Iter;
				auto dat = *(++Iter);
				*(--Iter) = dat;
				*(++Iter) = tmp;
			}
			--Iter;
		}
}

/*
void Sort(_Iter beg, _Iter end)
{
	_Iter p_Head = beg;
	_Iter p_Tail = end - 1;
	auto tmp = *beg;
	for (; end != beg; --end)
		for (_Iter p_Iter = p_Head; p_Iter != p_Tail; ++p_Iter)
			if (*p_Iter < *(p_Iter + 1))
			{
				tmp = *p_Iter;
				*p_Iter = *(p_Iter + 1);
				*(p_Iter + 1) = tmp;
			}
}
*/

template <class _Iter1, class _Iter2, class _Pred>
void CopyIf(_Iter1 sourceBeg, _Iter1 sourceEnd, _Iter2 destBeg, _Pred p_Pred)
{
	for (; sourceBeg != sourceEnd; ++sourceBeg)
	{
		if (p_Pred(*sourceBeg))
		{
			*destBeg = *sourceBeg;
			destBeg++;
		}
	}
}