#pragma once

// Reset to default color
#define RESET   "\033[0m"

// Text colors
#define BLACK   "\033[30m"  // Black text
#define RED     "\033[31m"  // Red text
#define GREEN   "\033[32m"  // Green text
#define YELLOW  "\033[33m"  // Yellow text
#define BLUE    "\033[34m"  // Blue text
#define MAGENTA "\033[35m"  // Magenta text
#define CYAN    "\033[36m"  // Cyan text
#define WHITE   "\033[37m"  // White text

// Bold text colors (high intensity)
#define BBLACK   "\033[1;30m"  // Bold black text
#define BRED     "\033[1;31m"  // Bold red text
#define BGREEN   "\033[1;32m"  // Bold green text
#define BYELLOW  "\033[1;33m"  // Bold yellow text
#define BBLUE    "\033[1;34m"  // Bold blue text
#define BMAGENTA "\033[1;35m"  // Bold magenta text
#define BCYAN    "\033[1;36m"  // Bold cyan text
#define BWHITE   "\033[1;37m"  // Bold white text

// Background colors
#define BG_BLACK   "\033[40m"  // Black background
#define BG_RED     "\033[41m"  // Red background
#define BG_GREEN   "\033[42m"  // Green background
#define BG_YELLOW  "\033[43m"  // Yellow background
#define BG_BLUE    "\033[44m"  // Blue background
#define BG_MAGENTA "\033[45m"  // Magenta background
#define BG_CYAN    "\033[46m"  // Cyan background
#define BG_WHITE   "\033[47m"  // White background

// Bold background colors (high intensity)
#define BG_BBLACK   "\033[100m"  // Bright black background
#define BG_BRED     "\033[101m"  // Bright red background
#define BG_BGREEN   "\033[102m"  // Bright green background
#define BG_BYELLOW  "\033[103m"  // Bright yellow background
#define BG_BBLUE    "\033[104m"  // Bright blue background
#define BG_BMAGENTA "\033[105m"  // Bright magenta background
#define BG_BCYAN    "\033[106m"  // Bright cyan background
#define BG_BWHITE   "\033[107m"  // Bright white background

// Text styles
#define BOLD      "\033[1m"  // Bold text
#define DIM       "\033[2m"  // Dim text
#define UNDERLINE "\033[4m"  // Underlined text
#define BLINK     "\033[5m"  // Blinking text
#define REVERSE   "\033[7m"  // Reverse colors
#define HIDDEN    "\033[8m"  // Hidden text