/*
 * total_reward_objective.cpp
 *
 *  Created on: Sep 2, 2017
 *      Author: Nathaniel Rodriguez
 *
 * A Python extension module that creates a callable function from python for
 * use by optimization algorithms. It requires a parameter argument,
 * as well as a handler (python capsule) to the ALE environment, and
 * a handler to an agent.
 *
 */

#include <Python.h>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <ale_interface.hpp>
#include "arrayobject.h"
#include "total_cost_objective.h"
#include "../controllers/controller.h"
#include "../agents/player_agent.h"
#include "../common/utilities.h"

static PyObject *TotalCostObjective(PyObject *self, PyObject *args,
                                      PyObject *kwargs) {
  char *keyword_list[] = {"parameters", "ale", "agent", NULL};

  PyArrayObject* py_parameter_array;
  PyObject* ale_capsule;
  PyObject* agent_capsule;

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!OO", keyword_list,
      &PyArray_Type, &py_parameter_array, &ale_capsule, &agent_capsule)){
    return NULL;
  }

  ALEInterface* ale = (ALEInterface*)PyCapsule_GetPointer(ale_capsule,
      "ale_generator.ale");
  alectrnn::PlayerAgent* player_agent =
      (alectrnn::PlayerAgent*)PyCapsule_GetPointer(agent_capsule,
          "agent_generator.agent");
  double* cparameter_array(alectrnn::PyArrayToCArray(py_parameter_array));
  double total_cost(alectrnn::CalculateTotalCost(ale, cparameter_array,
      player_agent));

  return Py_BuildValue("d", total_cost);
}

static PyMethodDef TotalCostMethods[] = {
  { "TotalCostObjective", (PyCFunction) TotalCostObjective,
      METH_VARARGS | METH_KEYWORDS,
      "Objective function that sums game reward"},
  { NULL, NULL, 0, NULL}
};

static struct PyModuleDef TotalCostModule = {
  PyModuleDef_HEAD_INIT,
  "total_cost_objective",
  "Objective function that sums game reward",
  -1,
  TotalCostMethods
};

PyMODINIT_FUNC PyInit_total_cost_objective(void) {
  return PyModule_Create(&TotalCostModule);
}

namespace alectrnn {

double CalculateTotalCost(ALEInterface *ale, double* parameters,
    PlayerAgent* agent) {

  agent->Configure(parameters);
  Controller* game_controller = new Controller(ale, agent);
  game_controller->Run();
  double total_cost(-(double)game_controller->getCumulativeScore());

  delete game_controller;
  return total_cost;
}

}