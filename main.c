
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "call_arrival.h"
#include "main.h"
#include "erlang.h"

int NUMBER_OF_CHANNELS;
double Call_ARRIVALRATE;

/*******************************************************************************/

#define NUM_CHANNELS 20
#define TARGET_BLOCKING_PROB 0.01
#define TOLERANCE 0.001
#define MAX_LOAD 100

int main(void)
{
  int i;
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  FILE *fp = fopen("max_load_results.csv", "w");
  fprintf(fp, "Channels,MaxOfferedLoad\n");

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};

  for (i = 1; i <= NUM_CHANNELS; i++) {
    NUMBER_OF_CHANNELS = i;
    double min_load = 0, max_load = MAX_LOAD, current_load;

    while (max_load - min_load > TOLERANCE) {
      current_load = (min_load + max_load) / 2;
      Call_ARRIVALRATE = current_load / MEAN_CALL_DURATION;

      double total_blocking_prob = 0;
      int num_seeds = 0;

      while (RANDOM_SEEDS[num_seeds] != 0) {
        simulation_run = simulation_run_new();
        simulation_run_set_data(simulation_run, (void *)&data);

        data = (Simulation_Run_Data) {0};
        data.random_seed = RANDOM_SEEDS[num_seeds];

        data.channels = (Channel_Ptr *)xcalloc(NUMBER_OF_CHANNELS, sizeof(Channel_Ptr));
        for (int k = 0; k < NUMBER_OF_CHANNELS; k++) {
          *(data.channels + k) = server_new();
        }

        random_generator_initialize(data.random_seed);
        schedule_call_arrival_event(simulation_run,
                                    simulation_run_get_time(simulation_run) +
                                        exponential_generator((double)1 / Call_ARRIVALRATE));

        while (data.number_of_calls_processed < RUNLENGTH) {
          simulation_run_execute_event(simulation_run);
        }

        double blocking_prob = (double)data.blocked_call_count / data.call_arrival_count;
        total_blocking_prob += blocking_prob;

        cleanup(simulation_run);
        num_seeds++;
      }

      double avg_blocking_prob = total_blocking_prob / num_seeds;

      if (avg_blocking_prob > TARGET_BLOCKING_PROB) {
        max_load = current_load;
      } else {
        min_load = current_load;
      }
    }

    fprintf(fp, "%d,%.2f\n", NUMBER_OF_CHANNELS, current_load);
    printf("Completed calculation for %d channels. Max load: %.2f\n", NUMBER_OF_CHANNELS, current_load);
  }

  fclose(fp);
  printf("Calculations complete. Results written to max_load_results.csv\n");

  return 0;
}