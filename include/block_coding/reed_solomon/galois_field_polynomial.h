#ifndef GALOIS_FIELD_POLYNOMIAL_H
#define GALOIS_FIELD_POLYNOMIAL_H

#include "dbgdef.h"
#include "galois_field.h"
#include "symbol_vector.h"

namespace galois
{

class field_polynomial
{
public:

	field_polynomial(const field& gfield);
	field_polynomial(const field& gfield, const unsigned int& degree, const field_symbol element[] = nullptr);
	field_polynomial(const field_polynomial& polynomial);
	field_polynomial(const field_symbol element, const field& gfe);
	~field_polynomial() {}

	void operator  =  (const field_polynomial& polynomial);
	void operator +=  (const field_polynomial& element);
	void operator +=  (const field_symbol element);
	void operator *=  (const field_polynomial& polynomial);
	void operator *=  (const field_symbol element);
	void operator /=  (const field_symbol element);
	void operator %=  (const field_polynomial& divisor);
	void operator %=  (const unsigned int power);
	void operator ^=  (const unsigned int n);
	void operator <<= (const unsigned int n);
	field_symbol operator()(field_symbol value) const;

	field_symbol& operator[] (const uint32_t& term)
	{
		return poly_[term];
	}
	const field_symbol& operator[](const uint32_t& term) const
	{
		return poly_[term];
	}

	int deg() const
	{
		return  static_cast<int>(poly_.size()) - 1;
	}
	void derivative(field_polynomial& res) const;

private:
	void simplify();
	const field& field_;
	symbol_vector<field_symbol> poly_;
};


inline field_polynomial::field_polynomial(const field& gfield)
	: field_(const_cast<field&>(gfield))
{
}

inline field_polynomial::field_polynomial(const field& gfield, const unsigned int& degree, const field_symbol element[])
	: field_(const_cast<field&>(gfield))
{
	poly_.resize(degree + 1, 0);

	if (element != nullptr)
	{
		/*
		   It is assumed that element is an array of field elements
		   with size/element count of degree + 1.
		*/
		for (unsigned int i = 0; i <= degree; ++i)
		{
			poly_[i]  = (element[i]);
		}
	}
}

inline field_polynomial::field_polynomial(const field_polynomial& polynomial)
	: field_(const_cast<field&>(polynomial.field_)),
	  poly_(polynomial.poly_.size())

{
	memcpy(poly_.data(), polynomial.poly_.data(), poly_.size() * sizeof(poly_[0]));
}

inline field_polynomial::field_polynomial(const field_symbol element, const field& gfe)
	: field_(const_cast<field&>(gfe))
{
	poly_.resize(1, element);
}


inline void field_polynomial::operator = (const field_polynomial& polynomial)
{
	uint32_t n = polynomial.poly_.size();
	poly_.resize(n, polynomial.poly_[0]);
	memcpy(poly_.data(), polynomial.poly_.data(), n * sizeof(poly_[0]));
}

inline void field_polynomial::operator += (const field_polynomial& polynomial)
{
	if (poly_.size() < polynomial.poly_.size())
	{
		uint32_t old_size =  poly_.size();
		poly_.resize(polynomial.poly_.size(), 0);

		for (uint32_t i = 0 ; i < old_size; i++)
		{
			poly_[i] ^= polynomial.poly_[i];
		}

		for (uint32_t i = old_size ; i < poly_.size(); i++)
		{
			poly_[i] = polynomial.poly_[i];
		}
	}
	else
	{
		for (uint32_t i = 0; i < polynomial.poly_.size(); ++i)
		{
			poly_[i] ^= polynomial.poly_[i];
		}
	}

	simplify();
}

inline void field_polynomial::operator += (const field_symbol element)
{
	poly_[0] ^= element;
}

inline void field_polynomial::operator *= (const field_polynomial& polynomial)
{
	field_polynomial product(field_, deg() + polynomial.deg() + 1);

	for (uint32_t i = 0; i < poly_.size(); ++i)
	{
		for (uint32_t j = 0; j < polynomial.poly_.size(); ++j)
		{
			product[i + j] ^= field_.mul(poly_[i], polynomial.poly_[j]);
		}
	}

	product.simplify();
	poly_.move(product.poly_);
}

inline void field_polynomial::operator *= (const field_symbol element)
{
	for (uint32_t i = 0; i < poly_.size(); ++i)
	{
		poly_[i] = field_.mul(poly_[i],  element);
	}
}

inline void field_polynomial::operator /= (const field_symbol element)
{
	for (uint32_t i = 0; i < poly_.size(); ++i)
	{
		poly_[i] = field_.div(poly_[i], element);
	}
}

inline void field_polynomial::operator %= (const field_polynomial& divisor)
{
	DIE_IF(this->deg() < divisor.deg());

	int mydeg = this->deg();
	int div_deg = divisor.deg();
	int r_deg = div_deg - 1;
	field_polynomial remainder(field_, r_deg);

	for (int i = 0; i <= r_deg; i++)
	{
		remainder[i] = poly_[mydeg - r_deg + i];
	}

	field_symbol quotient;
	int q_deg = mydeg - div_deg;

	for (int i = q_deg; i >= 0; i--)
	{
		quotient = field_.div(remainder[r_deg], divisor[div_deg]);

		for (int j = r_deg; j > 0; --j)
		{
			remainder[j] = remainder[j - 1] ^ field_.mul(quotient, divisor[j]);
		}

		remainder[0] = poly_[i] ^ field_.mul(quotient, divisor[0]);
	}

	poly_.move(remainder.poly_);

}

inline void field_polynomial::operator %= (const unsigned int power)
{
	if (poly_.size() >= power)
	{
		poly_.resize(power, 0);
		simplify();
	}
}

inline void field_polynomial::operator ^= (const unsigned int n)
{
	field_polynomial result = *this;

	for (uint32_t i = 0; i < n; ++i)
	{
		result *= *this;
	}

	poly_.move(result.poly_);
}

inline void field_polynomial::operator <<= (const unsigned int n)
{
	if (poly_.size() > 0)
	{
		size_t initial_size = poly_.size();

		poly_.resize(poly_.size() + n, 0);

		for (size_t i = initial_size - 1; static_cast<int>(i) >= 0; --i)
		{
			poly_[i + n] = poly_[i];
		}

		for (unsigned int i = 0; i < n; ++i)
		{
			poly_[i] = 0;
		}
	}
}

inline field_symbol field_polynomial::operator () (field_symbol value) const
{
	if (poly_.size())
	{
		field_symbol total_sum = 0 ;

		for (uint32_t i = 0; i < poly_.size(); ++i)
		{
			total_sum ^= field_.mul(field_.exp(value, i), poly_[i]);
		}

		return total_sum;
	}

	return 0;
}

inline void field_polynomial::derivative(field_polynomial& res) const
{
	if (poly_.size() > 1)
	{
		const uint32_t upper_bound = poly_.size() - 1;
		field_polynomial deriv(field_, deg());

		for (uint32_t i = 0; i < upper_bound; i += 2)
		{
			deriv.poly_[i] = poly_[i + 1];
		}

		deriv.simplify();
		res.poly_.move(deriv.poly_);
	}
}


inline void field_polynomial::simplify()
{
	uint32_t poly_size = poly_.size();

	if ((poly_size > 0) && (poly_[poly_size - 1] == 0))
	{
		const field_symbol* it    = poly_.end  ();
		const field_symbol* begin = poly_.begin();

		uint32_t count = 0;

		while ((begin != it) && ((*(--it)) == 0))
		{
			++count;
		}

		if (0 != count)
		{
			poly_.resize(poly_size - count, 0);
		}
	}
}

} // namespace galois

#endif
