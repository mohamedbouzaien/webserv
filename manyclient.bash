#!/bin/bash

for N in {1..10}
do
	ruby client.rb 3 & 
done
