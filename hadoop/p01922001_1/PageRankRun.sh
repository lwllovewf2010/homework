#!/bin/bash

hadoop fs -rmr /small.graph.out
hadoop fs -rmr /big.graph.out
hadoop fs -put small.graph /small.graph
hadoop fs -put big.graph /big.graph

make test4
#make test5

hadoop fs -ls /
hadoop fs -cat /small.graph.out/*
hadoop fs -cat /big.graph.out/*

