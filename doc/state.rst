.. _state:

State
=====

In the :ref:`design` section we say how a transducer may extend the state from
the *reducing function*.  Not only does this allow it to include feedback in the
process, but also to signal to process whether the computation has terminated.

This section describes the abstract interface that comunicates between the state
and the reducing process, and also the concrete state wrapping utilities
provided for transducer implementors.

state_traits
------------

.. doxygengroup:: state_traits
   :content-only:

state_wrapper
-------------

.. doxygengroup:: state_wrapper
   :content-only:

with_state
----------

.. doxygenfunction:: with_state

maybe_reduced
-------------

.. doxygengroup:: maybe_reduced
   :content-only:

skip
----

.. doxygengroup:: skip
   :content-only:

any_state
---------

.. doxygenclass:: zug::any_state
