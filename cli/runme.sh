#!/usr/bin/env bash

mkdir -p /vagrant/cli/build
pushd /vagrant/cli/build
cmake ..
make
time ./cli
