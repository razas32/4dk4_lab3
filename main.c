
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
double MEAN_PATIENCE_TIME;

int main(void)
{
  int i, j;
  int n_operators[] = {5, 10};
  double mean_patience_times[] = {5, 10};  // w values in minutes
  double offered_loads[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  FILE *fp = fopen("waiting_probability_results.csv", "w");
  fprintf(fp, "OfferedLoad,Operators,MeanPatienceTime,WaitingProbability\n");

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;

  for (i = 0; i < sizeof(n_operators) / sizeof(n_operators[0]); i++) {
    for (j = 0; j < sizeof(mean_patience_times) / sizeof(mean_patience_times[0]); j++) {
      for (int k = 0; k < sizeof(offered_loads) / sizeof(offered_loads[0]); k++) {
        NUMBER_OF_CHANNELS = n_operators[i];
        MEAN_PATIENCE_TIME = mean_patience_times[j] * 60; // Convert to seconds
        double current_offered_load = offered_loads[k];
        Call_ARRIVALRATE = current_offered_load / MEAN_CALL_DURATION;

        simulation_run = simulation_run_new();
        simulation_run_set_data(simulation_run, (void *)&data);

        data = (Simulation_Run_Data) {0};
        data.call_queue = fifoqueue_new();

        data.channels = (Channel_Ptr *)xcalloc((int)NUMBER_OF_CHANNELS, sizeof(Channel_Ptr));
        for (int m = 0; m < NUMBER_OF_CHANNELS; m++) {
          *(data.channels + m) = server_new();
        }

        random_seed = RANDOM_SEEDS[0];  // Use the first seed
        random_generator_initialize((unsigned)random_seed);

        schedule_call_arrival_event(simulation_run,
                                    simulation_run_get_time(simulation_run) +
                                        exponential_generator((double)1 / Call_ARRIVALRATE));

while (data.number_of_calls_processed < RUNLENGTH) {
  simulation_run_execute_event(simulation_run);
}

double waiting_probability = (double)data.calls_that_waited / data.call_arrival_count;

printf("Debug: calls_that_waited=%ld, call_arrival_count=%ld, waiting_probability=%.6f\n",
       data.calls_that_waited, data.call_arrival_count, waiting_probability);

fprintf(fp, "%.2f,%d,%.0f,%.6f\n", current_offered_load, NUMBER_OF_CHANNELS, MEAN_PATIENCE_TIME/60, waiting_probability);


        fprintf(fp, "%.2f,%d,%.0f,%.6f\n", current_offered_load, NUMBER_OF_CHANNELS, MEAN_PATIENCE_TIME/60, waiting_probability);
        printf("Completed simulation for %d operators, %.0f minutes mean patience time, and %.2f offered load\n", 
               NUMBER_OF_CHANNELS, MEAN_PATIENCE_TIME/60, current_offered_load);

        cleanup(simulation_run);
      }
    }
  }

  fclose(fp);
  printf("Simulation complete. Results written to waiting_probability_results.csv\n");

  return 0;
}