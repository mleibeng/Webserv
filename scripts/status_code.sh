#!/bin/bash

# Function to create HTML content for a status page
generate_html() {
    local code=$1
    local title=$2
    local description=$3
    local icon=$4

    cat << EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>${code} - ${title}</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: system-ui, -apple-system, sans-serif;
            background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 2rem;
        }

        .error-container {
            background: white;
            padding: 2rem;
            border-radius: 12px;
            box-shadow: 0 8px 24px rgba(0,0,0,0.1);
            text-align: center;
            max-width: 500px;
            width: 100%;
        }

        .status-code {
            font-size: 4rem;
            font-weight: 700;
            color: #2d3748;
            margin-bottom: 0.5rem;
        }

        .status-text {
            font-size: 1.5rem;
            color: #4a5568;
            margin-bottom: 1.5rem;
        }

        .description {
            color: #718096;
            line-height: 1.6;
            margin-bottom: 2rem;
        }

        .back-button {
            display: inline-block;
            padding: 0.75rem 1.5rem;
            background: #4299e1;
            color: white;
            text-decoration: none;
            border-radius: 6px;
            transition: background 0.2s;
        }

        .back-button:hover {
            background: #3182ce;
        }

        .icon {
            font-size: 3rem;
            margin-bottom: 1rem;
        }
    </style>
</head>
<body>
    <div class="error-container">
        <div class="icon">${icon}</div>
        <div class="status-code">${code}</div>
        <div class="status-text">${title}</div>
        <p class="description">
            ${description}
        </p>
        <a href="/" class="back-button">Back to Home</a>
    </div>
</body>
</html>
EOF
}

# Create directory for status pages if it doesn't exist
mkdir -p status_pages

# Array of status codes and their details
# Format: "code|title|description|icon"
status_pages=(
    "200|Success|The request has succeeded. The resource has been successfully created, fetched, or modified according to the request's method.|✓|false|0"
    "201|Created|The request succeeded and a new resource was created as a result.|✓|false|0"
    "202|Accepted|The request has been accepted for processing, but the processing has not been completed.|✓|false|0"
    "204|No Content|The request succeeded but there is no content to send in the response.|✓|false|0"

    # 3xx Redirects
    "300|Multiple Choices|Multiple options exist for this resource. Please select one of the available options.|↻|true|5"
    "301|Moved Permanently|This resource has been permanently moved to a new location. Please update your bookmarks.|↻|true|3"
    "302|Found|This resource is temporarily located at a different URL. You will be redirected shortly.|↻|true|3"
    "303|See Other|The response to this request can be found at a different URL. Redirecting you there.|↻|true|3"
    "307|Temporary Redirect|This resource is temporarily located at a different URL. You will be redirected shortly.|↻|true|3"
    "308|Permanent Redirect|This resource has been permanently moved to a new location. Please update your bookmarks.|↻|true|3"

    # 4xx Client Errors
    "400|Bad Request|The server cannot process the request due to client error. Please verify your request and try again.|⚠|false|0"
    "401|Unauthorized|Authentication is required to access this resource. Please log in and try again.|⚠|false|0"
    "403|Forbidden|You don't have permission to access this resource. Please contact the administrator if you believe this is an error.|⚠|false|0"
    "404|Not Found|The requested resource could not be found on the server. Please check the URL and try again.|⚠|false|0"
    "405|Method Not Allowed|The requested method is not allowed for this resource. Please check the documentation for allowed methods.|⚠|false|0"
    "408|Request Timeout|The server timed out waiting for the request. Please try again.|⚠|false|0"
    "429|Too Many Requests|You have sent too many requests in a given amount of time. Please try again later.|⚠|false|0"

    # 5xx Server Errors
    "500|Internal Server Error|An unexpected error occurred on the server. Please try again later or contact support if the problem persists.|✕|false|0"
    "501|Not Implemented|The server does not support the functionality required to fulfill this request.|✕|false|0"
    "502|Bad Gateway|The server received an invalid response from the upstream server.|✕|false|0"
    "503|Service Unavailable|The server is temporarily unavailable due to maintenance or high load. Please try again later.|✕|false|0"
    "504|Gateway Timeout|The server did not receive a timely response from the upstream server.|✕|false|0"
	"508|Redirection Loop|The server is stuck in an endless redirection loop.|x|false|0"
)

# Generate status pages
for page in "${status_pages[@]}"; do
    IFS='|' read -r code title description icon <<< "$page"
    echo "Generating ${code}.html..."
    generate_html "$code" "$title" "$description" "$icon" > "status_pages/${code}.html"
done

echo "All status pages have been generated in the 'status_pages' directory!"