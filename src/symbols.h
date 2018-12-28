/*
 * 	symbols.h
 *
 *	Version: 1.0.0
 *  Created on: Dec 14, 2018
 *  Author: Julian Stiefel
 *  License: BSD 3-Clause
 *  Description: File containing the bitmaps for direction arrows.
 */

#ifndef _symbols_H_
#define _symbols_H_

#include "Arduino.h"

#define width 64
#define height 64

extern const uint8_t ic_nav_arrow_finish[] PROGMEM;
extern const uint8_t ic_nav_arrow_fork_left[] PROGMEM;
extern const uint8_t ic_nav_arrow_fork_right[] PROGMEM;
extern const uint8_t ic_nav_arrow_goto_start[] PROGMEM;
extern const uint8_t ic_nav_arrow_keep_going[] PROGMEM;
extern const uint8_t ic_nav_arrow_keep_left[] PROGMEM;
extern const uint8_t ic_nav_arrow_keep_right[] PROGMEM;
extern const uint8_t ic_nav_arrow_start[] PROGMEM;
extern const uint8_t ic_nav_arrow_turn_hard_left[] PROGMEM;
extern const uint8_t ic_nav_arrow_turn_hard_right[] PROGMEM;
extern const uint8_t ic_nav_arrow_turn_left[] PROGMEM;
extern const uint8_t ic_nav_arrow_turn_right[] PROGMEM;
extern const uint8_t ic_nav_arrow_uturn[] PROGMEM;
extern const uint8_t ic_nav_outof_route[] PROGMEM;
extern const uint8_t ic_nav_roundabout_ccw1_1[] PROGMEM;
extern const uint8_t ic_nav_roundabout_ccw1_2[] PROGMEM;
extern const uint8_t ic_nav_roundabout_ccw1_3[] PROGMEM;
extern const uint8_t ic_nav_roundabout_ccw2_2[] PROGMEM;
extern const uint8_t ic_nav_roundabout_ccw2_3[] PROGMEM;
extern const uint8_t ic_nav_roundabout_cw3_3[] PROGMEM;
extern const uint8_t ic_nav_roundabout_exit_ccw[] PROGMEM;
extern const uint8_t ic_nav_roundabout_exit_cw[] PROGMEM;
extern const uint8_t ic_nav_roundabout_fallback[] PROGMEM;
extern const uint8_t ic_nav_roundabout_ccw3_3[] PROGMEM ;
extern const uint8_t ic_nav_roundabout_cw1_1[] PROGMEM;
extern const uint8_t ic_nav_roundabout_cw1_2[] PROGMEM;
extern const uint8_t ic_nav_roundabout_cw1_3[] PROGMEM;
extern const uint8_t ic_nav_roundabout_cw2_2[] PROGMEM;
extern const uint8_t ic_nav_roundabout_cw2_3[] PROGMEM;
extern const uint8_t not_defined[] PROGMEM;
extern const uint8_t *nav_symbols[] PROGMEM;

#endif /* _symbols_H_ */
