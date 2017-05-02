#!/usr/bin/env bash

exec 2>&1 

mkdir -p /vagrant/cli/build
pushd /vagrant/cli/build
if [ ${HOSTNAME} == "client1" ]; then
  cmake ..
  make clean
  make
fi
time ./cli
