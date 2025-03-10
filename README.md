# reinforcement-learning-for-per-flow-buffer-sizing
Contact: Maximilian Bachl

This repository contains the code for the upcoming paper [LFQ: Online Learning of Per-flow Queuing Policies using Deep Reinforcement Learning](https://arxiv.org/abs/2007.02735).

The code was run on *Debian 10 (buster)* on Linux kernel ```4.19.0-9-amd64``` with g++ ```8.3.0```. 

# Installation

## Downloading libtorch

* Download the [C++ version of libtorch for CPUs](https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.5.1%2Bcpu.zip). If you want to download a newer version, make sure that it is the *Pre-cxx11 ABI* version. Otherwise you'll have to change some flags in the build process. 
* Unpack it and move the contained directory ```libtorch``` to the root directory of this repository (the same directory in which there's the ```ns-allinone-3.30.1``` directory)

## Compiling

* ```cd ns-allinone-3.30.1/ns-3.30.1/```
* ```./waf clean```
* ```./waf configure```
* ```./waf build```

# Training an RL model

If the build finished successfully (this can take some time), you can train a model. 

Assuming you're in the ```ns-3.30.1``` directory, run 

    OMP_NUM_THREADS=1 LD_LIBRARY_PATH=../../libtorch/lib:$LD_LIBRARY_PATH ./waf -v --run "examples/traffic-control/rl"
    
To change parameters like the number of cpus to use for training, the tradeoff parameter alpha etc., change the variables on top of the file ```ns-allinone-3.30.1/ns-3.30.1/examples/traffic-control/rl.cc``` and recompile. 
    
# Evaluating

## Evaluating an RL model

To evaluate a set of trained weights run 

    OMP_NUM_THREADS=1 LD_LIBRARY_PATH=../../libtorch/lib:$LD_LIBRARY_PATH ./waf -v --run "examples/traffic-control/rl <path_to_weights>"
    
## Evaluating other AQM mechanisms

To evaluate, for example, FqCoDel, run

        OMP_NUM_THREADS=1 LD_LIBRARY_PATH=../../libtorch/lib:$LD_LIBRARY_PATH ./waf -v --run "examples/traffic-control/rl FqCoDelQueueDisc"
        
To run Fifo with a maximum queue size of 100, run

        OMP_NUM_THREADS=1 LD_LIBRARY_PATH=../../libtorch/lib:$LD_LIBRARY_PATH ./waf -v --run "examples/traffic-control/rl FifoQueueDisc 100"
        
# Plotting

## Plotting training runs

During training, various values are logged.

To plot them, run the following command:

    ./plot_metrics.py results/RLQueueDisc/logs/<name_of_the_file_to_be_plotted>

## Plotting evaluation results

The following commands are supposed to be run after an AQM mechanism was evaluated. 

To plot, for example, all queue traces produced by an evaluation of Fifo with queue size 1000, run the following command:

    ls -d $PWD/results/FifoQueueDisc/queueTraces/1000/*.plotme | xargs ./plot_something.py
    
To plot the behavior of an AQM mechanism and get performance metrics, run, for example, the following command:

    ./plot_df.py results/RLQueueDisc/logs/*

# Pre-trained weights

All weights that are used to generate the plots in the paper can be found in the directory ```reinforcement-learning-for-per-flow-buffer-sizing/ns-allinone-3.30.1/ns-3.30.1/results/RLQueueDisc/weights```.
