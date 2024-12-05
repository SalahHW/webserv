#!/usr/bin/env python3

import os
import sys

# Print the HTTP response headers
print("Content-Type: text/html")
print()

# Parse the environment variables (set by the web server)
method = os.environ.get("REQUEST_METHOD", "GET")
query_string = os.environ.get("QUERY_STRING", "")
content_length = int(os.environ.get("CONTENT_LENGTH", "0") or 0)

# Read POST data if it's a POST request
post_data = ""
if method == "POST" and content_length > 0:
    post_data = sys.stdin.read(content_length)

# Generate a response
print("<html>")
print("<head><title>CGI Script</title></head>")
print("<body>")
print("<h1>CGI Script Response</h1>")
print("<p><strong>Request Method:</strong> {}</p>".format(method))
print("<p><strong>Query String:</strong> {}</p>".format(query_string))
if post_data:
    print("<p><strong>POST Data:</strong> {}</p>".format(post_data))
print("</body>")
print("</html>")
