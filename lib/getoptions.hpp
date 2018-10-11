/*
 * Created by Dmitry Lyssenko
 *
 * this class is a simple wrapper of unix getopt with easy interface.
 *
 * class Getopt follows POSIX conventions for command line arguments described
 * at http://www.gnu.org/s/hello/manual/libc/Argument-Syntax.html
 * also see 'man 3 getopt'.
 *
 * Class Getopt is based on system's getopt (supports only short options)
 * and has following synopsis:
 *
 *    Getopt opt;
 *    try
 *     { opt.parse(argc, argv, format); }
 *    catch(stdException & e)
 *     { std::cout << "exception: " << e.what() << std::endl; exit(e.code()); }
 *
 * Method parse() takes 3 arguments: main's standard (argc, argv) and optional format
 * "format" is the optional c-like std::string that follows the same syntax as in 
 * system getopt()
 *
 * Getopt object will throw if getopt ever returns ':' - it means that format has 
 * a syntax error
 *
 * By default, Getopt also will throw an exception if getopt returns '?'. If 'opterr'
 * is set to null, 'invalid_option' is thrown, otherwise 'argumentMissing' is thrown
 * User always can retrieve an option which has caused the exception with exception()
 * method
 *
 * Upon exception, getopt will print into stderr corresponding message, if user
 * wants to suppress printing, then suppress_opterr() should be called prior to
 * parse().
 *
 * User also can suppress throwing exception (for invalid_option/opt_argument_missing
 * reasons only) by calling suppress_exceptions(), then burden on processing returned
 * option '?' is on the user.
 *
 * A bare qualifier '-' in argument would cause setting option '-', so that later
 * on user could test (e.g. with if(p['-'].hits()>0) ... ) whether bare qualifier
 * was used.
 *
 * It's also possible to check option/arguments via direct addressing, like in
 * the example:
 *
 *    Getopt opt;
 *
 *    try{
 *     opt(argc, argv, "ab:");
 *    }
 *    catch(...)
 *     { opt.usage(); return 0; }
 *
 *    if(opt['a'].hits() > 0)
 *     std::cout << "option a is set." << std::endl;
 *    if(opt['b'].hits() > 0)
 *     std::cout << "option b is set to the value: " << opt['b'] << std::endl;
 *
 *    for(int idx = 0; idx < opt.arguments(); idx++)
 *     std::cout << "Arg " << idx+1 << ": " << opt[idx] << std::endl;
 *
 * Instead of relying on a rather short format string, options could be defined more
 * verbosely via setting options directly (before parsing, obviously) via methods:
 *  - bind(const str::string &); - sets a default value for the option
 *  - name(const char *); - sets option's parameter name (or the name of a standalone
 *    argument)
 *  - desc(const char *); - provides a verbose description of the option (help)
 *
 * Thus, a much nicer way to define formatted std::string "ab:" would be:
 *
 *    Getopt opt;
 *
 *    opt['a'].desc("option 'a' for some usage as a boolean flag");
 *    opt['b'].bind("0").name("b-opt").desc("option 'b' for some usage with a value");
 *    opt[0].bind("127.0.0.1").name("host").desc("ip address of some host");
 *
 *    try { opt(argc,argv) } catch(...) { opt.usage(); return 0; }
 *
 *    bool opt_a = opt['a'];
 *    long opt_b = opt['b'];
 *    if(opt_a)
 *     std::cout << "option a is set." << std::endl;
 *    std::cout << "option b is set to the value: " << obt_b << std::endl;
 *
 * If options are defined that way, the autohelp (which is always on, which, in turn
 * could be suppressed either via explicit call suppress_autohelp(), or implicitly
 * by defining a '-h' option) will autogenerate a nice help screen:
 *
 *    usage: <prog_name> [-a] [-h] [-b b-opt] [host]
 *
 *    optional arguments:
 *     -a             option 'a' for some usage as a boolean flag
 *     -h             help screen
 *     -b b-opt       option 'b' for some usage with a value [default: 0]
 *
 *    standalone arguments:
 *      [host]        ip address of some host [default: 127.0.0.1]
 *
 * If options are defined via getopt format string only, the implicit last argument
 * (opt[0]) will collect all parsed standalone arguments (i.e. arg[0] is defined
 * then as variadic)
 *
 * If any of the options/arguments are defined via Getopt methods, variadic behavior
 * of the last defined argument is suppressed. To re-enable it call opt.variadic(true).
 *
 * Note: opt.variadic(true) must be called only once all arguments have been defined
 *       i.e. variadic behavior will apply to the last defined standalone argument
 *
 * Once options are parsed, following methods exist to access options:
 *  - str(); - returns selected option's parsed std::string (option must be parametrical, 
 *           or arg)
 *  - c_str() - same as str, but return c-string instead
 * Note: user may specify the same option multiple times, str() and c_str() by default
 *       return last specified std::string value. User may specify index of the
 *       returned instance, e.g.: nth_b = opt['b'].str(n);
 *       opt['b'].size() will return number of recorded instances (invocations)
 *       for parametric options and arguments (always accounting default value too)
 *       for boolean options size is always 1 (accounting unused default value).
 *
 *       if default binding was provided, opt['b'].str(0) will return default std::string
 *       value: opt['b'].str(-1) will return a value of the last one (everything
 *       here applies to the variadic standalone argument too)
 *
 * Original order of options in user cli could be recovered using either via
 * Getopt\s order(idx) call, whose idx refers to the position of an option in cli
 * (zero based index), or via Option's order(count) call (count here is option's
 * count, which is starts from 1 (minding always present default value)
 *
 */

#pragma once

#include <string.h>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <math.h>       // floor()
#include <unistd.h>     // getopt
#include <type_traits>
#include "extensions.hpp"
#include "dbg.hpp"


#define OPT_ARG_OFFSET 256
#define OPT_WIDTH 80





class Getopt;
class Option {
 // option container: provides aggregated access to the options, i.e., options
 // could have been specified multiple times, this class aggregates all multiple
 // encounters of the same option into a single container.
 // interface before parsing:
 //  - define option: set name, its description (help line), default value
 // after parsing:
 //  - access to the option's value(s), including default, hit count, argument size.
 // in addition it provides reference to the order in which options were given
    typedef std::vector<std::string> vec_string;
    typedef vec_string::iterator iter_opt;
    typedef vec_string::const_iterator citer_opt;

 public:
    friend Getopt;

    // all program arguments are 2 kinds: options, e.g '-d', or
    // standalone arguments; a bare '-' argument is set as a boolean option
    // options can be either boolean (w/o parameter) or parametric
    // standalone arguments are always parametric type
    enum ArgKind { opt, arg };
    enum OptType { boolean, parametric };

                        Option(void) = delete;
                        Option(short opt, Getopt *go): id_{opt}, go_{go}
                         { name_ = toupper(opt); }

    // User interface:
    short int           id(void) const { return id_; }          // could be: 'a', 'x', 256, 257, ...
    ArgKind             kind(void) const
                         { return id_ < OPT_ARG_OFFSET? opt: arg; }
    OptType             type(void) const { return ot_; }

    // methods typically used to setup/configure options
    Option &            bind(const std::string &deflt = "");    // set a default value
    Option &            name(const std::string &n);             // parameter's/arg's name in help
    const std::string & name(void) const { return name_; }
    Option &            desc(const std::string &d) { desc_ = d; return *this; }
    const std::string & desc(void) const { return desc_; }
    Option &            description(const std::string &d) { return desc(d); }
    const std::string & description(void) const { return desc(); }

    // methods typically used after parsing
    Option &            operator=(const std::string &s);        // append a new value & record a hit
    Option &            hit(void);                              // record a hit (& update order)
                        operator double(void) const
                         { return type() == boolean? hits(): atof(c_str()); }
    size_t              size(void) const { return val_.size(); }
                        // size: the number of times the value was recorded for given parametric
                        // option/argument, size is always >= 1 (due to a reserved place for a
                        // default value)
    size_t              hits(void) const { return order_.size()-1; }
                        // hits: the number of times given option was given (boolean/parametric)
    size_t              order(long i = -1) const
                         { return order_[mod_(order_.size(), i)]; }
                        // order() provides options to back trace their original position
                        // in the user cli (i.e. recover the order of options)
                        // argument i == 0 refers to the default value and is unused
    const char *        c_str(int i = -1) const
                         { return val_[mod_(val_.size(), i)].c_str(); }
    const std::string & str(int i = -1) const
                         { return val_[mod_(val_.size(), i)]; }
                        // str() and c_str() let inspect any recorded option parameters/arguments,
                        // including a default value (if set). by default, str() and c_str()
                        // return the last set value (including default)
    Option &            reset(void)
                         { val_.resize(1); order_.resize(1); return *this; }
                        // reset to the state after setup and before parse

    // methods to iterate over parametric option values
    iter_opt            begin(void) { return ++val_.begin(); }
    citer_opt           begin(void) const { return ++val_.cbegin(); }
    iter_opt            end(void) { return val_.end(); }
    citer_opt           end(void) const { return val_.cend(); }

 protected:
    const std::string & deflt_(void) const { return val_.front(); }

    short int           id_{0};                                 // option symbol / arg's position
                        // for options, id_ is the ascii code of the options symbol,
                        // for arguments, id_ is based off 256, i.e arg[0] has id 256, etc
    OptType             ot_{boolean};                           // boolean or parametric (type)
    vec_string          val_{1};                                // values set by user
                        // place for the default value is always reserved, it might be unset
                        // - if val_.front() is empty, then the default value was never bound
    std::vector<size_t> order_{0};                              // record order of the option
    std::string         name_;                                  // name of the opt's parameter
    std::string         desc_;                                  // opt/arg description
    Getopt *            go_{nullptr};                           // pointer back to GetOpt

 private:
    long                mod_(long x, long i) const
                         { return i - x * static_cast<long>(floor(i/static_cast<double>(x))); }
                        // that mod function, provides similar to x % i operation, however
                        // for negative values -1, -2, ... returns size()-1, size()-2, etc
};



Option & Option::bind(const std::string &deflt) {
 // calling bind() sets default value for the option and makes option 'parametric'
 if(not deflt.empty())
  val_.front() = deflt;
 ot_ = parametric;
 return *this;
}


Option & Option::name(const std::string &n) {
 // setup name of the argument and make option parametric
 name_ = n;
 ot_ = parametric;
 return *this;
}


Option & Option::operator=(const std::string &v) {
 // append a new value to the option (and make it parametric)
 val_.push_back(v);
 ot_ = parametric;
 hit();
 return *this;
}


std::ostream & operator<<(std::ostream & os, const Option & opt) {
 return opt.type() == Option::boolean? os << opt.hits(): os << opt.c_str();
}





class Getopt {
 // provides interface to option container:
 //   o before parsing: allows describing options (bind default values, description, etc)
 //   o after parsing: provides access to the recorded option values
 // parses provided options (argv),
 // allows auto generating help and usage outputs
    typedef std::map<short, Option> map_opt;
    typedef map_opt::iterator iter_opt;

    class OptionOrder {
     // facilitates back-tracing every option to it's original position in user cli
     public:
                            OptionOrder(void) = delete;
                            OptionOrder(short opt, size_t cnt, Getopt *go):
                             opt_{opt}, cnt_{cnt}, go_(go)
                              {}

        size_t              count(void) const { return cnt_; }
        Option &            option(void) { return go_->om_.at(opt_); }
        const Option &      option(void) const { return go_->om_.at(opt_); }
        short               id(void) const { return opt_; }
        Option::ArgKind     kind(void) const { return go_->om_.at(opt_).kind(); }
        Option::OptType     type(void) const { return go_->om_.at(opt_).type(); }
        const std::string & name(void) const { return go_->om_.at(opt_).name(); }
        const std::string & desc(void) const { return go_->om_.at(opt_).desc(); }
        size_t              order() const { return go_->om_.at(opt_).order(); }
        size_t              hits(void) const { return go_->om_.at(opt_).hits(); }
        const char *        c_str(void) const { return option().c_str(count()); }
        const std::string & str(void) const{ return option().str(count()); }

     private:
        short               opt_;                               // option id: 'a', 'x', etc
        size_t              cnt_;                               // counter of an option (1,2,3,...)
        Getopt *            go_{nullptr};
    };

    friend OptionOrder;
    friend Option;
 public:

    #define THROWREASON \
                invalid_option, \
                opt_argument_missing, \
                std_argument_missing, \
                format_missing, \
                too_many_arguments, \
                undefined_option, \
                non_ordinal_idex, \
                inconsistent_format_string, \
                end_of_throw
    ENUMSTR(ThrowReason, THROWREASON)


    Getopt &            suppress_opterr(void) { opterr = 0; return *this; }
    Getopt &            suppress_exceptions(void) { throwException_ = false; return *this; }
    Getopt &            suppress_autohelp(void) { autohelp_ = false; return *this; }
    Getopt &            variadic(bool x = true) { variadic_ = x; return *this; }
    void                parse(int argc, char *argv[], const char *fmt = nullptr);
    int                 arguments(void) const { return arguments_; }
    char                exception(void) const { return exception_; }
    Option &            operator[](char opt);                   // option access
    Option &            operator[](int idx);                    // access to standalone argument
    Getopt &            prolog(const char *p) { prolog_ = p; return *this; }
    Getopt &            epilog(const char *e) { epilog_ = e; return *this; }
    const std::string & prog_name(void) const { return prgname_; }
    Getopt &            usage(void);                            // auto-generated usage
    Getopt &            help(void);                             // auto-generated help
    iter_opt            begin(void) { return om_.begin(); }
    iter_opt            end(void) { return om_.end(); }
    Getopt &            reset(void) { for(auto &om: om_) om.second.reset(); return *this; }
    bool                defined(char opt) const { return om_.count(opt) == 1; }
    std::vector<OptionOrder> &
                        order(void) { return ov_; }
    OptionOrder &       order(size_t idx) { return ov_[idx]; }

    EXCEPTIONS(ThrowReason)

 protected:
    char                exception_;                             // option that generated exception
    bool                throwException_{true};                  // let throw is a default behavior
    bool                autohelp_{true};
    bool                variadic_{true};
                        // last argument can be variadic (accept multiple values), however,
                        // defining any option (or standalone arg) resets variadic_ to FALSE
                        // - user then must explicitly turn on variadic behavior. Thus the default
                        // behavior is for case when arguments are defined via format string,
                        // i.e. mimics getopt() behavior, which does not limit a number of args
    int                 argc_;
    char **             argv_{nullptr};
    std::string         prolog_;
    std::string         epilog_;
    int                 arguments_;                             // number of found std. arguments
    std::string         prgname_;                               // program name, after parsing

    map_opt             om_;                                    // opt. map: { 'c' -> Option('c') }
    std::vector<OptionOrder>                                    // keeps record of options
                        ov_;                                    // in the order they come

 private:
    //void                update_order_(short option_id);
    void                parseInputArgs_(int argc, char *argv[], const std::string &fmt);
    void                processStandalone_(int argc, char *argv[]);
    void                usagePrintOptions_(std::stringstream &, int indent);
    void                usagePrintArgs_(std::stringstream &, int indent);
    std::string         recoverFormat_(const char *fmt);
    unsigned            countArgs_(void);                       // counts defined standalone args
    void                optionHelpLine_(const Option & opt);
    void                outputOpt_(std::stringstream &ss, int indent, const std::string &);
};

STRINGIFY(Getopt::ThrowReason, THROWREASON)
#undef THROWREASON



Option & Option::hit(void) {
 // record a hit (by virtue of pushing a value into order_ vector)
 // and update order of the option itself
 go_->ov_.emplace_back(id(), hits() + 1, go_);                  // update order of the option
 order_.push_back(go_->ov_.size() - 1);                         // update my order reference
 return *this;
}



// for description of variables optreset, optind, optarg and optopt refer to description
// of getopt() function. Those are external parameters which getopt sets and is based on

//void Getopt::update_order_(short option) {
// record given option into order vector - to keep track or options original order
//}


void Getopt::parse(int argc, char *argv[], const char *f) {
 // 1. recover format string from defined options (if any) - so that getopt could parse it
 // 2. generate auto-help (if -h given) and setup program name
 // 3. process input 'argv' array (via getopt call), then post process standalone arguments
 #ifndef __linux__
 optreset = 1;                                                  // this is done in order to allow
 #endif
 optind = 1;                                                    // - linux way to use optreset
 std::string fmt = recoverFormat_(f);                           // recovered format

 if(autohelp_) {
  if(fmt.find('h') != std::string::npos)
   autohelp_ = false;                                           // defined 'h' disables autohelp
  else                                                          // else, add 'h' to the format str
   fmt += 'h';
 }

 argc_ = argc;
 argv_ = argv;
 auto const name = strrchr(argv_[0], '/');
 prgname_ = name? name + 1: argv_[0];

 parseInputArgs_(argc, argv, fmt);                              // process all inputs in argc/argv
 processStandalone_(argc, argv);                                // now process all args
}


void Getopt::parseInputArgs_(int argc, char *argv[], const std::string &fmt) {
 // parse all provided arguments with getopt():
 // extract option one by one, create Option container out of extracted values

 char option;
 while((option = getopt(argc, argv, fmt.c_str())) != -1) {      // read next option character
  if(option == ':')
   throw EXP(opt_argument_missing);                             // missing argument, forced ':'

  if(option == '?')                                             // '?' means invalid option
   if(throwException_) {                                         // if throwing is allowed
    exception_ = optopt;                                        //   optopt = either unknown option,
    throw fmt.find(exception_) == std::string::npos?            //   (if not found in fmt string)
           EXP(invalid_option): EXP(opt_argument_missing);      //   or missing arg, throw anyway
   }                                                            // otherwise let user handle it

  if(optarg)                                                    // if option has argument
   om_.at(option) = optarg;                                     // set option with argument
  else {                                                        // is boolean option then
   if(option == 'h' and autohelp_)
    { usage().help(); exit(0); }                                // process auto-help
   om_.at(option).hit();                                        // increment hit for boolean
  }
 } // while(...

 om_.emplace('-', Option{'-', this});
 // boolean option '-' is always defined, however hit count let user know if it's ever hit
}


void Getopt::processStandalone_(int argc, char *argv[]) {
 // process only standalone arguments and bare '-' input redirector
 // last variadic argument may collect multiple values
 arguments_ = 0;                                                // track number of arguments
 int ha = 0;                                                    // ha: hyphen adjust

 for(int i = optind; i<argc; i++) {                             // process all standalone arguments
  short idx = OPT_ARG_OFFSET + i - optind - ha;                 // arg index in the map (adjusted)
  if(argv[i][0] == '-' and argv[i][1] == '\0') {                // if arg is a bare qualifier '-'
   om_.at('-').hit();
   ++ha;
   continue;
  }

  if(om_.count(idx) == 0) {                                      // argument was not defined/setup
   if(variadic_ == false)
    throw EXP(too_many_arguments);                              // throw if not variadic
   if(idx == OPT_ARG_OFFSET)                                    // if it's a first argument
    om_.emplace(OPT_ARG_OFFSET, Option{OPT_ARG_OFFSET, this});  // record it,
   om_.rbegin()->second = argv[i];                              // select last index (it's variadic)
  }
  else                                                          // argument was defined
   om_.at(idx) = argv[i];
  ++arguments_;
 } // for(...

 // now check if any mandatory arguments are missing
 for(auto &o: om_) {                                             // go through all the parsed option
  auto & opt = o.second;
  if(opt.kind() == Option::arg)                                 // argument defined/described
   if(opt.size() == 1 and opt.str(0).empty())                   // but its value is unset
    throw EXP(std_argument_missing);                            // standalone argument is missed
 }
}


class Option & Getopt::operator[](char opt) {
 // user access to the option. If undefined option is accessed after parsing
 // the program will throw (deemed to be a programming error)
 auto found = om_.find(opt);

 if(found != om_.end())
  return found->second;                                         // return existing option
 if(not prgname_.empty())
  throw EXP(undefined_option);                                  // accessed undefined past parsing

 variadic_ = false;                                             // user must define explicitly now
 return om_.emplace(opt, Option{opt, this}).first->second;
}


class Option & Getopt::operator[](int opt) {
 // user access to the standalone arguments

 short idx = OPT_ARG_OFFSET + opt;

 if(om_.find(idx) != om_.end())                                 // argument is found (defined)
  return om_.at(idx);

 if(idx > OPT_ARG_OFFSET and om_.find(idx-1) == om_.end())      // check arguments ordinality
  throw EXP(non_ordinal_idex);                                  // i.e. [0] and then [2]
 if(not prgname_.empty())
  throw EXP(undefined_option);                                  // accessed undefined past parsing

 variadic_ = false;                                             // user must define explicitly now
 return om_.emplace(idx, Option{idx, this}).first->second;
}


Getopt & Getopt::usage(void) {
 // print usage on the console: first go options then arguments
 std::stringstream ss;

 ss << "usage: " << prog_name();
 int indent = ss.tellp();

 if(autohelp_)
  om_.emplace('h', Option{'h', this}).first->second.desc("help screen");

 usagePrintOptions_(ss, indent);
 usagePrintArgs_(ss, indent);

 std::cout << ss.str() << std::endl;
 return *this;
}


void Getopt::usagePrintOptions_(std::stringstream &ss, int indent) {
 // print options only here
 std::stringstream os;
 os << " [-";
 for(auto &m: om_) {                                             // print boolean options first
  auto &opt = m.second;
  if(opt.type() != Option::boolean) continue;                   // process options w/a params here
  if(opt.id() == '-' ) continue;                                // skip input redirector
  os << static_cast<char>(opt.id());
 }
 os << "]";
 outputOpt_(ss, indent, os.str());
 os.str(std::string());

 for(auto &m: om_) {                                             // print options with parameters
  auto &opt = m.second;
  if(opt.kind() == Option::arg) continue;
  if(opt.type() != Option::parametric) continue;
  os << " [-" << static_cast<char>(opt.id()) << ' ' << opt.name() << ']';
  outputOpt_(ss, indent, os.str());
  os.str(std::string());
 }
}


void Getopt::usagePrintArgs_(std::stringstream &ss, int indent) {
 // print arguments taking into account possible variadic behavior of the last argument
 bool noneDefined = true;

 for(auto &m: om_) {
  std::stringstream os;
  auto & opt = m.second;
  if(opt.kind() == Option::opt) continue;

  noneDefined = false;
  os << ' ';

  if(not opt.str(0).empty())                                    // if default value is set:
   os << '[';                                                   // - setting DV makes arg optional
  os << opt.name();

  if(opt.id() == om_.rbegin()->second.id() and variadic_)       // i.e. if it's the last one
   os << "...";

  if(not opt.str(0).empty()) os << ']';
  outputOpt_(ss, indent, os.str());
 } // for(...

 if(noneDefined and variadic_) std::cout << " ...";
}


Getopt & Getopt::help(void) {
 // print help screen
 std::cout << prolog_ << std::endl;

 if(om_.size() - arguments()) {                                  // if any options deinfed
  std::cout << "optional arguments:" << std::endl;

  for(auto &m: om_) {                                            // first print boolean options
   auto & opt = m.second;
   if(opt.id() == '-') continue;
   if(opt.type() == Option::boolean)
    optionHelpLine_(opt);
  }

  for(auto &m: om_) {                                            // print parametrical options
   auto & opt = m.second;
   if(opt.kind() == Option::opt and opt.type() == Option::parametric)
    optionHelpLine_(opt);
  }
 } // if(om_.size() ...

 if(countArgs_() > 0) {                                         // print standalone args if any
  std::cout << std::endl << "standalone arguments:" << std::endl;

  for(auto &m: om_) {
   auto & opt = m.second;
   if(opt.kind() == Option::arg)
    optionHelpLine_(opt);
  }
 } // if(countArgs_ ...

 std::cout << epilog_ << std::endl;
 return *this;
}


void Getopt::optionHelpLine_(const Option & opt) {
 // printing options help line
 std::stringstream out;
 constexpr int descIndent = 16;                                 // indent of the help line

 if(opt.kind() == Option::opt) {
  out << " -" << static_cast<char>(opt.id());
  if(opt.type() == Option::parametric)
   out << ' ' << opt.name();
 }
 else                                                           // standalone arg
  out << "  " << opt.name();

 out << std::string(out.tellp() < descIndent? descIndent - out.tellp(): descIndent, ' ');
 out << opt.desc();

 if(not opt.deflt_().empty())                                   // if default value is set
   out << " [default: " << opt.deflt_() << ']';

 std::cout << out.str() << std::endl;
}


std::string Getopt::recoverFormat_(const char *f) {
// recovers getopt's format string from given binds/names/descriptions (in om_)
// if both fmt and option (om_) defined, merge those
 std::string fmt(f? f: "");

 if(om_.empty() and fmt.empty())
  throw EXP(format_missing);                                    // either must be defined

 for(auto &m: om_) {                                             // recover fmt from defined options
  auto & opt = m.second;
  if(opt.kind() == Option::arg)
   continue;                                                    // process only options for now

  if(fmt.find(opt.id()) == std::string::npos) {                  // if option is not in fmt yet - add
   fmt += opt.id();
   if(opt.type() == Option::parametric)
    fmt += ':';
   continue;
  }

  auto ops = fmt.find(opt.id());                                // option defined, check consistency
  if(not ((opt.type() == Option::boolean) xor (fmt[ops+1] == ':')))
   throw EXP(inconsistent_format_string);
 } // for(...

 auto itr = om_.begin();                                       // point to last found option
 for(auto ops: fmt)                                            // ensure all ops are booked, per fmt
  if(ops == ':')
   itr->second.bind();                                         // make parametric
  else                                                         // ops == ':', option is parametric
   if(om_.count(ops) == 0)
    itr = om_.emplace(ops, Option{ops, this}).first;
   else
    itr = om_.find(ops);

 return fmt;
}


unsigned Getopt::countArgs_(void) {
 // counts defined standalone args (i.e. before parsing)
 return
  count_if(om_.begin(), om_.end(),
           [](const auto &p) { return p.second.kind()==Option::arg; } );
}


void Getopt::outputOpt_(std::stringstream &ss, int indent, const std::string &str) {
 // in usage: output minding max width (OPT_WIDTH)
 if(static_cast<size_t>(ss.tellp()) + str.size() <= OPT_WIDTH)
  { ss << str; return; }
 std::cout << ss.str() << std::endl;
 ss.str(std::string());                                         // reset ss
 ss << std::string(indent, ' ') << str;                         // output ident only
}





#undef OPT_ARG_OFFSET
#undef OPT_WIDTH

