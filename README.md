# SY5 - PROJET SYSTEMES

A Simulation of a printers System.

## Compilation

    make

## To Execute

To start running the printers simulators you need to execute the following command, which will read inside the file "fichierConfiguration" in the project directory, where each line is of the format "printer_name printer_tube\n". 

	./init_simulateurs -c fichierConfiguration

This command will execute as many printers as is the number of lines in the configuration file.
To start the Server you need to specify through which "tube" file it will wait for incoming requests and how to communicate with printers (-c option):

    ./print_demon -t tubeServeur -c fichierConfiguration

To send a simple printing request, execute the client with the "-P" option:

    ./mpr -P printer_name file_to_print client_tube

To cancel a printing requests:

    ./mpr -P printer_name file_to_print client_tube -A printing_id

To send N printing requests for the file "file_to_print":

    ./mpr -P printer_name file_to_print client_tube -N number_of_copies

To print all the files specified in file_list (a file in which each line is a path to the file to print):

    ./mpr -P printer_name file_to_print client_tube -M file_list

## Synopsis 

The project is a Simulation of a System where clients can ask to print a file through a specif printer. The central Servers keeps listening for incoming printing requests and distributes them to the right printer, which add the new file to print in a queue.  

The Client sends a printing requests to the Server and receives an ID in response, then waits for the Job to be done by the printer.
The client can still stop a file from printing by sending a request with "-A" option, if the job hasn't been treated by the printer yet.

The Server-Client and Server-Printers communciation take place through "Tubes", but it could be easily replace with network function to make this a distributed system.