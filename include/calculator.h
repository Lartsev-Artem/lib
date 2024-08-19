/**
* @note Класс реализующий консольный калькулятор с операциями /+*-!^()%
* 
* Example:
#include "calculator.h"
#include <iostream>
int main()
{
	Calculator<int> calc;
	std::cout << calc("a=2") << std::endl;
	std::cout << calc("b=3") << std::endl;		
	std::cout << calc("a*(b+2)") << std::endl;
	
	for (auto  it = calc.list_vars().begin; it != calc.list_vars().end; it++)
	{
		std::cout << it->first << "= " << it->second << std::endl;
	}
}
*/
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <map>
#include <cstring>

#define MAX_LENGHTH_VAR_NAME 80

static const char* err_msgs[5] = {
    "Broken balance brackets",
    "Unexpected end of expression",
    "Unexpected operator",
    "Overflow",
    "Unknown symbol"
};

template <typename T>
class Calculator
{
public:
    enum ERRORS { ERR_BAL, ERR_END, ERR_OP, ERR_OVER, ERR_UNKNOWN };

    class compcl
    {
    public:
        bool operator()(const char* a, const char* b) const
        {
            return strcmp(a, b) < 0;
        }
    };

private:
    enum { NONE = 0, NUM, OP, VAR };

    char token[MAX_LENGHTH_VAR_NAME];
    char tok_type;
    const char* exp;

    std::map<const char*, T, compcl> vars;
    char lastVar[MAX_LENGHTH_VAR_NAME];

    bool _was_error : 1;
    int  _error_code : 7;

    void next_token();

    void eval_exp5(T& result); // =
    void eval_exp4(T& result); // + -
    void eval_exp3(T& result); // * / %
    void eval_exp2(T& result); // + - Унарные
    void eval_exp1(T& result); // ^ !
    void eval_exp0(T& result); // ( )
    void atom(T& result);
public:
    Calculator();

    T operator()(const char* exp);

    inline bool was_error() { return _was_error; }
    inline const char* error_message() { return err_msgs[_error_code]; }

    struct itr_range
    {        
       typename std::map<const char*, T, compcl>::const_iterator begin;
       typename std::map<const char*, T, compcl>::const_iterator end;
    };    

    inline const itr_range list_vars() const
    {
        return {
            vars.cbegin(),
            vars.cend()
        };
    }

private:
    inline uint64_t fact(int64_t a)
    {
        if (a < 0)
            throw ERR_OVER;
        return a > 1 ? fact(a - 1) * a : 1;
    }
};
//=============================================================================================
#include <cctype>
#include <cstdlib>
#include <cmath>

template <typename T>
Calculator<T>::Calculator()
{
    tok_type = NONE;
    exp = nullptr;
    memset(lastVar, 0, sizeof(lastVar));
    memset(token, 0, sizeof(token));
    _was_error = false;
    _error_code = ERR_UNKNOWN;
}

template <typename T>
void Calculator<T>::next_token()
{    
    auto isoperator{ [](int ch)
    {
        return (bool)strchr("()!^*/%+-=", ch);
    } };

    auto skipSpaces{[](const char*& str)
    {
        while (isspace(*str))
            str++;
    }};

    skipSpaces(exp);
    //Проверка на конец выражения
    if (exp[0] == 0)
    {
        tok_type = NONE;
        return;
    }

    //Проверка типа
    if (isdigit(exp[0]))     //Число
    {
        tok_type = NUM;

        int i = 0;
        //Копирование числа
        while (isdigit(exp[0]) || exp[0] == '.')
        {
            token[i] = *exp++;
            i++;
        }
        token[i] = 0;
    }
    else if (isalpha(exp[0]))    //Переменная
    {
        tok_type = VAR;

        int i = 0;
        //Копирование переменной
        while (isalpha(exp[0]))
        {
            token[i] = *exp++;
            i++;
        }
        token[i] = 0;
    }
    else if ( isoperator(exp[0])) //Оператор
    {
        *token = *exp++;
        tok_type = OP;
    }
    else
        throw ERR_UNKNOWN;
}

template <typename T>
T Calculator<T>::operator()(const char* exp)
{
    this->exp = exp;
    T result = 0;
    this->_was_error = false;

    try
    {
        eval_exp5(result);
    }
    catch (ERRORS ec)
    {
        _was_error = true;
        _error_code = ec;
        result = 0;
    }
    return result;
}

template <typename T>
void Calculator<T>::eval_exp5(T& result)    // =
{
    next_token();
    eval_exp4(result);

    if (tok_type == OP &&
        token[0] == '=')
    {
        char* tmp = new char[strlen(lastVar) + 1];
        strcpy(tmp, lastVar);
        next_token();
        eval_exp4(result);
        if (result)
            vars[tmp] = result;
        else
            vars.erase(tmp);
    }
}

template <typename T>
void Calculator<T>::eval_exp4(T& result)    // + -
{
    eval_exp3(result);

    char op = token[0];
    while (tok_type == OP &&
        (op == '+' ||
            op == '-'))
    {
        next_token();
        T tmp = 0;
        eval_exp3(tmp);
        switch (op)
        {
        case '+':
            result += tmp;
            break;
        case '-':
            result -= tmp;
            break;
        }
        op = token[0];
    }
}

template <typename T>
void Calculator<T>::eval_exp3(T& result)    // * / %
{
    eval_exp2(result);

    char op = token[0];
    while (tok_type == OP &&
        (op == '*' ||
            op == '/' ||
            op == '%'))
    {
        next_token();
        T tmp = 0;
        eval_exp2(tmp);
        switch (op)
        {
        case '*':
            result *= tmp;
            break;
        case '/':
            result /= tmp;
            break;
        case '%':
            result = (T)((int64_t)floor(result) % (int64_t)floor(tmp));
            break;
        }
        op = token[0];
    }
}

template <typename T>
void Calculator<T>::eval_exp2(T& result)    // + - Унарные
{
    char op = '+';

    if (tok_type == OP
        && (token[0] == '+'
            || token[0] == '-'))    //Есть подходящий оператор
    {
        op = token[0];
        next_token();
    }

    eval_exp1(result);

    switch (op)
    {
    case '+':
        break;
    case '-':
        result = -result;
        break;
    }
}

template <typename T>
void Calculator<T>::eval_exp1(T& result)    // ^ !
{
    eval_exp0(result);

    char op = token[0];
    while (tok_type == OP &&
        (op == '^'
            || op == '!')
        )
    {
        next_token();
        T tmp = 0;
        switch (op)
        {
        case '^':
            eval_exp1(tmp);
            result = (T)pow(result, tmp);
            break;
        case '!':
            try
            {
                result = (T)fact((uint64_t)abs(floor(result)));    // = |_result_|!
            }
            catch (...)
            {
                throw ERR_OVER;
            }
            break;
        }
        op = token[0];
    }
}

template <typename T>
void Calculator<T>::eval_exp0(T& result)    // ( )
{
    if (tok_type == OP && *token == '(')
    {
        eval_exp5(result);
        if (!(tok_type == OP &&
            token[0] == ')'))
            throw ERR_BAL;
        else
            next_token();
    }
    else
        atom(result);
}

template <typename T>
void Calculator<T>::atom(T& result)
{
    if (tok_type == OP)
        throw ERR_OP;
    if (tok_type == NONE)
        throw ERR_END;

    if (tok_type == VAR)
    {
        strcpy(lastVar, token);

        try
        {
            result = vars.at(token);
        }
        catch (...)
        {            
            result = 0;
        }

    }
    else
    {
        result = (T)atof(token);
    }
    next_token();
}

#endif //!CALCULATOR_H