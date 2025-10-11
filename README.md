# Logging System

Debug Logging system on top of some example code at https://github.com/gdevine-ucsc/imgui-starter.git

os system: Mac

Design:

- a pop up window that can be imported via a function
- three logging level, if Info, Warning, and Error
- A logging function that takes in the logging level, and a message that can optionally contain arguments, like a C style text
- Each log entry is stored in a vector, with a specified struct that contains the time being logged, level type, and the message
- can custom changed the file the logs are saved to

Questions I asked the teacher prior to starting:

- How would the logging system be generally called, and to what degree of sophesication does the log level have to display?
- would the logging system handle system crashes automatically?


after getting answers from the above question, I decided to make a logging system that focus on simplicity and the essence of logging: displaying, storing, and separting logs.

