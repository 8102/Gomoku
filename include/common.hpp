#ifndef               ___COMMON_HPP___
# define              ___COMMON_HPP___

#include              <string>
#include              <iostream>
#include              <vector>
#include              <map>
#include              <memory>

#define               vf(x, y)                   sf::Vector2f((x), (y))
#define               vi(x, y)                   sf::Vector2i((x), (y))

/* To have an equivalent to make_shared< T > for  std::unique_ptr */
template < class T, class ... Args >
std::unique_ptr< T > make_unique(Args&& ... args) {
  return std::unique_ptr< T >(new T(std::forward< Args >(args)...));
}

#endif                /* !___COMMON_HPP___ */
