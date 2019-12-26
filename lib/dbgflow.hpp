/*
 * Created by Dmitry Lyssenko
 *
 * facilitates tracking of entry-leave points in the execution flow
 */

#include "dbg.hpp"

#ifdef BG_FLOW                                                  // -DBG_FLOW to compile this debug
 #ifdef DBGBL_REF                                               // reference to a DEBUGGABLE class
  __Dbg_flow__ dbg_flow(DBGBL_REF.DBG(),
                        DBGBL_REF.DBG().prompt(__func__, __Dbg_flow__::ind + 1,
                                               DBGBL_REF.DBG().stamped(), true));
 #else
  __Dbg_flow__ dbg_flow(DBG(),
                        DBG().prompt(__func__, __Dbg_flow__::ind + 1, DBG().stamped(), true));
 #endif
#endif
