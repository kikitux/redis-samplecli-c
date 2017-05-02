#!/usr/bin/env bash

echo "running server provisioning"

which redis-server 2>/dev/null || {
  export DEBIAN_FRONTEND=noninteractive
  apt-get update
  apt-get install -y redis-server
  sed -i 's/bind.*/bind 0.0.0.0/' /etc/redis/redis.conf
  update-rc.d redis-server defaults
}
/etc/init.d/redis-server status 2>/dev/null && /etc/init.d/redis-server force-reload 2>/dev/null || /etc/init.d/redis-server start 2>/dev/null
