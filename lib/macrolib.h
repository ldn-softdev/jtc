/*
 * Created by Dmitry Lyssenko, last modified July 3, 2015
 *
 * definitions for x-macro interface.
 *
 * SYNOPSIS:
 *
 * There are 3 macros defined here:
 * 1. COUNT_ARGS - counts arguments in a definition
 *
 *    #define COLORS Red, Amber, Blue
 *    cout << "number of colors: " << COUNT_ARGS(COLORS) << endl;
 *
 * Output:
 *    number of colors: 3
 *
 *
 *
 * 2. MACRO_TO_ARGS - apply user-provided macro to arguments
 *
 *    #define TO_COUT(X) #X << " "
 *    cout << "all defined colors: " << MACRO_TO_ARGS(TO_COUT, COLORS) << endl;
 *
 * Output:
 *    all colors: Red Amber Blue
 *
 *
 *
 * 3. XMACRO_FOR_EACH - same as 2, but applies previously defined XMACRO,
 *    thus following has the same effect:
 *
 *    #define XMACRO(X) #X << " "
 *    cout << "all colors: " << XMACRO_FOR_EACH(COLORS) << endl;
 *
 * Output:
 *    all colors: Red Amber Blue
 */






#pragma once


#define __GET_101_ARG__(_01, _02, _03, _04, _05, _06, _07, _08, _09, _10, \
                        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
                        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
                        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
                        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
                        _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
                        _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
                        _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
                        _91, _92, _93, _94, _95, _96, _97, _98, _99, _100,\
                        N, ...) N



#define __COUNT_VARARGS__(args...) __GET_101_ARG__(100, ##args,\
                        99, 98, 97, 96, 95, 94, 93, 92, 91, 90,\
                        89, 88, 87, 86, 85, 84, 83, 82, 81, 80,\
                        79, 78, 77, 76, 75, 74, 73, 72, 71, 70,\
                        69, 68, 67, 66, 65, 64, 63, 62, 61, 60,\
                        59, 58, 57, 56, 55, 54, 53, 52, 51, 50,\
                        49, 48, 47, 46, 45, 44, 43, 42, 41, 40,\
                        39, 38, 37, 36, 35, 34, 33, 32, 31, 30,\
                        29, 28, 27, 26, 25, 24, 23, 22, 21, 20,\
                        19, 18, 17, 16, 15, 14, 13, 12, 11, 10,\
                        09, 08, 07, 06, 05, 04, 03, 02, 01, 00)



#define __IF_VARARGS__(args...) __GET_101_ARG__(true, ##args,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, true,\
                        true, true, true, true, true, true, true, true, true, false)



// recursively expand selected in MACRO_TO_ARGS macro
#define _XM_00(__macro__)
#define _XM_01(__macro__, x) __macro__(x)
#define _XM_02(__macro__, x, ...) __macro__(x) _XM_01(__macro__, __VA_ARGS__)
#define _XM_03(__macro__, x, ...) __macro__(x) _XM_02(__macro__, __VA_ARGS__)
#define _XM_04(__macro__, x, ...) __macro__(x) _XM_03(__macro__, __VA_ARGS__)
#define _XM_05(__macro__, x, ...) __macro__(x) _XM_04(__macro__, __VA_ARGS__)
#define _XM_06(__macro__, x, ...) __macro__(x) _XM_05(__macro__, __VA_ARGS__)
#define _XM_07(__macro__, x, ...) __macro__(x) _XM_06(__macro__, __VA_ARGS__)
#define _XM_08(__macro__, x, ...) __macro__(x) _XM_07(__macro__, __VA_ARGS__)
#define _XM_09(__macro__, x, ...) __macro__(x) _XM_08(__macro__, __VA_ARGS__)
#define _XM_10(__macro__, x, ...) __macro__(x) _XM_09(__macro__, __VA_ARGS__)
#define _XM_11(__macro__, x, ...) __macro__(x) _XM_10(__macro__, __VA_ARGS__)
#define _XM_12(__macro__, x, ...) __macro__(x) _XM_11(__macro__, __VA_ARGS__)
#define _XM_13(__macro__, x, ...) __macro__(x) _XM_12(__macro__, __VA_ARGS__)
#define _XM_14(__macro__, x, ...) __macro__(x) _XM_13(__macro__, __VA_ARGS__)
#define _XM_15(__macro__, x, ...) __macro__(x) _XM_14(__macro__, __VA_ARGS__)
#define _XM_16(__macro__, x, ...) __macro__(x) _XM_15(__macro__, __VA_ARGS__)
#define _XM_17(__macro__, x, ...) __macro__(x) _XM_16(__macro__, __VA_ARGS__)
#define _XM_18(__macro__, x, ...) __macro__(x) _XM_17(__macro__, __VA_ARGS__)
#define _XM_19(__macro__, x, ...) __macro__(x) _XM_18(__macro__, __VA_ARGS__)
#define _XM_20(__macro__, x, ...) __macro__(x) _XM_19(__macro__, __VA_ARGS__)
#define _XM_21(__macro__, x, ...) __macro__(x) _XM_20(__macro__, __VA_ARGS__)
#define _XM_22(__macro__, x, ...) __macro__(x) _XM_21(__macro__, __VA_ARGS__)
#define _XM_23(__macro__, x, ...) __macro__(x) _XM_22(__macro__, __VA_ARGS__)
#define _XM_24(__macro__, x, ...) __macro__(x) _XM_23(__macro__, __VA_ARGS__)
#define _XM_25(__macro__, x, ...) __macro__(x) _XM_24(__macro__, __VA_ARGS__)
#define _XM_26(__macro__, x, ...) __macro__(x) _XM_25(__macro__, __VA_ARGS__)
#define _XM_27(__macro__, x, ...) __macro__(x) _XM_26(__macro__, __VA_ARGS__)
#define _XM_28(__macro__, x, ...) __macro__(x) _XM_27(__macro__, __VA_ARGS__)
#define _XM_29(__macro__, x, ...) __macro__(x) _XM_28(__macro__, __VA_ARGS__)
#define _XM_30(__macro__, x, ...) __macro__(x) _XM_29(__macro__, __VA_ARGS__)
#define _XM_31(__macro__, x, ...) __macro__(x) _XM_30(__macro__, __VA_ARGS__)
#define _XM_32(__macro__, x, ...) __macro__(x) _XM_31(__macro__, __VA_ARGS__)
#define _XM_33(__macro__, x, ...) __macro__(x) _XM_32(__macro__, __VA_ARGS__)
#define _XM_34(__macro__, x, ...) __macro__(x) _XM_33(__macro__, __VA_ARGS__)
#define _XM_35(__macro__, x, ...) __macro__(x) _XM_34(__macro__, __VA_ARGS__)
#define _XM_36(__macro__, x, ...) __macro__(x) _XM_35(__macro__, __VA_ARGS__)
#define _XM_37(__macro__, x, ...) __macro__(x) _XM_36(__macro__, __VA_ARGS__)
#define _XM_38(__macro__, x, ...) __macro__(x) _XM_37(__macro__, __VA_ARGS__)
#define _XM_39(__macro__, x, ...) __macro__(x) _XM_38(__macro__, __VA_ARGS__)
#define _XM_40(__macro__, x, ...) __macro__(x) _XM_39(__macro__, __VA_ARGS__)
#define _XM_41(__macro__, x, ...) __macro__(x) _XM_40(__macro__, __VA_ARGS__)
#define _XM_42(__macro__, x, ...) __macro__(x) _XM_41(__macro__, __VA_ARGS__)
#define _XM_43(__macro__, x, ...) __macro__(x) _XM_42(__macro__, __VA_ARGS__)
#define _XM_44(__macro__, x, ...) __macro__(x) _XM_43(__macro__, __VA_ARGS__)
#define _XM_45(__macro__, x, ...) __macro__(x) _XM_44(__macro__, __VA_ARGS__)
#define _XM_46(__macro__, x, ...) __macro__(x) _XM_45(__macro__, __VA_ARGS__)
#define _XM_47(__macro__, x, ...) __macro__(x) _XM_46(__macro__, __VA_ARGS__)
#define _XM_48(__macro__, x, ...) __macro__(x) _XM_47(__macro__, __VA_ARGS__)
#define _XM_49(__macro__, x, ...) __macro__(x) _XM_48(__macro__, __VA_ARGS__)
#define _XM_50(__macro__, x, ...) __macro__(x) _XM_49(__macro__, __VA_ARGS__)
#define _XM_51(__macro__, x, ...) __macro__(x) _XM_50(__macro__, __VA_ARGS__)
#define _XM_52(__macro__, x, ...) __macro__(x) _XM_51(__macro__, __VA_ARGS__)
#define _XM_53(__macro__, x, ...) __macro__(x) _XM_52(__macro__, __VA_ARGS__)
#define _XM_54(__macro__, x, ...) __macro__(x) _XM_53(__macro__, __VA_ARGS__)
#define _XM_55(__macro__, x, ...) __macro__(x) _XM_54(__macro__, __VA_ARGS__)
#define _XM_56(__macro__, x, ...) __macro__(x) _XM_55(__macro__, __VA_ARGS__)
#define _XM_57(__macro__, x, ...) __macro__(x) _XM_56(__macro__, __VA_ARGS__)
#define _XM_58(__macro__, x, ...) __macro__(x) _XM_57(__macro__, __VA_ARGS__)
#define _XM_59(__macro__, x, ...) __macro__(x) _XM_58(__macro__, __VA_ARGS__)
#define _XM_60(__macro__, x, ...) __macro__(x) _XM_59(__macro__, __VA_ARGS__)
#define _XM_61(__macro__, x, ...) __macro__(x) _XM_60(__macro__, __VA_ARGS__)
#define _XM_62(__macro__, x, ...) __macro__(x) _XM_61(__macro__, __VA_ARGS__)
#define _XM_63(__macro__, x, ...) __macro__(x) _XM_62(__macro__, __VA_ARGS__)
#define _XM_64(__macro__, x, ...) __macro__(x) _XM_63(__macro__, __VA_ARGS__)
#define _XM_65(__macro__, x, ...) __macro__(x) _XM_64(__macro__, __VA_ARGS__)
#define _XM_66(__macro__, x, ...) __macro__(x) _XM_65(__macro__, __VA_ARGS__)
#define _XM_67(__macro__, x, ...) __macro__(x) _XM_66(__macro__, __VA_ARGS__)
#define _XM_68(__macro__, x, ...) __macro__(x) _XM_67(__macro__, __VA_ARGS__)
#define _XM_69(__macro__, x, ...) __macro__(x) _XM_68(__macro__, __VA_ARGS__)
#define _XM_70(__macro__, x, ...) __macro__(x) _XM_69(__macro__, __VA_ARGS__)
#define _XM_71(__macro__, x, ...) __macro__(x) _XM_70(__macro__, __VA_ARGS__)
#define _XM_72(__macro__, x, ...) __macro__(x) _XM_71(__macro__, __VA_ARGS__)
#define _XM_73(__macro__, x, ...) __macro__(x) _XM_72(__macro__, __VA_ARGS__)
#define _XM_74(__macro__, x, ...) __macro__(x) _XM_73(__macro__, __VA_ARGS__)
#define _XM_75(__macro__, x, ...) __macro__(x) _XM_74(__macro__, __VA_ARGS__)
#define _XM_76(__macro__, x, ...) __macro__(x) _XM_75(__macro__, __VA_ARGS__)
#define _XM_77(__macro__, x, ...) __macro__(x) _XM_76(__macro__, __VA_ARGS__)
#define _XM_78(__macro__, x, ...) __macro__(x) _XM_77(__macro__, __VA_ARGS__)
#define _XM_79(__macro__, x, ...) __macro__(x) _XM_78(__macro__, __VA_ARGS__)
#define _XM_80(__macro__, x, ...) __macro__(x) _XM_79(__macro__, __VA_ARGS__)
#define _XM_81(__macro__, x, ...) __macro__(x) _XM_80(__macro__, __VA_ARGS__)
#define _XM_82(__macro__, x, ...) __macro__(x) _XM_81(__macro__, __VA_ARGS__)
#define _XM_83(__macro__, x, ...) __macro__(x) _XM_82(__macro__, __VA_ARGS__)
#define _XM_84(__macro__, x, ...) __macro__(x) _XM_83(__macro__, __VA_ARGS__)
#define _XM_85(__macro__, x, ...) __macro__(x) _XM_84(__macro__, __VA_ARGS__)
#define _XM_86(__macro__, x, ...) __macro__(x) _XM_85(__macro__, __VA_ARGS__)
#define _XM_87(__macro__, x, ...) __macro__(x) _XM_86(__macro__, __VA_ARGS__)
#define _XM_88(__macro__, x, ...) __macro__(x) _XM_87(__macro__, __VA_ARGS__)
#define _XM_89(__macro__, x, ...) __macro__(x) _XM_88(__macro__, __VA_ARGS__)
#define _XM_90(__macro__, x, ...) __macro__(x) _XM_89(__macro__, __VA_ARGS__)
#define _XM_91(__macro__, x, ...) __macro__(x) _XM_90(__macro__, __VA_ARGS__)
#define _XM_92(__macro__, x, ...) __macro__(x) _XM_91(__macro__, __VA_ARGS__)
#define _XM_93(__macro__, x, ...) __macro__(x) _XM_92(__macro__, __VA_ARGS__)
#define _XM_94(__macro__, x, ...) __macro__(x) _XM_93(__macro__, __VA_ARGS__)
#define _XM_95(__macro__, x, ...) __macro__(x) _XM_94(__macro__, __VA_ARGS__)
#define _XM_96(__macro__, x, ...) __macro__(x) _XM_95(__macro__, __VA_ARGS__)
#define _XM_97(__macro__, x, ...) __macro__(x) _XM_96(__macro__, __VA_ARGS__)
#define _XM_98(__macro__, x, ...) __macro__(x) _XM_97(__macro__, __VA_ARGS__)
#define _XM_99(__macro__, x, ...) __macro__(x) _XM_98(__macro__, __VA_ARGS__)



// for-each construct for variadic macros. Supports up to 99 args
#define __XMACRO_TO_ARGS__(xmacro, args...) \
 __GET_101_ARG__(100, ##args, \
                 _XM_99, _XM_98, _XM_97, _XM_96, _XM_95, _XM_94, _XM_93, _XM_92, _XM_91, _XM_90,\
                 _XM_89, _XM_88, _XM_87, _XM_86, _XM_85, _XM_84, _XM_83, _XM_82, _XM_81, _XM_80,\
                 _XM_79, _XM_78, _XM_77, _XM_76, _XM_75, _XM_74, _XM_73, _XM_72, _XM_71, _XM_70,\
                 _XM_69, _XM_68, _XM_67, _XM_66, _XM_65, _XM_64, _XM_63, _XM_62, _XM_61, _XM_60,\
                 _XM_59, _XM_58, _XM_57, _XM_56, _XM_55, _XM_54, _XM_53, _XM_52, _XM_51, _XM_50,\
                 _XM_49, _XM_48, _XM_47, _XM_46, _XM_45, _XM_44, _XM_43, _XM_42, _XM_41, _XM_40,\
                 _XM_39, _XM_38, _XM_37, _XM_36, _XM_35, _XM_34, _XM_33, _XM_32, _XM_31, _XM_30,\
                 _XM_29, _XM_28, _XM_27, _XM_26, _XM_25, _XM_24, _XM_23, _XM_22, _XM_21, _XM_20,\
                 _XM_19, _XM_18, _XM_17, _XM_16, _XM_15, _XM_14, _XM_13, _XM_12, _XM_11, _XM_10,\
                 _XM_09, _XM_08, _XM_07, _XM_06, _XM_05, _XM_04, _XM_03, _XM_02, _XM_01, _XM_00)\
                (xmacro, ##args)


// helper definition so that null-arguments can be handled as well
#define __MACRO_TO_ARGS__(macro_args...) \
    __XMACRO_TO_ARGS__(macro_args)






// user facing macro: count arguments (from 0 up to 99) - typically in #define directive
#define COUNT_ARGS(args...) __COUNT_VARARGS__(args)



// user facing macro: returns false or true if 0 or more arguments present respectively
#define IF_ARGS(args...) __IF_VARARGS__(args)



// user facing macro: will run user-defined macro for an arbitrary set of arguments (up to 99)
#define MACRO_TO_ARGS(macro, macro_args...) \
    __MACRO_TO_ARGS__(macro, ##macro_args)



// user facing macro: will run user-defined XMACRO for an arbitrary set of arguments (up to 99)
#define XMACRO_FOR_EACH(macro_args...) \
    __MACRO_TO_ARGS__(XMACRO, ##macro_args)













