#include "libBase.h"

libBase::libBase()
{};

libBase::~libBase()
{};

const bool libBase::is_equal_to(const int& another_number) const
{
	return another_number == this->variable;
}