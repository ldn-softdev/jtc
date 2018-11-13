/*
 *  Created by Dmitry Lyssenko
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
 * int main() {
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





#define __OUTABLE_TAB__ 3
#define __OUTABLE_IND__(X) std::string(__OUTABLE_TAB__ * (X>0? X: 0), ' ')
#define __COUTABLE_SFX__ ", "
#define __COUTABLE_TRL__ ".. "
#define __OUTABLE_TRL__ "'..."

constexpr int size_of_coutable_sfx(void) { return sizeof(__COUTABLE_SFX__) - 1; }

// compact out-able form (single-line) output
#define __COUT_ARG__(VAR) __cout_arg__(__coutable_ss__, #VAR, VAR);
#define COUTABLE(CLASS, VARS...) \
    friend std::ostream & operator<<(std::ostream & os, const CLASS &x) \
        { return x.__coutme__(os); } \
    std::ostream & __coutme__(std::ostream & os) const { \
        std::ostringstream __coutable_ss__(#CLASS __COUTABLE_TRL__); \
        MACRO_TO_ARGS(__COUT_ARG__, VARS) \
        std::string result(__coutable_ss__.str()); \
        return os << (result.back() != ' '? result: result.erase(result.size()-2)); \
    } \
    std::ostream & __outme__(std::ostream & __outable_os__, int __outable_ind__, \
                             const char * __outable_class_name__=#CLASS) const { \
        std::ostringstream __coutable_ss__(#CLASS __COUTABLE_TRL__); \
        MACRO_TO_ARGS(__COUT_ARG__, VARS) \
        std::string result(__coutable_ss__.str()); \
        return __outable_os__ << result.erase(result.size()-2); \
    }


// full out-able form (multi-line) output
#define __OUT_ARG__(VAR) __out_arg__(__outable_os__, abs(__outable_ind__)+1, \
                                     __outable_class_name__, #VAR, VAR);
#define OUTABLE(CLASS, VARS...) \
    friend std::ostream & operator<<(std::ostream & os, const CLASS &x) \
        { return x.__outme__(os, 0, #CLASS); } \
    std::ostream & __outme__(std::ostream & __outable_os__, int __outable_ind__, \
                             const char * __outable_class_name__ = #CLASS) const { \
        __outable_os__ << __OUTABLE_IND__(__outable_ind__) << "class '" #CLASS __OUTABLE_TRL__; \
        MACRO_TO_ARGS(__OUT_ARG__, VARS) \
        return __outable_os__; \
    } \
    std::ostream & __coutme__(std::ostream & os) const { \
        std::ostringstream __coutable_ss__(#CLASS __COUTABLE_TRL__); \
        MACRO_TO_ARGS(__COUT_ARG__, VARS) \
        std::string result(__coutable_ss__.str()); \
        return os << (result.back() != ' '? result: result.erase(result.size()-2)); \
    }


// ROUTABLE is an extension to OUTABLE() interface for recursive data pointers, e.g.:
// ROUTABLE(myClass, PTR(ptr1, ptr2), x, y, ...);
// CAUTION: if you find yourself using ROUTABLE interface, quite likely your class
//          design is bad. Modern C++ let getting away w/o using pointers as resource
//          holders in most cases
#define PTR(RPTRS...) RPTRS
#define __ROUT_ARG__(X) if(X) { os << endl; X->__routme__(os, abs(ind)+2, class_name); }
#define ROUTABLE(CLASS, PTR, VARS...) \
    friend std::ostream & operator<<(std::ostream & os, const CLASS &x) \
        { return x.__routme__(os, 0, #CLASS); } \
    std::ostream & __routme__(std::ostream & os, int ind, const char * class_name=#CLASS) const { \
        os << __OUTABLE_IND__(ind) << "class '" #CLASS "' (" << (void*)this << ")..."; \
        MACRO_TO_ARGS(__OUT_ARG__, VARS) \
        MACRO_TO_ARGS(__ROUT_ARG__, PTR) \
        return os; \
    } \
    std::ostream & __outme__(std::ostream & os, int ind, const char * class_name=#CLASS) const { \
        os << __OUTABLE_IND__(ind) << "class '" #CLASS "' (" << (void*)this << ")..."; \
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
__out_arg__(std::ostream & os, int ind, const char *class_name, const char *var_name, T var) {
 os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": " << var;
}

// cout-able format
template<typename T>
typename std::enable_if<std::is_fundamental<T>::value, void>::type
__cout_arg__(std::ostream & os, const char *var_name, T var) {
 os << (var_name? std::string(var_name) + ":": "") << var << __COUTABLE_SFX__;
}



// 1b. enum type output: out-able format
template<typename T>
typename std::enable_if<std::is_enum<T>::value, void>::type
__out_arg__(std::ostream & os, int ind, const char *class_name, const char *var_name, T var) {
 os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": " << var;
}

// cout-able format
template<typename T>
typename std::enable_if<std::is_enum<T>::value, void>::type
__cout_arg__(std::ostream & os, const char *var_name, T var) {
 os << (var_name? std::string(var_name) + ":": "") << var << __COUTABLE_SFX__;
}



// 1c. const char * type output: out-able format
void __out_arg__(std::ostream & os, int ind,
                 const char *class_name, const char *var_name, const char *var) {
 if(var)
  os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": \"" << var << '"';
 else
  os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": nullptr";
}

// cout-able format
void __cout_arg__(std::ostream & os, const char *var_name, const char *var) {
 if(var)
  os << (var_name? std::string(var_name) + ":\"": "\"") << var << "\"" << __COUTABLE_SFX__;
 else
 os << (var_name? std::string(var_name) + ":": "") << "nullptr" << __COUTABLE_SFX__;
}



// 1d. pointer type output: out-able format
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type
__out_arg__(std::ostream & os, int ind,
            const char *class_name, const char *var_name, const T & var) {
 if(var)
  os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": " << (void *)var;
 else
  os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": nullptr";
}

// cout-able format
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type
__cout_arg__(std::ostream & os, const char *var_name, const T & var) {
 if(var)
  os << (var_name? std::string(var_name) + ":": "") << (void *)var << __COUTABLE_SFX__;
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
__out_arg__(std::ostream & os, int ind,
            const char *class_name, const char *var_name, const T & var) {
 os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": ";
 var.__outme__(os, -ind);
}

// cout-able format
template<typename T>
typename std::enable_if<std::is_member_function_pointer<decltype(& T::__coutme__)>::value,
                        void>::type
__cout_arg__(std::ostream & os, const char *var_name, const T & var) {
 os << (var_name? std::string(var_name) + ":": "");
 var.__coutme__(os);
}



// 2a. basic string output: out-able format
void __out_arg__(std::ostream & os, int ind, const char *class_name,
                 const char *var_name, const std::basic_string<char> &var) {
 os << std::endl << __OUTABLE_IND__(ind) << class_name << "::" << var_name << ": \"" << var << '"';
}

// cout-able format
void __cout_arg__(std::ostream & os, const char *var_name, const std::basic_string<char> &var) {
 os << (var_name? std::string(var_name) + ":\"": "\"") << var << "\"" << __COUTABLE_SFX__;
}



// 2b. native arrays type output: out-able format
template<typename T>
typename std::enable_if<std::is_array<T>::value, void>::type
__out_arg__(std:: ostream & os, int ind, const char *class_name,
            const char *var_name, const T & var) {
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __out_arg__(os, ind, class_name, (vn+'['+std::to_string(idx++)+']').c_str(), v);
}

// cout-able format
template<typename T>
typename std::enable_if<std::is_array<T>::value, void>::type
__cout_arg__(std:: ostream & os, const char *var_name, const T & var) {
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __cout_arg__(os, (vn + '['+std::to_string(idx++)+']').c_str(), v);
}



// 2c. linear containers type output: out-able format
template<template<typename, typename> class Container, typename T, typename Alloc>
typename std::enable_if<std::is_member_function_pointer<decltype(& Alloc::allocate)>::value,
                        void>::type
__out_arg__(std:: ostream & os, int ind, const char *class_name,
            const char *var_name, const Container<T, Alloc> & var) {
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __out_arg__(os, ind, class_name, (vn + '[' + std::to_string(idx++) + ']').c_str(), v);
 if(var.empty())
  { __out_arg__(os, ind, class_name, var_name, '['); os << ']'; }
}

// cout-able format
template<template<typename, typename> class Container, typename T, typename Alloc>
typename std::enable_if<std::is_member_function_pointer<decltype(& Alloc::allocate)>::value,
                        void>::type
__cout_arg__(std:: ostream & os, const char *var_name, const Container<T, Alloc> & var) {
 std::string vn{var_name};
 size_t idx = 0;
 for(auto &v: var)
  __cout_arg__(os, (vn + '[' + std::to_string(idx++) + ']').c_str(), v);
 if(var.empty()) {
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
void __out_arg__(std::ostream & os, int ind, const char *class_name,
                 const char *var_name, const Map<K, V, C, A> & var) {
 for(auto &i: var) {
  std::ostringstream ss;
  ss << var_name << "[ ";
  __cout_arg__(ss, nullptr, i.first);
  ss.seekp(-size_of_coutable_sfx(), ss.cur);
  ss << " ]";
  __out_arg__(os, ind, class_name, ss.str().c_str(), i.second);
 }
 if(var.empty())
  { __out_arg__(os, ind, class_name, var_name, '['); os << ']'; }
}

// cout-able format
template<template<typename, typename, typename, typename>
         class Map, typename K, typename V, typename C, typename A>
void __cout_arg__(std::ostream & os, const char *var_name, const Map<K, V, C, A> & var) {
 for(auto &i: var) {
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











