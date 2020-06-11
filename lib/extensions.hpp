/*
 * Created by Dmitry Lyssenko
 *
 * Macro definitions for ENUM stringification for in-class enum declarations:
 *
 * ENUM - will declare enum type w/o any possibility for stringification (mimics enum declaration)
 * ENUMSTR - will declare enum type with a possibility for later stringification
 * STRINGIFY - stringification of previously declared enum type with ENUMSTR macro
 *
 * trivial macro definitions making enum stringification easy:
 * declare enums now in a few simple steps:
 *  1. define a macro enlisting all enum items:
 *          #define MY_COLORS \
 *                  Red, \
 *                  Amber, \
 *                  Green
 *
 *  2. declare enums with ENUM macro if non-class declaration, or with ENUMSTR for in-class:
 *          ENUMSTR(trafficLightColors, MY_COLORS)
 *     - ENUM macro only declares trafficLightColors of enums, while ENUMSTR in addition
 *       declares an array of: static const char* trafficLightColors_[]
 *
 *  3. initialize an array of c-strings to generated by STRINGIFY macro (non in-class):
 *          STRINGIFY(SomeClass::trafficLightColors, MY_COLORS)
 *     - declares STRINGIFY macro outside of class, for non in-class declaration drop the
 *     cдass qualifier:
 *          STRINGIFY(trafficLightColors, MY_COLORS)
 *
 *  Now, enum trafficLightColors is defined, as well as its c-string representation:
 *          cout << "All traffic-light colors:";
 *          for(int i=0; i<COUNT_ARGS(MY_COLORS); ++i)
 *           cout << ' ' << SomeClass::trafficLightColors_str[i];
 *           // or equally: cout << ' ' << ENUMS(trafficLightColors, i);
 *          cout << endl;
 *
 * Obvious caveat: enums declared that way do not allow value re-definition
 * However, consider enum value redefinition a bad programming practice.
 * If required, provide enum-remapping facility instead
 */

#pragma once
#include <string>
#include <vector>
#include "macrolib.hpp"
#include <type_traits>



#define __COMMA_SEPARATED__(X) X,
#define __STR_COMMA_SEPARATED__(X) #X,


#define ENUM(ENUM_CLASS, ENDEF...) \
    enum ENUM_CLASS { MACRO_TO_ARGS(__COMMA_SEPARATED__, ENDEF) };


#define ENUMSTR(ENUM_CLASS, ENDEF...) \
    enum ENUM_CLASS { MACRO_TO_ARGS(__COMMA_SEPARATED__, ENDEF) }; \
    static const char * ENUM_CLASS ## _str[];


#define STRINGIFY(ENUM_CLASS, ENDEF...) \
    const char * ENUM_CLASS ## _str[] { MACRO_TO_ARGS(__STR_COMMA_SEPARATED__, ENDEF) };

#define ENUMS(ENUM_CLASS, ENUM_IDX) ENUM_CLASS ## _str[ENUM_IDX]





/*
 * A trivial wrapper around std::exception
 * - to be used with enum stringification in classes (ENUMSTR, STRINGIFY)
 *
 * Synopsis:
 * // 1. define ENUMSTR within the class, enumerating exception reasons
 *
 *      class myClass {
 *       public:
 *        ...
 *          define THROWREASON
 *              InvalidInput, \
 *              IncorrectUsage, \
 *              WrongType
 *          ENUMSTR(ThrowReason, THROWREASON)
 *
 *          ...
 *          EXCEPTION(ThrowReason)
 *      };
 *      STRINGIFY(myClass::ThrowReason, THROWREASON)
 *      #undef THROWREASON
 *
 *
 * // 2. use in throwing defined exception reasons, e.g.:
 *      throw EXP(InvalidInput);
 *
 *
 * // 3. Possible output upon non-handled exception:
 * libc++abi.dylib: terminating with uncaught exception of type myClass::stdException: InvalidInput
 *
 *
 * // 4. define catching exception:
 *      try { ... }     // something producing ThrowReason exception
 *      catch(myClass::stdException & e) {  // or std::exception & e, but then to access code()
 *                                          // and where() down-casting is required
 *       std::cout << "exception string: " << e.what() << std::endl;
 *       std::cout << "exception code: " << e.code() << std::endl;
 *       std::cout << "exception in: " << e.where() << std::endl;
 *      }
 *
 */


// return std::exception from classes;
// upon throwing a copy of the object is made (throwing is by value). In our case
// a shallow copy will suffice despite the pointer: the class is meant to be used
// with ENUMSTR / STRINGIFY, which are static data.
// for in-class declaration:
#define EXCEPTIONS(THROW_ENUM) \
    class stdException: public std::exception { \
     public: \
                            stdException(void) = delete; \
                            stdException(int reason, const char *what, \
                                         const char *func, const char *file, int line): \
                             ec_{reason}, msg_{what}, func_{func}, file_{file}, line_{line} {} \
        const char *        what(void) const noexcept { return msg_; } \
        std::string         where(void) const noexcept { \
                             return std::string{"file: '"} + file_ + \
                                    "', func: '" + func_ + \
                                    "()', line: " + std::to_string(line_); \
                            } \
        const char *        func(void) const noexcept { return func_; } \
        const char *        file(void) const noexcept { return file_; } \
        int                 line(void) const noexcept { return line_; } \
        int                 code(void) const noexcept { return ec_; } \
     private: \
        int                 ec_; \
        const char *        msg_; \
        const char *        func_; \
        const char *        file_; \
        int                 line_; \
    }; \
    stdException __exp__(THROW_ENUM __reason__, const char *__funcname__, \
                         const char *__filename__, int __line__) const \
        { return stdException{__reason__, ENUMS(THROW_ENUM, __reason__), \
                              __funcname__, __filename__, __line__}; }


constexpr const char * drop_leading_dot_slash(const char * path) {
  return path[0] == '.' && path[1] == '/' ? path + 2 : path;
}

// for in-place throw parameter
#define EXP(TROW_REASON) __exp__(TROW_REASON, __func__, drop_leading_dot_slash(__FILE__), __LINE__)





/*
 * Following trivial extension facilitates ability to check if a value is in
 * enumeration (similar to python's "x in [...]" construct)
 *
 * Synopsis:
 *
 *      x = 5;
 *      if(x AMONG(1,2,3,5,6))
 *       std::cout << "x is in!" << std::endl;
 *      else
 *       std::cout << "x is not in!" << std::endl;
 *
 *
 * CAVEAT on usage with c-strings:
 * - first parameter in AMONG macros to be type-casted as <const char*>:
 *
 *      const char *x = "abc";
 *      if(x AMONG(static_cast<const char*>("abc"), "def", "xyz")) ...
 *
 *
 * Preferably use AMONG construct, when number of enlisted arguments > 2, otherwise
 * go with conditional 'or'
 */

template<class T>
bool operator==(const T &__a__, std::vector<T> __v__) {
 for(auto &x: __v__)
  if(x == __a__) return true;
 return false;
}

bool operator==(const std::string &__a__, std::vector<const char *> __b__) {
 for(auto x: __b__)
  if(__a__ == x) return true;
 return false;
}

#define AMONG(FIRST, REST...) \
    == std::vector<decltype(FIRST)>{FIRST, MACRO_TO_ARGS(__COMMA_SEPARATED__, REST)}





/*
 * GUARD is a polymorphic macro allowing preserving and automatically reinstating the
 * value of the preserved object accessed either by a reference, or via getters/setters
 *
 * 1. following GUARD interface provides a guard functionality for an arbitrary objects
 * accessible by reference: it will preserve the object value upon interface declaration
 * and will restore the object value upon exiting the scope (GUARD's destruction);
 *
 * Synopsis:
 *
 *      double x = 3.14;
 *      cout << "x: "  << x << endl;
 *      {
 *       GUARD(x)
 *       x = 2.71;
 *       cout << "x: "  << x << endl;
 *      }
 *      cout << "x: "  << x << endl;
 *
 * Output:
 *      x: 3.14
 *      x: 2.71
 *      x: 3.14
 *
 *
 * 2. Sometimes classes cater only getter and setter methods to access
 * their objects. For such case, this GUARD interface provides a solution:
 *
 *      class MyX {
 *       public:
 *          int                 get(void) const { return x_; }
 *          void                set(int x) { x_ = x; }
 *       private:
 *          int              x_;
 *      };
 *
 *      MyX x;
 *      x.set(123);
 *      cout << "x: "  << x.get() << endl;
 *      {
 *       GUARD(x.get, x.set)     // spell here object's getter and setter
 *       x.set(-1);
 *       cout << "x: "  << x.get() << endl;
 *      }
 *      cout << "x: "  << x.get() << endl;
 *
 * Output:
 *      x: 123
 *      x: -1
 *      x: 123
 *
 *
 * in case where's multiple objects to guard, list them one by one each on the
 * new line:
 *      // ...
 *      GUARD(x)
 *      GUARD(y)
 *      // ...
 */


// There are 2 forms of GUARD: for a {single object} and for {getter, setter}
// Forms demultiplexing occurs in __GUARD_CHOOSER__ macro, which results into
// expanding __GUARD_1_ARG__ for the former case and into __GUARD_2_ARG__ for
// the latter
//
// __GUARD_1_ARG__: declares a trivial class __Guard_X__, which stores object's
// value and its pointer. Restoration of the object's value occurs upon __GUARD_X__'s
// destruction
//
// __GUARD_2_ARG__: declares a child class of __Guard_X__, where it only captures
// the value of the object through its getter (in the constructor of the child class
// xptr_ is getting nullified so that when destroyed the restoration at parent is skipped)
// Restoration of the object is designed through capturing object's setter via lambda
// and calling the lambda (i.e. object's setter effectively) with preserved value
// in the destructor of the child class
//
// Each form of GUARD is appended __LINE__ macro to ensure uniqueness of declarations
// allowing  multiple invocations of the macro dodging name clashing

#define __STITCH_2TKNS__(X,Y) X ## Y
#define STITCH_2TKNS(X,Y) __STITCH_2TKNS__(X, Y)


#define __GUARD_1_ARG__(X) \
    __Guard_X__<decltype(X)> STITCH_2TKNS(__my_Guard_X__, __LINE__)(X);
#define __GUARD_2_ARG__(X, Y) \
    struct STITCH_2TKNS(__Guard_GS__, __LINE__): public __Guard_X__<decltype(X())> { \
        STITCH_2TKNS(__Guard_GS__, __LINE__)(decltype(X()) __Guard_X_arg__, \
                                               std::function<void(decltype(X()))> __Guard_X_l__): \
         __Guard_X__(__Guard_X_arg__), lambda{__Guard_X_l__} { xptr_ = nullptr; } \
       ~STITCH_2TKNS(__Guard_GS__, __LINE__)(void) { lambda(x_); }; \
        std::function<void (decltype(X()))> lambda; \
    } STITCH_2TKNS(__my_Guard_GS__, __LINE__) \
        { X(), [&](decltype(X()) __Guard_X_arg__) { Y(__Guard_X_arg__); } };
#define __GUARD_3RD_ARG__(ARG1, ARG2, ARG3, ...) ARG3
#define __GUARD_CHOOSER__(ARGS...) \
    __GUARD_3RD_ARG__(ARGS, __GUARD_2_ARG__, __GUARD_1_ARG__)
#define GUARD(ARGS...) \
    __GUARD_CHOOSER__(ARGS)(ARGS)





template <typename T>
class __Guard_X__ {
 // Guard class itself
 public:
                        __Guard_X__(void) = delete;
                        __Guard_X__(typename std::remove_reference<T>::type & __Guard_X_arg__):
                         x_{__Guard_X_arg__}, xptr_{&__Guard_X_arg__} {}
                        __Guard_X__(typename std::remove_reference<T>::type && __Guard_X_arg__):
                         x_{std::move(__Guard_X_arg__)}, xptr_{&__Guard_X_arg__} {}
                       ~__Guard_X__(void) { if(xptr_) *xptr_ = std::move(x_); }
 protected:
    typename std::remove_reference<T>::type     x_;
    typename std::remove_reference<T>::type *   xptr_;
};





/*
 * A trivial SWAP macro facilitating plain `void swap(left, right)` operation:
 *
 * Synopsis:
 * class my_class {
 *  friend SWAP(my_class, a_, b_, c_)
 *  ...
 * }
 *
 * The above translates into:
 * class my_class {
 *  friend void swap(my_class &left, my_class &right) {
 *   using std::swap;
 *   swap(left.a_, right.a_);
 *   swap(left.b_, right.b_);
 *   swap(left.c_, right.c_);
 *  }
 *  ...
 * }
 *
 *
 * Another COPY macro defying a copy for all enumerated elements: void copy(to, const from)
 * - similar to SWAP, but copies by value - to facilitate copying in non-default CC:
 *
 * Synopsis:
 * class my_class {
 *  friend COPY(my_class, a_, b_, c_)
 *                      my_class(const my_class &rhs)  {        // CC
 *                       copy(*this, rhs);
 *                       ...
 *                      }
 *  ...
 *  private:
 *      some_type        a_, b_, c_;
 * }
 *
 */

#define __SWAP_PAIR__(X) swap(__left__.X, __right__.X);

#define SWAP(TYPE, ARGS...) \
    void swap(TYPE &__left__, TYPE &__right__) \
     { using std::swap; MACRO_TO_ARGS(__SWAP_PAIR__, ARGS) }



#define __COPY_VAR__(X) __left__.X = __right__.X;

#define COPY(TYPE, ARGS...) \
    void copy(TYPE &__left__, const TYPE &__right__) \
     { MACRO_TO_ARGS(__COPY_VAR__, ARGS) }





/*
 * A couple of definitions for mutexes:
 * ULOCK - declare a unique_lock (mutex sguard)
 * TLOCK - declare an operator-like lock
 *
 * Synopsis:
 * {
 *  ULOCK(mtx)
 *  ...
 * } // mtx will be automatically released
 *
 * TLOCK(mtx) { // block is executed under mtx
 *  ...
 * }
 *
 */
#define ULOCK(MTX) \
    std::unique_lock<std::mutex> STITCH_2TKNS(__ulck__, __LINE__){MTX};

#define TLOCK(MTX) \
    for(std::unique_lock<std::mutex> __tlck__(MTX); __tlck__.owns_lock(); __tlck__.unlock())











