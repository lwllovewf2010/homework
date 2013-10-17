#!/bin/bash

hadoop fs -rmr /small.list.out
hadoop fs -rmr /big.list.out
hadoop fs -put small.list /small.list
hadoop fs -put big.list /big.list

make test1
make test2

hadoop fs -ls /
hadoop fs -cat /small.list.out/*
hadoop fs -cat /big.list.out/*

