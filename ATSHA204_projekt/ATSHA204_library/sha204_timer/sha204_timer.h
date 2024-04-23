/*
 * sha204_timer.h
 *
 * Created: 4/16/2024 16:25:45
 *  Author: Student
 */ 


#ifndef SHA204_TIMER_H_
#define SHA204_TIMER_H_

#define TIME_UTILS_US_CALIBRATION           //__asm__ volatile ("\n\tnop\n\tnop\n\tnop\n")
#define TIME_UTILS_MS_CALIBRATION        ((uint8_t) 104)

/** Decrement the inner loop of delay_10us() this many times to achieve 10 us per
 *  iteration of the outer loop.
 */
#define TIME_UTILS_LOOP_COUNT            ((uint8_t)  14)

#endif /* SHA204_TIMER_H_ */