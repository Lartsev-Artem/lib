#ifndef GALOIS_FIELD_H
#define GALOIS_FIELD_H

#include <stdint.h>

namespace galois
{
typedef int field_symbol;

class field
{
public:

	field(const unsigned int  pwr, const unsigned int* primitive_poly);
	~field();

	inline field_symbol alpha(const field_symbol value) const
	{
		return alpha_to_[value];
	}

	inline unsigned int size() const
	{
		return field_size_;
	}

	inline field_symbol mul(const field_symbol a, const field_symbol b) const
	{
		if ((a == 0) || (b == 0))
		{
			return 0;
		}
		else
		{
			return alpha_to_[__normalize(index_of_[a] + index_of_[b])];
		}
	}

	inline field_symbol div(const field_symbol a, const field_symbol b) const
	{
		if ((a == 0) || (b == 0))
		{
			return 0;
		}
		else
		{
			return alpha_to_[__normalize(index_of_[a] - index_of_[b] + field_size_)];
		}
	}

	inline field_symbol exp(const field_symbol a, int n) const
	{
		if (a != 0)
		{
			if (n < 0)
			{
				while (n < 0)
				{
					n += field_size_;
				}

				return (n ? alpha_to_[__normalize(index_of_[a] * n)] : 1);
			}
			else
				if (n)
				{
					return alpha_to_[__normalize(index_of_[a] * static_cast<field_symbol>(n))];
				}
				else
				{
					return 1;
				}
		}
		else
		{
			return 0;
		}
	}

private:
	inline field();
	inline field(const field& gfield);
	field& operator=(const field& gfield);

	void         __generate_field(const unsigned int* prim_poly_);
	field_symbol __normalize(field_symbol x) const;

	const unsigned int   power_;
	const unsigned int   field_size_;
	field_symbol*  alpha_to_;    // aka exponential or anti-log
	field_symbol*  index_of_;    // aka log
};

inline field::field(const unsigned int pwr, const unsigned int* primitive_poly)
	: power_(pwr),
	  field_size_((1 << power_) - 1)
{
	alpha_to_    = new field_symbol [field_size_ + 1];
	index_of_    = new field_symbol [field_size_ + 1];
	__generate_field(primitive_poly);
}

inline field::~field()
{
	if (nullptr !=  alpha_to_)
	{
		delete [] alpha_to_;
		alpha_to_  = nullptr;
	}

	if (nullptr !=  index_of_)
	{
		delete [] index_of_;
		index_of_  = nullptr;
	}
}

inline field_symbol field::__normalize(field_symbol x) const
{
	while (x < 0)
	{
		x += static_cast<field_symbol>(field_size_);
	}

	while (x >= static_cast<field_symbol>(field_size_))
	{
		x -= static_cast<field_symbol>(field_size_);
		x  = (x >> power_) + (x & field_size_);
	}

	return x;
}

inline void field::__generate_field(const unsigned int* prim_poly)
{
	/*
	   Note: It is assumed that the degree of the primitive
	         polynomial will be equivelent to the m value as
	         in GF(2^m)
	*/

	field_symbol mask = 1;

	alpha_to_[power_] = 0;

	for (field_symbol i = 0; i < static_cast<field_symbol>(power_); ++i)
	{
		alpha_to_[i]            = mask;
		index_of_[alpha_to_[i]] = i;

		if (prim_poly[i] != 0)
		{
			alpha_to_[power_] ^= mask;
		}

		mask <<= 1;
	}

	index_of_[alpha_to_[power_]] = power_;

	mask >>= 1;

	for (field_symbol i = power_ + 1; i < static_cast<field_symbol>(field_size_); ++i)
	{
		if (alpha_to_[i - 1] >= mask)
		{
			alpha_to_[i] = alpha_to_[power_] ^ ((alpha_to_[i - 1] ^ mask) << 1);
		}
		else
		{
			alpha_to_[i] = alpha_to_[i - 1] << 1;
		}

		index_of_[alpha_to_[i]] = i;
	}

	index_of_[0] = -1; //GFERROR;
	alpha_to_[field_size_] = 1;
}

} // namespace galois

#endif //