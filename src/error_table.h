/***********************************************************************
 * Filename: error_table.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Defines a set of errors using the ERROR macro, which includes 
 *     information about debounce timing, blocking functions, clearing 
 *     conditions, descriptions, error codes, verbosity levels, and 
 *     corresponding events.
 *
 ***********************************************************************/


// ERROR(_name,_debup,_debdown,_blocked_fun, _clearing_by, _txt, _error_code, _verbose, _event)
ERROR(encoder_lim_max,100,100,MOVE_UP, MOVE_DOWN | SELF_CLEAR, "Koncova poloha otevreni", DorazH, v_info, ev_high_limit)
ERROR(encoder_lim_min,100,100,MOVE_DOWN, MOVE_UP | SELF_CLEAR, "Koncova poloha zavreni", DorazD, v_info, ev_low_limit)
ERROR(ovrl_up,2,10,MOVE_UP | MOVE_CTRL_BLOCK, MOVE_UP | MOVE_DOWN, "Nadproud otevirani",NadProud1N, v_warning, ev_overload_up)
ERROR(ovrl_down,2,10,MOVE_DOWN | MOVE_CTRL_BLOCK,  MOVE_UP | MOVE_DOWN, "Nadproud zavirani",NadProud1D, v_warning, ev_overload_down)
ERROR(fast_ovrl_up,34,30,MOVE_UP | MOVE_CTRL_BLOCK,  MOVE_UP | MOVE_DOWN, "Velky proud otevirani",NadProud2N, v_error, ev_overload_up)
ERROR(fast_ovrl_down,34,30,MOVE_DOWN | MOVE_CTRL_BLOCK,  MOVE_UP | MOVE_DOWN, "Velky proud zavirani",NadProud2D, v_error, ev_overload_down)
ERROR(no_move,1,100,MOVE_DOWN | MOVE_UP, MOVE_UP | MOVE_DOWN, "Neni snimana zmena polohy", NeniPohyb, v_error, ev_no_move)
ERROR(target_pos,10,20,MOVE_DOWN | MOVE_UP | MOVE_CTRL_BLOCK, MOVE_UP | MOVE_DOWN, "", NeniChyba, v_empty, ev_target)
ERROR(error_close,25,100, MOVE_CTRL_BLOCK, MOVE_UP | MOVE_DOWN, "Nelze zavrit", ChybaZavirani, v_error, no_event)
ERROR(error_open,25,100, MOVE_CTRL_BLOCK, MOVE_UP | MOVE_DOWN, "Nelze otevrit", ChybaOtevirani, v_error, no_event)