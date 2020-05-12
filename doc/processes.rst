.. _process:

Processes
=========

A transducer in itself describes a transformation between abstract sequences,
but it is completely agnostic of the physical or temporal nature of that
sequence.  We call a *process* the mechanism that actually feeds data in and out
out a transducer.

In this section, we describe several *processes* provided by the library.  A few
of them are processes for processing C++ iterator based sequences and ranges.
Some, like `reductor`, may help implementing your own processes. For an example
of a third-party library that implements its own transducible components, check
the cursors module in Lager_.

.. _Lager: https://sinusoid.es/lager

reduce
------
.. doxygenfunction:: reduce

reduce_nested
-------------
.. doxygenfunction:: reduce_nested

transduce
---------
.. doxygenfunction:: transduce

run
---
.. doxygenfunction:: run

into
----
.. doxygenfunction:: into
.. doxygenfunction:: into_vector

sequence
--------
.. doxygengroup:: sequence
   :content-only:

reductor
--------

Reductors encapsulate a *reducing function* and its *reduction state* in a more
object oriented interface.  It can be useful when implementing your own
processes that supports transducers.

----

.. doxygengroup:: reductor
   :content-only:

.. doxygenstruct:: zug::reductor_base
   :members:
