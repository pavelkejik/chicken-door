/***********************************************************************
 * Filename: door_ctrl_table.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Defines the finite state machine for door control, using a macro 
 *     that associates each state with the appropriate actions for various 
 *     events such as stopping, opening, closing, reaching target positions, 
 *     detecting overloads, and handling errors.
 *
 ***********************************************************************/

//                            Stop            Otevrit         Zavrit         Cilova poloha    dolni doraz      horni doraz        pretizeni N         pretizeni D        bez pohybu     time expired
State(NeznaznamaPoloha,      noaction,       init_open,       init_close,       noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       noaction)
State(Otevirani,             stop,           noaction,        break_open,       opened,          noaction,         unable_open,      unable_open,        noaction,        stop,           noaction)
State(OteviraniMax,          stop,           noaction,        break_open,       stop,            noaction,         detect_open,      detect_open,        noaction,        stop,           noaction)
State(Otevreno,              noaction,       noaction,        to_close,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       noaction)
State(PreruseniOtevirani,    stop,           noaction,        break_open,       noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       to_open)
State(UvolneniOtevirani,     stop,           noaction,        break_open,       repeat_open,     repeat_open,      noaction,         noaction,           repeat_open,     stop,           noaction)

State(Zavirani,              stop,           break_close,     noaction,         close_to_min,    repeat_to_min,    noaction,         noaction,           unable_close,    stop,           noaction)
State(ZaviraniMin,           stop,           break_close,     noaction,         stop,            detect_close,     noaction,         noaction,           detect_close,    stop,           noaction)
State(Zavreno,               noaction,       to_open,         noaction,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       noaction)
State(PreruseniZavirani,     stop,           break_close,     noaction,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       to_close)
State(UvolneniZavirani,      stop,           break_close,     noaction,         repeat_close,    noaction,         repeat_close,     repeat_close,       noaction,        stop,           noaction)
State(MeziPoloha,            noaction,       to_open,         to_close,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       noaction)

State(StopZavirani,          noaction,       noaction,        to_close,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       to_open)
State(StopOtevirani,         noaction,       to_open,         noaction,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       to_close)

State(ChybaDveri,            relay_on,       relay_on,        relay_on,         noaction,        noaction,         noaction,         noaction,           noaction,        noaction,       relay_off)
