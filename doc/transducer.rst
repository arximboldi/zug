
Transducers
===========

The main part of this library is a wide collection of transducers implemented
using the techniques described in the :ref:`design` section.  Many of them
borrow the names of their Clojure_ equivalents, but there are additional ones
inspired by `Python itertools`_ and other sequence manipulation libraries.

.. _Python itertools: https://docs.python.org/3/library/itertools.html
.. _Clojure: https://clojure.org/reference/transducers

cat
---
.. doxygenvariable:: zug::cat

chain
-----
.. doxygenfunction:: zug::chain(InputRangeTs...)
.. doxygenfunction:: zug::chainr(InputRangeTs...)
.. doxygenfunction:: zug::chainl(InputRangeTs...)

count
-----
.. doxygenfunction:: zug::count

cycle
-----
.. doxygenfunction:: zug::cycle(InputRangeTs&&...)

dedupe
------
.. doxygenvariable:: zug::dedupe

distinct
--------
.. doxygenvariable:: zug::distinct

drop
----
.. doxygenfunction:: zug::drop

drop_while
----------
.. doxygenfunction:: zug::drop_while

each
----
.. doxygenfunction:: zug::each

eager
-----
.. doxygenfunction:: zug::eager
.. doxygenvariable:: zug::sorted
.. doxygenvariable:: zug::reversed

enumerate
---------
.. doxygenvariable:: zug::enumerate
.. doxygenfunction:: zug::enumerate_from

filter
------
.. doxygenfunction:: zug::filter

interleave
----------
.. doxygenvariable:: zug::interleave

interpose
---------
.. doxygenfunction:: zug::interpose

iter
----
.. doxygenfunction:: zug::iter(InputRangeT&&, InputRangeTs&&...)

mapcat
------
.. doxygenfunction:: zug::mapcat

map
---
.. doxygenfunction:: zug::map

map_indexed
-----------
.. doxygenfunction:: zug::map_indexed

partition
---------
.. doxygenfunction:: zug::partition

partition_by
------------
.. doxygenfunction:: zug::partition_by

product
-------
.. doxygenfunction:: zug::product(InputRangeT1&&, InputRangeTs&&...)

random_sample
-------------
.. doxygenfunction:: zug::random_sample

range
-----
.. doxygenfunction:: zug::range(StopT&&)
.. doxygenfunction:: zug::range(StartT&&, StopT&&)
.. doxygenfunction:: zug::range(StartT&&, StopT&&, StepT&&)

readbuf
-------
.. doxygenfunction:: zug::readbuf(InputStreamT&)
.. doxygenfunction:: zug::readbuf(InputStreamT&, std::size_t)

read
----
.. doxygenfunction:: read

remove
------
.. doxygenfunction:: remove

repeat
------
.. doxygenfunction:: repeat(ValueT&&, ValueTs&&...)
.. doxygenfunction:: repeatn

replace
-------
.. doxygenfunction:: replace
.. doxygenfunction:: replace_all
.. doxygenfunction:: replace_all_safe

sink
----
.. doxygenfunction:: sink

scan
----
.. doxygenfunction:: scan

take
----
.. doxygenfunction:: take

take_nth
--------
.. doxygenfunction:: take_nth

take_while
----------
.. doxygenfunction:: take_while

unzip
-----
.. doxygenvariable:: unzip

writebuf
--------
.. doxygenfunction:: zug::writebuf

write
-----
.. doxygenfunction:: write(OutputStreamT&, InSeparatorT, ArgSeparatorT)

zip
---
.. doxygenvariable:: zip
