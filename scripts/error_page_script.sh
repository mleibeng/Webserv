#!/bin/bash

# Define the common error status codes
error_codes=(400 401 403 404 405 408 409 410 411 412 413 414 415 416 417 421 422 423 424 425 426 428 429 431 451 500 501 502 503 504 505 506 507 508 510 511)

# Define the directory where the error pages will be created
error_dir="error"

# Check if the error directory exists
if [ ! -d "$error_dir" ]; then
    echo "Error directory does not exist. Please create the 'error' directory first."
    exit 1
fi

# Loop through the error codes and create blank HTML files
for code in "${error_codes[@]}"
do
    # Create a blank HTML file in the error directory
    echo "<!DOCTYPE html><html><head><title>Error $code</title></head><body><h1>Error $code</h1></body></html>" > "$error_dir/$code.html"
done

echo "Error page files created successfully in the 'error' directory."
