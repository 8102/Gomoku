#ifndef _ERROR_HH
# define _ERROR_HH

# include <exception>
# include <string>

#endif // _ERROR_HH

#ifndef _GOMOKU_ERROR
# define _GOMOKU_ERROR

class 	GomokuError : public std::exception
{
public:

	GomokuError(std::string const &message);
	~GomokuError() throw();

public:

	const char	*what() const throw();

protected:
	std::string		_message;
};

#endif // _GOMOKU_ERROR

#ifndef _DOUBLE_THREE_RULE
# define _DOUBLE_THREE_RULE

class 	DoubleThreeRule : public GomokuError
{
public:

	DoubleThreeRule(std::string const &message);
};

#endif // _DOUBLE_THREE_RULE

#ifndef _NOT_EMPTY_ERROR
# define _NOT_EMPTY_ERROR

class NotEmptyError : public GomokuError
{
public:
	NotEmptyError(std::string const &message);
};

#endif // _NOT_EMPTY_ERROR