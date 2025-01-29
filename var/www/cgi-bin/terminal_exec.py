import os
import sys
import subprocess
import urllib.parse
import html

# Read POST data manually (only needed if CGI is broken)
content_length = int(os.environ.get("CONTENT_LENGTH", 0))
post_data = sys.stdin.read(content_length) if content_length > 0 else ""

# Parse the input
params = urllib.parse.parse_qs(post_data)
command = "ls" 

# Ensure command is safe
if not command:
    output = "No command entered."
elif command.lower() in ["rm", "reboot", "shutdown", "poweroff", "halt"]:
    output = "Command not allowed for security reasons."
else:
    try:
        # Execute command and capture output
        result = subprocess.run(command, shell=True, text=True, capture_output=True, timeout=5)
        output = result.stdout if result.stdout else result.stderr
    except Exception as e:
        output = f"Error executing command: {str(e)}"

# Escape output for HTML
escaped_output = html.escape(output)

# Generate response body
response_body = f"""\
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Terminal Output</title>
    <style>
        body {{
            background: #0414a7;
            color: #e0e2f4;
            font: normal 20px/1.25rem 'VT323', monospace;
        }}
        .output-container {{
            max-width: 800px;
            margin: 50px auto;
            padding: 20px;
            background: #000;
            border: 2px solid #e0e2f4;
            border-radius: 10px;
            overflow-y: auto;
        }}
    </style>
</head>
<body>
    <div class="output-container">
        <p><strong>Command:</strong> {html.escape(command)}</p>
        <pre>{escaped_output}</pre>
        <br>
        <a href="/">Back</a>
    </div>
</body>
</html>
"""

# Calculate Content-Length
content_length = len(response_body.encode('utf-8'))

# Print HTTP headers
print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print(f"Content-Length: {content_length}\n")  # Must be followed by a blank line

# Print response body
print(response_body)