#!/bin/bash
cron
php-fpm &
/usr/local/openresty/nginx/sbin/nginx -g "daemon off;"