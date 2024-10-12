
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

int main(void)
{
  int i, j;
  int n_channels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  double offered_loads[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  // Open a file to write results for plotting
  FILE *fp = fopen("simulation_results.csv", "w");
  fprintf(fp, "Channels,OfferedLoad,SimulatedBlockingProb\n");

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;

  for (i = 0; i < sizeof(n_channels) / sizeof(n_channels[0]); i++) {
    for (j = 0; j < sizeof(offered_loads) / sizeof(offered_loads[0]); j++) {
      int seed_index = 0;
      NUMBER_OF_CHANNELS = n_channels[i];
      double current_offered_load = offered_loads[j];
      Call_ARRIVALRATE = current_offered_load / MEAN_CALL_DURATION;

      double total_blocking_prob = 0;
      int num_seeds = 0;

      while ((random_seed = RANDOM_SEEDS[seed_index++]) != 0) {
        num_seeds++;
        simulation_run = simulation_run_new();
        simulation_run_set_data(simulation_run, (void *)&data);

        data.blip_counter = 0;
        data.call_arrival_count = 0;
        data.calls_processed = 0;
        data.blocked_call_count = 0;
        data.number_of_calls_processed = 0;
        data.accumulated_call_time = 0.0;
        data.random_seed = random_seed;

        data.channels = (Channel_Ptr *)xcalloc((int)NUMBER_OF_CHANNELS, sizeof(Channel_Ptr));
        for (int k = 0; k < NUMBER_OF_CHANNELS; k++) {
          *(data.channels + k) = server_new();
        }

        random_generator_initialize((unsigned)random_seed);
        schedule_call_arrival_event(simulation_run,
                                    simulation_run_get_time(simulation_run) +
                                        exponential_generator((double)1 / Call_ARRIVALRATE));

        while (data.number_of_calls_processed < RUNLENGTH) {
          simulation_run_execute_event(simulation_run);
        }

        double blocking_prob = (double)data.blocked_call_count / data.call_arrival_count;
        total_blocking_prob += blocking_prob;

        cleanup(simulation_run);
      }

      double avg_blocking_prob = total_blocking_prob / num_seeds;
      fprintf(fp, "%d,%.2f,%.6f\n", NUMBER_OF_CHANNELS, current_offered_load, avg_blocking_prob);
      printf("Completed simulation for %d channels and %.2f offered load\n", 
             NUMBER_OF_CHANNELS, current_offered_load);
    }
  }

  fclose(fp);
  printf("Simulation complete. Results written to simulation_results.csv\n");

  return 0;
}