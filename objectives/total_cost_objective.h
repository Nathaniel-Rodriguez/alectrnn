/*
 * total_reward_objective.h
 *
 *  Created on: Sep 2, 2017
 *      Author: Nathaniel Rodriguez
 *
 * A Python extension module that creates a callable function from python for
 * use by optimization algorithms. It requires a parameter argument,
 * as well as a handler (python capsule) to the ALE environment, and
 * a handler to an agent.

 */

#ifndef ALECTRNN_OBJECTIVES_TOTAL_COST_OBJECTIVE_H_
#define ALECTRNN_OBJECTIVES_TOTAL_COST_OBJECTIVE_H_

#include <Python.h>
#include <ale_interface.hpp>
#include "../agents/player_agent.h"

PyMODINIT_FUNC PyInit_total_cost_objective(void);

namespace alectrnn {

double CalculateTotalCost(ALEInterface *ale, double* parameters,
    PlayerAgent* agent);

}

#endif /* OBJECTIVES_TOTAL_COST_OBJECTIVE_H_ */
