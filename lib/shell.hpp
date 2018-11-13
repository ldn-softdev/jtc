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
 *  cout << s.stdout() << endl;
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "extensions.hpp"
#include "dbg.hpp"






class Shell {
 public:
    #define THROWREASON \
                could_not_popen_file_handle, \
                end_of_trhow
    ENUMSTR(ThrowReason, THROWREASON)

                        Shell(size_t s = 1024) { buf_.resize(s); }

    Shell &             system(const std::string & cmd, const std::string & mode = "r");
    const std::string & stdout(void) const { return out_; }
    int                 rc(void) const { return rc_; }
    size_t              buff_size(void) const { return buf_.size(); }
    Shell &             buff_size(size_t s) { buf_.resize(s); return *this; }

    DEBUGGABLE()

 protected:
    std::string         out_;                                   // resulting output string
    int                 rc_{-1};

 private:
    std::vector<char>   buf_;

    EXCEPTIONS(ThrowReason)                                     // see "enums.hpp"
};

STRINGIFY(Shell::ThrowReason, THROWREASON)
#undef THROWREASON



Shell & Shell::system(const std::string &cmd, const std::string &mode) {
 FILE *fh = popen(cmd.c_str(), mode.c_str());
 if(fh == nullptr)
  throw EXP(could_not_popen_file_handle);
 out_.clear();
 
 DBG(0) DOUT() << "executing cmd '" << cmd << "'" << std::endl;

 while(fgets(buf_.data(), buf_.size(), fh) != nullptr)
  out_ += buf_.data();
 rc_ = pclose(fh);

 DBG(0) DOUT() << "return " << rc_ << ": '" << out_ << "'" << std::endl;

 return *this;
}















