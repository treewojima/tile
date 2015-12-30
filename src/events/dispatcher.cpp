#include "defines.hpp"
#include "events/dispatcher.hpp"

Events::Dispatcher::TypeSubscriberMap Events::Dispatcher::_map;
Events::Dispatcher::TypeAsyncSubscriberMap Events::Dispatcher::_asyncMap;
