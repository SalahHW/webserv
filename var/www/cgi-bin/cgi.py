#!/usr/bin/env python3

import os
import cgi
import cgitb

# Enable debugging for CGI errors
cgitb.enable()

def main():
    # Parse form data
    #form = cgi.FieldStorage()

    # Get the filename and content from the form
    #filename = form.getvalue("filename")
    #content = form.getvalue("content")
    filename = "lolololo.txt"
    content = "klklklklklklklkl"

    # Output content-type header
    print("Content-Type: text/html\n")

    # Start HTML output
    print("<html>")
    print("<head><title>File Upload</title></head>")
    print("<body>")

    # Validate inputs
    if not filename or not content:
        print("<h1>Error</h1>")
        print("<p>Both file name and content are required.</p>")
    else:
        try:
            # Save the file
            upload_dir = "/home/sickest-one/Travail/webserv/var/upload/"
            os.makedirs(upload_dir, exist_ok=True)  # Ensure the directory exists
            file_path = os.path.join(upload_dir, filename)

            with open(file_path, "w") as file:
                file.write(content)

            print("<h1>File Saved</h1>")
            print(f"<p>File <strong>{filename}</strong> has been saved successfully at <strong>{file_path}</strong>.</p>")
        except Exception as e:
            print("<h1>Error</h1>")
            print(f"<p>Could not save the file: {e}</p>")

    # End HTML output
    print("</body>")
    print("</html>")

if __name__ == "__main__":
    main()