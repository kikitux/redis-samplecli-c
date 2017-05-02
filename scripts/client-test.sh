#!/usr/bin/env bash
set -e

echo "running client test"
SERVER=192.168.56.11
redis-cli -h ${SERVER} -p 6379 ping

if [ ${HOSTNAME} == "client1" ]; then
  redis-cli -h ${SERVER} -p 6379 set mykey somevalue
fi

VALUE=`redis-cli -h ${SERVER} -p 6379 get mykey`

if [ "${VALUE}" ]; then
  echo "we got the value ${VALUE}, all good"
else
  echo "warn: not able to get the value, something not good"
  exit 1
fi
