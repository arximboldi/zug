.. image:: https://travis-ci.org/arximboldi/zug.svg?branch=master
   :target: https://travis-ci.org/arximboldi/zug
   :alt: Travis Badge

.. image:: https://codecov.io/gh/arximboldi/zug/branch/master/graph/badge.svg
   :target: https://codecov.io/gh/arximboldi/zug
   :alt: CodeCov Badge

.. image:: https://cdn.jsdelivr.net/gh/arximboldi/zug/doc/_static/sinusoidal-badge.svg
   :target: https://sinusoid.al
   :alt: Sinusoidal Engineering badge
   :align: right

.. raw:: html

  <img width="100%"
       src="https://raw.githubusercontent.com/arximboldi/zug/cc2a682d75f40a3739db091009b26902919a0b03/doc/_static/logo-front.svg"
       alt="Logotype">

.. include:introduction/start

**zug** is a C++ library providing `transducers`_. Transducers are composable
sequential transformations independent of the source. They are extremely
lightweight, and can be used to express algorithms over pull-based sequences
(iterators, files) but also push based sequences (signals, events, asynchronous
streams) in a generic way.

.. _transducers: https://clojure.org/reference/transducers

* **Documentation** (Contents_)
* **Code** (GitHub_)
* **CppCon 2015 Talk**: *Transducers: from Clojure to C++* (`YouTube
  <https://www.youtube.com/watch?v=vohGJjGxtJQ>`_, `Slides
  <https://sinusoid.es/talks/transducers-cppcon15/>`_)

.. _contents: https://sinusoid.es/zug/#contents
.. _github: https://github.com/arximboldi/zug

  .. raw:: html

     <a href="https://www.patreon.com/sinusoidal">
         <img align="right" src="https://cdn.rawgit.com/arximboldi/immer/master/doc/_static/patreon.svg">
     </a>

  This project is part of a long-term vision helping interactive and
  concurrent C++ programs become easier to write. **Help this project's
  long term sustainability by becoming a patron or buying a
  sponsorship package:** juanpe@sinusoid.al

.. include:index/end

Example
-------

.. code-block:: c++

   auto xf = zug::filter([](int x) { return x > 0; }),
           | zug::map([](int x) { return std::to_string(x); }));

Here ``xf`` is a *transducer*, a transformation over that can be fed in a
sequence of integers, resulting in a sequence of strings.  Note, however, that
this transformation makes no reference to whatever it is transforming.  In fact,
we can apply it in many ways.

Transforming a range
~~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   auto data1 = std::vector<int>{3, -2, 42, -10};
   auto data2 = zug::into(std::vector<std::string>{}, xf, data);
   assert(data2 == {"3", "42"});

As a lazy iterator
~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   auto data1 = std::vector<int>{ ... };
   auto data2 = zug::sequence(xf, data);
   std::copy(data2.begin(), data2.end(), ...);

Generators and sinks
~~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   zug::run(zug::read<int>(std::cin) | xf |
            zug::write(std::cout));

Reads integers from the terminal and outputs back the positive ones.

Transforming cursors
~~~~~~~~~~~~~~~~~~~~

The library is used in `Lager`_, a library implementing the unidirectional
data-flow architecture for C++ interactive applications. It is used to treat
reactive values as a temporal sequence that can be transformed in arbitrary
ways. For example:

.. _Lager: https://sinusoid.es/lager
.. code-block:: c++

   auto x = lager::state<int>{42};
   auto y = lager::reader<std::string>{x.xform(xf)}
   y.watch([] (auto&& v) { std::cout << v << std::endl; });
   x.set(10); // outputs: 10
   x.set(-2); // no output


Why?
----

You have learn Sean Parent's lesson: `No Raw Loops
<https://www.youtube.com/watch?v=W2tWOdzgXHA>`_. Instead of iterating over
sequences directly, you use STL algoriths like `transform`_, `filter`_, etc, or
even better, the new `ranges`_ library.

However, what if you have a *sequence* that can not be easily or efficiently
expressed as an iterator?  Then, you may have to reimplement all these
algorithms again, on top of whatever sequence abstraction you have invented, for
example, see `RxCpp`_...  Or you use transducers.

Transducers these generic algorithms transformations, in a way that is
completely agnostic of the actual sequence that is being transformed.  As a
library author, you can add transducer support for your library, and
automatically get access to our wide `collection of transducers`_ and allow your
users to simply `write their own`_.

.. _ranges: https://en.cppreference.com/w/cpp/ranges
.. _transform: https://en.cppreference.com/w/cpp/algorithm/transform
.. _filter: https://en.cppreference.com/w/cpp/algorithm/filter
.. _RxCpp: https://github.com/ReactiveX/RxCpp
.. _collection of transducers: https://sinusoid.es/zug/transducer.html
.. _write their own: https://sinusoid.es/zug/transducer.html

Dependencies
------------

This library is written in **C++14** and a compliant compiler is
necessary.  It is `continuously tested`_ with Clang 3.8 and GCC 6, but
it might work with other compilers and versions.

No external library is necessary and there are no other requirements.

.. _continuously tested: https://travis-ci.org/arximboldi/immer

Usage
-----

This is a **header only** library but to be configured correctly you need to run
`CMake`_ first::

    mkdir -p build && cd build
    cmake ..

Or you can just copy the ``zug`` subfolder somewhere in your *include path*.

Development
-----------

.. _nix package manager: https://nixos.org/nix
.. _cmake: https://cmake.org/

In order to develop the library, you will need to compile and run the
examples, tests and benchmarks.  These require some additional tools.
The easiest way to install them is by using the `Nix package
manager`_.  At the root of the repository just type::

    nix-shell

This will download all required dependencies and create an isolated
environment in which you can use these dependencies, without polluting
your system.

Then you can proceed to generate a development project using `CMake`_::

    mkdir build && cd build
    cmake ..

From then on, one may build and run all tests by doing::

    make check

License
-------

.. image:: https://upload.wikimedia.org/wikipedia/commons/c/cd/Boost.png
   :alt: Boost logo
   :target: http://boost.org/LICENSE_1_0.txt
   :align: left

**This software is licensed under the Boost Software License 1.0**.

The full text of the license is can be accessed `via this link
<http://boost.org/LICENSE_1_0.txt>`_ and is also included
in the ``LICENSE`` file of this software package.
