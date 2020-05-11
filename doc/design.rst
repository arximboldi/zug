.. _design:

Design
======

This section elaborates on what a *transducer* is.  It will help you understand
how to use them, but also how to write your own transducers and processes.

What is a transducer?
---------------------

.. image:: https://public.sinusoid.es/talks/transducers-cppcon15/pic/rich2.jpg
   :align: right
   :width: 40%
..

  .. centered:: "Transducers extract the essence of *map* and *filter*"

  |  Rich Hickey, author of Clojure, *on Transducers*
     `(video) <https://www.youtube.com/watch?v=6mTbuzafcII>`_
     `(blog) <https://rakhim.org/2020/01/summary-of-transducers-a-talk-by-rich-hickey/>`_

.. _accumulate: https://en.cppreference.com/w/cpp/algorithm/accumulate
.. _reduce: https://clojuredocs.org/clojure.core/reduce
.. _fold: https://wiki.haskell.org/Fold

Transducers come after the realization that the most general algorithm over
sequences is what in C++ we call accumulate_, but in other languages is often
called reduce_ or fold_.  Accumulate applies a *reducing function* iteratively
over a sequence.  The reducing function takes a *state* and some *input* as
arguments, and returns an updated state.

For example, we can define a *reducing function* ``output`` that takes an output
iterator as *state*, some value as *input*, and when invoked, outputs the value
using the iterator and returns the iterator into the next position:

.. code-block:: c++

   auto output = [](auto state, auto input) {
       *state = input;
       return ++state;
   };

Using this function, we can implement ``std::copy`` in terms of ``accumulate``:

.. code-block:: c++

   template <typename InputIt, typename OutputIt>
   OutputIt copy(InputIt first, InputIt last, InputIt out)
   {
       return accumulate(first, last, out, output);
   }

We can also use this structure to implement ``transform`` and ``filter``:

.. code-block:: c++

   template <typename InputIt, typename OutputIt, typename Mapping>
   OutputIt transform(InputIt first, InputIt last, InputIt out,
                      Mapping mapping)
   {
       return accumulate(first, last, out, [&](auto state, auto input) {
           return output(state, mapping(input));
       });
   }

   template <typename InputIt, typename OutputIt, typename Predicate>
   OutputIt filter(InputIt first, InputIt last, InputIt out,
                   Predicate pred)
   {
       return accumulate(first, last, out, [&](auto state, auto input) {
           return pred(input)
               ? output(state, input)
               : state;
       });
   }

Notice something?  The functions look almost identical.  There is a part that is
almost identical: calls to ``accumulate`` and ``output`` that specify the input
and output parts of the :ref:`process`.  The meat of the algorithm is
sandwitched in between.

We can extract their by writing them as functions take a *reducing function*
(like output) as an argument, and as result return another *reducing function*
that, for example, maps the input betweeing passing it over the next reducing
function.

   A **transducer** is *a function* that takes a *reducing function*, and returns
   another *reducing function* that wraps it.

.. code-block:: c++

   template <typename Mapping>
   auto map(Mapping mapping)
   {
       return [=] (auto step) {
           return [=] (auto s, auto... ins) {
               return step(s, mapping(ins...));
           };
       };
   }

   template <typename Predicate>
   auto filter(Predicate pred)
   {
       return [=] (auto step) {
           return [=] (auto s, auto... ins) {
               return pred(input) ? step(s, ins...) : s;
           };
       };
   }

.. warning:: This code could be improved by leveraging **move semantics** and
   **perfect forwarding** wherever we pass stuff to the underlying transducer.
   This, however, adds too much noise to the code examples.  These concepts are
   already hard enough to understand to those uninitated in functional
   programming.

   As an exercise can think about you would use ``std::move`` and
   ``std::forward`` in these examples.  You can check the implementation of
   these transducers in the library itself for some inspiration.

These two functions return a *transducer*, this a function that takes a
*reducing function* called ``step`` as an argument, and return another reducing
function.  We can revise our implementation of our standard algorithms using
them:

.. code-block:: c++

   template <typename InputIt, typename OutputIt, typename Mapping>
   OutputIt transform(InputIt first, InputIt last, InputIt out,
                      Mapping mapping)
   {
       return accumulate(first, last, out, map(mapping)(output));
   }

   template <typename InputIt, typename OutputIt, typename Predicate>
   OutputIt filter(InputIt first, InputIt last, InputIt out,
                   Predicate pred)
   {
       return accumulate(first, last, out, filter(pred)(output));
   }

What if you wanted to write a function that filters and transforms all in one
go?

.. code-block:: c++

   template <typename InputIt, typename OutputIt,
             typename Predicate, typename Mapping>
   OutputIt filter_and_transform(
                 InputIt first, InputIt last, InputIt out,
                 Predicate pred, Mapping mapping)
   {
       return accumulate(first, last, out,
                         filter(pred)(map(mapping)(output)));
   }

Or using piping for function composition and extracting out the transducer:

.. code-block:: c++

   template <typename InputIt, typename OutputIt,
             typename Predicate, typename Mapping>
   OutputIt filter_and_transform(
                 InputIt first, InputIt last, InputIt out,
                 Predicate pred, Mapping mapping)
   {
       auto xf = filter(pred) | mapping(mapping);
       return accumulate(first, last, out, xf(output)));
   }

.. admonition:: Tip
   :class: tip

   The library provides a function ``comp`` for function composition, but also
   ``operator|`` for more natural reading syntax.  You can also turn any
   function ``f`` into a pipeable one by using ``comp(f)``.

.. admonition:: Order of composition

   The ``filter_and_transform`` function above performs filtering and mapping
   precisely in that order: it filters first, and only if the value passes the
   predicate is it mapped at all.  This, *composition of transducers reads from
   left to right*.

   This may be unintuitive if that this is simply function composition, and
   function composition reads from right to left.  However, remember that a
   transducer, as a function, is not a transformation over a sequence, but a
   transformation over an operation.  Each transducer is, indeed, called from
   right to left.  Each transducer transforms the operation, wrapping it into a
   more complex operation. It is like an onion, each transducer adds a new layer
   to the onion.  The result is a more complex operation.  When data is fed into
   the operation, it will enter through the external layer of the onion, this is
   through the layer that the last transducer (the leftmost!), and penetrate the
   onion in this reverse order until it reaches the core of the onion---to the
   rightmost transducer, and then the original *reducing function*.

   .. image:: _static/composition-flat.svg
      :width: 100%

Dealing with state
------------------

Now you know how to write a simple transducer.

But what if you want to do something more advanced?  For example, you may want
to write a transducer like Python's enumerate_, that given a sequence of values,
pairs each element with an index.  You may be tempted to do it like this:

.. code-block:: c++

   auto enumerate = [=] (auto step)
   {
       return [=, idx = 0] (auto s, auto... ins) mutable
       {
           return step(s, idx++, ins...);
       };
   };

This works, but it has one problem: it hides a mutable variable in the reducer.
It needs to be marked ``mutable`` and it is no longer a `pure function`_. We
have to be careful about which instance are we using when.

But we already have a functional, explicit, *state*, the variable `s`.  The
problem is that we can not make any assumptions about its type or manipulate it
in any way --- only somewhere down in the next reducing function ``step`` can it
be understood.

.. _pure function: https://en.wikipedia.org/wiki/Pure_function
.. _enumerate: https://book.pythontips.com/en/latest/enumerate.html

The solution is to instead *wrap* the incoming state, adding any additional data
you need next to it. There is some nuance to it: the first time the transducer
is invoked there state you receive is unwrapped, but the second time you'll
receive a wrapped state.  The library provides some methods to deal with this:

- ``state_wrap(s, d)``, returns a new wrapped state containing both the state
  ``s`` and the additional data ``d``.

- ``state_unwrap(s)``, returns the underlying state from the potentially wrapped
  state ``s``.

- ``state_data(s, [] { return ...; })``, returns the wrapped data in the state
  ``s``.  If none exists, it, produces an initial value using the supplied
  callback.

Using this tool we can now improve our implementation of ``enumerate``:

.. code-block:: c++

   auto enumerate = [=] (auto step)
   {
       return [=] (auto s, auto... ins) mutable {
           auto idx  = state_data(s, [] { return 0; });
           auto next = step(s, idx, ins...);
           return wrap_state(next, ++idx);
       };
   };

.. note:: In Clojure, stateful transducers hide state within the transducer the
          way we showed in the initial example.  The reason, I believe, that
          they do not follow our approach, is that since Clojure is a dynamic
          language, this would add too much of a runtime overhead, as one would
          need to constantly check whether we are dealing with wrapped or
          unwrapped data.  In C++ all this happens at compile time, with no
          runtime overhead.  Ironically C++ happens to be *more* functional than
          Clojure in their transducers implementation 😉

Early termination
-----------------

Some transducers stop producing any output after certain conditions.  One
example is ``take`` which it lets at most ``n`` elements of the sequence in.
Howe can we signal to the reducing process, and from within the reducing
function, that there is no more work to be done?

One option would be to throw an exception, that may be catched by the reducing
process.  But exceptions are an inefficient way of control flow in C++.
Instead, we may realize that termination, whether we are done or not, is a
property of the state.  Think about it: the transducer ``take`` is gonna anotate
the state with some kind of *count*.  The transducer is done when this count
reaches zero.  We say that a state that represents a finished computation is
*reduced*.  We can express this in the library like this:

.. code-block:: c++

   struct take_tag {};

   bool state_wrapper_data_is_reduced(take_tag, int n)
   {
       return n <= 0;
   }

   auto take(int n)
   {
       return comp([=](auto step) {
           return [=](auto s, auto... is) {
               return wrap_state<take_tag>(
                   step(state_unwrap(s), is...),
                   state_data(s, [=] { return n; }) - 1);
           };
       });
   }

Note how we added a ``take_tag`` to identify our state, and how we introduced
the ``state_wrapper_data_is_reduced`` to specify whether the data associated to
a particular state indicates termination.

.. admonition:: Is ``accumulate`` a valid reducing process?

   You may have noted that we are increasingly adding stuff to how the reducing
   *process* interacts with the *reducing function*.  For once, the type of the
   state changes after the first iteration.  Also, we now have to consider
   whether the state is reduced (the library provides ``state_is_reduced`` for
   that) and stop the process in that case.

   For this reason: ``std::accumulate`` may not work, in general, with the
   reducing functions produced by our transducers.  The library provides it's
   own function, ``reduce()`` and the convenience ``transduce()`` that work
   like *accumulate* but considering all these state management quirks.

Conditional invocation
----------------------

In the presence of state, we need to reconsider transducers that conditionally
invoke the next reducing function, like ``filter``.  The problem is: if the next
reducing function wraps the state with additional state, we may need to return
something of a different type depending on whether we call the reducing function
(we get a wrapped state) or don't (we may or may not have a wrapped state at
hand already).

The solution is to return something like a ``variant`` of a wrapped or an
unwrapped state.  To ease this, the library provides the functions ``call`` and
``skip``, that take the same arguments (an invocation of the next reducing
function) and return the same type, but the latter does not actually call the
reducing function.  This may be clearer considering this improved implementation
of ``filter``:

.. code-block:: c++

   template <typename Predicate>
   auto filter(Predicate pred)
   {
       return [=] (auto step) {
           return [=] (auto s, auto... ins) {
               return pred(input)
                   ? call(step, s, ins...)
                   : skip(step, s, ins...);
           };
       };
   };

Inspecting the state
--------------------

We have seen how we can use ``state_data`` and ``state_unwrap`` and
``state_wrap`` to add our oun state to the computation without having special
control flow for the first invocation of the reducing function.

However, sometimes we may want to indeed do something different when we receive
an unwrapped state, signaling that we have not yet done any work.  One example
is ``dedupe`` which removes duplicate consecutive elements from the sequence.
On the first invocation, we know that the element is no duplicate, because there
was none before, so we can call the next reducing function unconditionally.

The library provides a ``with_state(s, fn1, fn2)`` that takes a state ``s`` and
calls ``fn1`` or ``fn2`` depending on whether ``s`` is already wrapped or not.
We can use it to implemente ``dedupe`` as follows:

.. code-block:: c++

    auto dedupe = [](auto step)
    {
        return [=](auto s, auto... is) {
            return with_state(s,
                [&](auto s) {
                    auto next = std::make_tuple(is...);
                    return wrap_state(step(state_unwrap(s), is...),
                                      next);
                },
                [&](auto s) {
                    auto next = std::make_tuple(is...);
                    return next == state_wrapper_data(st)
                         ? s
                         : wrap_state(step(state_unwrap(st), is...),
                                      next);
                });
        };
    };


Variadics
---------

Note how in the *reducing* functions above we are always receiving and passing
the inputs using ``ins...``.  This is because *transducers are variadic*, this
is, an input maybe consist of none, one, or many elements. This allows for some
interesting use-cases.

Generators
~~~~~~~~~~

A transducer with no arguments can act as generator, that with every "pulse" it
receives no input, but produces a value.  For example, we can use a transducer
to create a vector with 10 pseudo-random numbers like this:

.. code-block:: c++

    auto v = into(std::vector<int>{}, zug::map(&std::rand) | zug::take(10));
    assert(v.size() == 10);

Note that if we did not use ``take(10)`` there, that function would never end.
However, we may alternatively define an infinite lazy range of pseudo-random
numbers using:

.. code-block:: c++

    auto s = zug::sequence(zug::map(&std::rand));
    std::copy(s.begin(), s.end() + 10, ...);

Zipping
~~~~~~~

Alternatively, we can use transducers to combine multiple sequences into one,
for example, by pairing every two elements.  When using ranges this can be
slightly cumbersome, since every range must have one and only one value type, so
the every element must be combined into a tuple.  With transducers, the two
elements can be passed to the next transducer to combine them however you want.
For example, if we have two vectors of integers, we can produce a vector with
the sum of every two elements like this:

.. code-block:: c++

   auto v1 = std::vector<int>{ 1,  2,  3,  4};
   auto v2 = std::vector<int>{10, 20, 30, 40};
   auto xf = zug::map(std::plus<>{});
   auto r  = zug::into(std::vector<int>{}, xf, v1, v2);
   assert(r == {11, 22, 33, 44});

Of course, if what you want is, in fact, to generate tuples, the library has you
covered:

.. code-block:: c++

   auto v1 = std::vector<int>{ 1,  2,  3,  4};
   auto v2 = std::vector<int>{10, 20, 30, 40};
   auto r  = zug::into(std::vector<std::tuple<int, int>>{},
                       zug::zip, v1, v2);
   assert(r == {{1, 10}, {2, 20}, {3, 30}, {4, 40}});

Or maybe you have tuples, and want to operate on the elements:

.. code-block:: c++

   auto v  = std::vector<std::tuple<int, int>>{
      {1, 10}, {2, 20}, {3, 30}, {4, 40}
   };
   auto xf = zug::unzip | zug::map(std::plus<>{});
   auto r  = zug::into(std::vector<int>{}, xf, v);
   assert(r == {11, 22, 33, 44});

Performance
-----------

After being amazed, or maybe horrified, by these castles of lambda functions,
you shall be wondering: *but how the heck does this perform?*

The short answer is: **very well**.

Most of the time, a combination of transducers can be as fast as a hand-rolled
loop that performs the transformation.  At every step, a transducer is just a
simple function and it is very easy to inline.

In fact, when transforming a range eagerly it is normally more efficient than
equivalent C++20 range adaptor, since there is no intermediate chaining of
polling iterators, instead, the data is passed directly through a chain of
reducing functions.  However, some things might be slightly slower, and using
``zug::sequence`` to create a lazy iterator based on a transducer can often be
slower than a range adaptor.

The comparison is kind of apple to oranges, since they are different
abstractions.  In fact, while indeed transducers can be convenient to express
transformations over containers, they can also express, with minimal overhead,
transformations over push-based temporal sequences.  This is something that is
simply impossible with the standard library.  Check `Lager`_ for an application
of this ability of transducers.

.. _Lager: https://github.com/arximboldi/lager


.. admonition:: Contribute!

   These conclussions are based on ad-hoc inspection of the generated assembly
   and benchmarking done during the development of the library.  Sadly, there is
   no sistematic benchmark suite included with the library.  Of course,
   developing one would be a good learning exercise, `contact us`_ if you would
   like to contribute one!

.. _contact us: https://github.com/arximboldi/zug
