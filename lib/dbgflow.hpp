/*
 * Created by Dmitry Lyssenko
 *
 * facilitates tracking of entry-leave points in the execution flow
 */

#include "dbg.hpp"

#ifdef BG_FLOW                                                  // -DBG_FLOW to compile this debug
 #ifdef DBGBL_REF                                               // reference to a DEBUGGABLE class
  __Dbg_flow__ __dbg_flow__(DBGBL_REF.DBG(), __func__);
 #else
  __Dbg_flow__ __dbg_flow__(DBG(), __func__);
 #endif
#endif
