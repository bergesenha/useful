# useful
Amalgamation of previous personal libraries

* array_view:
A mutable view to any contiguous region of memory providing a Container like interface.

* circular_iterator:
An iterator adaptor for any RandomAccessIterator to any container type that enables you to treat it like a ring-buffer.

* <function_deduction.hpp>:
Several metafunctions deducing return type, parameter types, type of object that owns the member function etc... of free functions and member functions.

* <function_type_descriptor.hpp>:
Several metafunctions for storing, decomposing and composing function types and function pointers at compile time.

* handle_map:
A cache friendly 'map' type with contiguous underlying storage. A handle is returned at insertion of an element that can be used to retrieve the element. The 'key' cannot be chosen. handle_map::erase utilizes 'swap and pop' and handle_map::insert always inserts at end of contiguous storage.

* member_iterator:
An iterator adaptor for retrieving the members of structs or classes. member_iterator can wrap any iterator of any Iterator Category and makes it possible to treat a container of structs/classes as a container of one of the structs'/classes' members. member_iterator will inherit the capabilities of the underlying iterator.

* <member_variable_deduction.hpp>:
Metafunctions for deducing the types of member variables and types of the object holding the member variables.

* <sfinae.hpp>:
Metafunctions deducing type capabilities at compile time. specialization_defined_v determines wether a type has a certain specialization defined. integral_specialization_defined_v determines wether a particular specialization of a type with a non-type template parameter is defined.

* sliding_window:
A very specialized container originally used for the implementation of an order book in a trading system. It's storage is implemented as a circular-/ring-buffer and provides functions 'push_back' 'push_front' 
