#ifndef               ___COMMON_HPP___
# define              ___COMMON_HPP___

#include              <string>
#include              <iostream>
#include              <vector>
#include              <map>
#include              <memory>

#define               vf(x, y)                   sf::Vector2f((x), (y))
#define               vi(x, y)                   sf::Vector2i((x), (y))

#ifndef              _unused
# ifdef                __GNUC__
#   define              _unused    __attribute__((unused))
# else
#   define              _unused
# endif                /* !__GNUC__ */
#endif                /* !_unused */

#endif                /* !___COMMON_HPP___ */
