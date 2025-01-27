import os, cgi, cgitb

#def main():
#    # Retrieve the REQUEST_METHOD environment variable
#    request_method = os.environ.get("REQUEST_METHOD", "")
#
#    # Output HTTP headers
#    print("Content-Type: text/html")
#    print("")  # Empty line to indicate end of headers
#
#    # Output HTML content
#    print("<html>")
#    print("<head><title>CGI Script Output</title></head>")
#    print("<body>")
#    print("<h1>Python CGI Script</h1>")
#    print("<p>REQUEST_METHOD: {request_method}</p>")
#    print("</body>")
#    print("</html>")
#
#if __name__ == "__main__":
#    main()


def main():
# Parse the form data
    form = cgi.FieldStorage()
    # Get the filename and content from the form
    filename = form.getvalue("filename")
    content = form.getvalue("content")
    
    # Set the content-type header
    print("Content-Type: text/html\n")
    
    # Validate inputs
    if not filename or not content:
        print("<h1>Error</h1>")
        print("<p>Both file name and content are required.</p>")
    else:
        try:
            # Save the file
            with open(f"/path/to/save/{filename}", "w") as file:
                file.write(content)
            print("<h1>File Saved</h1>")
            print(f"<p>File <strong>{filename}</strong> has been saved successfully.</p>")
        except Exception as e:
            print("<h1>Error</h1>")
            print(f"<p>Could not save the file: {e}</p>")
