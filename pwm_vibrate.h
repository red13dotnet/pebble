/*
The MIT License (MIT)

Copyright (c) 2014 Jeffrey R. Blum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <pebble.h>

/*
 * the minimum length of each PWM pulse in milliseconds DO NOT CHANGE THIS
 */
#define MIN_PWM_PULSE_PERIOD 10

/*
 * increasing extends the pulse period, making it more "choppy" but allowing longer more complicated patterns
 */
#define PWM_PULSE_PERIOD_MULTIPLIER 1

/*
 * max number of vibration pulses * 2 AFTER it is broken into PWM.
 * always uses this number of bytes (inefficient...)
 * NOTE: Silent failure when truncating PWM pattern to this size
 * (e.g. if you have a long pattern, it may not play completely
 * in order to avoid going past end of this fixed-size buffer.
 * at 512*2, you get a max of about 5 seconds of total vibration.)
 */
#define MAX_PATTERN_SIZE 512*2   


/*
 * Pebble VibePatternPWM structure, essentially like a normal VibePattern
 * but the durations is not declared const, so that helper functions are simplified.
 * 
 * Instead of alternating on and off,
 * the "durations" field of the struct instead contains duration/intensity alternating pairs.
 * Intensity ranges from 0-10, with 10 full intensity, and 0 no vibration
 * For example, durations "100 5 200 7 300 0 150 2" would output:
 * 	100ms pulse at intensity 5 (half)
 * 	200ms pulse at intensity 7 (70% of full intensity)
 * 	300ms pause
 * 	150ms pulse at intensity 2 (very low)
 * 
 * NOTE: durations are floored (rounded down) to 10ms boundaries.
 */
typedef struct {
  uint32_t *durations;
  uint32_t num_segments;
} VibePatternPWM;


/*
 *  Enqueues a VibePatternPWM, breaking the durations down into 
 *  individual PWM pulses and sending them through Pebble's
 *  normal vibes_enqueue_custom_pattern function
 */
void vibes_enqueue_custom_pwm_pattern(VibePatternPWM *pwmPat);


/*
 * Helper function that takes a segments array and appends a duration/force tuple.
 */
VibePatternPWM * vibesPatternPWM_addpulse(VibePatternPWM *pat, uint32_t duration, uint32_t force);