/*
 * Created by Dmitry Lyssenko.
 *
 * This class let managing threads easily:
 *  - provision (book) thread seats,
 *  - dispatch a new thread in a first available seat, or
 *  - dispatch a new thread in a given seat,
 *  - see if the seat is vacant,
 *  - await until a seat becomes vacant
 *
 * In addition, class provides following macros:
 * ULOCK(m) - lock mutex guard, until out of scope, e.g.: { ...; ULOCK(m) ... }
 * TLOCK(m) { ... } - lock mutex m for the followed execution operation/block
 *
 * SYNOPSIS:
 *  ThreadMaster tm;
 *  tm.run(task1, <list of task 1 arguments>);
 *  tm.run(task2, <list of task 2 arguments>);
 *  // ...
 *  tm.join();
 */


#pragma once

#include <vector>
#include <assert.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>              // std::forward
#include <climits>              // LLONG_MIN
#include "dbg.hpp"
#include "extensions.hpp"


#define SIZE_T(N) static_cast<size_t>(N)





class ThreadMaster {
 public:
    #define THROWREASON \
                Seat_must_be_vacant, \
                Bug_unreachable_point, \
                End_of_throw
    ENUMSTR(ThrowReason, THROWREASON)
    #undef THROWREASON

    #define SEATSTATE \
                Seat_vacant, \
                Seat_taken
    ENUM(SeatState, SEATSTATE)
    #undef SEATSTATE

                        ThreadMaster(size_t trds = 0) {         // DC
                         if(trds == 0)
                          trds = std::thread::hardware_concurrency();
                         vt_.resize(trds);
                         vs_.resize(trds, Seat_vacant);
                         ussm_ = std::unique_lock<std::mutex>(ssm_, std::defer_lock_t());
                        }
                        ThreadMaster(const ThreadMaster &) = delete;
                        ThreadMaster(ThreadMaster &&) = delete;

    std::mutex &        mtx(void) { return cm_; }               // common mutex
    void                lock(void) { mtx().lock(); }            // lock common mutex
    void                unlock(void) { mtx().unlock(); }        // unlock common mutex

    size_t              thread_count(void) {                    // # of running threads
                         size_t cnt = 0;
                         for(auto status: vs_) if(status == Seat_taken) ++cnt;
                         return cnt;
                        }
    size_t              seat_count(void)                        // # of empty/available seats
                         { return seats_total() - thread_count(); }
    size_t              seats_total(void) const                 // # of total/max slots/threads
                         { return vt_.size(); }
    size_t              size(void) const
                         { return seats_total(); }
    size_t              vacant_seat(void) {                     // find first vacant seat index
                         // vacant seat is seat where prior thread has finished
                         for(size_t seat = 0; seat < seats_total(); ++seat)
                          if(vs_[seat] == Seat_vacant) return seat;
                         return -1;                             // -1: no vacated seats yet
                        }
    size_t              my_seat(void) {                         // to be called within the thread
                         for(size_t seat = 0; seat < seats_total(); ++seat)
                          if(vs_[seat] == Seat_taken)
                           if(vt_[seat].get_id() == std::this_thread::get_id())
                            return seat;
                         return -1;                             // called not from a managed thread
                        }
    void                resize(size_t trds) {
                         ULOCK(mtx())
                         if(trds == 0)
                          trds = std::thread::hardware_concurrency();
                         vt_.resize(trds);
                         vs_.resize(trds, Seat_vacant);
                        }

    size_t              await_seat(void);                       // until a seat becomes available
    std::thread &       seat(size_t s) { return vt_.at(s); }    // get thread itself
    std::thread &       operator[](size_t s) { return seat(s); }

    template<typename... Args>
    size_t              run(Args&&... args);
    // runs a user task in a new thread:
    // - allocates first available seat (waits if none available)
    // - and return taken seat number

    template<typename... Args>
    size_t              run_seat(size_t seat, Args&&... args);
    // runs a user task in a new thread in a given seat (seat must be vacant!)
    // and returns given seat number

    void                start_sync(void);                       // "go" signal for start_sync(..)
    template<typename... Args>
    size_t              start_sync(Args&&... args);
    // starts a new thread to run user task but suspends until start_sync(void) called

    void                join(void)
                         { for(auto &t: vt_) if(t.joinable()) t.join(); }

    DEBUGGABLE()
    EXCEPTIONS(ThrowReason)                                     // see "extensions.hpp"

 protected:
    std::vector<std::thread>
                        vt_;                                    // vector of threads
 std::vector<SeatState> vs_;                                    // seat taken/vacant: vacant/taken
    std::mutex          cm_;                                    // common mutex (for users)

 private:
    std::mutex          rm_;                                    // release mutex (used by class)
std::condition_variable crm_;                                   // for thread arbitration (with rm_)

    std::mutex          ssm_;                                   // start_sync's mutex
std::unique_lock<std::mutex>
                        ussm_;                                  // ulock for start_sync's mutex
std::condition_variable ssc_;                                   // start_sync's condition_variable

    template <class Fn, class... Args>
    void                run_(Fn &&fn, Args&&... args);          // thread wrapper
    void                availale_seats_(void);                  // debug output only

};



template<typename... Args>
size_t ThreadMaster::run(Args&&... args) {
 // dispatch a new thread
 auto glambda = [&] (auto&&... arg) { return run_(std::forward<decltype(arg)>(arg)...); };

 size_t vs = await_seat();                                      // first vacant seat;
 ULOCK(mtx())
 availale_seats_();                                             // debug output
 DBG(2) DOUT() << "a new thread is taking up a vacant seat[" << vs << "]" << std::endl;
 vt_[vs] = std::thread(glambda, std::forward<decltype(args)>(args)...);
 vs_[vs] = Seat_taken;
 return vs;
}



template<typename... Args>
size_t ThreadMaster::run_seat(size_t vs, Args&&... args) {
 // dispatch a new thread in a given vacant seat
 auto glambda = [&] (auto&&... arg) { return run_(std::forward<decltype(arg)>(arg)...); };

 ULOCK(mtx())
 if(vs_[vs] == Seat_taken)
  throw EXP(Seat_must_be_vacant);

 DBG(2) DOUT() << "a new thread is taking up a vacant seat[" << vs << "]" << std::endl;
 vt_[vs] = std::thread(glambda, std::forward<decltype(args)>(args)...);
 vs_[vs] = Seat_taken;                                          // indicate seat taken
 return vs;
}



template<typename... Args>
size_t ThreadMaster::start_sync(Args&&... args) {
 // newly dispatched threads will wait for "go" signal by calling start_sync(void)
 auto glambda = [&] (auto&&... arg) {
  ssc_.wait(ussm_);                                             // unlock ussm_ and suspend
  ussm_.mutex()->unlock();
  return run_(std::forward<decltype(arg)>(arg)...);
 };

 size_t vs = await_seat();                                      // next vacant seat;
 ULOCK(mtx())
 availale_seats_();                                             // debug output
 DBG(2) DOUT() << "a new thread is taking up a seat[" << vs << "] and suspends" << std::endl;
 ussm_.lock();                                                  // thread will unlck once suspended
 vt_[vs] = std::thread(glambda, std::forward<decltype(args)>(args)...);
 vs_[vs] = Seat_taken;
 ussm_.mutex()->lock();                                         // wait for the thread to suspend
 ussm_.unlock();

 return vs;
}



void ThreadMaster::start_sync(void) {
 // this call provides "go" signal for start_sync
 DBG(2) DOUT() << "releasing all suspended threads" << std::endl;
 ssc_.notify_all();
}


size_t ThreadMaster::await_seat(void) {
 // returns first *available* seat, or waits until one becomes available
 size_t vs;                                                     // vacant seat;
 std::unique_lock<std::mutex> rmulck{rm_};                      // prevent threads from finishing
 while(true) {
  lock();
  vs = vacant_seat();                                           // get first vacant seat
  if(vs != SIZE_T(-1)) { unlock(); break;}                      // found a vacant seat
  DBG(2) DOUT() << "no seats available, continue waiting..." << std::endl;
  unlock();
  crm_.wait(rmulck);    // wait here for signal (via 'notify_one'), also, unlocks rm_
  // condition_variable::wait() works this way:
  // 1. unlocks the mutex (by lock rmulck here)
  // 2. suspends the thread (now via mutex way)
  // 3. upon receiving a notification from `notify_one()` (attempts to) locks the mutex again
 }

 if(vt_[vs].joinable())
  vt_[vs].join();                                               // ensure thread completed
 return vs;
}



template <class Fn, class... Args>
void ThreadMaster::run_(Fn &&fn, Args&&... args) {
 fn(std::forward<decltype(args)>(args)...);                     // run user's task

 // do clean up after thread finishes
 ULOCK(rm_)
 ULOCK(mtx())
 for(size_t seat = 0; seat < vt_.size(); ++seat)
  if(vt_[seat].get_id() == std::this_thread::get_id()) {        // find my seat
   DBG(2)
    DOUT() << "thread id " << std::this_thread::get_id()
           << " is finishing and vacating its seat[" << seat << "]" << std::endl;
   vs_[seat] = Seat_vacant;                                     // indicate vacated seat
   return crm_.notify_one();                                    // notify awaitSeat: let to continue
  }

 throw EXP(Bug_unreachable_point);
}


void ThreadMaster::availale_seats_(void) {
 // print debug outputs of available/vacant seats now (it's under mtx() already)
 size_t last_output{0}, prior_vacant{0}, pfx{0};

 DBG(2) {
  DOUT() << "seat[";
  for(size_t seat = 0; seat < seats_total(); ++seat)
   if(vs_[seat] == Seat_vacant) {
    if(pfx++ == 0) {                                            // first vacant seat
     DOUT() << seat;
     prior_vacant = last_output = seat;
     continue;
    }

    if(seat - prior_vacant == 1)
     { prior_vacant = seat; continue; }

    auto delta = prior_vacant - last_output;
    if(delta != 0) DOUT() << ((delta == 1)? ',': '-') << prior_vacant;
    DOUT() << ',' << seat;
    prior_vacant = last_output = seat;
   }

  if(prior_vacant - last_output != 0)
   DOUT() << ((prior_vacant - last_output == 1)? ',': '-') << prior_vacant;
  DOUT() << "] " << (pfx==1? "is": "are")
         << " available (out of 0.." << seats_total()-1 << ')' << std::endl;
 }
}














