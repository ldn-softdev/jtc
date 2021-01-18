/*
 * Created by Dmitry Lyssenko
 *
 * A simple class to intercept and handle common interrupt signals
 * - intercepted signals will produce a tracebacks
 * - INT signal (once installed) can handle signal deferrals
 *
 * Synopsis:
 *      Signal sgn;             // or: Signal sgn(Signal::SEGV, Signal::BUS, ...);
 *                              // or: Signal sgn(all);
 *      // ...
 *      sgn.install(Signal::INT);
 *
 *      sgn.defer_ui();             // do not let user interrupt with INT signal
 *      // ... some non-interruptible code
 *      sgn.rehab_ui();             // if INT was received, it will be applied at this time
 */

#pragma once

#include <signal.h>     // signal, raise, sig_atomic_t
#include <execinfo.h>   // backtrace_symbols_fd()
#include <unistd.h>
#include <cxxabi.h>
#include <iomanip>
#include <vector>
#include <initializer_list>
#include "extensions.hpp"

#define STACK_DEPTH 128





// provides back-tracing for defined signals
// and allows deferring SIGINT
class Signal {
    typedef void (Signal::*mptr)(void);
    typedef void func(int);

 public:
    // define required signals, extend once needed
    #define __SIGNALS__ \
                HUP, \
                INT, \
                QUIT, \
                ILL, \
                TRAP, \
                ABRT, \
                FPE, \
                BUS, \
                SEGV, \
                SYS, \
                PIPE, \
                ALRM, \
                TERM, \
                XCPU, \
                XFSZ, \
                VTALRM, \
                PROF, \
                USR1, \
                USR2
    ENUMSTC(eSignal, __SIGNALS__);

    #define __INTCPT__ \
                none, \
                all
    ENUM(eInterception, __INTCPT__);
    #undef __INTCPT__

    // Constructor will initialize data for all defined above signals:
    // vsi holds installation handlers for all signals
    // vri holds restore handlers for all signals
    // vpi holds intercepted (prior) signal handlers
    #define XMACRO(X) \
                vsi_.push_back(& Signal::SIG##X##_install); \
                vri_.push_back(& Signal::SIG##X##_restore); \
                vpi_.push_back(& Signal::dummy_);

                        Signal(eInterception x = none) {        // no signals intercepted by dflt.
                         XMACRO_FOR_EACH(__SIGNALS__);
                         if(x == all) install_all();
                        }
                        template<typename... Sgnls>
                        Signal(Sgnls... sgnls) {
                         XMACRO_FOR_EACH(__SIGNALS__)
                         install(sgnls...);
                        }
                       ~Signal(void) { restore_all(); }
    #undef XMACRO

    // define installer and restore handlers for all signals:
    // e.g.: XMACRO(INT) expands to:
    // void SIGINT_install(void) { vpi_[INT] = signal(SIGINT, Signal::SIGINT_handler); }
    // void SIGINT_restore(void) { signal(SIGINT, vpi_[INT]); vpi_[INT] = &Signal::dummy; }
    #define XMACRO(X) \
        void SIG ## X ##_install(void) \
            { vpi_[X] = signal(SIG##X, Signal::SIG ## X ##_handler); } \
        void SIG ## X ##_restore(void) \
            { signal(SIG ## X, vpi_[X]); vpi_[X] = &Signal::dummy_; }
    XMACRO_FOR_EACH(__SIGNALS__)
    #undef XMACRO


    // SIGINT_handler's methods
    bool                defer_ui(void) { user_interrupt = 1; return true; }
    void                rehab_ui(bool x) { if(x) return rehab_ui(); }
    void                rehab_ui(void)
                         { user_interrupt = 0; if(ui_received) SIGINT_handler(-SIGINT); }
    bool                is_ui_pending(void) const { return ui_received; }
    bool                is_ui_deferred(void) const { return user_interrupt > 0; }

    template<typename S, typename... Args>
    void                install(S sgnl, Args... rest) { install_(sgnl); install(rest...); }
    void            	install(eSignal s) { install_(s); }     // variadic install() termination
    template<typename S, typename... Args>
    void                restore(S sgnl, Args... rest) {restore_(sgnl); restore(rest...); }
    void                restore(eSignal s) { restore_(s); }

    bool                install_all(void) {
                         size_t sgn = 0;
                         for(auto i: vsi_)
                          if(vpi_[sgn++] == &Signal::dummy_)    // i.e. hdlr not yet installed
                           (this->*i)();
                         return true;
                        }
    void                restore_all(bool x = true) {
                         if(x == false) return;
                         size_t sgn = 0;
                         for(auto r: vri_)
                          if(vpi_[sgn++] != &Signal::dummy_)    // i.e. hdlr was installed
                           (this->*r)();
                        }

    static std::string  int_deferred;
    static std::string  int_activated;

 private:
    std::vector<mptr>   vsi_;                                   // vector of signals installers;
    std::vector<mptr>   vri_;                                   // vector of signals restorers;

    void                install_(eSignal s)
                         { if(vpi_[s] == &Signal::dummy_) (this->*vsi_[s])(); }
    void                restore_(eSignal s)
                         { if(vpi_[s] != &Signal::dummy_) (this->*vri_[s])(); }

   static std::vector<func*> vpi_;                              // prior (preserved) interrupts;
    static sig_atomic_t user_interrupt;                         // 0: enabled, >0: disabled
    static bool         ui_received;                            // user SIGINT received
    static void         dummy_(int);                            // use instead of a nullptr

    static void         rip_(const char *str, eSignal esgn, int sig);
    static void         demangle_(const char *bt, size_t n);


    // defined handlers
    #define XMACRO(X) \
        static void     SIG ## X ## _handler(int sig);
    XMACRO_FOR_EACH(__SIGNALS__)
    #undef XMACRO
};


STRINGIFY(Signal::eSignal, __SIGNALS__)
//#undef __SIGNALS__                                            // spare signals definitions


std::vector<Signal::func*> Signal::vpi_;
sig_atomic_t Signal::user_interrupt{0};
bool Signal::ui_received{false};
void Signal::dummy_(int) {}                                     // unique ptr, instead of nullptr
std::string Signal::int_deferred{"!!! interrupt received but deferred, wait until reinstated !!!"};
std::string Signal::int_activated{"aborted due to user interrupt received: "};



// add handlers below requiring special handling
void Signal::SIGINT_handler(int sig) {
 // sig > 0: interrupt received while wasn't deferred
 if( user_interrupt++ > 0) {
  ui_received = true;
  std::cerr << Signal::int_deferred << std::endl;
  return;
 }
 std::cerr << Signal::int_activated << (sig < 0? "deferred " : "")
           << "SIG" << STRENM(eSignal, INT) << " (" << abs(sig) << ")" << std::endl;
 if(vpi_[INT] != nullptr) vpi_[INT](SIGINT);                    // pass on the interrupt
 exit(EXIT_FAILURE);
}



#define STD_HLR(STRSGN, SGNLS) \
         void Signal::SIG ## SGNLS ## _handler(int sig) \
          { rip_(STRSGN, SGNLS, sig); }

 // add handlers below with a general/common handling flow
 STD_HLR("hangup", HUP)
 STD_HLR("quit", QUIT)
 STD_HLR("illegal instruction", ILL)
 STD_HLR("trace/bpt trap", TRAP)
 STD_HLR("abort trap", ABRT)
 STD_HLR("floating point exception", FPE)
 STD_HLR("bus error", BUS)
 STD_HLR("segmentation fault", SEGV)
 STD_HLR("bad system call", SYS)
 STD_HLR("pipe with no reader", PIPE)
 STD_HLR("alarm clock", ALRM)
 STD_HLR("terminated", TERM)
 STD_HLR("cputime limit exceeded", XCPU)
 STD_HLR("filesize limit exceeded", XFSZ)
 STD_HLR("virtual timer expired", VTALRM)
 STD_HLR("profiling timer expired", PROF)
 STD_HLR("user defined signal 1", USR1)
 STD_HLR("user defined signal 2", USR2)

#undef STD_HLR



void Signal::rip_(const char *str, eSignal esgn, int sig) {
 // terminate procedure, prints the stack trace
 std::cerr << str << ": SIG" << STRENM(eSignal, esgn) << " (" << abs(sig) << ")" << std::endl;

 char ** bt_symbols{nullptr};
 std::vector<void*> vtrace;
 try {                                                          // obtain current stack,
  vtrace.resize(STACK_DEPTH);                                   // (ensure it fits entirely
  size_t btsize, vtsize;
  do {
   vtsize = vtrace.size();
   btsize = backtrace(vtrace.data(), vtsize);
   if(btsize == vtsize)                                         // i.e. entire array filled
    vtrace.resize(vtrace.size() * 2);                           // double it and retry
  } while(btsize == vtsize);
  vtrace.resize(btsize);                                        // resize to actual stack depth
 }
 catch(...){ };

 bt_symbols = backtrace_symbols(vtrace.data(), vtrace.size());  // get symbols
 if(bt_symbols == nullptr or vtrace.empty()) {
  std::cerr << "NO MEMORY LEFT TO BUILD THE BACKTRACE!" << std::endl;
  vtrace.clear();
 }

 for(size_t i = 1; i < vtrace.size(); ++i) {                    // print all the frames to stderr
  #ifdef __MACH__
   demangle_(bt_symbols[i], i - 1);
  #else
   std::cerr << bt_symbols[i] << std::endl;
  #endif
 }

 exit(EXIT_FAILURE);
}



void Signal::demangle_(const char *bt, size_t n) {
 // find function name in bt (it's after 3'rd space) and demangle

 #define __BTPARTS__ \
            frame, \
            module, \
            address, \
            function, \
            offset, \
            EOBTP
 ENUM(eBtParts, __BTPARTS__);
 #undef __BTPARTS__ \

 typedef std::pair<std::string, size_t> btparts_t;              // <symbolic part of bt, width>
 std::vector<btparts_t> parts;
 const char *prtptr = bt;                                       // parts pointer
 auto print_bt = [n, bt](void) {
                  std::cerr << n << (n % 10 == 9? " ": "")      // compensate for discrepancy
                            << (isdigit(*bt)? strchr(bt, ' '): " ")
                            << (isdigit(*bt)? "": bt) << std::endl;
                  return;
                 };
 try {                                                          // reserve may throw ...
  parts.reserve(EOBTP);
  for(size_t i = 0; i < offset; ++i) {                          // extract all symbolic parts
   auto next_prt = strchr(prtptr, ' ');                         // skip to end of part
   if(next_prt == nullptr) break;                               // unexpected
   std::string part(prtptr, next_prt - prtptr);

   while(*next_prt == ' ') ++next_prt;                          // find a a next part
   if(next_prt == nullptr) break;                               // unexpected

   parts.emplace_back(part, next_prt - prtptr + (i == 0 and n % 10 == 9? 1: 0));
   prtptr = next_prt;
  }
 }
 catch(...) {}

 if(parts.size() < offset) return print_bt();                   // incomplete parts parsing
 parts.emplace_back(prtptr + 2, 0);                             // add offset, width is unimportant

 int status;
 char* demangled = abi::__cxa_demangle(parts[function].first.c_str(), nullptr, nullptr, &status);

 if (status == 0) {                                             // successful c++ demangling
  parts[function].first = demangled;
  parts[frame].first = std::to_string(n);

  for(size_t i = 0; i < function; ++i)                            // print all parts except offset
   std::cerr << std::setw(parts[i].second) << std::left << parts[i].first;

  std::cerr << parts[function].first << " + " << parts[offset].first << std::endl;
  free(demangled);
  return;
 }

 if(demangled) free(demangled);
 return print_bt();                                             // unsuccessful c++ demangling
}





/* define SIGINT guard and block operator for convenience
 *
 * Synopsis:
 *      Signal s(Signal::INT);
 *
 *      {
 *       GUARD(s.defer_ui, s.rehab_ui);
 *       // ... some non-interruptible code
 *      }
 *
 *      // ... or:
 *
 *      DEFER_SIGINT(s) {
 *       // ... some non-interruptible code
 *      }
 *
 */

// MACRO to run SIGINT-deferred code block
#define DEFER_SIGINT(X) \
    for(X.defer_ui(); X.is_ui_deferred(); X.rehab_ui())





#undef STACK_DEPTH

























