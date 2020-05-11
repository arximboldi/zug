
Type erasure
============

Each transducer has its own distinct type.  Therefore, if we had code like this:

.. code-block:: c++

   [???] xf;
   if (...) {
       xf = filter([] (int x) { return x % 2; });
   } else {
       xf = map([] (int x) { return x * 2; });
   }

What should the type for the transducer ``xf`` be?

These invocations of ``filter`` and ``map`` return objects of different types.
This is part of what makes the library so efficient---all the information about
the processing chain is assembled at compile time.  However, in this case, we do
not have a suitable type for ``xf``.

The solution is called `type erasure`_, providing a type that removes some of
the details, leaving the part that we are interested in.  For example,
``std::function<Signature>`` allows us to take any function or callable object,
regardless of its type, as long as they have a compatible signature.  Similarly,
this library provides a type ``transducer<Inputs [,Outputs]>`` that can hold any
transducer that can take and produce inputs and outputs of the given types.

.. _type erasure: https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure

----

.. doxygenclass:: zug::transducer
