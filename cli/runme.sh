#!/usr/bin/env bash

exec 2>&1 

mkdir -p /vagrant/cli/build
pushd /vagrant/cli/build
cmake ..
make
time ./cli
