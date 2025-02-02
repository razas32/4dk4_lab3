
/*
 * 
 * Call Blocking in Circuit Switched Networks
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/*******************************************************************************/

//#define Call_ARRIVALRATE 3   /* calls/minute */
#define MEAN_CALL_DURATION 3 /* minutes */
#define RUNLENGTH 1e4 /* number of successful calls */
#define BLIPRATE 1e3
//#define NUMBER_OF_CHANNELS 11

int NUMBER_OF_CHANNELS;
double Call_ARRIVALRATE;

extern double MEAN_PATIENCE_TIME;

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 333333, 444444, 400394381, 400338538, 881697, 3816127, 873834, 7810562, 1402813, 2033860

/*******************************************************************************/

#endif /* simparameters.h */




