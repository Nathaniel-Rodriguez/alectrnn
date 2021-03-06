/*
 * network_constructor.cpp
 *
 *  Created on: Sep 2, 2017
 *      Author: Nathaniel Rodriguez
 *
 * Defines a couple functions that can be used to generate all-to-all networks.
 * New functions may be added in the future to create more specific structures.
 * The InEdge struct is used as the basic block for NeuralNetworks.
 *
 */

#include <cstddef>
#include <vector>
#include "network_constructor.hpp"

namespace ctrnn {

InEdge::InEdge() {
  source = -1;
  weight = 0;
}

InEdge::InEdge(int source_node, float source_weight)
  : source(source_node), weight(source_weight) {
}

// Creates a vector for all the in coming edges in an all-to-all network
std::vector< std::vector<InEdge> > All2AllNetwork(std::size_t num_nodes,
    const float *weights) {
  std::vector< std::vector<InEdge> > node_neighbors(num_nodes);
  for (std::size_t iii = 0; iii < num_nodes; iii++) {
    std::vector<InEdge> neighbors(num_nodes);
    for (std::size_t jjj = 0; jjj < num_nodes; jjj++) {
      neighbors[jjj] = InEdge(jjj, weights[iii * num_nodes + jjj]);
    }
    node_neighbors[iii] = neighbors;
  }
  return node_neighbors;
}

std::vector< std::vector<InEdge> > All2AllNetwork(std::size_t num_nodes,
    float default_weight) {
  std::vector< std::vector<InEdge> > node_neighbors(num_nodes);
  for (std::size_t iii = 0; iii < num_nodes; iii++) {
    std::vector<InEdge> neighbors(num_nodes);
    for (std::size_t jjj = 0; jjj < num_nodes; jjj++) {
      neighbors[jjj] = InEdge(jjj, default_weight);
    }
    node_neighbors[iii] = neighbors;
  }
  return node_neighbors;
}

std::vector< std::vector<InEdge> > FullSensorNetwork(std::size_t num_sensors,
    std::size_t num_nodes, const float *weights) {
  std::vector<std::vector<InEdge> > node_sensors(num_nodes);
  for (std::size_t iii = 0; iii < num_nodes; iii++) {
    std::vector<InEdge> sensors(num_sensors);
    for (std::size_t jjj = 0; jjj < num_sensors; jjj++) {
      sensors[jjj] = InEdge(jjj, weights[iii * num_sensors + jjj]);
    }
    node_sensors[iii] = sensors;
  }
  return node_sensors;
}

std::vector< std::vector<InEdge> > FullSensorNetwork(std::size_t num_sensors,
    std::size_t num_nodes, float default_weight) {
  std::vector<std::vector<InEdge> > node_sensors(num_nodes);
  for (std::size_t iii = 0; iii < num_nodes; iii++) {
    std::vector<InEdge> sensors(num_sensors);
    for (std::size_t jjj = 0; jjj < num_sensors; jjj++) {
      sensors[jjj] = InEdge(jjj, default_weight);
    }
    node_sensors[iii] = sensors;
  }
  return node_sensors;
}

void FillFullSensorNetwork(std::vector< std::vector<InEdge> > &network,
    const float *weights) {
  for (std::size_t iii = 0; iii < network.size(); iii++) {
    for (std::size_t jjj = 0; jjj < network[iii].size(); jjj++) {
      network[iii][jjj].weight = weights[iii * network[iii].size() + jjj];
    }
  }
}

void FillAll2AllNetwork(std::vector< std::vector<InEdge> > &network,
    const float *weights) {
  for (std::size_t iii = 0; iii < network.size(); iii++) {
    for (std::size_t jjj = 0; jjj < network[iii].size(); jjj++) {
      network[iii][jjj].weight = weights[iii * network[iii].size() + jjj];
    }
  }
}

}
