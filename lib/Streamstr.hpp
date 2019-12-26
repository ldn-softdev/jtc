/*
 * Created by Dmitry Lyssenko
 *
 */
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include "extensions.hpp"
#include "dbg.hpp"


#define HB_SIZE 1024
#define RB_SIZE 1024



class Streamstr {
 // operates in 2 modes: buffer mode, stream mode.
 // in buffer mode it reads entire file/cin into the buffer and let iterating until the end of it
 // in a stream mode reads cin only byte by byte until cin is closed
 // by default class is initialized in a buffer mode, stream mode requires explicit initialization

    // trivial helper class to facilitate a circular fixed-length buffer
    class Circular {
     public:
                            Circular(void) = default;
                            Circular(size_t r) { hb_.reserve(r); }

        void                push_back(char c) {
                             if(hb_.size() < hb_.capacity()) return hb_.push_back(c);
                             if(cur_ >= hb_.size()) cur_ = 0;
                             hb_[cur_++] = c;
                            }
        std::string         str(size_t len = -1) const {
                             if(len > hb_.size()) len = hb_.size();

                             if(hb_.size() < hb_.capacity()) {
                              size_t offset = len >= hb_.size()? 0: hb_.size() - len;
                              return {&hb_[offset], len};
                             }
                             size_t offset = len <= cur_? hb_.size(): cur_ + hb_.size() - len;
                             std::string s(&hb_[offset], hb_.size() - offset);
                             offset = len <= cur_? cur_ - len: 0;
                             s += std::string{&hb_[offset], cur_ - offset};
                             return s;
                            }
        const char &        chr(size_t offset = 0) const {
                             offset %= hb_.size();
                             return cur_ > offset?
                                     hb_[cur_ - offset - 1]:
                                     hb_[hb_.size() + cur_ - offset - 1];
                            }
        size_t              size(void) const { return hb_.size(); }
        size_t              capacity(void) const { return hb_.capacity(); }
        void                reserve(size_t c) { hb_.reserve(c); }
        void                reset(void) { hb_.clear(); cur_ = 0; }

     protected:
        std::vector<char>   hb_;                                    // historical buffer
        size_t              cur_{0};                                // cursor
    };

 public:

    class const_iterator;
    friend class const_iterator;

    #define STRMOD \
                streamed_cin,       /* cin is source, read byte by byte */ \
                buffered_src,       /* buffer set programmatically (via source_buffer )*/ \
                buffered_cin,       /* facilitates buffered cin */ \
                buffered_file       /* facilitate multiple files, read file by file */
    ENUMSTR(Strmod, STRMOD)

                        Streamstr(Strmod m = buffered_cin): mod_{m} {}
                        Streamstr(size_t hbs):                  // stream mode, w. hb allocation
                         mod_{streamed_cin}, hb_(hbs) {}

    const std::string & buffer(void) const { return buf_; };
    bool                is_streamed(void) const { return mod_ == streamed_cin; }
    bool                is_buffered(void) const { return mod_ != streamed_cin; }
    bool                is_buffered_src(void) const { return mod_ == buffered_src; }
    bool                is_buffered_cin(void) const { return mod_ == buffered_cin; }
    bool                is_buffered_file(void) const { return mod_ == buffered_file; }
    const std::string & filename(void) const { return cf_; }

    void                source_file(const std::string &fn) {
                         fn_.push_back(fn);
                         if(not is_buffered_file())
                          { mod_ = buffered_file; buf_.clear(); }
                         if(cf_.empty()) cf_ = fn_.front();
                        }
    template<typename... Args>
    void                source_file(const std::string &first, Args... rest)
                         { source_file(first); source_file(rest...); }

    void                source_buffer(std::string buf) {
                         if(is_buffered()) {
                          mod_ = buffered_src;
                          buf_ = std::move(buf);
                         }
                        }

    const_iterator      begin(void);
    const_iterator      end(void);

    size_t              stream_size(void) const { return cnt_; }
    std::string         str(void) const { return hb_.str(); }
    void                reset(Strmod m, size_t cbs = HB_SIZE) {
                         mod_ = m;
                         buf_.clear();
                         cnt_ = 0;
                         hb_.reset();
                         hb_.reserve(cbs);
                        }
    size_t              hb_size(void) const { return hb_.size(); }
    Circular &          hb(void) { return hb_; }
    DEBUGGABLE()

 protected:

    void                ss_init_(const_iterator &);

    Strmod              mod_;
    std::string         buf_;
std::deque<std::string> fn_;                                    // source file names container                        ;
    std::string         cf_;                                    // file being read;
    size_t              cnt_{0};                                // offset from beginning of stream

 private:

    Circular            hb_{HB_SIZE};
};

STRINGIFY(Streamstr::Strmod, STRMOD)
#undef STRMOD



class Streamstr::const_iterator: public std::iterator<std::bidirectional_iterator_tag, Streamstr> {
    friend class Streamstr;
    friend long         distance(const const_iterator & l, const const_iterator & r) {
                         using std::distance;
                         return l.is_streamed()?
                                 (r.cnt_ - r.rwd_) - (l.cnt_ - l.rwd_):     // stream
                                 (r.pos_ - r.rwd_) - (l.pos_ - l.rwd_);     // buffer
                        }
 public:
                        const_iterator(void) = default;

    Streamstr &         streamstr(void) { return *ssp_; }
    const Streamstr &   streamstr(void) const { return *ssp_; }

    bool                is_streamed(void) const { return mod_ == Streamstr::streamed_cin; }
    bool                is_buffered(void) const { return mod_ != Streamstr::streamed_cin; }
    bool                is_buffered_src(void) const { return mod_ == Streamstr::buffered_src; }
    bool                is_buffered_cin(void) const { return mod_ == Streamstr::buffered_cin; }
    bool                is_buffered_file(void) const { return mod_ == Streamstr::buffered_file; }
    bool                is_last_in_file(void) const             // facing current file's last char?
                         { return is_buffered_file() and pos_ - rwd_ == ssp_->buf_.size() - 1; }
    std::string         str(size_t len = -1) const;
    size_t              char_read(void) const { return cnt_; }
    size_t              offset(void) const { return char_read() - rwd_; }

    const char &        operator*(void);
    const char *        operator->(void)
                         { return &operator*(); }
    const_iterator &    operator++(void)
                         { return read_next_(); }
    const_iterator &    operator--(void)
                         { ++rwd_; return *this; }
    bool                operator==(const const_iterator & rhs) const {
                         if(ssp_ != rhs.ssp_) return false;     // must be of the same Streamstr
                         if(is_streamed()) {                    // stream iterator
                          if(pos_ != rhs.pos_) return false;    // i.e. one is end() other is not
                          if(pos_ == static_cast<size_t>(-2)) return true;  // both are end()
                          return cnt_ - rwd_ == rhs.cnt_ - rhs.rwd_;
                         }
                         return pos_ - rwd_ == rhs.pos_ - rhs.rwd_; }
    bool                operator!=(const const_iterator & rhs) const
                         { return not operator==(rhs); }

    COUTABLE(Streamstr::const_iterator, mod_, ssp_, pos_, cnt_, rwd_)

 protected:
                        const_iterator(Strmod m, Streamstr * ssp, size_t offset = 0):
                         mod_(m), ssp_{ssp}, pos_{offset} {}

    Strmod              mod_;
    Streamstr *         ssp_{nullptr};                          // pointer to host class
    size_t              pos_{0};                                // curr. read pos. of ssp_->buf_
    size_t              cnt_{0};                                // offset from beginning of itr.
    size_t              rwd_{0};
 private:

    const_iterator &    read_next_(void);
};



//
//  Streamstr definitions
//
Streamstr::const_iterator Streamstr::begin(void) {
 const_iterator it{mod_, this};
 ss_init_(it);
 return it;
}



Streamstr::const_iterator Streamstr::end(void) {
 if(is_streamed()) {
  const_iterator it{mod_, this, static_cast<size_t>(-2)};
  it.cnt_ = cnt_;
  return it;
 }
 return {mod_, this, buf_.size()};
}



void Streamstr::ss_init_(const_iterator &it) {
 DBG(0) DOUT() << "initializing: " << ENUMS(Streamstr::Strmod, mod_) << std::endl;
 if(is_buffered_src()) return;                                  // buffered source
                                                                // mod == stream
 if(is_streamed()) {
  std::cin.tie(nullptr);                                        // speedup cin
  std::cin >> std::noskipws;
  buf_.resize(1);
  ++it;
  return;
 }
 // this is C++ idiomatic way to read cin/file, unfortunately it's 2-3 times slower
 //buf_ = std::string{                                            // read file/stdin into buffer
 //        std::istream_iterator<char>(is_buffered_cin()?
 //         std::cin >> std::noskipws:
 //         std::ifstream{fn_.front().c_str(), std::ifstream::in} >> std::noskipws),
 //        std::istream_iterator<char>{}};

 if(is_buffered_cin()) {                                        // buffered_cin
  std::vector<std::vector<char>> aob;                           // array of buffers
  std::ifstream fin("/dev/stdin", std::ios::in);
  do {
   aob.emplace_back(std::vector<char>(RB_SIZE));
   fin.read(aob.back().data(), RB_SIZE - 1);
  } while(not fin.eof());
  buf_.reserve(aob.size() * (RB_SIZE - 1));
  for(auto &b: aob) buf_ += b.data();
 }
 else                                                           // buffered_file
  do {
   std::ifstream fin(fn_.front().c_str(), std::ios::in);        // read next file in fn_
   if(fin) {
    fin.seekg(0, std::ios::end);
    buf_.resize(fin.tellg());
    fin.seekg(0, std::ios::beg);
    fin.read(&buf_[0], buf_.size());
    cf_ = fn_.front();
   }
   else
    std::cerr << "error: could not open file '" <<  fn_.front() << "'" << std::endl;
   fn_.pop_front();
  } while(buf_.empty() and not fn_.empty());

 DBG(0) DOUT() << "read file: " << (is_buffered_cin()? "<stdin>":filename())
               << " (" << buf_.size() << " bytes)" << std::endl;
}



//
//  const_iterator definitions
//
const char & Streamstr::const_iterator::operator*(void) {
 // demultiplex streamed and buffered dereferencing
 if(is_streamed())
  if(cnt_ < ssp_->cnt_)                                         // stream ran away from iterator
   { rwd_ += ssp_->cnt_ - cnt_; cnt_ = ssp_->cnt_; }

 if(rwd_ == 0)                                                  // there was no back out
  return ssp_->buffer()[pos_];
                                                                // back-outing occurred
 if(is_streamed())
  return ssp_->hb_.chr(rwd_ - pos_);                            // get data from historical buffer
                                                                // mod_ = buffer
 return ssp_->buf_[pos_ - rwd_];
}



Streamstr::const_iterator & Streamstr::const_iterator::read_next_(void) {
 // facilitate ++operation for streamed/buffered operations
 if(is_streamed())
  if(cnt_ < ssp_->cnt_)                                         // stream advanced ahead of iter.
   { rwd_ += ssp_->cnt_ - cnt_; cnt_ = ssp_->cnt_; }            // adjust so it looks like back-out

 if(rwd_ > 0)                                                   // there was a back out
  { --rwd_; return *this; }

 if(++pos_ < ssp_->buffer().size())
  { ++ssp_->cnt_; ++cnt_; return *this; }
                                                                // out of buffer
 if(is_streamed()) {                                            // stream mode
  if(not std::cin.good()) { pos_ = static_cast<size_t>(-2); return *this; }
  char c;
  std::cin.read(&c, 1);
  ssp_->hb_.push_back(c);
  ssp_->buf_.front() = c;
  pos_ = 0;
  ++ssp_->cnt_; ++cnt_;
  return *this;
 }
                                                                // buffer mode
 if(is_buffered_src()) return *this;                            // buffer is up (cin/buffer src)
 if(is_buffered_cin()) return *this;                            // buffer is up (cin/buffer src)
 if(ssp_->fn_.empty()) return *this;                            // no more files to read from
                                                                // read in next file then
 pos_ = 0;
 ssp_->buf_.clear();
 ssp_->ss_init_(*this);
 return *this;
}



std::string Streamstr::const_iterator::str(size_t len) const {
 // return string: for streaming mode - return historical string (up till current pointer),
 // for buffer mode - from current pointer onwards
 if(is_streamed()) return ssp_->hb_.str(len);
 return {ssp_->buf_, pos_ - rwd_, len};
}





#undef HB_SIZE
#undef RB_SIZE


















