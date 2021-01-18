/*
 * Created by Dmitry Lyssenko
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
 * DBG().level(4);          // set global debug depth to 4: this will cause DBG(0),
 *                          // DBG(1), DBG(2) and DBG(3) output (given severity was 0)
 * DBG().severity(3);       // set my Debug's severity to 3: acts as an offset for
 *                          // debugging: my DBG(0) becomes like DBG(3)
 * DBG().increment(-1)      // increment my Debug's severity by -1, (i.e. becomes 2)
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
 *  DBG_PMT(n)          // alias for DBG_PROMPT
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
#include <climits>
#include <functional>
#include <unistd.h>             // STDOUT_FILENO - for term width
#include <sys/ioctl.h>          // ioctl() and TIOCGWINSZ - for term width
#include "macrolib.hpp"
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
#define DEBUGGABLE(ARGS...) __DEBUGGABLE_ARG__(IF_ARGS(ARGS), ARGS)
#define __DEBUGGABLE_ARG__(X, ARGS...) __DEBUGGABLE_IF__(X, ARGS)
#define __DEBUGGABLE_IF__(X, ARGS...) __DEBUGGABLE_##X##__(ARGS)
#define __DEBUGGABLE_PROPAGATE__(X) X.__dbg__.severity(__sev__ + 1, X);
#define __DEBUGGABLE_false__() \
    Debug   __dbg__;
#define __DEBUGGABLE_true__(ARGS...) \
    Debug   __dbg__{*this}; /* ensure debug severity proliferation upon init */\
    void    __dbg_propagate__(int __sev__) \
             { MACRO_TO_ARGS(__DEBUGGABLE_PROPAGATE__, ##ARGS) }


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

#define __DBG_0_ARG__() __dbg__     /* DBG(): access to debug object */
#define __DBG_1_ARG__(X)            /* DBG(N) {..}: debug operator, prints debug indention */\
    if( __dbg__(X) ) \
     TLOCK(__dbg__.mutex()) \
      if( __dbg__(X, __func__) )                                // now print the prompt
#define __DBG_2_ARG__(O, X)         /* DBG(F, N) {..}: using debug operator from F */\
    if( O.__dbg__(X) ) \
     TLOCK(O.__dbg__.mutex()) \
      if( O.__dbg__(X, __func__) )                              // now print the prompt
#define __DBG_4TH_ARG__(arg1, arg2, arg3, arg4, ...) arg4
#define __DBG_CHOOSER__(ARGS...) \
    __DBG_4TH_ARG__(dummy, ##ARGS, __DBG_2_ARG__, __DBG_1_ARG__, __DBG_0_ARG__)
#define DBG(ARGS...) __DBG_CHOOSER__(ARGS)(ARGS)


#ifdef NDEBUG                                                   // compiled with -DNDEBUG option
 #undef __DBG_1_ARG__
 #undef __DBG_2_ARG__
 #define __DBG_1_ARG__(...) if(false)
 #define __DBG_2_ARG__(...) if(false)
#endif



// DOUT() macros and init Debug's global macros:
// 1. DOUT() - an alias for the current output class, optionally defined via use_ostream()
// 2. DOUT(X) - uses class X to utilize its DOUT() output alias
//
// DOUT() has 3 output manipulators:
//  o Debug::btw - when a printing line requires truncating at the back by the term width
//  o Debug::ftw - when a printing line requires truncating at the front by the term width
//  o Debug::ctw(x) - when a printing line requires center-adjusting of the output at the
//                    position X and truncating might occur at the front and/or back
//
#define __DOUT_0_ARG__() (DBG().dout())
#define __DOUT_1_ARG__(X) (X.DBG().dout())
#define __DOUT_3RD_ARG__(arg1, arg2, arg3, ...) arg3
#define __DOUT_CHOOSER__(ARGS...) \
    __DOUT_3RD_ARG__(dummy, ##ARGS, __DOUT_1_ARG__, __DOUT_0_ARG__)
#define DOUT(ARGS...) __DOUT_CHOOSER__(ARGS)(ARGS)

// init Debug's global macros
#define DBG_INDENT "."                                          // default debug's indent prefix
#define DBG_ALT_INDENT " "                                      // alternative prefix
#define DBG_SUFFIX ", "                                         // default debug's suffix
#define DBG_PROMPT(X) DBG().prompt(__func__, X + 1)
#define DBG_PMT(X) DBG().prompt(__func__, X + 1)
#define DBG_WDTH 100                                            // default term width for debug
#define NDBG SHRT_MAX/2
// NDBG definition is just a ridiculously low debug severity - to be used when certain debugs have
// to be suppressed; /2 instead of using SHRT_MAX is to avoid value overflow when delegating debugs

// below definition let checking whether '__dbg_propagate__()' method is present or not:
template<typename T>
constexpr auto __is_dbg_propagatable__(int)
                -> decltype(std::declval<T>().__dbg_propagate__(0), bool())
                { return true; }
template<typename T>
constexpr bool __is_dbg_propagatable__(...) { return false; }





class __Dbg_flow__;

class Debug {
 friend __Dbg_flow__;

    #define TOS_TYPE        /* indent type: native (default '.'), or alternative*/\
                No_trunc, \
                Trunc_back, \
                Trunc_front, \
                Trunc_both
    ENUM(TosType, TOS_TYPE)
    #undef TOS_TYPE

    struct Ctw {};                                              // used only as enablement for ctw

    class DbgStream: public std::ostream {
     // custom class used for debug outputs (via dout()), it's required to plug-in
     // own std::stringbuf based custom class (DbgStringBuf) to handle processing of
     // term-width truncated debug outputs (via Debug::cut modifier). The modifier
     // has a transient effect until `std::endl`
        class DbgStringBuf: public std::stringbuf {
         public:
                                DbgStringBuf() = default;
                               ~DbgStringBuf()
                                 { if (pbase() != pptr()) sync_(); }

            // sync is called when `std::endl` is processed by ostream/stringbuf
            virtual int         sync() { sync_(); return 0; }
            void                use_ostream(std::ostream & os) { osp_ = &os; }

         private:
            std::ostream *      osp_{&std::cout};
            void                sync_(void);
            void                utf8_trim_back_(std::string &, size_t width);
            size_t              utf8_trim_front_(std::string &, size_t width);
        };

     public:
                            DbgStream(void): std::ostream(&sbuf_) {}
        void                use_ostream(std::ostream & os) { sbuf_.use_ostream(os); }

     private:
        DbgStringBuf        sbuf_;
    };

 public:

    #define IND_TYPE        /* indent type: native (default '.'), or alternative*/\
                Native, \
                Alternative
    ENUM(Indention, IND_TYPE)
    #undef IND_TYPE


                        Debug(void) = default;
                        Debug(short x): ds_{x} {};
    template<class X>   Debug(X &x) { x.DBG().severity(x); };

    short               level(void) const { return udl_; }
    Debug &             level(short ul) { udl_=ul; return *this; }
    bool                indented(void) const { return indented_; }
    Debug &             indented(bool x) { indented_ = x; return *this; }
    std::string         prefix(void) const { return indent_; }
    void                prefix(const std::string & i) const { indent_ = i; }
    Debug &             prefix(const char *i) { indent_ = i; return *this; }
    std::string         alt_prefix(void) const { return alt_indent_; }
    void                alt_prefix(const std::string & i) const { alt_indent_ = i; }
    Debug &             alt_prefix(const char *i) { alt_indent_ = i; return *this; }
    Debug &             suffix(const char *s) { suffix_ = s; return *this; }
    bool                stamped(void) const { return ts_; }
    Debug &             stamped(bool x) { ts_ = x; return *this; }
    Debug &             stamp_ms(bool x = true)
                         { ms_ = x; if(x == true) stamped(x); return *this; }
    Debug &             stamp_us(bool x = true)
                         { us_ = x; if(x == true) stamp_ms(); return *this; }
    Debug &             stamp_delta(bool x = true) { delta_ = x; return *this; }
    Debug &             filter_out(bool x) { ft_ = x; return *this; }
    Debug &             filter(const char *s) { filter_.push_back(s); return *this; }
    Debug &             reset_filter(void) { filter_.clear(); filter_out(false); return *this; }
    std::mutex &        mutex(void) const { return *Debug::mp_; }
    Debug &             mutex(std::mutex & mtx) { mp_ = &mtx; return *this; }
    Debug &             reset_mutex(void) { mp_ = &mtx_; return *this; }
    std::ostream &      dout(void) const { return dos_; }
    Debug &             use_ostream(std::ostream & os) { dos_.use_ostream(os); return *this; }
    Debug &             reset_ostream(void) { dos_.use_ostream(std::cout); return *this; }
    bool                operator()(short d, const char *fn=nullptr) const;// check and print prompt
    std::string         prompt(const char *fn, int ms=0,
                               Indention it_type = Indention::Native) const;
    size_t              ctw_adjust(void) { return Debug::adj_; }
                        // after invoking Debug::ctw(pos), ctw_adjust() will return an offset
                        // where position pos was displayed

                        // severity() and increment() definitions implement mechanism which
                        // allows propagating calling class' debug policy to its debuggable
                        // members
    int                 severity(void) const { return ds_; }    // also used to terminate variadic
    template<typename T, typename... Rest>
    typename std::enable_if<std::is_integral<T>::value, void>::type
                        severity(T sev, Rest &&... rest)
                         { ds_ = sev; severity(std::forward<decltype(rest)>(rest)...); }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<__is_dbg_propagatable__<Subclass>(0), void>::type
                        severity(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity() + 1);     // compel s' severity to mine
                         s.__dbg_propagate__(s.DBG().severity());   // propagate down s' members
                         severity(std::forward<decltype(rest)>(rest)...);
                        }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<
        not __is_dbg_propagatable__<Subclass>(0) and std::is_class<Subclass>::value, void>::type
                        severity(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity() + 1);     // compel s' severity to mine
                         severity(std::forward<decltype(rest)>(rest)...);
                        }

    void                increment(void) {}                       // to terminate variadic calls
    template<typename T, typename... Rest>
    typename std::enable_if<std::is_integral<T>::value, void>::type
                        increment(T sev, Rest &&... rest)
                         { ds_ += sev; increment(std::forward<decltype(rest)>(rest)...); }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<__is_dbg_propagatable__<Subclass>(0), void>::type
                        increment(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity() + 1);     // compel s' severity to mine
                         s.__dbg_propagate__(s.DBG().severity());   // propagate down s' members
                         increment(std::forward<decltype(rest)>(rest)...);
                        }
    template<typename Subclass, typename... Rest>
    typename std::enable_if<
        not __is_dbg_propagatable__<Subclass>(0) and std::is_class<Subclass>::value, void>::type
                        increment(Subclass & s, Rest &&... rest) {
                         if(&s.DBG() != this)                   // updating foreign debuggable
                          s.DBG().severity(severity() + 1);     // compel s' severity to mine
                         increment(std::forward<decltype(rest)>(rest)...);
                        }

    // these declarations are for SERDES interface to work
    short &             value(void) { return ds_; }
    const short &       value(void) const { return ds_; }

    // term_width() is used by Debug class when dbg-output modifiers btw, ftw, ctw are invoked
    static size_t       term_width(bool forced = false);        // read terminal width

  static std::ostream & btw(std::ostream &os)       // cut output's end by term width
                         { Debug::tos_ = Debug::Trunc_back; return os; }

  static std::ostream & ftw(std::ostream &os)       // cut output's front by term width
                         { Debug::tos_ = Debug::Trunc_front; return os; }

  friend std::ostream & operator<<(std::ostream & os, Debug::Ctw)// ctw is unused, but it's
                         { return os; }             // required for ADL type of use
  static  Ctw           ctw(size_t pos)             // center-adjust  and cut by term width
                         { Debug::tos_ = Debug::Trunc_both; Debug::adj_ = pos; return Ctw{}; }


 protected:
    short               ds_{0};                                 // my debug severity offset

 private:
    #define MONTH Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    ENUMSTR( Month, MONTH )
    #undef MONTH

    static short        udl_;                                   // user debug level - set by user
    static bool         indented_;                              // is prompt indented?
    static std::string  indent_;                                // used for indenting (prefix)
    static std::string  alt_indent_;                            // used for alt. indenting (prefix)
    static std::string  suffix_;                                // suffix (last of debug's promp)
    static bool         ts_;                                    // build time-stamp into prompt?
    static bool         ms_;                                    // use ms in time-stamp
    static bool         us_;                                    // use us in time-stamp
    static bool         delta_;                                 // use delta time-stamp
    static timeval      lts_;                                   // last time stamp, for delta_
    static std::mutex   mtx_;                                   // default mutex used by debug
    static std::mutex * mp_;                                    // pointer to currently used mutex
  static DbgStream      dos_;                                   // debug's output stream
    static bool         ft_;                                    // filter-in(true) or -out(false)
  static std::vector<std::string>
                        filter_;                                // facility filter
    static size_t       tw_;                                    // term width
    static size_t       lups_;                                  // last used prompt size
    static TosType      tos_;                                   // flag: truncate output stream
    static size_t       adj_;                                   // value for Trunc_both

    const std::string   timestamp_(void) const;
    const std::string   stamp_str_(time_t t_stamp) const;
    bool                match_(const char *f) const;
};



short                   Debug::udl_{0};                         // 0: debugs disabled
bool                    Debug::indented_{true};                 // by default prompt is indented
std::string             Debug::indent_{DBG_INDENT};
std::string             Debug::alt_indent_{DBG_ALT_INDENT};
std::string             Debug::suffix_{DBG_SUFFIX};
bool                    Debug::ts_{false};
bool                    Debug::ms_{false};
bool                    Debug::us_{false};
bool                    Debug::delta_{false};
timeval                 Debug::lts_;                            // last time stamp
std::mutex              Debug::mtx_;
std::mutex *            Debug::mp_{&Debug::mtx_};
Debug::DbgStream        Debug::dos_;
bool                    Debug::ft_{false};                      // filter-in by default
std::vector<std::string>Debug::filter_;
size_t                  Debug::tw_{0};                          // term width
size_t                  Debug::lups_{0};                        // last used prompt size
Debug::TosType          Debug::tos_{Debug::No_trunc};           // truncate output stream
size_t                  Debug::adj_;                            // adjustment for Trunc_both




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
 if(fn == nullptr) return true;                                 // user wants no printed prompt
 if(not match_(fn)) return false;                               // filter does not match

 dout() << prompt(fn, d + 1);
 return true;
}


std::string Debug::prompt(const char *fn, int msg_sev, Indention ind_type) const {
 // useAltPfx: use alternative prefix (indent)?
 // useTs: use Debug::ts to drive time-stamp update?
 std::stringstream so;

 if(indented())
  for(int i = severity() + msg_sev; i > 0; --i)
   so << (ind_type == Native? Debug::indent_: Debug::alt_indent_);
 if(stamped())
  so << "[" << timestamp_() << "] ";
 so << fn << "()";
 so << Debug::suffix_;
 Debug::lups_ = so.str().size();
 return so.str();
}



size_t Debug::term_width(bool forced) {
 // read term width upon the first call, and on all subsequent return the read value
 struct winsize ws;
 auto read_ws = [&ws]{ return ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); };

 if(forced)                                                     // force ioctl
  { Debug::tw_ = read_ws() == 0? ws.ws_col: DBG_WDTH; return Debug::tw_; }

 if(Debug::tw_ != 0) return tw_;                                // != 0 => ioclt already called
 Debug::tw_ = read_ws() == 0? ws.ws_col: DBG_WDTH;
 return Debug::tw_;
}



const std::string Debug::timestamp_(void) const {
 // build a time-stamp of the local TZ, possibly including ms and us
 std::stringstream so;

 timeval t, ts;
 gettimeofday(&t, nullptr);
 ts = t;                                                        // timeval saved, for delta_

 if(delta_) {
  if(lts_.tv_sec + lts_.tv_usec == 0) lts_ = t;
  if(lts_.tv_usec > t.tv_usec)
   { t.tv_usec += 1000000; --t.tv_sec; }
  t.tv_usec -= lts_.tv_usec;
  t.tv_sec -= lts_.tv_sec;
 }

 so << stamp_str_(t.tv_sec);
 if(ms_) {
  so << '.' << std::setfill('0') << std::setw(3) << t.tv_usec/1000;
  if(us_)
   so << '.' << std::setfill('0') << std::setw(3) << t.tv_usec%1000;
 }

 if(delta_) lts_ = ts;
 return so.str();
};



const std::string Debug::stamp_str_(time_t t_stamp) const {
 // build a date-time-stamp in the format: YYYY-MMM-DD hh:mm:ss
 std::stringstream so;

 if(delta_) {
  so << std::setw(3) << std::setfill('0') << t_stamp;
  return so.str();
 }

 tm * tmp = localtime(&t_stamp);

 so << tmp->tm_year + 1900 << '-'
    << Debug::Month_str[tmp->tm_mon] << '-'
    << std::setfill('0') << std::setw(2) << tmp->tm_mday << ' '
    << std::setfill('0') << std::setw(2) << tmp->tm_hour << ':'
    << std::setfill('0') << std::setw(2) << tmp->tm_min << ':'
    << std::setfill('0') << std::setw(2) << tmp->tm_sec;

 return so.str();
}



bool Debug::match_(const char *fn) const {
 // filter-in or filter-out any matches setup in Debug::filter_
 // if Debug::filter is not set, always return true
 // match occurs from the beginning of the function's name only.
 if( filter_.empty() )                                          // if filter wasn't setup
  return true;                                                  // match is always true

 std::string fname(fn);
 if(Debug::ft_) {                                               // filter-out
  for(auto &f: Debug::filter_)
   { if(fname.find(f) == 0) return false; }
  return true;
 }

 // filter-in
 for(auto &f: Debug::filter_)
  { if(fname.find(f) == 0) return true; }
 return false;
}



void Debug::DbgStream::DbgStringBuf::sync_(void) {
 // process upon receiving std::endl (output buffered string)
 auto rst = [&](void) {                                         // do all housekeeping at the end
  if(str().size() > 1024)                                       // buffer grew too big,
   { std::stringbuf temp; swap(temp); }                         // reset buffer to a new one
  Debug::tos_ = Debug::No_trunc; Debug::lups_ = 0; str("");     // reset all that required
  osp_->flush();
 };

 if(Debug::tos_ == Debug::No_trunc)                             // no truncating
  { (*osp_) << str(); rst(); return; }

 std::string s = str();
 while(not s.empty() and (s.back() == '\r' or s.back() == '\n'))// remove trailing \n and/or \r
  s.pop_back();
 if(Debug::tos_ == Debug::Trunc_back) {                         // btw
  utf8_trim_back_(s, Debug::term_width());
  (*osp_) << s << std::endl;
  return rst();
 }

 std::string sp = s.substr(0, Debug::lups_);                    // sp: string's prompt
 s.erase(0, Debug::lups_);
 if(Debug::tos_ == Debug::Trunc_front) {                        // ftw
  utf8_trim_front_(s, Debug::term_width() - Debug::lups_);
  (*osp_) << sp << s << std::endl;
  return rst();
 }

 // must be Debug::Trunc_both                                   // ctw(..)
 if(Debug::adj_ >= s.size()) Debug::adj_ = s.size() - 1;
 std::string fp = s.substr(0, Debug::adj_);                     // front part
 s.erase(0, Debug::adj_);                                       // now s is a the back part
 Debug::adj_ =  utf8_trim_front_(fp, (Debug::term_width() - Debug::lups_ + 1) / 2);
 utf8_trim_back_(s, Debug::term_width() - Debug::lups_ - Debug::adj_ - 1);
 (*osp_) << sp << fp << s << std::endl;
 return rst();
}


void Debug::DbgStream::DbgStringBuf::utf8_trim_back_(std::string &s, size_t tw) {
 // trim input string's back by term width and add ellipsis if trimming occurs
 // this logic assumes that utf8 encoding is not broken!
 static const char sfx[] = "...";
 char * ptr = &s.front();
 size_t ts{0};                                                  // trim size
 for(size_t uoffset = 0; uoffset < tw; ++ptr) {
  unsigned char chr = *ptr;
  if(chr == '\0') return;                                       // does not require trimming
  if(uoffset == tw - sizeof(sfx)) ts = ptr - &s.front();        // possibly a trimming point
  if(chr > 0 and chr < 128) {                                   // non-unicode char
   if(chr == '\t' ) *ptr = chr = ' ';                           // tabs shouldn't be in debugs
   if(chr == '\r' or chr == '\n') *ptr = chr = '|';             // intermediate new line chars
   if(chr >= ' ') ++uoffset;                                    // count only printable chars
   continue;
  }                                                             // else - unicode bytes
  if((static_cast<unsigned char>(chr) & 0xFC) == 0xD8)          // high surrogate, must be followed
   { ptr += 3; ++uoffset; continue; }                           // by a low surrogate pair
  while((static_cast<unsigned char>(chr) & 0xC0) != 0x80)       // otherwise process unicode per
   { ++ptr; chr <<= 1; }                                        // number of bytes
  ++uoffset;
 }
 s.resize(ts);
 s += sfx;
}



size_t Debug::DbgStream::DbgStringBuf::utf8_trim_front_(std::string &s, size_t tw) {
 // trim input string's front by term width and add ellipsis if trimming occurs
 // this logic assumes that utf8 encoding is not broken!
 if(s.empty()) return 0;
 static const char pfx[] = "...";
 char * ptr = &s.back();
 size_t ts{0}, uoffset{0};                                      // trim size, offset in utf8 chars
 for(; uoffset < tw; --ptr) {
  char chr = *ptr;
  if(ptr == &s.front()) return uoffset + 1;                     // does not require trimming
  if(uoffset == tw - sizeof(pfx)) ts = &s.back() - ptr;         // possibly a trimming point
  if(chr >= 0) {                                                // non-unicode char
   if(chr == '\t' ) *ptr = chr = ' ';                           // tabs shouldn't be in debugs
   if(chr == '\r' or chr == '\n') *ptr = chr = '|';             // intermediate new line chars
   if(chr >= ' ') ++uoffset;                                    // count only printable chars
   continue;
  }                                                             // else - unicode bytes
  if((static_cast<unsigned char>(chr) & 0xC0) == 0x80) continue;// unicode trailing byte
  if((static_cast<unsigned char>(chr) & 0xFC) == 0xDC) ptr -= 2;// preceded by high surrogate
  ++uoffset;
 }
 while((static_cast<unsigned char>(s[s.size() - ts]) & 0xC0) == 0x80) --ts;
 s.erase(0, s.size() - ts);
 s = pfx + s;
 return tw - 1;
}



/* This is a guard-type class to facilitate debugs of the execution flow, utilized by macro
 *  defined in "dbgflow.hpp": it let tracing entry point and exit points
 *
 * SYNOPSIS:
 * typical usage (first line in every method/function whose flow to be debugged):
 * #include "dbgflow.hpp")              // works for DEBUGGABLE classes (methods)/functions only
 *
 * for non-debuggable classes requires a helper to be setup before usage:
 * #define DBGBL_REF debuggable_class   // provide a reference to a DEBUGGABLE class
 * ...
 * #include "dbgflow.hpp"
 *
 *
 * NOTES:
 *  o for effectuating this debug, a flag -DBG_FLOW to be passed upon compiling
 *  o this debug utilizes alt_prefix, so it's best to redefine a default value (which is " ")
 */


class __Dbg_flow__ {
 public:
                        __Dbg_flow__(const Debug & dbg, const char * fnc): dbg_(dbg), dfnc_(fnc) {
                         if(dbg_(__Dbg_flow__::ind_, nullptr)) {
                          ULOCK(dbg_.mutex());
                          dbg_.dout() << dbg_.prompt(dfnc_, __Dbg_flow__::ind_ + 1,
                                                     Debug::Indention::Alternative)
                                      << "-->Fn entered"
                                      << std::endl;
                         }
                         ++__Dbg_flow__::ind_;
                        }
                       ~__Dbg_flow__(void) {
                         --__Dbg_flow__::ind_;
                         if(dbg_(__Dbg_flow__::ind_, nullptr)) {
                          ULOCK(dbg_.mutex());
                          dbg_.dout() << dbg_.prompt(dfnc_, __Dbg_flow__::ind_ + 1,
                                                     Debug::Indention::Alternative)
                                      << "left Fn-->"
                                      << std::endl;
                         }
                        }

    static size_t       ind(void) { return __Dbg_flow__::ind_; }


 protected:
    const Debug &       dbg_;                                   // debug reference
    const char *        dfnc_;                                  // debug function

    static size_t       ind_;
};

size_t __Dbg_flow__::ind_{0};                                   // debug's indent






















