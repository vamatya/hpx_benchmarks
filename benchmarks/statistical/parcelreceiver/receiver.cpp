#include <hpx/runtime/components/component_factory.hpp>

#include <hpx/util/portable_binary_iarchive.hpp>
#include <hpx/util/portable_binary_oarchive.hpp>

#include <boost/serialization/version.hpp>
#include <boost/serialization/export.hpp>
#include "receiver.hpp"

HPX_REGISTER_COMPONENT_MODULE();
HPX_REGISTER_MINIMAL_COMPONENT_FACTORY_EX(
    hpx::components::simple_component<
        hpx::components::server::parcelreceiver>, parcelreceiver,
        hpx::components::factory_disabled);

HPX_DEFINE_GET_COMPONENT_TYPE(hpx::components::server::parcelreceiver)
