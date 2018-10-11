/*
 * Created by Dmitry Lyssenko, last modified July 3, 2015
 *
 *
 * an easy Debug class, featuring following qualities:
 *  o unified class declaration interface for in-class and in-functions
 *  o for in-class declaration, allows enumerating list of subordinate debuggable
 *    class members (those which comply with host class' debug policy)
 *  o automatic propagation of debug policy down subordinate debuggable class members
 *  o operator-like debuggable calls:
 *    - user may use a block or a single expression after defining a debug point
 *  o thread-safe debugs, additionally allowing user setting up own mutex for debug
 *    operations
 *  o piggy-backing on other debuggable class objects
 *  o user-control of debug outputs:
 *    - define prefix char/string (aka indent)
 *    - use or not indents relevant to the severity of debug point
 *    - define suffix string
 *    - optionally include time-stamps (with control of ms, us)
 *    - let setup filters for function-names and control filter mode:
 *      either filter-in or filter-out
 *    - allows redirecting all debug outputs to user-defined output stream
 *  o the class is trivially copyable/movable/assignable, no extra handling required
 *  o compile out any debugs with -DNDEBUG option
 *
 *
 *  Understanding Debug class policy:
 *  - there are 3 debug values that form debug policy:
 *  1. debug level (setup with level() method):
 *     - defines global level of debugging, typically set once per program
 *       by user:
 *       - level 0 (default) means no debugs allowed,
 *       - level 1 allows debugs with combined debug severity 0 (highest severity)
 *         to be executed
 *       - level 2 allows debugs with combined debug severity 0 and 1 to be executed
 *       - and so on and so forth
 *  2. Debug point severity:
 *     - defines relative severity of given debug placement in the code (setup
 *       with DBG(n) { ... } macro):
 *       - 'n' is the relative severity number. e.g. a user may define a following
 *         debug point within his code:
 *
 *         DBG(0) std::cout << "some debug..." << std::endl;
 *
 *         if the debug level was set to 2, then the above debug will be executed
 *         only if debug point severity (in this case 0) combined with severity
 *         of the Debug object is 0 or 1. Namely in this example, current Debug's
 *         severity also must be 0 or 1.
 *  3. Debug object's severity:
 *     - Debug object severity is combined (added) with debug point severity, then
 *       compared against allowed debug level (the level number must be higher than
 *       combined severity) and allows executing code under defined debug point:
 *     - Debug objects may (and should) maintain policy hierarchy:
 *       say, a class, defined as Debug-capable (a.k.a DEBUGGABLE), contains other
 *       class-members, which are also DEBUGGABLE. Then, if other members enumerated
 *       in Debug declaration, they become subordinate to the host class - their
 *       debug severity will be automatically set to the severity of the host class
 *       +1, e.g.:
 *
 *          class MyDebuggableClass {
 *              ...
 *              OtherDebuggableClass x, y, z;
 *              DEBUGGABLE(x, y, z)
 *              ....
 *          } abc;
 *
 *       in the above snippet, x,y,z's debug severity will be set to the severity
 *       of the MyDebuggableClass + 1 (i.e. lower severity by 1). Namely, if debug
 *       severity of abc happens to be 1 (assume abc is hosted in another debuggable
 *       class), then severity of abc.x, abc.y and abc.z will be set to 2.
 *
 *
 * SYNOPSIS:
 *
 * // Debug declaration (makes class/function DEBUGGABLE):
 *
 * DEBUGGABLE()             // (1) used for in-function declaration and for in-class
 *                          // when the class has no debuggable subordinates (debuggable
 *                          // class members)
 * DEBUGGABLE(a,b,c)        // (2) used for in-class declarations only when the
 *                          // host class has debuggable members (a,b,c)
 *
 * // in the 2nd form of declaration, subordinate debuggable class members (a,b,c)
 * // will conform host class debug policy: their debug severity will be automatically
 * // propagated down the hierarchy increasing severity number by 1 with each level
 * // of the hierarchy
 *
 * // there are 2 ways to facilitate automatic debug policy propagation upon object
 * // creation:
 * // 1. 'DEBUGGABLE' macro must be defined *after* all enumerated subordinate
 * //    debuggable class members are defined, like here:
 *
 * class MyClass {
 *  public:
 *      ...
 *      SomeDebuggableClass x;
 *      DEBUGGABLE(x)       // DEBUGGABLE defined after x - debuggable object
 *      ...
 *  };
 *
 * // 2. include propagation of debug policy into the class constructor:
 *
 * class MyClass {
 *  public:
 *      MyClass(void) { DBG().severity(*this); }
 *      ...
 *      DEBUGGABLE(x)       // DEBUGGABLE defined before x - debuggable object
 *  private:
 *      SomeDebuggableClass x;
 *      ...
 *  };
 *
 *
 * // referring to own Debug object:
 *
 * DBG()                    // this macro simply refers to the prior declared Debug
 *                          // object (with DEBUGGABLE macro) in given scope
 *
 * // That macro let access all the methods of Debug class, e.g.:
 *
 * DBG().level(4);          // set global debug level to 4
 * DBG().severity(3);       // set my Debug's severity to 3
 * DBG().increment(-1)      // increment my Debug's severity by -1, (to 2)
 * DBG().indented(false);   // print only prefix (don't print debug prompt indented
 *                          // proportionally to its severity)
 * DBG().prefix(">")        // use ">" as debug's prompt prefix (default is ".")
 * // etc
 *
 *
 * // debug's placement point notation:
 *
 * DBG(n) { .... }          // executes block if n + my debug's severity < DBG().level()
 *
 * // debug placement point example:
 *
 * DBG(0) std:cout << "some debug" << std::endl;
 *                          // couts "some debug" if my debug's severity < DBG().level()
 *
 * // when execution is allowed at a given debug placement, before executing a block
 * // or a statement, debug will output a Debug prompt, a possible output would look
 * // like this:
 *
 * ..main(), some debug
 *
 * // by default Debug uses 'cout' as the output stream, however, user may redirect
 * // debug outputs to any other output stream, e.g.:
 *
 * DBG().use_ostream(myFileHanlder);
 *
 * // Unfortunately, that will partition the above debug statement:
 * // - debug's prompt "..main(), " - will go into myFileHanlder.
 * // - while user's output ("some debug") will go to cout
 * // in order to be sure that the debug output is inline with Debug's output
 * // stream, use DOUT() macro instead of any predefined streams like cout or cerr.
 * // DOUT() is just an alias for the current output stream used by Debug, thus
 * // prior example would look like this:
 *
 * DBG(0) DOUT() << "some debug" << std::endl;
 *
 * // all debug placement points made thru this interface (DBG(n), or DBG(x,n))
 * // are guaranteed to be thread-safe
 *
 *
 * // propagating debug severity down debuggable classes,
 * // methods severity() and increment():
 * // both methods allow altering severity of the called Debugs:
 * // - severity(n) sets absolute severity level, while
 * // - increment(+/-n) alters adjust severity level by given offset, consider:
 *
 * DBG().severity(5);       // sets my Debug's severity to 5;
 * DBG().increment(+2)      // sets my Debug severity to 7;
 * x.DBG().severity(3);     // sets x's Debug severity to 3;
 *
 * // however, both methods can accept as an argument other debuggable class references:
 *
 * main() {
 *  DEBUGGABLE();           // main() is debuggable with default severity 0.
 *  DBG().level(5);         // allow all debugs with severities 0 thru 4;
 *
 *  DebuggableClass x;      // object x, now has a Debug severity of 0;
 *  DBG().severity(1);      // main()'s Debug's severity is 1 now;
 *  DBG().severity(x);      // now x's Debug severity is 2 and each subordinate
 *                          // debuggable members of x will have severity of 3,
 *                          // in turn their subordinate members will have severity
 *                          // 4 and so on, down the hierarchy
 *  ...
 * }
 *
 * // i.e. when 'DBG().severity(x);' is called, it takes DBG()'s (main()'s) severity
 * // and makes object x comply my (DBG()'s) debug policy - propagates severity
 * //down the hierarchy of all debuggable members of class x (and their members
 * // and so on) adjusting Debugs of each layer of hierarchy respectively
 * // increment() methods also accepts reference to debuggable objects, and
 * // both methods are variadic, which let combine last two statements into one:
 *
 * DBG().severity(1, x);
 *
 * // another example: say, we want to propagate severity offset by +2 from our
 * // current severity to the object x, but effectively w/o altering our severity?
 * // this is the way to do it:
 *
 * DBG().increment(+2, x, -2);  // increase temporarily our severity by +2, make
 *                              // object x conform our severity and then alter
 *                              // back our severity by -2.
 *
 *
 * // relying on other class' Debugs:
 * // - if for some reasons your class/function is non-DEBUGGABLE, but has an access
 * //   to a DEBUGGABLE class, then it's possible to piggy-back on Debug of that
 * //   class:
 *
 * void my_non_debuggable_func(SomeDebuggableClass &x) {
 *  ...
 *   DBG(x, 1) {            // piggy-back on x's Debugs
 *    for(...)              // do some debugs in this block
 *     DOUT(x) << ... << std::endl;
 *   }
 *  ...
 * }
 *
 *
 * // Debug class is trivially copyable/movable/assignable:
 *
 * DebuggableClass x;       // declare debuggable class x (with default severity)
 * x.DBG().severity(10, x); // Set severity to 10 and propagate it down the hierarchy
 * ...
 * DebuggableClass y{x};    // class y will have the same severity hierarchy now
 *
 *
 * // how to perform debug-check w/o printing Debug's prompt?
 * // sometimes such check is needed when debug message is split in parts:
 *
 * DBG(2) DOUT() << "it took to process: ";
 * // ...
 * // do some processing here, calculate t
 * // ...
 * if(DBG()(2))             // no debug prompt is printed, only severity is checked
 *   DOUT() << t << " usec of processor time" << std::endl;
 *
 * // the above example is not entirely thread-safe (only the code covered by DBG(2)
 * // is), so it's okay to use in a single-threaded environment;
 * // below is one of the possible solutions for maintaining thread-safe continuity
 * // of the same debug message:
 *
 * {                        // let's open a code block
 *  unique_lock<mutex> lck(DBG().mutex());  // and put a mutex guard on the block
 *  if(DBG()(2, __func__))  // this will produce a debug prompt
 *   DOUT() << "it took to process: ";
 *  // ...
 *  // do some processing here, calculate t
 *  // ...
 *  if(DBG()(2))            // no debug prompt is produced, no deadlock here
 *   DOUT() << t << " usec of processor time" << std::endl;
 * }                        // this will release the mutex
 *
 *
 * All Debug methods have global effect (for all Debug objects) except:
 * - severity(), increment(), value()
 *
 *
 * Guideline to severity usage in DBG(n) placements:
 *  0 - to debug one time/rare occurrences
 *  1 - for frequent / repetitive debugs (talkative)
 *  2 - for repetitive debugs in loops ( very chatty)
 *  3 - very large debugs dumps of repetitive debugs in a loop
 *  4 - etc
 *
 * to recap up all the user-facing macros:
 *  DEBUGGABLE()        // make debuggable a function or a class w/o subordinates
 *  DEBUGGABLE(x, ...)  // make a debuggable class with subordinate debuggables
 *  DBG()               // refer to the debug object
 *  DBG(n) { ... }      // in-code debug placement using severity n
 *  DBG(c, n) { ... }   // in-code debug placement using debug object from c
 *  DOUT()              // refers to ostream used by debug class
 *  DOUT(c)             // refers to ostream used by debug class c
 *  DBG_PROMPT(n)       // print debug prompt with indent n (+object severity)
 */

#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <type_traits>
#include <exception>
#include <sys/time.h>
#include "macrolib.h"
#include "extensions.hpp"


// below macros define user declaration forms of the debuggable class object:
// declaration forms:
//      DEBUGGABLE() // (1)
//          , or:
//      DEBUGGABLE(x, y, ...) // (2)
// 1st form is used in functions, or in-class when class has no DEBUGGABLE members,
// 2nd form is used only for in-class declarations and let enumerate all DEBUGGABLE
// members. Enumerated debuggable members then conform declared class object debug
// policy
#define DEBUGGABLE(args...) __DEBUGGABLE_ARG__(IF_ARGS(args), args)
#define __DEBUGGABLE_ARG__(X, args...) __DEBUGGABLE_IF__(X, args)
#define __DEBUGGABLE_IF__(X, args...) __DEBUGGABLE_##X##__(args)
#define __DEBUGGABLE_PROPAGATE__(X) X.__dbg__.severity(sev + 1, X);
#define __DEBUGGABLE_false__() \
    Debug   __dbg__;
#define __DEBUGGABLE_true__(args...) \
    Debug   __dbg__{*this}; \
    void    __dbg_propagate__(int sev) \
             { MACRO_TO_ARGS(__DEBUGGABLE_PROPAGATE__, ##args) }


// below macros define debug usage cases:
// 1. DBG() is an alias for the debug object in the given scope
// 2. DBG(N) {...} defines a debug block with severity N, typically would be used
//    as:
//      DBG(2) std::cout << ...
// 3. DBG(C, N) {...} defines a debug block with severity N, based on the debug
//    of object C, e.g.:
//      DBG(x, 1) std::cout << ...  // x's debug severity is used
//
// macros 2. and 3. are entirely covered by Debug's mutex

#define __DBG_0_ARG__() __dbg__                                 // DBG(): access to debug object
#define __DBG_1_ARG__(X) \
    if( __dbg__(X) ) \
     for(std::unique_lock<std::mutex> mgard(__dbg__.mutex()); mgard.owns_lock(); mgard.unlock()) \
      if( __dbg__(X, __func__) )                                // now print the prompt
#define __DBG_2_ARG__(O, X) \
    if( O.__dbg__(X) ) \
     for(std::unique_lock<std::mutex> mgard(O.__dbg__.mutex()); mgard.owns_lock(); mgard.unlock()) \
      if( O.__dbg__(X, __func__) )                              // now print the prompt
#define __DBG_4TH_ARG__(arg1, arg2, arg3, arg4, ...) arg4
#define __DBG_CHOOSER__(args...) \
    __DBG_4TH_ARG__(dummy, ##args, __DBG_2_ARG__, __DBG_1_ARG__, __DBG_0_ARG__)
#define DBG(args...) __DBG_CHOOSER__(args)(args)


#ifdef NDEBUG                                                   // compiled with -DNDEBUG option
 #undef __DBG_1_ARG__
 #undef __DBG_2_ARG__
 #define __DBG_1_ARG__(...) if(false)
 #define __DBG_2_ARG__(...) if(false)
#endif



// DOUT() macros and init Debug's global macros
#define __DOUT_0_ARG__() (DBG().dout())
#define __DOUT_1_ARG__(X) (X.DBG().dout())
#define __DOUT_3RD_ARG__(arg1, arg2, arg3, ...) arg3
#define __DOUT_CHOOSER__(args...) \
    __DOUT_3RD_ARG__(dummy, ##args, __DOUT_1_ARG__, __DOUT_0_ARG__)
#define DOUT(args...) __DOUT_CHOOSER__(args)(args)
#define DBG_INDENT "."                                          // default debug's indent prefix
#define DBG_ALT_INDENT " "                                      // alternative prefix
#define DBG_SUFFIX ", "                                         // default debug's suffix
#define DBG_PROMPT(X) DBG().prompt(__func__, X+1)
#define NDBG 9999
// NDBG definition is just insanely low debug severity - to be used when certain
// debugs needed to be suppressed

// below definition let check whether '__dbg_propagate__()' method is present or not:
template<typename T>
constexpr auto __is_dbg_propagatable__(int)
                -> decltype(std::declval<T>().__dbg_propagate__(0), bool())
                { return true; }
template<typename T>
constexpr bool __is_dbg_propagatable__(...) { return false; }





class Debug {
 public:
                        Debug(void) = default;
    template<class X>   Debug(X &x) { x.DBG().severity(x); };

    short               level(void) const { return udl_; }
    Debug &             level(short ul) { udl_=ul; return *this; }
    bool                indented(void) const { return indented_; }
    Debug &             indented(bool x) { indented_ = x; return *this; }
    Debug &             prefix(const char *i) { indent_ = i; return *this; }
    Debug &             alt_prefix(const char *i) { alt_indent_ = i; return *this; }
    Debug &             suffix(const char *s) { suffix_ = s; return *this; }
    bool                stamped(void) const { return ts_; }
    Debug &             stamped(bool x) { ts_ = x; return *this; }
    Debug &             stamp_ms(bool x) { ms_ = x; return *this; }
    Debug &             stamp_us(bool x) { us_ = x; return *this; }
    Debug &             filter_out(bool x) { ft_ = x; return *this; }
    Debug &             filter(const char *s) { filter_.push_back(s); return *this; }
    Debug &             reset_filter(void) { filter_.clear(); filter_out(false); return *this; }
    std::mutex &        mutex(void) const { return Debug::mtx_; }
    Debug &             mutex(std::mutex & mtx) { mp_ = &mtx; return *this; }
    Debug &             reset_mutex(void) { mp_ = &mtx_; return *this; }
    std::ostream &      dout(void) const { return *op_; }
    Debug &             use_ostream(std::ostream & os) { op_ = &os; return *this; }
    Debug &             reset_ostream(void) { op_ = &std::cout; return *this; }
    bool                operator()(short d, const char *fn=nullptr) const;// check and print prompt
    std::string         prompt(const char *fn, int ms=0,
                               bool useTs=Debug::ts_, bool useAltPfx=false) const;

                        // severity() and increment() definitions implement mechanism which
                        // allows propagating calling class' debug policy to its debuggable
                        // members
    int                 severity(void) const { return ds_; }    // also used to terminate variadic
    template<typename T, typename... Rest>
    typename std::enable_if<std::is_integral<T>::value, void>::type
                        severity(T sev, Rest &&... rest)
                         { ds_=sev; severity(std::forward<decltype(rest)>(rest)...); }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<__is_dbg_propagatable__<Subclass>(0), void>::type
                        severity(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity()+1);       // compel s' severity to mine
                         s.__dbg_propagate__(s.DBG().severity());   // propagate down s' members
                         severity(std::forward<decltype(rest)>(rest)...);
                        }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<
        not __is_dbg_propagatable__<Subclass>(0) and std::is_class<Subclass>::value, void>::type
                        severity(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity()+1);       // compel s' severity to mine
                         severity(std::forward<decltype(rest)>(rest)...);
                        }

    void                increment(void) {}                       // to terminate variadic calls
    template<typename T, typename... Rest>
    typename std::enable_if<std::is_integral<T>::value, void>::type
                        increment(T sev, Rest &&... rest)
                         { ds_+=sev; increment(std::forward<decltype(rest)>(rest)...); }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<__is_dbg_propagatable__<Subclass>(0), void>::type
                        increment(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity()+1);       // compel s' severity to mine
                         s.__dbg_propagate__(s.DBG().severity());   // propagate down s' members
                         increment(std::forward<decltype(rest)>(rest)...);
                        }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<
        not __is_dbg_propagatable__<Subclass>(0) and std::is_class<Subclass>::value, void>::type
                        increment(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity()+1);       // compel s' severity to mine
                         increment(std::forward<decltype(rest)>(rest)...);
                        }

    // these declarations are for SERDES interface to work
    short &             value(void) { return ds_; }
    const short &       value(void) const { return ds_; }

 protected:
    short               ds_{0};                                 // my debug severity offset

 private:
    #define MONTH Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    ENUMSTR( Month, MONTH )

    static short        udl_;                                   // user debug level - set by user
    static bool         indented_;                              // is prompt indented?
    static std::string  indent_;                                // chars used for indenting (prefix)
    static std::string  alt_indent_;                            // chars used for indenting (prefix)
    static std::string  suffix_;                                // suffix (last of debug's promp)
    static bool         ts_;                                    // build time-stamp into prompt?
    static bool         ms_;                                    // use ms in time-stamp
    static bool         us_;                                    // use us in time-stamp
    static std::mutex   mtx_;                                   // default mutex used by debug
    static std::mutex * mp_;                                    // pointer to currently used mutex
  static std::ostream * op_;                                    // pointer to currently ostream
    static bool         ft_;                                    // filter-in(true) or -out(false)
  static std::vector<std::string>
                        filter_;                                // facility filter

    static const std::string
                        timestamp_(void);
    static const std::string
                        stamp_str_(time_t t_stamp);
    static bool         match_(const char *f);
};

STRINGIFY( Debug::Month, MONTH)
#undef MONTH

short                   Debug::udl_{0};                         // 0: debugs disabled
bool                    Debug::indented_{true};                 // by default prompt is indented
std::string             Debug::indent_{DBG_INDENT};
std::string             Debug::alt_indent_{DBG_ALT_INDENT};
std::string             Debug::suffix_{DBG_SUFFIX};
bool                    Debug::ts_{false};
bool                    Debug::ms_{false};
bool                    Debug::us_{false};
std::mutex              Debug::mtx_;
std::mutex *            Debug::mp_{&Debug::mtx_};
std::ostream *          Debug::op_{&std::cout};
bool                    Debug::ft_{false};                      // filter-in by default
std::vector<std::string>Debug::filter_;


bool Debug::operator()(short d, const char * fn) const {
 // check if combined severity (d: debug block severity + ds_: this debug severity offset)
 // is higher than debug level set by user:
 // 0 - highest severity, 1 - lower, etc. User debug level must be set to value
 // higher then debug's severity. e.g.: level set to 2, will enable all debugs with
 // combined severities 0 and 1. debug level 0 disables all debugs (assuming no negative
 // debug severities used)
 #ifdef NDEBUG                                                  // compiled with -DNDEBUG option
 return false;
 #endif

 if(d + ds_ >= level()) return false;                           // severity lower than set by user
 if(fn == nullptr) return true;                                 // user does not want printed prompt
 if(not match_(fn)) return false;                               // filter does not match

 dout() << Debug::indent_ << prompt(fn, d);
 return true;
}


std::string Debug::prompt(const char *fn, int msgSev, bool useTs, bool useAltPfx) const {
 // useAltPfx: use alternative prefix (indent)?
 // useTs: use Debug::ts to drive time-stamp update?
 static std::stringstream so;
 so.str("");

 if(indented())
  for(int i=severity()+msgSev; i>0; --i)
   so << (useAltPfx? Debug::alt_indent_: Debug::indent_);
 so << fn << "()";
 if(stamped())
  so << " [" << timestamp_() << "]";
 so << Debug::suffix_;
 return so.str();
}


const std::string Debug::timestamp_(void) {
 // build a time-stamp of the local TZ, possibly including ms and us
 static struct timeval t;
 static std::stringstream so;

 so.str("");
 gettimeofday(&t, nullptr);

 so << stamp_str_(t.tv_sec);
 if(ms_) {
  so << '.' << std::setfill('0') << std::setw(3) << t.tv_usec/1000;
  if(us_)
   so << '.' << std::setfill('0') << std::setw(3) << t.tv_usec%1000;
 }

 return so.str();
};


const std::string Debug::stamp_str_(time_t t_stamp) {
 // build a date-time-stamp in the format: YYYY-MMM-DD hh:mm:ss
 static std::stringstream so;
 static tm * tmp;

 so.str("");
 tmp = localtime(&t_stamp);

 so << tmp->tm_year+1900 << '-'
    << Debug::Month_str[tmp->tm_mon] << '-'
    << std::setfill('0') << std::setw(2) << tmp->tm_mday << ' '
    << std::setfill('0') << std::setw(2) << tmp->tm_hour << ':'
    << std::setfill('0') << std::setw(2) << tmp->tm_min << ':'
    << std::setfill('0') << std::setw(2) << tmp->tm_sec;

 return so.str();
}


bool Debug::match_(const char *fn) {
 // filter-in or filter-out any matches setup in Debug::filter_
 // if Debug::filter is not set, always return true
 // match occurs from the beginning of the function's name only.
 if( filter_.empty() )                                          // if filter wasn't setup
  return true;                                                  // match is always true

 std::string fname(fn);
 if(Debug::ft_) {                                                // filter-out
  for(auto &f: Debug::filter_)
   { if(fname.find(f) == 0) return false; }
  return true;
 }

 // filter-in
 for(auto &f: Debug::filter_)
  { if(fname.find(f) == 0) return true; }
 return false;
}

















