/*
 *  Created by Dmitry Lyssenko, last modified July 18, 2018
 *
 * [C/R]OUTABLE interface:
 *
 * - provides an easy interface for arbitrary custom classes to become out'able.
 * Provide [C]OUTABLE() macro call enumerating all class objects (or class printing
 * methods), which you would like to expose to an output stream.
 *
 * COUTABLE macro provides a compact form of output (a single line format)
 * OUTABLE macro provides a multi-line format
 * ROUTABLE provides a multi-line format for data structure with recursive pointers
 *
 *
 * SYNOPSYS:
 * struct myClass {
 *      //...
 *      int             i;
 *      string          s;
 *      vector<int>     vi;
 *      // ...
 *
 *      OUTABLE(myClass, i, s, vi)              // First parameter is always a class itself
 * };
 *
 * int main(){
 *  myClass uc{123, "3.14", {1, 23}};
 *
 *  cout << "uc: " << uc << endl;
 * }
 *
 * Output:
 * uc: class 'myClass'...
 *     myClass::i: 123
 *     myClass::s: "3.14"
 *     myClass::vi[0]: 1
 *     myClass::vi[1]: 23
 *
 * Output if myClass was definined with COUTABLE macro:
 * uc: myClass.. i:123, s:"3.14", vi[0]:1, vi[1]:23,
 */

#pragma once

#include <string>
#include <sstream>
#include <type_traits>
#include "macrolib.h"





#define OUTABLE_TAB 4
#define OUTABLE_IND std::string(OUTABLE_TAB * (idl>0? idl: 0), ' ')
#define COUTABLE_SFX ", "
constexpr int size_of_coutable_sfx(void){ return sizeof(COUTABLE_SFX) - 1; }

// compact out-able form (single-line) output
#define __COUT_ARG__(VAR) __cout_arg__(ss, #VAR, VAR);
#define COUTABLE(CLASS, VARS...) \
    friend std::ostream & operator<<(std::ostream & os, const CLASS &x) \
        { return x.__coutme__(os); } \
    std::ostream & __coutme__(std::ostream & os) const { \
        std::ostringstream ss; \
        ss << #CLASS ".. "; \
        MACRO_TO_ARGS(__COUT_ARG__, VARS) \
        ss.seekp(-size_of_coutable_sfx(), ss.cur); ss << '\0'; \
        return os << ss.str(); \
    } \
    std::ostream & __outme__(std::ostream & os, int idl, const char * class_name=#CLASS) const { \
        os << OUTABLE_IND << "class '" #CLASS "'..."; \
        MACRO_TO_ARGS(__OUT_ARG__, VARS) \
        return os; \
    }


// full out-able form (multi-line) output
#define __OUT_ARG__(VAR) __out_arg__(os, abs(idl)+1, class_name, #VAR, VAR);
#define OUTABLE(CLASS, VARS...) \
    friend std::ostream & operator<<(std::ostream & os, const CLASS &x) \
        { return x.__outme__(os, 0, #CLASS); } \
    std::ostream & __outme__(std::ostream & os, int idl, const char * class_name=#CLASS) const { \
        os << OUTABLE_IND << "class '" #CLASS "'..."; \
        MACRO_TO_ARGS(__OUT_ARG__, VARS) \
        return os; \
    } \
    std::ostream & __coutme__(std::ostream & os) const { \
        std::ostringstream ss; \
        ss << #CLASS ".. "; \
        MACRO_TO_ARGS(__COUT_ARG__, VARS) \
        ss.seekp(-size_of_coutable_sfx(), ss.cur); ss << '\0'; \
        return os << ss.str(); \
    }


// ROUTABLE is an extension to OUTABLE() interface for recursive data pointers, e.g.:
// ROUTABLE(myClass, PTR(ptr1, ptr2), x, y, ...);
// CAUTION: if you find yourself using ROUTABLE interface, quite likely your class
//          design is bad. Modern C++ let getting away w/o using pointers as resource
//          holders in most cases
#define PTR(RPTRS...) RPTRS
#define __ROUT_ARG__(X) if(X){ os << endl; X->__routme__(os, abs(idl)+2, class_name); }
#define ROUTABLE(PTR, CLASS, VARS...) \
    friend std::ostream & operator<<(std::ostream & os, const CLASS &x) \
        { return x.__routme__(os, 0, #CLASS); } \
    std::ostream & __routme__(std::ostream & os, int idl, const char * class_name=#CLASS) const { \
        os << OUTABLE_IND << "class '" #CLASS "' (" << (void*)this << ")..."; \
        MACRO_TO_ARGS(__OUT_ARG__, VARS) \
        MACRO_TO_ARGS(__ROUT_ARG__, PTR) \
        return os; \
    } \
    std::ostream & __outme__(std::ostream & os, int idl, const char * class_name=#CLASS) const { \
        os << OUTABLE_IND << "class '" #CLASS "' (" << (void*)this << ")..."; \
        MACRO_TO_ARGS(__OUT_ARG__, VARS) \
        MACRO_TO_ARGS(__ROUT_ARG__, PTR) \
        return os; \
    }



//
// Define compact and full format for all types of data
//

// 1a. atomic (fundamental) type output: out-able format
template<typename T>
typename std::enable_if<std::is_fundamental<T>::value, void>::type
__out_arg__(std::ostream & os, int idl, const char *class_name, const char *var_name, T var) {
 os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": " << var;
}
// cout-able format
template<typename T>
typename std::enable_if<std::is_fundamental<T>::value, void>::type
__cout_arg__(std::ostream & os, const char *var_name, T var) {
 os << (var_name? std::string(var_name) + ":": "") << var << COUTABLE_SFX;
}



// 1b. enum type output: out-able format
template<typename T>
typename std::enable_if<std::is_enum<T>::value, void>::type
__out_arg__(std::ostream & os, int idl, const char *class_name, const char *var_name, T var){
 os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": " << var;
}
// cout-able format
template<typename T>
typename std::enable_if<std::is_enum<T>::value, void>::type
__cout_arg__(std::ostream & os, const char *var_name, T var){
 os << (var_name? std::string(var_name) + ":": "") << var << COUTABLE_SFX;
}



// 1c. const char * type output: out-able format
void __out_arg__(std::ostream & os, int idl,
                 const char *class_name, const char *var_name, const char *var){
 if(var)
  os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": \"" << var << '"';
 else
  os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": nullptr";
}
// cout-able format
void __cout_arg__(std::ostream & os, const char *var_name, const char *var){
 if(var)
  os << (var_name? std::string(var_name) + ":\"": "\"") << var << "\"" << COUTABLE_SFX;
 else
 os << (var_name? std::string(var_name) + ":": "") << "nullptr" << COUTABLE_SFX;
}



// 1d. pointer type output: out-able format
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type
__out_arg__(std::ostream & os, int idl,
            const char *class_name, const char *var_name, const T & var){
 if(var)
  os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": " << (void *)var;
 else
  os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": nullptr";
}
// cout-able format
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type
__cout_arg__(std::ostream & os, const char *var_name, const T & var){
 if(var)
  os << (var_name? std::string(var_name) + ":": "") << (void *)var << COUTABLE_SFX;
 else
  os << (var_name? std::string(var_name) + ":": "") << "nullptr, ";
}
// CAUTION: if you find yourself using pointers as resource holders, quite likely
//          your class design is bad. Modern C++ let getting away w/o using pointers
//          in most cases.


// 1e. outable class output: out-able format
template<typename T>
typename std::enable_if<std::is_member_function_pointer<decltype(& T::__outme__)>::value,
                        void>::type
__out_arg__(std::ostream & os, int idl,
            const char *class_name, const char *var_name, const T & var){
 os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": ";
 var.__outme__(os, -idl);
}
// cout-able format
template<typename T>
typename std::enable_if<std::is_member_function_pointer<decltype(& T::__coutme__)>::value,
                        void>::type
__cout_arg__(std::ostream & os, const char *var_name, const T & var){
 os << (var_name? std::string(var_name) + ":": "");
 var.__coutme__(os);
}



// 2a. basic string output: out-able format
void __out_arg__(std::ostream & os, int idl, const char *class_name,
                 const char *var_name, const std::basic_string<char> &var){
 os << std::endl << OUTABLE_IND << class_name << "::" << var_name << ": \"" << var << '"';
}
// cout-able format
void __cout_arg__(std::ostream & os, const char *var_name, const std::basic_string<char> &var){
 os << (var_name? std::string(var_name) + ":\"": "\"") << var << "\"" << COUTABLE_SFX;
}



// 2b. native arrays type output: out-able format
template<typename T>
typename std::enable_if<std::is_array<T>::value, void>::type
__out_arg__(std:: ostream & os, int idl, const char *class_name,
            const char *var_name, const T & var){
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __out_arg__(os, idl, class_name, (vn+'['+std::to_string(idx++)+']').c_str(), v);
}
// cout-able format
template<typename T>
typename std::enable_if<std::is_array<T>::value, void>::type
__cout_arg__(std:: ostream & os, const char *var_name, const T & var){
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __cout_arg__(os, (vn + '['+std::to_string(idx++)+']').c_str(), v);
}




// 2c. linear containers type output: out-able format
template<template<typename, typename> class Container, typename T, typename Alloc>
typename std::enable_if<std::is_member_function_pointer<decltype(& Alloc::allocate)>::value,
                        void>::type
__out_arg__(std:: ostream & os, int idl, const char *class_name,
            const char *var_name, const Container<T, Alloc> & var){
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __out_arg__(os, idl, class_name, (vn + '[' + std::to_string(idx++) + ']').c_str(), v);
 if(var.empty())
  { __out_arg__(os, idl, class_name, var_name, '['); os << ']'; }
}
// cout-able format
template<template<typename, typename> class Container, typename T, typename Alloc>
typename std::enable_if<std::is_member_function_pointer<decltype(& Alloc::allocate)>::value,
                        void>::type
__cout_arg__(std:: ostream & os, const char *var_name, const Container<T, Alloc> & var){
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __cout_arg__(os, (vn + '[' + std::to_string(idx++) + ']').c_str(), v);
 if(var.empty()){
  std::ostringstream ss;
  __cout_arg__(ss, var_name, '[');
  ss.seekp(-size_of_coutable_sfx(), ss.cur);
  ss << "], ";
  os << ss.str();
 }
}



// 2d. container map out: out-able format
template<template<typename, typename, typename, typename>
         class Map, typename K, typename V, typename C, typename A>
void __out_arg__(std::ostream & os, int idl, const char *class_name,
                 const char *var_name, const Map<K, V, C, A> & var){
 for(auto &i: var){
  std::ostringstream ss;
  ss << var_name << "[ ";
  __cout_arg__(ss, nullptr, i.first);
  ss.seekp(-size_of_coutable_sfx(), ss.cur);
  ss << " ]";
  __out_arg__(os, idl, class_name, ss.str().c_str(), i.second);
 }
 if(var.empty())
  { __out_arg__(os, idl, class_name, var_name, '['); os << ']'; }
}
// cout-able format
template<template<typename, typename, typename, typename>
         class Map, typename K, typename V, typename C, typename A>
void __cout_arg__(std::ostream & os, const char *var_name, const Map<K, V, C, A> & var){
 for(auto &i: var){
  std::ostringstream ss;
  ss << var_name << "[ ";
  __cout_arg__(ss, nullptr, i.first);
  ss.seekp(-size_of_coutable_sfx(), ss.cur);
  ss << " ]";
  __cout_arg__(os, ss.str().c_str(), i.second);
 }
 if(var.empty()) {
  std::ostringstream ss;
  __cout_arg__(ss, var_name, '[');
  ss.seekp(-size_of_coutable_sfx(), ss.cur);
  ss << "], ";
  os << ss.str();
 }
}



// add other types as needed ....
















