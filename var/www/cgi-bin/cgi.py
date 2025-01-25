import os, cgi, cgitb

def main():
    # Retrieve the REQUEST_METHOD environment variable
    request_method = os.environ.get("REQUEST_METHOD", "")

    # Output HTTP headers
    print("Content-Type: text/html")
    print("")  # Empty line to indicate end of headers

    # Output HTML content
    print("<html>")
    print("<head><title>CGI Script Output</title></head>")
    print("<body>")
    print("<h1>Python CGI Script</h1>")
    print("<p>REQUEST_METHOD: {request_method}</p>")
    print("</body>")
    print("</html>")

if __name__ == "__main__":
    main()