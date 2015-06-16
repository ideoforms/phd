# Evolution and Learning in Heterogeneous Environments: Simulation Code

## Summary

C++ simulation code used in Jones (2015), *Evolution and Learning in Heterogeneous Environments*.

This simulation models an evolving ecosystem of individuals, each of which can engage in multiple types of information transmission: genetic evolution, individual learning, and social learning.

## Requirements

This code was developed on Mac OS X 10.7 - 10.10, and has been tested successfully with `gcc 4.2` and `llvm 6.1.0`, with `GNU Make 3.81`.

Compiling this simulation has the following prerequisites:

* **gengetopt**: to handle configuration files and command-line config options
* **boost**: for bitset vectors
* **gsl**: for pseudo-random number operations

To build and run the GUI elements, Xcode and Cinder (`http://libcinder.org/`) are required (tested with Xcode 6.3.1 and Cinder 0.8.6).

## Compilation

To build the command-line simulation on OS X, as long as the above prerequisites are installed, simply run:

	make

To build the GUI simulation, open the .xcodeproj file in Xcode. You will need to set CINDER_PATH to point to your Cinder distribution.

## Configuration

Basic configuration options can be found in `settings.conf`. For a full index of extended configuration options, see `sim/config/settings.ggo`. 

For usage details (with extended command-line flags mirroring the configuration files above), run

	./app --help

