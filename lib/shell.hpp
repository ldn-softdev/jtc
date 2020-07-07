/*
 * Created by Dmitry Lyssenko
 *
 * a trivial micro class executing shell command and returning entire stdout
 *
 * - wrapper for popen() system call
 *
 * SYNOPSIS:
 *  Shell s;
 *
 *  s.system("ls -la 2>&1");
 *  cout << s.out() << endl;
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "dbg.hpp"
#include "extensions.hpp"





class Shell {
 public:
    #define THROWREASON \
                could_not_popen_file_handle, \
                end_of_trhow
    ENUMSTR(ThrowReason, THROWREASON)

                        Shell(size_t s = 1024) { buf_.resize(s); }

    Shell &             system(const std::string & cmd);
    const std::string & out(void) const { return out_; }
    int                 rc(void) const { return rc_; }
    size_t              buff_size(void) const { return buf_.size(); }
    Shell &             buff_size(size_t s) { buf_.resize(s); return *this; }

    DEBUGGABLE()
    EXCEPTIONS(ThrowReason)                                     // see "extensions.hpp"

 protected:
    std::string         out_;                                   // resulting output string
    int                 rc_{-1};

 private:
    std::vector<char>   buf_;
};

STRINGIFY(Shell::ThrowReason, THROWREASON)
#undef THROWREASON



Shell & Shell::system(const std::string &cmd) {
 // execute cmd: run popen and return exit code via rc_
 FILE *fh;
 const char * fn = __func__;

 auto open_pipe = [&] {
       fh = popen(cmd.c_str(), "r");
       if(fh == nullptr)
        throw EXP(could_not_popen_file_handle);
       return true;
      };
 auto close_pipe = [&](bool) {
       rc_ = pclose(fh);
       if(DBG()(0, fn)) {
        ULOCK(DBG().mutex())
        DOUT() << "return " << rc_ << ": '" << out_ << "'" << std::endl;
       }
      };
 GUARD(open_pipe, close_pipe)

 DBG(0) DOUT() << "executing cmd '" << cmd << "'" << std::endl;
 out_.clear();
 while(fgets(buf_.data(), buf_.size() - 1, fh) != nullptr)
  out_ += buf_.data();

 return *this;
}












