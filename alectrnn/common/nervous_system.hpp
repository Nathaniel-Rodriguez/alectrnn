
#ifndef NN_NERVOUS_SYSTEM_H_
#define NN_NERVOUS_SYSTEM_H_

#include <cstddef>
#include <cassert>
#include <vector>
#include <initializer_list>
#include "layer.hpp"
#include "multi_array.hpp"

namespace nervous_system {

/*
 * NervousSystem takes ownership of the layers it is given.
 */
template<typename TReal>
class NervousSystem {
  public:
    typedef std::size_t Index ;

    NervousSystem(const std::vector<Index>& input_shape) : parameter_count_(0) {
      network_layers_.push_back(new InputLayer<TReal>(input_shape));
    }

    // If NervousSystem is owner, it needs this destructor
    ~NervousSystem() {
      for (auto obj_ptr = network_layers_.begin(); obj_ptr != network_layers_.end(); ++obj_ptr) {
        delete *obj_ptr;
      }
    }

    void Step() {
      for (Index iii = 1; iii < network_layers_.size(); ++iii) {
        (*network_layers_[iii])(network_layers_[iii-1]);
      }
    }
    
    void Reset() {
      for (auto iter = network_layers_.begin(); iter != network_layers_.end(); ++iter) {
        (*iter)->Reset();
      }
    }

    void Configure(const multi_array::ConstArraySlice<TReal>& parameters) {
      assert(parameters.size() == parameter_count_);
      Index slice_start(0);
      for (auto layer_ptr = network_layers_.begin()+1; 
          layer_ptr != network_layers_.end(); ++layer_ptr) {
        (*layer_ptr)->Configure(parameters.slice(
          slice_start, (*layer_ptr)->GetParameterCount()));
        slice_start += parameters.stride() * (*layer_ptr)->GetParameterCount();
      }
    }

    /*
     * First layer is the input layer, its states are set here
     */
    template<typename T>
    void SetInput(const std::vector<T>& inputs) {
      for (Index iii = 0; iii < network_layers_[0]->NumNeurons(); iii++)
      {
        network_layers_[0]->SetNeuronState(iii, inputs[iii]);
      }
    }

    /*
     * It is assumed that the last layer is the output of the network
     * Its states will be returned as a Tensor.
     */
    const multi_array::Tensor<TReal>& GetOutput() const {
      return network_layers_[network_layers_.size()-1]->state();
    }

    void AddLayer(Layer<TReal>* layer) {
      network_layers_.push_back(layer);
      parameter_count_ += layer->GetParameterCount();
    }

    std::size_t GetParameterCount() const {
      return parameter_count_;
    }

    const Layer<TReal>& operator[](Index index) const {
      return *network_layers_[index];
    }

    Index size() const {
      return network_layers_.size();
    }

  protected:
    std::size_t parameter_count_;
    std::vector< Layer<TReal>* > network_layers_;
};

} // End nervous_system namespace

#endif /* NN_NERVOUS_SYSTEM_H_ */