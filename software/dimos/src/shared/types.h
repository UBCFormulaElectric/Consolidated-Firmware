#ifndef QT_DEMO_TYPES_H
#define QT_DEMO_TYPES_H

#include "variant"

template<typename Data, typename Error>
using Result = std::variant<Data, Error>;
#endif //QT_DEMO_TYPES_H
