#!/bin/bash

SERVER_URL="http://localhost:8080"
SERVER_PROCESS="./webserver test_config.conf"

# Start the server
$SERVER_PROCESS &
SERVER_PID=$!

# Wait for the server to start
sleep 2

# Test basic GET request
echo "Testing basic GET request..."
curl -s -o /dev/null -w "%{http_code}" $SERVER_URL
echo

# Test non-existent page
echo "Testing non-existent page..."
curl -s -o /dev/null -w "%{http_code}" $SERVER_URL/notfound
echo

# Test multiple connections
echo "Testing multiple connections..."
ab -n 100 -c 10 $SERVER_URL/

# Add more tests here (e.g., for different routes, methods, CGI, file uploads)

# Stop the server
kill $SERVER_PID

echo "Tests completed."