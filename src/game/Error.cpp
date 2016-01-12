#include "Error.hh"

/*
** Gomoku Error
*/

GomokuError::GomokuError(std::string const &message) :
	_message(message)
{

}

GomokuError::~GomokuError() throw()
{

}

const char	*GomokuError::what() const throw()
{
	return (_message.c_str());
}

/*
** Double three rule
*/

DoubleThreeRule::DoubleThreeRule(std::string const &message) :
	GomokuError(message)
{

}

/*
** NotEmptyError
*/

NotEmptyError::NotEmptyError(std::string const &message) :
	GomokuError(message)
{

}
